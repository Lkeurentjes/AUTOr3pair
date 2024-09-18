// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#include "CompositeSolid.h"
#include "tu3djson_convert.h"
#include "SolidInteractionr3pair.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair{
	vector<json> CompositeSolid::RepairSolidIErrors() {
		vector<json> repairs_done;
		bool repaired = false;

		vector<vector<vector<vector<vector<int>>>>> boundary = tu3djson["features"][0]["geometry"]["boundaries"];
		for (int error = 0; error < RepairsNeeded["SolidIErrors"].size(); ++error) {
			repaired = true;
			json done;
			int code = RepairsNeeded["SolidIErrors"][error][0];
			if (code == 501) {
				// INTERSECTION SOLIDS
				vector<vector<vector<vector<vector<int>>>>> replace501 = AUTOr3pair::SolidIr3pair501(boundary);
        SMTassigner(replace501);
				if (replace501.size() == 1) {
					tu3djson["features"][0]["geometry"]["boundaries"] = replace501[0];
					tu3djson["features"][0]["geometry"]["type"] = "Solid";
					done["boundary_now"] = replace501[0];
				}
				else {
					tu3djson["features"][0]["geometry"]["boundaries"] = replace501;
					done["boundary_now"] = replace501;
				}
			}
			if (code == 502) {
				// DUPLICATED SOLIDS
				vector<vector<vector<vector<vector<int>>>>> replace502 = AUTOr3pair::SolidIr3pair502(boundary);
        SMTassigner(replace502);
				if (replace502.size() == 1) {
					tu3djson["features"][0]["geometry"]["boundaries"] = replace502[0];
					tu3djson["features"][0]["geometry"]["type"] = "Solid";
					done["boundary_now"] = replace502[0];
				}
				else {
					tu3djson["features"][0]["geometry"]["boundaries"] = replace502;
					done["boundary_now"] = replace502;
				}
			}
			if (code == 503) {
				// DUPLICATED SOLIDS

				if (STANDARDS["UseCaseRepair"]["Watertight"]) {
					vector<vector<vector<vector<vector<vector<int>>>>>> replace503 = AUTOr3pair::SolidIr3pair503(
						boundary);
          SMTassigner(replace503);
					done["boundary_now"] = replace503;
					tu3djson["features"] = json::array();
					for (int i = 0; i < replace503.size(); ++i) {
						json feature;
						feature["type"] = "feature";
						if (replace503[i].size() == 1) {
							feature["geometry"]["type"] = "Solid";
							feature["geometry"]["boundaries"] = replace503[i][0];
						}
						else {
							feature["geometry"]["type"] = "CompositeSolid";
							feature["geometry"]["boundaries"] = replace503[i];
						}
						feature["geometry"]["vertices"] = VERTICES.get_verticeslist();
						tu3djson["features"].push_back(feature);
					}
				}
				else {
					tu3djson["features"][0]["geometry"]["boundaries"] = boundary;
					tu3djson["features"][0]["geometry"]["type"] = "MultiSolid";
					done["boundary_now"] = boundary;
				}
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
						RepairsNeeded["SolidIErrors"][error][1] << endl;
				}
			}
		}
		update_vertices();
		return repairs_done;
	}
} // AUTOr3pair
