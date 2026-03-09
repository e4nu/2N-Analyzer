
# Reco analyzer settings

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

This directory contains small, mostly-POD configuration structs that act as the "switchboard" for what the reconstruction-level analysis does in a given run.

At a high level, the flow is:

1. `ExperimentParameters` parses the sample path and derives the canonical sample name, beam-energy bucket, target, data vs MC, etc.
2. `RunParameters` snapshots the derived values that other settings need.
3. The settings structs initialize to conservative defaults.
4. The main code calls the `RefreshSettings(...)` methods to enforce consistent combinations (for example, disabling incompatible modes).
5. `AnalysisCutSettings::SetClas12anaCuts(...)` applies the chosen cut configuration to `clas12ana` and populates the `CutValueManager` objects used for logging and plotting.

The structs here are designed to:

- Keep the main program readable by centralizing boolean switches.
- Auto-disable incompatible options at runtime (rather than silently doing the wrong thing).
- Provide a single place to control analysis modes like acceptance-map generation, truth-level calculations, and momentum-resolution workflows.

## Files and responsibilities

### `RunParameters.h`

`RunParameters` is the small "run snapshot" that most other settings consume.

**What it stores**

- `SampleName`, `VaryingSampleName` (derived by `ExperimentParameters`).
- `beamE` (numeric beam energy derived from the sample name).
- `Target`, `TargetPDG`.
- Flags: `isLocal`, `isMC`, `isData`, and beam-energy buckets (`is2GeVSample`, `is4GeVSample`, `is6GeVSample`).

**Typical usage**

Construct it once near the start of the run:

```cpp
ExperimentParameters Experiment(RecoSamplePath, ReconHipoDir);
RunParameters parameters(Experiment, RecoSamplePath, ReconHipoDir);
```

Then pass `parameters` to settings refresh functions.

### `EventSelectionSettings.h`

`EventSelectionSettings` controls which final states are analyzed and how truth-level (TL) information is used.

**Final-state toggles (Reco-level)**

- `calculate_1p`, `calculate_1n`, `calculate_2p`, `calculate_pFDpCD`, `calculate_nFDpCD`.

**Truth-level control**

- `calculate_truth_level`: master TL switch.
- `fill_TL_plots`: enable filling TL histograms.
- `Rec_wTL_ES`: apply TL event-selection rules to reco plots (force TL-style selection).
- `TL_plots_only_for_NC`: TL plots only after beta-fit (neutral-candidate stage).
- Efficiency helpers: `Eff_calc_with_one_reco_electron`, `Calc_eff_overlapping_FC`, `ZoomIn_On_mom_th_plots`, etc.

**FD neutrals**

- `Enable_FD_photons`: usually kept `false` to reduce RES/DIS background.
- `Enable_FD_neutrons`: can be auto-disabled depending on strategy.
- `ES_by_leading_FDneutron`: when `false`, `RefreshSettings()` disables FD-neutron handling by setting `Enable_FD_neutrons = false`.

**Key point**

`RefreshSettings()` enforces internal consistency for FD neutral handling. This avoids accidentally enabling FD neutrons when the analysis is not selecting by a leading FD neutron.

### `AcceptanceMapsSettings.h`

`AcceptanceMapsSettings` controls generation of acceptance maps (AMaps) and efficiency maps (WMaps).

**Main switches**

- `Generate_Electron_AMaps`: acceptance maps for electrons (used mainly for fiducial-cut studies).
- `Generate_Nucleon_AMaps`: acceptance maps for nucleons.
- `Generate_WMaps`: efficiency maps (currently produced together with the uniform-map workflows).
- `AMaps_calc_with_one_reco_electron`: when `true`, maps are built using a single reconstructed electron definition.

**Binning configuration**

- `P_e_bin_profile` and `P_nuc_bin_profile`: strings selecting pre-defined binning strategies.
- Slice controls: `Electron_single_slice_test`, `Nucleon_single_slice_test`, and `TestSlices`.
- Histogram discretization:
  - `NumberNucOfMomSlices`, `NumberElecOfMomSlices`
  - `HistElectronSliceNumOfXBins`, `HistNucSliceNumOfXBins`

**Runtime enforcement**

