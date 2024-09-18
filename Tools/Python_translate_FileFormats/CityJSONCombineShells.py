#  Copyright (c) 2024. made for a proof of concept for theis Automatic repair of 3d citymodels by Lisa Keurentjes
import json
import os


def make_multi(base, out):
    base["CityObjects"]['id_1']["geometry"][0]["boundaries"] = [base["CityObjects"]['id_1']["geometry"][0]["boundaries"],
                                                           base["CityObjects"]['id_1']["geometry"][0]["boundaries"]]
    base["CityObjects"]['id_1']["geometry"][0]["type"] = "MultiSolid"
    geom = base["CityObjects"]['id_1']["geometry"][0]
    for key in ['semantics', 'material', 'texture']:
        if key in geom:
            if key in ['material', 'texture']:
                for item in geom[key].values():
                    item["values"] = [item["values"] * 2]
            else:
                geom[key]["values"] = [geom[key]["values"] * 2]

    outMsolid = out[:-15] + "MSolid.city.json"
    with open(outMsolid, "w") as out:
        json.dump(base, out)


def Combine_file(filename, out, addbasecube, addinner):
    print("\n", filename, out, addbasecube, addinner)
    with open(filename) as f:
        data = json.load(f)

    if addbasecube:
        print("\tAdd to base cube")
        for CO in data["CityObjects"].values():
            for geom in CO['geometry']:

                for shell in range(len(geom["boundaries"])):
                    for face in range(len(geom["boundaries"][shell])):
                        for ring in range(len(geom["boundaries"][shell][face])):
                            for v in range(len(geom["boundaries"][shell][face][ring])):
                                geom["boundaries"][shell][face][ring][v] += 8

                SM = any(key in geom for key in ['semantics', 'material', 'texture'])
                if SM:
                    with open("data/Test_repairs/Solid_Level/with/basecube_SM_Solid.city.json") as f:
                        base = json.load(f)
                else:
                    with open("data/Test_repairs/Solid_Level/without/basecube_Solid.city.json") as f:
                        base = json.load(f)

                for cityobject in base["CityObjects"].values():
                    for start in cityobject['geometry']:
                        start["boundaries"] += geom["boundaries"]
                base["vertices"] += data['vertices']
                print(base)

                if addinner:
                    print("\t\tAdd inner")
                    if SM:
                        with open("data/Test_repairs/Solid_Level/with/inner_shell_SM_Solid.city.json") as f:
                            inner = json.load(f)
                    else:
                        with open("data/Test_repairs/Solid_Level/without/inner_shell_Solid.city.json") as f:
                            inner = json.load(f)

                    add = len(base["vertices"])
                    for CO in inner["CityObjects"].values():
                        for geom in CO['geometry']:
                            for shell in range(len(geom["boundaries"])):
                                for face in range(len(geom["boundaries"][shell])):
                                    for ring in range(len(geom["boundaries"][shell][face])):
                                        for v in range(len(geom["boundaries"][shell][face][ring])):
                                            geom["boundaries"][shell][face][ring][v] += add

                    for cityobject in base["CityObjects"].values():
                        for start in cityobject['geometry']:
                            start["boundaries"] += geom["boundaries"]
                    base["vertices"] += inner['vertices']
                    print(base)

        with open(out, "w") as w:
            json.dump(base, w)
            make_multi(base, out)


bascube = {
    '401_1_Solid.city.json': True,
    '401_2_Solid.city.json': True,
    '401_3_Solid.city.json': True,
    '401_4_Solid.city.json': True,
    '401_5_Solid.city.json': True,
    '401_6_Solid.city.json': True,
    '401_Solid.city.json': True,
    '402_in_1_Solid.city.json': True,
    '403_Solid.city.json': True,
    '404_Solid.city.json': True,
    'basecube_Solid.city.json': False,
    'inner_shell_Solid.city.json': False
}

inner = {
    '401_1_Solid.city.json': False,
    '401_2_Solid.city.json': True,
    '401_3_Solid.city.json': True,
    '401_4_Solid.city.json': False,
    '401_5_Solid.city.json': False,
    '401_6_Solid.city.json': True,
    '401_Solid.city.json': False,
    '402_in_1_Solid.city.json': True,
    '403_Solid.city.json': False,
    '404_Solid.city.json': False,
    'basecube_Solid.city.json': False,
    'inner_shell_Solid.city.json': False
}
# 1 Directory
directory = "data/Test_repairs/Solid_Level/without"
outdirectory = "data/Test_repairs/Solid_Level/Without_SemanticsMaterials"
# directory = "data/Test_repairs/Poly_Level/With_SemanticsMaterials"
file_list = os.listdir(directory)
for file in file_list:
    # Choose input file
    filename = os.path.join(directory, file)
    filenout = os.path.join(outdirectory, file)
    Combine_file(filename, filenout, bascube[file], inner[file])

# # 1 FILE
# filename = "data/Test_repairs/Poly_Level/With_SemanticsMaterials/201_1_SM_MSurf.city.json"
# translate_file(filename, True, True, True)
