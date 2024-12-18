// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#include <algorithm>
#include "Defenitions.h"
#include "Geometrytools.h"
#include "GeometrytoolsDetriangulationPoint3.h"
#include "DefenitionsCGAL.h"
#include "Geometrytools_makeCGAL.h"
#include <iostream>
#include <vector>

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {

    Vector3 compute_polygon_normal(vector<Point3> &points) {
      if (points.size() < 3) {
        throw std::invalid_argument("A face must have at least 3 points");
      }
      // Initialize components of the normal
      typedef KE::FT FT;
      auto nx = 0.0, ny = 0.0, nz = 0.0;

      // Use Newell's method to calculate the normal
      std::size_t num_points = points.size();
      for (std::size_t i = 0; i < num_points; ++i) {
        const Point3 &p0 = points[i];
        const Point3 &p1 = points[(i + 1) % num_points]; // Wrap around

        // Newell's formula
        nx += (p0.y() - p1.y()) * (p0.z() + p1.z());
        ny += (p0.z() - p1.z()) * (p0.x() + p1.x());
        nz += (p0.x() - p1.x()) * (p0.y() + p1.y());
      }

      // Construct the normal vector from the components
      Vector3 normal(nx, ny, nz);

      // Normalize the normal vector (optional but recommended)
      if (normal.squared_length() > 0) {
        normal = normal / CGAL::sqrt(normal.squared_length());
      }

      return normal;
    }

    vector<vector<vector<Point3>>> detriangulate(vector<vector<vector<Point3>>> shell) {
      vector<vector<vector<Point3>>> newshell;

      // STEP 1 FIND COPLANNAR PATCHES
      vector<vector<vector<Point3>>> coplanar_groups;
      for (const auto &poly: shell) {
        vector<Point3> face = poly[0];
        bool found_group = false;
        for (auto &group: coplanar_groups) {
          // Check if the new face is coplanar with any face in the group
          if (are_faces_coplanar(group[0], face)) {
            group.push_back(face);  // Add face to the coplanar group
            found_group = true;
            break;
          }
        }
        // If no coplanar group found, create a new group
        if (!found_group) {
          coplanar_groups.push_back({face});
        }
      }

      // STEP 2 FIND CONNECTED COMPONENTS IN COPLANNAR PATCHES
      int group_id = 1;
      for (const auto &group: coplanar_groups) {

        // Step 1: Build adjacency list (graph)
        vector<vector<int>> adjacency_list = build_graph(group);
        // Step 2: Find connected components
        vector<set<int>> connected_components = find_connected_components(adjacency_list);
        // Step 3: Group triangles by components
        vector<vector<vector<Point3>>> components = group_triangles_by_components(group, connected_components);

        // STEP 3 MERGE TRIANGLES OF SAME GROUP
        for (const auto &component: components) {
          vector<vector<Point3>> result = merge_triangles(component);
          newshell.push_back(result);
        }
      }
      return newshell;
    }

    Vector3 computeTriangleNormal(const vector<Point3> &face) {
      // Cross product of two edges of the triangle
      Vector3 v1 = face[1] - face[0];
      Vector3 v2 = face[2] - face[0];
      return CGAL::cross_product(v1, v2);
    }

    bool are_faces_coplanar(const vector<Point3> &face1, const vector<Point3> &face2) {
      // Compute the normal of the first face
      Vector3 normal1 = computeTriangleNormal(face1);

      // Create the plane using the first face
      Plane plane(face1[0], normal1);

      // Check if all points of the second face lie on the same plane within a tolerance
      for (const auto &point: face2) {
        if (!is_point_on_plane(plane, point)) {
          return false;
        }
      }
      Vector3 normal2 = computeTriangleNormal(face2);
      double dot_product = CGAL::to_double(normal1 * normal2);
      if (dot_product < 0){
        return false;
      }
      return true;
    }

    bool is_point_on_plane(const Plane &plane, const Point3 &point) {
      // The point-plane equation gives a value close to zero if the point is on the plane.
      const double EPSILON = 1e-6;
      // Compute the signed distance using the plane equation
      double signed_distance = (plane.a() * point.x() +
                                plane.b() * point.y() +
                                plane.c() * point.z() + plane.d()) /
                               std::sqrt(plane.a() * plane.a() +
                                         plane.b() * plane.b() +
                                         plane.c() * plane.c());

      // Check if the signed distance is within the tolerance
      return fabs(signed_distance) < EPSILON;
    }

    bool points_almost_equal(const Point3 &p1, const Point3 &p2) {
      const double EPSILON = 1e-3;
      return (fabs(p1.x() - p2.x()) < EPSILON &&
              fabs(p1.y() - p2.y()) < EPSILON &&
              fabs(p1.z() - p2.z()) < EPSILON);
    }

    bool triangles_share_edge(const vector<Point3> &tri1, const vector<Point3> &tri2) {
      int shared_points = 0;
      for (const auto &p1: tri1) {
        for (const auto &p2: tri2) {
          if (points_almost_equal(p1, p2)) {
            ++shared_points;
          }
        }
      }
      return shared_points == 2;
    }

    vector<vector<int>> build_graph(const vector<vector<Point3>> &triangles) {
      int n = triangles.size();
      vector<vector<int>> adjacency_list(n);

      for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
          if (triangles_share_edge(triangles[i], triangles[j])) {
            adjacency_list[i].push_back(j);
            adjacency_list[j].push_back(i);
          }
        }
      }
      return adjacency_list;
    }

    vector<vector<vector<Point3>>> group_triangles_by_components(
            const vector<vector<Point3>> &triangles,
            const vector<set<int>> &connected_components) {

      vector<vector<vector<Point3>>> components;

      for (const auto &component: connected_components) {
        vector<vector<Point3>> triangle_group;
        for (int index: component) {
          triangle_group.push_back(triangles[index]);
        }
        components.push_back(triangle_group);
      }

      return components;
    }

    vector<vector<Point3>> merge_triangles(const vector<vector<Point3>> &triangles) {
      vector<vector<Point3>> result;

      // get all the (half) edges with count of half edges
      map<pair<Point3, Point3>, int> EdgesMap;
      for(const auto& tri : triangles){
        for (size_t i = 0; i < tri.size(); ++i) {
          size_t nextIndex = (i + 1) % tri.size();
          pair<Point3, Point3> OppositeHalfEdge(tri[nextIndex], tri[i]);
          if (EdgesMap.contains(OppositeHalfEdge)){
            EdgesMap[OppositeHalfEdge]+=1;
          } else{
            pair<Point3, Point3> HalfEdge(tri[i], tri[nextIndex]);
            EdgesMap[HalfEdge] = 1;
          }
        }
      }

      // Get all the Boundary edges
      vector<pair<Point3, Point3>> Outers;
      for (const auto& entry : EdgesMap) {
        const pair<Point3, Point3>& edge = entry.first;
        int count = entry.second;
        if (count == 1){
          Outers.push_back(edge);
        }
      }

      if (Outers.empty()) {
        result.push_back(triangles[0]);
        std::cerr << "No boundary edges found. Exiting." << std::endl;
        return result;
      }


      vector<Point3> Ring = {Outers[0].first, Outers[0].second};
      Point3 Current = Outers[0].second;
      Outers.erase(Outers.begin() + 0);
      vector<vector<Point3>> Rings;
      while (Outers.size() > 0){
        int del = 0;
        bool found = false;

        for (const auto& [first, second] : Outers) {

          if (first == Current){
            found = true;
            Current = second;
            if (second!= Ring[0]){
              Ring.push_back(second);
            }
            break;
          }
          del++;
        }
        if (!found){
          std::cerr << "Couldn't finish detriangulation contact developer" << endl;
          return result;
        }
        Outers.erase(Outers.begin() + del);

        if (Current == Ring[0]){
          Rings.push_back(Ring);
          if (Outers.empty()){
            break;
          }
          Ring = {Outers[0].first, Outers[0].second};
          Current = Outers[0].second;
          Outers.erase(Outers.begin() + 0);
        }

      }
      if (Rings.size() == 1){
        return Rings;
      } else{
        vector<pair<double, vector<Point3>>> areas;
        for (const auto& ring: Rings) {
          if (ring.size()>2){
            double area = compute_area_from_3d_polygon(ring);
            areas.push_back({area, ring});
          } else{
            areas.push_back({0.0, ring});
          }
        }
        std::sort(areas.begin(), areas.end(),
                  [](const std::pair<double, vector<Point3>>& a, std::pair<double, vector<Point3>>& b) {
                      return a.first > b.first;  // Sort in ascending order of area
                  });
        for (const auto& pair : areas) {
          result.push_back(pair.second);
        }
        return result;
      }
    }

} // AUTOr3pair
