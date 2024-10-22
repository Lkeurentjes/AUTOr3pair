// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#include "Geometrytools_makeCGAL.h"
#include "Defenitions.h"
#include "DefenitionsCGAL.h"
#include "Geometrytools.h"
#include "GeometrytoolsDetriangulationPoint3.h"
#include "GeometrytoolsDetriangulationPoint3E.h"


using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
    void make_ring(vector<int> &inring, vector<Point3> &outring) {
      vector<Point3> verticesP3 = VERTICES.get_verticesPoint3();
      for (auto &v: inring) { outring.push_back(verticesP3[v]); }
    }

    CgalPolyhedron::Plane_3 get_best_fitted_plane(const std::vector<Point3> &lsPts) {
      CgalPolyhedron::Plane_3 p;
      linear_least_squares_fitting_3(lsPts.begin(), lsPts.end(), p, CGAL::Dimension_tag<0>());
      K::FT tol = 1e-12;
      K::FT a = p.a();
      K::FT b = p.b();
      K::FT c = p.c();
      bool updated = false;
      if (CGAL::abs(p.a()) < tol) {
        a = 0;
        updated = true;
      }
      if (CGAL::abs(p.b()) < tol) {
        b = 0;
        updated = true;
      }
      if (CGAL::abs(p.c()) < tol) {
        c = 0;
        updated = true;
      }
      if (updated == false)
        return p;
      else { return CgalPolyhedron::Plane_3(a, b, c, p.d()); }
    }

    void
    create_2Dcgal_polygon(const vector<Point3> &lsPts, const CgalPolyhedron::Plane_3 &plane, Polygon &outpgn) {
      for (const auto &lsPt: lsPts) { outpgn.push_back(plane.to_2d(lsPt)); }
    }

    void
    create_2Dcgal_polygon(const vector<Point3E> &lsPts, const CgalPolyhedronE::Plane_3 &plane, PolygonE &outpgn) {
      for (const auto &lsPt: lsPts) { outpgn.push_back(plane.to_2d(lsPt)); }
    }

    CgalPolyhedronE::Plane_3 get_best_fitted_plane(const std::vector<Point3E> &lsPts) {
      CgalPolyhedronE::Plane_3 p;
      linear_least_squares_fitting_3(lsPts.begin(), lsPts.end(), p, CGAL::Dimension_tag<0>());
      K::FT tol = 1e-12;
      auto a = p.a();
      auto b = p.b();
      auto c = p.c();
      bool updated = false;
      if (CGAL::abs(p.a()) < tol) {
        a = 0;
        updated = true;
      }
      if (CGAL::abs(p.b()) < tol) {
        b = 0;
        updated = true;
      }
      if (CGAL::abs(p.c()) < tol) {
        c = 0;
        updated = true;
      }
      if (updated == false)
        return p;
      else { return CgalPolyhedronE::Plane_3(a, b, c, p.d()); }
    }

    CgalPolyhedronE::Plane_3 get_best_fitted_planeE(const std::vector<Point3E> &lsPts) {
      CgalPolyhedronE::Plane_3 p;
      linear_least_squares_fitting_3(lsPts.begin(), lsPts.end(), p, CGAL::Dimension_tag<0>());
      K::FT tol = 1e-12;
      auto a = p.a();
      auto b = p.b();
      auto c = p.c();
      bool updated = false;
      if (CGAL::abs(p.a()) < tol) {
        a = 0;
        updated = true;
      }
      if (CGAL::abs(p.b()) < tol) {
        b = 0;
        updated = true;
      }
      if (CGAL::abs(p.c()) < tol) {
        c = 0;
        updated = true;
      }
      if (updated == false)
        return p;
      else { return CgalPolyhedronE::Plane_3(a, b, c, p.d()); }
    }

    void make_2D_poly(vector<Point3E> &lsPts, CgalPolyhedronE &poly) {
      // builder
      Builder builder(poly.hds(), true);

      builder.begin_surface(lsPts.size(), lsPts.size());

      // Add vertices to the poly
      for (const Point3E &vertex: lsPts) { builder.add_vertex(vertex); }

      // Add faces to the poly
      builder.begin_facet();
      for (int i = 0; i < lsPts.size(); ++i) { builder.add_vertex_to_facet(i); }
      builder.end_facet();

      // Finish building the poly
      builder.end_surface();
    }

    void make_2D_poly(vector<Point3E> &lsPts, Nef_polyhedron &Poly) {
      CgalPolyhedronE p;
      make_2D_poly(lsPts, p);
      const Nef_polyhedron P(p);
      Poly += P;
    }

    bool make_2D_polySMT(vector<int> &face, Nef_polyhedron &Poly) {
      return false;
      // check based on points
      set<int> seen;
      vector<int> uniqueFace;

      for (int index : face) {
        if (seen.find(index) == seen.end()) {
          uniqueFace.push_back(index);
          seen.insert(index);
        }
      }
      if (uniqueFace.size() < 3 || face.size() != uniqueFace.size()){
        return false;
      }

      vector<Point3E> lsPts;
      make_ring(face, lsPts);

      // test if not sameplane
      set<Point3E> all_v;
      all_v.insert(lsPts.begin(), lsPts.end());
      if (!all_points_on_same_planeSMT(all_v)) {
        return false;
      }

      // Check for nearly coincident points
      const double tolerance = 1; // Threshold for coincidence
      for (size_t i = 0; i < lsPts.size(); ++i) {
        for (size_t j = i + 1; j < lsPts.size(); ++j) {
          if (CGAL::squared_distance(lsPts[i], lsPts[j]) < tolerance * tolerance) {
            return false;
          }
        }
      }

      // Check for nearly collinear points
      for (size_t i = 0; i < lsPts.size(); ++i) {
        for (size_t j = i + 1; j < lsPts.size(); ++j) {
          for (size_t k = j + 1; k < lsPts.size(); ++k) {
            Vector3E v1 = lsPts[j] - lsPts[i];
            Vector3E v2 = lsPts[k] - lsPts[i];
            Vector3E cross_product = CGAL::cross_product(v1, v2);
            if (cross_product.squared_length() < tolerance) {
              return false;
            }
          }
        }
      }

      // To check if it is simple
      CgalPolyhedronE::Plane_3 best_plane = get_best_fitted_plane(lsPts);
      PolygonE pgn;
      create_2Dcgal_polygon(lsPts, best_plane, pgn);
      bool test = pgn.is_simple();
      if (!test) {return false;}
      double area = abs(CGAL::to_double(pgn.area()));

      if (area < 3){
        return false;
      }

      CgalPolyhedronE p;
      make_2D_poly(lsPts, p);
      CGAL::Polygon_mesh_processing::remove_degenerate_faces(p);
      // Test here: Validate the polyhedron
      if (!p.is_pure_bivalent () || !p.is_valid() || p.empty() ) {
        return false;
      }

      if (CGAL::Polygon_mesh_processing::does_self_intersect(p)) {
        return false;
      }
      std::cout << p << endl;

      const Nef_polyhedron P(p);
      Poly += P;
      return true;
    }

    void make_3D_poly(vector<vector<vector<int>>> &boundary, map<int, Point3E> &lsPts, CgalPolyhedronE &Poly) {
      // builder
      Builder builder(Poly.hds(), true);

      int facessize = boundary.size();
      int verticessize = 0;
      for (const auto &outerVec: boundary) {
        for (const auto &middleVec: outerVec) { verticessize += middleVec.size(); }
      }

      builder.begin_surface(lsPts.size(), facessize);
      // Add vertices to the Poly
      map<int, int> counter;
      int count = 0;
      for (const auto vertexpair: lsPts) {
        builder.add_vertex(vertexpair.second);
        counter[vertexpair.first] = count;
        count++;
      }

      // Add faces to the Poly
      for (vector<vector<int>> &face: boundary) {
        builder.begin_facet();
        // only do outer, polyhedron cannot make holes
        for (int index: face[0]) { builder.add_vertex_to_facet(counter[index]); }
        builder.end_facet();
      }

      // Finish building the Poly
      builder.end_surface();
    }

    void make_3D_poly(vector<vector<vector<int>>> &boundary, map<int, Point3E> &lsPts, Nef_polyhedron &Poly) {
      CgalPolyhedronE p;
      make_3D_poly(boundary, lsPts, p);
      Nef_polyhedron P(p);
      Poly += P;
    }

    void make_ring(vector<int> &inring, vector<Point3E> &outring) {
      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      for (auto &v: inring) { outring.push_back(verticesP3[v]); }
    }

    void make_rings(vector<vector<int>> &face, vector<Nef_polyhedron> &rings, map<Point3E, int> &indexes) {
      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      for (int i = 0; i < face.size(); ++i) {
        vector<Point3E> ring;
        for (int j = 0; j < face[i].size(); ++j) {
          ring.push_back(verticesP3[face[i][j]]);
          indexes[verticesP3[face[i][j]]] = face[i][j];
        }
        Nef_polyhedron R;
        make_2D_poly(ring, R);
        rings.push_back(R);
        // all_ringsP3.push_back(ring);
      }
    }

    void make_rings(vector<vector<int>> &face, vector<vector<Point3E>> &rings, vector<Point3E> &points,
                    map<Point3E, int> &indexes) {
      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      for (int i = 0; i < face.size(); ++i) {
        vector<Point3E> ring;
        for (int j = 0; j < face[i].size(); ++j) {
          points.push_back(verticesP3[face[i][j]]);
          indexes[verticesP3[face[i][j]]] = face[i][j];
          ring.push_back(verticesP3[face[i][j]]);
        }
        rings.push_back(ring);
      }
    }

    void make_rings(vector<vector<int>> &face, vector<vector<Point3>> &rings, vector<Point3> &points,
                    map<Point3, int> &indexes) {
      vector<Point3> verticesP3 = VERTICES.get_verticesPoint3();
      for (int i = 0; i < face.size(); ++i) {
        vector<Point3> ring;
        for (int j = 0; j < face[i].size(); ++j) {
          points.push_back(verticesP3[face[i][j]]);
          indexes[verticesP3[face[i][j]]] = face[i][j];
          ring.push_back(verticesP3[face[i][j]]);
        }
        rings.push_back(ring);
      }
    }

    void make_shell(vector<vector<vector<int>>> &shell, Nef_polyhedron &polyShell, map<Point3E, int> &indexes) {
      // makes non manifold nef polyhederon
      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      for (auto &face: shell) {
        vector<Nef_polyhedron> faceP;
        for (int i = 0; i < face.size(); ++i) {
          vector<Point3E> ring;
          for (auto &v: face[i]) {
            indexes[verticesP3[v]] = v;
            ring.push_back(verticesP3[v]);
          }
          Nef_polyhedron ringP;
          make_2D_poly(ring, ringP);
          faceP.push_back(ringP);
        }
        for (int i = 1; i < faceP.size(); ++i) { faceP[0] -= faceP[i]; }
        polyShell += faceP[0];
      }
    }

    bool make_shell(vector<vector<vector<int>>> &shell, MeshE &MeshShell, map<Point3E, int> &indexes) {
      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      vector<vector<int>> faces;
      vector<vector<int>> holes;
      for (auto &face: shell) {
        faces.push_back(face[0]);
        for (int i = 1; i < face.size(); ++i) { holes.push_back(face[i]); }
        for (auto &ring: face) {
          for (auto &v: ring) {
            indexes[verticesP3[v]] = v;
          }
        }
      }
      PMP::polygon_soup_to_polygon_mesh(verticesP3, faces, MeshShell);

      // Remove unused vertices (just for clearity)
      remove_unused_vertices(MeshShell);

      // Triangulate mesh
      PMP::triangulate_faces(MeshShell);

      if (!holes.empty()) {
        MeshE Holes;
        PMP::polygon_soup_to_polygon_mesh(verticesP3, holes, Holes);
        PMP::triangulate_faces(Holes);

        PMP::split(MeshShell, Holes);

        set<set<Point3E>> holefaces;
        for (Mesh::Face_index fi: Holes.faces()) {
          set<Point3E> holeface;
          Mesh::Halfedge_index h = Holes.halfedge(fi);
          Mesh::Halfedge_index h_end = h;
          do {
            Mesh::Vertex_index v = Holes.target(h);
            Point3E p = Holes.point(v);
            holeface.insert(p);
            h = Holes.next(h);
          } while (h != h_end);
          holefaces.insert(holeface);
        }

        vector<Mesh::Face_index> faces_to_remove;

        while (true){
          bool found = false;
          int count = 0;
          for (Mesh::Face_index fi: MeshShell.faces()) {
            Mesh::Halfedge_index h = MeshShell.halfedge(fi);
            Mesh::Halfedge_index h_end = h;
            set<Point3E> face;
            do {
              Mesh::Vertex_index v = MeshShell.target(h);
              Point3E p = MeshShell.point(v);
              face.insert(p);
              h = MeshShell.next(h);
            } while (h != h_end);

            if (holefaces.contains(face)){
              halfedge_descriptor hi = halfedge(fi, MeshShell);
              // Remove the face
              CGAL::Euler::remove_face(hi,  MeshShell);
              found = true;
              break;
            }
            count++;
          }
          if (!found){break;}
        }

        MeshShell.collect_garbage();

        MeshE copy;
        std::map<Mesh::Vertex_index, Mesh::Vertex_index> vmap;
        std::map<Point3E, Mesh::Vertex_index> point_map_duplicates;  // Secondary map for points to vertex index
        for (auto v: MeshShell.vertices()) {
          Point3E p = MeshShell.point(v);
          auto it = point_map_duplicates.find(p);
          if (it != point_map_duplicates.end()) {
            // Point exists, link to existing vertex
            vmap[v] = it->second;
          } else {
            // Point does not exist, add a new vertex to copy and update both maps
            Mesh::Vertex_index new_vertex = copy.add_vertex(p);
            vmap[v] = new_vertex;
            point_map_duplicates[p] = new_vertex;  // Store the point and its corresponding vertex index
          }
        }
        for (auto f: MeshShell.faces()) {
          std::vector<Mesh::Vertex_index> vertices;
          for (auto v: vertices_around_face(MeshShell.halfedge(f), MeshShell)) {
            vertices.push_back(vmap[v]);
          }
          copy.add_face(vertices);
        }
        remove_unused_vertices(copy);
        MeshShell = copy;
      }
        return true;
    }

    bool make_shell(vector<vector<vector<int>>> &shell, Mesh &MeshShell, map<Point3, int> &indexes) {
      vector<Point3> verticesP3 = VERTICES.get_verticesPoint3();
      vector<vector<int>> faces;
      vector<vector<int>> holes;
      for (auto &face: shell) {
        faces.push_back(face[0]);
        for (int i = 1; i < face.size(); ++i) { holes.push_back(face[i]); }
        for (auto &ring: face) {
          for (auto &v: ring) {
            indexes[verticesP3[v]] = v;
          }
        }
      }
      PMP::polygon_soup_to_polygon_mesh(verticesP3, faces, MeshShell);

      // Remove unused vertices (just for clearity)
      remove_unused_vertices(MeshShell);

      // Triangulate mesh
      PMP::triangulate_faces(MeshShell);

      if (!holes.empty()) {
        Mesh Holes;
        PMP::polygon_soup_to_polygon_mesh(verticesP3, holes, Holes);
        PMP::triangulate_faces(Holes);

        PMP::split(MeshShell, Holes);

        set<set<Point3>> holefaces;
        for (Mesh::Face_index fi: Holes.faces()) {
          set<Point3> holeface;
          Mesh::Halfedge_index h = Holes.halfedge(fi);
          Mesh::Halfedge_index h_end = h;
          do {
            Mesh::Vertex_index v = Holes.target(h);
            Point3 p = Holes.point(v);
            holeface.insert(p);
            h = Holes.next(h);
          } while (h != h_end);
          holefaces.insert(holeface);
        }

        vector<Mesh::Face_index> faces_to_remove;

        while (true){
          bool found = false;
          int count = 0;
          for (Mesh::Face_index fi: MeshShell.faces()) {
            Mesh::Halfedge_index h = MeshShell.halfedge(fi);
            Mesh::Halfedge_index h_end = h;
            set<Point3> face;
            do {
              Mesh::Vertex_index v = MeshShell.target(h);
              Point3 p = MeshShell.point(v);
              face.insert(p);
              h = MeshShell.next(h);
            } while (h != h_end);

            if (holefaces.contains(face)){
              halfedge_descriptor hi = halfedge(fi, MeshShell);
              // Remove the face
              CGAL::Euler::remove_face(hi,  MeshShell);
              found = true;
              break;
            }
            count++;
          }
          if (!found){break;}
        }

        MeshShell.collect_garbage();

        Mesh copy;
        std::map<Mesh::Vertex_index, Mesh::Vertex_index> vmap;
        std::map<Point3, Mesh::Vertex_index> point_map_duplicates;  // Secondary map for points to vertex index
        for (auto v: MeshShell.vertices()) {
          Point3 p = MeshShell.point(v);
          auto it = point_map_duplicates.find(p);
          if (it != point_map_duplicates.end()) {
            // Point exists, link to existing vertex
            vmap[v] = it->second;
          } else {
            // Point does not exist, add a new vertex to copy and update both maps
            Mesh::Vertex_index new_vertex = copy.add_vertex(p);
            vmap[v] = new_vertex;
            point_map_duplicates[p] = new_vertex;  // Store the point and its corresponding vertex index
          }
        }
        for (auto f: MeshShell.faces()) {
          std::vector<Mesh::Vertex_index> vertices;
          for (auto v: vertices_around_face(MeshShell.halfedge(f), MeshShell)) {
            vertices.push_back(vmap[v]);
          }
          copy.add_face(vertices);
        }
        remove_unused_vertices(copy);
        MeshShell = copy;
      }
      return true;
    }

    void make_shells(vector<vector<vector<vector<int>>>> &solid, vector<Nef_polyhedron> &shells,
                     map<Point3E, int> &indexes) {
      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      for (vector<vector<vector<int>>> shell: solid) {
        map<int, Point3E> shellindexes;
        for (int i = 0; i < shell.size(); ++i) {
          for (int j = 0; j < shell[i].size(); ++j) {
            for (int k = 0; k < shell[i][j].size(); ++k) {
              indexes[verticesP3[shell[i][j][k]]] = shell[i][j][k];
              shellindexes[shell[i][j][k]] = verticesP3[shell[i][j][k]];
            }
          }
        }
        Nef_polyhedron shellpoly;
        make_3D_poly(shell, shellindexes, shellpoly);
        shells.push_back(shellpoly);
      }
    }

    void make_shell3D(vector<vector<vector<int>>> &shell, Nef_polyhedron &shells,
                     map<Point3E, int> &indexes) {
      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      map<int, Point3E> shellindexes;
      for (int i = 0; i < shell.size(); ++i) {
        for (int j = 0; j < shell[i].size(); ++j) {
          for (int k = 0; k < shell[i][j].size(); ++k) {
            indexes[verticesP3[shell[i][j][k]]] = shell[i][j][k];
            shellindexes[shell[i][j][k]] = verticesP3[shell[i][j][k]];
          }
        }
      }

      Nef_polyhedron shellpoly;
      make_3D_poly(shell, shellindexes, shellpoly);
      shells += shellpoly;

    }

    void make_shells(vector<vector<vector<vector<int>>>> &solid, vector<CgalPolyhedronE> &shells,
                     map<Point3E, int> &indexes) {
      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      for (vector<vector<vector<int>>> shell: solid) {
        map<int, Point3E> shellindexes;
        for (int i = 0; i < shell.size(); ++i) {
          for (int j = 0; j < shell[i].size(); ++j) {
            for (int k = 0; k < shell[i][j].size(); ++k) {
              indexes[verticesP3[shell[i][j][k]]] = shell[i][j][k];
              shellindexes[shell[i][j][k]] = verticesP3[shell[i][j][k]];
            }
          }
        }
        CgalPolyhedronE shellpoly;
        make_3D_poly(shell, shellindexes, shellpoly);
        shells.push_back(shellpoly);
      }
    }

    void make_Solids(vector<vector<vector<vector<vector<int>>>>> &Csolid, vector<vector<Nef_polyhedron>> &Solids,
                     map<Point3E, int> &indexes) {
      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      for (auto &solid: Csolid) {
        vector<Nef_polyhedron> shells;
        for (auto &shell: solid) {
          map<int, Point3E> shellindexes;
          for (auto &face: shell) {
            for (auto &ring: face) {
              for (auto &v: ring) {
                indexes[verticesP3[v]] = v;
                shellindexes[v] = verticesP3[v];
              }
            }
          }
          Nef_polyhedron shellpoly;
          make_3D_poly(shell, shellindexes, shellpoly);
          shells.push_back(shellpoly);
        }
        Solids.push_back(shells);
      }
    }

    void make_PolygonSoupShell(vector<vector<vector<int>>> &shell, MeshE &MeshShell, map<Point3E, int> &indexes) {
      vector<Point3E> verticesP3 = VERTICES.get_verticesPoint3E();
      vector<vector<int>> faces;
      vector<vector<int>> holes;
      for (auto &face: shell) {
        faces.push_back(face[0]);
        for (int i = 1; i < face.size(); ++i) { holes.push_back(face[i]); }
        for (auto &ring: face) {
          for (auto &v: ring) {
            indexes[verticesP3[v]] = v;
          }
        }
      }
      typedef MeshE::Vertex_index Vertex_index;
      std::vector<Vertex_index> vertex_indices;
      for (const auto& vertex : verticesP3) {
        vertex_indices.push_back(MeshShell.add_vertex(vertex));
      }

      // Insert faces
      for (const auto& face : faces) {
        std::vector<Vertex_index> face_vertices;
        for (const auto& idx : face) {
          face_vertices.push_back(vertex_indices[idx]);
        }
        MeshShell.add_face(face_vertices);
      }

    }
}
