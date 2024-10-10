// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef AUTOMATIC_REPAIR_V1_DEFENITIONS_H
#define AUTOMATIC_REPAIR_V1_DEFENITIONS_H

//CGAL & Val3dity defenitions
#include "Vertices.h"
#include <string>
#include "nlohmann-json/json.hpp"


using json = nlohmann::json;
using namespace std;

// STANDARDS
inline string VERSION = "1.0";
inline json STANDARDS = json::parse(R"({
        "Tollerances":{
          "overlap_tol": 0.001,
          "planarity_d2p_tol": 0.01,
          "planarity_n_tol": 20,
          "snap_tol": 0.001
          },
        "InputParameters":{
          "OBJgeomtype": "Solid"
        },
        "RepairDepths":{
          "MaxRepairDepth": 50,
          "TotalRepairDepth": 500
          },
        "GeometryRepair":{
          "SolveAll":true,
          "ErrorsToRepair":[]
          },
        "UseCaseRepair":{
          "KeepEverything": false,
          "SkipLowRepairs": false,
          "Watertight" : false,
          "Orientation": false,
          "MergeTol": 0.1,
          "Overlap": true,
          "SemanticsValidate": true,
          "SemanticsAdd": true,
          "Simplification": false,
          "Triangulation": false,
          "RemeshSlivers" : false
          },
        "OutputParameters":{
          "ShowProgress":true,
          "AddAttribute" : true,
          "Debugging":false
          }
        }
        )");

inline string FILENAME;
inline string FILETYPE;
inline string USERINPUTFILENAME;
inline string LOD;
inline AUTOr3pair::Vertices VERTICES;

#endif //AUTOMATIC_REPAIR_V1_DEFENITIONS_H
