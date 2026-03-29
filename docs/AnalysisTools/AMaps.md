# AMaps Class Guide

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

`AMaps` manages detector acceptance products in `(phi, theta)` momentum slices for electrons, protons, and neutrons.

It supports two modes:

- `AMaps`: binary acceptance maps (`0/1`) after thresholding `Reco/TL`.
- `WMaps`: continuous weight maps for acceptance correction.

## Core Responsibilities

- Fill truth/reco hit maps from events.
- Compute acceptance efficiency (`Reco/TL`).
- Build in-memory map matrices:
  - `e_AMap_Slices`, `p_AMap_Slices`, `n_AMap_Slices`, `n_AMap_ConstSlices`
  - `e_WMap_Slices`, `p_WMap_Slices`, `n_WMap_Slices`, `n_WMap_ConstSlices`
- Save/load map slices in `.par` format.
- Save map ROOT/PDF products.
- Provide acceptance matching and weight lookup during analysis.

## Main Methods

- `hFillMaps(...)`: fills per-particle truth/reco histograms by event.
- `CalcAcceptanceEfficiency()`: computes `Reco/TL` histograms.
- `GenerateFilteredRecoMaps(cP_minR, nP_minR)`: applies thresholds to ratio maps.
- `GenerateMapMatrices(cP_minR, nP_minR)`: converts histograms to AMap/WMap matrices.
- `DrawAndSaveMaps(...)`: full generation + draw + save pipeline.
- `SaveMaps(...)`: writes mode-dependent `.par` slice files.
- `ReadMapSlices(...)` / `ReadMap(...)`: load `.par` slices.
- `MatchToAMap(...)`, `GetWeight(...)`, `IsInFDQuery(...)`: analysis-time map queries.

## New WMap Loader From ROOT Ratios

`LoadWMapsFromRatioROOTFiles(...)` in `AMaps` reads ratio ROOT files and recreates WMap `.par` files.

Signature:

```cpp
void LoadWMapsFromRatioROOTFiles(
    const std::string& SampleName,
    const std::string& InputAcceptanceMapsDirectory,
    const std::string& OutputAcceptanceMapsDirectory = ""
);
```

Behavior:

- Input directory is used to read ratio ROOT files and slice-limit references.
- Output directory is used to write WMap `.par` files.
- If output directory is empty, input directory is reused.
- Thresholding used during WMap export:
  - electrons/protons: keep ratio `>= 0.7`, else `0`
  - neutrons (regular and const): keep ratio `>= 0.2`, else `0`

## `.par` Output Format

Each slice file includes:

- `Lower_P_lim`, `Upper_P_lim`
- bin labels (`HistElectronSliceNumOfXBins` / `HistNucSliceNumOfXBins`, etc.)
- row data with `Weight\t` and `:`-separated bin values

Each particle directory also has a slice-limits file:

- `e_slice_limits.par`
- `p_slice_limits.par`
- `n_slice_limits.par`

## Directory Layout (Current)

Per sample (`<SampleName>`):

- AMaps mode output:
  - `e_AMaps_by_slice/`
  - `p_AMaps_by_slice/`
  - `n_AMaps_by_slice/`
  - `n_AMaps_by_ConstSlice/`
  - `e_extended_AMaps_by_slice/`
- WMaps mode output:
  - `e_WMap_by_slice/`
  - `p_WMap_by_slice/`
  - `n_WMap_by_slice/`
  - `n_WMap_by_ConstSlice/`

ROOT summary outputs are also written with prefixes:

- `00_AMaps_BC_-_`
- `01_AMap_TL_-_`
- `02_AMap_Reco_-_`
- `03_AMap_Ratio_-_`
- `04_cPart_Sep_AMaps_-_` (and const neutron variant)
- `05_AMaps_-_`

## Binning Profiles

Electron profiles:

- `reformat_e_bins`
- `varying_P_e_bins`
- `uniform_P_e_bins`
- `equi_inverted_P_e`

