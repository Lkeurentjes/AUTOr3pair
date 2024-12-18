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
    json counter(json &geometry) {
      json out;
      set<int> vertices;

      if (geometry["type"] == "Surface") {
        for (int i = 0; i < geometry["boundaries"].size(); ++i) {
          for (int j = 0; j < geometry["boundaries"][i].size(); ++j) {
            vertices.insert(int(geometry["boundaries"][i][j]));
          }
        }
      } else if (geometry["type"] == "MultiSurface" or geometry["type"] == "CompositeSurface") {
        out["numberfaces"] = 0;
        for (int i = 0; i < geometry["boundaries"].size(); ++i) {
          out["numberfaces"] = int(out["numberfaces"]) + 1;
          for (int j = 0; j < geometry["boundaries"][i].size(); ++j) {
            for (int k = 0; k < geometry["boundaries"][i][j].size(); ++k) {
              vertices.insert(int(geometry["boundaries"][i][j][k]));
            }
          }
        }
      } else if (geometry["type"] == "Solid") {
        out["numberfaces"] = 0;
        out["numbershells"] = 0;
        for (int i = 0; i < geometry["boundaries"].size(); ++i) {
          out["numbershells"] = int(out["numbershells"]) + 1;
          for (int j = 0; j < geometry["boundaries"][i].size(); ++j) {
            out["numberfaces"] = int(out["numberfaces"]) + 1;
            for (int k = 0; k < geometry["boundaries"][i][j].size(); ++k) {
              for (int l = 0; l < geometry["boundaries"][i][j][k].size(); ++l) {
                vertices.insert(int(geometry["boundaries"][i][j][k][l]));
              }
            }
          }
        }
      } else if (geometry["type"] == "MultiSolid" or geometry["type"] == "CompositeSolid") {
        out["numberfaces"] = 0;
        out["numbershells"] = 0;
        out["numbersolids"] = 0;
        for (int i = 0; i < geometry["boundaries"].size(); ++i) {
          out["numbersolids"] = int(out["numbersolids"]) + 1;
          for (int j = 0; j < geometry["boundaries"][i].size(); ++j) {
            out["numbershells"] = int(out["numbershells"]) + 1;
            for (int k = 0; k < geometry["boundaries"][i][j].size(); ++k) {
              out["numberfaces"] = int(out["numberfaces"]) + 1;
              for (int l = 0; l < geometry["boundaries"][i][j][k].size(); ++l) {
                for (int m = 0; m < geometry["boundaries"][i][j][k][l].size(); ++m) {
                  vertices.insert(int(geometry["boundaries"][i][j][k][l][m]));
                }
              }
            }
          }
        }
      }

      out["numbervertices"] = vertices.size();
      return out;
    }

    bool all_points_on_same_plane(set<Point3> &lsPts) {
      auto it = lsPts.begin();
      // Use the first three points to calculate the normal vector of the plane
      Point3 p1 = *it++;
      Point3 p2 = *it++;
      Point3 p3 = *it++;

      // Find the first three non-collinear points
      while (CGAL::collinear(p1, p2, p3) && it != lsPts.end()) {
        p1 = p2;
        p2 = p3;
        p3 = *it++;
      }

      if (CGAL::collinear(p1, p2, p3)) {
        // All points are collinear, cannot define a plane
        return true;
      }

      Plane plane(p1, p2, p3);
      auto normal = plane.orthogonal_vector();
      double normal_length = std::sqrt(normal.squared_length());

      // Check if all other points lie on the same plane
      for (it = lsPts.begin(); it != lsPts.end(); ++it) {
        double distance = std::abs(plane.a() * it->x() + plane.b() * it->y() + plane.c() * it->z() + plane.d()) / normal_length;
        double tol = STANDARDS["Tollerances"]["snap_tol"];
        if (distance > tol) {
          return false;
        }
      }

      return true; // All points are coplanar
    };

    bool all_points_on_same_planeSMT(set<Point3E> &lsPts) {
      auto it = lsPts.begin();
      // Use the first three points to calculate the normal vector of the plane
      Point3E p1 = *it++;
      Point3E p2 = *it++;
      Point3E p3 = *it++;

      // Find the first three non-collinear points
      while (CGAL::collinear(p1, p2, p3) && it != lsPts.end()) {
        p1 = p2;
        p2 = p3;
        p3 = *it++;
      }

      if (CGAL::collinear(p1, p2, p3)) {
        // All points are collinear, cannot define a unique plane
        return true;
      }

      // Define the plane using the first three non-collinear points
      PlaneE plane(p1, p2, p3);

      // Check if all other points lie on the same plane
      for (it = lsPts.begin(); it != lsPts.end(); ++it) {
        if (!plane.has_on(*it)) {
          return false;
        }
      }

      return true; // All points are coplanar
    }


    bool all_points_on_same_plane(std::set<Point3E> &lsPts) {
      if (lsPts.size() < 3) {
        // Less than three points, trivially coplanar
        return true;
      }

      auto it = lsPts.begin();
      // Use the first three points to calculate the normal vector of the plane
      Point3E p1 = *it++;
      Point3E p2 = *it++;
      Point3E p3 = *it++;

      // Find the first three non-collinear points
      while (CGAL::collinear(p1, p2, p3) && it != lsPts.end()) {
        p1 = p2;
        p2 = p3;
        p3 = *it++;
      }

      if (CGAL::collinear(p1, p2, p3)) {
        // All points are collinear or there are less than three points to form a plane
        return true;
      }

      // Define the plane using three non-collinear points
      PlaneE plane(p1, p2, p3);

      // Check if all other points lie on the same plane
      for (it = lsPts.begin(); it != lsPts.end(); ++it) {
        if (!plane.has_on(*it)) {
          return false;
        }
      }

      return true; // All points are coplanar
    }


    int findPointIndexInPolygon(const Polygon &polygon, const Point2 &point) {
      int index = 0;
      for (auto v = polygon.vertices_begin(); v != polygon.vertices_end(); ++v) {
        if (v->x() == point.x() and v->y() == point.y()) { return index; }
        index += 1;
      }
      // if not found
      return -1;
    }

    double CalculateVolume(const Nef_polyhedron &poly) {
      CgalPolyhedronE p;
      poly.convert_to_polyhedron(p);
      Mesh sm;
      CGAL::copy_face_graph(p, sm);
      return PMP::volume(sm);
    }

    void create_convex_hull(const vector<Point3> &lsPts, CgalPolyhedron &poly) {
      // compute convex hull of non-collinear points
      CGAL::convex_hull_3(lsPts.begin(), lsPts.end(), poly);
    }

    void remove_unused_vertices(MeshE &mesh) {
      vector<Mesh::Vertex_index> to_remove;

      for (auto v: mesh.vertices()) {
        // Check if the vertex is isolated: no incident edges
        if (mesh.is_isolated(v)) {
          to_remove.push_back(v);
        }
      }

      // Remove isolated vertices
      for (auto v: to_remove) {
        mesh.remove_vertex(v);
      }

      mesh.collect_garbage();
    };

    void remove_unused_vertices(Mesh &mesh) {
      vector<Mesh::Vertex_index> to_remove;

      for (auto v: mesh.vertices()) {
        // Check if the vertex is isolated: no incident edges
        if (mesh.is_isolated(v)) {
          to_remove.push_back(v);
        }
      }

      // Remove isolated vertices
      for (auto v: to_remove) {
        mesh.remove_vertex(v);
      }

      mesh.collect_garbage();
    };

    vector<vector<int>> flip_face(vector<vector<int>> &face) {
      vector<vector<int>> newface;
      for (auto &ring: face) {
        vector<int> newring = ring;
        ranges::reverse(newring);
        newface.push_back(newring);
      }
      return newface;
    }

    vector<vector<vector<int>>> flip_shell(vector<vector<vector<int>>> &shell) {
      vector<vector<vector<int>>> newshell;
      for (int i = 0; i < shell.size(); ++i) {
        vector<vector<int>> newface;
        for (int j = 0; j < shell[i].size(); ++j) {
          vector<int> newring = shell[i][j];
          reverse(newring.begin(), newring.end());
          newface.push_back(newring);
        }
        newshell.push_back(newface);
      }
      return newshell;
    }

    bool are_points_collinear(const vector<Point3> &points) {
      if (points.size() < 3) {
        return true; // Any set of fewer than 3 points is trivially collinear
      }
      // Check all subsets of three points
      for (size_t i = 0; i < points.size() - 2; ++i) {
        for (size_t j = i + 1; j < points.size() - 1; ++j) {
          for (size_t k = j + 1; k < points.size(); ++k) {
            if (!CGAL::collinear(points[i], points[j], points[k])) {
              return false; // If any three points are not collinear, return false
            }
          }
        }
      }
      return true; // All points are collinear
    };

    void dfs(int node, const vector<vector<int>> &adjacency_list,
             vector<bool> &visited, set<int> &component) {
      visited[node] = true;
      component.insert(node);
      for (int neighbor: adjacency_list[node]) {
        if (!visited[neighbor]) {
          dfs(neighbor, adjacency_list, visited, component);
        }
      }
    }

    vector<set<int>> find_connected_components(const vector<vector<int>> &adjacency_list) {
      int n = adjacency_list.size();
      vector<bool> visited(n, false);
      vector<set<int>> connected_components;

      for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
          set<int> component;
          dfs(i, adjacency_list, visited, component);
          connected_components.push_back(component);
        }
      }
      return connected_components;
    }

    double compute_area_from_3d_polygon(const vector<Point3E> &points3) {
      // Compute the best-fit plane for the polygon
      PlaneE best_fit_plane;
      CGAL::linear_least_squares_fitting_3(points3.begin(), points3.end(), best_fit_plane, CGAL::Dimension_tag<0>());

      // Choose an axis to project to, based on the orientation of the best-fit plane's normal
      Vector3E normal = best_fit_plane.orthogonal_vector();
      double nx = CGAL::to_double(normal.x());
      double ny = CGAL::to_double(normal.y());
      double nz = CGAL::to_double(normal.z());

      // Projection based on the dominant component of the normal vector
      std::vector<Point2E> points2;
      if (nz >= nx && nz >= ny) {
        // Project onto the XY plane
        for (const auto& p : points3) {
          points2.push_back(Point2E(p.x(), p.y()));
        }
      } else if (ny >= nx && ny >= nz) {
        // Project onto the XZ plane
        for (const auto& p : points3) {
          points2.push_back(Point2E(p.x(), p.z()));
        }
      } else {
        // Project onto the YZ plane
        for (const auto& p : points3) {
          points2.push_back(Point2E(p.y(), p.z()));
        }
      }

      // Construct a 2D polygon from the projected points
      PolygonE polygon(points2.begin(), points2.end());

      // Check if the polygon is simple (non-self-intersecting)
      if (!polygon.is_simple()) {
        std::cout << "Problems" << endl;
        std::cout << polygon << endl;
        std::cerr << "Polygon is not simple!" << std::endl;
        return 0.0;
      }

      // Compute the area of the 2D polygon
      double area = CGAL::to_double(polygon.area());

      // The area might be negative depending on the orientation, so take the absolute value
      return CGAL::to_double(CGAL::abs(area));
    }

    double compute_area_from_3d_polygon(const vector<Point3> &points3) {
      // Compute the best-fit plane for the polygon
      Plane best_fit_plane;
      CGAL::linear_least_squares_fitting_3(points3.begin(), points3.end(), best_fit_plane, CGAL::Dimension_tag<0>());

      // Choose an axis to project to, based on the orientation of the best-fit plane's normal
      Vector3 normal = best_fit_plane.orthogonal_vector();
      double nx = CGAL::to_double(normal.x());
      double ny = CGAL::to_double(normal.y());
      double nz = CGAL::to_double(normal.z());

      // Projection based on the dominant component of the normal vector
      std::vector<Point2> points2;
      if (nz >= nx && nz >= ny) {
        // Project onto the XY plane
        for (const auto& p : points3) {
          points2.push_back(Point2(p.x(), p.y()));
        }
      } else if (ny >= nx && ny >= nz) {
        // Project onto the XZ plane
        for (const auto& p : points3) {
          points2.push_back(Point2(p.x(), p.z()));
        }
      } else {
        // Project onto the YZ plane
        for (const auto& p : points3) {
          points2.push_back(Point2(p.y(), p.z()));
        }
      }

      // Construct a 2D polygon from the projected points
      Polygon polygon(points2.begin(), points2.end());

      // Check if the polygon is simple (non-self-intersecting)
      if (!polygon.is_simple()) {
        std::cout << "Problems" << endl;
        std::cout << polygon << endl;
        std::cerr << "Polygon is not simple!" << std::endl;
        return 0.0;
      }

      // Compute the area of the 2D polygon
      double area = CGAL::to_double(polygon.area());

      // The area might be negative depending on the orientation, so take the absolute value
      return CGAL::to_double(CGAL::abs(area));
    }
} // AUTOr3pair
