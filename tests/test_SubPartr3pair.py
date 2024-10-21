import os

import pytest
import json

from tests.conftest import data_from_dir

# ---------------------------------------------------------------------------------------------------------------- Data
# ---------------------------------------------------------------------------------------- User input tests
@pytest.fixture(scope="module",
                params=["USERINPUT_SubPart.json"])
def data_SUBPART_UserInput(request, dir_SubPart):
    path = dir_SubPart + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_SubPart_Merge0.json"])
def data_SUBPART_Merge0_UserInput(request, dir_SubPart):
    path = dir_SubPart + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_SubPart_Merge1.json"])
def data_SUBPART_Merge1_UserInput(request, dir_SubPart):
    path = dir_SubPart + "/" + request.param
    return path


# ---------------------------------------------------------------------------------------- per repair
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/SubPart_Level", True, True, 601))
def data_601(request, dir_SubPart, unittests, solid, data_SUBPART_UserInput):
    path = dir_SubPart + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SUBPART_UserInput)




# ----------------------------------------------------"other options" & sementics and materials

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/SubPart_Level", True, True, 601))
def data_601_merge0(request, dir_SubPart, unittests, solid, data_SUBPART_Merge0_UserInput):
    path = dir_SubPart + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SUBPART_Merge0_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/SubPart_Level", True, True, 601))
def data_601_merge1(request, dir_SubPart, unittests, solid, data_SUBPART_Merge1_UserInput):
    path = dir_SubPart + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SUBPART_Merge1_UserInput)
# ---------------------------------------------------------------------------------------------------------------- Tests

def test_601(repair_return_test, outputnames, validate, data_601):
    data, option, user = data_601
    repair = 601
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0
    assert error == ""

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)


def test_601_Merge0(repair_return_test, outputnames, validate, data_601_merge0):
    data, option, user = data_601_merge0
    repair = 601
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0
    assert error == ""

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)


def test_601_Merge1(repair_return_test, outputnames, validate, data_601_merge1):
    data, option, user = data_601_merge1
    repair = 601
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0
    assert error == ""

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

def test_cityJSON_Schema_valid_SubPartR3pair(Valid_CityJSON, dir_SubPart):
    done = Valid_CityJSON(dir_SubPart)
    assert done == 1

@pytest.mark.keep
def test_clean_up_SubPart_directory(cleanupRepair, dir_SubPart):
    done = cleanupRepair(dir_SubPart)
    assert done == 1
