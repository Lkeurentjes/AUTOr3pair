#  Copyright (c) 2024. // made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes
import pytest

@pytest.fixture(scope="module",
                params=["USERINPUT_BAG.json"])
def USERINPUT_BAG(request, dir_Bag):
    path = dir_Bag + "/" + request.param
    return path

@pytest.fixture(scope="module",
                params=[
                    "9-324-628.city.json",
                        "10-316-624.city.json",
                        "10-316-626.city.json",
                        "10-316-628.city.json",
                        "10-316-630.city.json",
                        "10-318-624.city.json",
                        "10-318-626.city.json",
                        "10-318-628.city.json",
                        "10-318-630.city.json",
                        "10-320-624.city.json",
                        "10-320-626.city.json",
                        "10-320-628.city.json",
                        "10-320-630.city.json",
                        "10-322-624.city.json",
                        "10-322-626.city.json",
                        "10-322-628.city.json",
                        "10-322-630.city.json",
                        "10-324-624.city.json",
                        "10-324-626.city.json",
                        "10-326-624.city.json",
                        "10-326-626.city.json"])
def data_BAG(request, dir_Bag, USERINPUT_BAG):
    path = dir_Bag + "/" + request.param
    return (path, USERINPUT_BAG)



# def test_BAG(repair_return_test, data_BAG):
#     data, user = data_BAG
#     code, error = repair_return_test([data, user])
#     assert code == 0