# The `gst` class

Source files:

- [`include/AnalysisTools/gst.h`](../../include/AnalysisTools/gst.h)
- [`source/AnalysisTools/gst.cpp`](../../source/AnalysisTools/gst.cpp)

## Purpose

`gst` is the ROOT tree wrapper for GENIE Standard Tree content.

## What It Contains

- scalar event metadata such as interaction channel and kinematics
- fixed-size arrays for initial-state and final-state particle content
- `TBranch*` members for each bound tree branch

## Status In This Repository

- The type still exists and is documented
- The current production workflow is centered on HIPO reconstruction input, not GST input
- Older truth-level or generator-side utilities may still depend on this wrapper

If you are working on the active reconstructed analysis path, start with `RecoAnalyzer` rather than `gst`.