- `RefreshSettingsByParameters(const RunParameters&)`:
  - Forces all map generation off for data runs.
  - Ensures electron maps only run for samples whose name contains `Uniform_1e`.
  - Ensures nucleon maps only run for samples whose name contains `Uniform_ep` or `Uniform_en`.
  - Enforces mutual exclusion between electron and nucleon AMaps.

- `RefreshSettingsByEventSelection(const EventSelectionSettings&, bool& AMaps_plots, bool& WMaps_plots)`:
  - Disables WMaps if TL calculation is disabled.
  - Disables plotting flags when the corresponding maps are not being generated.

### `MomentumResolutionSettings.h`

`MomentumResolutionSettings` controls the momentum-resolution (momRes) workflow, including multi-stage resolution extraction and optional smearing/corrections.

**Core switches**

- `plot_and_fit_MomRes`: produce and fit momentum-resolution plots.
- `Calculate_momResS2`: calculate the stage-2 momRes variables.
- `Run_with_momResS2`: run smearing using momResS2 while correcting using momResS1.

**Slicing and modes**

- `DeltaSlices`: delta bin size (default `0.05`).
- `VaryingDelta`: whether delta changes with momentum.
- `ForceSmallpResLimits`: helper flag used for the first momResS1 pass.
- `SmearMode`, `CorrMode`: string selectors (for example `pol1_wKC`).

**Order constraints**

The comment block in the header documents the intended run order:

1. momResS1 calculation pass 1 (fixed delta)
2. momResS1 calculation pass 2 (varying delta)
3. momResS2 calculation
4. momResS2 run

`RefreshSettings()` enforces a key consistency rule:

- If `plot_and_fit_MomRes` is enabled together with either `Calculate_momResS2` or `Run_with_momResS2`, then `ForceSmallpResLimits` is disabled.

### `CutValueManager.h`

`CutValueManager` owns the numeric values that define cuts and the metadata needed to log and plot them.

The central object used here is `DSCuts`, which stores:

- A cut label (what variable), region, particle, and analysis channel tag.
- Mean/sigma when relevant.
- Lower/upper cut boundaries.

**What this struct contains**

- Electron-quality and PID cut containers:
  - `Nphe_cuts_FD`, `SF_cuts`, `P_cuts`, edge cuts (`PCAL_edge_cuts`, `DC_e_edge_cuts`, `DC_edge_cuts`).
  - Charged-particle chi2 containers for multiple species and regions (`Chi2_*_cuts_CD/FD`).
- Vertex cuts:
  - `Vz_cut`, `Vz_cut_FD`, `Vz_cut_CD` and `dVz_cuts`, `dVz_cuts_FD`, `dVz_cuts_CD`.
- Momentum-threshold cuts:
  - Reco thresholds: `e_mom_th`, `ph_mom_th`, `pip_mom_th`, `pim_mom_th`, plus nucleon thresholds (`p_mom_th`, `n_mom_th`, and FD/CD variants).
  - Electron threshold policy: `e_mom_th` defaults to `0.8 GeV/c`, following the project convention that reco-electron reconstruction below that is not trusted in CLAS12; this value is also reused as the truth-level electron momentum threshold.
  - Optional "no proton threshold" placeholder for efficiency studies: `no_p_mom_th`.
  - Truth-level mirrors: `TL_*_mom_cuts`.
- Beta-fit bookkeeping for neutron selection:
  - `Beta_max_cut_ABF_FD_n_from_ph`, `Beta_max_cut_ABF_FD_n_from_ph_apprax`.
  - `n_momentum_cuts_ABF_FD_n_from_ph`, `n_momentum_cuts_ABF_FD_n_from_ph_apprax`.
- Neutral-candidate veto bookkeeping:
  - `Neutron_veto_cut`, `ChargedECALveto_veto_cut`, `NeutralECALveto_veto_cut`.
- Geometry and monitoring cuts:
  - `ThetaFD`, `ThetaCD`, plus specialized versions used by AMaps.
  - Ghost-track handling windows (`Theta_*_2p`, `dphi_*_2p`, and `pFDpCD`/`nFDpCD` variants).
  - Reaction monitoring windows (`P_miss_1N_*`, `E_miss_1N_*`, `xB_cut`, `Theta_q_pCD_cut`).

