# Legacy Histogram Plotting Functions

Source files:

- [`include/functions/HistogramPlottingFunctions.h`](../../include/functions/HistogramPlottingFunctions.h)
- [`source/functions/HistogramPlottingFunctions.cpp`](../../source/functions/HistogramPlottingFunctions.cpp)

## Purpose

This module contains the older free-function plotting API that predates the `hPlot1D`, `hPlot2D`, and `hsPlots` wrapper classes.

## What It Provides

- Many `histPlotter1D(...)` overloads for:
  - symmetric cut overlays
  - asymmetric cut overlays
  - one-sided cut overlays
  - stack and non-stack output
  - chi2-specific drawing variants
- `histPlotter1DwFit(...)` for 1D drawing with fit overlays
- `stackPlotter1D(...)` for ROOT `THStack` output
- Several `histPlotter2D(...)` overloads, including beta-vs-momentum plot helpers

## Current Status

- Still built and available through `TwoNAnalysisCore`
- Kept mainly for compatibility with older plotting call sites
- Newer code in this repository generally prefers the wrapper classes documented in:
  - [`../AnalysisTools/hPlot1D.md`](../AnalysisTools/hPlot1D.md)
  - [`../AnalysisTools/hPlot2D.md`](../AnalysisTools/hPlot2D.md)
  - [`../AnalysisTools/hsPlots.md`](../AnalysisTools/hsPlots.md)
