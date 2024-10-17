// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#include "CompositeSurface.h"
#include "tu3djson_convert.h"
#include "Defenitions.h"
#include "DefenitionsVal3dity.h"
#include "DefenitionsCGAL.h"
#include "Shellr3pair.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {

    vector<json> CompositeSurface::RepairShellErrors() {
      vector<json> repairs_done;
      vector<vector<vector<int>>> newboundary;
      vector<vector<vector<vector<int>>>> add_shells;
      bool repaired = false;
      bool stop = false;
      vector<vector<vector<int>>> boundary = tu3djson["features"][0]["geometry"]["boundaries"];

      for (int error = 0; error < RepairsNeeded["ShellErrors"].size(); ++error) {
        string test = RepairsNeeded["ShellErrors"][error][1];
        repaired = true;
        json done;
        int code = RepairsNeeded["ShellErrors"][error][0];
        if (code == 300) {
          // NOT VALID 2 MANIFOLD
          stop = true;
          vector<vector<vector<int>>> OGboundary = OGtu3djson["features"][0]["geometry"]["boundaries"];
          vector<vector<vector<int>>> replace300 = AUTOr3pair::Shellr3pair300(OGboundary);
          newboundary = replace300;
          SMTassigner(replace300);
          done["boundary_now"] = replace300;
        } else if (code == 301) {
          // TOO FEW POLYGONS
          stop = true;
          // not possible

        } else if (code == 302) {
          // SHELL NOT CLOSED
          stop = true;
          // not possible
        } else if (code == 303) {
          // NON MANIFOLD CASE
          stop = true;
          vector<bool> vol;
          vector<vector<vector<vector<int>>>> replace303 = AUTOr3pair::Shellr3pair303(boundary, vol);
          SMTassigner(replace303);
          if (replace303.empty()) {

          } else if (replace303.size() == 1) {
            newboundary = replace303[0];
            done["boundary_now"] = replace303[0];
          } else {
            newboundary = replace303[0];
            done["boundary_now"] = replace303;
            for (int g = 1; g < replace303.size(); ++g) {
              if (STANDARDS["UseCaseRepair"]["Orientation"]) {
                // split into 2 composite surfaces
                json feature;
                feature["type"] = "feature";
                feature["geometry"]["type"] = "CompositeSurface";
                feature["geometry"]["boundaries"] = replace303[g];
                feature["geometry"]["vertices"] = VERTICES.get_verticeslist();
                tu3djson["features"].push_back(feature);
              } else {
                // split into 1 composite and 1 MultiSurface
                json feature;
                feature["type"] = "feature";
                feature["geometry"]["type"] = "MultiSurface";
                feature["geometry"]["boundaries"] = replace303[g];
                feature["geometry"]["vertices"] = VERTICES.get_verticeslist();
                tu3djson["features"].push_back(feature);
              }
            }
          }
        } else if (code == 305) {
          // MULTIPLE CONNECTED COMPONENTS
          stop = true;
          if (!STANDARDS["UseCaseRepair"]["Orientation"]) {
            // change to Multi Surface
            newboundary = boundary;
            tu3djson["features"][0]["geometry"]["type"] = "MultiSurface";
          } else {
            vector<bool> vol;
            vector<vector<vector<vector<int>>>> replace305 = AUTOr3pair::Shellr3pair305(boundary, vol);
            SMTassigner(replace305);
            newboundary = replace305[0];
            if (replace305.size() == 1) {
              done["boundary_now"] = replace305[0];
            } else {
              done["boundary_now"] = replace305;
              for (int g = 1; g < replace305.size(); ++g) {
                // split into multiple composite surfaces
                json feature;
                feature["type"] = "feature";
                feature["geometry"]["type"] = "CompositeSurface";
                feature["geometry"]["boundaries"] = replace305[g];
                feature["geometry"]["vertices"] = VERTICES.get_verticeslist();
                tu3djson["features"].push_back(feature);
              }
            }
          }
        } else if (code == 306) {
          // SHELL SELF INTERSECTION
          stop = true;
          vector<vector<vector<int>>> replace306 = AUTOr3pair::Shellr3pair306(boundary);
          SMTassigner(replace306);
          newboundary = replace306;
          done["boundary_now"] = replace306;
        } else if (code == 307) {
          // POLYGON WRONG ORIENTATION
          stop = true;
          int wrongface = std::stoi(test.substr(test.find_last_of('=') + 1));
          vector<vector<vector<int>>> replace307 = AUTOr3pair::Shellr3pair307(boundary, wrongface);
          SMTassigner(replace307);
          newboundary = replace307;
          done["boundary_now"] = replace307;
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
                      RepairsNeeded["ShellErrors"][error][1] << endl;
          }
        }
        if (stop) { break; }
      }

      tu3djson["features"][0]["geometry"]["boundaries"] = newboundary;
      update_vertices();
      return repairs_done;
    }
} // AUTOr3pair