Nucleon profiles:

- `equi_inverted_P_nuc`
- `varying_P_nuc_bins`
- `uniform_P_nuc_bins`

## Usage Notes

- In generation mode, use the generation constructor and call `DrawAndSaveMaps(...)`.
- In loading mode, use the loading constructor and `ReadMapSlices`/query methods.
- `IsInFDQuery(...)` behavior depends on generation/loading context and map availability.
- Neutron maps are managed both in regular momentum slices and const slices.

# AMaps Class Guide

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

---

# Overview

`AMaps` is the acceptance–map backend of the analysis framework.  
It manages the full lifecycle of detector acceptance products in angular
`(phi, theta)` space for electrons, protons, and neutrons.

The class supports two operating modes:

| Mode | Purpose |
|-----|-----|
| **AMaps** | Binary acceptance masks used as fiducial cuts |
| **WMaps** | Floating efficiency maps used for acceptance weighting |

Conceptually the maps represent detector acceptance as

```
Efficiency(theta,phi) = Reco(theta,phi) / Truth(theta,phi)
```

This quantity is then used either as

- a **binary mask** (`AMaps`) after thresholding
- a **floating correction weight** (`WMaps`)

---

# Core Responsibilities

The class performs the following tasks:

### Map Generation

- Fill truth‑level `(theta,phi)` hit maps
- Fill reconstructed `(theta,phi)` hit maps
- Compute acceptance efficiency (`Reco/TL`)
- Remove low‑efficiency regions
- Convert histograms to matrix maps

### Disk I/O

- Save map slices to `.par` files
- Load map slices for analysis
- Save ROOT and PDF diagnostics

### Analysis Queries

- Determine if a particle lies inside detector acceptance
- Provide acceptance weights for physics corrections

---

# Internal Map Containers

AMaps maintains in‑memory matrix representations of acceptance maps.

## Binary acceptance maps

```
e_AMap_Slices
p_AMap_Slices
n_AMap_Slices
n_AMap_ConstSlices
```

These store integer masks (`0` or `1`).

## Weight maps

```
e_WMap_Slices
p_WMap_Slices
n_WMap_Slices
n_WMap_ConstSlices
```

These store floating efficiency values.

Neutron maps exist in two forms:

| Type | Purpose |
|-----|-----|
| **Regular slices** | neutron acceptance by momentum |
| **Const slices** | replicated slices used for nucleon overlap logic |

---

# Main Generation Pipeline

The map generation workflow is executed by `DrawAndSaveMaps(...)`.

## Generation Steps

1. Fill truth/reco histograms via

```
hFillMaps(...)
```

2. Compute acceptance efficiency

```
CalcAcceptanceEfficiency()
```

3. Remove poorly reconstructed regions

```
GenerateFilteredRecoMaps(...)
```

4. Convert histograms to matrix maps

```
GenerateMapMatrices(...)
```

5. Extend electron acceptance regions

```
GenerateExtendedElectronMapMatrices(...)
```

6. Save slice files

```
SaveMaps(...)
```

7. Export diagnostic plots

```
DrawAndSaveMapsBySliceToPDFs(...)
```

---

# Map Matching (Analysis Mode)

During analysis the class provides fast map lookups.

## Matching entry point

```
MatchToAMap(...)
```

This dispatches matching to

```
MatchToElectronAMap(...)
MatchToNucleonAMap(...)
```

Both use a fast indexing algorithm:

1. Convert `(theta,phi)` directly to bin indices
2. Binary search the momentum slice list
3. Query the map matrix

This avoids the original triple nested loops and scales as

```
O(1) in theta
O(1) in phi
O(log Nslice) in momentum
```

This optimization is critical when processing millions of events.

---

# Acceptance Weight Queries

## Public interface

```
GetWeight(...)
```

Behavior:

| Particle | Weight returned |
|-----|-----|
| Electron | 1 |
| Proton | weight from proton WMap |
| Neutron | weight from neutron WMap |

