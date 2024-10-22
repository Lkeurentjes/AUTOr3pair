# Methodology for Automatic Repair of Semantics 3D City Models
The workflow for the proposed repair methodology is outlined in the figure below. The automatic repair process is done through a repair loop. This chapter focuses on geometric repairs and the use of Val3dity for validating and identifying errors. The repairs are organized per primitive level, as outlined in sections below. Each approach preserves semantics and materials where possible.

![Methodology Flowchart](_images/Methodology.svg)

## Validation by Val3dity
Val3dity validates geometries and reports errors at three levels:
1. **File errors** (9xx) – Out of scope for this thesis.
2. **Feature errors** (6xx, 7xx) – These refer to city model objects like buildings.
3. **Geometry errors** (1xx-5xx) – These errors relate to geometric primitives.

A Val3dity report can be used to trigger repairs for invalid locations. Since Val3dity works hierarchically, repairs are applied per level, assuming valid lower-level primitives.

![Val3dity Report](_images/Diagrams/Val3dityReport.svg)

Val3dity uses tolerances for handling small errors. Three situations where tolerance is applied include:
- **Planarity of polygons** (default tolerance is 1mm for vertices and 20° for normals).
- **Snapping close vertices** (default tolerance 1mm).
- **Distance between primitives** (used for ensuring no overlaps or disjoint parts).

### Figure: Example of tolerance applied
![Tolerance Example](_images/tollerance.svg)

## Ring-Level Repair Approaches
Geometries should meet specific standards at the ring level. Below are repair strategies for common errors detected by Val3dity:

### 101 - Too Few Points
When a ring has only one line (a curve with two points), deletion is the only option.
![Too Few Points](_images/Repairs/Repair101.svg)

### 102 - Consecutive Points Same
If two consecutive points are the same, one is kept. This repair ensures that the ring doesn't become invalid afterward.
![Consecutive Points Same](_images/Repairs/Repair102.svg)

### 103 - Ring Not Closed
Since this error is not applicable to CityJSON or OBJ formats, it is out of scope. However, it may be addressed in GML or JSON-FG formats by closing the ring.

### 104 - Ring Self-Intersection
When a ring self-intersects, the repair involves deleting or applying a convex hull to restore a valid structure.
![Self-Intersection](_images/Repairs/Repair104.svg)

## Polygon-Level Repair Approaches
Polygons are simple, planar surfaces that must adhere to certain rules:

### 201 - Intersecting Rings
Boolean operations are used to repair intersections between rings in a polygon.
![Intersecting Rings](_images/Repairs/Repair201_exterior.svg)

### 202 - Duplicate Rings
Preferably, only the first of the duplicate rings is kept.
![Duplicate Rings](_images/Repairs/Repair202.svg)

### 203 - Non-Planar Polygon (Distance to Plane)
This repair either triangulates the polygon or projects outlier points onto a new plane.
![Non-Planar Polygon](_images/Repairs/Repair203.svg)

### 204 - Non-Planar Polygon (Normal Deviation)
When a "fold" exists in the polygon, the affected triangles are projected onto a new plane.
![Non-Planar Deviation](_images/Repairs/Repair204.svg)

### 205 - Polygon Interior Disconnected
If the polygon's interior is disconnected, it is split into multiple polygons.
![Interior Disconnected](_images/Repairs/Repair205.svg)

### 206 - Inner Ring Outside
If an inner ring is outside the exterior ring, it is either deleted or converted into a new polygon.
![Inner Ring Outside](_images/Repairs/Repair206.svg)

### 207 - Inner Rings Nested
Nested inner rings are either deleted or reversed and added as new polygons.
![Nested Rings](_images/Repairs/Repair307.svg)

### 208 - Orientation Rings Same
The orientation of the exterior and interior rings must be opposite. Inner rings are reversed if needed.
![Orientation Rings Same](_images/Repairs/Repair208.svg)

## Shell-Level Repair Approaches
Shells are defined by closed polygon sets and must meet specific validity standards.

### 300 - Not Valid 2-Manifold
If no local repair is possible, a global method like alpha wrap is applied to generate a valid triangulation.
![Not Valid 2-Manifold](_images/Repairs/Repair300.svg)

### 301 - Too Few Polygons
A shell must have at least four polygons. Local methods like hole filling are attempted first, followed by alpha wrapping if needed.
![Too Few Polygons](_images/Repairs/Repair301.svg)

### 302 - Shell Not Closed
Holes and gaps are reconstructed using naked edge detection or alpha wrapping.
![Shell Not Closed](_images/Repairs/Repair302.svg)

### 303 - Non-Manifold Case
In this case, connected components are split along overused edges or vertices.
![Non-Manifold Case](_images/Repairs/Repair303.svg)

### 305 - Multiple Connected Components
Disconnected parts are either deleted or converted into MultiSurface or MultiSolid, based on user requirements.
![Multiple Components](_images/Repairs/Repair305.svg)

### 306 - Shell Self-Intersection
Intersecting faces are deleted, and holes are patched. Alpha wrapping is used as a fallback.
![Self-Intersection](_images/Repairs/Repair306.svg)

### 307 - Polygon Wrong Orientation
Wrongly oriented polygons are flipped to restore a valid shell.
![Wrong Orientation](_images/Repairs/Repair307.svg)

## Solid-Level Repair Approaches
Solids are defined by one or more shells, and common errors include:

### 401 - Intersecting Shells
Boolean operations are used to resolve shell intersections.
![Intersecting Shells](_images/Repairs/Repair401_exterior.svg)

### 402 - Duplicate Shells
Duplicate shells are removed, preserving the first one.
![Duplicate Shells](_images/Repairs/Repair402.svg)

### 403 - Inner Shell Outside
Inner shells that lie outside the exterior are either deleted or converted into new solids.
![Inner Shell Outside](_images/Repairs/Repair403.svg)

### 404 - Solid Interior Disconnected
Disconnected interiors are split into multiple solids.
![Solid Interior Disconnected](_images/Repairs/Repair404.svg)

### 405 - Wrong Orientation Shell
If shell polygons are wrongly oriented, the shell is flipped to restore validity.
![Wrong Orientation Shell](_images/Repairs/Repair405.svg)

## Solid Interaction Level Repair Approaches
Composite solids are defined by two or more solids that must be connected but not overlapping.

### 501 - Intersection Solids
Boolean operations handle overlapping solids.
![Intersection Solids](_images/Repairs/Repair501.svg)

### 502 - Duplicate Solids
Duplicate solids are removed.
![Duplicate Solids](_images/Repairs/Repair502.svg)

### 503 - Disconnected Solids
Disconnected solids are either added as separate entities or converted into MultiSolid geometries.
![Disconnected Solids](_images/Repairs/Repair503.svg)

## Building Part-Level Repair Approaches
BuildingParts must not overlap, and Val3dity can validate them based on overlap tolerance.

### 601 - BuildingParts Overlap
Overlapping BuildingParts are repaired using Boolean operations, with semantics and materials preserved.
![Building Parts Overlap](_images/Repairs/Repair601.svg)

## Global Approach
If the local repair methods fail to resolve issues, a global repair approach is triggered, which consists of four stages:
1. Alpha wrap on polygons
2. Alpha wrap on vertices
3. Convex hull of geometry
4. Oriented bounding box

![Global Repair](_images/Repairs/RepairGlobal.svg)
