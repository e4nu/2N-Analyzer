
# The `DataAnalyzer` class

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

`DataAnalyzer` is a lightweight front-end that selects an analysis mode based on an input file type and dispatches to the appropriate analyzer.

In the current code path, the reconstructed HIPO workflow is the active path: it constructs `RecoAnalyzer` with the selected sample and CLI options.

## Files

- [`include/AnalysisTools/DataAnalyzer.h`](../../include/AnalysisTools/DataAnalyzer.h)
  - Declares the `DataAnalyzer` class.
- [`source/AnalysisTools/DataAnalyzer.cpp`](../../source/AnalysisTools/DataAnalyzer.cpp)
  - Defines the `DataAnalyzer` methods.

## What the class does

### Constructor

```cpp
DataAnalyzer(const std::string& FileType,
             const std::string& RecoSamplePath,
             const std::string& ReconHipoDir,
             const std::string& InputHipoFiles,
             const reco_cli::RecoCliOptions& cli);
```

Flow:

1. Calls `ConfigureAnalyzerMode(FileType)`.
2. Calls `RunEventAnalyzer(AnalyzerMode, RecoSamplePath, ReconHipoDir, InputHipoFiles, cli)`.

### ConfigureAnalyzerMode

```cpp
std::string ConfigureAnalyzerMode(const std::string& FileType);
```

Maps `FileType` to an internal mode string:

- `"root"`  -> `"Truth level"`
- `"hipo"`  -> `"Detector Simulation"`

If `FileType` is anything else, the code prints an error and terminates the process via `exit(1)`.

### RunEventAnalyzer

```cpp
void RunEventAnalyzer(const std::string& AnalyzerMode,
                      const std::string& RecoSamplePath,
                      const std::string& ReconHipoDir,
                      const std::string& InputHipoFiles,
                      const reco_cli::RecoCliOptions& cli);
```

Current behavior:

- If `AnalyzerMode == "Detector Simulation"`, constructs a local `RecoAnalyzer` object:
  ```cpp
  RecoAnalyzer recoEventAnalyzer(RecoSamplePath, ReconHipoDir, InputHipoFiles, cli);
  ```
  The work then proceeds inside the `RecoAnalyzer` constructor and its downstream setup/event-loop code.

The `TruthAnalyzer` path is present as commented code and is not currently executed.

## How to use

Typical usage from a driver (example):

The active application path creates `DataAnalyzer` from [`source/TwoNAnalyzer/RecoAnalyzer/RecoApp.cpp`](../../source/TwoNAnalyzer/RecoAnalyzer/RecoApp.cpp) after parsing `reco_cli::RecoCliOptions`.

Notes:

- Use `FileType = "hipo"` to run the reconstructed workflow.
- `InputHipoFiles` is passed through to `RecoAnalyzer` as provided (string meaning is defined by `RecoAnalyzer`).

## Design and build notes

- `RunEventAnalyzer` creates a local `RecoAnalyzer` instance; it does not store it as a member.
- The modern header layout uses `RecoAnalyzer.h`; the older "include a .cpp file from the header" pattern is no longer the active design here.

- The error handling in `ConfigureAnalyzerMode` terminates the process. If you later need the caller to handle errors, replace `exit(1)` with an exception or an error return.

## Inputs

`DataAnalyzer` does not validate paths; it forwards them:

- `RecoSamplePath`: full path that identifies the reconstructed sample.
- `ReconHipoDir`: directory for reconstructed HIPO output/input (as expected by `RecoAnalyzer`).
- `InputHipoFiles`: a user-provided string (often a file list or pattern) consumed by `RecoAnalyzer`.

## Outputs

All outputs are produced by `RecoAnalyzer` (and future analyzers), not by `DataAnalyzer` itself.
