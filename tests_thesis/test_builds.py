#  Copyright (c) 2023. made for a proof of concept for theis Automatic repair of 3d citymodels by Lisa Keurentjes
import pytest


# --------------------------------------------------------------------------------------------------------------- Tests
@pytest.mark.build
def test_make(cmake_make):
    assert cmake_make == 0


@pytest.mark.build
def test_build(cmake_build):
    assert cmake_build == 0


@pytest.mark.build
def test_make_val3dity(cmake_make_val3dity):
    assert cmake_make_val3dity == 0


@pytest.mark.build
def test_build_val3dity(cmake_build_val3dity):
    assert cmake_build_val3dity == 0
