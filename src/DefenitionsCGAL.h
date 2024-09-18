// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef DEFENITIONSCGAL_H
#define DEFENITIONSCGAL_H

// CGAL kernel
#include <CGAL/basic.h>
#include <CGAL/Exact_integer.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Filtered_extended_homogeneous.h>
#include <CGAL/Homogeneous.h>

// CGAL "Objects"
#include <CGAL/Line_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>

// CGAL Nef
#include <CGAL/Nef_polyhedron_2.h>
#include <CGAL/Nef_polyhedron_3.h>

// CGAL Mesh
#include <CGAL/Surface_mesh.h>
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_stop_predicate.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/region_growing.h>
#include <CGAL/Polygon_mesh_processing/remesh_planar_patches.h>
#include <CGAL/Polygon_mesh_processing/random_perturbation.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/detect_features.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/clip.h>
#include <CGAL/Polygon_mesh_processing/self_intersections.h>
#include <CGAL/Polygon_mesh_processing/locate.h>
#include <CGAL/Polygon_mesh_processing/smooth_shape.h>
#include <CGAL/Polygon_mesh_processing/repair.h>
#include <CGAL/Polygon_mesh_processing/polygon_mesh_to_polygon_soup.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>
#include <CGAL/boost/graph/Euler_operations.h>
#include <boost/property_map/vector_property_map.hpp>

// CGAL Triangulations
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_id_2.h>


// CGAL Calculations
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/algorithm.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/linear_least_squares_fitting_3.h>
#include <CGAL/minkowski_sum_3.h>
#include <CGAL/squared_distance_3.h>
#include <CGAL/alpha_wrap_3.h>
#include <CGAL/Kd_tree.h>
#include <CGAL/Kd_tree_rectangle.h>
#include <CGAL/Search_traits_3.h>
#include <CGAL/Orthogonal_k_neighbor_search.h>

// Draws for CGAL --> use with debugging
#include <CGAL/draw_nef_3.h>
#include <CGAL/draw_polyhedron.h>
#include <CGAL/draw_polygon_with_holes_2.h>
#include <CGAL/draw_polygon_2.h>
#include <CGAL/draw_polygon_set_2.h>
#include <CGAL/draw_triangulation_2.h>
#include <CGAL/draw_polyhedron.h>
#include <CGAL/draw_triangulation_3.h>
#include <CGAL/draw_surface_mesh.h>

#include <iostream>
#include <string>
#include "nlohmann-json/json.hpp"


using json = nlohmann::json;
using namespace std;

struct FaceInfo2{
	FaceInfo2() {
	}

	int nesting_level;

	bool in_domain() { return nesting_level % 2 == 1; }
};

const double PI = CGAL_PI;

//// inexact kernel
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// CGAL geometries inexact kernel
typedef K::Point_2 Point2;
typedef K::Line_2 line2;
typedef K::Point_3 Point3;
typedef K::Vector_3 Vector3;
typedef K::Triangle_3 Triangle3;
typedef K::Segment_3 Segment3;
typedef K::Tetrahedron_3 Tetrahedron;
typedef K::Plane_3 Plane;
typedef CGAL::Bbox_3 Bbox3;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Polygon_set_2<K> Polygon_set_2;
typedef CGAL::Polyhedron_3<K> CgalPolyhedron;
typedef CGAL::Kd_tree<K, vector<Point3>::iterator> KdTreeP;
typedef CGAL::Kd_tree_rectangle<K> KdTreeRect;
typedef CGAL::Search_traits_3<K> TreeTraits;
typedef CGAL::Kd_tree<TreeTraits> KdTree;
typedef CGAL::Orthogonal_k_neighbor_search<TreeTraits> Neighbor_search;
typedef Neighbor_search::Tree NBTree;
typedef K::FT FT;


//// exact kernel (for NEF actions)
typedef CGAL::Exact_predicates_exact_constructions_kernel KE;
// CGAL geometries exact kernel
typedef KE::Point_2 Point2E;
typedef KE::Line_2 line2E;
typedef KE::Point_3 Point3E;
typedef KE::Plane_3 PlaneE;
typedef KE::Vector_3 Vector3E;
typedef CGAL::Polygon_2<KE> PolygonE;
typedef CGAL::Polygon_with_holes_2<KE> PolygonHolesE;
typedef CGAL::Polyhedron_3<KE> CgalPolyhedronE;
typedef CGAL::Polyhedron_incremental_builder_3<CgalPolyhedronE::HalfedgeDS> Builder;
typedef CGAL::Nef_polyhedron_3<KE> Nef_polyhedron;
typedef CGAL::Aff_transformation_3<KE> Transformation;
typedef CgalPolyhedronE::HalfedgeDS HalfedgeDS;

