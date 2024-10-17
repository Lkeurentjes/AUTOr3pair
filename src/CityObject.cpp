// Copyright (c) 2024. made for a proof of concept for thesis: Automatic repair of 3 citymodels by Lisa Keurentjes

#include "CityObject.h"
#include <iostream>

#include "Defenitions.h"
#include "DefenitionsCGAL.h"
#include "Geometrytools.h"

#include "MultiSurface.h"
#include "CompositeSurface.h"
#include "Solid.h"
#include "CompositeSolid.h"
#include "MultiSolid.h"
#include "tu3djson_convert.h"


using json = nlohmann::json;
using namespace std;


namespace AUTOr3pair {
    CityObject::CityObject(string name, json object, bool repair) {
      id = name;
      type = object["type"];

      RepairReport["id"] = id;
      RepairReport["type"] = type;
      RepairReport["repaired"] = false;
      RepairReport["repairs"] = json::array();
      RepairReport["Primitives"] = json::array();

      CObject = object;


      if (repair) {
        vector<json> RepairedGeometries;
        for (int i = 0; i < CObject["geometry"].size(); ++i) {
          vector<json> rgeom;
          if (LOD != "" && CObject["geometry"][i]["lod"] != LOD){
            // don't repair if not given LOD for repair
            RepairedGeometries.push_back(CObject["geometry"][i]);
          } else if (CObject["geometry"][i]["type"] == "MultiSurface") {
            AUTOr3pair::MultiSurface geometry(id, i, CObject["geometry"][i]);
            rgeom = GeometryRepair(geometry, i);
            RepairedGeometries.insert(RepairedGeometries.end(), rgeom.begin(), rgeom.end());
          } else if (CObject["geometry"][i]["type"] == "CompositeSurface") {
            AUTOr3pair::CompositeSurface geometry(id, i, CObject["geometry"][i]);
            rgeom = GeometryRepair(geometry, i);
            RepairedGeometries.insert(RepairedGeometries.end(), rgeom.begin(), rgeom.end());
          } else if (CObject["geometry"][i]["type"] == "Solid") {
            AUTOr3pair::Solid geometry(id, i, CObject["geometry"][i]);
            rgeom = GeometryRepair(geometry, i);
            RepairedGeometries.insert(RepairedGeometries.end(), rgeom.begin(), rgeom.end());
          } else if (CObject["geometry"][i]["type"] == "MultiSolid") {
            AUTOr3pair::MultiSolid geometry(id, i, CObject["geometry"][i]);
            rgeom = GeometryRepair(geometry, i);
            RepairedGeometries.insert(RepairedGeometries.end(), rgeom.begin(), rgeom.end());
          } else if (CObject["geometry"][i]["type"] == "CompositeSolid") {
            AUTOr3pair::CompositeSolid geometry(id, i, CObject["geometry"][i]);
            rgeom = GeometryRepair(geometry, i);
            RepairedGeometries.insert(RepairedGeometries.end(), rgeom.begin(), rgeom.end());
          }
        }

        json OGgeom = CObject["geometry"];
        CObject["geometry"] = {};
        for (auto geometry: RepairedGeometries) {
          if (!geometry["boundaries"].empty()) {
            CObject["geometry"].push_back(geometry);
          }
        }
        if (CObject["geometry"].empty()) {
          CObject.erase("geometry");
          if (CObject.contains("geographicalExtent")){
            CObject.erase("geographicalExtent");
          }
        }

        // change "geographicalExtent" if needed
        if (CObject.contains("geographicalExtent") && OGgeom != CObject["geometry"]){
          set<int> all_vertices_CO;
          for (auto& geom: CObject["geometry"]) {
            recursive_all_indexes(geom["boundaries"], all_vertices_CO);
          }
          CObject["geographicalExtent"] = VERTICES.get_Geographical_extendSubSet(all_vertices_CO);
        }

      } else {
        for (int i = 0; i < CObject["geometry"].size(); ++i) {
          json primitive;
          primitive["id"] = i;
          primitive["type"] = CObject["geometry"][i]["type"];
          primitive["repaired"] = false;
          primitive["repairs"] = json::array();
          primitive.update(counter(CObject["geometry"][i]));

          RepairReport["Primitives"].push_back(primitive);
        }
      }
    }