### Nucleon weight lookup

```
GetNucleonWeight(...)
```

This uses the same fast slice/bin indexing as the matching routines.

If the stored map value is

```
w
```

then the returned weight is

```
1 / w
```

---

# Forward Detector Query

High‑level FD selection is performed by

```
IsInFDQuery(...)
```

The function checks

1. map acceptance
2. `Theta` inside the FD angular cut

When generating maps (`Generate_AMaps == true`) the map test is skipped
so that statistics are not artificially reduced.

---

# Map File Format (`.par`)

Each slice file contains

- slice momentum limits
- histogram bin counts
- rows of bin values

Example:

```
Lower_P_lim 0.5
Upper_P_lim 1.0
Weight0 0.92:0.85:0.00
Weight1 1.00:0.90:0.10
```

Separate slice‑limit files also exist:

```
e_slice_limits.par
p_slice_limits.par
n_slice_limits.par
```

---

# Directory Layout

For each sample:

## AMaps mode

```
e_AMaps_by_slice/
p_AMaps_by_slice/
n_AMaps_by_slice/
n_AMaps_by_ConstSlice/
e_extended_AMaps_by_slice/
```

## WMaps mode

```
e_WMap_by_slice/
p_WMap_by_slice/
n_WMap_by_slice/
n_WMap_by_ConstSlice/
```

ROOT diagnostic files are also written using prefixes

```
00_AMaps_BC
01_AMap_TL
02_AMap_Reco
03_AMap_Ratio
04_cPart_Sep_AMaps
05_AMaps
```

---

# Loading Maps

Map slices can be loaded using

```
ReadMapSlices(...)
ReadMap(...)
```

Slice boundaries are read with

```
ReadAMapLimits(...)
```

These methods reconstruct the matrix containers used during matching.

---

# WMap Reconstruction From ROOT Ratio Files

The class also supports reconstructing WMaps directly from ROOT ratio files.

```
LoadWMapsFromRatioROOTFiles(...)
```

Workflow:

1. Read ratio ROOT files
2. Extract `TH2D` efficiency histograms
3. Convert histograms to matrices
4. Load slice boundaries
5. Re‑export `.par` weight maps

Thresholds applied during export:

| Particle | Ratio threshold |
|-----|-----|
| Electron | 0.7 |
| Proton | 0.7 |
| Neutron | 0.2 |

---

# Momentum Binning Profiles

Electron momentum slicing supports

```
reformat_e_bins
varying_P_e_bins
uniform_P_e_bins
equi_inverted_P_e
```

Nucleon momentum slicing supports

```
equi_inverted_P_nuc
varying_P_nuc_bins
uniform_P_nuc_bins
```

These determine how `(theta,phi)` acceptance maps are grouped by
particle momentum.

---

# Important Design Choices

### Separate charged and neutral handling

Electrons and protons rely strongly on momentum‑dependent detector
acceptance, so they are always sliced in momentum.

Neutrons are treated differently and may use

- physical neutron maps
- const‑slice neutron maps for overlap logic

### Binary vs floating maps

The class intentionally separates

- **fiducial acceptance** (`AMaps`)
- **acceptance corrections** (`WMaps`)

This prevents selection logic from depending on floating corrections.

### Fast lookup algorithm

Direct bin indexing and binary search replaced the original
triple‑loop matching algorithm, dramatically improving runtime.

---

# Typical Usage

## Map generation

```
AMaps maps(...);
maps.DrawAndSaveMaps(...);
```

## Map loading and queries

```
AMaps maps(...);

maps.MatchToAMap(...);

maps.GetWeight(...);

maps.IsInFDQuery(...);
```

---

# Summary

`AMaps` is the central infrastructure for detector acceptance in the
analysis.

It connects

```
Monte Carlo truth
→ reconstructed detector response
→ acceptance maps
→ fiducial selection
→ physics weights
```

while keeping generation, storage, and analysis usage unified inside
one class.