**Constructor behavior**

`CutValueManager(ExperimentParameters&, const EventSelectionSettings&)` initializes vertex windows from `ExperimentParameters`, then selects nucleon momentum-threshold behavior based on `ESSettings.limless_mom_eff_plots`:

- If `limless_mom_eff_plots` is `true`, nucleon momentum thresholds are effectively removed.
- Otherwise, default thresholds are applied (protons in FD and CD can differ).

After that, the TL momentum cuts are set to match the chosen reco thresholds.

**Updating chi2 PID numbers**

`UpdateChargedParticleChi2Cuts(clas12ana&)` pulls mean and sigma values out of `clas12ana` and stores them in the relevant `DSCuts` objects for logging and downstream use.

**Updating chi2 histogram peak positions (Xmax)**

`FindChi2Xmax(...)` now takes the 1e-cut chi2 histograms as explicit `hPlot1D&` arguments (electron FD, then proton/kaon/pion/deuteron in CD+FD).  
This method no longer relies on out-of-scope identifiers from `RecoAnalyzer.cpp`.

It also includes a safety fallback for empty histograms: if no valid peak bin is found, the previous stored mean is kept.
Because the current workflow provides only an FD electron chi2 histogram at this stage, the method mirrors the FD electron peak into `Chi2_Electron_cuts_CD`.

Typical call site:

```cpp
CutManager.FindChi2Xmax(
    hChi2_Electron_1e_cut_FD,
    hChi2_Proton_1e_cut_CD, hChi2_Proton_1e_cut_FD,
    hChi2_Kplus_1e_cut_CD, hChi2_Kplus_1e_cut_FD,
    hChi2_Kminus_1e_cut_CD, hChi2_Kminus_1e_cut_FD,
    hChi2_piplus_1e_cut_CD, hChi2_piplus_1e_cut_FD,
    hChi2_piminus_1e_cut_CD, hChi2_piminus_1e_cut_FD,
    hChi2_deuteron_1e_cut_CD, hChi2_deuteron_1e_cut_FD
);
```

### `AnalysisCutSettings.h`

`AnalysisCutSettings` is the main "cut policy" struct. It decides which families of cuts are active, and it configures `clas12ana` accordingly.

It contains three layers of logic:

1. **User-facing switches**: booleans that enable or disable whole groups of cuts.
2. **Auto-refresh**: `RefreshSettings(...)` that forces consistent combinations based on run mode.
3. **Cut application**: `SetClas12anaCuts(...)` that calls the appropriate `clas12ana` setters and fills `CutValueManager` bookkeeping.

#### Cut families

- Master switches:
  - `apply_cuts`.
  - Special modes: `only_preselection_cuts`, `only_electron_quality_cuts`.

- Preselection (event-level) cuts:
  - Vertex cuts: `apply_Vz_e_cuts`, `apply_Vz_cuts`, `apply_dVz_cuts`.
  - DC fiducial: `apply_DC_e_fiducial_cuts`, `apply_DC_fiducial_cuts`.

- Electron quality:
  - `apply_Nphe_cut`, `apply_ECAL_SF_cuts`, `apply_ECAL_P_cuts`, `apply_ECAL_fiducial_cuts`, `apply_Electron_beta_cut`.

- PID chi2:
  - `apply_chi2_cuts_1e_cut` (controls whether charged-hadron PID cuts are applied through `clas12ana`).

- Analysis-specific nucleon and physics cuts:
  - `apply_nucleon_cuts` (enables nucleon selection and fitted neutron beta/momentum thresholds).
  - `apply_nucleon_physical_cuts` and sub-switches: CD edge/region, proton PID, ghost tracks, ECAL vetoes, beta-fit cuts, fiducial/kinematics, smearing/corrections.

- Reaction monitoring:
  - `apply_reaction_monitoring_cuts` and sub-windows for `P_miss`, `E_miss`, `xB`, and `theta(q,p_CD)`.

#### `RefreshSettings(...)` logic (most important rules)

`RefreshSettings(const RunParameters&, EventSelectionSettings&, AcceptanceMapsSettings&, MomentumResolutionSettings&)` performs the run-time consistency enforcement.

Key behaviors visible in the implementation:

