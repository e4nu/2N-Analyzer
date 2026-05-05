# Simulation Samples

The current repository does not keep a separate `simulation_samples.h` helper.

## Current Behavior

Simulation samples are represented through the shared YAML sample-chain format and then classified by `ExperimentParameters`.

Relevant files:

- [`config/samples/`](../../../config/samples)
- [`include/io/analysis_samples.h`](../../../include/io/analysis_samples.h)
- [`include/AnalysisTools/ExperimentParameters.h`](../../../include/AnalysisTools/ExperimentParameters.h)

## Path-Based Inference

`ExperimentParameters` infers simulation metadata from the sample path, including:

- target
- GENIE tune
- beam-energy bucket
- Q2 label
- canonical sample name

That metadata feeds cut configuration, output naming, acceptance maps, and momentum-resolution inputs.
