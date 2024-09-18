
// Copyright (c) 2024. // made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#ifndef CITYOBJECT_CPP_BUILDINGPARTR3PAIR_H
#define CITYOBJECT_CPP_BUILDINGPARTR3PAIR_H

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
    json BuildingPartr3pair601(json& part1, json& part2, string& LOD);
    json BuildingPartr3pair601(json& part1, string& LOD);

} // AUTOr3pair

#endif //CITYOBJECT_CPP_BUILDINGPARTR3PAIR_H
