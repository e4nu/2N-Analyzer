# Build System

The project uses CMake and builds one main executable, `TwoNAnalyzerReco`.

## Primary Files

- [`CMakeLists.txt`](../CMakeLists.txt)
- [`source/CMakeLists.txt`](../source/CMakeLists.txt)
- [`apps/CMakeLists.txt`](../apps/CMakeLists.txt)

## Requirements

Configuration expects these environment variables to be set:

- `CLAS12ROOT`
- `HIPO`
- `ROOTSYS`

The top-level CMake file fails fast if `CLAS12ROOT` or `HIPO` are missing.

## Targets

### Library

`source/CMakeLists.txt` builds `TwoNAnalysisCore` from the code under:

- `source/AnalysisTools`
- `source/Settings`
- `source/io`
- `source/TwoNAnalyzer/RecoAnalyzer`
- `source/libraries`

### Executable

`apps/CMakeLists.txt` builds:

- `TwoNAnalyzerReco`

from:

- [`apps/TwoNAnalyzer/RecoAnalyzer/main.cpp`](../apps/TwoNAnalyzer/RecoAnalyzer/main.cpp)

and links it against `TwoNAnalysisCore`.

## Dependency Handling

- `yaml-cpp` is vendored and added with `add_subdirectory(...)`.
- ROOT is discovered with `find_package(ROOT REQUIRED)`.
- HIPO4 is located with `find_library(...)`.
- MPI is optional behind `-DUSE_MPI=ON`.

## Build Notes

- The project enforces C++17.
- `TwoNAnalysisCore` uses a CMake unity build.
- Build-time macros define `_CLAS12ANA_DIR` and `_CONFIG_DIR`.
- [`include/CMakeLists.txt`](../include/CMakeLists.txt) is explicitly marked legacy and should not be added as a build subdirectory.
