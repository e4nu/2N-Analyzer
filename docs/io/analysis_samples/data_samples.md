# Data Samples

The current repository does not keep a separate `data_samples.h` helper.

## Current Behavior

Data samples are described through the shared sample-chain YAML format in:

- [`config/samples/`](../../../config/samples)

They are then identified at runtime by:

- [`include/AnalysisTools/ExperimentParameters.h`](../../../include/AnalysisTools/ExperimentParameters.h)

The path parser treats data samples as those under the `clas12/rg-m/production` naming convention.

## Relevant Runtime Effects

- `RunParameters::isData` becomes true
- truth-level and simulation-only workflows are disabled by settings updates
- `HipoChainLoader` can skip probing for data in `BuildFromList(...)`
