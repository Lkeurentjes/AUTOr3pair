#  Copyright (c) 2024. // made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes
import pytest
import json


# ---------------------------------------------------------------------------------------------------------------- Data
@pytest.fixture(scope="module",
                params=["Valid_Msurf.city.json",
                        "Valid_Solid.city.json",
                        "Valid_MSolid.city.json",
                        "Valid_CSolid.city.json",
                        "Valid_Csurf.city.json",
                        "Valid_obj.obj"])
def data_valid_cubes(request, dir_Valid):
    path = dir_Valid + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["Valid_Msurf_LOD0.city.json"])
def data_valid_cubes_LOD0(request, dir_Valid):
    path = dir_Valid + "/" + request.param
    return path
@pytest.fixture(scope="module",
                params=["USERINPUT_KeepEverything.json"])
def data_userinput_KeepEverything(request, dir_UseCaseParameters):
    path = dir_UseCaseParameters + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_MergeTOL0.1.json"])
def data_userinput_MergeTOL0_1(request, dir_UseCaseParameters):
    path = dir_UseCaseParameters + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_MergeTOL0.9.json"])
def data_userinput_MergeTOL0_9(request, dir_UseCaseParameters):
    path = dir_UseCaseParameters + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_orientation.json"])
def data_userinput_orientation(request, dir_UseCaseParameters):
    path = dir_UseCaseParameters + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_Overlap.json"])
def data_userinput_Overlap(request, dir_UseCaseParameters):
    path = dir_UseCaseParameters + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_RemeshSlivers.json"])
def data_userinput_RemeshSlivers(request, dir_UseCaseParameters):
    path = dir_UseCaseParameters + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_semanticsAdd.json"])
def data_userinput_semanticsAdd(request, dir_UseCaseParameters):
    path = dir_UseCaseParameters + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_semanticsValidate.json"])
def data_userinput_semanticsValidate(request, dir_UseCaseParameters):
    path = dir_UseCaseParameters + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_Simplification.json"])
def data_userinput_Simplification(request, dir_UseCaseParameters):
    path = dir_UseCaseParameters + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_SkipLowRepairs.json"])
def data_userinput_SkipLowRepairs(request, dir_UseCaseParameters):
    path = dir_UseCaseParameters + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_Triangulation.json"])
def data_userinput_Triangulation(request, dir_UseCaseParameters):
    path = dir_UseCaseParameters + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["USERINPUT_watertight.json"])
def data_userinput_watertight(request, dir_UseCaseParameters):
    path = dir_UseCaseParameters + "/" + request.param
    return path


# --------------------------------------------------------------------------------------------------------------- Test

def test_parameter_KeepEverything(repair_return_test, validate, outputnames, data_valid_cubes,
                                  data_userinput_KeepEverything,
                                  unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes, data_userinput_KeepEverything])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes, data_userinput_KeepEverything)
    with open(report) as f:
        rr = json.load(f)


def test_parameter_MergeTOL0_1(repair_return_test, validate, outputnames, data_valid_cubes,
                               data_userinput_MergeTOL0_1, unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes, data_userinput_MergeTOL0_1])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes, data_userinput_MergeTOL0_1)
    with open(report) as f:
        rr = json.load(f)


def test_parameter_MergeTOL0_9(repair_return_test, validate, outputnames, data_valid_cubes,
                               data_userinput_MergeTOL0_9, unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes, data_userinput_MergeTOL0_9])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes, data_userinput_MergeTOL0_9)
    with open(report) as f:
        rr = json.load(f)


def test_parameter_orientation(repair_return_test, validate, outputnames, data_valid_cubes,
                               data_userinput_orientation, unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes, data_userinput_orientation])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes, data_userinput_orientation)
    with open(report) as f:
        rr = json.load(f)


def test_parameter_Overlap(repair_return_test, validate, outputnames, data_valid_cubes,
                           data_userinput_Overlap, unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes, data_userinput_Overlap])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes, data_userinput_Overlap)
    with open(report) as f:
        rr = json.load(f)


def test_parameter_RemeshSlivers(repair_return_test, validate, outputnames, data_valid_cubes,
                                 data_userinput_RemeshSlivers, unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes, data_userinput_RemeshSlivers])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes, data_userinput_RemeshSlivers)
    with open(report) as f:
        rr = json.load(f)


def test_parameter_semanticsAdd(repair_return_test, validate, outputnames, data_valid_cubes,
                                data_userinput_semanticsAdd, unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes, data_userinput_semanticsAdd])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes, data_userinput_semanticsAdd)
    with open(report) as f:
        rr = json.load(f)


def test_parameter_semanticsValidate(repair_return_test, validate, outputnames, data_valid_cubes,
                                     data_userinput_semanticsValidate, unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes, data_userinput_semanticsValidate])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes, data_userinput_semanticsValidate)
    with open(report) as f:
        rr = json.load(f)


def test_parameter_Simplification(repair_return_test, validate, outputnames, data_valid_cubes,
                                  data_userinput_Simplification, unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes, data_userinput_Simplification])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes, data_userinput_Simplification)
    with open(report) as f:
        rr = json.load(f)


def test_parameter_SkipLowRepairs(repair_return_test, validate, outputnames, data_valid_cubes,
                                  data_userinput_SkipLowRepairs, unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes, data_userinput_SkipLowRepairs])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes, data_userinput_SkipLowRepairs)
    with open(report) as f:
        rr = json.load(f)


def test_parameter_Triangulation(repair_return_test, validate, outputnames, data_valid_cubes,
                                 data_userinput_Triangulation, unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes, data_userinput_Triangulation])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes, data_userinput_Triangulation)
    with open(report) as f:
        rr = json.load(f)


def test_parameter_watertight(repair_return_test, validate, outputnames, data_valid_cubes,
                              data_userinput_watertight, unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes, data_userinput_watertight])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes, data_userinput_watertight)
    with open(report) as f:
        rr = json.load(f)

def test_parameter_watertight_LOD0(repair_return_test, validate, outputnames, data_valid_cubes_LOD0,
                              data_userinput_watertight, unittests):
    # Does the code run
    code, error = repair_return_test([data_valid_cubes_LOD0, data_userinput_watertight])
    assert code == 0

    repaired, report = outputnames(data_valid_cubes_LOD0, data_userinput_watertight)
    with open(report) as f:
        rr = json.load(f)
@pytest.mark.keep
def test_clean_up_Valid_directory(cleanup, dir_Valid):
    done = cleanup(dir_Valid)
    assert done == 1