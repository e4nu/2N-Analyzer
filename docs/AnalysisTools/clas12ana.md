
# The `clas12ana` and `clas12debug` classes

**Author:** RG-M members ([rgm repository](https://github.com/awild7/rgm/tree/main))
**Modified by:** Alon Sportes
**Project:** `2N-Analyzer-e4nu`

## Overview

This folder contains two closely-coupled classes used throughout the analysis:

- `clas12ana`: event-level particle selection and cut application.
- `clas12debug`: optional debug histograms filled before and after key selections.

Both classes are built on top of `clas12root` (CLAS12 C++ analysis framework) and rely on CLAS12 banks accessed through `clas12::clas12reader`.

## Quick start

Minimal usage pattern:

```cpp
#include "clas12ana.h"

// ... create / configure your clas12::clas12reader (or use HipoChain) ...

clas12ana ana(true);                 // true enables debug plots
ana.setDebugFile("debugPlots.root");

// Optional: enable or disable specific cut groups
ana.setEcalSFCuts(true);
ana.setEcalPCuts(true);
ana.setEcalDiagCuts(false);
ana.setEcalEdgeCuts(true);
ana.setDCEdgeCuts(true);
ana.setCDEdgeCuts(true);
ana.setPidCuts(true);
ana.setProtonPidCuts(true);
ana.setGhostTrackCuts(true);
ana.setVertexCuts(true);
ana.setVertexCorrCuts(true);
ana.setNpheCuts(false);

// Event loop
while (c12->next()) {
    ana.Run(c12);

    const auto electrons = ana.getByPid(11);
    if (electrons.size() != 1) continue;  // require a good trigger electron

    const auto protons = ana.getByPid(2212);
    const auto piplus  = ana.getByPid(211);
    const auto piminus = ana.getByPid(-211);

    // Charged-particle multiplicity that survived the analysis cuts
    const double nCharged = ana.getEventMult();

    // All particles kept by the analyzer (electrons + charged + neutrals)
    const auto all = ana.getParticles();

    // ... downstream analysis ...
}
// Debug histograms are written in clas12ana destructor when debug_plots=true
```

## Class: `clas12ana`

### Purpose

`clas12ana` is the central event-selection object. For each event it:

1. Clears internal per-event containers.
2. Reads the run number and updates cut parameterizations if needed.
3. Selects electrons (`pid = 11`) using ECAL, HTCC, DC, and vertex requirements.
4. Requires exactly one good electron to proceed.
5. Classifies remaining reconstructed particles by PID and applies analysis cuts.
6. Maintains per-event containers of accepted particles by PID.
7. Optionally fills debug histograms before and after selections.

### What `Run()` does

`void Run(const std::unique_ptr<clas12::clas12reader>& c12)` performs a full event pass.

High-level logic (as implemented):

- Always starts with `Clear()`.
- Calls `checkCutParameters()` to load the correct parameter files for the current run range.
- Collects reconstructed particles via `c12->getDetParticles()`.
- Builds a list of candidate electrons from `c12->getByID(11)`.
- Applies electron cuts (enabled by flags):
  - HTCC Nphe cut (`HTCCNpheCuts` when `f_NpheCuts=true`).
  - ECAL sampling fraction cuts vs energy (`checkEcalSFCuts`).
  - ECAL sampling fraction cuts vs momentum (`checkEcalPCuts`).
  - ECAL diagonal cut (`checkEcalDiagCuts`).
  - ECAL edge cut (`EcalEdgeCuts`).
  - Vertex cuts (`checkVertex`).
  - DC edge cuts (`DCEdgeCuts`).
  - A hard minimum momentum requirement `p > 0.8 GeV`.
- Continues only if `electrons.size() == 1`.
- Loops over all detected particles and:
  - Immediately keeps neutrals (charge 0, excluding electrons), classifying them by PID.
  - For charged non-electrons:
    - Increments `event_mult`.
    - Applies PID selection:
      - Either standard `chi2pid` windows (`checkPidCut`) or a dedicated CD proton TOF-vs-p parameterization (`checkProtonPidCut`), depending on flags.
    - Applies vertex, fiducial, and electron-particle vertex correlation cuts.
    - Adds surviving particles into PID-specific containers.

### Per-event containers

`clas12ana` maintains these vectors (cleared every event):

- `electrons`, `protons`, `deuterons`, `neutrals`, `neutrons`, `piplus`, `piminus`, `kplus`, `kminus`, `otherpart`
- `lead_proton`, `recoil_proton` (SRC helper output)
- `allparticles` (a superset that collects everything kept by the analyzer)

You can retrieve them with:

- `getByPid(int pid)` for a PID-specific container
- `getParticles()` for `allparticles`

### Cut flags

Most selections are controlled via boolean flags with setters:

- Electron and ECAL:
  - `setEcalSFCuts(bool)`
  - `setEcalPCuts(bool)`
  - `setEcalDiagCuts(bool)`
  - `setEcalEdgeCuts(bool)`
  - `setNpheCuts(bool)`

- Tracking and fiducials:
  - `setDCEdgeCuts(bool)`
  - `setCDEdgeCuts(bool)`
  - `setCDRegionCuts(bool)`
  - `setGhostTrackCuts(bool)`

- PID and vertex:
  - `setPidCuts(bool)`
  - `setProtonPidCuts(bool)`
  - `setVertexCuts(bool)`
  - `setVertexCorrCuts(bool)`

Debug control:

- `setDebugPlots(bool)`
- `setDebugFile(TString)`

### Run-dependent parameter loading

`checkCutParameters()` updates cut parameterizations when the run number changes run ranges. It also changes ECAL sampling-fraction parameter sets based on the SF timeline split around run 15542.

Important notes:

- Events with `run == 0` are treated as headers and ignored by the parameter-switching logic.
- For MC, the run number is typically `11`. Automatic parameter switching is not attempted for MC, so the analysis should supply the intended parameter files explicitly.

### PID classification and `allparticles`

Particle classification is centralized in `setByPid(const region_part_ptr&)`.

- It can override the apparent PID for CD protons when `f_protonpidCuts` is enabled.
- It applies a ghost-track rejection for protons (`checkGhostTrackCD`) when `f_ghostTrackCuts` is enabled.
- It pushes every accepted particle into both its PID-specific vector and into `allparticles`.

Because of that, the commented-out `addToAllParticles(...)` calls in `Run()` are intentionally redundant.

### Multiplicity monitoring histograms

`clas12ana` allocates several ROOT histograms intended for quick monitoring:

- Before cuts (BC): filled from the raw `getByID` vectors.
- After cuts (AC): filled from the post-selection PID containers.

They are stored as member pointers allocated with `new`.

## Class: `clas12debug`

### Purpose

`clas12debug` is a helper that owns and fills a large set of debug histograms. It is designed to answer questions like:

- What do ECAL sampling fractions look like before and after selection?
- Are DC edge cuts trimming expected regions?
- How do PID observables look in FD and CD before and after cuts?

`clas12ana` owns an instance `debug_c` and calls into it when `debug_plots` is enabled.

### Key methods

- `InitDebugPlots()`
  - Allocates all histogram objects.

- `fillBeforeEl(...)` and `fillAfterEl(...)`
  - Electron-focused monitoring (ECAL, DC edges, vertex-z, hit maps).

- `fillBeforePart(...)` and `fillAfterPart(...)`
  - Charged-particle monitoring (protons, pions, deuterons, kaons, neutrals).

- `WriteDebugPlots(TString file)`
  - Writes all histograms to a ROOT file.

### Ownership model

Histograms are allocated with `new` and managed manually.

- The code intentionally avoids `std::unique_ptr` for ROOT histograms because ROOT can also take ownership during writing, and that can lead to double deletion when smart pointers are used.
- `clas12ana` writes debug histograms in its destructor if debug is enabled.

If you add additional histograms, follow the existing pattern:

1. Allocate with `new`.
2. Store pointers as class members.
3. Write them in `WriteDebugPlots()`.

## Common pitfalls and sanity checks

- `clas12ana` inherits from `clas12reader` but its `Run()` takes a `clas12reader` instance by const reference. This is intentional: `clas12ana` acts as an analysis helper, not as the event source.

- `checkEcalSFCuts()` and `checkEcalPCuts()` return `false` for non-electrons. That is correct because they are electron-specific cuts.

- `event_mult` counts charged non-electron particles that are inspected in the charged-particle loop, not necessarily the number of particles that survive cuts.

- `allparticles` is filled through `setByPid()` only. If you bypass `setByPid()` you will silently break `getParticles()`.

## Extending the code

If you add a new cut or PID category:

1. Add a flag and setter (if it should be configurable).
2. Implement the cut function.
3. Apply it in the appropriate `Run()` section.
4. If needed, add debug histograms in `clas12debug` and fill them before and after the new cut.

## Files

- `clas12ana.h`, `clas12ana.cpp`: analysis event selector
- `clas12debug.h`, `clas12debug.cpp`: debug histogram helper
- `README.md`: this document