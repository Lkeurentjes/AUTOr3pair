// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef AUTOMATIC_REPAIR_V1_VERTICES_H
#define AUTOMATIC_REPAIR_V1_VERTICES_H

#include "../thirdparty/nlohmann-json/json.hpp"
#include "DefenitionsCGAL.h"


using json = nlohmann::json;
using namespace std;


namespace AUTOr3pair {

    class Vertices {
    private:
        int size;
        KdTree kd_tree;
        vector<vector<double>> vertices_list;
        vector<vector<double>> scaled_vertices_list;
        vector<Point3> vertices_point3_List;
        vector<Point3E> vertices_point3E_List;
        vector<int> newids;
        bool duplicates = false;
        void makePoint3Elist();
        
        vector<double> scale;
        vector<double> translate;

        template <typename T>
        void recursive_counter(T& num, vector<int>& count) {
          if (num.is_number()) {
            count[num] += 1;
          }
          else {
            for (int i = 0; i < num.size(); ++i) {
              recursive_counter(num[i], count);
            }
          }
        }

        template <typename T>
        void recursive_changer(T& num, vector<int>& newidsOrphan) {
          if (num.is_number()) {
            num = newidsOrphan[num];
          }
          else {
            for (int i = 0; i < num.size(); ++i) {
              recursive_changer(num[i], newidsOrphan);
            }
          }
        }

    public:
        // constructor
        Vertices(); //for defenitions.h
        Vertices(json& jsonvertices, json& jsonscale, json& jsontranslate);
        void initialize(json& jsonvertices, json& jsonscale, json& jsontranslate);

        // Helper functions
        bool CheckDuplicates(const Point3 &add, int& value);
        json ChangeduplicateVertices(json& geometry);
        int addvertex(vector<double>& add, bool scaled= true);
        int addvertex(Point3E &pt);
        int addvertex(Point3 &pt);
        void changevertex(Point3E &pt, int& index);
        void changevertex(Point3 &pt, int& index);
        void printVertices();



        //output
        vector<int> get_duplicates(bool& duplicates);
        vector<vector<double>> get_verticeslist(bool scaled= true);
        vector<vector<int64_t>> get_verticeslistEnd();
        vector<Point3> get_verticesPoint3();
        vector<Point3E> get_verticesPoint3E();
        vector<double> get_Geographical_extend();
        vector<double> get_Geographical_extendSubSet(set<int>& indexes);
        vector<double> get_Bbox(set<int>& indexes);
        vector<double> get_scale();
        void deleteorphanVertices(json& CO);
        int get_size();



    };

} // AUTOr3pair

#endif //AUTOMATIC_REPAIR_V1_VERTICES_H
