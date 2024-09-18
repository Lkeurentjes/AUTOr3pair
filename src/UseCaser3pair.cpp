
// Copyright (c) 2024. // made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#include "UseCaser3pair.h"
#include "Defenitions.h"
#include "Geometrytools.h"
#include "DefenitionsCGAL.h"
#include "GeometryTools_makeCO.h"
#include "Geometrytools_makeCGAL.h"
#include "GeometrytoolsDetriangulationPoint3.h"

namespace AUTOr3pair {
    int get_surface_index(json &surfaces, const string &surface_type) {
      // Iterate through the surfaces to find an exact match
      for (size_t i = 0; i < surfaces.size(); ++i) {
        if (surfaces[i]["type"] == surface_type) {
          // Ensure the surface has no extra fields beyond "type"
          if (surfaces[i].size() == 1) {
            return i;  // Return the index if it's an exact match
          }
        }
      }

      // if not found
      json new_surface = {{"type", surface_type}};
      surfaces.push_back(new_surface);

      // Return the index of added surface
      return surfaces.size() - 1;
    }

    int getvalue(vector<int> &face, int &ground, int &roof, int &wall) {
      vector<Point3> ring;
      make_ring(face, ring);
      Vector3 result = compute_polygon_normal(ring);



      // Define the Z-axis unit vector
      Vector3 z_axis(0, 0, 1);

      // Compute the dot product between the normal vector and the Z-axis
      double dot_product = result * z_axis;

      // Calculate the angle in degrees using arccos
      double angle = std::acos(dot_product) * 180.0 / M_PI;
//      double angle = 0;

      // Return values based on the angle
      if (angle >= 0.0 && angle <= 85.0) {
        return roof; // Upward facing
      } else if (angle >= 160.0 && angle <= 180.0) {
        return ground; // Downward facing
      } else {
        return wall; // Near-horizontal or sloped
      }

      return 0;
    }

    void semanticsRepair(json &geom) {
      vector<int> face;
      if (geom.contains("semantics")) {
        int indexGroundsurface = AUTOr3pair::get_surface_index(geom["semantics"]["surfaces"], "GroundSurface");
        int indexRoofurface = AUTOr3pair::get_surface_index(geom["semantics"]["surfaces"], "RoofSurface");
        int indexWallsurface = AUTOr3pair::get_surface_index(geom["semantics"]["surfaces"], "WallSurface");


        for (int i = 0; i < geom["semantics"]["values"].size(); ++i) {
          if (geom["semantics"]["values"][i].is_array()) {
            for (int j = 0; j < geom["semantics"]["values"][i].size(); ++j) {
              if (geom["semantics"]["values"][i][j].is_array()) {
                for (int k = 0; k < geom["semantics"]["values"][i][j].size(); ++k) {
                  if (geom["semantics"]["values"][i][j][k].is_null() ||
                      STANDARDS["UseCaseRepair"]["SemanticsValidate"]) {
                    face = geom["boundaries"][i][j][k][0].get<vector<int>>();
                    geom["semantics"]["values"][i][j][k] = getvalue(face, indexGroundsurface, indexRoofurface,
                                                                    indexWallsurface);
                  } else if (geom["semantics"]["values"][i][j][k].is_number()) {
                    continue;
                  }
                }
              } else if (geom["semantics"]["values"][i][j].is_null() ||
                         STANDARDS["UseCaseRepair"]["SemanticsValidate"]) {
                face = geom["boundaries"][i][j][0].get<vector<int>>();
                geom["semantics"]["values"][i][j] = getvalue(face, indexGroundsurface, indexRoofurface,
                                                             indexWallsurface);
              } else if (geom["semantics"]["values"][i][j].is_number()) {
                continue;
              }
            }
          } else if (geom["semantics"]["values"][i].is_null() || STANDARDS["UseCaseRepair"]["SemanticsValidate"]) {
            face = geom["boundaries"][i][0].get<vector<int>>();
            geom["semantics"]["values"][i] = getvalue(face, indexGroundsurface, indexRoofurface, indexWallsurface);
          } else if (geom["semantics"]["values"][i].is_number()) {
            continue;
          }
        }
      } else {
        json j;
        j["surfaces"] = json::array({{{"type", "GroundSurface"}},
                                     {{"type", "RoofSurface"}},
                                     {{"type", "WallSurface"}}});
        int ground = 0, roof = 1, wall = 2;

        j["values"] = json::array();  // Ensure values is initialized as an array

        for (int i = 0; i < geom["boundaries"].size(); ++i) {
          if (geom["boundaries"][i][0][0].is_array()) {
            j["values"].push_back(json::array());  // Create a new array for each boundary

            for (int boundary_j = 0; boundary_j < geom["boundaries"][i].size(); ++boundary_j) {

              if (geom["boundaries"][i][boundary_j][0][0].is_array()) {
                j["values"][i].push_back(json::array());  // Create sub-array for sub-boundaries

                for (int k = 0; k < geom["boundaries"][i][boundary_j].size(); ++k) {
                  // C-MSolid case
                  face = geom["boundaries"][i][boundary_j][k][0].get<vector<int>>();
                  j["values"][i][boundary_j].push_back(getvalue(face, ground, roof, wall));
                }
              } else {
                // Solid case
                face = geom["boundaries"][i][boundary_j][0].get<vector<int>>();
                j["values"][i].push_back(getvalue(face, ground, roof, wall));
              }
            }
          } else {
            // M-Csurface case
            face = geom["boundaries"][i][0].get<vector<int>>();
            j["values"].push_back(getvalue(face, ground, roof, wall));
          }
        }
        geom["semantics"] = j;
      }

    }

