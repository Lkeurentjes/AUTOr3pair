import os

import pytest
import json

from tests.conftest import data_from_dir


# ---------------------------------------------------------------------------------------------------------------- Data
# ---------------------------------------------------------------------------------------- User input tests
@pytest.fixture(scope="module",
                params=["USERINPUT_Poly.json"])
def data_POLY_UserInput(request, dir_Poly):
    path = dir_Poly + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_Poly_skip.json"])
def data_POLY_skip_UserInput(request, dir_Poly):
    path = dir_Poly + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_Poly_keep.json"])
def data_POLY_keep_all_UserInput(request, dir_Poly):
    path = dir_Poly + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_Poly_tdp.json"])
def data_POLY_changeTDP_UserInput(request, dir_Poly):
    path = dir_Poly + "/" + request.param
    return path

# ---------------------------------------------------------------------------------------- per repair
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 201))
def data_201(request, dir_Poly, unittests, solid, data_POLY_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 202))
def data_202(request, dir_Poly, unittests, solid, data_POLY_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 203))
def data_203(request, dir_Poly, unittests, solid, data_POLY_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 204))
def data_204(request, dir_Poly, unittests, solid, data_POLY_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 205))
def data_205(request, dir_Poly, unittests, solid, data_POLY_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 206))
def data_206(request, dir_Poly, unittests, solid, data_POLY_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 207))
def data_207(request, dir_Poly, unittests, solid, data_POLY_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 208))
def data_208(request, dir_Poly, unittests, solid, data_POLY_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_UserInput)



# ---------------------------------------------------------------------------------------- per repair special cases

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 201))
def data_201_skip(request, dir_Poly, unittests, solid, data_POLY_skip_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_skip_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 202))
def data_202_skip(request, dir_Poly, unittests, solid, data_POLY_skip_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_skip_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 203))
def data_203_skip(request, dir_Poly, unittests, solid, data_POLY_skip_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_skip_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 203))
def data_203_tdp(request, dir_Poly, unittests, solid, data_POLY_changeTDP_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_changeTDP_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 204))
def data_204_skip(request, dir_Poly, unittests, solid, data_POLY_skip_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_skip_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 205))
def data_205_skip(request, dir_Poly, unittests, solid, data_POLY_skip_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_skip_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 206))
def data_206_skip(request, dir_Poly, unittests, solid, data_POLY_skip_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_skip_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", False, True, 206))
def data_206_keep_all(request, dir_Poly, unittests, solid, data_POLY_keep_all_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_keep_all_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 207))
def data_207_skip(request, dir_Poly, unittests, solid, data_POLY_skip_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_skip_UserInput)

@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", False, True, 207))
def data_207_keep_all(request, dir_Poly, unittests, solid, data_POLY_keep_all_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_keep_all_UserInput)


@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, 208))
def data_208_skip(request, dir_Poly, unittests, solid, data_POLY_skip_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_skip_UserInput)


# ----------------------------------------------------------------------test all together in one object
@pytest.fixture(scope="module",
                params=data_from_dir("../data/Test_geometry_repairs/Poly_Level", True, True, "PolyAll"))
def data_PolyALL(request, dir_Poly, unittests, solid, data_POLY_UserInput):
    path = dir_Poly + request.param
    if ".json" in request.param:
        option = unittests
    else:
        option = solid
    return (path, option, data_POLY_UserInput)


# ---------------------------------------------------------------------------------------------------------------- Tests
def test_201(repair_return_test, outputnames, validate, data_201):
    data, option, user = data_201
    repair = 201
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (len(d["boundary_now"]) < len(d["boundary_before"]))

def test_201_SkipLowRepair(repair_return_test,outputnames, validate, data_201_skip):
    data, option, user = data_201_skip

    repair = 201
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                if "MSurf" in report:
                                    assert (len(d["boundary_now"]) < len(d["boundary_before"]))
                                else:
                                    assert (d["boundary_now"] == [])

def test_203(repair_return_test, outputnames, validate, data_203):
    data, option, user = data_203
    repair = 203
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (d["boundary_now"][0] == d["boundary_before"] or
                                        all(len(face[0]) == 3 for face in d["boundary_now"]))

def test_203_SkipLowRepair(repair_return_test, outputnames, validate, data_203_skip):
    data, option, user = data_203_skip

    repair = 203
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                if "MSurf" in report:
                                    assert (d["boundary_now"][0] == d["boundary_before"] or
                                            all(len(face[0]) == 3 for face in d["boundary_now"]))
                                else:
                                    assert (d["boundary_now"] == [])

