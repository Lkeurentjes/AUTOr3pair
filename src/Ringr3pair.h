// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef AUTOMATIC_REPAIR_V1_RINGR3PAIR_H
#define AUTOMATIC_REPAIR_V1_RINGR3PAIR_H

#include "../thirdparty/nlohmann-json/json.hpp"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {

    vector<vector<int>> Ringr3pair101(vector<vector<int>>& surface);
    vector<vector<int>> Ringr3pair102(vector<vector<int>>& surface);
    vector<vector<int>> Ringr3pair103(vector<vector<int>>& surface); // DOESNT EXSIST RIGHT NOW
    vector<vector<int>> Ringr3pair104(vector<vector<int>>& surface);

} // AUTOr3pair

#endif //AUTOMATIC_REPAIR_V1_RINGR3PAIR_H
