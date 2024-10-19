// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#include "Solid.h"
#include <iostream>
#include "tu3djson_convert.h"
#include "Ringr3pair.h"
#include "Polyr3pair.h"
#include "Shellr3pair.h"
#include "Solidr3pair.h"
#include "Globalr3pair.h"
#include "Geometrytools.h"
#include "Defenitions.h"
#include "DefenitionsVal3dity.h"
#include "DefenitionsCGAL.h"
#include "Geometrytools_makeCGAL.h"


using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {

    void Solid::startFacesSMT() {
      if (FacesSMT.empty()) {
        json SMT;
        for (int i = 0; i < tu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
          for (int j = 0; j < tu3djson["features"][0]["geometry"]["boundaries"][i].size(); ++j) {
            if (tu3djson["properties"].contains("semantics")) {
              SMT["semantics"] = tu3djson["properties"]["semantics"]["values"][i][j];
            }
            if (tu3djson["properties"].contains("material")) {
              for (const auto &m: tu3djson["properties"]["material"].items()) {
                SMT["material"][m.key()] = m.value()["values"][i][j];
              }
            }
            FacesSMT[(tu3djson["features"][0]["geometry"]["boundaries"][i][j][0])] = SMT;
            Nef_polyhedron facepoly;
            vector<int> face = tu3djson["features"][0]["geometry"]["boundaries"][i][j][0];
            bool makeNef = make_2D_polySMT(face, facepoly);
            if (makeNef) {
              SMTnefs.push_back({facepoly, face});
            }
          }
        }
      }
    }

    void Solid::writeFacesSMT() {
      if (semantics) {
        tu3djson["properties"]["semantics"]["values"] = json::array();
      }
      if (material) {
        for (const auto &m: tu3djson["properties"]["material"].items()) {
          tu3djson["properties"]["material"][m.key()]["values"] = json::array();
        }
      }

      for (int i = 0; i < tu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
        //fix arrays
        if (semantics) {
          tu3djson["properties"]["semantics"]["values"].push_back(json::array());
        }
        if (material) {
          for (const auto &m: tu3djson["properties"]["material"].items()) {
            tu3djson["properties"]["material"][m.key()]["values"].push_back(json::array());
          }
        }

        // assign the values
        for (int j = 0; j < tu3djson["features"][0]["geometry"]["boundaries"][i].size(); ++j) {
          json SMT = FacesSMT[(tu3djson["features"][0]["geometry"]["boundaries"][i][j][0])];
          if (SMT.is_null()) {
            std::cerr << "Lost info of a face in the repair process" << std::endl;
          }
          if (semantics) {
            tu3djson["properties"]["semantics"]["values"][i].push_back(SMT["semantics"]);
          }
          if (material) {
            for (const auto &m: tu3djson["properties"]["material"].items()) {
              tu3djson["properties"]["material"][m.key()]["values"][i].push_back(SMT["material"][m.key()]);
            }
          }
        }
      }
    }

    vector<json> Solid::RepairRingErrors() {
      vector<json> repairs_done;
      set<vector<int>> deletedSurfaces;

      vector<vector<vector<vector<int>>>> newboundaries;
      for (int i = 0; i < tu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
        vector<vector<vector<int>>> newshell;
        for (int j = 0; j < tu3djson["features"][0]["geometry"]["boundaries"][i].size(); ++j) {
          bool repaired = false;
          vector<vector<int>> boundary = tu3djson["features"][0]["geometry"]["boundaries"][i][j];

          for (int error = 0; error < RepairsNeeded["RingErrors"].size(); ++error) {
            string test = RepairsNeeded["RingErrors"][error][1];
            test += "|";
            if (test.find(("shell=" + to_string(i) + "|face=" + to_string(j) + "|")) != string::npos) {
              repaired = true;
              json done;
              int code = RepairsNeeded["RingErrors"][error][0];
              if (code == 101) {
                // TOO FEW POINTS
                vector<vector<int>> replace101 = AUTOr3pair::Ringr3pair101(boundary);
                if (!replace101.empty()) {
                  newshell.push_back(replace101);
                }
                done["boundary_now"] = replace101;
              } else if (code == 102) {
                // CONSECUTIVE POINTS SAME
                vector<vector<int>> replace102 = AUTOr3pair::Ringr3pair102(boundary);
                newshell.push_back(replace102);
                FacesSMT[replace102[0]] = FacesSMT[boundary[0]];
                done["boundary_now"] = replace102;
              } else if (code == 103) {
                // RING NOT CLOSED
                // cannot happen with CityJSON and TU3D if CityGML or other is ever implemented
                // easy fix would be to add first vertex to last place
                std::cerr << "Gives an error 103, that should not be possible" << endl;
                repaired = false;
              } else if (STANDARDS["UseCaseRepair"]["SkipLowRepairs"]) {
                done["boundary_now"] = json::array();
                deletedSurfaces.insert({i, j});
              } else if (code == 104) {
                // RING SELF INTERSECTION
                vector<vector<int>> replace104 = AUTOr3pair::Ringr3pair104(boundary);
                done["boundary_now"] = replace104;
                if (!replace104.empty()) {
                  newshell.push_back(replace104);
                  FacesSMT[replace104[0]] = FacesSMT[boundary[0]];
                }
              };
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
          if (!repaired) { newshell.push_back(boundary); }
        }
        newboundaries.push_back(newshell);
      }
      tu3djson["features"][0]["geometry"]["boundaries"] = newboundaries;
      update_vertices();
      return repairs_done;
    }

    vector<json> Solid::RepairPolyErrors() {
      vector<json> repairs_done;
      vector<vector<vector<vector<int>>>> newboundaries;
      bool stop = false;
      for (int i = 0; i < tu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
        vector<vector<vector<int>>> newshell;
        for (int j = 0; j < tu3djson["features"][0]["geometry"]["boundaries"][i].size(); ++j) {
          bool repaired = false;
          vector<vector<int>> boundary = tu3djson["features"][0]["geometry"]["boundaries"][i][j];

          for (int error = 0; error < RepairsNeeded["PolyErrors"].size(); ++error) {
            if (stop) { break; }
            string test = RepairsNeeded["PolyErrors"][error][1];
            test += "|";
            if (test.find(("shell=" + to_string(i) + "|face=" + to_string(j) + "|")) != string::npos) {
              repaired = true;
              json done;
              int code = RepairsNeeded["PolyErrors"][error][0];
              if (STANDARDS["UseCaseRepair"]["SkipLowRepairs"]) {
                done["boundary_now"] = json::array();
              } else if (code == 201) {
                // INTERSECTION RINGS
                vector<vector<vector<int>>> replace201 = AUTOr3pair::Polyr3pair201(boundary);
                if (!replace201.empty()) {
                  for (int face = 0; face < replace201.size(); face++) {
                    newshell.push_back(replace201[face]);
                    FacesSMT[replace201[face][0]] = FacesSMT[boundary[0]];
                  }
                }
                done["boundary_now"] = replace201;
              } else if (code == 202) {
                // DUPLICATED RINGS
                repaired = false;
                //this error doesn't exist right now in val3dity, seeing same rings also intersect
                std::cerr << "Gives an error 202, that should not be possible, contact developper" << endl;
              } else if (code == 203) {
                // NON PLANAR POLYGON DISTANCE PLANE
                string info = RepairsNeeded["PolyErrors"][error][2];
                vector<vector<vector<int>>> replace203 = AUTOr3pair::Polyr3pair203(boundary, info);
                for (int face = 0; face < replace203.size(); face++) {
                  newshell.push_back(replace203[face]);
                  FacesSMT[replace203[face][0]] = FacesSMT[boundary[0]];
                }
                stop = true; // Seeing possible projection of vertices
                done["boundary_now"] = replace203;
              } else if (code == 204) {
                // NON PLANAR POLYGON NORMALS DEVIATION
                vector<vector<int>> replace204 = AUTOr3pair::Polyr3pair204(boundary);
                newshell.push_back(replace204);
                FacesSMT[replace204[0]] = FacesSMT[boundary[0]];
                done["boundary_now"] = replace204;
                stop = true; // Seeing projection of vertices
              } else if (code == 205) {
                // POLYGON INTERIOR DISCONNECTED
                vector<vector<vector<int>>> replace205 = AUTOr3pair::Polyr3pair205(boundary);
                done["boundary_now"] = replace205;
                for (int face = 0; face < replace205.size(); face++) {
                  newshell.push_back(replace205[face]);
                  FacesSMT[replace205[face][0]] = FacesSMT[boundary[0]];
                }
              } else if (code == 206) {
                // INNER RING OUTSIDE
                vector<vector<vector<int>>> replace206 = AUTOr3pair::Polyr3pair206(boundary);
                //when deleted
                if (replace206.size() == 1) {
                  newshell.push_back(replace206[0]);
                  done["boundary_now"] = replace206[0];
                } else {
                  done["boundary_now"] = replace206;
                  //when kept
                  for (int face = 0; face < replace206.size(); face++) {
                    newshell.push_back(replace206[face]);
                    FacesSMT[replace206[face][0]] = FacesSMT[boundary[0]];
                  }
                }
              } else if (code == 207) {
                // INNER RING NESTED
                vector<vector<vector<int>>> replace207 = AUTOr3pair::Polyr3pair207(boundary);

                //when deleted
                if (replace207.size() == 1) {
                  newshell.push_back(replace207[0]);
                  done["boundary_now"] = replace207[0];
                } else {
                  done["boundary_now"] = replace207;
                  //when kept
                  for (int face = 0; face < replace207.size(); face++) {
                    newshell.push_back(replace207[face]);
                    FacesSMT[replace207[face][0]] = FacesSMT[boundary[0]];
                  }
                }
              } else if (RepairsNeeded["PolyErrors"][error][0] == 208) {
                // ORIENTATION RINGS SAME
                vector<vector<int>> replace208 = AUTOr3pair::Polyr3pair208(boundary);
                newshell.push_back(replace208);
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
          if (!repaired) { newshell.push_back(boundary); }
        }
        newboundaries.push_back(newshell);
      }
      tu3djson["features"][0]["geometry"]["boundaries"] = newboundaries;
      update_vertices();
      return repairs_done;
    }

    vector<json> Solid::RepairShellErrors() {
      vector<json> repairs_done;

      vector<vector<vector<vector<int>>>> newboundaries;
      bool to_multi = false;
      bool to_surface = false;
      vector<vector<vector<vector<int>>>> add_shells;
      for (int i = 0; i < tu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
        bool repaired = false;
        bool stop = false;
        vector<vector<vector<int>>> boundary = tu3djson["features"][0]["geometry"]["boundaries"][i];
        for (int error = 0; error < RepairsNeeded["ShellErrors"].size(); ++error) {
          string test = RepairsNeeded["ShellErrors"][error][1];
          if (test.find("shell=" + to_string(i) + "|") != std::string::npos) {
            repaired = true;
            json done;
            int code = RepairsNeeded["ShellErrors"][error][0];
            if (code == 300) {
              // NOT VALID 2 MANIFOLD
              stop = true;
              if (i <= OGtu3djson["features"][0]["geometry"]["boundaries"].size()) {
                vector<vector<vector<int>>> OGboundary = OGtu3djson["features"][0]["geometry"]["boundaries"][i];
                vector<vector<vector<int>>> replace300 = AUTOr3pair::Shellr3pair300(OGboundary);
                SMTassigner(replace300);
                newboundaries.push_back(replace300);
                done["boundary_now"] = replace300;
              } else {
                done["boundary_now"] = {};
              }
              vector<vector<vector<int>>> OGboundary = OGtu3djson["features"][0]["geometry"]["boundaries"][i];
              vector<vector<vector<int>>> replace300 = AUTOr3pair::Shellr3pair300(OGboundary);
              SMTassigner(replace300);
              newboundaries.push_back(replace300);
              done["boundary_now"] = replace300;
            } else if (code == 301) {
              // TOO FEW POLYGONS
              stop = true;
              bool sameplane = false;
              vector<vector<vector<int>>> replace301 = AUTOr3pair::Shellr3pair301(boundary, sameplane);
              SMTassigner(replace301);
              if (sameplane) {
                // convert to multisurface
                newboundaries.push_back(replace301);
                SMTassigner(replace301);
                if (i == 0) {
                  //convert
                  to_surface = true;
                } else {
                  // skip/delete
                }
              } else {
                if (replace301.empty()) {
                  if (i <= OGtu3djson["features"][0]["geometry"]["boundaries"].size()) {
                    vector<vector<vector<int>>> OGboundary = OGtu3djson["features"][0]["geometry"]["boundaries"][i];
                    replace301 = AUTOr3pair::Shellr3pair300(OGboundary);
                    SMTassigner(replace301);
                    newboundaries.push_back(replace301);
                  }
                  vector<vector<vector<int>>> OGboundary = OGtu3djson["features"][0]["geometry"]["boundaries"][i];
                  replace301 = AUTOr3pair::Shellr3pair300(OGboundary);
                } else {
                  newboundaries.push_back(replace301);
                }
                done["boundary_now"] = replace301;
              }

            } else if (code == 302) {
              // SHELL NOT CLOSED
              stop = true;
              vector<vector<vector<int>>> replace302 = AUTOr3pair::Shellr3pair302(boundary);
              if (replace302.empty()) {
                if (i <= OGtu3djson["features"][0]["geometry"]["boundaries"].size()) {
                  vector<vector<vector<int>>> OGboundary = OGtu3djson["features"][0]["geometry"]["boundaries"][i];
                  replace302 = AUTOr3pair::Shellr3pair300(OGboundary);
                  newboundaries.push_back(replace302);
                }
              } else {
                newboundaries.push_back(replace302);
              }
              SMTassigner(replace302);
              done["boundary_now"] = replace302;
            } else if (code == 303) {
              // NON MANIFOLD CASE
              stop = true;
              vector<bool> vol;
              vector<vector<vector<vector<int>>>> replace303 = AUTOr3pair::Shellr3pair303(boundary, vol);
              SMTassigner(replace303);
              done["boundary_now"] = replace303;

              auto firsttrue = find(vol.begin(), vol.end(), true);
              int voladded = 0; // if None are volume keep first
              if (firsttrue != vol.end()) {
                voladded = distance(vol.begin(), firsttrue);// If first is not volume
              }
              newboundaries.push_back(replace303[voladded]);
              for (int g = 0; g < replace303.size(); ++g) {
                if ((vol[g] || STANDARDS["UseCaseRepair"]["KeepEverything"]) && g != voladded) {
                  if (i == 0 && vol[g]) {
                    if (STANDARDS["UseCaseRepair"]["Watertight"]) {
                      // pushback as Solid features
                      json feature;
                      feature["type"] = "feature";
                      feature["geometry"]["type"] = "Solid";
                      feature["geometry"]["boundaries"] = {replace303[g]};
                      feature["geometry"]["vertices"] = VERTICES.get_verticeslist();
                      tu3djson["features"].push_back(feature);
                    } else {
                      // make it MultiSolid
                      to_multi = true;
                      add_shells.push_back(replace303[g]);
                    }
                  } else if (STANDARDS["UseCaseRepair"]["Watertight"]) {
                    if (replace303[g].size()>2){ // otherwise it can never be closed
                      newboundaries.push_back(replace303[g]);
                    }
                  } else {
                    // pushback as Surface features
                    json feature;
                    feature["type"] = "feature";
                    feature["geometry"]["type"] = STANDARDS["UseCaseRepair"]["Orientation"].get<bool>()
                                                  ? "CompositeSurface" : "MultiSurface";
                    feature["geometry"]["boundaries"] = replace303[g];
                    feature["geometry"]["vertices"] = VERTICES.get_verticeslist();
                    tu3djson["features"].push_back(feature);
                  }
                }
              }
            } else if (code == 305) {
              // MULTIPLE CONNECTED COMPONENTS
              stop = true;
              vector<bool> vol;
              vector<vector<vector<vector<int>>>> replace305 = AUTOr3pair::Shellr3pair305(boundary, vol);
              SMTassigner(replace305);
              done["boundary_now"] = replace305;
              auto firsttrue = find(vol.begin(), vol.end(), true);
              if (firsttrue != vol.end()) {
                newboundaries.push_back(replace305[distance(vol.begin(), firsttrue)]);
              }
              for (int g = 0; g < replace305.size(); ++g) {
                if (g == distance(vol.begin(), firsttrue)) {
                  continue;
                }
                if (vol[g] && i == 0) {
                  if (STANDARDS["UseCaseRepair"]["Watertight"]) {
                    // pushback as Solid features
                    json feature;
                    feature["type"] = "feature";
                    feature["geometry"]["type"] = "Solid";
                    feature["geometry"]["boundaries"] = {replace305[g]};
                    feature["geometry"]["vertices"] = VERTICES.get_verticeslist();
                    tu3djson["features"].push_back(feature);
                  } else {
                    // make it MultiSolid
                    to_multi = true;
                    add_shells.push_back(replace305[g]);
                  }
                } else if (vol[g]) {
                  newboundaries.push_back(replace305[g]);
                } else {
                  // pushback as Msurf features
                  json feature;
                  feature["type"] = "feature";
                  feature["geometry"]["type"] = "MultiSurface";
                  feature["geometry"]["boundaries"] = replace305[g];
                  feature["geometry"]["vertices"] = VERTICES.get_verticeslist();
                  tu3djson["features"].push_back(feature);
                }

              }
            } else if (code == 306) {
              // SHELL SELF INTERSECTION
              stop = true;
              vector<vector<vector<int>>> replace306 = AUTOr3pair::Shellr3pair306(boundary);
              SMTassigner(replace306);
              newboundaries.push_back(replace306);
              done["boundary_now"] = replace306;
            } else if (code == 307) {
              // POLYGON WRONG ORIENTATION
              int wrongface = std::stoi(test.substr(test.find_last_of('=') + 1));
              vector<vector<vector<int>>> replace307 = AUTOr3pair::Shellr3pair307(boundary, wrongface);
              SMTassigner(replace307);
              newboundaries.push_back(replace307);
              done["boundary_now"] = replace307;
              stop = true;
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
            if (stop || to_surface || to_multi) { break; }
          }
          if (to_surface || to_multi) { break; }
        }
        if (to_surface || to_multi) { break; }
      }
      if (to_surface) {
        vector<vector<vector<int>>> newCompositeSurface;
        for (int i = 0; i < newboundaries.size(); ++i) {
          for (int j = 0; j < newboundaries[i].size(); ++j) {
            newCompositeSurface.push_back(newboundaries[i][j]);
          }
        }
        if (!STANDARDS["UseCaseRepair"]["Watertight"]) {
          tu3djson["features"][0]["geometry"]["boundaries"] = newCompositeSurface;
          tu3djson["features"][0]["geometry"]["type"] = "CompositeSurface";
        } else {
          tu3djson["features"][0]["geometry"]["boundaries"] = {};
        }

      } else if (to_multi) {
        vector<vector<vector<vector<vector<int>>>>> newMulti;
        newMulti.push_back(newboundaries);
        for (auto &s: add_shells) {
          newMulti.push_back({s});
        }
        tu3djson["features"][0]["geometry"]["boundaries"] = newMulti;
        tu3djson["features"][0]["geometry"]["type"] = "MultiSolid";
      } else {
        tu3djson["features"][0]["geometry"]["boundaries"] = newboundaries;
      }
      update_vertices();
      return repairs_done;
    }

    vector<json> Solid::RepairSolidErrors() {
      vector<json> repairs_done;
      vector<vector<vector<vector<int>>>> newboundaries;
      bool repaired = false;
      bool stop = false;
      vector<vector<vector<vector<int>>>> boundary = tu3djson["features"][0]["geometry"]["boundaries"];
      for (int error = 0; error < RepairsNeeded["SolidErrors"].size(); ++error) {
        repaired = true;
        json done;
        int code = RepairsNeeded["SolidErrors"][error][0];
        if (code == 401) {
          // INTERSECTION SHELLS
          vector<vector<vector<vector<vector<int>>>>> replace401 = AUTOr3pair::Solidr3pair401(boundary);
          SMTassigner(replace401);
          if (replace401.size() == 0) {
            tu3djson["features"][0]["geometry"]["boundaries"] = {};
          }
          if (replace401.size() == 1) {
            tu3djson["features"][0]["geometry"]["boundaries"] = replace401[0];
          } else {
            if (STANDARDS["UseCaseRepair"]["Watertight"]) {
              // split into 2 solids
              tu3djson["features"] = json::array();
              for (int i = 0; i < replace401.size(); ++i) {
                json feature;
                feature["type"] = "feature";
                feature["geometry"]["type"] = "Solid";
                feature["geometry"]["boundaries"] = replace401[i];
                feature["geometry"]["vertices"] = VERTICES.get_verticeslist();
                tu3djson["features"].push_back(feature);
              }
            } else {
              // make it MultiSolid
              tu3djson["features"][0]["geometry"]["boundaries"] = replace401;
              tu3djson["features"][0]["geometry"]["type"] = "MultiSolid";
            }
          }

          stop = true; // stop the repair process after this
          done["boundary_now"] = replace401;
        }
        if (code == 402) {
          // DUPLICATED SHELLS
          vector<vector<vector<vector<int>>>> replace402 = AUTOr3pair::Solidr3pair402(boundary);
          SMTassigner(replace402);
          tu3djson["features"][0]["geometry"]["boundaries"] = replace402;
          done["boundary_now"] = replace402;
        }
        if (code == 403) {
          // INNER SHELL OUTSIDE
          string info = RepairsNeeded["SolidErrors"][error][2];
          vector<vector<vector<vector<vector<int>>>>> replace403 = AUTOr3pair::Solidr3pair403(boundary, info);
          SMTassigner(replace403);
          if (replace403.size() == 1) {
            tu3djson["features"][0]["geometry"]["boundaries"] = replace403[0];
          } else {
            if (STANDARDS["UseCaseRepair"]["Watertight"]) {
              // split into 2 solids
              tu3djson["features"] = json::array();
              for (int i = 0; i < replace403.size(); ++i) {
                json feature;
                feature["type"] = "feature";
                feature["geometry"]["type"] = "Solid";
                feature["geometry"]["boundaries"] = replace403[i];
                feature["geometry"]["vertices"] = VERTICES.get_verticeslist();
                tu3djson["features"].push_back(feature);
              }
            } else {
              // make it MultiSolid
              tu3djson["features"][0]["geometry"]["boundaries"] = replace403;
              tu3djson["features"][0]["geometry"]["type"] = "MultiSolid";
            }
            stop = true; // stop the repair process after this
          }
          done["boundary_now"] = replace403;
        }
        if (code == 404) {
          // SOLID INTERIOR DISCONNECTED
          vector<vector<vector<vector<vector<int>>>>> replace404 = AUTOr3pair::Solidr3pair404(boundary);
          SMTassigner(replace404);
          if (STANDARDS["UseCaseRepair"]["Watertight"]) {
            // split into 2 solids
            tu3djson["features"] = json::array();
            for (int i = 0; i < replace404.size(); ++i) {
              json feature;
              feature["type"] = "feature";
              feature["geometry"]["type"] = "Solid";
              feature["geometry"]["boundaries"] = replace404[i];
              feature["geometry"]["vertices"] = VERTICES.get_verticeslist();
              tu3djson["features"].push_back(feature);
            }
          } else {
            // make it MultiSolid
            tu3djson["features"][0]["geometry"]["boundaries"] = replace404;
            tu3djson["features"][0]["geometry"]["type"] = "MultiSolid";
          }
          stop = true; // stop the repair process after this
          done["boundary_now"] = replace404;
        }
        if (code == 405) {
          // WRONG_ORIENTATION_SHELL
          string info = RepairsNeeded["SolidErrors"][error][2];
          vector<vector<vector<vector<int>>>> replace405 = AUTOr3pair::Solidr3pair405(boundary, info);
          SMTassigner(replace405);
          tu3djson["features"][0]["geometry"]["boundaries"] = replace405;
          done["boundary_now"] = replace405;
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
                      RepairsNeeded["SolidErrors"][error][1] << endl;
          }
        }
        if (stop) { break; }
      }
      update_vertices();
      return repairs_done;
    }

    vector<json> Solid::RepairGlobal() {
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\t\tRepaired Geometery " << id << " with global repair" << endl;
        std::cout << "\t\t\tTrying alpha wrap on faces" << endl;
      }
      vector<json> repairs_done;
      json done;
      done["boundary_before"] = OGtu3djson["features"][0]["geometry"]["boundaries"];

      // do alpha wrap on polygons
      vector<vector<vector<vector<int>>>> result;
      for (int i = 0; i < OGtu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
        vector<vector<vector<int>>> shell = OGtu3djson["features"][0]["geometry"]["boundaries"][i];
        vector<vector<vector<int>>> replaceAF = AUTOr3pair::Globalr3pairAlphaFaces(shell);
        if (i != 0) { replaceAF = flip_shell(replaceAF); }
        result.push_back(replaceAF);
      }
      update_vertices();
      tu3djson["features"][0]["geometry"]["boundaries"] = result;

      done["code"] = "AlphaWrapFaces";
      done["description"] = "Global alpha wrap on faces";
      done["id"] = id;
      done["boundary_now"] = result;

      val3dityReport();
      if (isvalid()) {
        if (STANDARDS["OutputParameters"]["ShowProgress"]) {
          std::cout << "\t\t\tMade alpha wrap on faces" << endl;
        }
        SMTassigner(result);
        repairs_done.push_back(done);
        return repairs_done;
      }
      // do alpha wrap on points
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\t\t\tTrying alpha wrap on points" << endl;
      }
      vector<vector<vector<vector<int>>>> result2;
      for (int i = 0; i < OGtu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
        vector<vector<vector<int>>> shell = OGtu3djson["features"][0]["geometry"]["boundaries"][i];
        vector<vector<vector<int>>> replaceAP = AUTOr3pair::Globalr3pairAlphaPoints(shell);
        if (i != 0) { replaceAP = flip_shell(replaceAP); }
        result2.push_back(replaceAP);
      }
      update_vertices();
      tu3djson["features"][0]["geometry"]["boundaries"] = result2;

      done["code"] = "AlphaWrapPoints";
      done["description"] = "Global alpha wrap on points";
      done["id"] = id;
      done["boundary_now"] = result2;


      val3dityReport();
      if (isvalid()) {
        if (STANDARDS["OutputParameters"]["ShowProgress"]) {
          std::cout << "\t\t\tMade alpha wrap on points" << endl;
        }
        SMTassigner(result2);
        repairs_done.push_back(done);
        return repairs_done;
      }

      // Do convex hull
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\t\t\tTrying convex hull on points" << endl;
      }
      vector<vector<vector<vector<int>>>> resultCH;
      for (int i = 0; i < OGtu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
        vector<vector<vector<int>>> shell = OGtu3djson["features"][0]["geometry"]["boundaries"][i];
        vector<vector<vector<int>>> replaceCH = AUTOr3pair::Globalr3pairConvexHull(shell);
        if (i != 0) { replaceCH = flip_shell(replaceCH); }
        resultCH.push_back(replaceCH);
      }
      update_vertices();
      tu3djson["features"][0]["geometry"]["boundaries"] = resultCH;

      done["code"] = "Convex hull";
      done["description"] = "Global alpha wrap on faces";
      done["id"] = id;
      done["boundary_now"] = resultCH;


      val3dityReport();
      if (isvalid()) {
        if (STANDARDS["OutputParameters"]["ShowProgress"]) {
          std::cout << "\t\t\tMade Convex hull" << endl;
        }
        SMTassigner(resultCH);
        repairs_done.push_back(done);
        return repairs_done;
      }


      // otherwise do bounding box
      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\t\t\tMade Boundingbox" << endl;
      }
      vector<vector<vector<vector<int>>>> result3;
      bool surface = false;
      for (int i = 0; i < OGtu3djson["features"][0]["geometry"]["boundaries"].size(); ++i) {
        set<int> indexesshell;
        recursive_all_indexes(OGtu3djson["features"][0]["geometry"]["boundaries"][i], indexesshell);
        vector<vector<vector<int>>> replaceBB = AUTOr3pair::Globalr3pairBoundingBox(indexesshell, surface);
        if (surface) {
          if (i == 0) {
            tu3djson["properties"]["lod"] = "0.1";
            result3.push_back(replaceBB);
          }
          break;
        } else {
          if (i == 0) {
            tu3djson["properties"]["lod"] = "1.0";
          }
          if (i != 0) { replaceBB = flip_shell(replaceBB); }
          result3.push_back(replaceBB);
        }
      }

      update_vertices();
      if (surface) {
        if (STANDARDS["UseCaseRepair"]["Watertight"]) {
          tu3djson["features"][0]["geometry"]["boundaries"] = {};
        } else {
          tu3djson["features"][0]["geometry"]["boundaries"] = result3[0];
          tu3djson["features"][0]["geometry"]["type"] = "MultiSurface";
          SMTassigner(result3);
        }
      } else {
        tu3djson["features"][0]["geometry"]["boundaries"] = result3;
        SMTassigner(result3);
      }

      done["code"] = "Boundingbox";
      done["description"] = "Did the last resort creating boundingbox per shell";
      done["id"] = id;
      done["boundary_now"] = tu3djson["features"][0]["geometry"]["boundaries"];

      repairs_done.push_back(done);
      return repairs_done;
    }


} // AUTOr3pair
