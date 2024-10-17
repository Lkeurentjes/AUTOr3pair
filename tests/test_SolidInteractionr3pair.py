import os

import pytest
import json

from tests.conftest import data_from_dir


# ---------------------------------------------------------------------------------------------------------------- Data
# ---------------------------------------------------------------------------------------- User input tests
@pytest.fixture(scope="module",
                params=["USERINPUT_SolidI.json"])
def data_SOLIDINTERACTION_UserInput(request, dir_SolidInteraction):
    path = dir_SolidInteraction + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_SolidI_Merge0.json"])
def data_SOLIDINTERACTION_Merge0_UserInput(request, dir_SolidInteraction):
    path = dir_SolidInteraction + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_SolidI_Merge1.json"])
def data_SOLIDINTERACTION_Merge1_UserInput(request, dir_SolidInteraction):
    path = dir_SolidInteraction + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_SolidI_Watertight.json"])
def data_SOLIDINTERACTION_watertight_UserInput(request, dir_SolidInteraction):
    path = dir_SolidInteraction + "/" + request.param
    return path


# ---------------------------------------------------------------------------------------- per repair
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/SolidInteraction_Level", True, True, 501))
def data_501(request, dir_SolidInteraction, unittests, solid, data_SOLIDINTERACTION_UserInput):
    path = dir_SolidInteraction + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLIDINTERACTION_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/SolidInteraction_Level", True, True, 502))
def data_502(request, dir_SolidInteraction, unittests, solid, data_SOLIDINTERACTION_UserInput):
    path = dir_SolidInteraction + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLIDINTERACTION_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/SolidInteraction_Level", True, True, 503))
def data_503(request, dir_SolidInteraction, unittests, solid, data_SOLIDINTERACTION_UserInput):
    path = dir_SolidInteraction + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLIDINTERACTION_UserInput)


# ----------------------------------------------------"other options"
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/SolidInteraction_Level", True, True, 501))
def data_501_merge0(request, dir_SolidInteraction, unittests, solid, data_SOLIDINTERACTION_Merge0_UserInput):
    path = dir_SolidInteraction + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLIDINTERACTION_Merge0_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/SolidInteraction_Level", True, True, 501))
def data_501_merge1(request, dir_SolidInteraction, unittests, solid, data_SOLIDINTERACTION_Merge1_UserInput):
    path = dir_SolidInteraction + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLIDINTERACTION_Merge1_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/SolidInteraction_Level", True, True, 503))
def data_503_watertight(request, dir_SolidInteraction, unittests, solid, data_SOLIDINTERACTION_watertight_UserInput):
    path = dir_SolidInteraction + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_SOLIDINTERACTION_watertight_UserInput)


# ---------------------------------------------------------------------------------------------------------------- Tests
def test_501(repair_return_test, outputnames, validate, data_501):
    data, option, user = data_501
    repair = 501
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
                                assert (len(d["boundary_now"]) <= len(d["boundary_before"]))

def test_501_Merge0(repair_return_test, outputnames, validate, data_501_merge0):
    data, option, user = data_501_merge0
    repair = 501
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
                                assert (len(d["boundary_now"]) < len(d["boundary_before"]))

def test_501_Merge1(repair_return_test, outputnames, validate, data_501_merge1):
    data, option, user = data_501_merge1
    repair = 501
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
                                assert (len(d["boundary_now"]) <= len(d["boundary_before"]))

def test_502(repair_return_test, outputnames, validate, data_502):
    data, option, user = data_502
    repair = 502
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
                                assert (len(d["boundary_now"]) < len(d["boundary_before"]))

def test_503(repair_return_test, outputnames, validate, data_503):
    data, option, user = data_503
    repair = 503
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

def test_503_watertight(repair_return_test,check_watertight, outputnames, validate, data_503_watertight):
    data, option, user = data_503_watertight
    repair = 503
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


def test_cityJSON_Schema_valid_SolidInteractionR3pair(Valid_CityJSON, dir_SolidInteraction):
    done = Valid_CityJSON(dir_SolidInteraction)
    assert done == 1

@pytest.mark.keep
def test_clean_up_SolidInteraction_directory(cleanupRepair, dir_SolidInteraction):
    done = cleanupRepair(dir_SolidInteraction)
    assert done == 1
