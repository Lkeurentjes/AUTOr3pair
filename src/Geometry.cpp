// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#include "Geometry.h"
#include "Defenitions.h"
#include "DefenitionsVal3dity.h"
#include "DefenitionsCGAL.h"
#include "Geometrytools_makeCGAL.h"
#include "GeometryTools_makeCO.h"
#include "Geometrytools.h"
#include <iostream>
#include "tu3djson_convert.h"
#include "val3dity.h"
#include "../thirdparty/nlohmann-json/json.hpp"
#include "UseCaser3pair.h"


using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
    Geometry::Geometry(string name, int index, json geometry) {
      type = geometry["type"];
      id = name + "-" + to_string(index);
      tu3djson = tu3djson::make_tu3djson_geometry(geometry);
      OGtu3djson = tu3djson;

      if (geometry.contains("semantics")) { semantics = true; }
      if (geometry.contains("material")) { material = true; }
      if (geometry.contains("texture")) { texture = true; }


      if (!STANDARDS["UseCaseRepair"]["SemanticsAdd"] &&
          !STANDARDS["UseCaseRepair"]["SemanticsValidate"] &&
          !STANDARDS["UseCaseRepair"]["Triangulation"] &&
          !STANDARDS["UseCaseRepair"]["Simplification"] &&
          !STANDARDS["UseCaseRepair"]["RemeshSlivers"]) {
        UseCasevalid = true;
      }
    }

    Geometry::Geometry(string name, json Tu3djson, unordered_map<vector<int>, json, VectorHash> FaceMap) {
      type = Tu3djson["features"][0]["geometry"]["type"];
      id = name;
      tu3djson = Tu3djson;
      OGtu3djson = tu3djson;
      FacesSMT = FaceMap;

      if (Tu3djson["properties"].contains("semantics")) { semantics = true; }
      if (Tu3djson["properties"].contains("material")) { material = true; }
      if (Tu3djson["properties"].contains("texture")) { texture = true; }

      if (!STANDARDS["UseCaseRepair"]["SemanticsAdd"] &&
          !STANDARDS["UseCaseRepair"]["SemanticsValidate"] &&
          !STANDARDS["UseCaseRepair"]["Triangulation"] &&
          !STANDARDS["UseCaseRepair"]["Simplification"] &&
          !STANDARDS["UseCaseRepair"]["RemeshSlivers"]) {
        UseCasevalid = true;
      }
    }

    void Geometry::update_vertices() {
      for (auto &feature: tu3djson["features"]) { feature["geometry"]["vertices"] = VERTICES.get_verticeslist(); }
    }

    void Geometry::emptyRepairsNeeded() {
      RepairsNeeded = {};
      vector<json> empty;
      RepairsNeeded["RingErrors"] = empty;
      RepairsNeeded["PolyErrors"] = empty;
      RepairsNeeded["ShellErrors"] = empty;
      RepairsNeeded["SolidErrors"] = empty;
      RepairsNeeded["SolidIErrors"] = empty;
    }

    bool Geometry::contains(vector<int> listA, vector<int> listB) {
      return std::any_of(listA.begin(), listA.end(),
                         [&](int i) {
                             return std::any_of(listB.begin(), listB.end(), [&](int j) { return i == j; });
                         });
    }


    json Geometry::getCityjsonObject() {
      json CO = tu3djson::make_CityObject_geometry(tu3djson);
      return CO;
    }

    bool Geometry::getSplitfeatures(vector<json> &features) {
      if (tu3djson["features"].size() == 1) { return false; }
      for (int i = 1; i < tu3djson["features"].size(); i++) {
        features.push_back(tu3djson::get_tu3djson_feature(tu3djson, i));
      }
      tu3djson["features"] = {tu3djson["features"][0]};
      return true;
    }

    bool Geometry::typechange(json &tu3d) {
      if (tu3djson["features"][0]["geometry"]["type"] == type) {
        return false;
      }
      tu3d = tu3djson;
      return true;
    }

    void Geometry::val3dityReport() {
//      std::cout << tu3djson << endl;
      // - GET REPORT -
      json report = val3dity::validate(tu3djson, val3dity::Parameters().tol_snap(
              STANDARDS["Tollerances"]["snap_tol"]).planarity_d2p_tol(
              STANDARDS["Tollerances"]["planarity_d2p_tol"]).planarity_n_tol(
              STANDARDS["Tollerances"]["planarity_n_tol"]).overlap_tol(
              STANDARDS["Tollerances"]["overlap_tol"]));
      Iso_Repairs = report["all_errors"];

      // -  USE THE REPAIR FUNCTION -
      emptyRepairsNeeded();
      emptyRepairsNeeded();
      if (!report["validity"]) {
        if (STANDARDS["OutputParameters"]["Debugging"]) { std::cout << report << endl; }

        for (const auto &feature: report["features"]) {
          for (const auto &error: feature["errors"]) {
            int errorCode = error["code"];
            bool add = contains({errorCode}, STANDARDS["GeometryRepair"]["ErrorsToRepair"]);
            if (STANDARDS["GeometryRepair"]["SolveAll"] or add) {
              if (TYPE_ERROR_CODE["RingErrors"].contains(errorCode)) {
                RepairsNeeded["RingErrors"].push_back({errorCode, error["id"], error["info"]});
              }

              if (TYPE_ERROR_CODE["PolyErrors"].contains(errorCode)) {
                RepairsNeeded["PolyErrors"].push_back({errorCode, error["id"], error["info"]});
              }

              if (TYPE_ERROR_CODE["ShellErrors"].contains(errorCode)) {
                RepairsNeeded["ShellErrors"].push_back({errorCode, error["id"], error["info"]});
              }

              if (TYPE_ERROR_CODE["SolidErrors"].contains(errorCode)) {
                RepairsNeeded["SolidErrors"].push_back({errorCode, error["id"], error["info"]});
              }

              if (TYPE_ERROR_CODE["SolidIErrors"].contains(errorCode)) {
                RepairsNeeded["SolidIErrors"].push_back({errorCode, error["id"], error["info"]});
              }
            }
          }
        }
        if (RepairsNeeded["RingErrors"].empty() && RepairsNeeded["PolyErrors"].empty() &&
            RepairsNeeded["ShellErrors"].empty() && RepairsNeeded["SolidErrors"].empty() &&
            RepairsNeeded["SolidIErrors"].empty()) {
          // all the errors found don't need to be fixed
          valid = true;
        }
        if (STANDARDS["OutputParameters"]["Debugging"]) { std::cout << RepairsNeeded << endl; }
      } else { valid = true; }
    }

    json::array_t Geometry::get_ISO_remaining() { return Iso_Repairs; }

    string Geometry::get_type() { return type; }

    bool Geometry::isvalid() { return valid; }

    bool Geometry::isUCvalid() { return UseCasevalid; }

    bool Geometry::istriangulated() { return triangulated; }

    vector<json> Geometry::RepairRingErrors() {
      std::cerr << "This repair isn't implemented for this geometry type yet" << endl;
      std::cerr << "\tOr there is an weird error, please contact developper" << endl;
      return {};
    }

    vector<json> Geometry::RepairPolyErrors() {
      std::cerr << "This repair isn't implemented for this geometry type yet" << endl;
      std::cerr << "\tOr there is an weird error, please contact developper" << endl;
      return {};
    }

    vector<json> Geometry::RepairShellErrors() {
      std::cerr << "This repair isn't implemented for this geometry type yet" << endl;
      std::cerr << "\tOr there is an weird error, please contact developper" << endl;
      return {};
    }

    vector<json> Geometry::RepairSolidErrors() {
      std::cerr << "This repair isn't implemented for this geometry type yet" << endl;
      std::cerr << "\tOr there is an weird error, please contact developper" << endl;
      return {};
    }

    vector<json> Geometry::RepairSolidIErrors() {
      std::cerr << "This repair isn't implemented for this geometry type yet" << endl;
      std::cerr << "\tOr there is an weird error, please contact developper" << endl;
      return {};
    }

    vector<json> Geometry::RepairGlobal() {
      std::cerr << "This repair isn't implemented for this geometry type yet" << endl;
      std::cerr << "\tOr there is an weird error, please contact developper" << endl;
      return {};
    }

    vector<json> Geometry::RepairSemantics() {
      vector<json> repairs_done;
      json done;

      done["code"] = "Semantic repairs";
      if (STANDARDS["UseCaseRepair"]["SemanticsValidate"]) {
        done["description"] = "Validate and set all semantics";
      } else {
        done["description"] = "Add missing semantics";
      }

      done["id"] = id;

      json geom;
      geom["boundaries"] = tu3djson["features"][0]["geometry"]["boundaries"];
      geom["type"] = tu3djson["features"][0]["geometry"]["type"];
      if (tu3djson["properties"].contains("semantics")) {
        geom["semantics"] = tu3djson["properties"]["semantics"];
        done["boundary_before"] = geom["semantics"]["values"];
      } else {
        done["boundary_before"] = {};
      }

      semanticsRepair(geom);
      if (geom["semantics"]["values"].empty()) {
        std::cout << "empty boundary so no adding of semantics " << endl;
        semantics = true;
        UseCasevalid = true;
        return repairs_done;
      }

      tu3djson["properties"]["semantics"] = geom["semantics"];
      done["boundary_after"] = geom["semantics"]["values"];

      // show progress
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\t\tRepaired Geometery " << id << " with Use Case repair: ";
        if (STANDARDS["UseCaseRepair"]["SemanticsValidate"]) {
          std::cout << "Validate and set all semantics" << endl;
        } else {
          std::cout << "Add missing semantics" << endl;
        }
      }
      semantics = true;
      UseCasevalid = true;
      repairs_done.push_back(done);
      return repairs_done;
    }

    vector<json> Geometry::RepairMesh() {
      vector<json> repairs_done;
      json done;
      done["code"] = "Mesh repairs";

      int numberdone = 0;
      string description;

      if (STANDARDS["UseCaseRepair"]["Triangulation"]) {
        description += "Triangulate object";
        numberdone += 1;
        triangulated = true;
      }
      if (STANDARDS["UseCaseRepair"]["Simplification"]) {
        if (numberdone > 0) { description += " and "; }
        description = "Simplify object";
        numberdone += 1;
      }
      if (STANDARDS["UseCaseRepair"]["RemeshSlivers"]) {
        if (numberdone > 0) { description += " and "; }
        description += "Remesh object";
        numberdone += 1;
      }
      done["description"] = description;
      done["id"] = id;


      done["boundary_before"] = tu3djson["features"][0]["geometry"]["boundaries"];
      triangulate(tu3djson["features"][0]["geometry"]);
      done["boundary_after"] = tu3djson["features"][0]["geometry"]["boundaries"];
      //TODO add SMT face


      // show progress
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\t\tRepaired Geometery " << id << " with Use Case repair: " << description << endl;
      }

      UseCasevalid = true;
      repairs_done.push_back(done);
      update_vertices();
      return repairs_done;
    }

    bool Geometry::isSMT() {
      return (semantics || material || texture);
    }

    void Geometry::startFacesSMT() {
      std::cout << "NOT SUPOSED TO COME HERE!" << endl;
    }

    void Geometry::writeFacesSMT() {
      std::cout << "NOT SUPOSED TO COME HERE!" << endl;
    }

    unordered_map<vector<int>, json, VectorHash> Geometry::getFacesSMT() {
      return FacesSMT;
    }

    // Comparator function to sort pairs by the double key in descending order
    bool compareDescending(const std::pair<double, std::vector<int>> &lhs,
                           const std::pair<double, std::vector<int>> &rhs) {
      return lhs.first > rhs.first;
    }

    void Geometry::SMTassigner(vector<int> &face) {
      // Check if the face exists in the FacesSMT map
      auto it = FacesSMT.find(face);

      // if it doesnt exist fix data
      if (it == FacesSMT.end()) {
        bool found = false;

        // TRY BASED ON POINT INDEXES
        set<int> faceSet(face.begin(), face.end());
        for (const auto &entry: FacesSMT) {
          set<int> CurrentFace(entry.first.begin(), entry.first.end());

          // test if new face is (sub)set of currentface
          bool isSubset = all_of(faceSet.begin(), faceSet.end(),
                                 [&CurrentFace](int val) {
                                     return CurrentFace.find(val) != CurrentFace.end();
                                 });
          if (isSubset) {
            FacesSMT[face] = FacesSMT[entry.first];
            found = true;
            Nef_polyhedron facepoly;
            bool makeNef = make_2D_polySMT(face, facepoly);
            if (makeNef) {
              SMTnefs.push_back({facepoly, face});
            }
            break;
          }

          Nef_polyhedron facepoly;
          bool makeNef = make_2D_polySMT(face, facepoly);

          // TRY BASED ON BOOLEAN OPERATIONS
          try {
            if (!found && makeNef){
              bool partly = false;
              vector<pair<double, vector<int>>> areaMap;
              for (const auto& nefPair: SMTnefs) {

                // check if completely inside
                Nef_polyhedron difference = facepoly - nefPair.first;  // Subtract checkpoly from facepoly
                if (difference.is_empty()) {
                  FacesSMT[face] = FacesSMT[nefPair.second];
                  found = true;
                  SMTnefs.push_back({facepoly, face});
                  break;
                }

                // check if partly inside
                double area = 0;
                Nef_polyhedron intersection = facepoly * nefPair.first;  // Perform intersection
                if (!intersection.is_empty()) {
                  vector<vector<vector<Point3E>>> intersectionrings = get_faces(intersection);
                  for (const auto &vec2: intersectionrings) {
                    for (const auto &vec1: vec2) {
                      area += compute_area_from_3d_polygon(vec1);
                    }
                  }
                  areaMap.push_back({area, nefPair.second});
                  partly = true;
                }
              }
              if (partly && !found) {
                // take the biggest
                sort(areaMap.begin(), areaMap.end(), compareDescending);
                for (const auto &entry: areaMap) {
                  if (!FacesSMT[entry.second].contains("empty")) {
                    found = true;
                    FacesSMT[face] = FacesSMT[entry.second];
                    SMTnefs.push_back({facepoly, face});
                    break;
                  }
                }
              }
            }
          }
          catch (...) {
            found = false;
          }

          // ADD VALUES AS NULL
          if (!found) {
            json empty;
            empty["empty"] = true;
            FacesSMT[face] = empty;
          }
        }
      }
    }

    void Geometry::SMTassigner(vector<vector<int>> &faces) {
      SMTassigner(faces[0]);
    }

    void Geometry::SMTassigner(vector<vector<vector<int>>> &faces) {
      for (int i = 0; i < faces.size(); ++i) {
        SMTassigner(faces[i]);
      }
    }

    void Geometry::SMTassigner(vector<vector<vector<vector<int>>>> &faces) {
      for (int i = 0; i < faces.size(); ++i) {
        SMTassigner(faces[i]);
      }
    }

    void Geometry::SMTassigner(vector<vector<vector<vector<vector<int>>>>> &faces) {
      for (int i = 0; i < faces.size(); ++i) {
        SMTassigner(faces[i]);
      }
    }

    void Geometry::SMTassigner(vector<vector<vector<vector<vector<vector<int>>>>>> &faces) {
      for (int i = 0; i < faces.size(); ++i) {
        SMTassigner(faces[i]);
      }
    }

    bool Geometry::isempty() {
      set<int> all;
      recursive_all_indexes(tu3djson["features"][0]["geometry"]["boundaries"], all);
      if (all.empty()) {
        tu3djson["features"][0]["geometry"]["boundaries"] = json::array();
        return true;
      }
      return false;
    }
} // AUTOr3pair
