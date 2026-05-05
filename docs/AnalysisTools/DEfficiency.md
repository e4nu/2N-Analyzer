# The `DEfficiency` class

Source files:

- [`include/AnalysisTools/DEfficiency.h`](../../include/AnalysisTools/DEfficiency.h)
- [`source/AnalysisTools/DEfficiency.cpp`](../../source/AnalysisTools/DEfficiency.cpp)

## Purpose

`DEfficiency` builds reconstructed-versus-truth efficiency products from 1D histograms and exports the resulting acceptance-correction plots.

## Main Responsibilities

- Load truth-level and reconstructed histograms
- Build efficiency / acceptance-correction views
- Draw and save the derived plots
- Optionally append the products into ROOT `TList` containers

## Main Entry Points

- `LoadHistograms(...)`
- `DrawACorrHistograms(...)`
- `DrawAndSaveACorrPlots(...)`
- `DrawAndSaveHistogram1D(...)`

## Notes

- The class operates on `hPlot1D` wrappers and raw `TH1D*` inputs.
- Internally it keeps cloned histograms so it can rebin and restyle them without mutating the caller-owned input objects.
