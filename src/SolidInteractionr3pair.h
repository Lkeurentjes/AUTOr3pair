// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#ifndef SOLIDINTERACTIONR3PAIR_H
#define SOLIDINTERACTIONR3PAIR_H

#include "../thirdparty/nlohmann-json/json.hpp"
#include "Defenitions.h"
#include "DefenitionsVal3dity.h"
#include "DefenitionsUseCase.h"
#include "DefenitionsCGAL.h"
#include "Geometrytools.h"
#include "GeometryTools_makeCO.h"
#include "Geometrytools_makeCGAL.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
	vector<vector<vector<vector<vector<int>>>>> SolidIr3pair501(vector<vector<vector<vector<vector<int>>>>>& Boundary);
	vector<vector<vector<vector<vector<int>>>>> SolidIr3pair502(vector<vector<vector<vector<vector<int>>>>>& Boundary);
	vector<vector<vector<vector<vector<vector<int>>>>>>  SolidIr3pair503(vector<vector<vector<vector<vector<int>>>>>& Boundary);

} // AUTOr3pair

#endif //SOLIDINTERACTIONR3PAIR_H
