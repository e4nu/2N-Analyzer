# 2N-Analyzer-e4nu

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Environment and settings

The runtime setup is now driven mainly by YAML files under [`config/`](config), plus a small set of command-line switches parsed by [`include/io/reco_cli.h`](include/io/reco_cli.h) and implemented in [`source/io/reco_cli.cpp`](source/io/reco_cli.cpp).

- Sample selection:
  - The analyzer loads the sample chain from a YAML file through [`include/io/analysis_samples.h`](include/io/analysis_samples.h).
  - Default sample chain: [`config/samples/RecoSampleChain_Default.yaml`](config/samples/RecoSampleChain_Default.yaml).
  - Other prepared sample chains: [`config/samples/RecoSampleChain_Data.yaml`](config/samples/RecoSampleChain_Data.yaml), [`config/samples/RecoSampleChain_Simulation.yaml`](config/samples/RecoSampleChain_Simulation.yaml), [`config/samples/RecoSampleChain_Uniform.yaml`](config/samples/RecoSampleChain_Uniform.yaml).
  - At runtime this file can be overridden with `--sample-chain <path>`.

- Paths and output directories:
  - Global directory definitions live in [`config/io/CodeDirectories_Default.yaml`](config/io/CodeDirectories_Default.yaml).
  - [`include/Settings/setup/path_definitions.h`](include/Settings/setup/path_definitions.h) loads [`config/io/CodeDirectories.yaml`](config/io/CodeDirectories.yaml) when it exists, and otherwise falls back to the default file.
  - `custom_plots_path_prefix` is taken from that YAML and is used when naming output folders.
  - The CLI option `--directories <path>` exists, but the current analyzer path-loading code does not consume it yet.

- Analysis settings loaded from YAML at runtime:
  - Event selection: [`config/EventSelection/EventSelectionSettings_Default.yaml`](config/EventSelection/EventSelectionSettings_Default.yaml) -> [`include/Settings/EventSelectionSettings.h`](include/Settings/EventSelectionSettings.h).
  - Analysis cut switches: [`config/cuts/AnalysisCutSettings_Default.yaml`](config/cuts/AnalysisCutSettings_Default.yaml) -> [`include/Settings/AnalysisCutSettings.h`](include/Settings/AnalysisCutSettings.h).
  - Acceptance / weight-map settings: [`config/tools/AcceptanceMapsSettings_Default.yaml`](config/tools/AcceptanceMapsSettings_Default.yaml) -> [`include/Settings/AcceptanceMapsSettings.h`](include/Settings/AcceptanceMapsSettings.h).
  - Momentum-resolution settings: [`config/tools/MomentumResolutionSettings_Default.yaml`](config/tools/MomentumResolutionSettings_Default.yaml) -> [`include/Settings/MomentumResolutionSettings.h`](include/Settings/MomentumResolutionSettings.h).
  - These files are loaded in [`source/TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.cpp`](source/TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.cpp) and can be overridden with the matching CLI flags.

- Cut values:
  - A YAML file exists at [`config/cuts/CutValueManager_Default.yaml`](config/cuts/CutValueManager_Default.yaml), and the CLI exposes `--cut-values <path>`.
  - However, the current reconstructed analyzer still initializes [`include/Settings/CutValueManager.h`](include/Settings/CutValueManager.h) from code via `CutManager.UpdateCutManager(...)`; the YAML file is not yet wired into the active runtime path.

- Runtime-only CLI controls:
  - Cleanup flags, test mode, event limit, and sample selection are applied from the CLI in [`RecoAnalyzer::UpdateSettingsFromCLI`](source/TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.cpp).
  - Important examples: `--sample-index`, `--test-run`, `--n-events`, `--delete-png-files`, `--delete-root-files`, `--delete-txt-files`, `--clear-old-directories`, and `--skip-hipo-files-prob`.

## Scripts

- Environment script:
  - Location: [`scripts/environment/set_environment.csh`](scripts/environment/set_environment.csh).
  - Sets `DIR_2N_ANALYSIS_CODE`, detects whether the host is a JLab/ifarm machine, and exports `IFARM_RUN`.
- Code updater script:
  - Location: [`scripts/code_updater.sh`](scripts/code_updater.sh).
  - Invoked by [`update_only.sh`](update_only.sh).
  - Performs a destructive repo sync: `git clean -fxd -e build/ -e build`, `git reset --hard`, then `git pull`.
- Update only script:
  - Location: [`update_only.sh`](update_only.sh).
  - Thin wrapper that sources [`scripts/code_updater.sh`](scripts/code_updater.sh).
- Run the code script:
  - Location: [`run.csh`](run.csh).
  - Main entry point for the workflow. It sources [`update_only.sh`](update_only.sh), then delegates configure/build/run work to [`scripts/build_and_run.csh`](scripts/build_and_run.csh).
  - Script-level flags:
    - `--git-pull <true|false>`
    - `--build <true|false>`
    - `--run <true|false>`
  - Any other arguments are forwarded to the C++ executable.
  - Current caveat: although `run.csh` parses `--git-pull`, it still sources [`update_only.sh`](update_only.sh) unconditionally. In the current implementation, `--git-pull false` only disables the extra `git pull` in [`scripts/build_and_run.csh`](scripts/build_and_run.csh); it does not prevent the earlier destructive update step.
