// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#include "Shellr3pair.h"
#include "Defenitions.h"
#include "Geometrytools.h"
#include "DefenitionsCGAL.h"
#include "Geometrytools_makeCGAL.h"
#include "GeometryTools_makeCO.h"
#include "GeometrytoolsDetriangulationPoint3.h"
#include "GeometrytoolsDetriangulationPoint3E.h"

namespace AUTOr3pair {
    vector<vector<vector<int>>> Shellr3pair300(vector<vector<vector<int>>> &shell) {
      vector<vector<vector<int>>> newshell;
      Mesh MeshShell;
      map<Point3, int> indexes;
      make_shell(shell, MeshShell, indexes);

      // test if not sameplane
      set<Point3> all_v;
      for (const auto &entry: indexes) {
        all_v.insert(entry.first); // Add the key (Point3) to the set
      }
      if (all_points_on_same_plane(all_v)) {
        return shell;
      }
      vector<Point3> PolygonSoupPoints;
      vector<vector<int>> PolygonSoupFaces;
      PMP::polygon_mesh_to_polygon_soup(MeshShell, PolygonSoupPoints, PolygonSoupFaces);
      if (PolygonSoupFaces.size() == 0 || PolygonSoupPoints.size() == 0) {
        return shell;
      }

      Mesh AlphwrappedShell;
      CGAL::alpha_wrap_3(PolygonSoupPoints, PolygonSoupFaces, 1.3, 0.3, AlphwrappedShell);  //-- values of Ivan
      vector<vector<vector<Point3>>> outshell = get_faces(MeshShell);
      vector<vector<vector<Point3>>> goodshell = delete_triangle_lines(outshell);

      if (!STANDARDS["UseCaseRepair"]["Triangulation"]) {
        // detriangulate
        vector<vector<vector<Point3>>> detrishell = detriangulate(goodshell);
        newshell = get_shell(detrishell, indexes);
      } else {
        newshell = get_shell(goodshell, indexes);
      }

      return newshell;
    }

    vector<vector<vector<int>>> Shellr3pair301(vector<vector<vector<int>>> &shell, bool &sameplane) {
      // TOO FEW POLYGONS
      vector<vector<vector<int>>> newshell;
      vector<Point3> verticesP3 = VERTICES.get_verticesPoint3();

      map<Point3, int> indexes;
      set<Point3> all_v;
      vector<vector<vector<Point3>>> shellP3;
      for (int i = 0; i < shell.size(); ++i) {
        vector<vector<Point3>> PolyP3;
        for (int j = 0; j < shell[i].size(); ++j) {
          vector<Point3> RingP3;
          for (int k = 0; k < shell[i][j].size(); ++k) {
            indexes[verticesP3[shell[i][j][k]]] = shell[i][j][k];
            all_v.insert(verticesP3[shell[i][j][k]]);
            RingP3.push_back(verticesP3[shell[i][j][k]]);
          }
          PolyP3.push_back(RingP3);
        }
        shellP3.push_back(PolyP3);
      }

      // check if not all points on same plane
      if (all_points_on_same_plane(all_v)) {
        sameplane = true;
        return shell;
      }

      // Try closing option from 302
      newshell = Shellr3pair302(shell);

      return newshell;
    }

