// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef GEOMETRYTOOLSDETRIANGULATIONPOINT3_H
#define GEOMETRYTOOLSDETRIANGULATIONPOINT3_H

#include "../thirdparty/nlohmann-json/json.hpp"
#include "DefenitionsCGAL.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
    //detriangulation Point3

    vector<vector<vector<Point3>>> detriangulate(vector<vector<vector<Point3>>> shell);
    Vector3 computeTriangleNormal(const vector<Point3>& face);
    bool are_faces_coplanar(const vector<Point3>& face1, const vector<Point3>& face2);
    bool is_point_on_plane(const Plane& plane, const Point3& point);
    bool points_almost_equal(const Point3& p1, const Point3& p2);
    bool triangles_share_edge(const vector<Point3>& tri1, const vector<Point3>& tri2);
    vector<vector<int>> build_graph(const vector<vector<Point3>>& triangles);
    vector<vector<vector<Point3>>> group_triangles_by_components(
            const vector<vector<Point3>>& triangles,
            const vector<set<int>>& connected_components);
    vector<vector<Point3>> merge_triangles(const vector<vector<Point3>>& triangles);


} // AUTOr3pair

#endif //GEOMETRYTOOLSDETRIANGULATIONPOINT3_H
