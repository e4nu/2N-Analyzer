
# Settings Helper Classes

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

This folder provides a small utility layer that standardizes how the analysis builds output directories for plots and log files.

It contains two classes:

1. **CustomPlotsDirectory**: builds a deterministic, human readable run folder name based on which cut blocks are enabled.
2. **Settings**: a thin extension point meant to hold analysis specific rules that can automatically disable or adjust toggles.

The intent is that *every run* can be reproduced and its outputs can be located just by looking at the directory name.

## CustomPlotsDirectory

### Purpose

`CustomPlotsDirectory` generates:

- a run directory name (`Run_dir_name`) that encodes the enabled cut configuration
- a plot output folder (`Plots_path`)
- a per run text log path (`Plots_log_save_Directory`)
- a per run skipped files list path (`Skipped_files_list_save_Directory`)

This allows running multiple samples and multiple cut configurations without overwriting outputs.


### Key concepts

#### Stages

The run directory name always starts with the **sample name** and a **stage prefix**:

- `Stage0_prefix = "_S00_NO_CUTS"`  
  Used when `apply_cuts == false`.

- `Stage1_prefix = "_S01ACwoChi2"`  
  Used when `apply_cuts == true` and `apply_chi2_cuts_1e_cut == false`.

- `Stage2_prefix = "_S02AC"`  
  Used when `apply_cuts == true`, `apply_chi2_cuts_1e_cut == true`, and `apply_nucleon_cuts == false`.

- `Stage3_prefix = "_S03AC"`  
  Used when `apply_cuts == true`, `apply_chi2_cuts_1e_cut == true`, and `apply_nucleon_cuts == true`.

These stage prefixes capture the broad analysis mode. Everything else is appended as **status tokens**.

#### Custom cut naming switch

The directory building can be enabled or disabled via:

- `Custom_cuts_naming` (default: `true`)

If `Custom_cuts_naming == false`, the getter methods intentionally do not override the caller provided paths.


### Public API

#### `ConfigureStatuses(...)`

This function computes a set of short status strings that describe the configuration.

Internally it builds two concatenated strings:

- `Added_PreStatuses = Clas12ana_particles_status + Preselection_Cuts_Status + Electron_Quality_Cuts_Status`
- `Added_Statuses = Preselection_Cuts_Status + Electron_Quality_Cuts_Status + Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status + PSmearing_Status + FiducialCuts_Status + KinCuts_Status + KinWei_Status + Additional_Status`

Those are later appended to the stage prefix when constructing the final run directory name.

The input booleans are treated as feature toggles and translated into compact tokens, for example:

- `clas12ana_particles == false` adds `"_no_clas12ana"`
- `only_preselection_cuts == true` adds `"_Preselection_Cuts"`
- `only_electron_quality_cuts == true` adds `"_eQC"`
- `apply_nucleon_cuts` becomes either `"NC"` or `"noNC"`
- `Enable_FD_photons` becomes `"_wFDph"` (or `"wFDph"` when nucleon cuts are disabled)
- nucleon smear and correct becomes `"_wNSaC"`
- fiducial cuts become `"_wFC"`
- kinematics:
  - cuts only: `"_wKC"`
  - weights only: `"_wKW"`
  - both: `"wKC" + "_wKW"` (note the missing underscore in `wKC`)
- additional flags (`Generate_AMaps`, `plot_and_fit_MomRes`, `VaryingDelta`, `Calculate_momResS2`, `Run_in_momResS2`, `nRes_test`) select one of multiple `Additional_Status` tokens such as `"_AMaps"`, `"_momResSS"`, `"_momResS1"`, `"_momResS2"`, `"_momResS2R"`, and their `T` test variants.

The intention is that the folder name itself acts as a compact run configuration record.

#### `SetPaths(WorkingDirectory, SampleName, plots_path, apply_cuts, apply_chi2_cuts_1e_cut, apply_nucleon_cuts)`

This function constructs the final paths based on:

