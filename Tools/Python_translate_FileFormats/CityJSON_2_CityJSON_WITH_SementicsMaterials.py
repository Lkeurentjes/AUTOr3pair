#  Copyright (c) 2024. made for a proof of concept for theis Automatic repair of 3d citymodels by Lisa Keurentjes
import json
import math
import os
import random


def addSemanticsMaterialsToMultiSurface(geom):
    geom["semantics"]["values"] = [random.choice([None, 0, 1, 2, 3, 0, 1, 2, 3]) for _ in geom["boundaries"]]
    geom["material"]["irradiation"]["values"] = [random.choice([None, 0, 1, 2, 0, 1, 2]) for _ in geom["boundaries"]]
    geom["material"]["irradiation-2"]["values"] = [random.choice([None, 0, 1, 2, 0, 1, 2]) for _ in geom["boundaries"]]


def addSemanticsMaterialsToSolid(geom):
    geom["semantics"]["values"] = [
        [random.choice([None, 0, 1, 2, 3, 0, 1, 2, 3]) for _ in solid] for solid in geom["boundaries"]
    ]
    geom["material"]["irradiation"]["values"] = [
        [random.choice([None, 0, 1, 2, 0, 1, 2]) for _ in solid] for solid in geom["boundaries"]
    ]
    geom["material"]["irradiation-2"]["values"] = [
        [random.choice([None, 0, 1, 2, 0, 1, 2]) for _ in solid] for solid in geom["boundaries"]
    ]


def addSemanticsMaterialsToMultiSolid(geom):
    geom["semantics"]["values"] = [
        [
            [random.choice([None, 0, 1, 2, 3, 0, 1, 2, 3]) for _ in solid] for solid in multi_solid
        ]
        for multi_solid in geom["boundaries"]
    ]
    geom["material"]["irradiation"]["values"] = [
        [
            [random.choice([None, 0, 1, 2, 0, 1, 2]) for _ in solid] for solid in multi_solid
        ]
        for multi_solid in geom["boundaries"]
    ]
    geom["material"]["irradiation-2"]["values"] = [
        [
            [random.choice([None, 0, 1, 2, 0, 1, 2]) for _ in solid] for solid in multi_solid
        ]
        for multi_solid in geom["boundaries"]
    ]


def addSementics_Materials(data):
    with open(data) as f:
        j = json.load(f)
        for co in j["CityObjects"].values():
            for geom in co["geometry"]:
                geom["semantics"] = {
                    "surfaces": [
                        {
                            "type": "GroundSurface"
                        },
                        {
                            "type": "RoofSurface"
                        },
                        {
                            "on_footprint_edge": True,
                            "type": "WallSurface"
                        },
                        {
                            "on_footprint_edge": False,
                            "type": "WallSurface"
                        }
                    ],
                    "values": []
                }
                geom["material"] = {
                    "irradiation": {
                        "values": []
                    },
                    "irradiation-2": {
                        "values": []
                    }
                }
                print("\t",geom["type"])

                if geom["type"] == "Solid":
                    addSemanticsMaterialsToSolid(geom)
                elif geom["type"] == "MultiSolid" or geom["type"] == "CompositeSolid":
                    addSemanticsMaterialsToMultiSolid(geom)
                elif geom["type"] == "MultiSurface" or geom["type"] == "CompositeSurface":
                    addSemanticsMaterialsToMultiSurface(geom)

    j["appearance"] = {"materials": [
        {
            "name": "roofandground",
            "ambientIntensity": 0.2000,
            "diffuseColor": [
                0.9000,
                0.1000,
                0.7500
            ],
            "emissiveColor": [
                0.9000,
                0.1000,
                0.7500
            ],
            "specularColor": [
                0.9000,
                0.1000,
                0.7500
            ],
            "shininess": 0.2,
            "transparency": 0.5,
            "isSmooth": False
        },
        {
            "name": "wall",
            "ambientIntensity": 0.4000,
            "diffuseColor": [
                0.1000,
                0.1000,
                0.9000
            ],
            "emissiveColor": [
                0.1000,
                0.1000,
                0.9000
            ],
            "specularColor": [
                0.9000,
                0.1000,
                0.7500
            ],
            "shininess": 0.0,
            "transparency": 0.5,
            "isSmooth": True
        },
        {
            "name": "floor",
            "ambientIntensity": 0.4000,
            "diffuseColor": [
                0.5000,
                0.1000,
                0.9000
            ],
            "emissiveColor": [
                0.1000,
                0.5000,
                0.9000
            ],
            "specularColor": [
                0.9000,
                0.1000,
                0.7500
            ],
            "shininess": 0.0,
            "transparency": 0.5,
            "isSmooth": True
        }
    ]
    }
    # print(j)
    return j


def write_cityjson_file(cityjson_data, output_file_path):
    with open(output_file_path, 'w') as outfile:
        json.dump(cityjson_data, outfile, indent=2)


# ## FOR 1 FILE
# # input
# filename = "/home/lisa/Documents/GitHub/AUTOr3pair_dev/data/Test_repairs/Poly_Level/Without_SemanticsMaterials/205_MSurf.city.json"
# cj = addSementics_Materials(filename)
#
# # output
# output_directory = "data/Test_repairs/Poly_Level/With_SemanticsMaterials"
# fileOut = filename[filename.rfind("/")+1:-16] + "SM_MSurf.city.json"
# output_path = os.path.join(output_directory, fileOut)
# print("write: ",output_path)
# write_cityjson_file(cj,output_path)

### FOR ALL FILES IN DIRECTORY
directory = "data/Test_geometry_repairs/SubPart_Level/Without_SemanticsMaterials"
output_directory = "data/Test_geometry_repairs/SubPart_Level/With_SemanticsMaterials"

file_list = os.listdir(directory)
for file in file_list:
    print(file)
    if file.endswith(".obj"):
        continue
    # input
    file_in = os.path.join(directory, file)
    cj = addSementics_Materials(file_in)

    # output
    fileOut = file_in[file_in.rfind("/") + 1:file_in.rfind("_") + 1] + "SM_" + file_in[file_in.rfind("_") + 1:]
    output_path = os.path.join(output_directory, fileOut)
    print("\t write: ",output_path)
    write_cityjson_file(cj,output_path)