- If `only_preselection_cuts` or `only_electron_quality_cuts` is enabled, `apply_cuts` is disabled and only that family is kept.
- For electron AMaps generation, `apply_cuts` is disabled to avoid PID/preselection bias in fiducial studies.
- For nucleon AMaps generation, `apply_cuts` is enabled but the cut-set is reshaped into a dedicated, consistent configuration (vertex strategy, DC strategy, ECAL strategy, and disabling chi2 PID).
- If cuts are globally disabled, dependent flags are shut off as well (for example chi2 and nucleon cuts).
- If nucleon physical cuts are disabled, all physics-cut sub-switches are disabled.
- Data runs disable TL and map-generation related workflows:
  - `ESSettings.calculate_truth_level = false`
  - `AMapsSettings.Generate_WMaps = false`
  - `MomResSettings.plot_and_fit_MomRes = false`
  - `MomResSettings.momRes_test = false`
- If TL is disabled, several TL-dependent flags are forced off:
  - `AMapsSettings.AMaps_calc_with_one_reco_electron = false`
  - `ESSettings.fill_TL_plots = false`
  - `ESSettings.Rec_wTL_ES = false`
- The momRes ordering constraints are enforced, and invalid combinations abort the run.

#### `SetClas12anaCuts(...)`

`SetClas12anaCuts(clas12ana& clasAna, CutValueManager& CutManager, const RunParameters& parameters, const EventSelectionSettings& ESSettings)` applies the chosen configuration into `clas12ana`.

Highlights:

- Loads parameter files for sampling-fraction and ECAL-P cuts when enabled.
- Enables or disables the corresponding `clas12ana` cut flags (`setEcalSFCuts`, `setEcalPCuts`, `setEcalEdgeCuts`, `setNpheCuts`, etc.).
- Controls PID cuts:
  - If `apply_chi2_cuts_1e_cut` is off, it loads a no-cuts parameter file and disables PID cuts.
  - If on, it loads the fitted PID-cuts file for the current sample and then calls `CutManager.UpdateChargedParticleChi2Cuts(clasAna)` to synchronize logged values.
- Applies vertex and vertex-correction windows based on the `CutValueManager` values (FD/CD/general).
- For nucleon cuts:
  - When disabled, it sets "no cut" placeholders for the beta-fit monitoring and momentum-cut bookkeeping.
  - When enabled, it loads nucleon-cut parameters, optionally uses beam-energy shortcuts for 2 GeV, otherwise pulls fitted cut values from `clas12ana`.

## Recommended initialization order

In the main analysis driver, a robust order is:

```cpp
// 1) Derived sample parameters
ExperimentParameters Experiment(RecoSamplePath, ReconHipoDir);
RunParameters parameters(Experiment, RecoSamplePath, ReconHipoDir);

// 2) Base settings
EventSelectionSettings ESSettings;
AcceptanceMapsSettings AMapsSettings;
MomentumResolutionSettings MomResSettings;
AnalysisCutSettings CutSettings;

// 3) First refresh pass
ESSettings.RefreshSettings();
AMapsSettings.RefreshSettingsByParameters(parameters);
CutSettings.RefreshSettings(parameters, ESSettings, AMapsSettings, MomResSettings);

// 4) Second refresh pass that depends on final cut/TL configuration
// (for example plotting flags in the caller)

// 5) Cut values and clas12ana configuration
CutValueManager CutManager(Experiment, ESSettings);
CutSettings.SetClas12anaCuts(clasAna, CutManager, parameters, ESSettings);
```

This mirrors what the code is doing conceptually: derive run identity, enforce settings consistency, then apply and log cut values.

## Notes and common pitfalls

- Electron and nucleon AMaps generation are mutually exclusive by design. If both are set, the code aborts.
- Several booleans are intentionally auto-overwritten by `RefreshSettings(...)`. If you think a flag is being "ignored", check the refresh logic first.
- For data runs, TL, AMaps/WMaps generation, and momentum-resolution studies are disabled in `AnalysisCutSettings::RefreshSettings(...)`.
- `AnalysisCutSettings` owns policy decisions. `CutValueManager` owns numbers and cut logging. Mixing their responsibilities tends to make debugging harder.
