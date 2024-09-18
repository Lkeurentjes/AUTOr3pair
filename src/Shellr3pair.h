// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#ifndef SHELLR3PAIR_H
#define SHELLR3PAIR_H

#include "../thirdparty/nlohmann-json/json.hpp"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
	vector<vector<vector<int>>> Shellr3pair300(vector<vector<vector<int>>>& shell);
	vector<vector<vector<int>>> Shellr3pair301(vector<vector<vector<int>>>& shell, bool& sameplane);
	vector<vector<vector<int>>> Shellr3pair302(vector<vector<vector<int>>>& shell);
	vector<vector<vector<vector<int>>>> Shellr3pair303(vector<vector<vector<int>>>& shell, vector<bool>& vol);
	vector<vector<vector<vector<int>>>> Shellr3pair305(vector<vector<vector<int>>>& shell, vector<bool>& vol);
  vector<vector<vector<vector<int>>>>  Shellr3pair306(vector<vector<vector<int>>>& shell);
	vector<vector<vector<int>>> Shellr3pair307(vector<vector<vector<int>>>& shell, int index);

} // AUTOr3pair

#endif //SHELLR3PAIR_H
