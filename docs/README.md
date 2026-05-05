# Documentation Overview

This folder documents the code that is currently built from `include/` and `source/`.

## What Changed

Older copies of these docs described a `framework/...` tree that no longer exists in this repository. The active code now lives under:

- `include/AnalysisTools`
- `include/Settings`
- `include/io`
- `include/TwoNAnalyzer`
- `source/...`

The top-level executable entry point is built from:

- [`apps/CMakeLists.txt`](../apps/CMakeLists.txt)
- [`apps/TwoNAnalyzer/RecoAnalyzer/main.cpp`](../apps/TwoNAnalyzer/RecoAnalyzer/main.cpp)
- [`source/TwoNAnalyzer/RecoAnalyzer/RecoApp.cpp`](../source/TwoNAnalyzer/RecoAnalyzer/RecoApp.cpp)

## Reading Order

If you are new to the codebase, start with:

1. [Build system](build_system.md)
2. [Architecture](architecture.md)
3. [Analysis framework](analysis_framework.md)
4. [Physics modules](physics_modules.md)

Then drill into the class pages under [AnalysisTools](AnalysisTools/) and [Settings](Settings/).

## Runtime Configuration

The reconstructed workflow is now configured primarily through YAML:

- Sample chain: [`config/samples/`](../config/samples)
- Event selection: [`config/EventSelection/`](../config/EventSelection)
- Analysis cuts: [`config/cuts/`](../config/cuts)
- Acceptance maps: [`config/tools/AcceptanceMapsSettings_Default.yaml`](../config/tools/AcceptanceMapsSettings_Default.yaml)
- Momentum resolution: [`config/tools/MomentumResolutionSettings_Default.yaml`](../config/tools/MomentumResolutionSettings_Default.yaml)

Key loader and settings types:

- [`include/io/analysis_samples.h`](../include/io/analysis_samples.h)
- [`include/io/Yaml_loader.h`](../include/io/Yaml_loader.h)
- [`include/Settings/EventSelectionSettings.h`](../include/Settings/EventSelectionSettings.h)
- [`include/Settings/AnalysisCutSettings.h`](../include/Settings/AnalysisCutSettings.h)
- [`include/Settings/AcceptanceMapsSettings.h`](../include/Settings/AcceptanceMapsSettings.h)
- [`include/Settings/MomentumResolutionSettings.h`](../include/Settings/MomentumResolutionSettings.h)

## Main Runtime Path

The current reconstructed analysis path is:

1. Parse CLI flags in [`include/io/reco_cli.h`](../include/io/reco_cli.h) and [`source/io/reco_cli.cpp`](../source/io/reco_cli.cpp).
2. Load the sample chain from YAML with [`include/io/analysis_samples.h`](../include/io/analysis_samples.h).
3. Dispatch by file type through [`include/AnalysisTools/DataAnalyzer.h`](../include/AnalysisTools/DataAnalyzer.h).
4. Run the main analysis in [`include/TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.h`](../include/TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.h) and [`source/TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.cpp`](../source/TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.cpp).

## Scripts

Operational scripts live under [`scripts/`](../scripts):

- Environment setup: [`scripts/environment/set_environment.csh`](../scripts/environment/set_environment.csh)
- Update helper: [`scripts/code_updater.sh`](../scripts/code_updater.sh)
- Build-and-run wrapper: [`scripts/build_and_run.csh`](../scripts/build_and_run.csh)

The repo root also keeps:

- [`run.csh`](../run.csh)
- [`update_only.sh`](../update_only.sh)
