# The `TLCuts` class

Source files:

- [`include/AnalysisTools/TLCuts.h`](../../include/AnalysisTools/TLCuts.h)
- [`source/AnalysisTools/TLCuts.cpp`](../../source/AnalysisTools/TLCuts.cpp)

## Purpose

`TLCuts` performs truth-level particle bookkeeping and event selection using the generator-side particle content from the CLAS12/GENIE inputs.

## Constructor Inputs

The constructor receives:

- sample and mode flags
- detector-angle cut definitions
- truth-level momentum cut definitions for electrons, nucleons, pions, and photons
- optional acceptance maps
- the active `clas12::clas12reader`

## What It Builds

- particle-index vectors by species
- FD/CD-specific truth-level index vectors
- momentum-threshold filtered vectors
- boolean flags describing whether the event satisfies the supported truth-level topologies

## Supported Topologies

- `1p`
- `1n`
- `pFDpCD`
- `nFDpCD`

This class is a truth-level selection helper, not a general settings container.
