# Analysis Framework

The active executable path is the reconstructed analysis workflow.

## Entry Points

- CLI parsing: [`include/io/reco_cli.h`](../include/io/reco_cli.h), [`source/io/reco_cli.cpp`](../source/io/reco_cli.cpp)
- Application wrapper: [`source/TwoNAnalyzer/RecoAnalyzer/RecoApp.cpp`](../source/TwoNAnalyzer/RecoAnalyzer/RecoApp.cpp)
- Main analyzer: [`include/TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.h`](../include/TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.h), [`source/TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.cpp`](../source/TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.cpp)

## Runtime Flow

1. Parse command-line options into `reco_cli::RecoCliOptions`.
2. Load a sample chain from YAML with `analysis_samples::LoadSampleChainFromYaml(...)`.
3. For each selected sample, infer the file type from the input pattern.
4. Dispatch through `DataAnalyzer`.
5. For HIPO input, construct `RecoAnalyzer`.
6. Inside `RecoAnalyzer`, load YAML settings, build output directories, construct a `HipoChain`, and run the event loop.

## Core Subsystems

- Sample metadata: [`AnalysisTools/ExperimentParameters.md`](AnalysisTools/ExperimentParameters.md)
- File loading and bad-file probing: [`AnalysisTools/HipoChainLoader.md`](AnalysisTools/HipoChainLoader.md)
- Detector and event cuts: [`AnalysisTools/ParticleID.md`](AnalysisTools/ParticleID.md), [`AnalysisTools/TLCuts.md`](AnalysisTools/TLCuts.md)
- Acceptance maps and weights: [`AnalysisTools/AMaps.md`](AnalysisTools/AMaps.md), [`AnalysisTools/DEfficiency.md`](AnalysisTools/DEfficiency.md)
- Momentum-resolution workflow: [`AnalysisTools/MomentumResolution.md`](AnalysisTools/MomentumResolution.md)
- Plot wrappers: [`AnalysisTools/hPlot1D.md`](AnalysisTools/hPlot1D.md), [`AnalysisTools/hPlot2D.md`](AnalysisTools/hPlot2D.md), [`AnalysisTools/hsPlots.md`](AnalysisTools/hsPlots.md)

## Configuration Sources

The framework expects runtime YAML under [`config/`](../config):

- `samples/`
- `EventSelection/`
- `cuts/`
- `tools/`
- `io/`

Those files are mapped into C++ settings structs via [`include/io/Yaml_loader.h`](../include/io/Yaml_loader.h).
