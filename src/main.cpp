// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#include <iostream>
#include <string>

#include "Defenitions.h"
#include "InputOutput.h"
#include "CityObject.h"
#include "BuildingPartr3pair.h"


#include "nlohmann-json/json.hpp"
#include "val3dity.h"


using json = nlohmann::json;
using namespace std;

int main(int argc, const char *argv[]) {
  if (argc == 1) {
    std::cerr << "No inputs are given, input a (city)JSON or OBJ file" << std::endl;
    return 1;
  } else if (argc == 2) {
    FILENAME = string(argv[1]);
  } else {
    FILENAME = string(argv[1]);
    //LOD can be second place
    std::string arg2 = std::string(argv[2]);
    if (arg2.rfind("-LOD", 0) == 0) {
      LOD = arg2.substr(4);  // Extract substring after "-LOD"
    } else {
      USERINPUTFILENAME = arg2;
    }
    //LOD can be second place
    if (argc > 3) {
      std::string arg3 = std::string(argv[3]);
      if (arg3.rfind("-LOD", 0) == 0) {
        LOD = arg3.substr(4);  // Extract substring after "-LOD"
      }
    }
    if (argc > 4) { std::cerr << "To much inputs are given, will only use the first 2" << std::endl; }
  }

//  FILENAME = "../data/Test_pre_post_processing/CITYJSON_cube_duplicateSNAP.json";
//  USERINPUTFILENAME = "../data/Test_use_case_parameters/USERINPUT_watertight.json";
  AUTOr3pair::filehandler InOutPutHandler;

  // ---   PARSE USER FILE     ---
  InOutPutHandler.set_standards();
  set<string> scopeTypes = {
          "Building", "BuildingPart", "BuildingInstallation", "BuildingConstructiveElement",
          "BuildingFurniture", "BuildingStorey", "BuildingRoom", "BuildingUnit"
  };
  if (STANDARDS["InputParameters"].contains("ExtendScope")) {
    for (string scope: STANDARDS["InputParameters"]["ExtendScope"]) { scopeTypes.insert(scope); }
  }
  //---   PARSE INPUT FILES     ---
  InOutPutHandler.set_input_file();
  json j;
  if (STANDARDS["OutputParameters"]["ShowProgress"]) {
    std::cout << "Start parsing the " << FILETYPE << " file" << std::endl;
  }
  if (FILETYPE == "json") {
    bool error = InOutPutHandler.parse_JSON(j);
    if (error) {
      InOutPutHandler.write_report();
      return 1;
    }
  } else if (FILETYPE == "obj") {
    bool error = InOutPutHandler.parse_obj(j);
    if (error) {
      InOutPutHandler.write_report();
      return 1;
    }
  } else {
    std::cerr << "Cannot repair '" << FILETYPE << "' type files at this moment" <<
              std::endl;
    InOutPutHandler.add_data_error(901);
    InOutPutHandler.write_report();
    return 1;
  }
  if (STANDARDS["OutputParameters"]["ShowProgress"]) {
    std::cout << "Finished parsing the " << FILETYPE << " file" << std::endl;
  }

  // -- CREATE VERTEX VECTOR/LIST --
  json scale;
  json translate;
  if (FILETYPE == "json") {
    scale = j["transform"]["scale"];
    translate = j["transform"]["translate"];
  }
  if (STANDARDS["OutputParameters"]["ShowProgress"]) {
    std::cout << "Set the vertices and check for duplicates" << std::endl;
  }
//  AUTOr3pair::Vertices vertices(j["vertices"], scale, translate);
//  VERTICES = vertices;
  VERTICES.initialize(j["vertices"], scale, translate);

  // ---       REPAIR LOOP      ---
  json RepairReport; //TODO make this features part function in INOUThandler
  json repaired_Objects;
  RepairReport["features"] = json::array();

  // -- GET A SINGLE OBJECT --
  if (STANDARDS["OutputParameters"]["ShowProgress"]) {
    std::cout << "Start the repair loop " << std::endl;
  }
  for (const auto &co: j["CityObjects"].items()) {
    if (STANDARDS["OutputParameters"]["ShowProgress"]) {
      std::cout << "Start the repair of: " << co.key() << std::endl;
    }

    // - CHECK IF OBJECT HAS GEOMETRY  -
    // otherwise it can be passed because it cannot be broken
    if (FILETYPE == "json" && co.value()["geometry"] == nullptr) {
      repaired_Objects[co.key()] = co.value();
      continue;
    }

    // -- PRE PROCESSING STEPS (FOR USE CASES)
    if (STANDARDS["OutputParameters"]["ShowProgress"]) {
      std::cout << "\tPre processing: changing duplicate vertex indexes " << std::endl;
    }
    // -- translate duplicate vertices indexes
    co.value()["geometry"] = VERTICES.ChangeduplicateVertices(co.value()["geometry"]);

    // -- watertight geometry primitives
    if (STANDARDS["UseCaseRepair"]["Watertight"]) {
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\tPre processing: Translate to watertight object if needed" << std::endl;
      }
      if (LOD != "0" || LOD != "0.0" || LOD != "0.1" || LOD != "0.2" || LOD != "0.3") {
        for (auto &geom: co.value()["geometry"]) {
          if (geom["type"] == "MultiSurface" or geom["type"] == "CompositeSurface") {
            geom["type"] = "Solid";
            geom["boundaries"] = {geom["boundaries"]};
            if (geom.contains("semantics")) {
              geom["semantics"]["values"] = {geom["semantics"]["values"]};
            }
            if (geom.contains("material")) {
              for (auto &m: geom["material"]) {
                m["values"] = {m["values"]};
              }
            }
          }
          if (geom["type"] == "MultiSolid") {
            geom["type"] = "CompositeSolid";
          }
        }
      }
    }
    // -- orientation geometry primitives
    if (STANDARDS["UseCaseRepair"]["Orientation"]) {
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\tPre processing: Translate to orrientation object if needed" << std::endl;
      }
      for (auto &geom: co.value()["geometry"]) {
        if (geom["type"] == "MultiSurface") {
          geom["type"] = "CompositeSurface";
        }
      }
    }
    // - CHECK IF TYPE IS IN SCOPE -
    bool inScope = false;
    if (scopeTypes.contains(co.value()["type"])) { inScope = true; }
    if (inScope) {
      //- GO TO CITYOBJECT CLASS -
      AUTOr3pair::CityObject CO(co.key(), co.value(), true);
      // -- WRITE TU3DJSON BACK TO CITYOBJECT --
      json repairedCO = CO.getCObject();
      for (auto &[key, value]: repairedCO.items()) { repaired_Objects[key] = value; }
      InOutPutHandler.addfeatureReport(CO.getRReport());

    } else {
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\tObject is not in repair scope" << std::endl;
      }
      // - GO TO CITYOBJECT CLASS -
      AUTOr3pair::CityObject CO(co.key(), co.value(), false);
      // -- WRITE  BACK TO CITYOBJECT --
      json repairedCO = CO.getCObject();
      for (auto &[key, value]: repairedCO.items()) { repaired_Objects[key] = value; }
      InOutPutHandler.addfeatureReport(CO.getRReport());
    }
    //todo add parent and child together in Feature RepairReport
  }
  if (STANDARDS["OutputParameters"]["ShowProgress"]) {
    std::cout << "Repair loop is done" << std::endl;
  }
  j["CityObjects"] = repaired_Objects;
  j["vertices"] = VERTICES.get_verticeslist();

  // 601 && overlap repair
  bool repair601 = true;
  if (STANDARDS["GeometryRepair"].contains("ErrorsToRepair")) {
    set<int> toRepair = STANDARDS["GeometryRepair"]["ErrorsToRepair"];
    if (!toRepair.contains(601)) {
      repair601 = false;
    }
  }

  if (FILETYPE == "json" && (repair601 || STANDARDS["GeometryRepair"]["SolveAll"])) {

    json copy = j;
    json report = val3dity::validate(copy, val3dity::Parameters().tol_snap(
            STANDARDS["Tollerances"]["snap_tol"]).planarity_d2p_tol(
            STANDARDS["Tollerances"]["planarity_d2p_tol"]).planarity_n_tol(
            STANDARDS["Tollerances"]["planarity_n_tol"]).overlap_tol(
            STANDARDS["Tollerances"]["overlap_tol"]));
    set<int> all_errors = report["all_errors"];
    bool visited_all = false;
    set<string> visited;
    while(all_errors.contains(601)) {
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "Fixing 601 errors" << std::endl;
      }
      bool found = false;
      for (const auto &feature: report["features"]) {
        for (const auto &error: feature["errors"]) {
          if (error["code"] == 601){
            string splitAND = "&&";
            string prefix = "coid=";
            string input = error["id"];
            if (visited.contains(input)){continue;}
            found = true;
            visited.insert(input);
            string LODl= error["info"];
            size_t pos = input.find(splitAND);

            // Remove the "coid=" prefix and everything after '|'
            auto processID = [&](std::string &str) {
                if (str.rfind(prefix, 0) == 0) {
                  str = str.substr(prefix.length());
                }
                size_t pipePos = str.find('|');
                if (pipePos != std::string::npos) {
                  str = str.substr(0, pipePos);
                }
            };

            string resultfirst = input.substr(0, pos);
            string resultsecond = input.substr(pos + splitAND.length());
            processID(resultfirst);
            processID(resultsecond);

            char delimiter = '=';
            size_t posIS = LODl.find(delimiter);
            string LOD = LODl.substr(posIS +1);

            if (STANDARDS["OutputParameters"]["ShowProgress"]) {
              // Output the pair
              std::cout << "\tRepairing 601 on " << resultfirst<< " && " << resultsecond << " on LOD " <<  LOD << std::endl;
            }
            if (resultfirst != resultsecond){
              json repairedrepo = AUTOr3pair::BuildingPartr3pair601(j["CityObjects"][resultfirst],j["CityObjects"][resultsecond], LOD);

              if (j["CityObjects"][resultsecond].contains("geometry")){
                AUTOr3pair::CityObject CO2(resultsecond, j["CityObjects"][resultsecond], true);
                json repairedCO2 = CO2.getCObject();
                for (auto &[key, value]: repairedCO2.items()) { j["CityObjects"][key] = value; }
//            InOutPutHandler.addfeatureReport(CO.getRReport());
              }
            }else{
              json repairedrepo = AUTOr3pair::BuildingPartr3pair601(j["CityObjects"][resultfirst], LOD);
            }


            // do geometry repairs on both parts to be sure they didn't change;
            AUTOr3pair::CityObject CO(resultfirst, j["CityObjects"][resultfirst], true);
            json repairedCO = CO.getCObject();
            for (auto &[key, value]: repairedCO.items()) { j["CityObjects"][key] = value; }
//            InOutPutHandler.addfeatureReport(CO.getRReport());


            j["vertices"] = VERTICES.get_verticeslist();
            copy = j;
            json report = val3dity::validate(copy, val3dity::Parameters().tol_snap(
                    STANDARDS["Tollerances"]["snap_tol"]).planarity_d2p_tol(
                    STANDARDS["Tollerances"]["planarity_d2p_tol"]).planarity_n_tol(
                    STANDARDS["Tollerances"]["planarity_n_tol"]).overlap_tol(
                    STANDARDS["Tollerances"]["overlap_tol"]));
            set<int> new_all = report["all_errors"];
            all_errors = new_all;

          }
          if (found){
            break;
          }
        }
        if (found){
          break;
        }
      }
      if (!found){
        break;
      }
    }
  }

  // ---      WRITE FILES     ---
  // -- POST PROCESSING --
  //remove parent child relations
  if (STANDARDS["OutputParameters"]["ShowProgress"]) {
    std::cout << "Post processing: Parent child relations" << std::endl;
  }
  InOutPutHandler.post_processing_child_parrent(j["CityObjects"]);
  InOutPutHandler.Sort_features();

  // delete orphan vertices
  if (STANDARDS["OutputParameters"]["ShowProgress"]) {
    std::cout << "Post processing: Orphan vertices" << std::endl;
  }

  VERTICES.deleteorphanVertices(j["CityObjects"]);


  if (STANDARDS["OutputParameters"]["ShowProgress"]) {
    std::cout << "Post processing: Change Multi-/Composite-Solid to one solid" << std::endl;
  }
  // change M/C Solid with 1 into Solid
  for (auto &co: j["CityObjects"]) {
    if (!co.contains("geometry")) { continue; }
    for (auto &geom: co["geometry"]) {
      if ((geom["type"] == "MultiSolid" || geom["type"] == "CompositeSolid") && geom["boundaries"].size() == 1) {
        geom["type"] = "Solid";
        geom["boundaries"] = geom["boundaries"][0];
        if (geom.contains("semantics")) { geom["semantics"]["values"] = geom["semantics"]["values"][0]; }
        if (geom.contains("material")) {
          for (auto &m: geom["material"]) {
            m["values"] = m["values"][0];
          }
        }
        if (geom.contains("texture")) {
          for (auto &t: geom["texture"]) {
            t["values"] = t["values"][0];
          }
        }
      }
    }
  }


  if (STANDARDS["OutputParameters"]["ShowProgress"]) {
    std::cout << "Post processing: Change Metadata" << std::endl;
  }
  if (FILETYPE == "json") {
    //todo update metadata
    // todo update attributes
    InOutPutHandler.update_metadata(j);
    if (j.contains("metadata") && j["metadata"].contains("geographicalExtent")) {
      j["metadata"]["geographicalExtent"] = VERTICES.get_Geographical_extend();
    }

    //todo check LOD
  }

  if (STANDARDS["OutputParameters"]["ShowProgress"]) {
    std::cout << "Write output file(s)" << std::endl;
  }

  InOutPutHandler.write_output(j);
  return 0;
}