    vector<vector<vector<int>>> Shellr3pair302(vector<vector<vector<int>>> &shell) {
      // SHELL NOT CLOSED
      vector<vector<vector<int>>> newshell;
      MeshE MeshShell;
      map<Point3E, int> indexes;
      make_shell(shell, MeshShell, indexes);
      Halfedge_descriptorE border_hedge;
      set<Halfedge_descriptorE> visited;
      bool filled = false;
      bool hole = true;
      int count = 0;
      while (hole && (count < 100)) {
        bool found_hole = false;
        for (auto h: halfedges(MeshShell)) {
          if (!visited.contains(h) && is_border(h, MeshShell)) {
            visited.insert(h);
            border_hedge = h;
            found_hole = true;
            break;
          }
        }
        if (found_hole) {
          filled = true;
          CGAL::Polygon_mesh_processing::triangulate_and_refine_hole(
                  MeshShell,
                  border_hedge
          );
        } else {
          hole = false;
        }
        count++;
      }
      MeshE BetterMesh;
      CGAL::Polygon_mesh_processing::remesh_planar_patches(MeshShell, BetterMesh);

      // if at least one of the holes is filled
      if (filled) {
        // get the shell
        vector<vector<vector<Point3E>>> outshell = get_faces(BetterMesh);
        vector<vector<vector<Point3E>>> goodshell = delete_triangle_lines(outshell);
        if (!STANDARDS["UseCaseRepair"]["Triangulation"]) {
          // detriangulate
          vector<vector<vector<Point3E>>> detrishell = detriangulate(goodshell);
          newshell = get_shell(detrishell, indexes);
        } else {
          newshell = get_shell(goodshell, indexes);
        }
      }
      return newshell;
    }

