
// Copyright (c) 2024. // made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes

#include "BuildingPartr3pair.h"
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>

namespace AUTOr3pair {

    json BuildingPartr3pair601(json &part1, json &part2, string &LOD) {
      json Repairs;
      vector<json> geomR1;
      vector<json> geomR2;

      vector<json> geom1;
      vector<json> geom2;

      vector<vector<Nef_polyhedron>> OuterShell1;
      vector<vector<Nef_polyhedron>> OuterShell2;
      map<Point3E, int> indexes;

      for (auto &geom: part1["geometry"]) {
        string geomtype = geom["type"];
        if (LOD.find(geom["lod"]) != std::string::npos && geomtype != "MultiSurface" &&
            geomtype != "CompositeSurface") {
          geom1.push_back(geom);
          if (geomtype == "Solid") {
            vector<vector<vector<int>>> shell = geom["boundaries"][0];
            Nef_polyhedron NefShell;
            make_shell3D(shell, NefShell, indexes);
            OuterShell1.push_back({NefShell});
          }
          if (geomtype == "MultiSolid" || geomtype == "CompositeSolid") {
            vector<Nef_polyhedron> shells;
            for (auto &Solids: geom["boundaries"]) {
              vector<vector<vector<int>>> shell = Solids[0];
              Nef_polyhedron NefShell;
              make_shell3D(shell, NefShell, indexes);
              shells.push_back(NefShell);
            }
            OuterShell1.push_back(shells);
          }
        } else {
          geomR1.push_back(geom);
        }
      }
      for (auto &geom: part2["geometry"]) {
        string geomtype = geom["type"];
        if (LOD.find(geom["lod"]) != std::string::npos && geomtype != "MultiSurface" &&
            geomtype != "CompositeSurface") {
          geom2.push_back(geom);
          if (geomtype == "Solid") {
            vector<vector<vector<int>>> shell = geom["boundaries"][0];
            Nef_polyhedron NefShell;
            make_shell3D(shell, NefShell, indexes);
            OuterShell2.push_back({NefShell});
          }
          if (geomtype == "MultiSolid" || geomtype == "CompositeSolid") {
            vector<Nef_polyhedron> shells;
            for (auto &Solids: geom["boundaries"]) {
              vector<vector<vector<int>>> shell = Solids[0];
              Nef_polyhedron NefShell;
              make_shell3D(shell, NefShell, indexes);
              shells.push_back(NefShell);
            }
            OuterShell2.push_back(shells);
          }
        } else {
          geomR2.push_back(geom);
        }
      }

      // Initialize changes1 and changes2 based on the sizes of OuterShell1 and OuterShell2
      vector<vector<bool>> changes1(OuterShell1.size()); // Number of rows based on OuterShell1
      vector<vector<bool>> changes2(OuterShell2.size()); // Number of rows based on OuterShell2

      // Populate changes1 to match the structure of OuterShell1
      for (int i = 0; i < OuterShell1.size(); ++i) {
        int m1 = OuterShell1[i].size(); // Number of columns in the current row of OuterShell1
        changes1[i] = vector<bool>(m1, false); // Initialize this row with 'false'
      }

      // Populate changes2 to match the structure of OuterShell2
      for (int i = 0; i < OuterShell2.size(); ++i) {
        int m2 = OuterShell2[i].size(); // Number of columns in the current row of OuterShell2
        changes2[i] = vector<bool>(m2, false); // Initialize this row with 'false'
      }

      for (int i = 0; i < OuterShell1.size(); ++i) {
        for (int j = 0; j < OuterShell1[i].size(); ++j) {
          if (!OuterShell1[i][j].is_valid() || OuterShell1[i][j].is_empty()){
            continue;
          }
          for (int k = 0; k < OuterShell2.size(); ++k) {
            for (int l = 0; l < OuterShell2[k].size(); ++l) {
              if (!OuterShell2[k][l].is_valid() || OuterShell2[k][l].is_empty()){
                continue;
              }
              Nef_polyhedron intersection = OuterShell1[i][j] * OuterShell2[k][l];

              if (intersection.number_of_volumes() >= 2){
                // check overlap vs merge tol
                if (STANDARDS["OutputParameters"]["Debugging"]) {
                  std::cout << "original " << CalculateVolume(OuterShell1[i][j]) << endl;
                  std::cout << "intersection " << CalculateVolume(intersection) << endl;
                  std::cout << "overlap% " << (CalculateVolume(intersection) / CalculateVolume(OuterShell1[i][j]))
                            << endl;
                }

                if ((CalculateVolume(intersection) / CalculateVolume(OuterShell1[i][j])) > STANDARDS["UseCaseRepair"][
                        "MergeTol"]) {
                  OuterShell1[i][j] += OuterShell2[k][l];
                  OuterShell2[k][l] -= OuterShell2[k][l];
                  changes1[i][j] = true;
                  changes2[k][l] = true;
                } else {
                  OuterShell2[k][l] -= OuterShell1[i][j];
                  changes2[k][l] = true;
                }
              }
            }
          }
        }
      }


      for (int i = 0; i < geom1.size(); ++i) {
        string geomtype = geom1[i]["type"];
        if (geomtype == "Solid") {
          if (changes1[i][0]) {
            if (OuterShell1[i][0].number_of_facets() == 0){
              continue;
            }
            vector<vector<vector<Point3E>>> outShell = get_faces_exterior(OuterShell1[i][0]);
            vector<vector<vector<int>>> intshell = get_shell(outShell, indexes);
            geom1[i]["boundaries"][0] = intshell;
          }
        }
        if (geomtype == "MultiSolid" || geomtype == "CompositeSolid") {
          int erased = 0;
          for (int j = 0; j < OuterShell1[i].size(); ++j) {
            if (changes1[i][j]) {
              if (OuterShell1[i][j].number_of_facets() == 0){
                geom1[i]["boundaries"].erase(geom1[i]["boundaries"].begin() + j - erased);
                erased +=1;
                continue;
              }
              vector<vector<vector<Point3E>>> outShell = get_faces_exterior(OuterShell1[i][j]);
              vector<vector<vector<int>>> intshell = get_shell(outShell, indexes);
              geom1[i]["boundaries"][j- erased][0] = intshell;
            }
          }
          if (geom1[i]["boundaries"].empty()){
            continue;
          }
        }
        geom1[i].erase("material");
        geom1[i].erase("semantics");
        geomR1.push_back(geom1[i]);
      }

      for (int i = 0; i < geom2.size(); ++i) {
        string geomtype = geom2[i]["type"];
        if (geomtype == "Solid") {
          if (changes2[i][0]) {
            if (OuterShell2[i][0].number_of_facets() == 0){
              continue;
            }
            vector<vector<vector<Point3E>>> outShell = get_faces_exterior(OuterShell2[i][0]);
            vector<vector<vector<int>>> intshell = get_shell(outShell, indexes);
            geom2[i]["boundaries"][0] = intshell;
          }
        }
        if (geomtype == "MultiSolid" || geomtype == "CompositeSolid") {
          int erased = 0;
          for (int j = 0; j < OuterShell2[i].size(); ++j) {
            if (changes2[i][j]) {
              if (OuterShell2[i][j].number_of_facets() == 0){
                geom2[i]["boundaries"].erase(geom2[i]["boundaries"].begin() + j - erased);
                erased +=1;
                continue;
              }
              vector<vector<vector<Point3E>>> outShell = get_faces_exterior(OuterShell2[i][j]);
              vector<vector<vector<int>>> intshell = get_shell(outShell, indexes);
              geom2[i]["boundaries"][j-erased][0] = intshell;
            }
          }
          if (geom2[i]["boundaries"].empty()){
            continue;
          }
        }
        geom2[i].erase("material");
        geom2[i].erase("semantics");
        geomR2.push_back(geom2[i]);
      }

      part1["geometry"] = geomR1;
      part2["geometry"] = geomR2;

      if (part2["geometry"].empty()){
        part2.erase("geometry");
      }
      return Repairs;
    }

