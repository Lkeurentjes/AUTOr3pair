#  Copyright (c) 2023. made for a proof of concept for theis Automatic repair of 3d citymodels by Lisa Keurentjes
import pytest


# ---------------------------------------------------------------------------------------- data
@pytest.fixture(scope="module",
                params=["Valid_Msurf.city.json",
                        "Valid_Solid.city.json",
                        "Valid_MSolid.city.json",
                        "Valid_CSolid.city.json",
                        "Valid_Csurf.city.json"])
def data_valid_JSON(request, dir_Valid):
    path = dir_Valid + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["Valid_obj.obj"])
def data_valid_OBJ(request, dir_Valid):
    path = dir_Valid + "/" + request.param
    return path

# ---------------------------------------------------------------------------------------- test
def test_Valid_JSON(repair_return_test, validate, data_valid_JSON, unittests):
    #are the error(s) there
    Verror_start = validate(data_valid_JSON, options=unittests)
    assert Verror_start == []

    # Does the code run
    code, error = repair_return_test([data_valid_JSON])
    assert code == 0
    assert "No repairs done, so no 'repaired' file will be written" in error

def test_Valid_OBJ(repair_return_test, validate, data_valid_OBJ, solid):
    #are the error(s) there
    Verror_start = validate(data_valid_OBJ, options=solid)
    assert Verror_start == []

    # Does the code run
    code, error = repair_return_test([data_valid_OBJ])
    assert code == 0
    assert "No repairs done, so no 'repaired' file will be written" in error


@pytest.mark.keep
def test_clean_up_Valid_directory(cleanup, dir_Valid):
    done = cleanup(dir_Valid)
    assert done == 1
