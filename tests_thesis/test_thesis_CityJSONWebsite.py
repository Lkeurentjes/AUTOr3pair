#  Copyright (c) 2024. // made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes
import pytest
import json


# ---------------------------------------------------------------------------------------------------------------- Data
@pytest.fixture(scope="module",
                params=["01_Brussels.json"])
def data_00_Brussel(request, dir_Other):
    path = dir_Other + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_Brussel_Orientation.json"])
def data_Brussel_UI_Orientation(request, dir_Other):
    path = dir_Other + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["USERINPUT_Brussel_Watertight.json"])
def data_Brussel_UI_Watertight(request, dir_Other):
    path = dir_Other + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=["01_BAG.city.json"])
def data_01_BAG(request, dir_CityJSON_website):
    path = dir_CityJSON_website + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["02_DenHaag.city.json"])
def data_02_DenHaag(request, dir_CityJSON_website):
    path = dir_CityJSON_website + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["03_Ingolstadt.city.json"])
def data_03_Ingolstadt(request, dir_CityJSON_website):
    path = dir_CityJSON_website + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["04_Montreal.city.json"])
def data_04_Montreal(request, dir_CityJSON_website):
    path = dir_CityJSON_website + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["05_NewYork.city.json"])
def data_05_NewYork(request, dir_CityJSON_website):
    path = dir_CityJSON_website + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["06_Railway.city.json"])
def data_06_Railway(request, dir_CityJSON_website):
    path = dir_CityJSON_website + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["07_Rotterdam.city.json"])
def data_07_Rotterdam(request, dir_CityJSON_website):
    path = dir_CityJSON_website + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["08_Vienna.city.json"])
def data_08_Vienna(request, dir_CityJSON_website):
    path = dir_CityJSON_website + "/" + request.param
    return path


@pytest.fixture(scope="module",
                params=["09_Zurich.city.json"])
def data_09_Zurich(request, dir_CityJSON_website):
    path = dir_CityJSON_website + "/" + request.param
    return path


# --------------------------------------------------------------------------------------------------------------- Tests

# def test_00_Brussel(repair_return_test, data_00_Brussel):
#     code, error = repair_return_test([data_00_Brussel])
#     assert code == 0

# def test_00_BrusselO(repair_return_test, data_00_Brussel, data_Brussel_UI_Orientation):
#     code, error = repair_return_test([data_00_Brussel, data_Brussel_UI_Orientation])
#     assert code == 0

# def test_00_BrusselW(repair_return_test, data_00_Brussel, data_Brussel_UI_Watertight):
#     code, error = repair_return_test([data_00_Brussel, data_Brussel_UI_Watertight])
#     assert code == 0

# def test_01_BAG(repair_return_test, data_01_BAG):
#     code, error = repair_return_test([data_01_BAG])
#     assert code == 0


# def test_01_BAG_CFD(repair_return_test, data_01_BAG, UseCaseCFD):
#     code, error = repair_return_test([data_01_BAG, UseCaseCFD])
#     assert code == 0


def test_01_BAG_ED(repair_return_test, data_01_BAG, UseCaseENERGYDEMAND):
    code, error = repair_return_test([data_01_BAG, UseCaseENERGYDEMAND])
    assert code == 0


def test_01_BAG_VIZ(repair_return_test, data_01_BAG, UseCaseVISUALIZATION):
    code, error = repair_return_test([data_01_BAG, UseCaseVISUALIZATION])
    assert code == 0


def test_01_BAG_SP(repair_return_test, data_01_BAG, UseCaseSOLARPOWER):
    code, error = repair_return_test([data_01_BAG, UseCaseSOLARPOWER])
    assert code == 0


def test_02_DenHaag(repair_return_test, data_02_DenHaag):
    code, error = repair_return_test([data_02_DenHaag])
    assert code == 0


def test_02_DenHaag_CFD(repair_return_test, data_02_DenHaag, UseCaseCFD):
    code, error = repair_return_test([data_02_DenHaag, UseCaseCFD])
    assert code == 0


