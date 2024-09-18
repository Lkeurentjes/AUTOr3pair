import pytest
import json

# ---------------------------------------------------------------------------------------------------------------- Data

# ----------------------------------------------------------------------------------------JSON file tests
@pytest.fixture(scope="module",
                params=["CITYJSON_cube.json"])
def data_valid_JSON_cube(request, dir_Valid_file):
    path = dir_Valid_file + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUTchangeAll.json",
                        "USERINPUTchangeSome.json"])
def data_userinput_test(request, dir_Valid_file):
    path = dir_Valid_file + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["OBJ_empty.obj",
                        "other.other",
                        "nonExistent.json"])
def data_userinput_incorrect_test(request, dir_InValid_file):
    path = dir_InValid_file + "/" + request.param
    return path


# ---------------------------------------------------------------------------------------- 1 input

def test_UseCase_DEFAULT(repair_return_test, data_valid_JSON_cube, outputnames):
    data = data_valid_JSON_cube
    code, error = repair_return_test([data_valid_JSON_cube])
    assert code == 0
    assert "No user input file given or not a valid JSON file. Standards will be used" in error

    repaired, report = outputnames(data, "")
    with open(report) as f:
        rr = json.load(f)
    assert rr["Parameters"]["UseCase"]["Name"] == "Default"

def test_UseCase_VISUALIZATION(repair_return_test, data_valid_JSON_cube, UseCaseVISUALIZATION,outputnames):
    data = data_valid_JSON_cube
    code, error = repair_return_test([data_valid_JSON_cube, UseCaseVISUALIZATION])
    assert code == 0
    assert "No user input file given or not a valid JSON file. Standards will be used" not in error

    repaired, report = outputnames(data, UseCaseVISUALIZATION)
    with open(report) as f:
        rr = json.load(f)
    assert rr["Parameters"]["UseCase"]["Name"] == "VISUALIZATION"

def test_UseCase_CFD(repair_return_test, data_valid_JSON_cube, UseCaseCFD,outputnames):
    data = data_valid_JSON_cube
    code, error = repair_return_test([data_valid_JSON_cube, UseCaseCFD])
    assert code == 0
    assert "No user input file given or not a valid JSON file. Standards will be used" not in error

    repaired, report = outputnames(data, UseCaseCFD)
    with open(report) as f:
        rr = json.load(f)
    assert rr["Parameters"]["UseCase"]["Name"] == "CFD"

def test_UseCase_ENERGYDEMAND(repair_return_test, data_valid_JSON_cube, UseCaseENERGYDEMAND,outputnames):
    data = data_valid_JSON_cube
    code, error = repair_return_test([data_valid_JSON_cube, UseCaseENERGYDEMAND])
    assert code == 0
    assert "No user input file given or not a valid JSON file. Standards will be used" not in error

    repaired, report = outputnames(data, UseCaseENERGYDEMAND)
    with open(report) as f:
        rr = json.load(f)
    assert rr["Parameters"]["UseCase"]["Name"] == "ENERGY DEMAND"

def test_UseCase_SOLARPOWER(repair_return_test, data_valid_JSON_cube, UseCaseSOLARPOWER,outputnames):
    data = data_valid_JSON_cube
    code, error = repair_return_test([data_valid_JSON_cube, UseCaseSOLARPOWER])
    assert code == 0
    assert "No user input file given or not a valid JSON file. Standards will be used" not in error

    repaired, report = outputnames(data, UseCaseSOLARPOWER)
    with open(report) as f:
        rr = json.load(f)
    assert rr["Parameters"]["UseCase"]["Name"] == "SOLAR POWER"

def test_UseCase_USERJSON(repair_return_test, data_valid_JSON_cube,data_userinput_test,outputnames):
    data = data_valid_JSON_cube
    code, error = repair_return_test([data_valid_JSON_cube,data_userinput_test])
    assert code == 0
    assert "No user input file given or not a valid JSON file. Standards will be used" not in error

    repaired, report = outputnames(data, data_userinput_test)
    with open(report) as f:
        rr = json.load(f)
    assert rr["Parameters"]["UseCase"]["Name"] == "Based on input file"

def test_UseCase_incorrect_USERJSON(repair_return_test, data_valid_JSON_cube,data_userinput_incorrect_test,outputnames):
    data = data_valid_JSON_cube
    code, error = repair_return_test([data_valid_JSON_cube,data_userinput_incorrect_test])
    assert code == 0
    assert "No user input file given or not a valid JSON file. Standards will be used" in error

    repaired, report = outputnames(data, data_userinput_incorrect_test)
    with open(report) as f:
        rr = json.load(f)
    assert rr["Parameters"]["UseCase"]["Name"] == "Default"

def test_UseCase_incorrect_USERJSON_Dir(repair_return_test, data_valid_JSON_cube, dir_test_file_formats,outputnames):
    data = data_valid_JSON_cube
    code, error = repair_return_test([data_valid_JSON_cube, dir_test_file_formats])
    assert code == 0
    assert "User input file given is a directory. Standards will be used" in error

    repaired, report = outputnames(data, "")
    with open(report) as f:
        rr = json.load(f)
    assert rr["Parameters"]["UseCase"]["Name"] == "Default"


@pytest.mark.keep
def test_clean_up_input_directory(cleanup, dir_Valid_file):
    done = cleanup(dir_Valid_file)
    assert done == 1
