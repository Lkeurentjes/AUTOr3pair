// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#ifndef AUTOR3PAIR_POLYR3PAIR_H
#define AUTOR3PAIR_POLYR3PAIR_H

#include "../thirdparty/nlohmann-json/json.hpp"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {

    vector<vector<int>> Polyr3pair201(vector<vector<int>>& surface);
    vector<vector<int>> Polyr3pair202(vector<vector<int>>& surface); // DOESNT EXSIST RIGHT NOW
    vector<vector<vector<int>>> Polyr3pair203(vector<vector<int>>& surface, string& info);
    vector<vector<int>> Polyr3pair204(vector<vector<int>>& surface);
    vector<vector<vector<int>>> Polyr3pair205(vector<vector<int>>& surface);
    vector<vector<vector<int>>> Polyr3pair206(vector<vector<int>>& surface);
    vector<vector<vector<int>>> Polyr3pair207(vector<vector<int>>& surface);
    vector<vector<int>> Polyr3pair208(vector<vector<int>>& surface);



} // ISOr3pair

#endif //AUTOR3PAIR_POLYR3PAIR_H
