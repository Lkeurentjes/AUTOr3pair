
// Copyright (c) 2024. // made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#ifndef CITYOBJECT_CPP_USECASER3PAIR_H
#define CITYOBJECT_CPP_USECASER3PAIR_H

#include "../thirdparty/nlohmann-json/json.hpp"
using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
    // Semantics
    int get_surface_index(json& surfaces, const string& surface_type);
    int getvalue(vector<int>& face, int& ground, int& roof, int& wall);
    void semanticsRepair(json& geometry);

    // triangulation/CFD
    void triangulate(json& geometry);
    vector<vector<vector<int>>> triangulateShell(vector<vector<vector<int>>>& shell);

} // AUTOr3pair

#endif //CITYOBJECT_CPP_USECASER3PAIR_H
