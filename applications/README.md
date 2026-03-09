
# Applications

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

This directory contains the top level analysis entry points of **2N-Analyzer-e4nu**.

The code here is intentionally thin: it wires together the framework classes (Settings, ExperimentParameters, RunParameters, cut settings, histogram wrappers, acceptance maps, resolutions, etc.), and then executes the main analysis loops.

## RecoAnalyzer

**Files:**
- `RecoAnalyzer.h`
- `RecoAnalyzer.cpp`

### Purpose
`RecoAnalyzer` is the reconstruction level analysis driver. It takes a reconstructed sample path and an input HIPO selection (directory + file list), configures the full analysis environment, and produces:

- a hierarchy of plot output directories
- ROOT histograms (via `hPlot1D`, `hPlot2D`, `hsPlots`) saved to a `.root` container
- a multi page PDF of selected plots
- acceptance maps (`AMaps`) and optional weight maps (`WMaps`)
- efficiency products (`DEfficiency`) and acceptance correction auxiliary outputs
- momentum resolution products (`MomentumResolution`) used for smearing/corrections

### High level flow (constructor)
The analysis is executed from the constructor:

1) **Code setup and input parsing**
   - Loads run wide configuration (`Settings`).
   - Builds `ExperimentParameters` from `RecoSamplePath` and `ReconHipoDir`.
   - Builds `RunParameters` (canonical `SampleName`, beam energy, flags like data vs simulation, etc.).

2) **Settings refresh**
   - Initializes `EventSelectionSettings`, `AcceptanceMapsSettings`, `MomentumResolutionSettings`.
   - Builds `AnalysisCutSettings` and synchronizes cut values through `CutValueManager`.
   - Applies custom naming so a single run can produce sample tagged outputs.

3) **Directory creation**
   - Creates a dedicated output tree through `Directories` so each plot category ends up in a predictable folder.

4) **Histogram containers and output sinks**
   - Creates:
     - `TList* plots` for `.root` output
     - a `std::vector<TObject*> HistoList` used by the PDF plotting utilities
   - Prepares output paths derived from the run directory name and/or `SampleName`.

5) **Plot selection**
   - A large set of booleans turns plot groups on and off.
   - Additional logic auto disables plot families when a prerequisite is off (for example, disabling TL plots when truth level is not requested).

6) **Optional cleanup of previous run outputs**
   - Deletes existing `.pdf` and/or `.root` files inside the run plot directory (and optionally `.txt` logs) before producing new outputs.

7) **Analysis objects**
   - Configures acceptance maps (`AMaps`) either in generation mode or in loading mode.
   - Configures efficiencies (`DEfficiency`).
   - Configures momentum resolution objects for neutrons and protons (`MomentumResolution`).
   - Configures optional debugging helpers in `ParticleID`.

8) **Histogram definitions and filling**
   - Defines all histograms with consistent binning and axis ranges.
   - During the event loop (later in the file), applies the selected cuts and fills the enabled histograms.

### Inputs
- `RecoSamplePath`: path that encodes the sample type, target, tune, beam energy bucket, and Q2 category (for GENIE samples).
- `ReconHipoDir`: directory that contains reconstructed HIPO files for that sample.
- `InputHipoFiles`: a string that points to the input HIPO(s). In practice, this is wired from `setup/codeSetup.h`.

### Outputs
All outputs are placed under the plots directory configured in `path_definitions::PathDefinitions` and are further grouped by the run directory name.

Typical products include:
- `*.root` file containing the `TList` with all histograms
- `*.pdf` file containing a selected subset of histograms for quick browsing
- per category subdirectories (cuts, kinematics, acceptance maps, resolution, etc.)
- optional text logs (skipped files list, event printouts)

### Notes
- `RecoAnalyzer` is designed to support running multiple samples in the same execution by using custom naming in `AnalysisCutSettings::CustomNamingRefresh`.
- Many plot groups depend on whether truth level quantities are requested (for example, TL efficiencies and some resolution workflows).

## TruthAnalyzer

**Files:**
- `TruthAnalyzer.h`
- `TruthAnalyzer.cpp`

### Purpose
`TruthAnalyzer` is a lightweight truth level analysis driver.

It derives from `gst` (GENIE truth tree interface) and runs the truth level event loop via `gst::Loop()` from its constructor.

### High level flow
- `TruthAnalyzer::TruthAnalyzer()` constructs the base `gst` and immediately calls `Loop()`.

### When to use
Use `TruthAnalyzer` when you want to analyze generator level information directly (GENIE truth), independent of detector reconstruction.

## Where the logic lives
Most of the analysis logic is implemented in the framework layer:

- settings and run identity: `Settings`, `ExperimentParameters`, `RunParameters`
- cuts: `AnalysisCutSettings`, `EventSelectionSettings`, `CutValueManager`
- histograms and plotting: `hPlot1D`, `hPlot2D`, `hsPlots`, draw and save namespaces
- acceptance and efficiency: `AMaps`, `DEfficiency`
- resolutions: `MomentumResolution`
- particle selection: `ParticleID`

The application classes in this directory mainly orchestrate these building blocks.

## Quick usage pattern

In the main program, construction triggers execution:

- Reconstruction analysis:
  - Instantiate `RecoAnalyzer(RecoSamplePath, ReconHipoDir, InputHipoFiles)`.

- Truth level analysis:
  - Instantiate `TruthAnalyzer()`.

The exact wiring of arguments is handled by the setup layer (`setup/codeSetup.h`) and the project build configuration.