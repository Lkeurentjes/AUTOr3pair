
// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#ifndef GEOMETRYTOOLS_MAKECGAL_H
#define GEOMETRYTOOLS_MAKECGAL_H

#include "DefenitionsCGAL.h"
#include "Geometrytools_makeCGAL.h"
#include "../thirdparty/nlohmann-json/json.hpp"


using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
    // Inexact Kernel
    CgalPolyhedron::Plane_3 get_best_fitted_plane(const std::vector<Point3> &lsPts);
    void create_2Dcgal_polygon(const std::vector<Point3> &lsPts, const CgalPolyhedron::Plane_3 &plane, Polygon &outpgn);


    // Exact kernerl
    CgalPolyhedronE::Plane_3 get_best_fitted_plane(const std::vector<Point3E> &lsPts);
    void make_ring(vector<int> &inring, vector<Point3> &outring);
    CgalPolyhedronE::Plane_3 get_best_fitted_planeE(const std::vector<Point3E> &lsPts);
    void
    create_2Dcgal_polygon(const std::vector<Point3E> &lsPts, const CgalPolyhedronE::Plane_3 &plane, PolygonE &outpgn);
    void make_2D_poly(vector<Point3E> &lsPts, CgalPolyhedronE &Poly);
    void make_2D_poly(vector<Point3E> &lsPts, Nef_polyhedron &Poly);
    bool make_2D_polySMT(vector<int> &lsPts, Nef_polyhedron &Poly);
    void make_3D_poly(vector<vector<vector<int>>> &boundary, map<int, Point3E> &lsPts, CgalPolyhedronE &Poly);
    void make_3D_poly(vector<vector<vector<int>>> &boundary, map<int, Point3E> &lsPts, Nef_polyhedron &Poly);

    // make vectors of CGAL objects
    void make_ring(vector<int> &inring, vector<Point3E> &outring);
    void make_rings(vector<vector<int>> &face, vector<Nef_polyhedron> &rings, map<Point3E, int> &indexes);
    void make_rings(vector<vector<int>> &face, vector<vector<Point3E>> &rings, vector<Point3E> &points,
                    map<Point3E, int> &indexes);
    void make_rings(vector<vector<int>> &face, vector<vector<Point3>> &rings, vector<Point3> &points,
                    map<Point3, int> &indexes);
    void make_shell(vector<vector<vector<int>>> &shell, Nef_polyhedron &polyShell, map<Point3E, int> &indexes);
    bool make_shell(vector<vector<vector<int>>> &shell, MeshE &MeshShell, map<Point3E, int> &indexes);
    void make_PolygonSoupShell(vector<vector<vector<int>>> &shell, MeshE &MeshShell, map<Point3E, int> &indexes);
    bool make_shell(vector<vector<vector<int>>> &shell, Mesh &MeshShell, map<Point3, int> &indexes);
    void make_shell3D(vector<vector<vector<int>>> &s, Nef_polyhedron &shells, map<Point3E, int> &indexes);
    void
    make_shells(vector<vector<vector<vector<int>>>> &solid, vector<Nef_polyhedron> &shells, map<Point3E, int> &indexes);
    void make_shells(vector<vector<vector<vector<int>>>> &solid, vector<CgalPolyhedronE> &shells,
                     map<Point3E, int> &indexes);
    void make_Solids(vector<vector<vector<vector<vector<int>>>>> &Csolid, vector<vector<Nef_polyhedron>> &Solids,
                     map<Point3E, int> &indexes);
}

#endif //GEOMETRYTOOLS_MAKECGAL_H
