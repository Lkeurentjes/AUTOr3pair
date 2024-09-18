import polyscope as ps
import geom_help
import json
import sys
import numpy as np
import math
from tqdm import tqdm
import click


@click.command()
@click.argument('file')
@click.argument('report')

def main(file, report):
    """vizR3pair: a simple visualiser for AUTOr3pair reports."""
    ps.init()
    j, vs, fs, lsurfs = load_cityjson(file)
    r3pairReport = json.loads(open(report).read())
    visualise(vs, fs, lsurfs, r3pairReport)


def get_bbox(vs):
    return [ np.min(vs[:,0]), np.min(vs[:,1]), np.max(vs[:,0]), np.max(vs[:,1]) ]


def load_cityjson(path):
    def recusionvisit(a, fs):
        if isinstance(a[0], int):
            print(len(a))
        else:
            for each in a:
                recusionvisit(each, fs)
    lsurfs = {}            
    j = json.loads(open(path).read())

    # get vertices
    v = []
    for each in j["vertices"]:
        x = (each[0] * j["transform"]["scale"][0]) + j["transform"]["translate"][0]
        y = (each[1] * j["transform"]["scale"][1]) + j["transform"]["translate"][1]
        z = (each[2] * j["transform"]["scale"][2]) + j["transform"]["translate"][2]
        v.append([x, y, z])
    vs = np.asarray(v)

    ts = []
    for co in tqdm(j["CityObjects"]):
        if "parents" not in j['CityObjects'][co]:
            # print("CO:", co)
            lsurfs[co] = [len(ts)]
            extract_surfaces(co, j, vs, ts)
            if 'children' in j['CityObjects'][co]:
                for child in j['CityObjects'][co]['children']:
                    extract_surfaces(child, j, vs, ts)
            lsurfs[co].append(len(ts) - 1)
    ts = np.array(ts, dtype=np.uint32).reshape((-1, 3))

    return j, vs, ts, lsurfs


def extract_surfaces(co, j, vs, ts):
    if 'geometry' in j['CityObjects'][co]:
        if j['CityObjects'][co]['geometry'] == None:
            # TODO fix bug in c++ and then delete this
            return None;
        for geom in j['CityObjects'][co]['geometry']:
            if (geom['type'] == 'MultiSurface') or (geom['type'] == 'CompositeSurface'):
                for i, face in enumerate(geom['boundaries']):
                    if ((len(face) == 1) and (len(face[0]) == 3)):
                        re = np.array(face)
                        ts.append(re[0])
                    else:
                        re, b = geom_help.triangulate_face_mapbox_earcut(face, vs)
                        if b == True:
                            for each in re:
                                ts.append(each)
            elif (geom['type'] == 'Solid'):
                for sidx, shell in enumerate(geom['boundaries']):
                    for i, face in enumerate(shell):
                        if ((len(face) == 1) and (len(face[0]) == 3)):
                            re = np.array(face)
                            ts.append(re[0])
                        else:
                            re, b = geom_help.triangulate_face_mapbox_earcut(face, vs)
                            if b == True:
                                for each in re:
                                    ts.append(each)
            elif ((geom['type'] == 'MultiSolid') or (geom['type'] == 'CompositeSolid')):
                for solididx, solid in enumerate(geom['boundaries']):
                    for sidx, shell in enumerate(solid):
                        for i, face in enumerate(shell):
                            if ((len(face) == 1) and (len(face[0]) == 3)):
                                re = np.array(face)
                                ts.append(re[0])
                            else:
                                re, b = geom_help.triangulate_face_mapbox_earcut(face, vs)
                                if b == True:
                                    for each in re:
                                        ts.append(each)
            elif (geom['type'] == 'GeometryInstance'):
                #-- TODO: implement GeometryInstance for the trees blocking the sun!
                print("GeometryInstance not supported in this viewer (yet)")
                pass

def get_bbox(vs):
    return [ np.min(vs[:,0]), np.min(vs[:,1]), np.max(vs[:,0]), np.max(vs[:,1]) ]

def get_highest_error(f):
    herr = 0
    if "errors" in f:
        for err in f["errors"]:
            if err["code"] > herr:
                herr = err["code"]
    if f["primitives"] is not None:
        for p in f["primitives"]:
            for err in p.get("errors", []):
                if err["code"] > herr:
                    herr = err["code"]
    return herr



def visualise(vs, fs, lsurfs, r3pairReport):

    ps.set_program_name("AUTOr3pair viewer")
    ps.set_up_dir("z_up")
    ps.set_ground_plane_mode("shadow_only")
    ps.set_ground_plane_height_factor(0.01, is_relative=True)
    ps.set_autocenter_structures(True)
    ps.set_autoscale_structures(True) 
    bbox = get_bbox(vs)

    #-- shift vs to smaller coords
    xs = vs[:, 0] - bbox[0]
    ys = vs[:, 1] - bbox[1] 
    zs = vs[:, 2]
    vs2 = np.column_stack((xs, ys, zs))


    #-- valid buildings
    fs2 = []
    for f in r3pairReport["features"]:
        # TODO fix bug in c++ and then delete this
        if "repaired" not in f:
            continue
        if f["repaired"] == False:
            if f["id"] in lsurfs:
                surfs = lsurfs[f["id"]]
                for each in fs[surfs[0]:surfs[1]]:
                    fs2.append(each)
    ts = np.array(fs2, dtype=np.uint32).reshape((-1, 3))
    mesh_valid = ps.register_surface_mesh("Not Repaired", vs2, ts, color=[0.8,0.8,0.8], edge_width=0)
    mesh_valid.set_transparency(0.2)
    mesh_valid.reset_transform()


    #-- invalid buildings
    fs2 = []
    ferrs = []
    for f in r3pairReport["features"]:
        #TODO fix bug in c++ and then delete this
        if "repaired" not in f:
            continue
        if f["repaired"] ==True:
            if f["id"] in lsurfs:
                surfs = lsurfs[f["id"]]
                # herr = get_highest_error(f)
                herr = max(f["all_ISOr3pairs_primitives"])
                for each in fs[surfs[0]:surfs[1]]:
                    fs2.append(each)
                    ferrs.append(herr)
    ts = np.array(fs2, dtype=np.uint32).reshape((-1, 3))
    mesh_invalid = ps.register_surface_mesh("invalid", vs2, ts, color=[1.0, 0.2, 0.2], edge_color=[0.1, 0.1, 0.1], edge_width=1.0)
    mesh_invalid.set_transparency(0.9)
    ferrs2 = np.array(ferrs, dtype=np.uint32)
    mesh_invalid.add_scalar_quantity("errors", ferrs2, defined_on='faces', cmap='spectral', enabled=True)
    mesh_invalid.reset_transform()
    ps.show()


if __name__ == '__main__':
    main()