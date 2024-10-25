// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#include "GeometryTools_makeCO.h"
#include "Defenitions.h"
#include "DefenitionsCGAL.h"
#include <iostream>

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair{
	vector<vector<Point2E>> get_routes(PolygonE& Poly, vector<Point2E>& cut) {
		vector<vector<Point2E>> Routes;
		set<Point2E> cutset(cut.begin(), cut.end());

		map<Point2E, Point2E> HE;
		for (auto e = Poly.edges_begin(); e != Poly.edges_end(); ++e) { HE[e->source()] = e->target(); }

		for (int i = 0; i < cut.size(); ++i) {
			vector<Point2E> Route;
			Route.push_back(cut[i]);
			Point2E next = HE[cut[i]];
			while (true) {
				Route.push_back(next);
				if (cutset.contains(next)) {
					Routes.push_back(Route);
					break;
				}
				next = HE[next];
			}
		}
		return Routes;
	}

	vector<Point3> make_boundary(CgalPolyhedron& poly) {
		vector<Point3> ring;
		Point3 source;
		Point3 target;
		for (auto he = poly.vertices_begin(); he != poly.vertices_end(); ++he) {
			source = he->point();
			std::cout << source << endl;
		}
		for (auto he = poly.halfedges_begin(); he != poly.halfedges_end(); ++he) {
			source = he->vertex_begin()->vertex()->point();
			std::cout << source << endl;
		}

		return ring;
	}


	vector<Point3E> make_boundary(Nef_polyhedron& poly) {
		vector<Point3E> ring;

		map<Point3E, vector<Point3E>> HEdges;
		Point3E source;
		Point3E target;
		for (auto he = poly.halfedges_begin(); he != poly.halfedges_end(); ++he) {
			source = he->source()->point();
			target = he->target()->point();
			//            std::cout << "  halfedge from " << source << " to " << target << std::endl;
			if (HEdges.contains(source)) { HEdges[source].push_back(target); }
			else { HEdges[source] = {target}; }
		}
		ring.push_back(source);
		ring.push_back(target);
		int i = 0;
		while (true) {
			source = target;
			if (HEdges[source][0] != ring[i]) { target = HEdges[source][0]; }
			else { target = HEdges[source][1]; }

			if (target == ring[0]) { break; }
			ring.push_back(target);
			i++;
		}
		return ring;
	}

	vector<vector<vector<Point3E>>> get_faces(Nef_polyhedron& poly) {
		vector<vector<vector<Point3E>>> faces;
		Volume_const_iterator c;
		Shell_explorer SE;
		CGAL_forall_volumes(c, poly) {
			Shell_entry_const_iterator it;
			CGAL_forall_shells_of(it, c) {
				poly.visit_shell_objects(SFace_const_handle(it), SE);
				faces = SE.get_faces();
			}
		}
		return faces;
	}

    vector<vector<vector<Point3E>>> get_faces_exterior(Nef_polyhedron& poly) {
      vector<vector<vector<Point3E>>> faces;
      Volume_const_iterator c;
      Shell_explorer SE;
      CGAL_forall_volumes(c, poly) {
        if (c->mark()) {
          Shell_entry_const_iterator it;
          CGAL_forall_shells_of(it, c) {
            poly.visit_shell_objects(SFace_const_handle(it), SE);
            faces = SE.get_faces();
          }
        }
      }
      return faces;
    }

	vector<vector<vector<Point3E>>> get_faces(MeshE& mesh) {
		vector<vector<vector<Point3E>>> faces;

		for (const auto& face : mesh.faces()) {
			vector<vector<Point3E>> f;
			vector<Point3E> r;

			// CGAL uses circulators to iterate over the vertices (or halfedges) around a face
			CGAL::Vertex_around_face_circulator<MeshE> vcirc(mesh.halfedge(face), mesh), done(vcirc);
			do {
				r.push_back(mesh.point(*vcirc));
			} while (++vcirc != done);
			f.push_back(r);
			faces.push_back(f);
		}

		return faces;
	}

    vector<vector<vector<Point3>>> get_faces(Mesh& mesh) {
      vector<vector<vector<Point3>>> faces;

      for (const auto& face : mesh.faces()) {
        vector<vector<Point3>> f;
        vector<Point3> r;

        // CGAL uses circulators to iterate over the vertices (or halfedges) around a face
        CGAL::Vertex_around_face_circulator<Mesh> vcirc(mesh.halfedge(face), mesh), done(vcirc);
        do {
          r.push_back(mesh.point(*vcirc));
        } while (++vcirc != done);
        f.push_back(r);
        faces.push_back(f);
      }

      return faces;
    }

	vector<vector<vector<vector<Point3E>>>> get_Shells(Nef_polyhedron& poly) {
		vector<vector<vector<vector<Point3E>>>> Shells;
		Volume_const_iterator c;
		Shell_explorer SE;
		CGAL_forall_volumes(c, poly) {
			// Mark makes sure it are interior shells
			if (c->mark()) {
				Shell_entry_const_iterator it;
				CGAL_forall_shells_of(it, c) {
					poly.visit_shell_objects(SFace_const_handle(it), SE);
					vector<vector<vector<Point3E>>> shell = SE.get_faces();
					Shells.push_back(shell);
					SE.clear_faces();
				}
			}
		}
		return Shells;
	}

	vector<vector<vector<vector<vector<Point3E>>>>> get_Solids(Nef_polyhedron& poly) {
		vector<vector<vector<vector<vector<Point3E>>>>> Solids;

		Volume_const_iterator c;
		Shell_explorer SE;
		CGAL_forall_volumes(c, poly) {
			vector<vector<vector<vector<Point3E>>>> Shells;
			// Mark makes sure it are interior shells
			if (c->mark()) {
				Shell_entry_const_iterator it;
				CGAL_forall_shells_of(it, c) {
					poly.visit_shell_objects(SFace_const_handle(it), SE);
					vector<vector<vector<Point3E>>> shell = SE.get_faces();
					Shells.push_back(shell);
					SE.clear_faces();
				}
				Solids.push_back(Shells);
			}
		}
		return Solids;
	}

	vector<int> get_ring(vector<Point3E>& ring, map<Point3E, int>& indexes) {
		vector<int> newring;
		for (auto& v : ring) {
			if (indexes.contains(v)) { newring.push_back(indexes[v]); }
			else {
        int index = VERTICES.addvertex(v);
				newring.push_back(index);
				indexes[v] = index;
			}
		}
		return newring;
	}

	vector<vector<int>> get_face(vector<vector<Point3E>>& face, map<Point3E, int>& indexes) {
		vector<vector<int>> newface;
		for (auto& ring : face) {
			vector<int> newring = get_ring(ring, indexes);
			newface.push_back(newring);
		}
		return newface;
	}

	vector<vector<vector<int>>> get_shell(vector<vector<vector<Point3E>>>& shell, map<Point3E, int>& indexes) {
		vector<vector<vector<int>>> newshell;
		for (auto& face : shell) {
			vector<vector<int>> newface = get_face(face, indexes);
			newshell.push_back(newface);
		}
		return newshell;
	}

	vector<vector<vector<vector<int>>>> get_solid(vector<vector<vector<vector<Point3E>>>>& solid,
	                                              map<Point3E, int>& indexes) {
		vector<vector<vector<vector<int>>>> newsolid;
		for (auto& shell : solid) {
			vector<vector<vector<int>>> newshell = get_shell(shell, indexes);
			newsolid.push_back(newshell);
		}
		return newsolid;
	}

	vector<vector<vector<vector<vector<int>>>>> get_Msolid(vector<vector<vector<vector<vector<Point3E>>>>>& Msolid,
		map<Point3E, int>& indexes) {
		vector<vector<vector<vector<vector<int>>>>> newMsolid;
		for (auto& solid : Msolid) {
			vector<vector<vector<vector<int>>>> newsolid = get_solid(solid, indexes);
			newMsolid.push_back(newsolid);
		}
		return newMsolid;
	}

    vector<vector<vector<Point3>>> delete_triangle_lines( const vector<vector<vector<Point3>>> &triangles) {
      vector<vector<vector<Point3>>> newshell;
      // Iterate over each vector<vector<Point3>> in triangles
      for (const auto& shell : triangles) {
        for (const auto& points : shell) {
          if (all_distances_greater_than_tol(points)) {
            newshell.push_back({points});
          }
        }
      }
      return newshell;
    }

    bool all_distances_greater_than_tol(const vector<Point3> &points) {
      double tol = STANDARDS["Tollerances"]["snap_tol"];
      double squared_tol = tol * tol;  // tol²
      for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
          // Compare the squared distance with tol²
          if (CGAL::squared_distance(points[i], points[j]) <= squared_tol) {
            return false;  // The points are too close
          }
        }
      }
      return true;
    }

    bool all_distances_greater_than_tol(const std::vector<Point3E> &points) {
      // Retrieve the tolerance from the external STANDARDS map
      double tol = STANDARDS["Tollerances"]["snap_tol"];
      double squared_tol = tol * tol;  // Compute tol² for squared distance comparison

      // Compare the squared distances between each pair of points
      for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
          if (CGAL::squared_distance(points[i], points[j]) <= squared_tol) {
            return false;  // The points are too close
          }
        }
      }
      return true;
    }

    vector<vector<vector<Point3E>>> delete_triangle_lines(const vector<vector<vector<Point3E>>> &triangles) {
      vector<vector<vector<Point3E>>> newshell;

      // Iterate over each vector<vector<Point3E>> in triangles
      for (const auto& shell : triangles) {
        vector<vector<Point3E>> filtered_shell;

        // Iterate over each vector<Point3E> in the shell
        for (const auto& points : shell) {
          // Only add the vector if all distances between points are greater than tol
          if (all_distances_greater_than_tol(points)) {
            filtered_shell.push_back(points);  // Add to filtered shell
          }
        }

        // Add the filtered shell to the result if it's not empty
        if (!filtered_shell.empty()) {
          newshell.push_back(filtered_shell);
        }
      }

      return newshell;
    }

    vector<int> get_ring(vector<Point3>& ring, map<Point3, int>& indexes) {
      vector<int> newring;
      for (auto& v : ring) {
        if (indexes.contains(v)) { newring.push_back(indexes[v]); }
        else {
          int index = VERTICES.addvertex(v);
          newring.push_back(index);
          indexes[v] = index;
        }
      }
      return newring;
    }

    vector<vector<int>> get_face(vector<vector<Point3>>& face, map<Point3, int>& indexes) {
      vector<vector<int>> newface;
      for (auto& ring : face) {
        vector<int> newring = get_ring(ring, indexes);
        newface.push_back(newring);
      }
      return newface;
    }

    vector<vector<vector<int>>> get_shell(vector<vector<vector<Point3>>>& shell, map<Point3, int>& indexes) {
      vector<vector<vector<int>>> newshell;
      for (auto& face : shell) {
        vector<vector<int>> newface = get_face(face, indexes);
        newshell.push_back(newface);
      }
      return newshell;
    }

    vector<vector<vector<vector<int>>>> get_solid(vector<vector<vector<vector<Point3>>>>& solid,
                                                  map<Point3, int>& indexes) {
      vector<vector<vector<vector<int>>>> newsolid;
      for (auto& shell : solid) {
        vector<vector<vector<int>>> newshell = get_shell(shell, indexes);
        newsolid.push_back(newshell);
      }
      return newsolid;
    }

    vector<vector<vector<vector<vector<int>>>>> get_Msolid(vector<vector<vector<vector<vector<Point3>>>>>& Msolid,
                                                           map<Point3, int>& indexes) {
      vector<vector<vector<vector<vector<int>>>>> newMsolid;
      for (auto& solid : Msolid) {
        vector<vector<vector<vector<int>>>> newsolid = get_solid(solid, indexes);
        newMsolid.push_back(newsolid);
      }
      return newMsolid;
    }
}
