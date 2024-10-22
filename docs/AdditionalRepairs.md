# Additional Use Case Repairs in AUTOr3pair

## Overview

AUTOr3pair is a framework designed to automatically repair 3D city models for various use cases. These repairs are tailored to different applications based on the requirements of each use case. The following describes the additional repairs for specific use cases and their implementation within AUTOr3pair.

## Use Case-Specific Repairs

Each use case has unique requirements. In this section, we detail how repairs are applied to address the specific needs of four primary use cases: Computational Fluid Dynamics (CFD), Energy Demand, Visualization, and Solar Irradiation.

### 1. **Computational Fluid Dynamics (CFD)**

#### Repair Requirements
- **Watertight Solids**: Ensure all geometries are closed solids to avoid leaks in simulations.
- **Non-Intersecting Geometry**: Prevent intersecting geometries to ensure simulation accuracy.
- **Triangulation**: Objects are triangulated for better handling in CFD simulations.

#### Implementation
AUTOr3pair addresses these requirements by setting `Watertight` to `true`, converting MultiSurfaces into Solids, and ensuring `Overlap` is set to `false` to prevent intersections. Triangulation is applied by enabling `Triangulate`, and small errors are ignored using `SkipLowRepairs`.

```
[location /] AUTOr3pair 3DCityModel_file_to_repair.json CFD
```
## 2. Energy Demand

### Repair Requirements
- **Volume Accuracy**: Buildings must be watertight for precise volume calculation.
- **Semantic Division**: Buildings are divided into roof, wall, and ground surfaces for more accurate energy analysis.

### Implementation
The parameters for this use case include `Watertight = true` for closed volumes and `SemanticsAdd = true` to ensure that external surfaces are correctly labeled. Errors like ring or polygon errors are skipped using `SkipLowRepairs`.

```
[location /] AUTOr3pair 3DCityModel_file_to_repair.json ENERGYDEMAND
```
## 3. Visualization

### Repair Requirements
- **Detail Preservation**: Local repairs to maintain the original geometry as closely as possible.
- **Semantic Validation**: Ensure that visual attributes are correct for proper rendering.

### Implementation
For visualization purposes, `KeepEverything = true` is set to retain geometric detail. Local repairs are preferred, and global fixes are avoided unless necessary.

```
[location /] AUTOr3pair 3DCityModel_file_to_repair.json VISUALIZATION
```
## 4. Solar Irradiation Estimation

### Repair Requirements
- **Roof Surface Orientation**: Correctly orient roof surfaces to ensure accurate solar irradiation calculations.
- **Shadow Calculation Accuracy**: Retain sufficient detail to compute shadows effectively.

### Implementation
For this use case, the parameters include `Orientation = true` to ensure roofs are properly aligned for solar panel calculations, and `SemanticsValidate` is used to check that roof surfaces are correctly labeled. Shadow accuracy is also a priority, requiring minimal modifications to original geometry.

```
[location /] AUTOr3pair 3DCityModel_file_to_repair.json SOLARPOWER
```

# Additional Repairs implementation

## Validate and Repair Semantics
Each surface's normal vector is compared with the Z-axis to determine its classification as a ground, roof, or wall surface. If no semantics exist, a new surface is created, and the `SemanticsAdd` or `SemanticsValidation` options ensure correct assignment.

## Watertight and Orientation Repairs
MultiSurface and CompositeSurface geometries are converted into solids by ensuring boundaries form closed volumes.

## Repair Intersections Between Geometries
Intersections are repaired using **CGAL's AABB tree**, which improves the efficiency of identifying and correcting intersecting objects.

## Mesh Simplification and Smoothing
Meshes are simplified by collapsing edges and removing degenerate faces. **CGAL’s Polygon Mesh Processing** simplifies geometry without compromising detail.
