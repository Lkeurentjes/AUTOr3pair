## What the code does
### main
the main file does the following things:
1. Read JSON files
    1. Parse the cityJSON and the UserInput
    2. Create vertices vector (and scale them if needed)
2. Repair loop
    1. Get single object
        1. check if object has geometry
        2. check if objecttype is in buildings scope
        3. write object to TU3DJSON in *tu3djson_convert.ccp*
    2. Do ISO repair
        1. get first report
        2. if errors --> go to repair function in *isoRepair.ccp*
    3. Do additional repairs
        1. todo
    4. write object bake to cityjson
3. Write CityJSON
    1. copy old cityjson and replace:
        1. vertices (and rescale them)
        2. cityobjects
    2. Do some post processing to clean the file
        1. TODO
    3. wite output JSON file

### TU3DJSON converter
file has two functions:
1. **make tu3djson object**, which converts a cityobject into a tu3djson
    1. setting type
    2. setting feature(s)
        1. setting geometry boundary
        2. setting type
        3. setting properties to no loose information
            1. cityobject information
            2. geometry information
2. **make cityobject**, which converts a tu3djson to a cityobject
    1. find name
    2. setting type
    3. setting other cityobject information
    4. setting geometry/geometries
        1. setting type
        2. setting lod
        3. setting boundaries
        4. setting extra information

### IsoRepair
IsoRepair is a class made to repair the errors based on val3dity.

The constructor sets the TU3DJSON, the userinput and the vertices list so they can be used easily.
The main function of this class is the **iso_repair_loop**.
This repair loop works the following:
1. the input for the function is the val3dity report
2. it loops over the features in the report and checks which ones are invalid
3. When invalid it search for the type (seeing the report needs to be read differently), and the feature gets send to a helper function. It has three helper functions for this:
    1. repair_solid (for solids)
    2. repair_Msolid (for multi- and compositesolids)
    3. repair_Msurface (for multisurfaces)
4. The helper function finds the errorcode(s)
5. When the error needs to be solved (based on user input) it is added to its "geometry_type" list
6. Those type list are used to know where the feature needs to be send to be repaired:
    1. Ring repairs go to the **RingRepair** class in *RingRepair.ccp*
    2. Polygon repairs go to the **PolygonRepair** class in *PolygonRepair.ccp*
    3. Shell repairs go to the **ShellRepair** class in *ShellRepair.ccp*
    4. Solid repairs go to the **SolidRepair** class in *SolidRepair.ccp*
    5. Solid interaction repairs go to the **SolidiRepair** class in *SolidiRepair.ccp*
7. The function can only do one type of repair before restarting the loop (so when ring repair is done, it doesn't do the solid repair)
8. With the new feature replaced in the TU3DJSON a new report is made
9. If there are error to be solved (based on user input) the whole function is "restarted" with the use of recursion.
10. This loops until all errors are solved or max repair depth is reached (standard or userinputted)

#### Ring Repair

#### Polygon Repair
todo
#### Shell Repair
todo
#### Solid Repair
todo
#### Solid interaction Repair
todo