    vector<vector<vector<vector<int>>>> Shellr3pair303(vector<vector<vector<int>>> &shell, vector<bool> &vol) {
      // NON MANIFOLD CASE
      vector<vector<vector<vector<int>>>> newshells;
      vector<vector<vector<vector<int>>>> no_vol_shells;

      // check if faces are on top of eachother
      set<int> duplicates;
      for (int i = 0; i < shell.size(); ++i) {
        for (int j = i + 1; j < shell.size(); ++j) {
          vector<int> outer1 = shell[i][0];
          vector<int> outer2 = shell[j][0];
          if (outer1.size() == outer2.size()) {
            std::sort(outer1.begin(), outer1.end());
            std::sort(outer2.begin(), outer2.end());
            if (outer1 == outer2) {
              duplicates.insert(j);
            }
          }
        }
      }
      if (!duplicates.empty()) {
        std::cout << "\t\tDouble face problem!" << endl;
        vector<vector<vector<int>>> newshell;
        vector<vector<vector<vector<int>>>> dupes;
        vol.push_back(true);
        for (int i = 0; i < shell.size(); ++i) {
          if (!duplicates.contains(i)) {
            newshell.push_back(shell[i]);
          } else {
            vol.push_back(false);
            dupes.push_back({shell[i]});
          }
        }
        newshells.push_back(newshell);
        newshells.insert(newshells.end(), dupes.begin(), dupes.end());
        return newshells;
      }

      // Map to store half-edges. Key is the half-edge; value is a vector of face indices.
      map<pair<int, int>, vector<int>> halfEdgesMap;
      for (int faceIndex = 0; faceIndex < shell.size(); ++faceIndex) {
        for (auto &face: shell[faceIndex]) {
          for (size_t i = 0; i < face.size(); ++i) {
            size_t nextIndex = (i + 1) % face.size();
            pair<int, int> halfEdge(face[i], face[nextIndex]);
            halfEdgesMap[halfEdge].push_back(faceIndex);
          }
        }
      }

      // Find out if it is an umbrella problem
      bool Umbrella_Problem = true;
      set<pair<int, int>> overused_edges;
      for (auto &[halfEdge, faceIndices]: halfEdgesMap) {
        if (faceIndices.size() != 1) {
          Umbrella_Problem = false;
          overused_edges.insert(halfEdge);
        }
      }


      if (Umbrella_Problem) {
        std::cout << "\t\tUmbrella problem!" << endl;
        vector<vector<vector<int>>> shell_copy = shell;

        vector<vector<vector<int>>> current_shell;
        set<pair<int, int>> Open_edges_current_shell;

        // set the first
        current_shell.push_back(shell_copy[0]);
        for (auto &face: shell_copy[0]) {
          for (size_t i = 0; i < face.size(); ++i) {
            size_t nextIndex = (i + 1) % face.size();
            pair<int, int> halfEdge(face[i], face[nextIndex]);
            Open_edges_current_shell.insert(halfEdge);
          }
        }
        shell_copy.erase(shell_copy.begin() + 0);

        // Build the shells based on edges
        while (shell_copy.size() > 0) {

          // look for faces to add
          bool found = false;
          int found_index;
          for (int i = 0; i < shell_copy.size(); ++i) {
            for (auto &face: shell_copy[i]) {
              for (size_t j = 0; j < face.size(); ++j) {
                size_t nextIndex = (j + 1) % face.size();
                pair<int, int> halfEdge(face[nextIndex], face[j]); // backwards from what is already there

                if (Open_edges_current_shell.contains(halfEdge)) {
                  found = true;
                  found_index = i;
                  break;
                }
              }
              if (found) { break; }
            }
            if (found) { break; }
          }

          if (found) {
            // add to new shell
            current_shell.push_back(shell_copy[found_index]);
            // update Open edges
            for (auto &face: shell_copy[found_index]) {
              for (size_t j = 0; j < face.size(); ++j) {
                size_t nextIndex = (j + 1) % face.size();
                pair<int, int> halfEdge(face[nextIndex], face[j]); // backwards from what is already there
                if (Open_edges_current_shell.contains(halfEdge)) {
                  Open_edges_current_shell.erase(halfEdge);
                } else {
                  pair<int, int> halfEdgeG(face[j], face[nextIndex]);
                  Open_edges_current_shell.insert(halfEdgeG);
                }
              }
            }
            // erase added
            shell_copy.erase(shell_copy.begin() + found_index);

          } else {
            // does it volume
            if (Open_edges_current_shell.size() == 0) {
              vol.push_back(true);
              newshells.push_back(current_shell);
            } else {
              vol.push_back(false);
              newshells.push_back(current_shell);
            }

            current_shell.clear();
            Open_edges_current_shell.clear();

            // start new current shell
            current_shell.push_back(shell_copy[0]);
            for (auto &face: shell_copy[0]) {
              for (size_t i = 0; i < face.size(); ++i) {
                size_t nextIndex = (i + 1) % face.size();
                pair<int, int> halfEdge(face[i], face[nextIndex]);
                Open_edges_current_shell.insert(halfEdge);
              }
            }
            shell_copy.erase(shell_copy.begin() + 0);
          }
        }
        // add remaining face if needed
        if (Open_edges_current_shell.empty()) {
          vol.push_back(true);
          newshells.push_back(current_shell);
        } else {
          vol.push_back(false);
          newshells.push_back(current_shell);
        }
      } else {
        std::cout << "\t\tEdge or flip problem!" << endl;

        vector<vector<vector<vector<int>>>> temp_Shells;
        vector<set<pair<int, int>>> temp_openEdge;

        vector<vector<vector<int>>> shell_copy = shell;

        vector<vector<vector<int>>> current_shell;
        set<pair<int, int>> Open_edges_current_shell;

        // set the first
        current_shell.push_back(shell_copy[0]);
        for (auto &face: shell_copy[0]) {
          for (size_t i = 0; i < face.size(); ++i) {
            size_t nextIndex = (i + 1) % face.size();
            pair<int, int> halfEdge(face[i], face[nextIndex]);
            Open_edges_current_shell.insert(halfEdge);
          }
        }
        shell_copy.erase(shell_copy.begin() + 0);

        // Build the shells based on edges
        while (shell_copy.size() > 0) {

          // look for faces to add
          bool found = false;
          int found_index;
          for (int i = 0; i < shell_copy.size(); ++i) {
            for (auto &face: shell_copy[i]) {
              for (size_t j = 0; j < face.size(); ++j) {
                size_t nextIndex = (j + 1) % face.size();
                pair<int, int> halfEdge(face[nextIndex], face[j]); // backwards from what is already there
                if (Open_edges_current_shell.contains(halfEdge) && !overused_edges.contains(halfEdge)) {
                  found = true;
                  found_index = i;
                  break;
                }
              }
              if (found) { break; }
            }
            if (found) { break; }
          }

          if (found) {
            // add to new shell
            current_shell.push_back(shell_copy[found_index]);
            // update Open edges
            for (auto &face: shell_copy[found_index]) {
              for (size_t j = 0; j < face.size(); ++j) {
                size_t nextIndex = (j + 1) % face.size();
                pair<int, int> halfEdge(face[nextIndex], face[j]); // backwards from what is already there
                if (Open_edges_current_shell.contains(halfEdge)) {
                  Open_edges_current_shell.erase(halfEdge);
                } else {
                  pair<int, int> halfEdgeG(face[j], face[nextIndex]);
                  Open_edges_current_shell.insert(halfEdgeG);
                }
              }
            }
            // erase added
            shell_copy.erase(shell_copy.begin() + found_index);

          } else {
            // does it volume
            if (Open_edges_current_shell.size() == 0) {
              vol.push_back(true);
              newshells.push_back(current_shell);
            } else {
              temp_Shells.push_back(current_shell);
              temp_openEdge.push_back(Open_edges_current_shell);
            }

            current_shell.clear();
            Open_edges_current_shell.clear();

            // start new current shell
            current_shell.push_back(shell_copy[0]);
            for (auto &face: shell_copy[0]) {
              for (size_t i = 0; i < face.size(); ++i) {
                size_t nextIndex = (i + 1) % face.size();
                pair<int, int> halfEdge(face[i], face[nextIndex]);
                Open_edges_current_shell.insert(halfEdge);
              }
            }
            shell_copy.erase(shell_copy.begin() + 0);
          }
        }
        // add remaining face if needed
        if (Open_edges_current_shell.size() == 0 && current_shell.size() > 2) {
          vol.push_back(true);
          newshells.push_back(current_shell);
        } else {
          temp_Shells.push_back(current_shell);
          temp_openEdge.push_back(Open_edges_current_shell);
        }

        //check if not closed shells can combine by flipping
        if (!temp_Shells.empty()) {
          // set the first
          current_shell.clear();
          Open_edges_current_shell.clear();
          current_shell = temp_Shells[0];
          Open_edges_current_shell = temp_openEdge[0];
          temp_Shells.erase(temp_Shells.begin() + 0);
          temp_openEdge.erase(temp_openEdge.begin() + 0);


          while (temp_Shells.size() > 0) {
            bool found = false;
            bool problem = false;
            int found_index;
            for (int i = 0; i < temp_openEdge.size(); ++i) {
              for (const auto &edges: temp_openEdge[0]) {
                if (Open_edges_current_shell.contains(edges)) {
                  found = true;
                }
                if (Open_edges_current_shell.contains({edges.second, edges.first})) {
                  problem = true;
                }
              }
              if (found && !problem) {
                break;
              }
            }
            if (found && !problem) {
              // flip shell and add to current shell
              for (int i = 0; i < temp_Shells[found_index].size(); ++i) {
                vector<vector<int>> flipped_face = flip_face(temp_Shells[found_index][i]);
                current_shell.push_back(flipped_face);
              }
              // update Open edges
              for (const auto &edges: temp_openEdge[found_index]) {
                if (Open_edges_current_shell.contains(edges)) {
                  Open_edges_current_shell.erase(edges);
                } else {
                  Open_edges_current_shell.insert({edges.second, edges.first});
                }
              }
              // erase added
              temp_Shells.erase(temp_Shells.begin() + found_index);
              temp_openEdge.erase(temp_openEdge.begin() + found_index);

            } else {
              // does it volume
              if (Open_edges_current_shell.size() == 0 && current_shell.size() > 2) {
                vol.push_back(true);
                newshells.push_back(current_shell);
              } else {
                vol.push_back(false);
                newshells.push_back(current_shell);
              }

              current_shell.clear();
              Open_edges_current_shell.clear();

              // start new current shell
              current_shell.clear();
              Open_edges_current_shell.clear();
              current_shell = temp_Shells[0];
              Open_edges_current_shell = temp_openEdge[0];
              temp_Shells.erase(temp_Shells.begin() + 0);
              temp_openEdge.erase(temp_openEdge.begin() + 0);
            }
          }
          // does it volume
          if (Open_edges_current_shell.size() == 0 && current_shell.size() > 2) {
            vol.push_back(true);
            newshells.push_back(current_shell);
          } else {
            vol.push_back(false);
            newshells.push_back(current_shell);
          }
        }
      }
      return newshells;
    }

