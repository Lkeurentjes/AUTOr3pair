#  Copyright (c) 2024. // made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes
import pytest
import json

# ---------------------------------------------------------------------------------------------------------------- Data
@pytest.fixture(scope="module", params=["CITYJSON_cube_change_scale.json"])
def data_change_scale_JSON_cube(request, dir_prepost_processing):
    path = dir_prepost_processing + "/" + request.param
    return path

@pytest.fixture(scope="module", params=["CITYJSON_cube_Csolid.json",
                                        "CITYJSON_cube_Msolid.json"])
def data_CMsolid_JSON_cube(request, dir_prepost_processing):
    path = dir_prepost_processing + "/" + request.param
    return path

@pytest.fixture(scope="module", params=["CITYJSON_cube_duplicate.json",
                                        "CITYJSON_cube_duplicateSNAP.json"])
def data_duplicate_JSON_cube(request, dir_prepost_processing):
    path = dir_prepost_processing + "/" + request.param
    return path

@pytest.fixture(scope="module", params=["CITYJSON_cube_geographicalExtent.json"])
def data_geographicalExtent_JSON_cube(request, dir_prepost_processing):
    path = dir_prepost_processing + "/" + request.param
    return path

@pytest.fixture(scope="module", params=["CITYJSON_cube_OrphanVertex.json"])
def data_OrphanVertex_JSON_cube(request, dir_prepost_processing):
    path = dir_prepost_processing + "/" + request.param
    return path

@pytest.fixture(scope="module", params=["CITYJSON_cube_texture.json"])
def data_texture_JSON_cube(request, dir_prepost_processing):
    path = dir_prepost_processing + "/" + request.param
    return path

@pytest.fixture(scope="module", params=["OBJ_cube_round_vertices.obj"])
def data_round_vertices_OBJ_cube(request, dir_prepost_processing):
    path = dir_prepost_processing + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_GenericCityObject.json"])
def data_userinput(request, dir_prepost_processing):
    path = dir_prepost_processing + "/" + request.param
    return path

# --------------------------------------------------------------------------------------------------------------- Test

def test_Change_Scale_JSON(repair_return_test, validate, outputnames,data_change_scale_JSON_cube, data_userinput, unittests):
    # Does the code run
    code, error = repair_return_test([data_change_scale_JSON_cube, data_userinput])
    assert code == 0
    assert not("No repairs done, so no 'repaired' file will be written" in error)

    repaired, report = outputnames(data_change_scale_JSON_cube, data_userinput)
    with open(report) as f:
        rr = json.load(f)

def test_CMsolid_to_Solid_JSON(repair_return_test, validate, outputnames, data_CMsolid_JSON_cube, data_userinput, unittests):
    # Does the code run
    code, error = repair_return_test([data_CMsolid_JSON_cube, data_userinput])
    assert code == 0
    assert not("No repairs done, so no 'repaired' file will be written" in error)

    repaired, report = outputnames(data_CMsolid_JSON_cube, data_userinput)
    with open(report) as f:
        rr = json.load(f)

def test_duplicateVertex_JSON(repair_return_test, validate, outputnames, data_duplicate_JSON_cube, data_userinput, unittests):
    # Does the code run
    code, error = repair_return_test([data_duplicate_JSON_cube, data_userinput])
    assert code == 0
    assert not("No repairs done, so no 'repaired' file will be written" in error)

    repaired, report = outputnames(data_duplicate_JSON_cube, data_userinput)
    with open(report) as f:
        rr = json.load(f)

def test_geographicalExtent_JSON(repair_return_test, validate, outputnames, data_geographicalExtent_JSON_cube, data_userinput, unittests):
    # Does the code run
    code, error = repair_return_test([data_geographicalExtent_JSON_cube, data_userinput])
    assert code == 0
    assert not("No repairs done, so no 'repaired' file will be written" in error)

    repaired, report = outputnames(data_geographicalExtent_JSON_cube, data_userinput)
    with open(report) as f:
        rr = json.load(f)

def test_OrphanVertex_JSON(repair_return_test, validate, outputnames, data_OrphanVertex_JSON_cube, data_userinput, unittests):
    # Does the code run
    code, error = repair_return_test([data_OrphanVertex_JSON_cube, data_userinput])
    assert code == 0
    assert not("No repairs done, so no 'repaired' file will be written" in error)

    repaired, report = outputnames(data_OrphanVertex_JSON_cube, data_userinput)
    with open(report) as f:
        rr = json.load(f)

def test_texture_JSON(repair_return_test, validate, outputnames, data_texture_JSON_cube, data_userinput, unittests):
    # Does the code run
    code, error = repair_return_test([data_texture_JSON_cube, data_userinput])
    assert code == 0
    assert not("No repairs done, so no 'repaired' file will be written" in error)

    repaired, report = outputnames(data_texture_JSON_cube, data_userinput)
    with open(report) as f:
        rr = json.load(f)

def test_round_up_vertices_OBJ(repair_return_test, validate, outputnames, data_round_vertices_OBJ_cube, data_userinput, unittests):
    # Does the code run
    code, error = repair_return_test([data_round_vertices_OBJ_cube, data_userinput])
    assert code == 0
    assert not("No repairs done, so no 'repaired' file will be written" in error)

    repaired, report = outputnames(data_round_vertices_OBJ_cube, data_userinput)
    with open(report) as f:
        rr = json.load(f)

@pytest.mark.keep
def test_clean_up_input_directory(cleanup, dir_prepost_processing):
    done = cleanup(dir_prepost_processing)
    assert done == 1