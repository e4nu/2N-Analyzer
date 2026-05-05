# The `Debugger` class

Source files:

- [`include/AnalysisTools/Debugger.h`](../../include/AnalysisTools/Debugger.h)
- [`source/AnalysisTools/Debugger.cpp`](../../source/AnalysisTools/Debugger.cpp)

## Purpose

`Debugger` centralizes runtime safety checks and structured diagnostic printing for the analysis code.

## What It Does

- Emit step-by-step execution markers with file and line information
- Print fatal configuration or runtime errors
- Warn about risky run configurations
- Run targeted safety checks on reconstructed and truth-level particle collections
- Validate assumptions in acceptance-map, nucleon-selection, and event-selection code paths

## Representative Methods

- `PrintStepTester(...)`
- `PrintErrorMessage(...)`
- `PrintRunConfigurationWarnings(...)`
- `SafetyCheck_FD_neutron(...)`
- `SafetyCheck_one_good_electron(...)`
- `SafetyCheck_AMaps_*`

## Usage Pattern

The project exposes a shared debugger instance through the debugging setup headers, and `RecoAnalyzer` uses it heavily during setup and event processing.

See also:

- [`../../include/Settings/setup/debugging.h`](../../include/Settings/setup/debugging.h)
