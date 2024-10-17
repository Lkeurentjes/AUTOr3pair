// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#include "MultiSurface.h"
#include <iostream>

#include "Ringr3pair.h"
#include "Polyr3pair.h"
#include "Globalr3pair.h"

#include "Defenitions.h"
#include "DefenitionsVal3dity.h"
#include "DefenitionsCGAL.h"
#include "Geometrytools_makeCGAL.h"


#include "../thirdparty/nlohmann-json/json.hpp"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {

    void MultiSurface::startFacesSMT() {
      if (FacesSMT.empty()) {
        json SMT;
        for (int i = 0; i < tu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
          if (tu3djson["properties"].contains("semantics")) {
            SMT["semantics"] = tu3djson["properties"]["semantics"]["values"][i];
          }
          if (tu3djson["properties"].contains("material")) {
            SMT["material"] = {};
            for (const auto &m: tu3djson["properties"]["material"].items()) {
              SMT["material"][m.key()] = m.value()["values"][i];
            }
          }
          FacesSMT[(tu3djson["features"][0]["geometry"]["boundaries"][i][0])] = SMT;
          Nef_polyhedron facepoly;
          vector<int> face = tu3djson["features"][0]["geometry"]["boundaries"][i][0];
          bool makeNef = make_2D_polySMT(face,facepoly);
          if (makeNef){
            SMTnefs.push_back({facepoly, face});
          }
        }
      }
    }

    void MultiSurface::writeFacesSMT() {
      if (semantics) {
        tu3djson["properties"]["semantics"]["values"] = json::array();
      }
      if (material) {
        for (const auto &m: tu3djson["properties"]["material"].items()) {
          tu3djson["properties"]["material"][m.key()]["values"] = json::array();
        }
      }

      for (int i = 0; i < tu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
        json SMT = FacesSMT[(tu3djson["features"][0]["geometry"]["boundaries"][i][0])];
        if (SMT.is_null()) {
          std::cerr << "Lost info of a face in the repair process" << std::endl;
        }

        if (semantics) {
          tu3djson["properties"]["semantics"]["values"].push_back(SMT["semantics"]);
        }
        if (material) {
          int count = 0;
          for (const auto &m: tu3djson["properties"]["material"].items()) {
            tu3djson["properties"]["material"][m.key()]["values"].push_back(SMT["material"][m.key()]);
            count += 1;
          }
        }
      }
    }

    vector<json> MultiSurface::RepairRingErrors() {
      vector<json> repairs_done;

      vector<vector<vector<int>>> newboundaries;
      for (int i = 0; i < tu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
        bool repaired = false;
        vector<vector<int>> boundary = tu3djson["features"][0]["geometry"]["boundaries"][i];

        for (int error = 0; error < RepairsNeeded["RingErrors"].size(); ++error) {
          string test = RepairsNeeded["RingErrors"][error][1];
          test += "|";
          if (test.find("face=" + to_string(i) + "|") != string::npos) {
            repaired = true;
            json done;
            int code = RepairsNeeded["RingErrors"][error][0];
            // std::cout << RepairsNeeded["RingErrors"] << endl;
            if (code == 101) {
              // TOO FEW POINTS
              vector<vector<int>> replace101 = AUTOr3pair::Ringr3pair101(boundary);
              if (!replace101.empty()) {
                newboundaries.push_back(replace101);
              }
              done["boundary_now"] = replace101;
            } else if (code == 102) {
              // CONSECUTIVE POINTS SAME
              vector<vector<int>> replace102 = AUTOr3pair::Ringr3pair102(boundary);
              newboundaries.push_back(replace102);
              FacesSMT[replace102[0]] = FacesSMT[boundary[0]];
              done["boundary_now"] = replace102;
            } else if (code == 103) {
              // RING NOT CLOSED
              // cannot happen with CityJSON and TU3D if CityGML or other is ever implemented easy fix would be to add first vertex to last place
              std::cerr << "Gives an error 103, that should not be possible" << endl;
              repaired = false;
            } else if (code == 104) {
              // RING SELF INTERSECTION
              vector<vector<int>> replace104 = AUTOr3pair::Ringr3pair104(boundary);
              if (!replace104.empty()) {
                newboundaries.push_back(replace104);
                FacesSMT[replace104[0]] = FacesSMT[boundary[0]];
              }
              done["boundary_now"] = replace104;
            }
            if (repaired) {
              // Report part
              done["boundary_before"] = boundary;
              done["code"] = code;
              done["description"] = ALL_ERRORS[code];
              done["id"] = id;
              repairs_done.push_back(done);
              // show progress
              if (STANDARDS["OutputParameters"]["ShowProgress"]) {
                std::cout << "\t\tRepaired Geometery " << id << " with error " << code << " on " <<
                          RepairsNeeded["RingErrors"][error][1] << endl;
              }
            }
          }
        }
        if (!repaired) { newboundaries.push_back(boundary); }
      }
      tu3djson["features"][0]["geometry"]["boundaries"] = newboundaries;
      update_vertices();
      return repairs_done;
    }

    vector<json> MultiSurface::RepairPolyErrors() {
      vector<json> repairs_done;

      vector<vector<vector<int>>> newboundaries;
      for (int i = 0; i < tu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
        bool repaired = false;
        vector<vector<int>> boundary = tu3djson["features"][0]["geometry"]["boundaries"][i];
        for (int error = 0; error < RepairsNeeded["PolyErrors"].size(); ++error) {
          string test = RepairsNeeded["PolyErrors"][error][1];
          test += "|";
          if (test.find("face=" + to_string(i) +"|") != string::npos) {
            repaired = true;
            json done;
            int code = RepairsNeeded["PolyErrors"][error][0];
            if (code == 201) {
              // INTERSECTION RINGS
              vector<vector<int>> replace201 = AUTOr3pair::Polyr3pair201(boundary);
              if (!replace201.empty()){
                newboundaries.push_back(replace201);
                FacesSMT[replace201[0]] = FacesSMT[boundary[0]];
              }
              done["boundary_now"] = replace201;
            } else if (code == 202) {
              // DUPLICATED RINGS
              repaired = false;
              //this error doesnt excist right now in val3dity, seeing same rings also intersect
              std::cerr << "Gives an error 202, that should not be possible, contact developper" << endl;
            } else if (code == 203) {
              // NON PLANAR POLYGON DISTANCE PLANE
              string info = RepairsNeeded["PolyErrors"][error][2];
              vector<vector<vector<int>>> replace203 = AUTOr3pair::Polyr3pair203(boundary, info);
              for (int face = 0; face < replace203.size(); face++) {
                newboundaries.push_back(replace203[face]);
                FacesSMT[replace203[face][0]] = FacesSMT[boundary[0]];
              }
              done["boundary_now"] = replace203;
            } else if (code == 204) {
              // NON PLANAR POLYGON NORMALS DEVIATION
              vector<vector<int>> replace204 = AUTOr3pair::Polyr3pair204(boundary);
              newboundaries.push_back(replace204);
              FacesSMT[replace204[0]] = FacesSMT[boundary[0]];
              done["boundary_now"] = replace204;
            } else if (code == 205) {
              // POLYGON INTERIOR DISCONNECTED
              vector<vector<vector<int>>> replace205 = AUTOr3pair::Polyr3pair205(boundary);
              done["boundary_now"] = replace205;
              for (int face = 0; face < replace205.size(); face++) {
                newboundaries.push_back(replace205[face]);
                FacesSMT[replace205[face][0]] = FacesSMT[boundary[0]];
              }
            } else if (code == 206) {
              // INNER RING OUTSIDE
              vector<vector<vector<int>>> replace206 = AUTOr3pair::Polyr3pair206(boundary);
              //when deleted
              if (replace206.size() == 1) {
                newboundaries.push_back(replace206[0]);
                done["boundary_now"] = replace206[0];
              } else {
                done["boundary_now"] = replace206;
                //when kept
                for (int face = 0; face < replace206.size(); face++) {
                  newboundaries.push_back(replace206[face]);
                  FacesSMT[replace206[face][0]] = FacesSMT[boundary[0]];
                }
              }
            } else if (code == 207) {
              // INNER RING NESTED
              vector<vector<vector<int>>> replace207 = AUTOr3pair::Polyr3pair207(boundary);

              //when deleted
              if (replace207.size() == 1) {
                newboundaries.push_back(replace207[0]);
                done["boundary_now"] = replace207[0];
              } else {
                done["boundary_now"] = replace207;
                //when kept
                for (int face = 0; face < replace207.size(); face++) {
                  newboundaries.push_back(replace207[face]);
                  FacesSMT[replace207[face][0]] = FacesSMT[boundary[0]];
                }

              }
            } else if (RepairsNeeded["PolyErrors"][error][0] == 208) {
              // ORIENTATION RINGS SAME
              vector<vector<int>> replace208 = AUTOr3pair::Polyr3pair208(boundary);
              newboundaries.push_back(replace208);
              done["boundary_now"] = replace208;
            }
            if (repaired) {
              // Report part
              done["boundary_before"] = boundary;
              done["code"] = code;
              done["description"] = ALL_ERRORS[code];
              done["id"] = id;
              repairs_done.push_back(done);
              // show progress
              if (STANDARDS["OutputParameters"]["ShowProgress"]) {
                std::cout << "\t\tRepaired Geometery " << id << " with error " << code << " on " <<
                          RepairsNeeded["PolyErrors"][error][1] << endl;
              }
            }
          }
        }
        if (!repaired) { newboundaries.push_back(boundary); }
      }
      tu3djson["features"][0]["geometry"]["boundaries"] = newboundaries;
      update_vertices();
      return repairs_done;
    }

    vector<json> MultiSurface::RepairGlobal() {
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\t\tRepaired Geometery " << id << " with global repair" << endl;
        std::cout << "\t\t\tTrying alpha wrap on faces" << endl;
      }

      vector<json> repairs_done;
      json done;
      vector<vector<vector<int>>> shell = OGtu3djson["features"][0]["geometry"]["boundaries"];
      done["boundary_before"] = shell;

      // do alpha wrap on polygons
      vector<vector<vector<int>>> replaceAF = AUTOr3pair::Globalr3pairAlphaFaces(shell);

      update_vertices();
      tu3djson["features"][0]["geometry"]["boundaries"] = replaceAF;

      done["code"] = "AlphaWrapFaces";
      done["description"] = "Global alpha wrap on faces";
      done["id"] = id;
      done["boundary_now"] = replaceAF;

      val3dityReport();
      if (isvalid()){
        if (STANDARDS["OutputParameters"]["ShowProgress"]) {
          std::cout << "\t\t\tMade alpha wrap on faces" << endl;
        }
        SMTassigner(replaceAF);
        repairs_done.push_back(done);
        return repairs_done;
      }
      // do alpha wrap on points
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\t\t\tTrying alpha wrap on points" << endl;
      }
      vector<vector<vector<int>>> replaceAP = AUTOr3pair::Globalr3pairAlphaPoints(shell);

      update_vertices();
      tu3djson["features"][0]["geometry"]["boundaries"] = replaceAP;


      done["code"] = "AlphaWrapFaces";
      done["description"] = "Global alpha wrap on point";
      done["id"] = id;
      done["boundary_now"] = replaceAP;


      val3dityReport();
      if (isvalid()){
        if (STANDARDS["OutputParameters"]["ShowProgress"]) {
          std::cout << "\t\t\tMade alpha wrap on points" << endl;
        }
        SMTassigner(replaceAP);
        repairs_done.push_back(done);
        return repairs_done;
      }

      // Do convex hull
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\t\t\tTrying convex hull on points" << endl;
      }
      vector<vector<vector<int>>> replaceCH = AUTOr3pair::Globalr3pairConvexHull(shell);

      update_vertices();
      tu3djson["features"][0]["geometry"]["boundaries"] = replaceCH;


      done["code"] = "Convex hull";
      done["description"] = "Global alpha wrap on faces";
      done["id"] = id;
      done["boundary_now"] = replaceCH;


      val3dityReport();
      if (isvalid()){
        if (STANDARDS["OutputParameters"]["ShowProgress"]) {
          std::cout << "\t\t\tMade Convex hull" << endl;
        }
        SMTassigner(replaceCH);
        repairs_done.push_back(done);
        return repairs_done;
      }

      // if that doesn't work do bounding box
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\t\t\tMade Boundingbox" << endl;
      }
      set<int> indexesshell;
      recursive_all_indexes(OGtu3djson["features"][0]["geometry"]["boundaries"], indexesshell);
      bool surface = false;
      vector<vector<vector<int>>> replaceBB = AUTOr3pair::Globalr3pairBoundingBox(indexesshell,surface);
      SMTassigner(replaceBB);
      update_vertices();
      tu3djson["features"][0]["geometry"]["boundaries"] = replaceBB;
      if (surface){
        tu3djson["properties"]["lod"] = "0.1";
      }else{
        tu3djson["properties"]["lod"] = "1.0";
      }


      done["code"] = "Boundingbox";
      done["description"] = "Did the last resort creating boundingbox per shell";
      done["id"] = id;
      done["boundary_now"] = replaceBB;


      repairs_done.push_back(done);
      return repairs_done;
    }

} // AUTOr3pair