def test_02_DenHaag_ED(repair_return_test, data_02_DenHaag, UseCaseENERGYDEMAND):
    code, error = repair_return_test([data_02_DenHaag, UseCaseENERGYDEMAND])
    assert code == 0


def test_02_DenHaag_VIZ(repair_return_test, data_02_DenHaag, UseCaseVISUALIZATION):
    code, error = repair_return_test([data_02_DenHaag, UseCaseVISUALIZATION])
    assert code == 0


def test_02_DenHaag_SP(repair_return_test, data_02_DenHaag, UseCaseSOLARPOWER):
    code, error = repair_return_test([data_02_DenHaag, UseCaseSOLARPOWER])
    assert code == 0


def test_03_Ingolstadt(repair_return_test, data_03_Ingolstadt):
    code, error = repair_return_test([data_03_Ingolstadt])
    assert code == 0


def test_03_Ingolstadt_CFD(repair_return_test, data_03_Ingolstadt, UseCaseCFD):
    code, error = repair_return_test([data_03_Ingolstadt, UseCaseCFD])
    assert code == 0


def test_03_Ingolstadt_ED(repair_return_test, data_03_Ingolstadt, UseCaseENERGYDEMAND):
    code, error = repair_return_test([data_03_Ingolstadt, UseCaseENERGYDEMAND])
    assert code == 0


def test_03_Ingolstadt_VIZ(repair_return_test, data_03_Ingolstadt, UseCaseVISUALIZATION):
    code, error = repair_return_test([data_03_Ingolstadt, UseCaseVISUALIZATION])
    assert code == 0


def test_03_Ingolstadt_SP(repair_return_test, data_03_Ingolstadt, UseCaseSOLARPOWER):
    code, error = repair_return_test([data_03_Ingolstadt, UseCaseSOLARPOWER])
    assert code == 0


def test_04_Montreal(repair_return_test, data_04_Montreal):
    code, error = repair_return_test([data_04_Montreal])
    assert code == 0


def test_04_Montreal_CFD(repair_return_test, data_04_Montreal, UseCaseCFD):
    code, error = repair_return_test([data_04_Montreal, UseCaseCFD])
    assert code == 0


def test_04_Montreal_ED(repair_return_test, data_04_Montreal, UseCaseENERGYDEMAND):
    code, error = repair_return_test([data_04_Montreal, UseCaseENERGYDEMAND])
    assert code == 0


def test_04_Montreal_VIZ(repair_return_test, data_04_Montreal, UseCaseVISUALIZATION):
    code, error = repair_return_test([data_04_Montreal, UseCaseVISUALIZATION])
    assert code == 0


def test_04_Montreal_SP(repair_return_test, data_04_Montreal, UseCaseSOLARPOWER):
    code, error = repair_return_test([data_04_Montreal, UseCaseSOLARPOWER])
    assert code == 0


def test_05_NewYork(repair_return_test, data_05_NewYork):
    code, error = repair_return_test([data_05_NewYork])
    assert code == 0


def test_05_NewYork_CFD(repair_return_test, data_05_NewYork, UseCaseCFD):
    code, error = repair_return_test([data_05_NewYork, UseCaseCFD])
    assert code == 0


def test_05_NewYork_ED(repair_return_test, data_05_NewYork, UseCaseENERGYDEMAND):
    code, error = repair_return_test([data_05_NewYork, UseCaseENERGYDEMAND])
    assert code == 0


def test_05_NewYork_VIZ(repair_return_test, data_05_NewYork, UseCaseVISUALIZATION):
    code, error = repair_return_test([data_05_NewYork, UseCaseVISUALIZATION])
    assert code == 0


def test_05_NewYork_SP(repair_return_test, data_05_NewYork, UseCaseSOLARPOWER):
    code, error = repair_return_test([data_05_NewYork, UseCaseSOLARPOWER])
    assert code == 0


def test_06_Railway(repair_return_test, data_06_Railway):
    code, error = repair_return_test([data_06_Railway])
    assert code == 0


