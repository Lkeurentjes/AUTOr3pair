import pytest


# ---------------------------------------------------------------------------------------------------------------- Data

# --------------------------------------------------------JSON file tests
@pytest.fixture(scope="module",
                params=["CITYJSON_cube.json",
                        "CITYJSON_3dbag.json"])
def data_valid_JSON_test(request, dir_Valid_file):
    path = dir_Valid_file + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["ksajgfsaejk.json",
                        "empty.json"])
def data_invalid_JSON_test(request, dir_InValid_file):
    path = dir_InValid_file + "/" + request.param
    return path


# -----------------------------------------------------OBJ file tests

@pytest.fixture(scope="module",
                params=["OBJ_cube_with_normals.obj",
                        "OBJ_multiplesolids.obj",
                        "OBJ_3dbag.obj"])
def data_valid_OBJ_test(request, dir_Valid_file):
    path = dir_Valid_file + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["OBJ_CFDexample.obj"])
def data_big_OBJ_test(request, dir_Valid_file):
    path = dir_Valid_file + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["ksajgfsaejk.obj",
                        "OBJ_empty.obj"])
def data_invalid_OBJ_test(request, dir_InValid_file):
    path = dir_InValid_file + "/" + request.param
    return path


# ----------------------------------------------------------------------------------------user input file tests
@pytest.fixture(scope="module",
                params=["USERINPUTchangeAll.json",
                        "USERINPUTchangeSome.json"])
def data_userinput_test(request, dir_Valid_file):
    path = dir_Valid_file + "/" + request.param
    return path


# ----------------------------------------------------------------------------------------other file tests

@pytest.fixture(scope="module",
                params=["other.OTHER"])
def data_other_test(request, dir_InValid_file):
    path = dir_InValid_file + "/" + request.param
    return path


# --------------------------------------------------------------------------------------------------- tests

# ---------------------------------------------------------------------------------------- 0 inputs
def test_0_input(repair_return_test):
    code, error = repair_return_test([])
    assert code == 1
    assert "No inputs are given, input a (city)JSON or OBJ file" in error

# ---------------------------------------------------------------------------------------- 1 input

# ------------json

def test_1_input_valid_CityJSON(repair_return_test, data_valid_JSON_test):
    code, error = repair_return_test([data_valid_JSON_test])
    assert code == 0
    assert "No user input file given or not a valid JSON file. Standards will be used" in error


def test_1_input_invalid_JSON(repair_return_test, data_invalid_JSON_test):
    code, error = repair_return_test([data_invalid_JSON_test])
    assert code == 1
    assert (("No user input file given or not a valid JSON file. Standards will be used" in error) and
            ("Input file not a valid (city)JSON file" in error))

def test_1_input_notCityJSON(repair_return_test, data_userinput_test):
    code, error = repair_return_test([data_userinput_test])
    assert code == 1
    assert (("No user input file given or not a valid JSON file. Standards will be used" in error) and
            ("Input file not a CityJSON file" in error))

# ------------ obj
def test_1_input_invalid_OBJ(repair_return_test, data_invalid_OBJ_test):
    code, error = repair_return_test([data_invalid_OBJ_test])
    assert code == 1
    assert (("No user input file given or not a valid JSON file. Standards will be used" in error) and
            ("Input file not a valid OBJ file" in error))

def test_1_input_valid_OBJ(repair_return_test, data_valid_OBJ_test):
    code, error = repair_return_test([data_valid_OBJ_test])
    assert code == 0
    assert "No user input file given or not a valid JSON file. Standards will be used" in error


@pytest.mark.full
def test_1_input_valid_big_OBJ(repair_return_test, data_CFD_OBJ_test):
    code, error = repair_return_test([data_CFD_OBJ_test])
    assert code == 0
    assert "No user input file given or not a valid JSON file. Standards will be used" in error

# ------------ other
def test_1_wrong_type(repair_return_test, data_other_test):
    code, error = repair_return_test([data_other_test])
    assert code == 1
    assert (("No user input file given or not a valid JSON file. Standards will be used" in error) and
            ("Cannot repair 'other' type files at this moment" in error))


def test_1_input_dir(repair_return_test, dir_test_file_formats):
    code, error = repair_return_test([dir_test_file_formats])
    assert code == 1


@pytest.mark.keep
def test_clean_up_input_directory(cleanup, dir_test_file_formats, dir_Valid_file, dir_InValid_file, dir_Valid):
    done = cleanup(dir_test_file_formats)
    done = cleanup(dir_Valid_file)
    done = cleanup(dir_InValid_file)
    done = cleanup(dir_Valid)
    assert done == 1
