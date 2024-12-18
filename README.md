# Automatic repair of 3D (city) models: AUTOr3pair

_Version 1.0 of proof of concept for my
finished [thesis ""An automatic geometry repair framework for semantic 3D city models"](https://repository.tudelft.nl/record/uuid:b5de420d-e0a2-4265-8bac-e7ae12f75e8c),
from now on it will be more of a hobby project_

<p align="center">
  <img src="docs/_images/logo_black.png" alt="logo AUTOr3pair"/>
</p>


A significant amount of 3D city models is not considered valid to the standards needed,
they contain geometric as well as topological errors. Some examples of these errors are:

* duplicate vertices
* missing surfaces (a)
* non-watertight solids (b)
* intersecting volumes (c)

<p align="center">
  <img src="docs/_images/Errors.png" alt="example errors"/>
</p>


Errors hinder the further analyzing or processing of these models, so pre-processing of the models needs to be done.
Since manual repair of 3D City models is very time-consuming and prone to errors,
automatic repair methods are highly desirable.

AUTOr3pair is an automatic repair method written as proof of concept of my
thesis ([A framework for automatic repair of 3D city
models](https://www.overleaf.com/project/62e7956cf561ac2c8ea86e7c), todo later add repository link).
In short, it verifies whether a 3D primitive respects the definition as given
in [ISO19107](http://www.iso.org/iso/catalogue_detail.htm?csnumber=26012),
with the help of [Val3dity](https://github.com/tudelft3d/val3dity/). If the primitive is not valid a repair will be
done.
The chosen repair will be based on the use-case (give by the user). For some use cases there are additional
requirements,
for example Computational fluid dynamics (CFD) and Energy Demand Analysis need all buildings to be watertight solids,
while the data could consist of Multi-surface primitives. Such additional requirements will also be tested and repaired.

The automatic repair of the following 3D primitives is fully supported:

- ``MultiSurface``
- ``CompositeSurface``
- ``Solid``
- ``MultiSolid``
- ``CompositeSolid``

Inner rings in polygons/surfaces are supported and so are cavities in solids (also called voids or inner shells).
Spheres and line strings are not supported.

## Usage

AUTOr3pair is a C++ program designed to repair 3D city models and can be executed either through the command line
interface or within an Integrated Development Environment (IDE).

### Building the programm

Before running AUTOr3pair, the program must be built using CMake (use `-DLIBRARY=true` as cmake parameter to link
val3dity correctly). AUTOr3pair depends on CGAL, val3dity, and Nlohmann-json:

- **CGAL** needs to be installed. AUTOr3pair works with version 5.5 (the version where Alpha wrap packages are
  introduced) and higher, with testing done using version 6.0. Using 6.0 is recommended, as it gives fewer segmentation
  errors when using Nef polyhedrons and surface meshes.
- **Val3dity** can be downloaded from their GitHub. AUTOr3pair depends on the "new" report structure, which has been
  implemented since version 2.3.1, but the experiments (see [Results](#results)) have been done with version 2.5.1.
  Val3dity depends on CGAL, Eigen (version used 3.4.0-4), and GEOS (version used 3.11.2).
- **Nlohmann-json** is included in the third-party directory and is on version 3.11.2.

The CMake build system manages the building process across different platforms,
ensuring that the necessary build files are generated.
To build the program, navigate to your chosen build directory and use the following command:

```
cmake --build [/your/build/location] --target AUTOr3pair [optional parameters]
```

- `[/your/build/location]`: Specify the path to your build directory.
- `--target AUTOr3pair`: This ensures that CMake builds the AUTOr3pair target.
- `[optional parameters]`: Additional parameters can be provided to customize the build process. For example, specifying
  the build configuration (`--config Release` or `--config Debug`).

Once built, the AUTOr3pair executable will be located in your build directory, ready to repair 3D city models. ( If you
want to know more on building look at [BuildCmake docs](docs/BuildCmake.md) )

### Running the programm

After successfully building the program, you can use AUTOr3pair to repair 3D city models through the command line.
The program requires one or two inputs depending on the use case. and can optionally have a specif LOD to repair.

```
[/your/build/location/]AUTOr3pair [3d city model to repair] [optional: Use Case (file)] [optional: LOD to repair]
```

**Inputs**:

1. `[3D city model to repair]`: This is the path to the 3D city model file that you want to repair. AUTOr3pair supports
   various file formats (discussed below).
2. `[optional: Use Case (file)]`: This parameter allows you to specify a predefined use case or a custom user preference
   file that contains specific standards for the repair process. If not provided, the program will use default repair
   standards.
3. `[optional: LOD to repair]`: The optional Level of Detail (LOD) parameter can be added to limit the repair process to
   a specific LOD in the model (explained further below).

#### Supported File Formats

AUTOr3pair supports multiple 3D city model file formats. The program can handle these formats:

- **CityJSON**: A JSON-based encoding for 3D city models.
    ```
    [/your/build/location/]AUTOr3pair [your_file_to_repair.json]
    ```
  Example:
    ```
    /build/AUTOr3pair city_model.json
    ```

- **OBJ**: A standard format for representing 3D geometry (e.g., vertices, textures, and more).
    ```
    [/your/build/location/]AUTOr3pair [your_file_to_repair.obj]
    ```
  Example:
    ```
    /build/AUTOr3pair model.obj
    ```

#### Use Cases

If only the input file is provided, the default repair standards are applied.
However, if you have specific requirements, such as those determined by a particular use case, you can provide a use
case or a user preferences file to adjust these standards accordingly.
AUTOr3pair supports various predefined use cases that tailor the repair process for different purposes. These include:

- [Visualisation](#headUC)

```
[/your/build/location/]AUTOr3pair [your_file_to_repair.json] VISUALIZATION
```

- [CFD](#headUC)

```
[/your/build/location/]AUTOr3pair [your_file_to_repair.json] CFD
```

- [Energy Demand](#headUC)

```
[/your/build/location/]AUTOr3pair [your_file_to_repair.json] ENERGYDEMAND
```

- [Solar Power estimation](#headUC)

```
[/your/build/location/]AUTOr3pair [your_file_to_repair.json] SOLARPOWER
```

- [UserInputfile](docs/UserInput.md)

```
[/your/build/location/]AUTOr3pair [your_file_to_repair.json] [your_prefferences.json]
```

This file can be used to customize parameters such as accuracy thresholds, element types, and more.

#### Level of Detail (LOD)

3D city models in JSON can contain various Levels of Detail (LOD).
AUTOr3pair allows you to target specific LODs during the repair process
by adding a `-LOD[number]` argument after the input file.

For example, to repair only LOD 1.0:

```
[/your/build/location/]AUTOr3pair [your_file_to_repair.json] -LOD1.0
```

Combining Use Cases and LOD targeting is also possible:

```
[/your/build/location/]AUTOr3pair [your_file_to_repair.json] SOLARPOWER -LOD1.0
```

Or, if you are using a user-defined preferences file:

```
[/your/build/location/]AUTOr3pair [your_file_to_repair.json] [your_preferences.json] -LOD1.0
```

This flexibility allows you to focus repairs on specific sections of the model based on the LOD, making the repair
process more efficient and suited to your exact needs.

### Standards

For the repair process Standards are used. Below are the Default standards described.
As user you can also change the Standards by inputting a user prefferences `json`,
in [this file](docs/UserInput.md) is explained in more depth where these Standards are used
and how you can make your own input `json`.

The default standards used for the program are:
**Input parameters**:

- `OBJ_geomtype`: `"Solid"`
- `ExtendScope`: `[]`

**Output parameters**:

- `ShowProgress`: `TRUE`
- `Debugging`: `FALSE`
- `AddAttribute`: `FALSE`

**Repair Depth(s)**:

- `maxRepairDepth`: 50
- `TotalRepairDepth`: 500

**Geometry repair standards**:

- `solve_all`: `TRUE`
- `errors_to_solve`: `[]`

**val3dity tolerances**:

- `overlap_tol`: -1
- `planarity_d2p_tol`: 0.01
- `planarity_n_tol`: 20
- `snap_tol`: 0.001

|                     | **Default AUTOr3pair** | **CFD**  | **Energy Demand** | **Visualization** | **Solar Power Estimation** |
|---------------------|------------------------|----------|-------------------|-------------------|----------------------------|
| `KeepEverything`    | 🟥 FALSE               | 🟥 FALSE | 🟥 FALSE          | ✅ TRUE            | ✅ TRUE                     |
| `SkipLowRepairs`    | 🟥 FALSE               | ✅ TRUE   | ✅ TRUE            | 🟥 FALSE          | 🟥 FALSE                   |
| `Watertight`        | 🟥 FALSE               | ✅ TRUE   | ✅ TRUE            | 🟥 FALSE          | 🟥 FALSE                   |
| `Orientation`       | 🟥 FALSE               | 🟥 FALSE | 🟥 FALSE          | ✅ TRUE            | ✅ TRUE                     |
| `MergeTol`          | ↔️ 0.1                 | ↔️ 0.25  | ↔️ 0.75           | ↔️ 0.1            | ↔️ 0.5                     |
| `Overlap`           | ✅ TRUE                 | 🟥 FALSE | 🟥 FALSE          | 🟥 FALSE          | 🟥 FALSE                   |
| `SemanticsAdd`      | ✅ TRUE                 | 🟥 FALSE | ✅ TRUE            | 🟥 FALSE          | ✅ TRUE                     |
| `SemanticsValidate` | ✅ TRUE                 | 🟥 FALSE | 🟥 FALSE          | 🟥 FALSE          | ✅ TRUE                     |
| `Triangulate`       | 🟥 FALSE               | ✅ TRUE   | 🟥 FALSE          | 🟥 FALSE          | 🟥 FALSE                   |
| `Simplification`    | 🟥 FALSE               | ✅ TRUE   | 🟥 FALSE          | 🟥 FALSE          | 🟥 FALSE                   |
| `RemeshSlivers`     | 🟥 FALSE               | ✅ TRUE   | 🟥 FALSE          | 🟥 FALSE          | 🟥 FALSE                   |

The Use case itself are also discussed in [AdditionalRepairs](docs/AdditionalRepairs.md)

## Repair implementation and methodology

The proposed methodology for the automatic repair of semantic 3D city models utilizes a repair loop focusing on
geometric errors identified by [Val3dity](https://github.com/tudelft3d/val3dity/), organized by primitive level to
preserve semantics and materials. Specific
repair strategies are outlined for ring, polygon, shell, solid, and composite solid levels, addressing common errors
such as insufficient points, self-intersections, non-manifold cases, and overlaps. If local repairs are insufficient, a
global approach is employed, encompassing techniques like alpha wrapping, convex hulls, and oriented bounding boxes to
ensure validity and coherence of the geometries. For more information, please refer
to [Errorcodes_vs_Repairs](docs/Errorcodes_vs_Repairs.md)
![allrepairs.svg](docs/_images/Repairs/allrepairs.svg)

## Output

Repaired 3D city models will be saved in their original formats (CityJSON or OBJ), with no output generated if no
repairs are made.
For CityJSON, the output is created by copying the original file while preserving its properties,
replacing the original vertices array with a new one, and adding an **AUTOr3pair** key to track repair details.
The OBJ output is generated using CGAL's I/O stream package,
which writes vertices and polygons from the repaired array and handles boundary conversions.
A repair report will also be generated in JSON format, detailing the repair process, parameters used, errors
encountered,
and providing an overview of repaired features. For more information, please refer to [Output](docs/Output.md).

![RepairReport.svg](docs/_images/RepairReport.svg)

## (Test) data

[Data README](data/README.md)outlines all the test data and various geometrical repair cases for CityJSON files,
detailing specific geometry
errors categorized by ring, polygon, shell, solid, and solid interaction levels. It also describes the user input
optionsthat allow for extending repair scopes and adjusting parameters, such as merge tolerance and semantic
validations. In
addition, preprocessing and postprocessing steps for 3D models are provided, along with supported input formats like
CityJSON and OBJ. The thesis data section summarizes the repair and evaluation of open-data 3D city models, highlighting
the validity percentage and geometric differences before and after repairs. Lastly, the document addresses
non-repairable non-manifolds and includes a demo section with curated files for demonstrations.



