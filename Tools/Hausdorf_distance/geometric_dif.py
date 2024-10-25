import json
import numpy as np
from scipy.spatial.distance import directed_hausdorff, pdist
import os
import sys


def extract_object_vertices_by_lod(cityjson_file):
    """
    Extract vertices for each object by LOD in the CityJSON file.
    :param cityjson_file: Path to the CityJSON file.
    :return: A dictionary where keys are LODs and values are dictionaries of object IDs with their vertices.
    """
    with open(cityjson_file, 'r') as f:
        data = json.load(f)

    vertices = np.array(data['vertices'])
    scale = data.get('transform', {}).get('scale', [1.0, 1.0, 1.0])
    scaled_vertices = vertices * scale

    lod_objects = {}

    for object_id, city_object in data['CityObjects'].items():
        if 'geometry' in city_object and city_object['geometry']:
            for geom in city_object['geometry']:
                lod = geom.get('lod')
                if lod is not None:
                    object_vertices = []
                    if 'boundaries' in geom:
                        for boundary in geom['boundaries']:
                            object_vertices.extend([scaled_vertices[idx] for idx in flatten_boundary(boundary)])
                    if object_vertices:
                        if lod not in lod_objects:
                            lod_objects[lod] = {}
                        lod_objects[lod][object_id] = np.array(object_vertices)
        else:
            print(f"Skipping object: {object_id}, no geometry found.")

    return lod_objects


def flatten_boundary(boundary):
    """
    Flatten a nested list of boundary indices.
    :param boundary: A nested list of boundary indices.
    :return: A flat list of indices.
    """
    if isinstance(boundary, int):
        return [boundary]
    else:
        flat_list = []
        for item in boundary:
            flat_list.extend(flatten_boundary(item))
        return flat_list


def hausdorff_distance_per_lod(cityjson_file1, cityjson_file2):
    """
    Compute the Hausdorff distance per LOD between two CityJSON files.
    :param cityjson_file1: Path to the first CityJSON file.
    :param cityjson_file2: Path to the second CityJSON file.
    :return: A dictionary where keys are LODs and values are Hausdorff distances per object, max difference, and missing count.
    """
    # Extract object vertices by LOD from both files
    lod_objects1 = extract_object_vertices_by_lod(cityjson_file1)
    lod_objects2 = extract_object_vertices_by_lod(cityjson_file2)

    lod_distances = {}
    max_difference_whole_file = 0.0
    highest_percentage_whole_file = 0.0  # Track the highest percentage across all LODs

    # Compare objects within the same LOD
    for lod in lod_objects1:
        if lod in lod_objects2:
            distances = {}
            max_difference_lod = 0.0
            missing_count = 0
            highest_percentage_lod = 0.0  # Initialize highest percentage for the current LOD

            # Compare objects within the same LOD
            for object_id in lod_objects1[lod]:
                if object_id in lod_objects2[lod]:
                    vertices1 = lod_objects1[lod][object_id]
                    vertices2 = lod_objects2[lod][object_id]

                    # Calculate Hausdorff distance
                    d_AB = directed_hausdorff(vertices1, vertices2)[0]
                    d_BA = directed_hausdorff(vertices2, vertices1)[0]
                    hausdorff_dist = round(max(d_AB, d_BA), 4)  # Rounded to 4 decimal places

                    # Calculate size (maximum distance between any two vertices in the first object)
                    size = round(np.max(pdist(vertices1)), 4)  # Rounded to 4 decimal places

                    # Calculate percentage
                    percentage = round((hausdorff_dist / size) if size > 0 else 0, 4)  # Rounded to 4 decimal places

                    # Update the highest percentage for this LOD and whole file
                    if max_difference_lod < hausdorff_dist:
                        highest_percentage_lod = percentage
                        max_difference_lod = hausdorff_dist
                        if max_difference_whole_file < hausdorff_dist:
                            highest_percentage_whole_file = percentage
                            max_difference_whole_file = hausdorff_dist


                    # Store the result with additional details
                    distances[object_id] = {
                        "distance": hausdorff_dist,
                        "size": size,
                        "percentage": percentage
                    }


                else:
                    distances[object_id] = None  # Object not found in the second file
                    missing_count += 1

            # Store the results for this LOD, including the highest percentage
            lod_distances[lod] = {
                "per object": distances,
                "maxDifference": max_difference_lod,
                "missingCount": missing_count,
                "highestPercentage": round(highest_percentage_lod, 4)  # Rounded to 4 decimal places
            }



    return lod_distances, max_difference_whole_file, highest_percentage_whole_file


