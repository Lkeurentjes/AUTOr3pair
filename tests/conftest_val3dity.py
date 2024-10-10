#  Copyright (c) 2023. made for a proof of concept for theis Automatic repair of 3d citymodels by Lisa Keurentjes
import pytest
import subprocess

# --------------------------------------------------------------------------------------------------------------- Build
@pytest.fixture(scope="session", autouse=True)
def cmake_make_val3dity(dir_src_val3dity, dir_buildtestVal3dity):
    make = subprocess.run(["cmake", "-S", dir_src_val3dity, "-B", dir_buildtestVal3dity], capture_output=True)
    return make.returncode


@pytest.fixture(scope="session", autouse=True)
def cmake_build_val3dity(dir_buildtestVal3dity, targetval3dity):
    optionsbuild = "-j18"  # needs this when running through Ubuntu -->  cause otherwise CPU blows up

    build = subprocess.run(["cmake", "--build", dir_buildtestVal3dity, "--target", targetval3dity, optionsbuild],
                           capture_output=True)
    return build.returncode


@pytest.fixture(scope="session")
def val3dity(dir_buildtestVal3dity, targetval3dity):
    run = dir_buildtestVal3dity + "/" + targetval3dity
    return run


@pytest.fixture(scope="session")
def dir_buildtestVal3dity():
    buildtestval3ditydirectory = "cmake-build-tests-val3dity"
    return buildtestval3ditydirectory


@pytest.fixture(scope="session")
def targetval3dity():
    target = "val3dity"
    return target


@pytest.fixture(scope="session")
def dir_src_val3dity():
    srcdirectory = "../../val3dity"
    return srcdirectory


# -------------------------------------------------------------------------------------------------------------- Options
@pytest.fixture(scope="session")
def solid():
    """val3dity options for validating a Solid"""
    return (["--unittests", "-p Solid",  "--overlap_tol 0.001"])


@pytest.fixture(scope="session")
def compositesurface():
    """val3dity options for validating a CompositeSurface"""
    return (["--unittests", "-p CompositeSurface",  "--overlap_tol 0.001"])


@pytest.fixture(scope="session")
def multisurface():
    """val3dity options for validating a MultiSurface"""
    return (["--unittests", "-p MultiSurface",  "--overlap_tol 0.001"])


@pytest.fixture(scope="session")
def unittests():
    """val3dity options for validating a file"""
    return (["--unittests",  "--overlap_tol 0.001"])


# -------------------------------------------------------- validation functions
@pytest.fixture(scope="session")
def validate(val3dity):
    def _validate(file_path, options=unittests, val3dity=val3dity):
        """Validate a file

        :return: The error numbers. Empty list if the file is valid.
        :rtype: list
        """
        try:
            assert isinstance(file_path, str)
            file_path = [file_path]
        except AssertionError:
            assert isinstance(file_path, list)
        except Exception:
            return (["Something went really wrong. Validate the file separately with val3dity."])
        # assert isinstance(options, list)

        command = [val3dity] + options + file_path

        try:
            proc = subprocess.run(command,
                                  stdout=subprocess.PIPE,
                                  stderr=subprocess.PIPE,
                                  universal_newlines=True,
                                  timeout=15)
        except subprocess.TimeoutExpired:
            return ([" ".join(["Something went really wrong.",
                               "Validate the file separately with val3dity.",
                               "Or set a higher timeout in conftest.py."])])

        out = proc.stdout
        err = proc.stderr
        rcode = proc.returncode

        if rcode < 0:
            # For example Segmentation fault
            return (["CRASH"])
        else:
            output = []
            if (out != ''):
                if (out.find('@VALID') != -1):
                    return (output)
                elif (out.find('@INVALID') != -1):
                    i = out.find('@INVALID')
                    s = out[i + 9:]
                    tmp = s.split(" ")
                    codes = map(int, tmp[:-1])
                    for each in codes:
                        if output.count(each) == 0:
                            output.append(each)
                    return (output)
                else:
                    return ([out.replace('\n', ' ')])
            else:
                return (output)

    return (_validate)


@pytest.fixture(scope="session")
def validate_full():
    """Validate a file and return the full stdout, stderr

    :rtype: list of strings
    """

    def _validate(command):
        try:
            proc = subprocess.run(command,
                                  stdout=subprocess.PIPE,
                                  stderr=subprocess.PIPE,
                                  universal_newlines=True,
                                  timeout=6)
        except subprocess.TimeoutExpired:
            return ([" ".join(["Something went really wrong.",
                               "Validate the file separately with val3dity.",
                               "Or set a higher timeout in conftest.py."])])
        out = proc.stdout
        err = proc.stderr
        rcode = proc.returncode
        if rcode < 0:
            # For example Segmentation fault
            return (["Something went really wrong. Validate the file separately with val3dity."])
        else:
            return ([out, err])

    return (_validate)
