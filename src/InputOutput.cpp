// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#include <iostream>
#include <fstream>
#include "InputOutput.h"
#include <CGAL/IO/OBJ.h>
#include "Defenitions.h"
#include "DefenitionsVal3dity.h"
#include "DefenitionsUseCase.h"
#include "DefenitionsCGAL.h"

namespace AUTOr3pair {
    filehandler::filehandler() {
    }


    // -------------------------------------------------------------------------------------------------Input functions

    void filehandler::set_input_file() {
      pos = FILENAME.find_last_of(".") + 1;
      string extension = FILENAME.substr(pos);
      ranges::transform(extension, extension.begin(), ::tolower);
      FILETYPE = extension;

      string outputCombi = FILENAME.substr(0, pos - 1);
      string citySuffix = ".city";
      size_t cityPos = outputCombi.rfind(citySuffix);
      if (cityPos != string::npos) {
        outputCombi.erase(cityPos, citySuffix.length());
      }

      if (USERINPUTFILENAME != "") {
        outputCombi += "__";
        size_t posI = USERINPUTFILENAME.find_last_of(".");
        size_t posSlash = USERINPUTFILENAME.find_last_of("/") + 1;
        if (posI != std::string::npos && posSlash != std::string::npos) {
          outputCombi += USERINPUTFILENAME.substr(posSlash, posI - posSlash);
        } else if (posI != std::string::npos) {
          outputCombi += USERINPUTFILENAME.substr(0, posI - 1);
        } else {
          outputCombi += USERINPUTFILENAME;
        }
      }
      if (LOD != ""){
        outputCombi += "__" + LOD;
      }

      if (pos != 0 and FILENAME.find("/") != pos) {
        ReportNAME = (outputCombi + "__RepairReport.json");
        OutNAME = (outputCombi + "__REPAIRED." + extension);
      } else {
        ReportNAME = FILENAME + "/directory_RepairReport.json";
        OutNAME = (outputCombi + "__REPAIRED." + extension);
      };
      start_report();
    }

    void filehandler::start_report() {
      RepairReport["input_file_type"] = FILETYPE;
      RepairReport["input_file"] = FILENAME;
      RepairReport["data_errors"] = json::array();
      RepairReport["features"] = json::array();
      RepairReport["type"] = "AUTOr3pairReport";
      RepairReport["version"] = VERSION;
      RepairReport["time"] = get_time();
    }

    string filehandler::get_time() {
      std::time_t rawtime;
      struct tm *timeinfo;
      std::time(&rawtime);
      timeinfo = std::localtime(&rawtime);

      char buffer[80];
      std::strftime(buffer, 80, "%c %Z", timeinfo);
      return buffer;
    }

    void filehandler::set_standards() {
      if (USERINPUTFILENAME == "VISUALIZATION") {
        STANDARDS["UseCaseRepair"] = VISUALIZATION;
        UseCase = "VISUALIZATION";
      } else if (USERINPUTFILENAME == "CFD") {
        STANDARDS["UseCaseRepair"] = CFD;
        UseCase = "CFD";
      } else if (USERINPUTFILENAME == "ENERGYDEMAND") {
        STANDARDS["UseCaseRepair"] = ENERGYDEMAND;
        UseCase = "ENERGY DEMAND";
      } else if (USERINPUTFILENAME == "SOLARPOWER") {
        STANDARDS["UseCaseRepair"] = SOLARPOWER;
        UseCase = "SOLAR POWER";
      } else {
        std::ifstream inputuser(USERINPUTFILENAME);
        json input;
        try {
          inputuser >> input;
          for (auto &[key, value]: STANDARDS.items()) {
            if (value.is_object()) {
              for (auto &[subKey, subValue]: value.items()) {
                if (input.contains(subKey)) {
                  STANDARDS[key][subKey] = input[subKey];
                }
              }
            }
          }

          if (input.contains("ExtendScope")) { STANDARDS["InputParameters"]["ExtendScope"] = input["ExtendScope"]; }
          if (input["InputParameters"].contains(
                  "ExtendScope")) { STANDARDS["InputParameters"]["ExtendScope"] = input["InputParameters"]["ExtendScope"]; }

          // for (auto it = test.begin(); it != test.end(); ++it) { STANDARDS[it.key()] = it.value(); }
          // Change Solve All
          if (STANDARDS["GeometryRepair"]["ErrorsToRepair"].size() != 0) {
            STANDARDS["GeometryRepair"]["SolveAll"] = false;
          }

          // Check snap_tol vs overlap_tol
          if (STANDARDS["Tollerances"]["overlap_tol"] < STANDARDS["Tollerances"]["snap_tol"] ){
            std::cerr << "Overlap tol is smaller than snap_tol, so overlap_tol is set to snap_tol" << endl;
            STANDARDS["Tollerances"]["overlap_tol"] = STANDARDS["Tollerances"]["snap_tol"];
          }

          UseCase = "Based on input file";
        }
        catch (nlohmann::detail::parse_error &e) {
          std::cerr << "No user input file given or not a valid JSON file. Standards will be used" << std::endl;
        }
        catch (std::ios_base::failure &e) {
          std::cerr << "User input file given is a directory. Standards will be used" << std::endl;
        }
      }
      set_parameters();
    }

