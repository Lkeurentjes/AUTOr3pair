// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef AUTOMATIC_REPAIR_V1_TU3DJSON_CONVERT_H
#define AUTOMATIC_REPAIR_V1_TU3DJSON_CONVERT_H
#include "../thirdparty/nlohmann-json/json.hpp"
#include "DefenitionsCGAL.h"


using json = nlohmann::json;
using namespace std;

namespace tu3djson
{
    json make_tu3djson_geometry(json Geom);
    json get_tu3djson_feature(json TU3D, int index);

    json make_CityObject_geometry(json TU3D);





};


#endif //AUTOMATIC_REPAIR_V1_TU3DJSON_CONVERT_H
