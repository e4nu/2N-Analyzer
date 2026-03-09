# `Directories` Class

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

`Directories` builds and manages the full plots-output directory tree for the analysis.

It does three things in one place:
- Creates directory structure under a root folder (default: `./plots`).
- Optionally clears old output under that root.
- Publishes canonical output paths through public maps (for example `Nphe_dir_map`, `ReacMon_dir_map`, `Ecal_dir_map`, etc.).

The class is intended to be constructed once at job startup, then used as a central source of output paths.

## Constructor Behavior

```cpp
Directories(const std::string& plots_path = "./plots", const bool Clear_Old_Directories = true);
```

When constructed:
1. Stores `plots_path` in the class member `Plots_Folder`.
2. Ensures the root folder exists (`mkdir -p`).
3. If `Clear_Old_Directories` is `true`, clears existing files under root (`rm -r <root>/*`).
4. Creates category-specific parent/daughter directories (according to `create_*_Dir` flags).
5. Fills all public directory maps with canonical paths ending in `/`.

## Current Internal Design

The implementation is helper-driven and optimized to reduce duplication.

### Path helpers

- `BuildPath(base, parent, daughter = "")`
  - Builds normalized `base/parent[/daughter]/` output path.

### Directory creation helper

- `CreateDirectories(create_flag, parent_folder, daughter_folders, clear_parent, root)`
  - Early return if `create_flag == false`.
  - Iterates daughter folders and delegates to `MakeDirectory(...)`.

### Map population helper

- `SetDirectory(map, key, root, parent_folder, daughter_folders, daughter_selector)`
  - Resolves daughter folder via cached lookup and writes a canonical path into the map.

### Lookup helpers

- `Find(...)`
  - Finds the first element containing the requested selector substring.
- `FindCached(...)`
  - Caches `Find(...)` results per daughter-folder vector to avoid repeated linear scans.

### Low-level shell call

- `MakeDirectory(...)`
  - Early return if creation disabled.
  - Executes `mkdir -p` for target directory.
  - Optionally executes parent cleanup command before creation.

## Public Maps

`Directories` exposes one `std::map<std::string, std::string>` per analysis category.

Main groups include:
- Cut/selection plots: `Nphe_dir_map`, `Chi2_dir_map`, `Vertex_dir_map`, `SF_dir_map`, `ECAL_fiducial_dir_map`, `Momentum_dir_map`
- Monitoring/physics plots: `ReacMon_dir_map`, `W_dir_map`, `Beta_dir_map`, `Angle_dir_map`, `Momentum_transfer_dir_map`, `E_e_dir_map`, `Omega_dir_map`, `Ecal_dir_map`, `TKI_dir_map`, `ToF_dir_map`
- Efficiency/corrections/validation: `Eff_and_ACorr_dir_map`, `AMaps_dir_map`, `Resolution_dir_map`, `Multiplicity_dir_map`

## Usage Example

```cpp
Directories dirs("./plots", true);

const std::string nphe_1p_out = dirs.Nphe_dir_map.at("Nphe_1p_Directory");
const std::string reacmon_2p_out = dirs.ReacMon_dir_map.at("ReacMon_2p_Directory");
```

## Conventions

- Stored map paths end with `/`.
- Parent/daughter strings are relative to the constructor root.
- Many daughter vectors include `""` as first item to ensure parent directory creation.
- Directory creation relies on shell `system(...)` calls.

## Safety Notes

- `Clear_Old_Directories = true` is destructive under the selected root.
- Use a dedicated output folder as `plots_path`.
- If paths may contain spaces or special shell characters, consider quoting/escaping logic in the implementation.

## Extending the Class

To add a new category:
1. Add private members in `Directories.h`:
   - `create_<New>_Dir`
   - `<New>_Parent_Directory`
   - `<New>_Daughter_Folders`
2. Add a public map:
   - `<New>_dir_map`
3. In constructor (`Directories.cpp`):
   - Call `CreateDirectories(...)` for the new category.
   - Fill map entries using `BuildPath(...)` / `SetDirectory(...)`.
4. Use stable key names; these are consumed by analysis modules.

## Known Quirks Preserved Intentionally

The class currently keeps some historical behavior exactly as-is (for compatibility), including repeated assignment patterns for some map keys where they already existed in the original implementation.