    void filehandler::set_parameters() {
      json pm;
      pm["Tollerances"] = STANDARDS["Tollerances"];

      if (FILETYPE == "obj") { pm["InputParameters"]["OBJgeomtype"] = STANDARDS["InputParameters"]["OBJgeomtype"]; }

      pm["RepairDepths"] = STANDARDS["RepairDepths"];

      pm["GeometryRepair"] = STANDARDS["GeometryRepair"];
      if (STANDARDS["GeometryRepair"]["SolveAll"]) { pm["GeometryRepair"].erase("ErrorsToRepair"); }
      else { pm["GeometryRepair"].erase("SolveAll"); }

      if (STANDARDS["InputParameters"].contains(
              "ExtendScope")) { pm["InputParameters"]["ExtendScope"] = STANDARDS["InputParameters"]["ExtendScope"]; }
      json UC;
      UC["Name"] = UseCase;
      UC["StandardsUsed"] = STANDARDS["UseCaseRepair"];
      pm["UseCase"] = UC;

      if (LOD != ""){
        pm["LOD"] = LOD;
      }

      RepairReport["Parameters"] = pm;
    }

    bool filehandler::parse_JSON(json &j) {
      std::ifstream input(FILENAME);
      try { input >> j; }
      catch (nlohmann::detail::parse_error &e) {
        add_data_error(901);
        std::cerr << "Input file not a valid (city)JSON file" << std::endl;
        return true;
      }
      input.close();
      // TODO is this needed?
      if (j["type"] != "CityJSON") {
        add_data_error(901);
        std::cerr << "Input file not a CityJSON file" << std::endl;
        return true;
      }
      original = j;
      return false;
    }

    bool filehandler::parse_obj(json &j) {
      string geomtype = STANDARDS["InputParameters"]["OBJgeomtype"];
      if (geomtype != "MultiSurface" && geomtype != "CompositeSurface") {
        STANDARDS["UseCaseRepair"]["Watertight"] = true;
      } else if (geomtype == "CompositeSurface") {
        STANDARDS["UseCaseRepair"]["Orientation"] = true;
      }
      std::ifstream input(FILENAME);
      vector<Point3> points;
      vector<vector<int>> polys;
      if (!input || !CGAL::IO::read_OBJ(input, points, polys)) {
        add_data_error(901);
        std::cerr << "Input file not a valid OBJ file" << std::endl;
        return true;
      }
      j["vertices"] = json::array();
      for (const auto &i: points) { j["vertices"].push_back(vector<double>{i.x(), i.y(), i.z()}); }

      // PMP can not deal with error 101 and 102 (only with same point index) so deal with them before PMP
      RepairReport["OBJ101102"] = json::array();
      for (auto &p: polys) {
        if (p.size() == 2) {
          RepairReport["OBJ101102"].push_back({
                                                      {"boundary_before", {p}},
                                                      {"code",            101},
                                                      {"description",     ALL_ERRORS[101]},
                                                      {"id",              ""},
                                                      {"boundary_now",    json::array()}
                                              });
        } else {
          unordered_set<int> PSet(p.begin(), p.end());
          if (PSet.size() != p.size()) {
            std::vector<int> newp(PSet.begin(), PSet.end());
            std::reverse(newp.begin(), newp.end());
            RepairReport["OBJ101102"].push_back({
                                                        {"boundary_before", {p}},
                                                        {"code",            102},
                                                        {"description",     ALL_ERRORS[102]},
                                                        {"id",              ""},
                                                        {"boundary_now",    {newp}}
                                                });
            p = newp;
          }
        }
      }
      if (RepairReport["OBJ101102"].empty()) { RepairReport.erase("OBJ101102"); }

      Mesh sm;
      PMP::polygon_soup_to_polygon_mesh(points, polys, sm);
      FCCmap fccmap = sm.add_property_map<face_descriptor, faces_size_type>("f:CC").first;
      PMP::connected_components(sm, fccmap);

      for (auto f: faces(sm)) {
        vector<vector<int>> indices(1);
        CGAL::Vertex_around_face_circulator<Mesh> vcirc(sm.halfedge(f), sm), done(vcirc);
        do indices[0].push_back(*vcirc++); while (vcirc != done);
        j["CityObjects"][to_string(fccmap[f])]["geometry"]["boundaries"].push_back(indices);
      }


      for (const auto &[key, value]: j["CityObjects"].items()) {
        j["CityObjects"][key]["type"] = "Building"; // this makes object always in repair scope
        json geom = j["CityObjects"][key]["geometry"];
        j["CityObjects"][key]["geometry"] = {geom};
        j["CityObjects"][key]["geometry"][0]["type"] = geomtype;
        j["CityObjects"][key]["geometry"][0]["lod"] = nullptr;

        auto &geometry = j["CityObjects"][key]["geometry"][0];
        if (geomtype == "Solid") { geometry["boundaries"] = {geometry["boundaries"]}; }
        if (geomtype == "MultiSolid" or geomtype == "CompositeSolid") {
          geometry["boundaries"] = {{geometry["boundaries"]}};
        }
        std::cout << j << endl;
      }
      original = j;
      return false;
    }

