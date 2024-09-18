
// Copyright (c) 2024. // made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#ifndef CITYOBJECT_CPP_GLOBALR3PAIR_H
#define CITYOBJECT_CPP_GLOBALR3PAIR_H

#include "../thirdparty/nlohmann-json/json.hpp"
#include <set>

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
    vector<vector<vector<int>>> Globalr3pairAlphaFaces(vector<vector<vector<int>>>& shell);
    vector<vector<vector<int>>> Globalr3pairAlphaPoints(vector<vector<vector<int>>> &shell);
    vector<vector<vector<int>>> Globalr3pairBoundingBox(set<int>& points, bool& plane);


} // AUTOr3pair

#endif //CITYOBJECT_CPP_GLOBALR3PAIR_H
