// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef AUTOMATIC_REPAIR_V1_COMPOSITESURFACE_H
#define AUTOMATIC_REPAIR_V1_COMPOSITESURFACE_H

#include "../thirdparty/nlohmann-json/json.hpp"
#include "MultiSurface.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {

    class CompositeSurface : public MultiSurface {
        //for now does all the same things as multiSurface cause same boundaries
        using MultiSurface::MultiSurface;

    public:
        vector<json> RepairShellErrors();


    };

} // AUTOr3pair

#endif //AUTOMATIC_REPAIR_V1_COMPOSITESURFACE_H