def test_203_Toldp(repair_return_test, outputnames, validate, data_203_tdp):
    data, option, user = data_203_tdp

    repair = 203
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

    assert rr["Parameters"]["Tollerances"]["planarity_d2p_tol"] != 0.01

def test_204(repair_return_test, outputnames, validate, data_204):
    data, option, user = data_204
    repair = 204
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (d["boundary_now"] == d["boundary_before"])

def test_204_SkipLowRepair(repair_return_test, outputnames, validate, data_204_skip):
    data, option, user = data_204_skip

    repair = 204
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                if d["code"] == repair:
                                    if "MSurf" in report:
                                        assert (d["boundary_now"] == d["boundary_before"])
                                    else:
                                        assert (d["boundary_now"] == [])

def test_205(repair_return_test, outputnames, validate, data_205):
    data, option, user = data_205
    repair = 205
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (len(d["boundary_now"]) > 1)

def test_205_SkipLowRepair(repair_return_test, outputnames, validate, data_205_skip):
    data, option, user = data_205_skip

    repair = 205
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                if "MSurf" in report:
                                    assert (len(d["boundary_now"]) > 1)
                                else:
                                    assert (d["boundary_now"] == [])

def test_206(repair_return_test, outputnames, validate, data_206):
    data, option, user = data_206
    repair = 206
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (len(d["boundary_now"]) < len(d["boundary_before"]))

def test_206_SkipLowRepair(repair_return_test, outputnames, validate, data_206_skip):
    data, option, user = data_206_skip

    repair = 206
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                if "MSurf" in report:
                                    assert (len(d["boundary_now"]) < len(d["boundary_before"]))
                                else:
                                    assert (d["boundary_now"] == [])

def test_206_keepEverything(repair_return_test, outputnames, validate, data_206_keep_all):
    data, option, user = data_206_keep_all
    repair = 206
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (len(d["boundary_now"]) > 1)


def test_207(repair_return_test, outputnames, validate, data_207):
    data, option, user = data_207
    repair = 207
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (len(d["boundary_now"]) < len(d["boundary_before"]))

def test_207_SkipLowRepair(repair_return_test, outputnames, validate, data_207_skip):
    data, option, user = data_207_skip

    repair = 207
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                if "MSurf" in report:
                                    assert (len(d["boundary_now"]) < len(d["boundary_before"]))
                                else:
                                    assert (d["boundary_now"] == [])

def test_207_keepEverything(repair_return_test, outputnames, validate, data_207_keep_all):
    data, option, user = data_207_keep_all
    repair = 207
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert (len(d["boundary_now"]) > 1)

def test_208(repair_return_test, outputnames, validate, data_208):
    data, option, user = data_208
    repair = 208
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                assert all(now == before or now == before[::-1] for now, before in zip(d["boundary_now"], d["boundary_before"]))


def test_208_SkipLowRepair(repair_return_test, outputnames, validate, data_208_skip):
    data, option, user = data_208_skip

    repair = 208
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
                    if r["kind_of_repair"] == "PolyRepairs":
                        for d in r["repairs_done"]:
                            if d["code"] == repair:
                                if "MSurf" in report:
                                    assert all(now == before or now == before[::-1] for now, before in zip(d["boundary_now"], d["boundary_before"]))
                                else:
                                    assert (d["boundary_now"] == [])


def test_Poly_all(repair_return_test, outputnames, validate, data_PolyALL):
    data, option, user = data_PolyALL
    repairs = [201, 202, 203, 204, 205, 206, 207, 208]
    # are the error(s) there
    Verror_start = validate(data, options=option)
    assert any(repair in Verror_start for repair in repairs)

    # Does the code run
    code, error = repair_return_test([data, user])
    assert code == 0
    assert error == ""

    # does the repaired object not have the error(s)
    repaired, report = outputnames(data, user)
    assert os.path.exists(repaired)
    Verror_end = validate(repaired, options=option)
    assert all(repair not in Verror_end for repair in repairs)

# -----------------------------------------------------------------------------------------------------  Schema & Clean

def test_cityJSON_Schema_valid_PolyR3pair(Valid_CityJSON, dir_Poly):
    done = Valid_CityJSON(dir_Poly)
    assert done == 1


@pytest.mark.keep
def test_clean_up_Poly_directory(cleanupRepair, dir_Poly):
    done = cleanupRepair(dir_Poly)
    assert done == 1
