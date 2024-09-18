// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#ifndef SOLIDR3PAIR_H
#define SOLIDR3PAIR_H

#include "../thirdparty/nlohmann-json/json.hpp"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
	vector<vector<vector<vector<int>>>> Solidr3pair401(vector<vector<vector<vector<int>>>>& solid);
	vector<vector<vector<vector<int>>>> Solidr3pair402(vector<vector<vector<vector<int>>>>& solid);
	vector<vector<vector<vector<vector<int>>>>> Solidr3pair403(vector<vector<vector<vector<int>>>>& solid, string& info);
	vector<vector<vector<vector<vector<int>>>>> Solidr3pair404(vector<vector<vector<vector<int>>>>& solid);
	vector<vector<vector<vector<int>>>> Solidr3pair405(vector<vector<vector<vector<int>>>>& solid, string& info);

} // AUTOr3pair

#endif //SOLIDR3PAIR_H