def test_06_Railway_CFD(repair_return_test, data_06_Railway, UseCaseCFD):
    code, error = repair_return_test([data_06_Railway, UseCaseCFD])
    assert code == 0


def test_06_Railway_ED(repair_return_test, data_06_Railway, UseCaseENERGYDEMAND):
    code, error = repair_return_test([data_06_Railway, UseCaseENERGYDEMAND])
    assert code == 0


def test_06_Railway_VIZ(repair_return_test, data_06_Railway, UseCaseVISUALIZATION):
    code, error = repair_return_test([data_06_Railway, UseCaseVISUALIZATION])
    assert code == 0


def test_06_Railway_SP(repair_return_test, data_06_Railway, UseCaseSOLARPOWER):
    code, error = repair_return_test([data_06_Railway, UseCaseSOLARPOWER])
    assert code == 0


def test_07_Rotterdam(repair_return_test, data_07_Rotterdam):
    code, error = repair_return_test([data_07_Rotterdam])
    assert code == 0


def test_07_Rotterdam_CFD(repair_return_test, data_07_Rotterdam, UseCaseCFD):
    code, error = repair_return_test([data_07_Rotterdam, UseCaseCFD])
    assert code == 0


def test_07_Rotterdam_ED(repair_return_test, data_07_Rotterdam, UseCaseENERGYDEMAND):
    code, error = repair_return_test([data_07_Rotterdam, UseCaseENERGYDEMAND])
    assert code == 0


def test_07_Rotterdam_VIZ(repair_return_test, data_07_Rotterdam, UseCaseVISUALIZATION):
    code, error = repair_return_test([data_07_Rotterdam, UseCaseVISUALIZATION])
    assert code == 0


def test_07_Rotterdam_SP(repair_return_test, data_07_Rotterdam, UseCaseSOLARPOWER):
    code, error = repair_return_test([data_07_Rotterdam, UseCaseSOLARPOWER])
    assert code == 0


def test_08_Vienna(repair_return_test, data_08_Vienna):
    code, error = repair_return_test([data_08_Vienna])
    assert code == 0


def test_08_Vienna_CFD(repair_return_test, data_08_Vienna, UseCaseCFD):
    code, error = repair_return_test([data_08_Vienna, UseCaseCFD])
    assert code == 0


def test_08_Vienna_ED(repair_return_test, data_08_Vienna, UseCaseENERGYDEMAND):
    code, error = repair_return_test([data_08_Vienna, UseCaseENERGYDEMAND])
    assert code == 0


def test_08_Vienna_VIZ(repair_return_test, data_08_Vienna, UseCaseVISUALIZATION):
    code, error = repair_return_test([data_08_Vienna, UseCaseVISUALIZATION])
    assert code == 0


def test_08_Vienna_SP(repair_return_test, data_08_Vienna, UseCaseSOLARPOWER):
    code, error = repair_return_test([data_08_Vienna, UseCaseSOLARPOWER])
    assert code == 0


def test_09_Zurich(repair_return_test, data_09_Zurich):
    code, error = repair_return_test([data_09_Zurich])
    assert code == 0


def test_09_Zurich_CFD(repair_return_test, data_09_Zurich, UseCaseCFD):
    code, error = repair_return_test([data_09_Zurich, UseCaseCFD])
    assert code == 0


def test_09_Zurich_ED(repair_return_test, data_09_Zurich, UseCaseENERGYDEMAND):
    code, error = repair_return_test([data_09_Zurich, UseCaseENERGYDEMAND])
    assert code == 0


def test_09_Zurich_VIZ(repair_return_test, data_09_Zurich, UseCaseVISUALIZATION):
    code, error = repair_return_test([data_09_Zurich, UseCaseVISUALIZATION])
    assert code == 0


def test_09_Zurich_SP(repair_return_test, data_09_Zurich, UseCaseSOLARPOWER):
    code, error = repair_return_test([data_09_Zurich, UseCaseSOLARPOWER])
    assert code == 0
