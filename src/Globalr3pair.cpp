
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
      vector<Point3> PolygonSoupPoints;
      vector<vector<int>> PolygonSoupFaces;
      PMP::polygon_mesh_to_polygon_soup	(MeshShell,PolygonSoupPoints,PolygonSoupFaces);
      if (PolygonSoupFaces.size() == 0 || PolygonSoupPoints.size() == 0){
        return shell;
      }

      Mesh AlphwrappedShell;
      CGAL::alpha_wrap_3(PolygonSoupPoints,PolygonSoupFaces, 1.3, 0.3, AlphwrappedShell); //-- values of Ivan
      vector<vector<vector<Point3>>> outshell = get_faces(MeshShell);
      vector<vector<vector<Point3>>> goodshell = delete_triangle_lines(outshell);
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
      newshell = get_shell(goodshell, indexes);
      return newshell;
    }

    vector<vector<vector<int>>> Globalr3pairConvexHull(vector<vector<vector<int>>> &shell) {
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

      Mesh ConvexMesh;
      CGAL::convex_hull_3(vertices.begin(), vertices.end(), ConvexMesh);
      vector<vector<vector<Point3>>> outshell = get_faces(ConvexMesh);
      vector<vector<vector<Point3>>> goodshell = delete_triangle_lines(outshell);
      newshell = get_shell(goodshell, indexes);
      return newshell;
    }

    vector<vector<vector<int>>> Globalr3pairBoundingBox(set<int> &points, bool& plane) {
      vector<Point3> allPts;
      vector<Point3> Vp3 = VERTICES.get_verticesPoint3();
      map<Point3, int> indexes;
      for (const int& index : points) {
        allPts.push_back(Vp3[index]);
        indexes[Vp3[index]] = index;
      }

      // get the box
      std::array<Point3, 8> obb_points;
      CGAL::oriented_bounding_box(allPts, obb_points, CGAL::parameters::use_convex_hull(true));

      vector<vector<vector<Point3>>> outshell;
      // check if it is single planes:
      if (CGAL::coplanar(obb_points[0], obb_points[6], obb_points[2], obb_points[4])) {
        plane = true;
        outshell = {{{obb_points[0], obb_points[6], obb_points[2], obb_points[4]}}};

      } else{
        // make box
        plane = false;
        Mesh Bbox;
        CGAL::make_hexahedron(obb_points[0], obb_points[1], obb_points[2], obb_points[3],
                              obb_points[4], obb_points[5], obb_points[6], obb_points[7], Bbox);

        outshell = get_faces(Bbox);

      }
      vector<vector<vector<int>>> newshell = get_shell(outshell, indexes);
      return newshell;
    }

}