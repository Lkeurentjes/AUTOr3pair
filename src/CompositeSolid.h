// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef AUTOMATIC_REPAIR_V1_COMPOSITESOLID_H
#define AUTOMATIC_REPAIR_V1_COMPOSITESOLID_H

#include "../thirdparty/nlohmann-json/json.hpp"
#include "MultiSolid.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {

    class CompositeSolid : public MultiSolid{
        //for now does all the same things as multiSolid cause same boundaries
        using MultiSolid::MultiSolid;
    public:
        vector<json> RepairSolidIErrors();
    };

} // AUTOr3pair

#endif //AUTOMATIC_REPAIR_V1_COMPOSITESOLID_H
