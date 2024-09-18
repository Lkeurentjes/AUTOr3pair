// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#include <iostream>
#include "tu3djson_convert.h"
#include "Defenitions.h"
#include "DefenitionsCGAL.h"


using json = nlohmann::json;
using namespace std;

namespace tu3djson {
    // ---  CONVERTS A CITYOBJECT INTO A TU3DJSON   ---
    json make_tu3djson_geometry(json Geom) {
        // -- TU3dJSON based on specifications https://github.com/tudelft3d/tu3djson
        json tu3d;
        tu3d["type"] = "tu3djson";
        json feature;
        feature["type"] = "feature";
        feature["geometry"]["type"] = Geom["type"];
        feature["geometry"]["boundaries"] = Geom["boundaries"];
        feature["geometry"]["vertices"] = VERTICES.get_verticeslist();

        tu3d["features"][0] = feature;

        tu3d["properties"]["lod"] = Geom["lod"];

        if (Geom.contains("semantics")){
            tu3d["properties"]["semantics"] = Geom["semantics"];
        }
        if (Geom.contains("material")){
            tu3d["properties"]["material"] = Geom["material"];
        }
        if (Geom.contains("texture")){
            tu3d["properties"]["texture"] = Geom["texture"];
        }
        return tu3d;
    }

    json get_tu3djson_feature(json TU3D, int index) {
        json tu3d;
        tu3d["type"] = "tu3djson";
        json feature;
        feature["type"] = "feature";
        feature["geometry"]["type"] = TU3D["features"][index]["geometry"]["type"];
        feature["geometry"]["boundaries"] = TU3D["features"][index]["geometry"]["boundaries"];
        feature["geometry"]["vertices"] = VERTICES.get_verticeslist();

        tu3d["features"][0] = feature;

        tu3d["properties"]["lod"] = TU3D["properties"]["lod"];

        //TODO fix the semantics
        // if (Geom.contains("semantics")){
        //     tu3d["properties"]["semantics"] = Geom["semantics"];
        // }
        // if (Geom.contains("material")){
        //     tu3d["properties"]["material"] = Geom["material"];
        // }
        // if (Geom.contains("texture")){
        //     tu3d["properties"]["texture"] = Geom["texture"];
        // }
        return tu3d;

    }

    // ---  CONVERTS A TU3DJSON TO A CITYOBJECT      ---
    json make_CityObject_geometry(json TU3D){
        json CO;
        CO["boundaries"] = TU3D["features"][0]["geometry"]["boundaries"];
        CO["lod"] = TU3D["properties"]["lod"];
        CO["type"] = TU3D["features"][0]["geometry"]["type"];

        if (TU3D["properties"].contains("semantics")){
            CO["semantics"] = TU3D["properties"]["semantics"];
        }

        if (TU3D["properties"].contains("material")){
            CO["material"] = TU3D["properties"]["material"];
        }

        if (TU3D["properties"].contains("texture")){
            CO["texture"] = TU3D["properties"]["texture"];
        }

        return CO;

    }

}