// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef AUTOMATIC_REPAIR_V1_GEOMETRYTOOLS_H
#define AUTOMATIC_REPAIR_V1_GEOMETRYTOOLS_H

#include "../thirdparty/nlohmann-json/json.hpp"
#include "DefenitionsCGAL.h"
#include "Geometrytools.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
    // Utilitys and calculations
    // Making CGAL objects can be found in Geometrytools_makeCGAL
    // Making CityObject (CO) can be found in Geometrytools_makeCO

    // report tools
    json counter(json& geometry);

    // geometry calculations
    bool all_points_on_same_plane(set<Point3> &lsPts);
    bool all_points_on_same_planeSMT( set<Point3E> &lsPts);
    bool all_points_on_same_plane(set<Point3E> &lsPts);
    int findPointIndexInPolygon(const Polygon& polygon, const Point2& point);
    double CalculateVolume(const Nef_polyhedron& poly);
    bool are_points_collinear(const vector<Point3>& points);
    double compute_area_from_3d_polygon(const std::vector<Point3E>& points3);


    //geometry transformations
    void create_convex_hull(const std::vector<Point3>& lsPts, CgalPolyhedron &outpgn);
    void remove_unused_vertices(MeshE& mesh);
    void remove_unused_vertices(Mesh& mesh);

    // Orientation calculations
    Vector3 compute_polygon_normal(vector<Point3>& points);
    Vector3E compute_polygon_normal(vector<Point3E>& points);
    vector<vector<int>> flip_face(vector<vector<int>>& face);
    vector<vector<vector<int>>> flip_shell(vector<vector<vector<int>>>& shell);

    bool handle_self_intersections(Mesh& mesh);

    //detriangulation helpers
    void dfs(int node, const vector<vector<int>>& adjacency_list, vector<bool>& visited, set<int>& component);
    vector<set<int>> find_connected_components(const vector<vector<int>>& adjacency_list);


} // AUTOr3pair

#endif //AUTOMATIC_REPAIR_V1_GEOMETRYTOOLS_H
