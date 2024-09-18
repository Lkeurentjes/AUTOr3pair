import os

import pytest
import json

from tests.conftest import data_from_dir


# ---------------------------------------------------------------------------------------------------------------- Data
# ---------------------------------------------------------------------------------------- User input tests
@pytest.fixture(scope="module",
                params=["USERINPUT_Shell.json"])
def data_SHELL_UserInput(request, dir_Shell):
    path = dir_Shell + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_Shell_keep.json"])
def data_SHELL_keep_UserInput(request, dir_Shell):
    path = dir_Shell + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_Shell_orientation.json"])
def data_SHELL_orientation_UserInput(request, dir_Shell):
    path = dir_Shell + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_Shell_triangulation.json"])
def data_SHELL_triangulation_UserInput(request, dir_Shell):
    path = dir_Shell + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_Shell_watertight.json"])
def data_SHELL_watertight_UserInput(request, dir_Shell):
    path = dir_Shell + "/" + request.param
    return path

# ---------------------------------------------------------------------------------------- per repair
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 300))
def data_300(request, dir_Shell, unittests, solid, data_SHELL_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 301))
def data_301(request, dir_Shell, unittests, solid, data_SHELL_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 302))
def data_302(request, dir_Shell, unittests, solid, data_SHELL_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 303))
def data_303(request, dir_Shell, unittests, solid, data_SHELL_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 305))
def data_305(request, dir_Shell, unittests, solid, data_SHELL_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 306))
def data_306(request, dir_Shell, unittests, solid, data_SHELL_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 307))
def data_307(request, dir_Shell, unittests, solid, data_SHELL_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_UserInput)

# ----------------------------------------------------"other options"
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 300))
def data_300_triangulation(request, dir_Shell, unittests, solid, data_SHELL_triangulation_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_triangulation_UserInput)
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 301))
def data_301_triangulation(request, dir_Shell, unittests, solid, data_SHELL_triangulation_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_triangulation_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 302))
def data_302_triangulation(request, dir_Shell, unittests, solid, data_SHELL_triangulation_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_triangulation_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 303))
def data_303_triangulation(request, dir_Shell, unittests, solid, data_SHELL_triangulation_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_triangulation_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 303))
def data_303_keep(request, dir_Shell, unittests, solid, data_SHELL_keep_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_keep_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 303))
def data_303_watertight(request, dir_Shell, unittests, solid, data_SHELL_watertight_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_watertight_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 303))
def data_303_orientation(request, dir_Shell, unittests, solid, data_SHELL_orientation_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_orientation_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 305))
def data_305_triangulation(request, dir_Shell, unittests, solid, data_SHELL_triangulation_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_triangulation_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 305))
def data_305_keep(request, dir_Shell, unittests, solid, data_SHELL_keep_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_keep_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 305))
def data_305_watertight(request, dir_Shell, unittests, solid, data_SHELL_watertight_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_watertight_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 305))
def data_305_orientation(request, dir_Shell, unittests, solid, data_SHELL_orientation_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_orientation_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 306))
def data_306_triangulation(request, dir_Shell, unittests, solid, data_SHELL_triangulation_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_triangulation_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 306))
def data_306_watertight(request, dir_Shell, unittests, solid, data_SHELL_watertight_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_watertight_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Shell_Level", True, True, 306))
def data_306_orientation(request, dir_Shell, unittests, solid, data_SHELL_orientation_UserInput):
    path = dir_Shell + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SHELL_orientation_UserInput)

# ---------------------------------------------------------------------------------------------------------------- Tests
def test_300(repair_return_test, outputnames, validate, data_300):
    data, option, user = data_300
    repair = 300
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_300_triangulation(repair_return_test, outputnames, validate, data_300_triangulation):
    data, option, user = data_300_triangulation
    repair = 300
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_301(repair_return_test, outputnames, validate, data_301):
    data, option, user = data_301
    repair = 301
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_301_triangulation(repair_return_test, outputnames, validate, data_301_triangulation):
    data, option, user = data_301_triangulation
    repair = 301
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_302(repair_return_test, outputnames, validate, data_302):
    data, option, user = data_302
    repair = 302
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_302_triangulation(repair_return_test, outputnames, validate, data_302_triangulation):
    data, option, user = data_302_triangulation
    repair = 302
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_303(repair_return_test, outputnames, validate, data_303):
    data, option, user = data_303
    repair = 303
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_303_triangulation(repair_return_test, outputnames, validate, data_303_triangulation):
    data, option, user = data_303_triangulation
    repair = 303
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_303_keep(repair_return_test, outputnames, validate, data_303_keep):
    data, option, user = data_303_keep
    repair = 303
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_303_watertight(repair_return_test, outputnames, validate, data_303_watertight):
    data, option, user = data_303_watertight
    repair = 303
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_303_orientation(repair_return_test, outputnames, validate, data_303_orientation):
    data, option, user = data_303_orientation
    repair = 303
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_305(repair_return_test, outputnames, validate, data_305):
    data, option, user = data_305
    repair = 305
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_305_triangulation(repair_return_test, outputnames, validate, data_305_triangulation):
    data, option, user = data_305_triangulation
    repair = 305
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_305_keep(repair_return_test, outputnames, validate, data_305_keep):
    data, option, user = data_305_keep
    repair = 305
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_305_watertight(repair_return_test, outputnames, validate, data_305_watertight):
    data, option, user = data_305_watertight
    repair = 305
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_305_orientation(repair_return_test, outputnames, validate, data_305_orientation):
    data, option, user = data_305_orientation
    repair = 305
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_306(repair_return_test, outputnames, validate, data_306):
    data, option, user = data_306
    repair = 306
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_306_triangulation(repair_return_test, outputnames, validate, data_306_triangulation):
    data, option, user = data_306_triangulation
    repair = 306
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_306_watertight(repair_return_test, outputnames, validate, data_306_watertight):
    data, option, user = data_306_watertight
    repair = 306
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

def test_306_orientation(repair_return_test, outputnames, validate, data_306_orientation):
    data, option, user = data_306_orientation
    repair = 306
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)
def test_307(repair_return_test, outputnames, validate, data_307):
    data, option, user = data_307
    repair = 307
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)


def test_cityJSON_Schema_valid_ShellR3pair(Valid_CityJSON, dir_Shell):
    done = Valid_CityJSON(dir_Shell)
    assert done == 1

@pytest.mark.keep
def test_clean_up_Shell_directory(cleanupRepair, dir_Shell):
    done = cleanupRepair(dir_Shell)
    assert done == 1