    json BuildingPartr3pair601(json &part1, string &LOD) {
      json Repairs;
      vector<json> geomR1;

      vector<json> geom1;

      vector<vector<Nef_polyhedron>> OuterShell1;
      map<Point3E, int> indexes;

      for (auto &geom: part1["geometry"]) {
        string geomtype = geom["type"];
        if (LOD.find(geom["lod"]) != std::string::npos && geomtype != "MultiSurface" &&
            geomtype != "CompositeSurface") {
          geom1.push_back(geom);
          if (geomtype == "Solid") {
            vector<vector<vector<int>>> shell = geom["boundaries"][0];
            Nef_polyhedron NefShell;
            make_shell3D(shell, NefShell, indexes);
            OuterShell1.push_back({NefShell});
          }
          if (geomtype == "MultiSolid" || geomtype == "CompositeSolid") {
            vector<Nef_polyhedron> shells;
            for (auto &Solids: geom["boundaries"]) {
              vector<vector<vector<int>>> shell = Solids[0];
              Nef_polyhedron NefShell;
              make_shell3D(shell, NefShell, indexes);
              shells.push_back(NefShell);
            }
            OuterShell1.push_back(shells);
          }
        } else {
          geomR1.push_back(geom);
        }
      }

      // Initialize changes1 and changes2 based on the sizes of OuterShell1 and OuterShell2
      vector<vector<bool>> changes1(OuterShell1.size()); // Number of rows based on OuterShell1

      // Populate changes1 to match the structure of OuterShell1
      for (int i = 0; i < OuterShell1.size(); ++i) {
        int m1 = OuterShell1[i].size(); // Number of columns in the current row of OuterShell1
        changes1[i] = vector<bool>(m1, false); // Initialize this row with 'false'
      }

      for (int i = 0; i < OuterShell1.size(); ++i) {
        for (int j = 0; j < OuterShell1[i].size(); ++j) {
          if (!OuterShell1[i][j].is_valid() || OuterShell1[i][j].is_empty()){
            continue;
          }
          for (int k = i+1; k < OuterShell1.size(); ++k) {
            for (int l = 0; l < OuterShell1[k].size(); ++l) {
              if (!OuterShell1[k][l].is_valid() || OuterShell1[k][l].is_empty() ){
                continue;
              }
              Nef_polyhedron intersection = OuterShell1[i][j] * OuterShell1[k][l];

              if (intersection.number_of_volumes() >= 2){
                // check overlap vs merge tol
                if (STANDARDS["OutputParameters"]["Debugging"]) {
                  std::cout << "original " << CalculateVolume(OuterShell1[i][j]) << endl;
                  std::cout << "intersection " << CalculateVolume(intersection) << endl;
                  std::cout << "overlap% " << (CalculateVolume(intersection) / CalculateVolume(OuterShell1[i][j]))
                            << endl;
                }

                if ((CalculateVolume(intersection) / CalculateVolume(OuterShell1[i][j])) > STANDARDS["UseCaseRepair"][
                        "MergeTol"]) {
                  OuterShell1[i][j] += OuterShell1[k][l];
                  OuterShell1[k][l] -= OuterShell1[k][l];
                  changes1[i][j] = true;
                  changes1[k][l] = true;
                } else {
                  OuterShell1[k][l] -= OuterShell1[i][j];
                  changes1[k][l] = true;
                }
              }
            }
          }
        }
      }

      for (int i = 0; i < geom1.size(); ++i) {
        string geomtype = geom1[i]["type"];
        if (geomtype == "Solid") {
          if (changes1[i][0]) {
            if (OuterShell1[i][0].number_of_facets() == 0){
              continue;
            }
            vector<vector<vector<Point3E>>> outShell = get_faces_exterior(OuterShell1[i][0]);
            vector<vector<vector<int>>> intshell = get_shell(outShell, indexes);
            geom1[i]["boundaries"][0] = intshell;
          }
        }
        if (geomtype == "MultiSolid" || geomtype == "CompositeSolid") {
          int erased = 0;
          for (int j = 0; j < OuterShell1[i].size(); ++j) {
            if (changes1[i][j]) {
              if (OuterShell1[i][j].number_of_facets() == 0){
                geom1[i]["boundaries"].erase(geom1[i]["boundaries"].begin() + j- erased);
                erased +=1;
                continue;
              }
              vector<vector<vector<Point3E>>> outShell = get_faces_exterior(OuterShell1[i][j]);
              vector<vector<vector<int>>> intshell = get_shell(outShell, indexes);
              geom1[i]["boundaries"][j-erased][0] = intshell;
            }
          }
          if (geom1[i]["boundaries"].empty()){
            continue;
          }
        }
        geom1[i].erase("material");
        geom1[i].erase("semantics");
        geomR1.push_back(geom1[i]);
      }


      part1["geometry"] = geomR1;

      return Repairs;
    }
} // AUTOr3pair