#  Copyright (c) 2024. made for a proof of concept for theis Automatic repair of 3d citymodels by Lisa Keurentjes
import json
import math
import os

def find_scale_factor(data):
    min_exponent = 0

    for point in data:
        for coord in point:
            if coord != 0:
                if coord.is_integer():
                    exponent = 0
                else:
                    cleanedcoord = round(coord, 8)
                    exponent = len(str(cleanedcoord).split('.')[-1])
                min_exponent = max(min_exponent, exponent)

    scale_factor = 10 ** (min_exponent)
    return scale_factor


def translatefile(filename):
    with open(filename) as f:
        lines = f.read().splitlines()

    # Parse nodes from the .poly file
    num_vertices = int(lines[0].split()[0])
    vertices = [list(map(float, line.split()[1:])) for line in lines[1:num_vertices + 1]]
    scale = find_scale_factor(vertices)
    vertices_as_int = [[int(v * scale) for v in point] for point in vertices]

    cityjson_data = {
        "type": "CityJSON",
        "version": "2.0",
        "CityObjects": {},
        "vertices": vertices_as_int,
        "transform": {
            "scale": [1 / scale, 1 / scale, 1 / scale],
            "translate": [0, 0, 0]
        }
    }

    # Parse segments from the .poly file
    num_segments = int(lines[num_vertices + 1].split()[0])
    current_line = num_vertices + 2  # Start reading segments from this line

    # Loop through segments
    segment = []
    for _ in range(num_segments):
        # Read faces in segment
        num_faces, extra = int(lines[current_line].split()[0]), int(lines[current_line].split()[1])
        current_line += 1

        # Read faces and add to the segment
        face = []
        for _ in range(num_faces):
            face_points = list(map(int, lines[current_line].split()[1:]))
            current_line += 1
            face.append(face_points)
        segment.append(face)
        current_line += extra

    # Create CityObject for each segment
    cityobject_id = f"id_{_+1}"
    cityjson_data["CityObjects"][cityobject_id] = {
        "type": "GenericCityObject",  # Adjust the type based on your data
        "geometry": [
            {
                "type": "CompositeSurface",
                "boundaries": segment,
                "lod": "1"  # Adjust the level of detail as needed
            }
        ],
    }

    return cityjson_data

def write_cityjson_file(cityjson_data, output_file_path):
    with open(output_file_path, 'w') as outfile:
        json.dump(cityjson_data, outfile, indent=2)


### FOR 1 FILE
## input
# filename = "/home/lisa/Documents/GitHub/val3dity/data/test_geometry_generic/208.poly"
# cj = translatefile(filename)

## output
# output_directory = "data/Test_repairs/Val3dity_tests"
# fileOut = filename[filename.rfind("/")+1:-5] + "_MSurf.city.json"
# output_path = os.path.join(output_directory, fileOut)
# print("write: ",output_path)
# write_cityjson_file(cj,output_path)

### FOR ALL FILES IN DIRECTORY
directory = "/home/lisa/Documents/GitHub/val3dity/data/test_geometry_generic"
output_directory = "data/Test_repairs/Val3dity_tests"

file_list = os.listdir(directory)
for file in file_list:
    if ".poly" not in file:
        continue
    #input
    file_in = os.path.join(directory, file)
    cj = translatefile(file_in)

    #output
    fileOut = file_in[file_in.rfind("/") + 1:-5] + "_CSurf.city.json"
    output_path = os.path.join(output_directory, fileOut)
    print("write: ",output_path)
    write_cityjson_file(cj,output_path)