    vector<vector<vector<vector<int>>>> Shellr3pair305(vector<vector<vector<int>>> &shell, vector<bool> &vol) {
      vector<vector<vector<vector<int>>>> newshells;
      // make the mesh
      MeshE MeshShell;
      map<Point3E, int> indexes;
      make_shell(shell, MeshShell, indexes);

      // Find the connected components
      FCCmapE fccmap = MeshShell.add_property_map<face_descriptorE, faces_size_typeE>("f:CC").first;
      PMP::connected_components(MeshShell, fccmap);
      map<faces_size_typeE, vector<vector<vector<int>>>> components;
      for (auto f: faces(MeshShell)) {
        vector<vector<int>> indices(1);
        CGAL::Vertex_around_face_circulator<MeshE> vcirc(MeshShell.halfedge(f), MeshShell), done(vcirc);
        do {
          Point3E point = MeshShell.point(*vcirc);
          int original_index = indexes[point];
          indices[0].push_back(original_index);
          ++vcirc;
        } while (vcirc != done);
        components[fccmap[f]].push_back(indices);
      }

      // Sort the elements big to small
      vector<pair<faces_size_type, vector<vector<vector<int>>>>> elems(components.begin(), components.end());
      auto comp = [](auto &a, auto &b) {
          return a.second.size() > b.second.size(); // Sorting by the size of the outer vector, largest to smallest
      };
      sort(elems.begin(), elems.end(), comp);

      // Check if it has volume and detriangulate if needed
      for (auto &elem: elems) {
        MeshE voltest;
        map<Point3E, int> indexesv;
        make_shell(elem.second, voltest, indexesv);
        bool test = false;

        if (PMP::volume(voltest) > 0) {
          test = true;
        }
        vol.push_back(test);

        if (!STANDARDS["UseCaseRepair"]["Triangulation"]) {
          // detriangulate
          vector<vector<vector<Point3E>>> outshell = get_faces(voltest);
          vector<vector<vector<Point3E>>> detrishell = detriangulate(outshell);
          vector<vector<vector<int>>> newshell = get_shell(detrishell, indexes);
          newshells.push_back(newshell);
        } else {
          newshells.push_back(elem.second);
        }
      }
      return newshells;
    }

