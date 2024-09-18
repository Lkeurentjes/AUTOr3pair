// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#ifndef DEFENITIONSUSECASE_H
#define DEFENITIONSUSECASE_H

inline json VISUALIZATION = json::parse(R"({
		"KeepEverything": true,
		"SkipLowRepairs": false,
		"Watertight" : false,
    "Orientation": true,
		"MergeTol": 0.1,
		"Overlap": false,
		"SemanticsValidate": false,
    "SemanticsAdd": false,
		"Simplification": false,
		"Triangulation": false,
		"RemeshSlivers" : false
		}
        )");

inline json CFD = json::parse(R"({
		"KeepEverything": false,
		"SkipLowRepairs": true,
		"Watertight" : true,
    "Orientation": false,
		"MergeTol": 0.25,
		"Overlap": false,
		"SemanticsValidate": false,
    "SemanticsAdd": false,
		"Simplification": true,
		"Triangulation": true,
		"RemeshSlivers" : true
		}
        )");

inline json ENERGYDEMAND = json::parse(R"({
		"KeepEverything": false,
		"SkipLowRepairs": true,
		"Watertight" : true,
    "Orientation": false,
		"MergeTol": 0.75,
		"Overlap": false,
		"SemanticsValidate": false,
    "SemanticsAdd": true,
		"Simplification": false,
		"Triangulation": false,
		"RemeshSlivers" : false
		}
        )");

inline json SOLARPOWER = json::parse(R"({
		"KeepEverything": true,
		"SkipLowRepairs": false,
		"Watertight" : true,
    "Orientation": true,
		"MergeTol": 0.5,
		"Overlap": false,
		"SemanticsValidate": true,
    "SemanticsAdd": true,
		"Simplification": false,
		"Triangulation": false,
		"RemeshSlivers" : false
		}
        )");

#endif //DEFENITIONSUSECASE_H
