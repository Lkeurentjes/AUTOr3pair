#  Copyright (c) 2024. // made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes
import pytest
import json

@pytest.fixture(scope="module",
                params=["CITYJSON_two_cubes_LOD.json"])
def data_two_LOD_cubes(request, dir_Valid_file):
    path = dir_Valid_file + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUTchangeSome.json"])
def data_userinput_test(request, dir_Valid_file):
    path = dir_Valid_file + "/" + request.param
    return path


# ---------------------------------------------------------------------------------------- tests
def test_LOD_as_2(repair_return_test, data_two_LOD_cubes, outputnames):
    data = data_two_LOD_cubes
    LOD = "-LOD1.0"
    code, error = repair_return_test([data_two_LOD_cubes, LOD])
    assert code == 0
    assert "No user input file given or not a valid JSON file. Standards will be used" in error

    repaired, report = outputnames(data, "", "1.0")
    with open(report) as f:
        rr = json.load(f)
    assert rr["Parameters"]["LOD"] == "1.0"

def test_LOD_as_3_afterFile(repair_return_test, data_two_LOD_cubes, data_userinput_test,outputnames):
    data = data_two_LOD_cubes
    LOD = "-LOD1.0"
    code, error = repair_return_test([data_two_LOD_cubes, data_userinput_test, LOD])
    assert code == 0

    repaired, report = outputnames(data, data_userinput_test, "1.0")
    with open(report) as f:
        rr = json.load(f)
    assert rr["Parameters"]["LOD"] == "1.0"

def test_LOD_as_3_afterUC(repair_return_test, data_two_LOD_cubes, UseCaseSOLARPOWER,outputnames):
    data = data_two_LOD_cubes
    LOD = "-LOD1.0"
    code, error = repair_return_test([data_two_LOD_cubes, UseCaseSOLARPOWER, LOD])
    assert code == 0

    repaired, report = outputnames(data, UseCaseSOLARPOWER, "1.0")
    with open(report) as f:
        rr = json.load(f)
    assert rr["Parameters"]["LOD"] == "1.0"

def test_LOD_NotExsisting(repair_return_test, data_two_LOD_cubes, outputnames):
    data = data_two_LOD_cubes
    LOD = "-LOD6.9"
    code, error = repair_return_test([data_two_LOD_cubes, LOD])
    assert code == 0
    assert "No user input file given or not a valid JSON file. Standards will be used" in error

    repaired, report = outputnames(data, "", "6.9")
    with open(report) as f:
        rr = json.load(f)
    assert rr["Parameters"]["LOD"] == "6.9"

@pytest.mark.keep
def test_clean_up_input_directory(cleanup, dir_Valid_file):
    done = cleanup(dir_Valid_file)
    assert done == 1