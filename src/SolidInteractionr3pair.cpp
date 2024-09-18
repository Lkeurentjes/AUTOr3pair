// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#include "SolidInteractionr3pair.h"

namespace AUTOr3pair{
	vector<vector<vector<vector<vector<int>>>>> SolidIr3pair501(vector<vector<vector<vector<vector<int>>>>>& Boundary) {
		// INTERSECTION SOLIDS
		vector<vector<vector<vector<vector<int>>>>> newboundaries;

		vector<vector<Nef_polyhedron>> solids;
		map<Point3E, int> indexes;
		make_Solids(Boundary, solids, indexes);
		for (int i = 0; i < solids.size(); ++i) {
			for (int j = i + 1; j < solids.size(); ++j) {
				Nef_polyhedron intersection = solids[i][0] * solids[j][0];
				if (intersection.number_of_volumes() >= 2) {
					// check overlap vs merge tol
					if (STANDARDS["OutputParameters"]["Debugging"]) {
						std::cout << "original " << CalculateVolume(solids[i][0]) << endl;
						std::cout << "intersection " << CalculateVolume(intersection) << endl;
						std::cout << "overlap% " << (CalculateVolume(intersection) / CalculateVolume(solids[i][0])) << endl;
					}

					if ((CalculateVolume(intersection) / CalculateVolume(solids[i][0])) > STANDARDS["UseCaseRepair"][
						"MergeTol"]) {
						solids[i][0] += solids[j][0];
						for (int b = 0; b < Boundary.size(); ++b) {
							if (b == i) {
								vector<vector<vector<vector<int>>>> newsolid;
								vector<vector<vector<vector<Point3E>>>> newshell = get_Shells(solids[i][0]);
								vector<vector<vector<int>>> newoutshell = get_shell(newshell[0], indexes);
								newsolid.push_back(newoutshell);
								for (int s = 1; s < Boundary[b].size(); ++s) { newsolid.push_back(Boundary[b][s]); }
								newboundaries.push_back(newsolid);
							}
							else if (b == j) {
								for (int s = 1; s < Boundary[b].size(); ++s) {
									newboundaries[i].push_back(Boundary[b][s]);
								}
							}
							else { newboundaries.push_back(Boundary[b]); }
						}
						return newboundaries; // directly return: only one merge per repair
					}
					else {
						// otherwise first stays same, second get smaller
						solids[j][0] -= solids[i][0];
						for (int b = 0; b < Boundary.size(); ++b) {
							if (b == j && CalculateVolume(solids[j][0]) > 0 ) {
								std::cout  << "j " << CalculateVolume(solids[j][0]) << endl;
								vector<vector<vector<vector<int>>>> newsolid;
								vector<vector<vector<vector<Point3E>>>> newshell = get_Shells(solids[j][0]);
								vector<vector<vector<int>>> newoutshell = get_shell(newshell[0], indexes);
								newsolid.push_back(newoutshell);
								for (int s = 1; s < Boundary[b].size(); ++s) {
									if (solids[j][0] > solids[j][s]) {
										newsolid.push_back(Boundary[b][s]);
									} else {
										newboundaries[i].push_back(Boundary[b][s]);
									}
								}
								newboundaries.push_back(newsolid);
							}
							else if(b != j) { newboundaries.push_back(Boundary[b]); }

						}
						return newboundaries; // directly return: only one difference per repair
					}
				}
			}
		}
		return newboundaries;
	}

	vector<vector<vector<vector<vector<int>>>>> SolidIr3pair502(vector<vector<vector<vector<vector<int>>>>>& Boundary) {
		// DUPLICATED SOLIDS
		vector<vector<vector<vector<vector<int>>>>> newboundaries;

		vector<vector<Nef_polyhedron>> solids;
		map<Point3E, int> indexes;
		make_Solids(Boundary, solids, indexes);

		set<int> duplicateSolids;
		for (int i = 0; i < solids.size(); ++i) {
			if (!duplicateSolids.contains(i)) {
				newboundaries.push_back(Boundary[i]);
				for (int j = i + 1; j < solids.size(); ++j) {
					if (!(solids[i][0] > solids[j][0])) { duplicateSolids.insert(j); }
				}
			}
		}
		return newboundaries;
	}

	vector<vector<vector<vector<vector<vector<int>>>>>>  SolidIr3pair503(vector<vector<vector<vector<vector<int>>>>>& Boundary) {
		// DISCONNECTED SOLIDS
		vector<vector<vector<vector<vector<vector<int>>>>>>  newboundaries;
		vector<vector<Nef_polyhedron>> solids;
		map<Point3E, int> indexes;
		make_Solids(Boundary, solids, indexes);

		// make connectivity graph
		std::vector<std::vector<int>> graph(Boundary.size());
		for (int i = 0; i <Boundary.size(); ++i) {
			for (int j = i + 1; j < Boundary.size(); ++j) {
				// Check if polyhedra[i] and polyhedra[j] are connected by checking intersection is non empty
				Nef_polyhedron intersection = solids[i][0] * solids[j][0];
				if (!intersection.is_empty()) {
					graph[i].push_back(j);
					graph[j].push_back(i);
				}
			}
		}

		// find connected components
		int n = graph.size();
		vector<bool> visited(n, false);
		vector<vector<int>> components;

		for (int i = 0; i < n; ++i) {
			if (!visited[i]) {
				vector<int> component;
				queue<int> q;
				q.push(i);
				visited[i] = true;

				while (!q.empty()) {
					int v = q.front(); q.pop();
					component.push_back(v);

					for (int u : graph[v]) {
						if (!visited[u]) {
							q.push(u);
							visited[u] = true;
						}
					}
				}

				components.push_back(component);

			}

		}
		for (auto& connected: components) {
			vector<vector<vector<vector<vector<int>>>>> newCsolid;
			for (auto& solid: connected) {
				newCsolid.push_back(Boundary[solid]);
			}
			newboundaries.push_back(newCsolid);
		}

		return newboundaries;
	}
} // AUTOr3pair
