import json
import os


def update_geometry(geom, new_type):
    geom["type"] = new_type
    if new_type == "Solid":
        geom["boundaries"] = [geom["boundaries"]]
    else:
        geom["boundaries"] = [[geom["boundaries"]] * 2]
    for key in ['semantics', 'material', 'texture']:
        if key in geom:
            if key in ['material', 'texture']:
                for item in geom[key].values():
                    if new_type == "Solid":
                        item["values"] = [item["values"]]
                    else:
                        item["values"] = [[item["values"]] * 2]
            else:
                if new_type == "Solid":
                    geom[key]["values"] = [geom[key]["values"]]
                else:
                    geom[key]["values"] = [[geom[key]["values"]] * 2]


def update_file_format(input_filename, output_filename, new_type, repetition=1):
    with open(input_filename) as f:
        data = json.load(f)

    for CO in data["CityObjects"].values():
        for GEOM in CO['geometry']:
            if GEOM["type"] in ["MultiSurface", "CompositeSurface"]:
                update_geometry(GEOM, new_type)

    with open(output_filename, "w") as out:
        json.dump(data, out)


def convert_to_obj(input_filename, output_filename):
    with open(input_filename) as f:
        data = json.load(f)

    vertices = data["vertices"]
    scale = data["transform"]["scale"]
    translate = data["transform"]["translate"]
    boundaries = []

    for CO in data["CityObjects"].values():
        for GEOM in CO['geometry']:
            type = GEOM["type"]
            Bound = GEOM["boundaries"]
            if type in ["MultiSurface", "CompositeSurface"]:
                for bo in Bound:
                    boundaries.append(bo[0])
            elif type in ["Solid"]:
                for shell in Bound:
                    for bo in shell:
                        boundaries.append(bo[0])
            elif type in ["MultiSolid", "CompositeSolid"]:
                for solid in Bound:
                    for shell in solid:
                        for bo in shell:
                            boundaries.append(bo[0])

    with open(output_filename, "w") as out:
        for v in vertices:
            line = "v " + " ".join(str((v[i] * scale[i]) + translate[i]) for i in range(3)) + "\n"
            out.write(line)

        for face in boundaries:
            line = "f " + " ".join(str(f + 1) for f in face) + "\n"
            out.write(line)


def translate_file(filename, solid, msolid, obj):
    # TO SOLID
    if solid:
        update_file_format(filename, filename[:-15] + "Solid.city.json", "Solid")

    # TO MSOLID
    if msolid:
        update_file_format(filename, filename[:-15] + "MSolid.city.json", "MultiSolid")

    # TO OBJ
    if obj:
        convert_to_obj(filename, filename[:-16] + "_obj.obj")


# 1 Directory
directory = "data/Test_geometry_repairs/Ring_Level/Without_SemanticsMaterials"
# directory = "data/Test_repairs/Poly_Level/With_SemanticsMaterials"
file_list = os.listdir(directory)
for file in file_list:
    # Choose input file
    if file.endswith("MSurf.city.json"):
        filename = os.path.join(directory, file)
        translate_file(filename, True, True, True)

# # 1 FILE
# filename = "data/Test_repairs/Poly_Level/With_SemanticsMaterials/201_1_SM_MSurf.city.json"
# translate_file(filename, True, True, True)