def write_report(lod_distances, max_difference_whole_file, highest_percentage_whole_file, output_file):
    """
    Write a JSON report with Hausdorff distances per LOD, max differences, highest percentages, and missing counts.
    :param lod_distances: A dictionary of distances and max differences per LOD.
    :param max_difference_whole_file: The maximum Hausdorff distance across all LODs.
    :param highest_percentage_whole_file: The highest percentage across all LODs and objects.
    :param output_file: Path to the output JSON report.
    """
    report = {
        "per LOD": lod_distances,
        "Maxdifferencewholefile": round(max_difference_whole_file, 4),  # Rounded to 4 decimal places
        "HighestPercentageWholeFile": round(highest_percentage_whole_file, 4)  # Rounded to 4 decimal places
    }

    with open(output_file, 'w') as f:
        json.dump(report, f, indent=4)
    print(f"Report written to {output_file}")


if __name__ == '__main__':
    # if len(sys.argv) < 3:
    #     print("Usage: python script_name.py <cityjson_file1> <cityjson_file2>")
    #     sys.exit(1)
    #
    # cityjson_file1 = sys.argv[1]
    # cityjson_file2 = sys.argv[2]
    #
    # output_file = cityjson_file2.replace('__REPAIRED.json', '_hausdorff_distance_report.json')
    # if output_file == cityjson_file2:
    #     output_file = cityjson_file2.replace('.json', '_hausdorff_distance_report.json')
    #
    # lod_distances, max_difference_whole_file, highest_percentage_whole_file = hausdorff_distance_per_lod(cityjson_file1,
    #                                                                                                      cityjson_file2)
    # write_report(lod_distances, max_difference_whole_file, highest_percentage_whole_file, output_file)

    ###### BAG
    # filestocheck =[
    #     "9-324-628.city.json",
    #     "10-316-624.city.json",
    #     "10-316-626.city.json",
    #     "10-316-628.city.json",
    #     "10-316-630.city.json",
    #     "10-318-624.city.json",
    #     "10-318-626.city.json",
    #     "10-318-628.city.json",
    #     "10-318-630.city.json",
    #     "10-320-624.city.json",
    #     "10-320-626.city.json",
    #     "10-320-628.city.json",
    #     "10-320-630.city.json",
    #     "10-322-624.city.json",
    #     "10-322-626.city.json",
    #     "10-322-628.city.json",
    #     "10-322-630.city.json",
    #     "10-324-624.city.json",
    #     "10-324-626.city.json",
    #     "10-326-624.city.json",
    #     "10-326-626.city.json"]
    #
    # for file in filestocheck:
    #     cityjson_file1 = "../../data/Test_data_thesis/BAG_tiles/" + file
    #     outfile = file.replace(".city.json", "__USERINPUT_BAG__REPAIRED.json")
    #     cityjson_file2 = "../../data/Test_data_thesis/BAG_tiles/" + outfile
    #
    #     output_file = cityjson_file2.replace('__REPAIRED.json', '_hausdorff_distance_report.json')
    #     output_file = output_file.replace('BAG_tiles/', 'BAG_tiles/Hausdorf_Distance_Report/')
    #
    #     lod_distances, max_difference_whole_file, highest_percentage_whole_file = hausdorff_distance_per_lod(
    #         cityjson_file1,
    #         cityjson_file2)
    #     write_report(lod_distances, max_difference_whole_file, highest_percentage_whole_file, output_file)


    # # Brussels
    # cityjson_file1 = '../../data/Test_data_thesis/Other/01_Brussels.json'
    # cityjson_file2 = '../../data/Test_data_thesis/Other/01_Brussels__REPAIRED.json'
    # cityjson_file2 = '../../data/Test_data_thesis/Other/01_Brussels__USERINPUT_Brussel_Orientation__REPAIRED.json'
    # cityjson_file2 = '../../data/Test_data_thesis/Other/01_Brussels__USERINPUT_Brussel_Watertight__REPAIRED.json'
    #
    # output_file = cityjson_file2.replace('__REPAIRED.json', '_hausdorff_distance_report.json')
    # output_file = output_file.replace('Other/', 'Other/Hausdorf_Distance_Report/')
    #
    # lod_distances, max_difference_whole_file, highest_percentage_whole_file = hausdorff_distance_per_lod(cityjson_file1,
    #                                                                                                      cityjson_file2)
    # write_report(lod_distances, max_difference_whole_file, highest_percentage_whole_file, output_file)

    #random thesis data
    cityjson_file1 = '../../data/Test_data_thesis/CityJSON_website/01_BAG.city.json'
    cityjson_file1 = '../../data/Test_data_thesis/CityJSON_website/02_DenHaag.city.json'
    cityjson_file1 = '../../data/Test_data_thesis/CityJSON_website/03_Ingolstadt.city.json'
    # cityjson_file1 = '../../data/Test_data_thesis/CityJSON_website/04_Montreal.city.json'
    # cityjson_file1 = '../../data/Test_data_thesis/CityJSON_website/06_Railway.city.json'
    # cityjson_file1 = '../../data/Test_data_thesis/CityJSON_website/07_Rotterdam.city.json'
    # cityjson_file1 = '../../data/Test_data_thesis/CityJSON_website/08_Vienna.city.json'
    cityjson_file2 = cityjson_file1.replace(".city.json", "__REPAIRED.json")

    output_file = cityjson_file2.replace('__REPAIRED.json', '_hausdorff_distance_report.json')
    output_file = output_file.replace('CityJSON_website/', 'CityJSON_website/Hausdorf_Distance_Report/')


    lod_distances, max_difference_whole_file, highest_percentage_whole_file = hausdorff_distance_per_lod(cityjson_file1,
                                                                                                         cityjson_file2)
    write_report(lod_distances, max_difference_whole_file, highest_percentage_whole_file, output_file)


    cityjson_file2 = cityjson_file1.replace(".city.json", "__CFD__REPAIRED.json")

    output_file = cityjson_file2.replace('__REPAIRED.json', '_hausdorff_distance_report.json')
    output_file = output_file.replace('CityJSON_website/', 'CityJSON_website/Hausdorf_Distance_Report/')

    lod_distances, max_difference_whole_file, highest_percentage_whole_file = hausdorff_distance_per_lod(cityjson_file1,
                                                                                                         cityjson_file2)
    write_report(lod_distances, max_difference_whole_file, highest_percentage_whole_file, output_file)

    cityjson_file2 = cityjson_file1.replace(".city.json", "__ENERGYDEMAND__REPAIRED.json")

    output_file = cityjson_file2.replace('__REPAIRED.json', '_hausdorff_distance_report.json')
    output_file = output_file.replace('CityJSON_website/', 'CityJSON_website/Hausdorf_Distance_Report/')

    lod_distances, max_difference_whole_file, highest_percentage_whole_file = hausdorff_distance_per_lod(cityjson_file1,
                                                                                                         cityjson_file2)
    write_report(lod_distances, max_difference_whole_file, highest_percentage_whole_file, output_file)

    cityjson_file2 = cityjson_file1.replace(".city.json", "__SOLARPOWER__REPAIRED.json")

    output_file = cityjson_file2.replace('__REPAIRED.json', '_hausdorff_distance_report.json')
    output_file = output_file.replace('CityJSON_website/', 'CityJSON_website/Hausdorf_Distance_Report/')

    lod_distances, max_difference_whole_file, highest_percentage_whole_file = hausdorff_distance_per_lod(cityjson_file1,
                                                                                                         cityjson_file2)
    write_report(lod_distances, max_difference_whole_file, highest_percentage_whole_file, output_file)

    cityjson_file2 = cityjson_file1.replace(".city.json", "__VISUALIZATION__REPAIRED.json")

    output_file = cityjson_file2.replace('__REPAIRED.json', '_hausdorff_distance_report.json')
    output_file = output_file.replace('CityJSON_website/', 'CityJSON_website/Hausdorf_Distance_Report/')

    lod_distances, max_difference_whole_file, highest_percentage_whole_file = hausdorff_distance_per_lod(cityjson_file1,
                                                                                                         cityjson_file2)
    write_report(lod_distances, max_difference_whole_file, highest_percentage_whole_file, output_file)