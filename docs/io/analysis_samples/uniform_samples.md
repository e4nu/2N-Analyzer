# Uniform Samples

Uniform samples are handled through the same YAML sample-chain mechanism as all other reconstructed inputs.

## Why They Matter

Uniform samples are used by the acceptance-map workflow to generate:

- electron acceptance maps
- nucleon acceptance maps
- weight maps

## Relevant Code

- [`include/io/analysis_samples.h`](../../../include/io/analysis_samples.h)
- [`include/AnalysisTools/ExperimentParameters.h`](../../../include/AnalysisTools/ExperimentParameters.h)
- [`include/Settings/AcceptanceMapsSettings.h`](../../../include/Settings/AcceptanceMapsSettings.h)
- [`include/AnalysisTools/AMaps.h`](../../../include/AnalysisTools/AMaps.h)

`AcceptanceMapsSettings::UpdateAMapsSettingsByParameters(...)` explicitly checks the inferred sample name and disables incompatible map-generation modes when the selected sample is not one of the expected uniform forms.
