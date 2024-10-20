## Test data
All testdata and describe what it is

### input files

### Geometry repairs
CityJSON geometries can exist as multiple geometry types, the geometry type is described in the file name.

CityJSON files have a version with and without Semantics and materials. 
They can be found in the corresponding directory and are marked with SM if they are present.

OBJ files are made when error is possible.

#### Ring level:
3D city models:
* **101**: Cube top face a line (with only 2 vertices)
* **102**: Cube with one duplicate vertex (repeated in a ring)
* **104**: Unit cube where top face has a bow tie
* **104_1**: Unit cube with top face having a self-intersecting surface (2D invalid), where 2 out of 3 triangles that are forming a square are bundled as one polygon (which self-intersects)
* **104_2**: One face closing in itself before the last vertex
* **104_3**: Similar to 104_1 but triangles meet in one corner of the square
* **104_4**: cube with top face having small fold (Kevin Wiebe's case); vertical shift can't be made (old pfold2.poly)
* **Ring_all**: cube with combination of all ring errors in one cube

User input:
* **Ring**: extend scope with GenericCityObject and focus only on ring repairs
* **Ring_skip**: like ring, but "SkipLowRepairs" is set to true

#### Polygon level
3D city models:
* **201**: Unit cube with intersecting rings in top face
* **201_1**: A single 3D polygon with an interior ring that instersects its exterior
* **201_2**: Unit cube with a duplicate inner ring top face
* **203**: Unit cube where the top face has 5 vertices, one of them (8) is not on the plane of the other 4 (+0.05)
* **203_1**: Unit cube with a rectangular hole drilled through it, where one vertex (8) on the inner ring in the top face of the cube is not on the plane of the face (+0.5)
* **203_2**: Same as 203 but non-planar vertex is +0.1
* **203_3**: same as 203 but non planar is +0.02 (so projection is used)
* **204_1**: Unit cube with small fold in the top face 
* **205**: Unit cube with a polygon with interior disconnected in top face
* **206**: Unit cube with a hole in top face located outside (in 2D the top face is not acc. to SFS)
* **206_1**: Unit cube with a hole in top face located outside and the hole is intersecting the exterior in one point
* **207**: Unit cube with a polygon with nested rings in top face
* **207_1**: Unit cube with a polygon with a nested ring in a nested ring in the top face
* **208**: Unit cube with a hole (inner ring) in the top face having same orientation as outer ring

User input:
* **Poly**: extend scope with GenericCityObject and focus only on poly repairs and lower order repairs
* **Poly_skip**: like ring, but "SkipLowRepairs" is set to true
* **Poly_keep**: like ring, but "KeepEverything" is set to true
* **Poly_tollerance_distance_plane**: like ring, but "planarity_d2p_tol" is set to 0.001

#### Shell level
* **301**: Flat cube, ie with volume of 0, only 2 surfaces
* **301_1**: Cube with only 3 surfaces
* **302**: Unit cube with one face missing (bottom one)
* **302_1**: Unit cube with a hole (inner ring) in the top face
* **302_2**: A vertex in the top surface does not close the shell (0.01 discrepancy), GML cube
* **303**: Unit cube with one dangling face touching the cube at one point only. The dangling face is the last in the list
* **303_1**: 2 unit cubes touching at one vertex
* **303_1b**: same as 303 but 2 unit cubes touching at one vertex are different vertices
* **303_2**: Torus where the hole in the top/bottom faces touches the side surfaces
* **303_cs**: one cube with a non-manifold vertex. As a Solid error 302, but CompositeSurface 303. Jeff Coukell's case.
* **303_4**: Unit cube with one extra face in the middle, splitting the cube in two parts
* **305**: Cube with one extra face floating in the air, not touching
* **305_1**: 2 unit cubes not touching at all, one above the other separated by 1unit
* **306**: House with tip below the ground
* **306_1**: House with tip touching the bottom faces
* **307**: Unit cube with one face (face 0) with opposite orientation
* **307_1**: Unit cube with a top surface composed of 2 polygons with opposite orientation
#### Solid level
* **401**: An inner-pyramid that pierces through the top of basecube (by 0.00001). Oracle agrees: oracle=54512
* **401_1**: basecube = An inner-pyramid that is outside of the shell of basecube, but pierces inside it (by 0.00001)
* **401_2**: Two cavities in basecube that share a face
* **401_3**: Two cavities in basecube that partly share a face
* **401_4**: A cavity which bottom face is shared with the bottom face of basecube
* **401_5**: A cavity which touches bottom and top face of basecube and splits volume
* **401_6**: A cavity which touches bottom and top face of basecube
* **401_7**: Duplicate cavities in basecube (original 402)
* **401_8**: basecube with inner shell that is also the base cube
* **403**: A pyramid cavity that is completely outside of the outer shell of basecube
* **404**: 404 is a blocker that divides the cube into two parts
* **405**: Pyramid with the normals pointing inwards
* **405_1**: basecube with inner Pyramid with the normals pointing outwards
#### Solid interaction level
* **501**:    CompositeSolid with 3 cubes adjacent, where cube 1-2 overlap by 1cm
* **501_1**:  CompositeSolid with (1) basecube + inner_shell (2) inner_shell filling the void
* **502**:    CompositeSolid with 3 cubes, where cube 1-2 are identical
* **503**:    CompositeSolid with 3 cubes, where cube 1 is not connected to other
#### Sub Part level
* **601**:    A Building with 2 BuildingParts that overlap by 50 units
* **601_1**:  A Building with 6 BuildingParts, part of open dataset Den Haag. Parts overlap by <1cm, with overlap_tol 0.01 it's valid


### Other data