- whether the caller uses the default output folder (`plots_path == WorkingDirectory + "00_plots"`)
- the stage logic described above
- the status strings built by `ConfigureStatuses()`
- an optional global prefix `Global_Run_Status` (defaults to `path_definitions::custom_plots_path_prefix`)

Outputs:

- `Run_dir_name` is set to the constructed run string.
- `Plots_path` becomes either:
  - `WorkingDirectory + run` (default output mode), or
  - `plots_path + "/" + run` (custom output mode)
- `Plots_log_save_Directory` becomes `Plots_path + "/Run_log_" + run + ".txt"`
- `Skipped_files_list_save_Directory` becomes `Plots_path + "/Skipped_files_list_" + run + ".txt"`  
  This is currently only set in the default output mode path branch.


### Typical usage pattern

1. Create the object.
2. Optionally call `SetCustomCutsNaming(false)` if you want the caller to manage paths directly.
3. Call `ConfigureStatuses(...)` with your analysis toggles.
4. Call `SetPaths(...)` to generate folder and log paths.
5. Use `GetPlotsPath(...)` and the log getters to retrieve the computed values.

Example (schematic):

```cpp
CustomPlotsDirectory out;

out.ConfigureStatuses(/* apply_cuts = */ true,
                     /* clas12ana_particles = */ true,
                     /* only_preselection_cuts = */ false,
                     /* apply_chi2_cuts_1e_cut = */ true,
                     /* only_electron_quality_cuts = */ false,
                     /* apply_nucleon_cuts = */ true,
                     /* Enable_FD_photons = */ true,
                     /* apply_nucleon_SmearAndCorr = */ true,
                     /* apply_kinematical_cuts = */ true,
                     /* apply_kinematical_weights = */ false,
                     /* apply_fiducial_cuts = */ true,
                     /* Generate_AMaps = */ false,
                     /* plot_and_fit_MomRes = */ false,
                     /* VaryingDelta = */ false,
                     /* Calculate_momResS2 = */ false,
                     /* Run_in_momResS2 = */ false,
                     /* nRes_test = */ false,
                     /* Rec_wTL_ES = */ false,
                     /* ZoomIn_On_mom_th_plots = */ false);

out.SetPaths(WorkingDirectory, SampleName, plots_path,
             /* apply_cuts = */ true,
             /* apply_chi2_cuts_1e_cut = */ true,
             /* apply_nucleon_cuts = */ true);

std::string outDir;
out.GetPlotsPath(outDir);
```


### Notes and pitfalls

- `GetPlotsPath(...)`, `GetPlotsLogSaveDirectory(...)`, and `GetSkippedHipoFilesLogSaveDirectory(...)` only write to their output arguments if `Custom_cuts_naming == true`.
- `SetPaths(...)` compares strings to detect the default output folder (`plots_path == WorkingDirectory + "00_plots"`). Make sure `WorkingDirectory` includes the expected trailing slash convention used elsewhere in the project.
- Some status tokens intentionally differ depending on whether nucleon cuts are enabled (for example `FD_photons_Status` sometimes has a leading underscore and sometimes does not). This affects the final run name.


## Settings

### Purpose

`Settings` inherits from `CustomPlotsDirectory` and is meant to centralize analysis wide configuration logic.

Right now it provides:

- a default constructor
- `AutoDisableVariables()` as an extension point

### Intended use

The idea is to keep policy logic in one place. For example, `AutoDisableVariables()` could implement rules such as:

- if only preselection cuts are requested, disable the downstream cut toggles
- if a mode requires nucleon cuts, enforce `apply_nucleon_cuts = true`
- if a mode runs momentum resolution steps, enforce compatible combinations of `VaryingDelta`, `Calculate_momResS2`, `Run_in_momResS2`, and `nRes_test`

The exact rule set is analysis specific and should be implemented so that it does not silently disable required physics selections.


### Relationship to CustomPlotsDirectory

Since `Settings` is a derived class, it can:

- call `ConfigureStatuses(...)` and `SetPaths(...)`
- expose the same `Get*` accessors for the computed directories

This makes it convenient to build consistent output folder names while also enforcing analysis rules.