// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#include "Polyr3pair.h"
#include "Geometrytools.h"
#include "Defenitions.h"
#include "DefenitionsCGAL.h"
#include "GeometryTools_makeCO.h"
#include "Geometrytools_makeCGAL.h"
#include <iostream>
#include <string>
#include <regex>


using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
    vector<vector<vector<int>>> Polyr3pair201(vector<vector<int>> &surface) {
      // INTERSECTION RINGS
      vector<vector<vector<int>>> newboundaries;
      vector<Nef_polyhedron> all_rings;
      map<Point3E, int> indexes;
      make_rings(surface, all_rings, indexes);

      vector<Point3E> start_ring;
      make_ring(surface[0], start_ring);

      for (int i = 1; i < all_rings.size(); ++i) { all_rings[0] -= all_rings[i]; }

      vector<vector<vector<Point3E>>> outfaces = get_faces(all_rings[0]);
      vector<vector<vector<int>>> faces = get_shell(outfaces, indexes);
      for (int i = 0; i < faces.size(); i += 2) {
        Vector3E start = compute_polygon_normal(start_ring);
        Vector3E end = compute_polygon_normal(outfaces[0][0]);
        double dot_product = CGAL::to_double(start * end);
        if (dot_product < 0) {
          for (auto &ring: faces[i]) { reverse(ring.begin(), ring.end()); }
        }
        newboundaries.push_back(faces[i]);
      }
      return newboundaries;
    };

    vector<vector<vector<int>>> Polyr3pair203(vector<vector<int>> &surface, string &info) {
      // NON PLANAR POLYGON DISTANCE PLANE
      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      double tol = STANDARDS["Tollerances"]["planarity_d2p_tol"];
      vector<vector<vector<int>>> newsurfaces;
      vector<vector<int>> newsurface;

      // use info to find distance
      regex pattern(R"(:\s([+-]?([0-9]*[.])?[0-9]+))");
      smatch match;
      double d;
      if (std::regex_search(info, match, pattern)) {
        std::string distance_str = match[1];
        d = std::stod(distance_str);
      }

      map<Point3E, int> indexes;
      vector<Point3E> allpoints;
      MeshE triangulatedPoly;
      vector<vector<Point3E>> all_rings;

      if (d > tol * 2) {
        vector<vector<vector<int>>> shell;
        shell.push_back(surface);
        make_shell(shell, triangulatedPoly, indexes);
        vector<vector<vector<Point3E>>> faces = get_faces(triangulatedPoly);
        newsurfaces = get_shell(faces, indexes);
        return newsurfaces;

      } else {
        make_rings(surface, all_rings, allpoints, indexes);
        CgalPolyhedronE::Plane_3 best_plane = get_best_fitted_planeE(allpoints);

        vector<Point3E> correctpoints;
        map<double, Point3E> distance;
        for (int i = 0; i < allpoints.size(); ++i) {
          double d2 = CGAL::to_double(CGAL::squared_distance(allpoints[i], best_plane));
          if (d2 < (tol * tol)) { correctpoints.push_back(allpoints[i]); }
          if (distance.contains(d2)) {
            d2 += tol * 0.01;
            if (distance.contains(d2)) { d2 += tol * 0.01; } // doing it 2 times is enough cause only need the three
          }
          distance[d2] = allpoints[i];
        }

        // if the plane is too weird, find "the 3 best matching points
        if (correctpoints.size() < 3) {
          set<Point3E> correctpoint_set(correctpoints.begin(), correctpoints.end());
          for (auto it = distance.begin(); it != distance.end(); ++it) {
            if (!correctpoint_set.contains(it->second)) { correctpoints.push_back(it->second); }
            if (correctpoints.size() >= 3) { break; }
          }
        }

        CgalPolyhedronE::Plane_3 used_plane = get_best_fitted_planeE(correctpoints);

        for (int i = 0; i < surface.size(); ++i) {
          vector<int> newring;
          for (int j = 0; j < surface[i].size(); ++j) {
            double d2 = CGAL::to_double(CGAL::squared_distance(verticesP3[surface[i][j]], used_plane));
            if (d2 > (tol * tol)) {
              Point3E projection = used_plane.projection(verticesP3[surface[i][j]]);
              VERTICES.changevertex(projection, surface[i][j]);
            }
            newring.push_back(surface[i][j]);
          }
          newsurface.push_back(newring);
        }
      }
      newsurfaces.push_back(newsurface);
      return newsurfaces;
    };

    vector<vector<int>> Polyr3pair204(vector<vector<int>> &surface) {
      // NON PLANAR POLYGON NORMALS DEVIATION
      vector<Point3> verticesP3 = VERTICES.get_verticesPoint3();
      double tol = STANDARDS["Tollerances"]["planarity_n_tol"];
      vector<vector<int>> newsurface;

      map<Point3, int> indexes;
      vector<Point3> allpoints;
      vector<vector<Point3>> all_rings;
      make_rings(surface, all_rings, allpoints, indexes);


      for (int i = 0; i < all_rings.size(); ++i) {
        vector<int> newring;
        Delaunay dt(all_rings[i].begin(), all_rings[i].end());
        // Iterate over the finite facets and compute normals
        // Store angles and corresponding facets
        vector<double> angles;
        vector<Delaunay::Finite_facets_iterator> correspondingFacets;
        map<Point3, int> vertexCounts;
        // Iterate over the finite facets
        for (auto it1 = dt.finite_facets_begin(); it1 != dt.finite_facets_end(); ++it1) {
          // Access the vertices of the first facet
          Point3 p0 = it1->first->vertex((it1->second + 1) % 4)->point();
          Point3 p1 = it1->first->vertex((it1->second + 2) % 4)->point();
          Point3 p2 = it1->first->vertex((it1->second + 3) % 4)->point();

          // Compute the normal vector of the first facet (cross product of two edges)
          Vector3 normal1 = CGAL::cross_product(p1 - p0, p2 - p0);

          // Iterate over the remaining finite facets
          for (auto it2 = std::next(it1); it2 != dt.finite_facets_end(); ++it2) {
            // Access the vertices of the second facet
            Point3 q0 = it2->first->vertex((it2->second + 1) % 4)->point();
            Point3 q1 = it2->first->vertex((it2->second + 2) % 4)->point();
            Point3 q2 = it2->first->vertex((it2->second + 3) % 4)->point();

            // Compute the normal vector of the second facet (cross product of two edges)
            Vector3 normal2 = CGAL::cross_product(q1 - q0, q2 - q0);

            // Compute the dot product and the norm of the two vectors
            double dot = CGAL::to_double(normal1 * normal2);
            double norm1 = CGAL::to_double(CGAL::sqrt(normal1 * normal1));
            double norm2 = CGAL::to_double(CGAL::sqrt(normal2 * normal2));

            // Calculate the angle between the two unit normal vectors
            double angle = std::acos(dot / (norm1 * norm2));

            // Check if the angle is greater than the specified tolerance
            if (angle * 180 / PI > tol) {
              vertexCounts[p0]++;
              vertexCounts[p1]++;
              vertexCounts[p2]++;
              vertexCounts[q0]++;
              vertexCounts[q1]++;
              vertexCounts[q2]++;
            }
          }
        }
        // Create a vector of pairs from the map
        std::vector<std::pair<Point3, int>> sorted_vertexcount(vertexCounts.begin(), vertexCounts.end());

        // Sort the vector based on the values
        std::sort(sorted_vertexcount.begin(), sorted_vertexcount.end(),
                  [](const auto &a, const auto &b) { return a.second < b.second; });

        vector<Point3> correctpoints;
        for (const auto &entry: sorted_vertexcount) {
          correctpoints.push_back(entry.first);
          if (correctpoints.size() >= 3) { break; }
        }
        CgalPolyhedron::Plane_3 used_plane = get_best_fitted_plane(correctpoints);
        for (const auto &v: all_rings[i]) {
          Point3 projection = used_plane.projection(v);
          if (indexes.contains(projection)) { newring.push_back(indexes[projection]); }
          else {
            VERTICES.changevertex(projection, indexes[v]);
            newring.push_back(indexes[v]);
          }
        }
        newsurface.push_back(newring);
      }
      return newsurface;
    };

    vector<vector<vector<int>>> Polyr3pair205(vector<vector<int>> &surface) {
      // INTERIOR DISCONNECTED
      return Polyr3pair201(surface); // same method as 201
    };

    vector<vector<vector<int>>> Polyr3pair206(vector<vector<int>> &surface) {
      // INNER RING OUTSIDE
      vector<vector<vector<int>>> newboundaries;
      newboundaries.push_back({surface[0]});

      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();

      // todo change this to best plan
      PlaneE plane(verticesP3[surface[0][0]], verticesP3[surface[0][1]], verticesP3[surface[0][2]]);

      vector<Point2E> e;
      for (int i = 0; i < surface[0].size(); ++i) {
        Point2E p = plane.to_2d(verticesP3[surface[0][i]]);
        e.push_back(p);
      }
      PolygonE exterior(e.begin(), e.end());

      //check per Polygon
      for (int i = 1; i < surface.size(); ++i) {
        Point2E p = plane.to_2d(verticesP3[surface[i][0]]);
        if (exterior.has_on_bounded_side(p)) { newboundaries[0].push_back(surface[i]); }
        else {
          if (STANDARDS["UseCaseRepair"]["KeepEverything"]) {
            reverse(surface[i].begin(), surface[i].end());
            newboundaries.push_back({surface[i]});
          }
        }
      }
      return newboundaries;
    };

    vector<vector<vector<int>>> Polyr3pair207(vector<vector<int>> &surface) {
      // INNER RING NESTED
      vector<vector<vector<int>>> newboundaries;
      newboundaries.push_back({surface[0]});

      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();


      PlaneE plane(verticesP3[surface[0][0]], verticesP3[surface[0][1]], verticesP3[surface[0][2]]);

      // make polygons form the inner rings
      vector<PolygonE> InnerRings;
      for (int i = 1; i < surface.size(); ++i) {
        vector<Point2E> e;
        for (int j = 0; j < surface[i].size(); ++j) {
          Point2E p = plane.to_2d(verticesP3[surface[i][j]]);
          e.push_back(p);
        }
        PolygonE exterior(e.begin(), e.end());
        InnerRings.push_back(exterior);
      }

      //check per inner ring
      set<int> index_nested;
      for (int i = 0; i < InnerRings.size(); ++i) {
        for (int j = 0; j < InnerRings.size(); ++j) {
          Point2E p = InnerRings[j][0];
          if (InnerRings[i].has_on_bounded_side(p)) { index_nested.insert(j + 1); }
        }
      }

      //Finish the new boundary
      for (int i = 1; i < surface.size(); ++i) {
        if (index_nested.contains(i)) {
          if (STANDARDS["UseCaseRepair"]["KeepEverything"]) {
            reverse(surface[i].begin(), surface[i].end());
            newboundaries.push_back({surface[i]});
          }
        } else { newboundaries[0].push_back(surface[i]); }
      }


      return newboundaries;
    };

    vector<vector<int>> Polyr3pair208(vector<vector<int>> &surfaceOG) {
      // ORIENTATION RINGS SAME
      vector<vector<int>> boundary;
      vector<vector<int>> surface = surfaceOG;
      boundary.push_back(surface[0]);
      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      vector<vector<Point3E>> poly;
      for (int i = 0; i < surface.size(); ++i) {
        vector<Point3E> ring;
        for (int j = 0; j < surface[i].size(); ++j) { ring.push_back(verticesP3[surface[i][j]]); }
        poly.push_back(ring);
      }

      Vector3E start = compute_polygon_normal(poly[0]);

      for (int i = 1; i < surface.size(); ++i) {
        Vector3E end = compute_polygon_normal(poly[i]);
        double dot_product = CGAL::to_double(start * end);
        // if negative they point different direction
        if (dot_product > 0) {
          ranges::reverse(surface[i]);
        }
        boundary.push_back(surface[i]);
      }
      return boundary;
    };
} // ISOr3pair