// Triangulations etc.
typedef CGAL::Triangulation_vertex_base_with_id_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2, K> Fbb;
typedef CGAL::Constrained_triangulation_face_base_2<K, Fbb> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> TDS;
typedef CGAL::Exact_intersections_tag Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag> CDT;
typedef CDT::Point Point;
typedef CDT::Edge Edge;
typedef CGAL::Delaunay_triangulation_3<K> Delaunay;
typedef CGAL::Delaunay_triangulation_3<KE> DelaunayE;

// Meshes
typedef CGAL::Surface_mesh<Point3> Mesh;
typedef boost::graph_traits<Mesh>::vertex_descriptor vertex_descriptor ;
typedef boost::graph_traits<Mesh>::face_descriptor face_descriptor;
typedef boost::graph_traits<Mesh>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<Mesh>::faces_size_type faces_size_type;
typedef Mesh::Property_map<face_descriptor, faces_size_type> FCCmap;
typedef CGAL::Face_filtered_graph<Mesh> Filtered_graph;

typedef CGAL::Surface_mesh<Point3E> MeshE;
typedef boost::graph_traits<MeshE>::halfedge_descriptor Halfedge_descriptorE;
typedef boost::graph_traits<MeshE>::face_descriptor face_descriptorE;
typedef boost::graph_traits<MeshE>::faces_size_type faces_size_typeE;
typedef MeshE::Property_map<face_descriptorE, faces_size_typeE> FCCmapE;
typedef CGAL::Face_filtered_graph<MeshE> Filtered_graphE;

namespace PMP = CGAL::Polygon_mesh_processing;
namespace SMS = CGAL::Surface_mesh_simplification;



// Nef polyhedron stuff
typedef Nef_polyhedron::Nef_polyhedron_S2 Nef_polyhedron_S2;
typedef Nef_polyhedron_S2::SVertex_const_handle SVertex_const_handle;
typedef Nef_polyhedron_S2::SHalfedge_const_handle SHalfedge_const_handle;
typedef Nef_polyhedron_S2::SHalfloop_const_handle SHalfloop_const_handle;
typedef Nef_polyhedron_S2::SFace_const_handle Sface_const_handle;
typedef Nef_polyhedron_S2::SFace_const_iterator SFace_const_iterator;
typedef Nef_polyhedron_S2::SFace_cycle_const_iterator SFace_cycle_const_iterator;

typedef Nef_polyhedron::Vertex_const_handle Vertex_const_handle;
typedef Nef_polyhedron::Halfedge_const_handle Halfedge_const_handle;
typedef Nef_polyhedron::Halffacet_const_handle Halffacet_const_handle;
typedef Nef_polyhedron::SHalfedge_const_handle SHalfedge_const_handle;
typedef Nef_polyhedron::SHalfloop_const_handle SHalfloop_const_handle;
typedef Nef_polyhedron::SFace_const_handle SFace_const_handle;
typedef Nef_polyhedron::Volume_const_iterator Volume_const_iterator;
typedef Nef_polyhedron::Shell_entry_const_iterator Shell_entry_const_iterator;
typedef Nef_polyhedron::Vertex_const_iterator Vertex_const_iterator;

//Shell explorer for Nef Polyhedron
class Shell_explorer{
	vector<vector<vector<Point3E>>> faces;

public:
	Shell_explorer() {
	}

	void visit(Vertex_const_handle v) {
		// Optional: Implement if needed for traversing vertices
	}

	void visit(Halfedge_const_handle) {
		// This method might be used if you need specific actions on halfedges
	}

	void visit(Halffacet_const_handle f) {
		vector<vector<Point3E>> face_vertices;
		// Assuming f points to a halffacet, and we want to iterate over its cycle of s-halfedges{
		for (auto edge = f->facet_cycles_begin(); edge != f->facet_cycles_end(); ++edge) {
			// Check if the cycle is an s-halfedge (which we can iterate over)
			if (edge.is_shalfedge()) {
				// The cycle element is an s-halfedge, we can now iterate over the cycle
				vector<Point3E> ring_vertices;
				SHalfedge_const_handle sedge = edge;
				SHalfedge_const_handle start = sedge;
				do {
					// Accessing the source vertex of the s-halfedge
					auto vertex = sedge->source()->source();
					Point3E p = vertex->point();
					ring_vertices.push_back(p);
					sedge = sedge->prev(); // Move to the next s-halfedge in the cycle
				}
				while (sedge != start); // Ensure we loop around the cycle once
				face_vertices.push_back(ring_vertices);
			}
		}
		faces.push_back(face_vertices);
	}

	void visit(SHalfedge_const_handle) {
		// Optional: Implement if needed for traversing s-halfedges
	}

	void visit(SHalfloop_const_handle) {
		// Optional: Implement if needed for traversing s-halfloops
	}

	void visit(SFace_const_handle) {
		// Optional: Implement if needed for specific actions on s-faces
	}

	const vector<vector<vector<Point3E>>>& get_faces() const { return faces; }
	void clear_faces() { faces.clear(); }
};



#endif //DEFENITIONSCGAL_H
