# Repair Report

- input_file : `string` filename
- input_file_type : `string` CityJSON (maybe later other types)
- output_file: `string` filename
- output_file_type : `string` CityJSON (maybe later other types)
- time: `string` time stamp ("Mon Aug  1 18:51:25 2022 CEST")
- type: `string` AUTOr3pairReport
- AUTOr3pair_version: `string` or `int` version
- repaired: `bool` true if so
- parameters: `json` of standards used
    - overlap_tol: `int` Standardused
    - planarity_d2p_tol: `int` Standardused
    - planarity_n_tol: `int` Standardused
    - snap_tol: `int` Standardused
    - extend_scope : `list`[`string` Standardused],
    - RepairDepth: `json` with:
        - maxRepairDepth: `int` Standardused
        - TotalRepairDepth: `int` Standardused
    - ISOr3pair: `json` with:
        - solve_all: `bool` Standardused
        - errors_to_solve:`list`[`int` Standardused],
    - ADDr3pair: `json` with:
        - "additional_repair": `list`[`int` or `string` Standardused],


- all_ISOr3pairs : `set`[`int` error val3dity]
- all_ADDr3pairs : `set`[`string` additional]
- data_errors : `list`[`int` >900 error val3dity]


- features : `list`[`json` per feature]
    - id : `string` original id
    - type : `string` original type
    - repaired: `bool` true if so
    - repairs : `list`[`int` iso error >600 or maybe addr3pair] (for now leave empty)
    - all_ISOr3pairs_primitives : `set`[`int` error val3dity]
    - all_ADDr3pairs_primitives : `set`[`string` error val3dity]
    - Primitives : `list`[`json` per primitive]
        - id : `string` original id or `None` if doenst excist
        - numberfaces : `int` count
        - numbershells : `int` count
        - numbersolids : `int` count
        - numbervertices : `int` count
        - type : `string` original geometry type
        - repaired: `bool` true if so
        - repairs : `list`[`json` per round]
            - round : `int` number of round
            - kind_of_repair : `string` repairname
            - repairs_done: `list`[`json` per repair]
                - code : `int` val3ditycode or addcode?
                - description : `string` val3dity description or add description
                - id : `string` where was the repair (same as val3dity)
                - boundary_before : `list`[original boundary]
                - boundary_now : `list`[new boundary]
        - ISOerrorsremaining : `json` with
            - RingErrors : `list`[`list`[`int` error, `string` where]]
            - PolyErrors : `list`[`list`[`int` error, `string` where]]
            - ShellErrors : `list`[`list`[`int` error, `string` where]]
            - SolidErrors : `list`[`list`[`int` error, `string` where]]
            - SolidIErrors : `list`[`list`[`int` error, `string` where]]


- features_overview : `list`[`json` of all features per type]
    - type : `string` featuretype
    - total : `int` count
    - repaired : `int` count


- primitives_overview : `list`[`json` of all primatives per type]
    - type : `string` primativetype
    - total : `int` count
    - repaired : `int` count