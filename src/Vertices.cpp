// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Vertices.h"
#include "DefenitionsCGAL.h"
#include "Defenitions.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
    Vertices::Vertices() {}

    Vertices::Vertices(json &jsonvertices, json &jsonscale, json &jsontranslate) {
      if (jsonscale != nullptr) { scale = {jsonscale[0], jsonscale[1], jsonscale[2]}; }
      else { scale = {1, 1, 1}; }

      if (jsontranslate != nullptr) { translate = {jsontranslate[0], jsontranslate[1], jsontranslate[2]}; }
      else { translate = {0, 0, 0}; }

      size = jsonvertices.size();

      for (int i = 0; i < size; ++i) {
        vector<double> add = {jsonvertices[i][0], jsonvertices[i][1], jsonvertices[i][2]};
        vector<double> add_scaled = {
                double(jsonvertices[i][0]) * scale[0], double(jsonvertices[i][1]) * scale[1],
                double(jsonvertices[i][2]) * scale[2]
        };
        Point3 addpt(add_scaled[0], add_scaled[1], add_scaled[2]);

        // remove duplicate and duplicvates within snap_tol
        int duplicateIndex;
        bool newvertex = CheckDuplicates(addpt, duplicateIndex);

        if (newvertex) {
          vertices_list.push_back(add);
          scaled_vertices_list.push_back(add_scaled);
          vertices_point3_List.push_back(addpt);
          kd_tree.insert(addpt);

          newids.push_back(vertices_list.size() - 1);

        } else {
          newids.push_back(duplicateIndex);
        }

      }
      size = vertices_list.size();
    }

    void Vertices::initialize(json& jsonvertices, json& jsonscale, json& jsontranslate){
      if (jsonscale != nullptr) { scale = {jsonscale[0], jsonscale[1], jsonscale[2]}; }
      else { scale = {1, 1, 1}; }

      if (jsontranslate != nullptr) { translate = {jsontranslate[0], jsontranslate[1], jsontranslate[2]}; }
      else { translate = {0, 0, 0}; }

      size = jsonvertices.size();

      int quarter = 0.25*size;
      int half = 0.5*size;
      int threequarters = 0.75*size;

      for (int i = 0; i < size; ++i) {
        if (STANDARDS["OutputParameters"]["ShowProgress"] && i == quarter) {
          std::cout << "\tAt 25% of adding the vertices" << std::endl;
        }
        if (STANDARDS["OutputParameters"]["ShowProgress"] && i == half) {
          std::cout << "\tAt 50% of adding the vertices" << std::endl;
        }
        if (STANDARDS["OutputParameters"]["ShowProgress"] && i == threequarters) {
          std::cout << "\tAt 75% of adding the vertices" << std::endl;
        }

        vector<double> add = {jsonvertices[i][0], jsonvertices[i][1], jsonvertices[i][2]};
        vector<double> add_scaled = {
                double(jsonvertices[i][0]) * scale[0], double(jsonvertices[i][1]) * scale[1],
                double(jsonvertices[i][2]) * scale[2]
        };
        Point3 addpt(add_scaled[0], add_scaled[1], add_scaled[2]);

        // remove duplicate and duplicvates within snap_tol
        int duplicateIndex;
        bool newvertex = CheckDuplicates(addpt, duplicateIndex);

        if (newvertex) {
          vertices_list.push_back(add);
          scaled_vertices_list.push_back(add_scaled);
          vertices_point3_List.push_back(addpt);
          kd_tree.insert(addpt);

          newids.push_back(vertices_list.size() - 1);

        } else {
          newids.push_back(duplicateIndex);
        }

      }
      size = vertices_list.size();
    }

    bool Vertices::CheckDuplicates(const Point3 &add, int &value) {
      // find nearest
      Neighbor_search search(kd_tree, add, 1);

      for (Neighbor_search::iterator it = search.begin(); it != search.end(); ++it) {
        //calculate distance
        double distance = std::sqrt(it->second);
        double tol = STANDARDS["Tollerances"]["snap_tol"];
        // +0.00001 is for floating point differences
        if (distance <= tol+0.0000001){
//          for (int i = 0; i < vertices_point3_List.size(); ++i) {
//            std::cout << "\t" << vertices_point3_List[i] << endl;
//          }
          // Find the index
          auto it_found = std::find(vertices_point3_List.begin(), vertices_point3_List.end(), it->first);
          if (it_found != vertices_point3_List.end()) {
            value = std::distance(vertices_point3_List.begin(), it_found);
            duplicates = true;
            return false;
          }
        }
      }
      return true;
    }


    void Vertices::printVertices() {
      std::cout << "There are " << size << " Vertices" << endl;
      std::cout << "scale factor " << scale[0] << " " << scale[1] << " " << scale[2] << endl;
      std::cout << "translation is " << translate[0] << " " << translate[1] << " " << translate[2] << endl;

      for (int i = 0; i < vertices_list.size(); ++i) {
        std::cout << "vertice " << i << " \t\t" << vertices_list[i][0] << " " << vertices_list[i][1] << " "
                  << vertices_list[i][2] << "\t\t and scaled it is " << scaled_vertices_list[i][0] << " "
                  << scaled_vertices_list[i][1] << " " << scaled_vertices_list[i][2]
                  //                      << "\t\t" << vertices_point3_List[i] << "\t\t" << vertices_point3E_List[i]
                  << endl;
      }
    }

    int Vertices::addvertex(vector<double> &add, bool scaled) {
      if (scaled) {

        Point3 p(add[0], add[1], add[2]);
        int duplicateIndex;
        if (CheckDuplicates(p, duplicateIndex)) {
          kd_tree.insert(p);
          scaled_vertices_list.push_back(add);
          vector<double> unscaled = {add[0] / scale[0], add[1] / scale[1], add[2] / scale[2]};
          vertices_list.push_back(unscaled);
          vertices_point3_List.push_back(p);
          size += 1;

          if (!vertices_point3E_List.empty()) {
            Point3E p(add[0], add[1], add[2]);
            vertices_point3E_List.push_back(p);
          }
          return size - 1 ;
        } else {
          return duplicateIndex;
        }

      } else {
        int duplicateIndex;
        vector<double> scaled = {add[0] * scale[0], add[1] * scale[1], add[2] * scale[2]};
        Point3 p(scaled[0], scaled[1], scaled[2]);
        std::cout << p << endl;
        if (CheckDuplicates(p, duplicateIndex)) {
          kd_tree.insert(p);
          vertices_list.push_back(add);
          scaled_vertices_list.push_back(scaled);
          vertices_point3_List.push_back(p);
          size += 1;

          if (!vertices_point3E_List.empty()) {
            Point3E p(scaled[0], scaled[1], scaled[2]);
            vertices_point3E_List.push_back(p);
          }

          return size - 1;
        } else {
          return duplicateIndex;
        }
      }
    }

    int Vertices::addvertex(Point3E &pt) {
      double x = to_double(pt.x());
      double y = to_double(pt.y());
      double z = to_double(pt.z());

      int duplicateIndex;
      Point3 p(x, y, z);
      if (CheckDuplicates(p, duplicateIndex)) {
        kd_tree.insert(p);
        size += 1;
        vector<double> scaled = {x, y, z};
        vector<double> unscaled = {x / scale[0], y / scale[1], z / scale[2]};
        vertices_point3E_List.push_back(pt);
        scaled_vertices_list.push_back(scaled);
        vertices_list.push_back(unscaled);
        vertices_point3_List.push_back(p);

        return size - 1;
      } else {
        return duplicateIndex;
      }
    }

    int Vertices::addvertex(Point3 &pt) {
      int duplicateIndex;
      if (CheckDuplicates(pt, duplicateIndex)) {
        kd_tree.insert(pt);
        double x = pt.x();
        double y = pt.y();
        double z = pt.z();
        vector<double> scaled = {x, y, z};
        vector<double> unscaled = {x / scale[0], y / scale[1], z / scale[2]};

        size += 1;
        vertices_point3_List.push_back(pt);
        scaled_vertices_list.push_back(scaled);
        vertices_list.push_back(unscaled);

        if (!vertices_point3E_List.empty()) {
          Point3E p(x, y, z);
          vertices_point3E_List.push_back(p);
        }
        return size - 1;
      } else {
        return duplicateIndex;
      }
    }


    vector<vector<double>> Vertices::get_verticeslist(bool scaled) {
      if (scaled) { return scaled_vertices_list; }
      else { return vertices_list; }
    }

    vector<vector<int>> Vertices::get_verticeslistEnd() {
      int Multiplymax = 0;
      for (const auto &vertex: vertices_list) {
        for (double coord: vertex) {
          int rounded_coord = static_cast<int>(round(coord));
          if (std::fabs(coord - rounded_coord) > STANDARDS["Tollerances"]["snap_tol"]) {
            int multiply_count = 0;
            double tol = STANDARDS["Tollerances"]["snap_tol"];
            while (std::fabs(coord - rounded_coord) >= tol) {
              tol *= 10;
              coord *= 10;
              rounded_coord = static_cast<int>(std::round(coord));
              ++multiply_count;
              // max 10^10 times bigger
              if (multiply_count>10){
                std::cout << "VERTICES TO INT CAME TO MAX MULTIPLIER OF 10, CHECK FILE" << endl;
                break;}
            }
          }
        }
      }

      // this is okay cause 10^0 = 1 so doEsnt change anything
      double multiplier = std::pow(10, Multiplymax);

      for (double &s: scale) {
        s /= multiplier;
      }

      vector<vector<int>> vertices_int;
      for (int i = 0; i < vertices_list.size(); ++i) {
        vertices_int.push_back({static_cast<int>(std::round(vertices_list[i][0] * multiplier)),
                                static_cast<int>(std::round(vertices_list[i][1] * multiplier)),
                                static_cast<int>(std::round(vertices_list[i][2] * multiplier))
                               });
      }

      return vertices_int;
    }


    vector<Point3> Vertices::get_verticesPoint3() {
      return vertices_point3_List;
    }

    void Vertices::makePoint3Elist() {
      for (int i = 0; i < scaled_vertices_list.size(); ++i) {
        Point3E point(scaled_vertices_list[i][0], scaled_vertices_list[i][1], scaled_vertices_list[i][2]);
        vertices_point3E_List.push_back(point);
      }
    }

    vector<Point3E> Vertices::get_verticesPoint3E() {
      if (vertices_point3E_List.empty()) { makePoint3Elist(); }
      return vertices_point3E_List;
    }

    int Vertices::get_size() { return size; }

    json Vertices::ChangeduplicateVertices(json &geometry) {
      if (duplicates) {
        for (auto &geom: geometry) {
          recursive_changer(geom["boundaries"], newids);
        }
      }

      return geometry;
    }

    vector<int> Vertices::get_duplicates(bool &duplicatesbool) {
      duplicatesbool = duplicates;
      return newids;
    }

    void Vertices::changevertex(Point3E &pt, int &index) {
      double x = to_double(pt.x());
      double y = to_double(pt.y());
      double z = to_double(pt.z());
      vector<double> scaled = {x, y, z};
      vector<double> unscaled = {x / scale[0], y / scale[1], z / scale[2]};
      vertices_point3E_List[index] = pt;
      scaled_vertices_list[index] = scaled;
      vertices_list[index] = unscaled;
      if (!vertices_point3_List.empty()) {
        Point3 p(x, y, z);
        vertices_point3_List[index] = p;
      }
    }

    void Vertices::deleteorphanVertices(json &CO) {
      vector<int> count(size, 0);
      vector<int> toDelete;
      vector<int> newidsOrphan(size, 0);

      bool geometry = false;
      for (const auto &co: CO) {
        if (!co.contains("geometry")){
          continue;
        }else{
          for (const auto &geom: co["geometry"]) {
            recursive_counter(geom["boundaries"], count);
          }
          geometry = true;
        }
      }
      if (!geometry){
        vertices_list = {};
        scaled_vertices_list = {};
        vertices_point3_List = {};
        vertices_point3E_List = {};
        return;
      }


      int nextfree = 0;
      for (int i = 0; i < count.size(); ++i) {
        if (count[i] == 0) {
          toDelete.push_back(i);
        } else {
          newidsOrphan[i] = nextfree;
          nextfree += 1;
        }
      }

      for (auto &co: CO) {
        if (!co.contains("geometry")){
          continue;
        }else{
          for (auto &geom: co["geometry"]) {
            recursive_changer(geom["boundaries"], newidsOrphan);
          }
        }

      }

      // sort from big to small to first delete highest
      sort(toDelete.begin(), toDelete.end(), greater<int>());
      for (int &d: toDelete) {
        vertices_list.erase(vertices_list.begin() + d);
        scaled_vertices_list.erase(scaled_vertices_list.begin() + d);
        // dont delete from Point3 en point3E cause it sometimes gives memory errors

      }

    }

    vector<double> Vertices::get_Geographical_extend() {
      // Initialize min and max values with large and small extremes
      double minx = numeric_limits<double>::max();
      double miny = numeric_limits<double>::max();
      double minz = numeric_limits<double>::max();
      double maxx = numeric_limits<double>::lowest();
      double maxy = numeric_limits<double>::lowest();
      double maxz = numeric_limits<double>::lowest();

      // Iterate through the list of vertices
      for (const auto &vertex: vertices_list) {
        // Update min and max for each coordinate
        minx = min(minx, vertex[0]);
        miny = min(miny, vertex[1]);
        minz = min(minz, vertex[2]);
        maxx = max(maxx, vertex[0]);
        maxy = max(maxy, vertex[1]);
        maxz = max(maxz, vertex[2]);
      }
      // translate to real world values (without the transform)
      minx = (minx * scale[0]) + translate[0];
      miny = (miny * scale[1]) + translate[1];
      minz = (minz * scale[2]) + translate[2];
      maxx = (maxx * scale[0]) + translate[0];
      maxy = (maxy * scale[1]) + translate[1];
      maxz = (maxz * scale[2]) + translate[2];
      return {minx, miny, minz, maxx, maxy, maxz};
    }

    vector<double> Vertices::get_Geographical_extendSubSet(set<int> &indexes) {
      // Initialize min and max values with large and small extremes
      double minx = numeric_limits<double>::max();
      double miny = numeric_limits<double>::max();
      double minz = numeric_limits<double>::max();
      double maxx = numeric_limits<double>::lowest();
      double maxy = numeric_limits<double>::lowest();
      double maxz = numeric_limits<double>::lowest();

      // Iterate through the list of vertices, only processing those whose index is in the 'indexes' set
      for (int i: indexes) {
        if (i < 0 || i >= vertices_list.size()) {
          // Skip invalid indices
          continue;
        }
        const auto &vertex = vertices_list[i];

        // Update min and max for each coordinate
        minx = min(minx, vertex[0]);
        miny = min(miny, vertex[1]);
        minz = min(minz, vertex[2]);
        maxx = max(maxx, vertex[0]);
        maxy = max(maxy, vertex[1]);
        maxz = max(maxz, vertex[2]);
      }
      // translate to real world values (without the transform)
      minx = (minx * scale[0]) + translate[0];
      miny = (miny * scale[1]) + translate[1];
      minz = (minz * scale[2]) + translate[2];
      maxx = (maxx * scale[0]) + translate[0];
      maxy = (maxy * scale[1]) + translate[1];
      maxz = (maxz * scale[2]) + translate[2];
      return {minx, miny, minz, maxx, maxy, maxz};
    }

    vector<double> Vertices::get_scale() {
      return scale;
    }

    vector<double> Vertices::get_Bbox(set<int> &indexes) {
      // Initialize min and max values with large and small extremes
      double minx = numeric_limits<double>::max();
      double miny = numeric_limits<double>::max();
      double minz = numeric_limits<double>::max();
      double maxx = numeric_limits<double>::lowest();
      double maxy = numeric_limits<double>::lowest();
      double maxz = numeric_limits<double>::lowest();

      // Iterate through the list of vertices, only processing those whose index is in the 'indexes' set
      for (int i: indexes) {
        if (i < 0 || i >= vertices_list.size()) {
          // Skip invalid indices
          continue;
        }
        const auto &vertex = vertices_list[i];

        // Update min and max for each coordinate
        minx = min(minx, vertex[0]);
        miny = min(miny, vertex[1]);
        minz = min(minz, vertex[2]);
        maxx = max(maxx, vertex[0]);
        maxy = max(maxy, vertex[1]);
        maxz = max(maxz, vertex[2]);
      }

      return {minx, miny, minz, maxx, maxy, maxz};
    }


    void Vertices::changevertex(Point3 &pt, int &index) {
      double x = pt.x();
      double y = pt.y();
      double z = pt.z();
      vector<double> scaled = {x, y, z};
      vector<double> unscaled = {x / scale[0], y / scale[1], z / scale[2]};
      vertices_point3_List[index] = pt;
      scaled_vertices_list[index] = scaled;
      vertices_list[index] = unscaled;
      if (!vertices_point3E_List.empty()) {
        Point3E p(x, y, z);
        vertices_point3E_List[index] = p;
      }
    }


} // AUTOr3pair
