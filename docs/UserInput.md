## Parameters
Parameters can influence the repair process. The parameters can be changed by users by inputting a use case  or by inputting an input `JSON`. The parameters can be divided into six categories: input parameters, output parameters, repair depths, geometry repairs, val3dity tolerances, and use case parameters. The first five can only be changed by using an input `JSON`, while the use case parameters are grouped by their use case.

**Input parameters** influence how the 3D city models are handled during the repair process. The parameters are:

- `OBJ_geomtype`: `"Solid"`, which decides what kind of geometric primitive the geometries in an OBJ file are. Alternately, users can change the value to other geometric primitives (MultiSurface, CompositeSurface, MultiSolid, CompositeSolid).
- `ExtendScope`: `[]` (empty list), can add other types of CityObjects to repair. For this thesis, only building and its subparts are in the scope, but users can add other types by listing them in this parameter.

**Output parameters** influence what the terminal shows during the repair process, and for CityJSON if extra attributes are added to explain the repair process. The parameters are:

- `ShowProgress`: `TRUE`, when set to true, the terminal shows which items it has validated and which repairs are done so that the user can follow the progress of AUTOr3pair.
- `Debugging`: `FALSE`, when set to true, the terminal shows more extensive output, such as the vertices list. This can be used when debugging the program or when a user wants to look further into specific steps.
- `AddAttribute`: `FALSE`, when true, CityJSON CityObjects get an extra attribute with what is repaired. This makes the file larger (primarily for large 3D city models). This is only done when the user changes the parameter. The standard attribute to the file indicates that it is repaired and when it will always be added.

**Repair Depth(s)** stop the repair process when their threshold is reached. The parameters are:

- `maxRepairDepth`: 50, is the maximum number of times the same category repair can be done on a CityObject before performing the global approach, alpha wrap. Users can change the value according to their time limitations.
- `TotalRepairDepth`: 500, is the maximum number of times the same CityObject is repaired before doing the global approach, alpha wrap. Users can set the value lower if they want the repair process to be faster and higher if they want to give the repair process more tries. However, manual testing did not yield better or different results with a higher value; when 500 is reached, it is mostly due to repair bugs which cannot be solved by trying it more often.

**Geometry repair standards** can help users exclude certain errors from the repair process. The parameters are:

- `solve_all`: `TRUE`, when set to true, all errors found by val3dity will be repaired.
- `errors_to_solve`: `[]` (empty list), when users want to exclude errors, they can add them to the list, and repairs for those errors will not be done. Due to how val3dity is implemented, higher-order errors will also not be repaired, as those are not checked. When users add errors to this parameter, `solve_all` will be changed to False automatically.