    void filehandler::add_data_error(int error) { RepairReport["data_errors"].push_back(error); }

    // -----------------------------------------------------------------------------------------------Process functions


    void filehandler::addfeatureReport(json &report) { RepairReport["features"].push_back(report); }

    void filehandler::post_processing_child_parrent(json &CO) {
      vector<json> newFreport;
      json features = RepairReport["features"];

      json fr;
      vector<json> children;
      for (int f = 0; f < features.size(); ++f) {
        string id = features[f]["id"];
        if (CO[id].contains("children")) {
          // it is a parent!
          vector<string> newchildren;
          for (int i = 0; i < CO[id]["children"].size(); ++i) {
            if (CO.contains((CO[id]["children"][i]))) { newchildren.push_back(CO[id]["children"][i]); }
          }
          if (newchildren.empty()) {
            // children dont excist after repairs
            CO[id].erase(CO[id].find(CO[id]["children"]));
          }
          fr[id] = features[f];
        } else {
          if (CO[id].contains("parents")) {
            // it is a child
            vector<string> newparents;
            for (int p = 0; p < CO[id]["parents"].size(); ++p) {
              if (CO.contains((CO[id]["parents"][p]))) {
                newparents.push_back(CO[id]["parents"][p]);
                json child;
                child["p"] = CO[id]["parents"][p];
                child["Primitives"] = features[f]["Primitives"];
                children.push_back(child);
              }
            }
            if (newparents.empty()) {
              // parents dont excist after repairs
              CO[id].erase(CO[id].find(CO[id]["parents"]));
              fr[id] = features[f];
            }
          } else {
            // it has no relations
            fr[id] = features[f];
          }
        }
      }

      for (int c = 0; c < children.size(); ++c) {
        for (int p = 0; p < children[c]["Primitives"].size(); ++p) {
          fr[(children[c]["p"])]["Primitives"].push_back(children[c]["Primitives"][p]);
        }
      }

      for (const auto &item: fr.items()) { newFreport.push_back(item.value()); }

      RepairReport["features"] = newFreport;
    }

