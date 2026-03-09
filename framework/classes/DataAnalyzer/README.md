
# The `DataAnalyzer` class

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

`DataAnalyzer` is a lightweight front-end that selects an analysis mode based on an input file type and then dispatches to the appropriate analyzer.

At the moment, only the reconstructed (HIPO) workflow is active: it instantiates `RecoAnalyzer` and immediately runs its constructor logic.

## Files

- `DataAnalyzer.h`
  - Declares the `DataAnalyzer` class.
  - Includes the implementation file `applications/RecoAnalyzer.cpp` directly.
- `DataAnalyzer.cpp`
  - Defines the `DataAnalyzer` methods.

## What the class does

### Constructor

```cpp
DataAnalyzer(const std::string& FileType, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles);
```

Flow:

1. Calls `ConfigureAnalyzerMode(FileType)`.
2. Calls `RunEventAnalyzer(AnalyzerMode, RecoSamplePath, ReconHipoDir, InputHipoFiles)`.

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
void RunEventAnalyzer(const std::string& AnalyzerMode, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles);
```

Current behavior:

- If `AnalyzerMode == "Detector Simulation"`, constructs a local `RecoAnalyzer` object:
  ```cpp
  RecoAnalyzer RecoEventAnalyzer(RecoSamplePath, ReconHipoDir, InputHipoFiles);
  ```
  Any work is expected to happen inside the `RecoAnalyzer` constructor (or methods it calls).

The `TruthAnalyzer` path is present as commented code and is not currently executed.

## How to use

Typical usage from a driver (example):

```cpp
#include "framework/classes/DataAnalyzer/DataAnalyzer.h"

int main() {
    const std::string fileType      = "hipo";
    const std::string recoSample    = "/path/to/reco/sample";
    const std::string reconHipoDir  = "/path/to/reconhipo";
    const std::string inputHipoList = "/path/to/input_files.txt";

    DataAnalyzer analyzer(fileType, recoSample, reconHipoDir, inputHipoList);
    return 0;
}
```

Notes:

- Use `FileType = "hipo"` to run the reconstructed workflow.
- `InputHipoFiles` is passed through to `RecoAnalyzer` as provided (string meaning is defined by `RecoAnalyzer`).

## Design and build notes

- `RunEventAnalyzer` creates a local `RecoAnalyzer` instance; it does not store it as a member. This is intentional given the current implementation pattern: the constructor call is used as the entry point.

- The header currently includes a `.cpp` file (`RecoAnalyzer.cpp`). This can work in some build setups but is unconventional and can easily lead to multiple-definition issues if the same translation unit is built elsewhere. If you later refactor:
  - Prefer including a `RecoAnalyzer.h` header and linking against the compiled `RecoAnalyzer` object/library.

- The error handling in `ConfigureAnalyzerMode` terminates the process. If you later need the caller to handle errors, replace `exit(1)` with an exception or an error return.

## Inputs

`DataAnalyzer` does not validate paths; it forwards them:

- `RecoSamplePath`: full path that identifies the reconstructed sample.
- `ReconHipoDir`: directory for reconstructed HIPO output/input (as expected by `RecoAnalyzer`).
- `InputHipoFiles`: a user-provided string (often a file list or pattern) consumed by `RecoAnalyzer`.

## Outputs

All outputs are produced by `RecoAnalyzer` (and future analyzers), not by `DataAnalyzer` itself.