    void triangulate(json &geometry) {
      // Do all the actions per shell to overcome weird intersection problems
      if (geometry["type"] == "MultiSurface" || geometry["type"] == "CompositeSurface"){
        vector<vector<vector<int>>> inshell = geometry["boundaries"].get<vector<vector<vector<int>>>>();
        vector<vector<vector<int>>> outshell = triangulateShell(inshell);
        geometry["boundaries"] = outshell;
      } else if (geometry["type"] == "Solid"){
        vector<vector<vector<vector<int>>>> solid;
        for (int i = 0; i < geometry["boundaries"].size(); ++i) {
          vector<vector<vector<int>>> inshell = geometry["boundaries"][i].get<vector<vector<vector<int>>>>();
          vector<vector<vector<int>>> outshell = triangulateShell(inshell);
          solid.push_back(outshell);
        }
        geometry["boundaries"] = solid;
      } else if (geometry["type"] == "MultiSolid" || geometry["type"] == "CompositeSolid"){
        vector<vector<vector<vector<vector<int>>>>> Msolid;
        for (int i = 0; i < geometry["boundaries"].size(); ++i) {
        vector<vector<vector<vector<int>>>> solid;
        for (int j = 0; j < geometry["boundaries"][i].size(); ++j) {
          vector<vector<vector<int>>> inshell = geometry["boundaries"][i][j].get<vector<vector<vector<int>>>>();
          vector<vector<vector<int>>> outshell = triangulateShell(inshell);
          solid.push_back(outshell);
        }
        Msolid.push_back(solid);
        }
        geometry["boundaries"] = Msolid;
      }


    }

    vector<vector<vector<int>>> triangulateShell(vector<vector<vector<int>>> &shell) {
      vector<vector<vector<int>>> newshell;
      Mesh MeshShell;
      map<Point3, int> indexes;
      make_shell(shell, MeshShell, indexes);
      if (STANDARDS["UseCaseRepair"]["Simplification"]){
        std::size_t initial_edges = MeshShell.num_edges();
        std::size_t target_edges_to_collapse = static_cast<std::size_t>(initial_edges * 0.10);  // 10% of the edges
        SMS::Edge_count_stop_predicate<Mesh> stop(target_edges_to_collapse);

        std::cout << "Before simplification: " << initial_edges << " edges." << endl;

        // Simplify the mesh
        int r = SMS::edge_collapse(MeshShell, stop);
        // Perform mesh cleanup
        PMP::remove_degenerate_edges(MeshShell);
        PMP::remove_degenerate_faces(MeshShell);

        std::cout << "After simplification: " << MeshShell.num_edges() << " edges. "
                  << r << " edges collapsed." << endl;

        // todo generalization?
      }

      if (STANDARDS["UseCaseRepair"]["RemeshSlivers"]){
        // Apply mesh smoothing

        // Constrain border vertices so they stay the same

        set<Mesh::Vertex_index> constrained_vertices;
        for (Mesh::Vertex_index v : vertices(MeshShell)) {
          if (is_border(v, MeshShell)) {
            constrained_vertices.insert(v);
          }
        }
        CGAL::Boolean_property_map<std::set<Mesh::Vertex_index>> vcmap(constrained_vertices);

        // Apply shape smoothing with 5 iterations and a small time step
        PMP::smooth_shape(MeshShell, 0.0001, CGAL::parameters::number_of_iterations(5).vertex_is_constrained_map(vcmap));

      }
      vector<vector<vector<Point3>>> outshell = get_faces(MeshShell);


      if (!STANDARDS["UseCaseRepair"]["Triangulation"]){
        vector<vector<vector<Point3>>> detrishell = detriangulate(outshell);
        newshell = get_shell(detrishell , indexes);
      }else{
        newshell = get_shell(outshell, indexes);
      }

      return newshell;
    }


} // AUTOr3pair