    void filehandler::Sort_features() {
      json out;
      json features = RepairReport["features"];

      out["features_repaired"] = json::array();

      // get all the repairs
      set<int> all_ISO_r3pairs;
      set<string> all_ADD_r3pairs;

      unordered_map<string, array<int, 2>> featureCounts;
      unordered_map<string, array<int, 2>> primitiveCounts;

      for (auto &feature: features) {
        bool featureRepaired = false;
        set<int> ISO_Repairs_Feature;
        set<string> ADD_Repairs_Feature;
        for (const auto &primitive: feature["Primitives"]) {
          string type = primitive["type"];
          bool primitiveRepaired = primitive["repaired"];
          primitiveCounts[type][0]++;
          primitiveCounts[type][1] += primitiveRepaired;

          if (primitiveRepaired) {
            featureRepaired = true;
            for (const auto &repair: primitive["repairs"]) {
              for (const auto &repairDone: repair["repairs_done"]) {
                if (repairDone["code"].is_number()) {
                  all_ISO_r3pairs.insert(int(repairDone["code"]));
                  ISO_Repairs_Feature.insert(int(repairDone["code"]));
                } else {
                  all_ADD_r3pairs.insert(repairDone["code"]);
                  ADD_Repairs_Feature.insert(repairDone["code"]);
                }
              }
            }
          }
        }
        if (!feature.contains("type")) {
          //TODO very buggy solution for  problem: find out how this can be empty
          continue;
        }
        featureCounts[feature["type"]][0]++;
        featureCounts[feature["type"]][1] += featureRepaired;
        if (featureRepaired) {
          out["features_repaired"].push_back(feature["id"]);
          feature["repaired"] = true;
        }
        feature["all_Geomr3pairs"] = ISO_Repairs_Feature;
        feature["all_UseCaser3pairs"] = ADD_Repairs_Feature;
      }

      out["features"] = features;

      for (const auto &[type, counts]: featureCounts) {
        json featureOverview = {
                {"type",     type},
                {"total",    counts[0]},
                {"repaired", counts[1]}
        };
        out["features_overview"].push_back(featureOverview);
      }

      for (const auto &[type, counts]: primitiveCounts) {
        json primitiveOverview = {
                {"type",     type},
                {"total",    counts[0]},
                {"repaired", counts[1]}
        };
        out["primitives_overview"].push_back(primitiveOverview);
      }

      if (!out["features_repaired"].empty()) { out["repaired"] = true; }
      else { out["repaired"] = false; }

      RepairReport.update(out);
    }

    void filehandler::update_metadata(json &j) {
    }


    // ------------------------------------------------------------------------------------------------Output functions

    void filehandler::write_report() {
      // -- WRITE Report json --
      if (FILETYPE == "obj") { ChangeReportBoundaries(); };

      bool duplicates;

      //TODO do i keep this
      vector<int> newids = VERTICES.get_duplicates(duplicates);
      if (duplicates) {
        RepairReport["Changed ids due to duplicates"] = newids;
      }

      std::ofstream r(ReportNAME);
//      r << setw(4) << RepairReport << std::endl;
      r << RepairReport << std::endl;
    }


    void filehandler::ChangeReportBoundaries() {
      // If file type is 1 based instead of 0 based change all boundaries with += 1 in report
      for (auto &f: RepairReport["features"]) {
        for (auto &p: f["Primitives"]) {
          for (auto &r: p["repairs"]) {
            for (auto &d: r["repairs_done"]) {
              if (d.contains("boundary_before")){
                recursive_increment(d["boundary_before"]);
              }
              if (d.contains("boundary_now")){
                recursive_increment(d["boundary_now"]);
              }
            }
          }
        }
      }
    }

    void filehandler::OBJ_RingError(json &j) {
      if (RepairReport.contains("OBJ101102")) {
        // auto& ringRepairs = RepairReport["OBJ101102"];

        for (auto &repair: RepairReport["OBJ101102"]) {
          set<int> boundary = repair["boundary_before"][0];
          bool found = false;

          for (auto &co: j["CityObjects"].items()) {
            for (int i = 0; i < co.value()["geometry"].size(); ++i) {
              auto &geom = co.value()["geometry"][i];
              set<int> vertices;
              get_boundary_set(geom["boundaries"], vertices);

              int count;
              count = ranges::count_if(vertices, [&](int elem) { return boundary.contains(elem); });

              if (count > 1) {
                found = true;
                foundRepair(repair, co.key(), i);
                break;
              }
            }

            if (found) { break; }
          }

          if (!found) { notfoundRepair(repair); }
        }

        RepairReport.erase("OBJ101102");
      }
    }

    void filehandler::foundRepair(const json &repair, const std::string &coKey, int i) {
      for (auto &feature: RepairReport["features"]) {
        if (feature["id"] == coKey) {
          auto &primitiveRepairs = feature["Primitives"][i]["repairs"];

          if (!primitiveRepairs.empty() && primitiveRepairs[0]["round"] == -1) {
            primitiveRepairs[0]["repairs_done"].push_back(repair);
          } else {
            json add{
                    {"kind_of_repair", "RingErrors"},
                    {"round",          -1},
                    {"repairs_done",   json::array({repair})}
            };
            primitiveRepairs.insert(primitiveRepairs.begin(), add);
          }
          break;
        }
      }
    }

    void filehandler::notfoundRepair(const json &repair) {
      auto &notFoundFeature = RepairReport["features"][-1];

      if (notFoundFeature["id"] == "not found") {
        notFoundFeature["Repairs"].push_back(repair);

        if (!notFoundFeature["all_Geomr3pairs"].contains(repair["code"])) {
          notFoundFeature["all_Geomr3pairs"].push_back(repair["code"]);
        }
      } else {
        json notFound{
                {"id",                 "Not found"},
                {"repaired",           true},
                {"type",               "Building"},
                {"Repairs",            json::array({repair})},
                {"all_UseCaser3pairs", json::array()},
                {"all_Geomr3pairs",    json::array({repair["code"]})}
        };

        RepairReport["features"].push_back(notFound);
      }
    }


