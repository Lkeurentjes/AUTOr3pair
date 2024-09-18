// Copyright (c) 2024. made for a proof of concept for thesis: Automatic repair of 3 citymodels by Lisa Keurentjes


#include "Ringr3pair.h"
#include "Defenitions.h"
#include "Geometrytools.h"
#include "DefenitionsCGAL.h"
#include "Geometrytools_makeCGAL.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair{
	vector<vector<int>> Ringr3pair101(vector<vector<int>>& surface) {
		// TOO FEW POINTS
		if (surface[0].size() < 3) {
			vector<vector<int>> empty;
			return empty;
		}
		vector<vector<int>> newsurface;
		for (int i = 0; i < surface.size(); ++i) { if (surface[i].size() >= 3) { newsurface.push_back(surface[i]); } }
		return newsurface;
	};

	vector<vector<int>> Ringr3pair102(vector<vector<int>>& surface) {
		// CONSECUTIVE POINTS SAME
		vector<vector<int>> newsurface;
		for (int i = 0; i < surface.size(); ++i) {
			vector<int> newring;
			for (int j = 0; j < surface[i].size(); ++j) {
				bool sameIndex;
				if (j != 0) { sameIndex = (surface[i][j] == surface[i][j - 1]); }
				else { sameIndex = (surface[i][j] == surface[i].back()); }

				if (!sameIndex) {
					// NOT SAME POINT
          newring.push_back(surface[i][j]);
				}
			}
			newsurface.push_back(newring);
		}
		return newsurface;
	};

	vector<vector<int>> Ringr3pair104(vector<vector<int>>& surface) {
		// RING SELF INTERSECTION
		vector<vector<int>> newsurface;

		vector<Point3> verticesP3 = VERTICES.get_verticesPoint3();

		for (int i = 0; i < surface.size(); ++i) {
			vector<int> newring;
			vector<Point3> Pnewring;

			vector<Point3> ring;
			for (int j = 0; j < surface[i].size(); ++j) { ring.push_back(verticesP3[surface[i][j]]); }

      // check if collinear
      if (are_points_collinear(ring)){
        if( i == 0){
          return newsurface;
        }else{
          continue;
        }
      }

			CgalPolyhedron::Plane_3 best_plane = get_best_fitted_plane(ring);
			Polygon pgn;
			create_2Dcgal_polygon(ring, best_plane, pgn);
			bool test = pgn.is_simple();
			if (test) { newsurface.push_back(surface[i]); }
			else {
				vector<Point2> convex_hull;
				CGAL::ch_graham_andrew(pgn.begin(), pgn.end(), std::back_inserter(convex_hull));

				for (int p = 0; p < convex_hull.size(); ++p) {
					int index = findPointIndexInPolygon(pgn, convex_hull[p]);
					newring.push_back(surface[i][index]);
					Pnewring.push_back(ring[index]);
				}
        Vector3 start = compute_polygon_normal(ring);
        Vector3 end = compute_polygon_normal(Pnewring);
        double dot_product = CGAL::to_double(start * end);
        // if negative they point different direction
        if (dot_product<0) {
          reverse(newring.begin(), newring.end());
        }
				newsurface.push_back(newring);
			}
		}
		return newsurface;
	}
} // AUTOr3pair