    template<typename T>
    vector<json> CityObject::GeometryRepair(T geometry, int idp) {

      vector<json> outputGEOMS;
      json primitive = {
              {"id",                 id + "." + to_string(idp)},
              {"type",               geometry.get_type()},
              {"repaired",           false},
              {"repairs",            json::array()},
              {"ISOerrorsremaining", geometry.get_ISO_remaining()}
      };

      geometry.val3dityReport();
      // make semantics and materials map
      if ((!geometry.isvalid() || !geometry.isUCvalid()) && geometry.isSMT()){
        geometry.startFacesSMT();
      }

      if (STANDARDS["OutputParameters"]["ShowProgress"]) {
        std::cout << "\tCityObject with id " << id << "." << idp << " geometry has the following errors: "
                  << geometry.get_ISO_remaining() << std::endl;
      }

      vector<string> errorTypes = {"RingErrors", "PolyErrors", "ShellErrors", "SolidErrors", "SolidIErrors"};
      map<string, function<json()>> repairFunctions = {
              {"RingErrors",   [&]() { return geometry.RepairRingErrors(); }},
              {"PolyErrors",   [&]() { return geometry.RepairPolyErrors(); }},
              {"ShellErrors",  [&]() { return geometry.RepairShellErrors(); }},
              {"SolidErrors",  [&]() { return geometry.RepairSolidErrors(); }},
              {"SolidIErrors", [&]() { return geometry.RepairSolidIErrors(); }},
      };

      map<string, string> repairtypes = {
              {"RingErrors",   "RingRepairs"},
              {"PolyErrors",   "PolyRepairs"},
              {"ShellErrors",  "ShellRepairs"},
              {"SolidErrors",  "SolidRepairs"},
              {"SolidIErrors", "SolidIRepairs"},
      };

      int counterCat = 1;
      string lastRepairCat;

      // REPAIR LOOP
      for (int totalCounter = 1; (!geometry.isvalid() || !geometry.isUCvalid()) &&
                                 counterCat < STANDARDS["RepairDepths"]["MaxRepairDepth"] &&
                                 totalCounter < STANDARDS["RepairDepths"]["TotalRepairDepth"];
           ++totalCounter) {


        // GEOMETRIC REPAIRS
        for (auto &errorType: errorTypes) {
          if (!geometry.RepairsNeeded[errorType].empty()) {
            // report and repair
            json repairs = {
                    {"round",          totalCounter},
                    {"kind_of_repair", repairtypes[errorType]},
                    {"repairs_done",   repairFunctions[errorType]()}
            };
            primitive["repairs"].push_back(repairs);

            // for repair depth
            if (lastRepairCat == errorType) { ++counterCat; }
            else {
              counterCat = 1;
              lastRepairCat = errorType;
            }

            break;
          }
        }

        vector<json> features;

        // IF GEOMETRIC REPAIR SPLIT FEATURE
        if (geometry.getSplitfeatures(features)) {
          int count = 0;
          for (auto &tu3d: features) {
            count++;
            vector<json> rgeom;
            if (tu3d["features"][0]["geometry"]["type"] == "MultiSurface") {
              AUTOr3pair::MultiSurface geom(id + "_" + to_string(count++), tu3d, geometry.getFacesSMT());
              rgeom = GeometryRepair(geom, idp);
              outputGEOMS.insert(outputGEOMS.end(), rgeom.begin(), rgeom.end());
            } else if (tu3d["features"][0]["geometry"]["type"] == "CompositeSurface") {
              AUTOr3pair::CompositeSurface geom(id + "_" + to_string(count++), tu3d, geometry.getFacesSMT());
              rgeom = GeometryRepair(geom, idp);
              outputGEOMS.insert(outputGEOMS.end(), rgeom.begin(), rgeom.end());
            } else if (tu3d["features"][0]["geometry"]["type"] == "Solid") {
              AUTOr3pair::Solid geom(id + "_" + to_string(count++), tu3d, geometry.getFacesSMT());
              rgeom = GeometryRepair(geom, idp);
              outputGEOMS.insert(outputGEOMS.end(), rgeom.begin(), rgeom.end());
            } else if (tu3d["features"][0]["geometry"]["type"] == "MultiSolid") {
              AUTOr3pair::MultiSolid geom(id + "_" + to_string(count++), tu3d, geometry.getFacesSMT());
              rgeom = GeometryRepair(geom, idp);
              outputGEOMS.insert(outputGEOMS.end(), rgeom.begin(), rgeom.end());
            } else if (tu3d["features"][0]["geometry"]["type"] == "CompositeSolid") {
              AUTOr3pair::CompositeSolid geom(id + "_" + to_string(count++), tu3d, geometry.getFacesSMT());
              rgeom = GeometryRepair(geom, idp);
              outputGEOMS.insert(outputGEOMS.end(), rgeom.begin(), rgeom.end());
            }
          }
        }

        // IF GEOMETRIC REPAIR CHANGED TYPE
        json tu3d;
        if (geometry.typechange(tu3d)) {
          vector<json> rgeom;
          if (tu3d["features"][0]["geometry"]["type"] == "MultiSurface") {
            AUTOr3pair::MultiSurface geom(id, tu3d, geometry.getFacesSMT());
            rgeom = GeometryRepair(geom, idp);
            outputGEOMS.insert(outputGEOMS.end(), rgeom.begin(), rgeom.end());
          } else if (tu3d["features"][0]["geometry"]["type"] == "CompositeSurface") {
            AUTOr3pair::CompositeSurface geom(id, tu3d, geometry.getFacesSMT());
            rgeom = GeometryRepair(geom, idp);
            outputGEOMS.insert(outputGEOMS.end(), rgeom.begin(), rgeom.end());
          } else if (tu3d["features"][0]["geometry"]["type"] == "Solid") {
            AUTOr3pair::Solid geom(id, tu3d, geometry.getFacesSMT());
            rgeom = GeometryRepair(geom, idp);
            outputGEOMS.insert(outputGEOMS.end(), rgeom.begin(), rgeom.end());
          } else if (tu3d["features"][0]["geometry"]["type"] == "MultiSolid") {
            AUTOr3pair::MultiSolid geom(id, tu3d, geometry.getFacesSMT());
            rgeom = GeometryRepair(geom, idp);
            outputGEOMS.insert(outputGEOMS.end(), rgeom.begin(), rgeom.end());
          } else if (tu3d["features"][0]["geometry"]["type"] == "CompositeSolid") {
            AUTOr3pair::CompositeSolid geom(id, tu3d, geometry.getFacesSMT());
            rgeom = GeometryRepair(geom, idp);
            outputGEOMS.insert(outputGEOMS.end(), rgeom.begin(), rgeom.end());
          }
          return outputGEOMS; //other type so stop this repairloop
        }

        if (geometry.isempty()){
          break;
        }


        geometry.val3dityReport();

        if (counterCat == STANDARDS["RepairDepths"]["MaxRepairDepth"] || totalCounter == STANDARDS["RepairDepths"]["TotalRepairDepth"]){
          if (!geometry.isvalid()){
            // report and repair
            std::cerr << "\t\tGeometry needs a global repair" << endl;
            json repairs = {
                    {"round",          totalCounter},
                    {"kind_of_repair", "GlobalRepair"},
                    {"repairs_done",   geometry.RepairGlobal()}
            };
            primitive["repairs"].push_back(repairs);

          }
        }

        // USE CASE REPAIRS (only if valid, but inside loop so if invalid result geometric repair)
        // first CFD than check valid again
        if (geometry.isvalid() &&
            ((STANDARDS["UseCaseRepair"]["Triangulation"] && !geometry.istriangulated() )||
             STANDARDS["UseCaseRepair"]["Simplification"] ||
             STANDARDS["UseCaseRepair"]["RemeshSlivers"])) {


          // report and repair
          json repairs = {
                  {"round",          totalCounter},
                  {"kind_of_repair", "UseCaseRepair"},
                  {"repairs_done",   geometry.RepairMesh()}
          };
          primitive["repairs"].push_back(repairs);

          // Recheck validity
          geometry.val3dityReport();
        }



        // second semantics doesnt change boundaries so keeps being valid
        if (geometry.isvalid() && FILETYPE == "json"){
          // write back Semantics and materials
          if (geometry.isSMT()){
            geometry.writeFacesSMT();
          }


          if (STANDARDS["UseCaseRepair"]["SemanticsAdd"] || STANDARDS["UseCaseRepair"]["SemanticsValidate"]) {
            // report and repair
            json repairs = {
                    {"round",          totalCounter},
                    {"kind_of_repair", "UseCaseRepair"},
                    {"repairs_done",   geometry.RepairSemantics()}
            };
            primitive["repairs"].push_back(repairs);
          }
        }
      }



      json newGeom = geometry.getCityjsonObject();
      if (!primitive["repairs"].empty()) {
        primitive["repaired"] = true;
        if (newGeom.contains("texture")){
          newGeom.erase("texture");
        }
      }
      primitive.update(counter(newGeom));
      RepairReport["Primitives"].push_back(primitive);
      outputGEOMS.insert(outputGEOMS.begin(), newGeom);
      return outputGEOMS;
    }


    json CityObject::getCObject() {
      json out;
      out[id] = CObject;
      return out;
    }

    json &CityObject::getRReport() { return RepairReport; }
} // AUTOr3pair
