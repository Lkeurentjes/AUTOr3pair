
// Copyright (c) 2024. // made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#ifndef CITYOBJECT_CPP_GEOMETRYTOOLSDETRIANGULATIONPOINT3E_H
#define CITYOBJECT_CPP_GEOMETRYTOOLSDETRIANGULATIONPOINT3E_H

#include "../thirdparty/nlohmann-json/json.hpp"
#include "DefenitionsCGAL.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair {
    // detriangulation Point3E (using exact kernel)
    vector<vector<vector<Point3E>>> detriangulate(vector<vector<vector<Point3E>>> shell);
    Vector3E computeTriangleNormal(const vector<Point3E>& face);
    bool are_faces_coplanar(const vector<Point3E>& face1, const vector<Point3E>& face2);
    bool is_point_on_plane(const PlaneE& plane, const Point3E& point);
    bool points_almost_equal(const Point3E& p1, const Point3E& p2);
    bool triangles_share_edge(const vector<Point3E>& tri1, const vector<Point3E>& tri2);
    vector<vector<int>> build_graph(const vector<vector<Point3E>>& triangles);
    vector<vector<vector<Point3E>>> group_triangles_by_components(
            const vector<vector<Point3E>>& triangles,
            const vector<set<int>>& connected_components);
    vector<vector<Point3E>> merge_triangles(const vector<vector<Point3E>>& triangles);
    vector<vector<Point3E>> cut_rings(const vector<Point3E>& points);
    vector<vector<Point3E>> sort_rings(const vector<vector<Point3E>>& points);

} // AUTOr3pair


#endif //CITYOBJECT_CPP_GEOMETRYTOOLSDETRIANGULATIONPOINT3E_H
