import pytest
import subprocess
from subprocess import Popen, PIPE, STDOUT
import os.path
import os
import sys
from cjio import cityjson
import json
import shutil

pytest_plugins = [
    "tests.conftest_val3dity",
]


# ---------------------------------------------------------------------------- add option for running the full test set
def pytest_addoption(parser):
    parser.addoption("--runfull", action="store_true",
                     default=False, help="run full set of tests")
    parser.addoption("--runBuild", action="store_true",
                     default=False, help="run also the build test")
    parser.addoption("--keepRepairs", action="store_true",
                     default=False, help="keeps all repaired files and reports")
    parser.addoption("--notFinished", action="store_true",
                     default=False, help="tests for not implemented repairs")
    parser.addoption("--ThesisData", action="store_true",
                     default=False, help="tests for thesis")


def pytest_collection_modifyitems(config, items):
    if not config.getoption("--runfull"):
        skip_full = pytest.mark.skip(reason="need --runfull option to run")
        for item in items:
            if "full" in item.keywords:
                item.add_marker(skip_full)
    if not config.getoption("--runBuild"):
        skip_build = pytest.mark.skip(reason="need --runbuild option to run")
        for item in items:
            if "build" in item.keywords:
                item.add_marker(skip_build)
    if not config.getoption("--ThesisData"):
        skip_thesis = pytest.mark.skip(reason="need --thesisdata option to run ")
        for item in items:
            if "thesis" in item.keywords:
                item.add_marker(skip_thesis)
    if not config.getoption("--notFinished"):
        skip_notfinished = pytest.mark.skip(reason="need --notFinished option to run non implemented repairs")
        for item in items:
            if "NotFinished" in item.keywords:
                item.add_marker(skip_notfinished)
    if config.getoption("--keepRepairs"):
        skip_clean = pytest.mark.skip(reason="files are not cleaned but kept, seeing you used --keepRepairs")
        for item in items:
            if "keep" in item.keywords:
                item.add_marker(skip_clean)
    return


# --------------------------------------------------------------------------------------------------running the process
@pytest.fixture(scope="session", autouse=True)
def cmake_make(dir_src, dir_buildtest):
    options = "-DLIBRARY=true -DCMAKE_BUILD_TYPE=Release"

    make = subprocess.run(["cmake", "-S", dir_src, "-B", dir_buildtest, options], capture_output=True)
    return make.returncode


@pytest.fixture(scope="session", autouse=True)
def cmake_build(dir_buildtest, target):
    optionsbuild = "-j12"  # needs this when running though Ubuntu -->  cause otherwise CPU blows up

    build = subprocess.run(["cmake", "--build", dir_buildtest, "--target", target, optionsbuild],
                           capture_output=True)
    return build.returncode


@pytest.fixture(scope="session")
def AUTOr3pair(dir_buildtest, target):
    run = dir_buildtest + "/" + target
    return run


@pytest.fixture(scope="session")
def target():
    target = "AUTOr3pair"
    return target


@pytest.fixture(scope="session")
def dir_src():
    srcdirectory = "../"
    return srcdirectory


@pytest.fixture(scope="session")
def dir_buildtest():
    buildtestdirectory = "cmake-build-tests"
    return buildtestdirectory


# ------------------------------------------------------------------------------------------------------- Use Cases
@pytest.fixture(scope="session")
def UseCaseVISUALIZATION():
    target = "VISUALIZATION"
    return target

@pytest.fixture(scope="session")
def UseCaseCFD():
    target = "CFD"
    return target

@pytest.fixture(scope="session")
def UseCaseENERGYDEMAND():
    target = "ENERGYDEMAND"
    return target

@pytest.fixture(scope="session")
def UseCaseSOLARPOWER():
    target = "SOLARPOWER"
    return target

# ------------------------------------------------------------------------------------------------------- data & folders

@pytest.fixture(scope="session")
def dir_data():
    directory = "../data"
    return directory


@pytest.fixture(scope="session")
def dir_test_file_formats(dir_data):
    directory = dir_data + "/Test_input_file_formats"
    return directory

@pytest.fixture(scope="session")
def dir_Valid(dir_test_file_formats):
    directory = dir_test_file_formats + "/Valid_no_repair_cube"
    return directory

@pytest.fixture(scope="session")
def dir_Valid_file(dir_test_file_formats):
    directory = dir_test_file_formats + "/Valid_file_formats"
    return directory

@pytest.fixture(scope="session")
def dir_InValid_file(dir_test_file_formats):
    directory = dir_test_file_formats + "/Invalid_file_formats"
    return directory

@pytest.fixture(scope="session")
def dir_Repair(dir_data):
    directory = dir_data + "/Test_geometry_repairs"
    return directory