    void filehandler::write_output(json &j) {
      j["vertices"] = VERTICES.get_verticeslistEnd();
      j["transform"]["scale"] = VERTICES.get_scale();

      if (j["vertices"] == original["vertices"] && j["CityObjects"] == original["CityObjects"] && !RepairReport.contains("OBJ101102")) {
        std::cerr << "No repairs done, so no 'repaired' file will be written" << std::endl;
        RepairReport["output_file"] = "Not written, cause no repairs";
        write_report();
        return;
      }
      RepairReport["output_file"] = OutNAME;
      RepairReport["output_file_type"] = FILETYPE;
      std::ofstream o(OutNAME);

      if (FILETYPE == "json") { write_CityJSON(o, j); }
      else if (FILETYPE == "obj") {
        write_OBJ(o, j);
        OBJ_RingError(j);
      } else { std::cerr << "I should never come here, please let me know when this error occurs" << endl; }
      write_report();
    }

    void filehandler::write_CityJSON(std::ofstream &o, json &j) {
      add_AUTOr3pair_stamp(j);
      o << j << std::endl;
    }

    void filehandler::add_AUTOr3pair_stamp(json &j) {
      j["AUTOr3pair"]["time"] = RepairReport["time"];
      j["AUTOr3pair"]["features_repaired"] = RepairReport["features_overview"];
      j["AUTOr3pair"]["primitives_repaired"] = RepairReport["primitives_overview"];
      j["AUTOr3pair"]["more_information"]["How"] =
              "this file is repaired by AUTOr3pair, more info about the repair can be found in the report";
      j["AUTOr3pair"]["more_information"]["Report_name"] = ReportNAME;
      j["AUTOr3pair"]["more_information"]["Want_to_know_more"] =
              "More information can be found on how the repair is done can be found on github";
      j["AUTOr3pair"]["more_information"]["Web:Github"] = "https://github.com/Lkeurentjes/AUTOr3pair";
      j["AUTOr3pair"]["more_information"]["Written by"] = "Lisa Keurentjes";

      if (STANDARDS["OutputParameters"]["AddAttribute"]) {
        for (auto &f: RepairReport["features"]) {
          // TODO buggy solution has to do with sort features bug
          if (!f.contains("all_Geomr3pairs")) {
            continue;
          }
          if (!f.contains("all_UseCaser3pairs")) {
            continue;
          }
          j["CityObjects"][f["id"]]["AUTOr3pair"]["ISO_RepairsDone"] = f["all_Geomr3pairs"];
          j["CityObjects"][f["id"]]["AUTOr3pair"]["UseCase_RepairsDone"] = f["all_UseCaser3pairs"];
        }
      }
    }

    void filehandler::write_OBJ(std::ofstream &o, json &j) {
      // better points
      vector<Point3> points;
      vector<double> scale = j["transform"]["scale"];
      for (const auto &v: j["vertices"]) {
        double x = static_cast<double>(v[0]) * scale[0];
        double y = static_cast<double>(v[1]) * scale[1];
        double z = static_cast<double>(v[2]) * scale[2];
        Point3 p(x, y, z);
        points.push_back(p);
      }


      vector<vector<int>> faces;
      for (const auto &[key, value]: j["CityObjects"].items()) {
        if (!j["CityObjects"][key].contains("geometry")) {
          continue;
        }
        string geomtype = j["CityObjects"][key]["geometry"][0]["type"];
        if (geomtype == "MultiSurface" or geomtype == "CompositeSurface") {
          for (const auto &f: j["CityObjects"][key]["geometry"][0]["boundaries"]) { faces.push_back(f[0]); }
        } else if (geomtype == "Solid") {
          for (const auto &shell: j["CityObjects"][key]["geometry"][0]["boundaries"]) {
            for (const auto &f: shell) { faces.push_back(f[0]); }
          }
        } else if (geomtype == "MultiSolid" or geomtype == "CompositeSolid") {
          for (const auto &solid: j["CityObjects"][key]["geometry"][0]["boundaries"]) {
            for (const auto &shell: solid) { for (const auto &f: shell) { faces.push_back(f[0]); }}
          }
        }
      }
      CGAL::IO::write_OBJ(o, points, faces);
    }
} // AUTOr3pair
