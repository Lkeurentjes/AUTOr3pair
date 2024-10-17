import os

import pytest
import json

from tests.conftest import data_from_dir


# ---------------------------------------------------------------------------------------------------------------- Data
# ---------------------------------------------------------------------------------------- User input tests
@pytest.fixture(scope="module",
                params=["USERINPUT_Ring.json"])
def data_RING_UserInput(request, dir_Ring):
    path = dir_Ring + "/"  + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_Ring_skip.json"])
def data_RING_skip_UserInput(request, dir_Ring):
    path = dir_Ring + "/"  + request.param
    return path


# ---------------------------------------------------------------------------------------- per repair
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Ring_Level", True, True, 101))
def data_101(request, dir_Ring, unittests, solid, data_RING_UserInput):
    path = dir_Ring  + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_RING_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Ring_Level", True, True, 102))
def data_102(request, dir_Ring, unittests, solid, data_RING_UserInput):
    path = dir_Ring + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_RING_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Ring_Level", True, True, 104))
def data_104(request, dir_Ring, unittests, solid, data_RING_UserInput):
    path = dir_Ring  + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_RING_UserInput)

# ------------------------------------------------------------------------------------ "skipped repairs"
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Ring_Level", True, True, 104))
def data_104_skip(request, dir_Ring, unittests, solid, data_RING_skip_UserInput):
    path = dir_Ring + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_RING_skip_UserInput)


# ---------------------------------------------------------------------------------------- all together
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Ring_Level", True, True, "all"))
def data_101_102_104(request, dir_Ring, unittests, solid, data_RING_UserInput):
    path = dir_Ring + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_RING_UserInput)


# ---------------------------------------------------------------------------------------------------------------- Tests
def test_101(repair_return_test, outputnames , validate, data_101):
    data, option, user = data_101
    repair = 101
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert error == ""
    assert code == 0

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
                    if r["kind_of_repair"] == "RingRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (len(d["boundary_now"]) < len(d["boundary_before"]))


def test_102(repair_return_test,outputnames, validate, data_102):
    data, option, user = data_102
    repair = 102
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert error == ""
    assert code == 0

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
                    if r["kind_of_repair"] == "RingRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                for i in range(len(d["boundary_now"])):
                                    assert (len(d["boundary_now"][i]) <= len(d["boundary_before"][i]))


def test_104(repair_return_test,outputnames, validate, data_104):
    data, option, user = data_104
    repair = 104
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert (repair in Verror_start)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert error == ""
    assert code == 0

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    Verror_end = validate(repaired, options=option)
    assert (repair not in Verror_end)

    with open(report) as f:
        rr = json.load(f)

    for f in rr["features"]:
        if repair in f["all_Geomr3pairs"]:
            for p in f["Primitives"]:
                assert (error not in p["ISOerrorsremaining"])
                for r in p["repairs"]:
                    if r["kind_of_repair"] == "RingRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (d["boundary_now"] != [])


def test_104_SkipLowRepairs(repair_return_test,outputnames, validate, data_104_skip):
    data, option, user = data_104_skip
    repair = 104
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

    for f in rr["features"]:
        if repair in f["all_Geomr3pairs"]:
            for p in f["Primitives"]:
                if p["type"] == "MultiSurface" or p["type"] == "CompositeSurface":
                    continue
                assert (error not in p["ISOerrorsremaining"])
                for r in p["repairs"]:
                    if r["kind_of_repair"] == "RingRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                if "MSurf" in report:
                                    assert (d["boundary_now"] != [])
                                else:
                                    assert (d["boundary_now"] == [])


def test_Ring_all(repair_return_test, outputnames, validate, data_101_102_104):
    data, option, user = data_101_102_104
    repairs = [101, 102, 104]
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert all(repair in Verror_start for repair in repairs)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0
    assert error == ""

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    Verror_end = validate(repaired, options=option)
    assert all(repair not in Verror_end for repair in repairs)


def test_cityJSON_Schema_valid_RingR3pair(Valid_CityJSON, dir_Ring):
    done = Valid_CityJSON(dir_Ring)
    assert done == 1


@pytest.mark.keep
def test_clean_up_Ring_directory(cleanupRepair, dir_Ring):
    done = cleanupRepair(dir_Ring)
    assert done == 1
