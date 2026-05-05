# Architecture

The repository is organized around a reusable C++ analysis library plus a thin executable layer.

## Top-Level Layout

- `apps/`: executable targets
- `include/`: public headers and configuration types
- `source/`: implementation files compiled into the main library
- `config/`: YAML runtime configuration
- `scripts/`: shell wrappers for environment setup and execution
- `vendor/yaml-cpp/`: vendored YAML parser dependency

## Code Layers

### Application layer

- [`apps/CMakeLists.txt`](../apps/CMakeLists.txt)
- [`apps/TwoNAnalyzer/RecoAnalyzer/main.cpp`](../apps/TwoNAnalyzer/RecoAnalyzer/main.cpp)
- [`source/TwoNAnalyzer/RecoAnalyzer/RecoApp.cpp`](../source/TwoNAnalyzer/RecoAnalyzer/RecoApp.cpp)

This layer parses CLI input and loops over the configured sample chain.

### Analysis library

- [`source/CMakeLists.txt`](../source/CMakeLists.txt)

This builds `TwoNAnalysisCore`, which contains:

- `AnalysisTools/`
- `Settings/`
- `io/`
- `TwoNAnalyzer/RecoAnalyzer/`
- helper libraries under `libraries/`

### Runtime configuration

- [`include/io/Yaml_loader.h`](../include/io/Yaml_loader.h)
- [`include/io/analysis_samples.h`](../include/io/analysis_samples.h)
- [`include/Settings/`](../include/Settings)
- [`config/`](../config)

### External dependencies

- ROOT
- CLAS12ROOT / Clas12Root
- HIPO4
- yaml-cpp
- optional MPI support

## Design Notes

- The repo has migrated away from older hard-coded setup headers toward YAML-driven runtime configuration.
- `include/CMakeLists.txt` is kept as a legacy file and is not the active build path.
- The code still contains some legacy interfaces for compatibility, especially around sample loading and histogram helpers.
