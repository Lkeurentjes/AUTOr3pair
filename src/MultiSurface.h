// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef AUTOMATIC_REPAIR_V1_MULTISURFACE_H
#define AUTOMATIC_REPAIR_V1_MULTISURFACE_H

#include "../thirdparty/nlohmann-json/json.hpp"
#include "DefenitionsCGAL.h"
#include "Geometry.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {

    class MultiSurface : public Geometry{
        using Geometry::Geometry;

    public:
        void startFacesSMT();
        void writeFacesSMT();

        vector<json> RepairRingErrors();
        vector<json> RepairPolyErrors();
        vector<json> RepairGlobal();
    };

} // AUTOr3pair

#endif //AUTOMATIC_REPAIR_V1_MULTISURFACE_H
