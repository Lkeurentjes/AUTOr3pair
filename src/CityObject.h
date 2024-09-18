// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#ifndef AUTOMATIC_REPAIR_V1_CITYOBJECT_H
#define AUTOMATIC_REPAIR_V1_CITYOBJECT_H


#include "DefenitionsCGAL.h"
#include "../thirdparty/nlohmann-json/json.hpp"


using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {

    class CityObject {
    private:
        string id;
        string type;
        json CObject;
        json RepairReport;

        // helper
        template <typename T>
        void recursive_all_indexes(T& num, set<int>& allSet) {
          if (num.is_number()) {
            int i = num;
            allSet.insert(i);
          }
          else {
            for (int i = 0; i < num.size(); ++i) {
              recursive_all_indexes(num[i], allSet);
            }
          }
        }

    public:
        CityObject(string name, json COobject, bool repair);

        //helper
        template<typename T>
        vector<json> GeometryRepair(T geometry, int idp);

        //output
        json getCObject();
        json& getRReport();



    };

} // AUTOr3pair

#endif //AUTOMATIC_REPAIR_V1_CITYOBJECT_H