**val3dity tolerances** are the tolerances of val3dity as explained in [val3dity](#val3dity). The parameters use the same standards as val3dity and are:

- `overlap_tol`: -1, which is the tolerance for testing the overlap between primitives in CompositeSolids and BuildingParts. The maximum allowed distance for overlaps. It helps to validate the topological relationship between Solids forming a CompositeSolid.
- `planarity_d2p_tol`: 0.01, which is the tolerance for planarity based on a distance to a plane. The distance between every point forming a surface and a plane must be less than the threshold.
- `planarity_n_tol`: 20, which is the planarity tolerance based on normal deviation. It helps to detect small folds in a surface. The threshold refers to the normal of each triangle after the surface has been triangulated.
- `snap_tol`: 0.001, which is the tolerance for how close vertices can be together before they are snapped into the same point.

### Use Case Parameters
Seeing some use cases have additional requirements for a 3D city model to be valid. Therefore, a user can also add additional requirements to the repair process. Users can give one of the four use cases. Then, a standard set of the parameters is used, shown in Table 1. How they influence the repair process is per use case discussed below. Users can also make their own standard set using an input `JSON` to change the combination of parameters to their use case.

|                     | **Default AUTOr3pair** | **CFD** | **Energy Demand** | **Visualization** | **Solar Power Estimation** |
|---------------------|------------------------|---------|-------------------|-------------------|---------------------------|
| `KeepEverything`    | 🟥 FALSE               | 🟥 FALSE| 🟥 FALSE          | ✅ TRUE           | ✅ TRUE                    |
| `SkipLowRepairs`    | 🟥 FALSE               | ✅ TRUE | ✅ TRUE           | 🟥 FALSE          | 🟥 FALSE                   |
| `Watertight`        | 🟥 FALSE               | ✅ TRUE | ✅ TRUE           | 🟥 FALSE          | 🟥 FALSE                   |
| `Orientation`       | 🟥 FALSE               | 🟥 FALSE| 🟥 FALSE          | ✅ TRUE           | ✅ TRUE                    |
| `MergeTol`          | ↔️ 0.1                  | ↔️ 0.25 | ↔️ 0.75           | ↔️ 0.1             | ↔️ 0.5                     |
| `Overlap`           | ✅ TRUE                | 🟥 FALSE| 🟥 FALSE          | 🟥 FALSE          | 🟥 FALSE                   |
| `SemanticsAdd`      | ✅ TRUE                | 🟥 FALSE| ✅ TRUE           | 🟥 FALSE          | ✅ TRUE                    |
| `SemanticsValidate` | ✅ TRUE                | 🟥 FALSE| 🟥 FALSE         | 🟥 FALSE          | ✅ TRUE                    |
| `Triangulate`       | 🟥 FALSE               | ✅ TRUE | 🟥 FALSE          | 🟥 FALSE          | 🟥 FALSE                   |
| `Simplification`    | 🟥 FALSE               | ✅ TRUE | 🟥 FALSE          | 🟥 FALSE          | 🟥 FALSE                   |
| `RemeshSlivers`     | 🟥 FALSE               | ✅ TRUE | 🟥 FALSE          | 🟥 FALSE          | 🟥 FALSE                   |


**Use case: CFD** requires that geometries are watertight and non-intersecting. Therefore, `Watertight` is set to true, which converts all geometries to Solids or CompositeSolids, and `Overlap` is set to false, which validates and repairs overlap between different CityObjects. Seeing small details is not very important, `SkipLowRepairs` is true, which deletes faces with Ring or Polygon errors and therefore starts repairing on shell level. Since for simulations it does not matter if objects are merged or split, the `MergeTol` is set to 25%, so geometries will be merged together if their overlapping volume is 25% of the smallest volume. Additionally, CFD needs triangulated objects (`Triangulate`), with no small features, sharp edges, and sliver triangles. `Simplification` and `RemeshSlivers` are therefore set to true.

**Use case: Energy demand** requires buildings to be watertight for accurate volume calculations, so `Watertight` is set to true, converting all geometries to Solids or CompositeSolids. Objects also must be non-intersecting; therefore, `Overlap` is false. Additionally, external surfaces should be subdivided into semantic parts (roof, wall, and ground surfaces) for more precise energy demand estimation, so `SemanticsAdd` is set to true to add (missing) semantics. However, existing semantics are not validated and repaired(`SemanticsValidate`); seeing windows and doors can affect the energy calculation.  Lastly, since energy demand is often calculated per building, `MergeTol` is adjusted to 75%, which makes the chance of buildings merging small, and since small details are not important, `SkipLowRepairs` is set to true, which deletes faces with Ring or Polygon errors and therefore starts repairing on shell level.

**Use case: Visualization** requires that building geometries have correctly oriented surfaces to avoid rendering errors, so `Orientation` is set to true, converting MultiSurfaces to CompositeSurfaces also to have orientation checked on the shell. To prevent rendering issues caused by overlapping surfaces, `Overlap` is set to false, ensuring that intersections between buildings are validated and repaired. `MergeTol` is adjusted to 10% to avoid significant changes from merging buildings. Lastly, local repairs are preferred to maintain a higher level of detail, so `KeepEverything` is set to true to keep all (dangling) parts in geometries, and `SkipLowRepairs` is set to false.

**Use case: Estimation of solar power** requires accurately oriented roof surfaces to estimate solar irradiation correctly, so `Orientation` and `SemanticsAdd` and `SemanticsValidate` are set to true to validate and repair roof face orientation and semantic values. To ensure shadow accuracy, detailed local repairs are preferred over global adjustments, so `KeepEverything` is set to true, and `SkipLowRepairs` is set to false. Additionally, to avoid intersecting roofs, `Overlap` is set to false. Lastly, since solar power potential is sometimes calculated per building, `MergeTol` is adjusted to 50%.