@pytest.fixture(scope="session")
def dir_Ring(dir_Repair):
    directory = dir_Repair + "/Ring_Level"
    return directory


@pytest.fixture(scope="session")
def dir_Poly(dir_Repair):
    directory = dir_Repair + "/Poly_Level"
    return directory


@pytest.fixture(scope="session")
def dir_Shell(dir_Repair):
    directory = dir_Repair + "/Shell_Level"
    return directory


@pytest.fixture(scope="session")
def dir_Solid(dir_Repair):
    directory = dir_Repair + "/Solid_Level"
    return directory


@pytest.fixture(scope="session")
def dir_SolidInteraction(dir_Repair):
    directory = dir_Repair + "/SolidInteraction_Level"
    return directory

@pytest.fixture(scope="session")
def dir_SubPart(dir_Repair):
    directory = dir_Repair + "/SubPart_Level"
    return directory

@pytest.fixture(scope="session")
def dir_Thesis(dir_data):
    directory = dir_data + "/Test_data_thesis"
    return directory

@pytest.fixture(scope="session")
def dir_Bag(dir_Thesis):
    directory = dir_Thesis + "/BAG_tiles"
    return directory

@pytest.fixture(scope="session")
def dir_CityJSON_website(dir_Thesis):
    directory = dir_Thesis + "/CityJSON_website"
    return directory

@pytest.fixture(scope="session")
def dir_Other(dir_Thesis):
    directory = dir_Thesis + "/Other"
    return directory

@pytest.fixture(scope="session")
def dir_prepost_processing(dir_data):
    directory = dir_data + "/Test_pre_post_processing"
    return directory

@pytest.fixture(scope="session")
def dir_UseCaseParameters(dir_data):
    directory = dir_data + "/Test_use_case_parameters"
    return directory

# -------------------------------------------------------------------------------------------------------------- repairs
@pytest.fixture(scope="session")
def repair_return_test(AUTOr3pair):
    def _repair(input, AUTOr3pair=AUTOr3pair):
        # return returncode, error(s)
        if len(input) == 0:
            run = AUTOr3pair
        else:
            run = [AUTOr3pair]
            for i in input:
                run.append(i)
        proc = subprocess.run(run, capture_output=True, text=True, check=False)
        return proc.returncode, proc.stderr

    return (_repair)


# ----------------------------------------------------------------------------------------------------------- cjio test
@pytest.fixture(scope="session")
def Valid_CityJSON(AUTOr3pair):
    def _cjval(input, AUTOr3pair=AUTOr3pair):
        dir_name = input
        test = os.listdir(dir_name)
        for item in test:
            if "REPAIRED.json" in item:
                # with open(os.path.join(dir_name, item), "r") as f:
                f = open(os.path.join(dir_name, item), "r", encoding="utf")
                cj = json.load(f)
                if cj["type"] == "CityJSON":

                    cargo_bin_path = os.path.join(os.path.expanduser('~'), '.cargo', 'bin')
                    os.environ['PATH'] = f"{cargo_bin_path}:{os.environ['PATH']}"

                    # Find the cjval executable in the system's PATH
                    cjval_path = shutil.which('cjval')
                    if cjval_path is None:
                        raise FileNotFoundError("cjval executable not found in PATH")
                        return 0

                    run = [cjval_path, os.path.join(dir_name, item)]
                    proc = subprocess.run(run, capture_output=True, text=True, check=False)
                    assert "File is valid but has warnings" in proc.stdout or "File is valid" in proc.stdout
                    print(item, "File is valid but has warnings" in proc.stdout or "File is valid" in proc.stdout)
        return 1

    return (_cjval)

# -------------------------------------------------------------------------------------------------- get test files
def dir_With_SM():
    directory = "/With_SemanticsMaterials"
    return directory


def dir_Without_SM():
    directory = "/Without_SemanticsMaterials"
    return directory


def data_from_dir(dir, BoolWith, BoolWithout, errornumber):
    itemsSM = os.listdir(dir + dir_With_SM())
    itemsnotSM = os.listdir(dir + dir_Without_SM())
    test_data = []
    if BoolWith:
        for item in itemsSM:
            if str(errornumber) in item and "_REPAIRED" not in item and "Report" not in item:
                test_data.append(dir_With_SM() + "/" + item)
    if BoolWithout:
        for item in itemsnotSM:
            if str(errornumber) in item and "_REPAIRED" not in item and "Report" not in item:
                test_data.append(dir_Without_SM() + "/" + item)

    return test_data

