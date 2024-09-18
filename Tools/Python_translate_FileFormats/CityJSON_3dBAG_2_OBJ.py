import json
# choose input file and LOF you want to make OBJ of
filename = "/data/Test_input_file_formats/CITYJSON_3dbag.json"
lod = "2.2"
fileOut = filename[:-4] + "obj"

with open(filename) as f:
    data = json.load(f)

vertices = data["vertices"]
s = data["transform"]["scale"]
t = data["transform"]["translate"]
b = []

for CO in data["CityObjects"].values():
    for GEOM in CO['geometry']:
        if GEOM["lod"] == lod:
            type = GEOM["type"]
            Bound = GEOM["boundaries"]
            if type == "MultiSurface" or type == "CompositeSurface":
                for bo in Bound:
                    b.append(bo[0])
            if type == "Solid":
                for shell in Bound:
                    for bo in shell:
                        b.append(bo[0])
            if type == "MultiSolid" or type == "CompositeSolid":
                for solid in Bound:
                    for shell in solid:
                        for bo in shell:
                            b.append(bo[0])

with (open(fileOut, "w") as out):
    for v in vertices:
        line = "v"
        for i in range(3):
            line += " " + str((v[i] + t[i])*s[i])
        line += "\n"
        out.write(line)
    for face in b:
        line = "f"
        for f in face:
            line += " " + str(f+1)
        line += "\n"
        out.write(line)

