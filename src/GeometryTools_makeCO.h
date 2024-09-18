
// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#ifndef GEOMETRYTOOLS_MAKECO_H
#define GEOMETRYTOOLS_MAKECO_H

#include "DefenitionsCGAL.h"
#include "GeometryTools_makeCO.h"
#include "../thirdparty/nlohmann-json/json.hpp"


using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair{
	// To Point2E
	vector<vector<Point2E>> get_routes(PolygonE& Poly, vector<Point2E>& cut);

	// To Point3(E)
	vector<Point3> make_boundary(CgalPolyhedron &poly);
  vector<vector<vector<Point3>>> get_faces(Mesh& mesh);
  bool all_distances_greater_than_tol(const vector<Point3>& points);
  vector<vector<vector<Point3>>> delete_triangle_lines(const vector<vector<vector<Point3>>>& triangles);

	// To Point3E
	vector<Point3E> make_boundary(Nef_polyhedron& poly);
	vector<vector<vector<Point3E>>> get_faces(Nef_polyhedron& poly);
  vector<vector<vector<Point3E>>> get_faces_exterior(Nef_polyhedron& poly);
	vector<vector<vector<Point3E>>> get_faces(MeshE& mesh);
  bool all_distances_greater_than_tol(const vector<Point3E>& points);
  vector<vector<vector<Point3E>>> delete_triangle_lines(const vector<vector<vector<Point3E>>>& triangles);

	vector<vector<vector<vector<Point3E>>>> get_Shells(Nef_polyhedron& poly);
	vector<vector<vector<vector<vector<Point3E>>>>> get_Solids(Nef_polyhedron& poly);

	// To index int
	vector<int> get_ring (vector<Point3E>& ring, map<Point3E, int>& indexes);
	vector<vector<int>> get_face(vector<vector<Point3E>>& face, map<Point3E, int>& indexes);
	vector<vector<vector<int>>> get_shell(vector<vector<vector<Point3E>>>& shell, map<Point3E, int>& indexes);
	vector<vector<vector<vector<int>>>> get_solid(vector<vector<vector<vector<Point3E>>>>& solid, map<Point3E, int>& indexes);
	vector<vector<vector<vector<vector<int>>>>> get_Msolid(vector<vector<vector<vector<vector<Point3E>>>>>& Msolid, map<Point3E, int>& indexes);

  vector<int> get_ring (vector<Point3>& ring, map<Point3, int>& indexes);
  vector<vector<int>> get_face(vector<vector<Point3>>& face, map<Point3, int>& indexes);
  vector<vector<vector<int>>> get_shell(vector<vector<vector<Point3>>>& shell, map<Point3, int>& indexes);
  vector<vector<vector<vector<int>>>> get_solid(vector<vector<vector<vector<Point3>>>>& solid, map<Point3, int>& indexes);
  vector<vector<vector<vector<vector<int>>>>> get_Msolid(vector<vector<vector<vector<vector<Point3>>>>>& Msolid, map<Point3, int>& indexes);
}


#endif //GEOMETRYTOOLS_MAKECO_H