# -------------------------------------------------------------------------------------------------- get new file names
@pytest.fixture(scope="session")
def outputnames(AUTOr3pair):
    def _outputnames(data, user, lod = ""):
        # Extract the file extension and convert to lowercase
        pos = data.rfind(".") + 1
        extension = data[pos:].lower()

        # Process the filename part (without extension)
        outputCombi = data[:pos - 1]

        # Process the filename part (without extension)
        outputCombi = data[:pos - 1]
        city_suffix = ".city"
        city_pos = outputCombi.rfind(city_suffix)

        if city_pos != -1:
            outputCombi = outputCombi[:city_pos] + outputCombi[city_pos + len(city_suffix):]

        # If user input filename is provided outputCombi
        if user != "":
            outputCombi += "__"
            pos_i = user.rfind(".")
            pos_slash = user.rfind("/")

            if pos_i != -1 and pos_slash != -1:
                outputCombi += user[pos_slash + 1:pos_i]
            elif pos_i != -1:
                outputCombi += user[:pos_i]
            elif pos_slash != -1:
                outputCombi += user[pos_slash + 1:]
            else:
                outputCombi += user

        if lod != "":
            outputCombi += "__" + lod

        # Construct the Outname and Reportname based on the filename and file extension
        if pos != 0 and data.find("/") != pos:
            report_name = f"{outputCombi}__RepairReport.json"
            out_name = f"{outputCombi}__REPAIRED.{extension}"
        else:
            report_name = f"{data}/directory_RepairReport.json"
            out_name = f"{outputCombi}__REPAIRED.{extension}"

        return out_name, report_name
    return (_outputnames)

# ------------------------------------------------------------------------------------------- Get faces from geometries
@pytest.fixture(scope="session")
def extract_faces_from_geometry():
    def _extract(geometry):
        faces = []
        geometry_type = geometry.get("type")

        if geometry_type in ["MultiSurface", "CompositeSurface"]:
            for surface in geometry["boundaries"]:
                faces.append(surface)

        elif geometry_type == "Solid":
            for shell in geometry["boundaries"]:
                for surface in shell:
                    faces.append(surface)

        elif geometry_type in ["MultiSolid", "CompositeSolid"]:
            for solid in geometry["boundaries"]:
                for shell in solid:
                    for surface in shell:
                        faces.append(surface)

        return faces
    return _extract

@pytest.fixture(scope="session")
def extract_faces_from_cityjson(extract_faces_from_geometry):
    def _extract_faces(cityjson_data):
        faces_per_object = {}

        for obj_id, city_object in cityjson_data["CityObjects"].items():
            if "geometry" in city_object:
                faces_per_object[obj_id] = []
                for geometry in city_object["geometry"]:
                    faces = extract_faces_from_geometry(geometry)
                    faces_per_object[obj_id].extend(faces)

        return faces_per_object
    return _extract_faces

# -------------------------------------------------------------------------------------------------- check watertight
@pytest.fixture(scope="session")
def check_watertight(AUTOr3pair):
    def _check(cityjson):
        for obj_id, obj_data in cityjson.get('CityObjects', {}).items():
            for geometry in obj_data.get('geometry', []):
                geom_type = geometry.get('type')
                # Ensure that the geometry type is not MultiSurface or CompositeSurface
                if geom_type in ['MultiSurface', 'CompositeSurface', "MultiSolid"]:
                    return False;
        return True
    return _check

# -------------------------------------------------------------------------------------------------- check Orientation
@pytest.fixture(scope="session")
def check_orientation(AUTOr3pair):
    def _check(cityjson):
        for obj_id, obj_data in cityjson.get('CityObjects', {}).items():
            for geometry in obj_data.get('geometry', []):
                geom_type = geometry.get('type')
                # Ensure that the geometry type is not MultiSurface or CompositeSurface
                if geom_type in ['MultiSurface']:
                    return False;
        return True
    return _check

# -------------------------------------------------------------------------------------------------- clean up afterward
@pytest.fixture(scope="session")
def cleanup(AUTOr3pair):
    def _clean(input):
        dir_name = input
        test = os.listdir(dir_name)
        for item in test:
            if item.endswith("_RepairReport.json"):
                os.remove(os.path.join(dir_name, item))
            if "REPAIRED" in item:
                os.remove(os.path.join(dir_name, item))
        return 1

    return (_clean)


@pytest.fixture(scope="session")
def cleanupRepair(AUTOr3pair):
    def _cleanR(input):
        dir_name = input + dir_With_SM()
        test = os.listdir(dir_name)
        for item in test:
            if item.endswith("_RepairReport.json"):
                os.remove(os.path.join(dir_name, item))
            if "REPAIRED" in item:
                os.remove(os.path.join(dir_name, item))
        dir_name = input + dir_Without_SM()
        test = os.listdir(dir_name)
        for item in test:
            if item.endswith("_RepairReport.json"):
                os.remove(os.path.join(dir_name, item))
            if "REPAIRED" in item:
                os.remove(os.path.join(dir_name, item))
        return 1

    return (_cleanR)



