// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef AUTOMATIC_REPAIR_V1_GEOMETRY_H
#define AUTOMATIC_REPAIR_V1_GEOMETRY_H

#include "../thirdparty/nlohmann-json/json.hpp"
#include "DefenitionsCGAL.h"


using json = nlohmann::json;
using namespace std;

// needed for FacesSMT:
struct VectorHash {
    std::size_t operator()(const std::vector<int>& v) const {
      std::size_t seed = v.size();
      for (auto& i : v) {
        seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      }
      return seed;
    }
};

namespace AUTOr3pair {

    class Geometry {
    private:

    protected:
        string id;
        json tu3djson;
        json OGtu3djson;
        bool semantics = false;
        bool material = false;
        bool texture = false;
        unordered_map<vector<int>, json, VectorHash> FacesSMT;
        vector<pair<Nef_polyhedron, vector<int>>> SMTnefs;
        vector<json> deletedsurfaces; // list of json points: sementics
        json::array_t Iso_Repairs;
        string type;
        bool valid = false;
        bool UseCasevalid = false;
        bool triangulated = false;

        //helper
        void emptyRepairsNeeded();
        static bool contains(vector<int> listA, vector<int> listB);
        void update_vertices();

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
        json RepairsNeeded; //todo get this back to protected maybe?

        // Constructor
        Geometry(string name, int index, json geometry);
        Geometry(string name, json Tu3djson, unordered_map<vector<int>, json, VectorHash> FaceMap);

        // Test_repairs
        void val3dityReport();

        //output
        json getCityjsonObject();
        bool getSplitfeatures(vector<json>& features);
        bool typechange(json& tu3d);
        json::array_t get_ISO_remaining();
        string get_type();
        bool isvalid();
        bool isUCvalid();
        bool istriangulated();
        bool isSMT();
        bool isempty();
        unordered_map<vector<int>, json, VectorHash> getFacesSMT();

        //helper for semantics
        static void startFacesSMT();
        static void writeFacesSMT();
        void SMTassigner(vector<int>& face);
        void SMTassigner(vector<vector<int>>& faces);
        void SMTassigner(vector<vector<vector<int>>>& faces);
        void SMTassigner(vector<vector<vector<vector<int>>>>& faces);
        void SMTassigner(vector<vector<vector<vector<vector<int>>>>>& faces);
        void SMTassigner(vector<vector<vector<vector<vector<vector<int>>>>>>& faces);

        // geometric repairs
        static vector<json> RepairRingErrors();
        static vector<json> RepairPolyErrors();
        static vector<json> RepairShellErrors();
        static vector<json> RepairSolidErrors();
        static vector<json> RepairSolidIErrors();
        static vector<json> RepairGlobal();

        // use case repairs
        vector<json> RepairSemantics();
        vector<json> RepairMesh();
    };

} // AUTOr3pair

#endif //AUTOMATIC_REPAIR_V1_GEOMETRY_H
