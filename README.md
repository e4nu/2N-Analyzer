# 2N-Analyzer-e4nu

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Environment and settings

To run over samples, select them in the sample loader:

- Main sample loader:
  - [`analysis_samples.h`](framework/namespaces/setup/analysis_samples/analysis_samples.h).

- Sample loaders by type:
  - Simulation: [`simulation_samples.h`](framework/namespaces/setup/analysis_samples/simulation_samples.h).
  - Data: [`data_samples.h`](framework/namespaces/setup/analysis_samples/data_samples.h).
  - Uniform: [`uniform_samples.h`](framework/namespaces/setup/analysis_samples/uniform_samples.h).

- Output directory paths are defined in:
  - [`framework/namespaces/setup/path_definitions.h`](framework/namespaces/setup/path_definitions.h).
  - Can be used to define a custom plots-path prefix by changing `custom_plots_path_prefix`.

- Code settings:
  - Code run parameters: [`RunParameters.h`](framework/structures/RecoAnalyzerSettings/RunParameters.h).
    - **Note:** in the future, this should be discarded and merged into the [`ExperimentParameters` class](framework/classes/ExperimentParameters/ExperimentParameters.h).
  - Analysis cuts application ON/OFF switches: [`AnalysisCutSettings.h`](framework/structures/RecoAnalyzerSettings/AnalysisCutSettings.h).
  - Analysis cuts values: [`CutValueManager.h`](framework/structures/RecoAnalyzerSettings/CutValueManager.h).
  - Acceptance and weight maps generation settings: [`AcceptanceMapsSettings.h`](framework/structures/RecoAnalyzerSettings/AcceptanceMapsSettings.h).
  - Event selection setting: [`EventSelectionSettings.h`](framework/structures/RecoAnalyzerSettings/EventSelectionSettings.h).
    - Used to disable or enable final-state analyses.
    - Also used to set up and plot efficiency plots.
  - FD nucleon momentum resolution setting: [`MomentumResolutionSettings.h`](framework/structures/RecoAnalyzerSettings/MomentumResolutionSettings.h).

## Scripts

- Environment script:
  - Location: [`framework/scripts/environment/set_environment.csh`](framework/scripts/environment/set_environment.csh).
- Code updater script:
  - Location: [`code_updater.sh`](framework/scripts/code_updater.sh).
  - Pulls updates from git. Clears any changes in the project's dir.
- Update only script:
  - Location: [`update_only.sh`](update_only.sh).
  - Used to pull updates without running the code.
- Run the code script:
  - Location: [`run.csh`](run.csh)
  - Used to launch the code.

## Code operation

- **Step 0:**
  - Set only `apply_cuts = false`.
  - Disables all cuts. This lets you inspect the vertex and vertex-correlation distributions in order to derive $`V_{z}`$ and $`dV_{z}`$ cuts.
  - **Note:** this stage might not be relevant with the latest [`clas12ana`](framework/classes/clas12ana/clas12ana.h).

- **Step 1:**
  - Set only `apply_chi2_cuts_1e_cut = false`.
  - Disables all nucleon cuts ($`\chi^{2}_{FD/CD}`$/PID/physical), but keeps preselection and electron-quality cuts. At this stage, the code automatically fits and extracts $`\chi^{2}_{FD/CD}`$ cuts.

- **Step 2:**
  - Set only `apply_nucleon_cuts = false`.
  - Disables PID and physical nucleon cuts but keeps the rest. At this stage, the code automatically extracts nucleon-specific cuts.
  - **Note:** this stage might not be relevant with the latest [`clas12ana`](framework/classes/clas12ana/clas12ana.h).

- **Step 3:**
  - Momentum resolution calculation using the [`MomentumResolution` class](framework/classes/MomentumResolution/MomentumResolution.h).
  - To run momentum resolution calculations, set the following settings:
  
  | Step | Mode                                       | `VaryingDelta` | `plot_and_fit_MomRes` | `Calculate_momResS2` | `Run_with_momResS2` |
  |------|--------------------------------------------|----------------|-----------------------|----------------------|---------------------|
  | 1a   | momResS1 calculation 1                     | false          | true                  | false                | false               |
  | 1b   | momResS1 calculation 1, stacked mom slices | true           | true                  | false                | false               |
  | 3    | momResS2 calculation                       | true           | true                  | true                 | false               |
  | 4    | momResS2 run                               | true           | false                 | false                | true                |
  
  - Use `momRes_test` to run the calculation without editing the files in the `data/NeutronResolution` directory. This option is not relevant for runs on ifarm if [`update_only.sh`](update_only.sh) is being used.

- **Step 4:**
  - Run the analysis with the desired cuts, event selections, and momentum-resolution inputs enabled.

## Settings to check before each run

- `TestRun` and `Limiter` in [`RecoAnalyzer.h`](apps/RecoAnalyzer.h).
- [`path_definitions.h`](framework/namespaces/setup/path_definitions.h).
- [`analysis_samples.h`](framework/namespaces/setup/analysis_samples/analysis_samples.h).
- [`AnalysisCutSettings.h`](framework/structures/RecoAnalyzerSettings/AnalysisCutSettings.h).
- [`EventSelectionSettings.h`](framework/structures/RecoAnalyzerSettings/EventSelectionSettings.h).
- [`MomentumResolutionSettings.h`](framework/structures/RecoAnalyzerSettings/MomentumResolutionSettings.h).
- [`debugging_settings.h`](framework/namespaces/settings/debugging_settings.h).
- [`zipping_settings.h`](framework/namespaces/settings/zipping_settings.h).
