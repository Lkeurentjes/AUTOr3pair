
// Copyright (c) 2024. // made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#include "Globalr3pair.h"
#include "Defenitions.h"
#include "Geometrytools.h"
#include "DefenitionsCGAL.h"
#include "Geometrytools_makeCGAL.h"
#include "GeometryTools_makeCO.h"
#include "GeometrytoolsDetriangulationPoint3.h"
#include "GeometrytoolsDetriangulationPoint3E.h"

namespace AUTOr3pair {


    vector<vector<vector<int>>> Globalr3pairAlphaFaces(vector<vector<vector<int>>> &shell) {
      vector<vector<vector<int>>> newshell;
      Mesh MeshShell;
      map<Point3, int> indexes;
      bool problem;
      try{
        problem = make_shell(shell, MeshShell, indexes);
      } catch (...){
        return shell;
      }
      if (!problem || !MeshShell.is_valid()){
        return shell;
      }


      // test if not sameplane
      set<Point3> all_v;
      for (const auto& entry : indexes) {
        all_v.insert(entry.first); // Add the key (Point3) to the set
      }
      if (all_points_on_same_plane(all_v)) {
        return shell;
      }
      std::cout << "I come back here" << endl;

      vector<Point3> PolygonSoupPoints;
      vector<vector<int>> PolygonSoupFaces;
      PMP::polygon_mesh_to_polygon_soup	(MeshShell,PolygonSoupPoints,PolygonSoupFaces);
      std::cout << "SIZE of faces is " << PolygonSoupFaces.size() << endl;
      std::cout << "SIZE of points is " << PolygonSoupPoints.size() << endl;
      if (PolygonSoupFaces.size() == 0 || PolygonSoupPoints.size() == 0){
        return shell;
      }

      Mesh AlphwrappedShell;
      CGAL::alpha_wrap_3(PolygonSoupPoints,PolygonSoupFaces, 1.3, 0.3, AlphwrappedShell); //-- values of Ivan
      vector<vector<vector<Point3>>> outshell = get_faces(MeshShell);
      vector<vector<vector<Point3>>> goodshell = delete_triangle_lines(outshell);
      // TODO do I want detriangulation here or is triangulated okay for global repair
      newshell = get_shell(goodshell, indexes);
      return newshell;
    }

    vector<vector<vector<int>>> Globalr3pairAlphaPoints(vector<vector<vector<int>>> &shell) {
      vector<vector<vector<int>>> newshell;
      vector<Point3> vertices;
      vector<Point3> verticesP3 = VERTICES.get_verticesPoint3();
      map<Point3, int> indexes;
      for (auto &face: shell) {
        for (auto &ring: face) {
          for (auto &v: ring) {
            vertices.push_back(verticesP3[v]);
            indexes[verticesP3[v]] = v;
          }
        }
      }

      // test if not sameplane
      set<Point3> all_v;
      for (const auto& entry : indexes) {
        all_v.insert(entry.first); // Add the key (Point3) to the set
      }
      if (all_points_on_same_plane(all_v)) {
        return shell;
      }

      Mesh wrappedMesh;
      CGAL::alpha_wrap_3(vertices, 1.3, 0.3, wrappedMesh);
      vector<vector<vector<Point3>>> outshell = get_faces(wrappedMesh);
      vector<vector<vector<Point3>>> goodshell = delete_triangle_lines(outshell);
      // TODO do I want detriangulation here or is triangulated okay for global repair
      newshell = get_shell(goodshell, indexes);
      return newshell;
    }

    vector<vector<vector<int>>> Globalr3pairBoundingBox(set<int> &points, bool& plane) {
      vector<double> bbox = VERTICES.get_Bbox(points);

      double minx = bbox[0], miny = bbox[1], minz = bbox[2];
      double maxx = bbox[3], maxy = bbox[4], maxz = bbox[5];


      if ((minx == maxx) || (miny == maxy) || minz == maxz){
        // return plane
        plane = true;
        vector<int> plane_polygon;

        if (minx == maxx) {
          // Degenerate in the x direction, return polygon in the yz plane
          vector<vector<double>> vertices = {
                  {minx, miny, minz}, {minx, maxy, minz}, {minx, maxy, maxz}, {minx, miny, maxz}
          };

          for (auto& v : vertices) {
            plane_polygon.push_back(VERTICES.addvertex(v, false));
          }

        } else if (miny == maxy) {
          // Degenerate in the y direction, return polygon in the xz plane
          vector<vector<double>> vertices = {
                  {minx, miny, minz}, {maxx, miny, minz}, {maxx, miny, maxz}, {minx, miny, maxz}
          };

          for (auto& v : vertices) {
            plane_polygon.push_back(VERTICES.addvertex(v, false));
          }

        } else if (minz == maxz) {
          // Degenerate in the z direction, return polygon in the xy plane
          vector<vector<double>> vertices = {
                  {minx, miny, minz}, {maxx, miny, minz}, {maxx, maxy, minz}, {minx, maxy, minz}
          };

          for (auto& v : vertices) {
            plane_polygon.push_back(VERTICES.addvertex(v, false));
          }
        }

        vector<vector<vector<int>>> newshell;
        newshell.push_back({plane_polygon});
        return newshell;

      }else{
        plane = false;
        vector<vector<double>> vertices;
        // 8 vertices of the cube, each vertex is a {x, y, z} tuple
        vertices.push_back({minx, miny, minz}); // Vertex 0
        vertices.push_back({maxx, miny, minz}); // Vertex 1
        vertices.push_back({maxx, maxy, minz}); // Vertex 2
        vertices.push_back({minx, maxy, minz}); // Vertex 3
        vertices.push_back({minx, miny, maxz}); // Vertex 4
        vertices.push_back({maxx, miny, maxz}); // Vertex 5
        vertices.push_back({maxx, maxy, maxz}); // Vertex 6
        vertices.push_back({minx, maxy, maxz}); // Vertex 7

        vector<int> indexes;
        for (auto& v: vertices){
          indexes.push_back(VERTICES.addvertex(v, false));
        }
        vector<vector<vector<int>>> newshell = {
                {{indexes[0], indexes[3], indexes[2], indexes[1]}}, // Bottom face (z = minz)
                {{indexes[4], indexes[5], indexes[6], indexes[7]}}, // Top face (z = maxz)
                {{indexes[0], indexes[1], indexes[5], indexes[4]}}, // Front face (y = miny)
                {{indexes[6], indexes[2], indexes[3], indexes[7]}}, // Back face (y = maxy)
                {{indexes[7], indexes[3], indexes[0], indexes[4]}}, // Left face (x = minx)
                {{indexes[1], indexes[2], indexes[6], indexes[5]}}  // Right face (x = maxx)
        };

        return newshell;

      }
    }
}