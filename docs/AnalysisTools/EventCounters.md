# The `EventCounters` struct

Source file:

- [`include/AnalysisTools/EventCounters.h`](../../include/AnalysisTools/EventCounters.h)

## Purpose

`EventCounters` is a plain aggregate used to collect event-category totals during analysis.

## What It Tracks

- overall processed-event totals
- generator-channel totals such as `QEL`, `MEC`, `RES`, and `DIS`
- electron-selection counts
- FD neutron and photon hit counts
- final-state topology counts such as `1e1p`, `1e2p`, `1n`, `pFDpCD`, and `nFDpCD`

## Design

- no methods
- all members default-initialize to zero
- intended for direct incrementing by the analysis loop

This type is a reporting container rather than a policy object.
