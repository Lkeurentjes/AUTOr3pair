// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef AUTOMATIC_REPAIR_V1_MULTISOLID_H
#define AUTOMATIC_REPAIR_V1_MULTISOLID_H

#include "../thirdparty/nlohmann-json/json.hpp"
#include "Geometry.h"
#include "DefenitionsCGAL.h"


using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {

    class MultiSolid : public Geometry{
        using Geometry::Geometry;
    public:
        void startFacesSMT();
        void writeFacesSMT();

        vector<json> RepairRingErrors();
        vector<json> RepairPolyErrors();
        vector<json> RepairShellErrors();
        vector<json> RepairSolidErrors();
        vector<json> RepairGlobal();


    };

} // AUTOr3pair

#endif //AUTOMATIC_REPAIR_V1_MULTISOLID_H