- Build/run helper script:
  - Location: [`scripts/build_and_run.csh`](scripts/build_and_run.csh).
  - Runs `build/apps/TwoNAnalyzerReco` and injects the selected YAML config files through CLI flags such as `--sample-chain`, `--event-selection`, `--analysis-cuts`, and `--momentum-resolution-config`.
  - Rebuilds only when build-relevant files changed since the last successful build.

## Code operation

The current workflow is driven by selecting the right YAML files, not by editing old setup headers.

- Cut studies:
  - No-cut / vertex-study stage:
    - Use [`config/cuts/AnalysisCutSettings_NO_CUTS.yaml`](config/cuts/AnalysisCutSettings_NO_CUTS.yaml).
    - This sets `apply_cuts: false`, which disables the full cut chain.
  - Electron-quality without chi2-driven nucleon stage:
    - Use [`config/cuts/AnalysisCutSettings_woChi2.yaml`](config/cuts/AnalysisCutSettings_woChi2.yaml).
    - This keeps the electron/preselection stages on, but sets `apply_chi2_cuts_1e_cut: false`, so nucleon cuts stay off.
  - Chi2-enabled but nucleon-cut extraction stage:
    - Use [`config/cuts/AnalysisCutSettings_noNC.yaml`](config/cuts/AnalysisCutSettings_noNC.yaml).
    - This keeps `apply_chi2_cuts_1e_cut: true` while `apply_nucleon_cuts: false`.
  - Full cut chain:
    - Use [`config/cuts/AnalysisCutSettings_Default.yaml`](config/cuts/AnalysisCutSettings_Default.yaml).

- Momentum-resolution workflow:
  - The prepared YAML presets live under [`config/tools/`](config/tools).
  - Presets currently in the tree:
    - Baseline / disabled: [`MomentumResolutionSettings_Default.yaml`](config/tools/MomentumResolutionSettings_Default.yaml)
    - momResS1 calculation: [`MomentumResolutionSettings_momResS1.yaml`](config/tools/MomentumResolutionSettings_momResS1.yaml)
    - momResS2 calculation: [`MomentumResolutionSettings_momResS2.yaml`](config/tools/MomentumResolutionSettings_momResS2.yaml)
    - momResS2 run mode: [`MomentumResolutionSettings_momResS2R.yaml`](config/tools/MomentumResolutionSettings_momResS2R.yaml)
  - The intended mode combinations come directly from [`include/Settings/MomentumResolutionSettings.h`](include/Settings/MomentumResolutionSettings.h):

  | Mode | `VaryingDelta` | `plot_and_fit_MomRes` | `Calculate_momResS2` | `Run_with_momResS2` |
  |------|----------------|-----------------------|----------------------|---------------------|
  | momResS0 / normal run | false | false | false | false |
  | momResS1 calculation | false | true | false | false |
  | momResS2 calculation | true | true | true | false |
  | momResS2 run | true | false | false | true |

  - `momRes_test` is still supported in [`include/Settings/MomentumResolutionSettings.h`](include/Settings/MomentumResolutionSettings.h) and is passed into the momentum-resolution code path.
  - The cut logic also enforces a few dependencies in [`include/Settings/AnalysisCutSettings.h`](include/Settings/AnalysisCutSettings.h):
    - momentum-resolution fits are disabled automatically on data
    - `Calculate_momResS2` and `Run_with_momResS2` cannot both be true
    - `Calculate_momResS2` requires `VaryingDelta = true`

- Running the executable:
  - You can launch through [`run.csh`](run.csh) and forward runtime overrides, for example:
    - `./run.csh --git-pull false --build false --run true --sample-chain config/samples/RecoSampleChain_Data.yaml`
    - `./run.csh --git-pull false --build false --run true --analysis-cuts config/cuts/AnalysisCutSettings_woChi2.yaml`
  - The forwarded flags are parsed by [`source/io/reco_cli.cpp`](source/io/reco_cli.cpp).

## Settings to check before each run

- The YAML files selected for the current run:
  - sample chain: [`config/samples/`](config/samples)
  - event selection: [`config/EventSelection/`](config/EventSelection)
  - analysis cuts: [`config/cuts/`](config/cuts)
  - momentum resolution: [`config/tools/`](config/tools)
- CLI runtime overrides:
  - review the available flags in [`include/io/reco_cli.h`](include/io/reco_cli.h) / [`source/io/reco_cli.cpp`](source/io/reco_cli.cpp)
  - especially `--sample-index`, `--test-run`, `--n-events`, `--skip-hipo-files-prob`, and the cleanup flags
- Path definitions and output naming:
  - [`include/Settings/setup/path_definitions.h`](include/Settings/setup/path_definitions.h)
  - [`config/io/CodeDirectories_Default.yaml`](config/io/CodeDirectories_Default.yaml)
- Debugging toggles:
  - [`include/Settings/setup/debugging.h`](include/Settings/setup/debugging.h)
- Zipping behavior:
  - [`include/Settings/setup/zipping.h`](include/Settings/setup/zipping.h)
- If you are using the wrapper scripts on a working tree with local changes:
  - do not run [`update_only.sh`](update_only.sh) or [`scripts/code_updater.sh`](scripts/code_updater.sh) unless you are willing to lose uncommitted tracked changes
