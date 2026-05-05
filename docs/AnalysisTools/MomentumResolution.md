# The `MomentumResolution` class

Source files:

- [`include/AnalysisTools/MomentumResolution.h`](../../include/AnalysisTools/MomentumResolution.h)
- [`source/AnalysisTools/MomentumResolution.cpp`](../../source/AnalysisTools/MomentumResolution.cpp)

## Purpose

`MomentumResolution` manages the neutron and proton momentum-resolution workflow: slice definition, histogram filling, slice fitting, coefficient loading, correction, and smearing.

## Main Responsibilities

- configure the run mode from `MomentumResolutionSettings`
- define truth-level and reconstructed momentum slices
- fit slice histograms and extract mean/width parameters
- save and reload fit coefficients
- apply momentum smearing and correction in analysis mode

## Important Interfaces

- `MomResInit(...)`
- `SetMomResCalculations(...)`
- `SetMomResSlicesByType(...)`
- fit-coefficient loading and saving helpers in the implementation

## Modes

The runtime modes are driven by [`include/Settings/MomentumResolutionSettings.h`](../../include/Settings/MomentumResolutionSettings.h):

- normal run
- `momResS1` fit generation
- `momResS2` coefficient generation
- `momResS2` run mode

## Notes

- The class is parameterized separately for `"Neutron"` and `"Proton"`.
- It owns a large amount of ROOT state, including slice histograms, fit folders, and random-number generators.
- The root README documents the intended high-level mode combinations.
