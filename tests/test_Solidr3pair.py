import os

import pytest
import json

from tests.conftest import data_from_dir, check_watertight


# ---------------------------------------------------------------------------------------------------------------- Data
# ---------------------------------------------------------------------------------------- User input tests
@pytest.fixture(scope="module",
                params=["USERINPUT_Solid.json"])
def data_SOLID_UserInput(request, dir_Solid):
    path = dir_Solid + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_Solid_keep.json"])
def data_SOLID_keep_UserInput(request, dir_Solid):
    path = dir_Solid + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_Solid_keep_watertight.json"])
def data_SOLID_keep_watertight_UserInput(request, dir_Solid):
    path = dir_Solid + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_Solid_watertight.json"])
def data_SOLID_watertight_UserInput(request, dir_Solid):
    path = dir_Solid + "/" + request.param
    return path


# ---------------------------------------------------------------------------------------- per repair
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Solid_Level", True, True, 401))
def data_401(request, dir_Solid, unittests, solid, data_SOLID_UserInput):
    path = dir_Solid + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLID_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Solid_Level", True, True, 402))
def data_402(request, dir_Solid, unittests, solid, data_SOLID_UserInput):
    path = dir_Solid + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLID_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Solid_Level", True, True, 403))
def data_403(request, dir_Solid, unittests, solid, data_SOLID_UserInput):
    path = dir_Solid + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLID_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Solid_Level", True, True, 404))
def data_404(request, dir_Solid, unittests, solid, data_SOLID_UserInput):
    path = dir_Solid + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLID_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Solid_Level", True, True, 405))
def data_405(request, dir_Solid, unittests, solid, data_SOLID_UserInput):
    path = dir_Solid + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLID_UserInput)

# ----------------------------------------------------"other options" & sementics and materials
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Solid_Level", True, True, 403))
def data_403_keep(request, dir_Solid, unittests, solid, data_SOLID_keep_UserInput):
    path = dir_Solid + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLID_keep_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Solid_Level", True, True, 403))
def data_403_keep_watertight(request, dir_Solid, unittests, solid, data_SOLID_keep_watertight_UserInput):
    path = dir_Solid + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLID_keep_watertight_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Solid_Level", True, True, 404))
def data_404_watertight(request, dir_Solid, unittests, solid, data_SOLID_watertight_UserInput):
    path = dir_Solid + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLID_watertight_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Solid_Level", True, True, 405))
def data_405_watertight(request, dir_Solid, unittests, solid, data_SOLID_watertight_UserInput):
    path = dir_Solid + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLID_watertight_UserInput)

# ---------------------------------------------------------------------------------------------------------------- Tests


def test_401(repair_return_test, outputnames, validate, data_401):
    data, option, user = data_401
    repair = 401
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

    with open(report) as f:
        rr = json.load(f)

    for f in rr["features"]:
        if repair in f["all_Geomr3pairs"]:
            for p in f["Primitives"]:
                assert (error not in p["ISOerrorsremaining"])
                for r in p["repairs"]:
                    if r["kind_of_repair"] == "SolidRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (len(d["boundary_now"][0]) < len(d["boundary_before"]))

def test_403(repair_return_test, outputnames, validate, data_403):
    data, option, user = data_403
    repair = 403
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

    with open(report) as f:
        rr = json.load(f)

    for f in rr["features"]:
        if repair in f["all_Geomr3pairs"]:
            for p in f["Primitives"]:
                assert (error not in p["ISOerrorsremaining"])
                for r in p["repairs"]:
                    if r["kind_of_repair"] == "SolidRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (len(d["boundary_now"][0]) < len(d["boundary_before"]))

def test_403_keep(repair_return_test, outputnames, validate, data_403_keep):
    data, option, user = data_403_keep
    repair = 403
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

    with open(report) as f:
        rr = json.load(f)

    for f in rr["features"]:
        if repair in f["all_Geomr3pairs"]:
            for p in f["Primitives"]:
                assert (error not in p["ISOerrorsremaining"])
                for r in p["repairs"]:
                    if r["kind_of_repair"] == "SolidRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (len(d["boundary_now"]) > 1)

def test_403_keep_watertight(repair_return_test, check_watertight, outputnames, validate, data_403_keep_watertight):
    data, option, user = data_403_keep_watertight
    repair = 403
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

    with open(repaired) as f:
        cj = json.load(f)
        assert check_watertight(cj)

def test_404(repair_return_test, outputnames, validate, data_404):
    data, option, user = data_404
    repair = 404
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

    with open(report) as f:
        rr = json.load(f)

    for f in rr["features"]:
        if repair in f["all_Geomr3pairs"]:
            for p in f["Primitives"]:
                assert (error not in p["ISOerrorsremaining"])
                for r in p["repairs"]:
                    if r["kind_of_repair"] == "SolidRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (len(d["boundary_now"]) > 1)

def test_404_watertight(repair_return_test,check_watertight, outputnames, validate, data_404_watertight):
    data, option, user = data_404_watertight
    repair = 404
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

    with open(repaired) as f:
        cj = json.load(f)
        assert check_watertight(cj)

def test_405(repair_return_test, outputnames, validate, data_405):
    data, option, user = data_405
    repair = 405
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

    with open(report) as f:
        rr = json.load(f)

    for f in rr["features"]:
        if repair in f["all_Geomr3pairs"]:
            for p in f["Primitives"]:
                assert (error not in p["ISOerrorsremaining"])
                for r in p["repairs"]:
                    if r["kind_of_repair"] == "SolidRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert all(
                                    now == before or now == before[::-1]
                                    for now_list, before_list in zip(d["boundary_now"], d["boundary_before"])
                                    for now_sublist, before_sublist in zip(now_list, before_list)
                                    for now, before in zip(now_sublist, before_sublist)
                                )

def test_cityJSON_Schema_valid_SolidR3pair(Valid_CityJSON, dir_Solid):
    done = Valid_CityJSON(dir_Solid)
    assert done == 1

@pytest.mark.keep
def test_clean_up_Solid_directory(cleanupRepair, dir_Solid):
    done = cleanupRepair(dir_Solid)
    assert done == 1
