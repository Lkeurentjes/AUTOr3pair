// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

//
// Created by lisa on 7-3-24.
//
#include <string>
#include <regex>
#include "Solidr3pair.h"
#include "Defenitions.h"
#include "Geometrytools.h"
#include "DefenitionsCGAL.h"
#include "GeometryTools_makeCO.h"
#include "Geometrytools_makeCGAL.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
    vector<vector<vector<vector<vector<int>>>>> Solidr3pair401(vector<vector<vector<vector<int>>>> &solid) {
      // INTERSECTION SHELLS
      vector<vector<vector<vector<vector<int>>>>> newsolids;

      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      vector<Nef_polyhedron> shells;
      map<Point3E, int> indexes;
      make_shells(solid, shells, indexes);

      for (int i = 1; i < shells.size(); i++) { shells[0] -= shells[i]; }

      vector<vector<vector<vector<vector<Point3E>>>>> outsolids = get_Solids(shells[0]);
      newsolids = get_Msolid(outsolids, indexes);
      return newsolids;
    }

    vector<vector<vector<vector<int>>>> Solidr3pair402(vector<vector<vector<vector<int>>>> &solid) {
      // DUPLICATED SHELLS
      vector<vector<vector<vector<int>>>> newsolid;

      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      vector<Nef_polyhedron> shells;
      map<Point3E, int> indexes;
      make_shells(solid, shells, indexes);

      set<int> duplicateshell;
      for (int i = 0; i < shells.size(); ++i) {
        if (!duplicateshell.contains(i)) {
          newsolid.push_back(solid[i]);
          for (int j = i + 1; j < shells.size(); ++j) {
            if (!(shells[i] > shells[j])) { duplicateshell.insert(j); }
          }
        }
      }
      return newsolid;
    }

    vector<vector<vector<vector<vector<int>>>>>
    Solidr3pair403(vector<vector<vector<vector<int>>>> &solid, string &info) {
      // INNER SHELL OUTSIDE
      vector<vector<vector<vector<vector<int>>>>> newsolids;
      regex re("\\d+");  // Regular expression to match digits
      smatch match;      // Object to store the match result
      regex_search(info, match, re);
      int number = stoi(match.str());

      vector<vector<vector<vector<int>>>> newsolid;
      for (int i = 0; i < solid.size(); ++i) {
        if (number != i) {
          newsolid.push_back({solid[i]});
        }
      }
      newsolids.push_back(newsolid);
      if (STANDARDS["UseCaseRepair"]["KeepEverything"]) {
        newsolids.push_back({flip_shell(solid[number])});
      }
      return newsolids;
    }

    vector<vector<vector<vector<vector<int>>>>> Solidr3pair404(vector<vector<vector<vector<int>>>> &solid) {
      // SOLID INTERIOR DISCONNECTED
      return Solidr3pair401(solid); // same method as 401
    }

    vector<vector<vector<vector<int>>>> Solidr3pair405(vector<vector<vector<vector<int>>>> &solid, string &info) {
      // WRONG_ORIENTATION_SHELL
      vector<vector<vector<vector<int>>>> newsolid;
      size_t pos = info.find('#');
      std::string numberPart = info.substr(pos + 1);
      int number = std::stoi(numberPart);
      for (int i = 0; i < solid.size(); ++i) {
        if (i == number) {
          newsolid.push_back(flip_shell(solid[i]));
        } else {
          newsolid.push_back(solid[i]);
        }
      }
      return newsolid;
    }
} // AUTOr3pair