    vector<vector<vector<int>>> Shellr3pair306(vector<vector<vector<int>>> &shell) {
      // Function to handle self-intersections
      vector<vector<vector<int>>> newshell;
      Mesh mesh;
      map<Point3, int> indexes;
      make_shell(shell, mesh, indexes);

      // Step 1: Detect self-intersections
      std::vector<std::pair<face_descriptor, face_descriptor>> intersecting_faces;
      PMP::self_intersections(mesh, std::back_inserter(intersecting_faces));

      // Step 2: Collect faces to remove
      std::set<face_descriptor> faces_to_remove;
      for (const auto &pair: intersecting_faces) {
        faces_to_remove.insert(pair.first);
        faces_to_remove.insert(pair.second);
      }

      // Step 3: Remove the intersecting faces
      for (auto fd: faces_to_remove) {
        // Get a halfedge of the face to remove
        halfedge_descriptor h = halfedge(fd, mesh);
        // Remove the face
        CGAL::Euler::remove_face(h, mesh);
      }

      // Optionally, you may want to remove isolated vertices after removing faces
      PMP::remove_isolated_vertices(mesh);

      vector<vector<vector<Point3>>> outshell = get_faces(mesh);
      vector<vector<vector<Point3>>> goodshell = delete_triangle_lines(outshell);
      vector<vector<vector<int>>> holeshell = get_shell(goodshell, indexes);
      newshell = Shellr3pair302(holeshell);
      return newshell;
    }

    vector<vector<vector<int>>> Shellr3pair307(vector<vector<vector<int>>> &shell, int index) {
      // POLYGON WRONG ORIENTATION
      vector<vector<vector<int>>> newshell;
      for (int i = 0; i < shell.size(); ++i) {
        if (i == index) { newshell.push_back(flip_face(shell[i])); }
        else { newshell.push_back(shell[i]); }
      }
      return newshell;
    }


} // AUTOr3pair
