# `analysis_samples`

Source file:

- [`include/io/analysis_samples.h`](../../../include/io/analysis_samples.h)

## Purpose

`analysis_samples` loads the reconstructed sample chain from YAML and keeps a compatibility layer for older code that still expects global vectors such as `RecoSamplePath_v`.

## Main Types

- `SampleChainEntry`
- `SampleChainConfig`

## Main Functions

- `LoadSampleChainConfigFromYaml(...)`
- `LoadSampleChainFromYaml(...)`
- `PopulateLegacySampleVectors(...)`
- `ClearSampleChain()`

## YAML Shape

The loader expects a top-level `samples` sequence. Each item supports keys such as:

- `label`
- `reco_sample_path`
- `recon_hipo_dir`
- `input_pattern`

Derived fields such as `PathToHipoFilesDir` and `InputHipoFiles` are refreshed automatically.

## Compatibility Layer

The namespace still exports global vectors and strings:

- `RecoSamplePath_v`
- `ReconHipoDir_v`
- `PathToHipoFilesDir_v`
- `InputHipoFiles_v`

That is how the current app wrapper in [`source/TwoNAnalyzer/RecoAnalyzer/RecoApp.cpp`](../../../source/TwoNAnalyzer/RecoAnalyzer/RecoApp.cpp) iterates over the selected samples.
