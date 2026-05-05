# Data Sample Note

There is no standalone `DataSample` class in the current repository layout.

## Current Replacements

Data-sample behavior is now split across:

- [`include/io/analysis_samples.h`](../../include/io/analysis_samples.h) for sample-chain YAML loading
- [`include/AnalysisTools/ExperimentParameters.h`](../../include/AnalysisTools/ExperimentParameters.h) for sample classification
- [`include/Settings/RunParameters.h`](../../include/Settings/RunParameters.h) for run-time flags derived from the chosen sample

## Why This Page Exists

Older documentation referred to dedicated sample-type headers and helper classes. The current tree instead uses:

- YAML sample-chain entries
- path-based inference in `ExperimentParameters`
- compatibility globals in `analysis_samples`

Use the pages under [`analysis_samples/`](analysis_samples/) and [`../AnalysisTools/ExperimentParameters.md`](../AnalysisTools/ExperimentParameters.md) for the active flow.
