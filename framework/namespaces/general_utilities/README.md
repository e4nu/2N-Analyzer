# General utilities

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

This directory contains small, mostly header-only namespaces that are shared across the analysis framework. The goal is to keep these utilities broadly reusable and avoid circular dependencies between higher-level classes.

A common pattern in this repository is to keep functionality grouped by *purpose* into namespaces (rather than monolithic classes), and then expose a convenient “umbrella include” via `utilities.h`.

## How this folder is meant to be used

- Include **individual headers** when you only need a specific utility.
- Include **`utilities.h`** when you want the full analysis utility stack (ROOT + YAML loader + math/physics helpers + histogram helpers).

Because some components are implemented as header-only (including a few `.cpp` files that are intentionally included as implementation headers), be careful to include them **exactly once per translation unit** and avoid mixing different include orders across compilation units.

---

## Overview of provided namespaces

### `basic_tools` (`basic_tools.h`)

Lowest-level helper namespace intended to be safe to use essentially everywhere in the project.

It provides four main groups of functionality:

- **Terminal print / formatting helpers**
  - `PrintLine(...)` prints a plain separator line.
  - `PrintLineColored(...)` prints the same kind of separator but wrapped in terminal color codes.
  - `PrintTitleBlock(...)` prints a large title block for major execution stages.
  - `PrintSectionHeader(...)` prints a lighter section header for subsections.
  - `PrintBanner(...)` prints a centered banner with decorative borders.
  - `PrintAlignedValue(...)` and `PrintAlignedValueColored(...)` print `label: value` pairs with alignment.
  - `PrintWrappedLabelValue(...)` and `PrintWrappedLabelValueColored(...)` print `label: value` pairs while wrapping long values over multiple lines.
  - `PrintMessage(...)` prints a standalone colored message.
  - `ExitWithError(...)` prints a formatted fatal error and terminates.
  - `PrintWarning(...)` prints a formatted warning and returns.

- **Basic conversions and string helpers**
  - `quit()` exits with failure.
  - `BoolToChar(bool)` returns a string literal `"true"` or `"false"`.
  - `BoolToString(bool)` returns an owning `std::string` version.
  - `ToStringWithPrecision(value, n)` formats a value with fixed decimal precision.
  - `FindSubstring(haystack, needle)` performs a simple substring search.
  - `ReplaceSubstring(input, toReplace, replaceWith)` replaces only the first occurrence.
  - `trim(str)` removes leading and trailing spaces/tabs.
  - `splitVarAndUnits(input)` splits labels like `Momentum [GeV/c]` into `{ "Momentum", "[GeV/c]" }`.

- **Beam-energy parsing helpers**
  - `GetBeamEnergyFromString(sn)` maps known sample-name patterns such as `5986MeV`, `4029MeV`, `2070MeV`, `6GeV`, `4GeV`, and `2GeV` to their numeric beam energies.
  - `GetBeamEnergyFromDouble(BeamE)` converts approximately 6, 4, or 2 GeV beam energies back to canonical string tags.

- **Environment / filesystem / SSH helpers**
  - `fileExists(path)` checks whether a file can be opened.
  - `GetRemoteIPFromSSHConnection(ssh_connection)` extracts the remote IP from the `SSH_CONNECTION` environment variable.
  - `ResolveHostNameFromIP(remote_ip, host_name)` performs reverse DNS lookup.
  - `CheckSSHConnection()` reports whether the code is running under SSH.
  - `CheckSSHConnectionAndHost()` also prints the parsed remote IP and tries to resolve the host name.
  - `GetSSHHostName()` returns the resolved host name directly.
  - `GetCurrentDirectory()` returns the current working directory.

- **HIPO-chain logging helper**
  - `LogSkippedHipoFiles(...)` writes a small report listing skipped HIPO files, together with chain statistics.

Design notes:

- `basic_tools` is intentionally kept dependency-light so that it can sit at the bottom of the utility stack.
- It should not depend on higher-level analysis code.
- In practice, its only non-standard-library dependencies here are `environment.h` and `TString.h`.

---

### `constants` (`constants.h`)

Central place for physical constants and a few CLAS12-specific defaults.

Includes:

- Particle masses in GeV: `m_n`, `m_p`, `m_e`, `m_pizero`, etc.
- Speed of light: `c` in cm/ns.
- PDG IDs for common particles.
- CLAS12 theta ranges implemented as `DSCuts` objects:
  - `ThetaFD` (FD range)
  - `ThetaCD` (CD range)

---

### `Yaml_loader` (`Yaml_loader.h`, `Yaml_loader.cpp`)

Loads a `DataSample` struct from a YAML configuration file.

Entry point:

- `DataSample LoadFromYamlFile(const std::string& yaml_path);`

Behavior:

- Parses YAML using `yaml-cpp`.
- Accepts multiple key spellings for backward compatibility (for example `SampleFilesTopDir` or `sample_file_path`).
- Performs basic validation after loading:
  - `IsSimulationSample` and `IsDataSample` cannot both be true or both be false.
  - Only one of `BeamAt2GeV`, `BeamAt4GeV`, `BeamAt6GeV` may be true.

Notes and conventions:

- `Yaml_loader.cpp` is used as an **implementation header** (it is included from `utilities.h`).
  - This makes the loader effectively header-only in the current structure.
  - If you later convert this to a normal compiled `.cpp`, update includes accordingly.

Minimal example:

```cpp
#include "framework/namespaces/general_utilities/Yaml_loader/Yaml_loader.h"

auto sample = Yaml_loader::LoadFromYamlFile("config/sample.yaml");
std::cout << sample.SampleName << std::endl;
```

---

### `data_processor` (`data_processor.h`)

String-parsing helpers for labeling plots and deriving human-readable names.

Highlights:

- `GetFS(Source)` derives a final-state label from a string by matching known patterns.
- `GetParticleName(Source, PluralParticles)` returns `Electron`, `Proton`, `Neutron`, `Photon`, etc. based on pattern matching.

These functions are used heavily for:

- histogram naming conventions
- automatic titles and annotations

---

### `lists` (`lists.h`)

Central map of variable metadata used for labeling histograms.

- `VariableNames` maps an internal variable key to:
  - `VarName`  (canonical name)
  - `VarLabel` (ROOT TLatex label)
  - `VarDim`   (units string)

This enables consistent axis labeling across the project.

---

### `analysis_physics` (`analysis_physics.h`)

Kinematic helpers for reconstructed or truth-level analysis.

Includes functions such as:

- `CalcQ2(Beam_4v, P_e_4v)`
- `CalcxB(Beam_4v, P_e_4v, omega, Nuc_mass)`
- `CalcW(omega, q_3v, Nuc_mass)`
- `CalcPmiss1N3v(P_nucL_3v, q_3v)`
- `CalcEmiss1N(omega, E_nucL, mass_nucL)`
- `CalcPmiss2N3v(P_nucL_3v, P_nucR_3v, q_3v)`
- `CalcEmiss2N(omega, E_nucL, E_nucR, mass_nucL)`

All functions follow the same pattern:

- Inputs are `TLorentzVector` and `TVector3` objects.
- Mass defaults use `constants::m_p` unless specified.

---

### `analysis_math` (`analysis_math.h`)

Math helpers that are used throughout reconstruction and plotting.

Provides:

- Degree/radian conversion: `RadToDeg`, `DegToRad`.
- Common angle computations: `CalcTheta_deg`, `CalcPhi_deg`, etc.
- Misc helpers like `RadCalc`.
- Experiment-specific helpers such as `GetPhi_e(OutPutFolder, phi_N)`.

Polynomial solver integration:

- `analysis_math.h` includes `poly_solver.cpp` to expose polynomial root solvers.
- The solver code lives in `analysis_math/poly_solver.h` and `analysis_math/poly_solver.cpp`.

Important note on include guards:

- `poly_solver.h` currently uses `#ifdef POLY_SOLVER_H` rather than the usual `#ifndef` pattern.
  - This means its declarations are *not* active unless `POLY_SOLVER_H` is defined before including the header.
  - In the current setup, `analysis_math.h` includes the **implementation** via `poly_solver.cpp`, which itself includes `poly_solver.h`.
  - If you later want to use `poly_solver.h` directly, fix the guard to the conventional `#ifndef POLY_SOLVER_H` form.

---

### `variable_correctors` (`variable_correctors.h`)

Helpers for extracting peak positions from histograms.

Main tools:

- `GetPeakFromHighestBin(TH1D*)`
  - Returns the bin center at the maximum bin and estimates uncertainty as half a bin width.

- `FitPeakToGaussian(TH1D*, std::vector<double> fitLimits = {})`
  - Fits a Gaussian in a chosen range.
  - If no range is provided, it infers a range based on the histogram name and peak position.
  - Returns the fitted mean and its uncertainty.

Notes:

- The fit function is created with `new TF1(...)` and attached to the histogram.
- The code clears and replaces the histogram function list, so it will overwrite previously attached fit functions.

---

### `reco_analysis_functions` (`reco_analysis_functions.h`)

Reconstruction-level helper namespace for working directly with `clas12::region_part_ptr` objects and their detector-bank content.

The namespace groups together several kinds of reusable analysis helpers:

- **Neutral kinematics utilities**
  - `GetFDNeutronP(...)` returns the momentum assigned to an FD neutral candidate.
  - `CalcPathnFD(...)` obtains the neutral path length, either from geometry or from the stored bank value.
  - `CalcToFnFD(...)` obtains the neutral timing quantity, either relative to the event start time or from the stored bank value.
  - `CalcPnFD(...)` optionally recomputes a neutron-mass momentum proxy for photon-tagged neutral candidates.

- **Detector hit-pattern helpers**
  - `CheckForNeutralFDECALHits(...)` fills booleans describing whether a neutral hit PCAL, ECIN, or ECOUT, and selects a reference ECAL layer.
  - `CheckForECALHits(...)` provides both an output-argument wrapper and a tuple-returning form for ECAL hit checks.
  - `CheckForFTOFHits(...)` returns whether a particle hit FTOF1A, FTOF1B, or FTOF2.

- **Standard reconstruction-quality helpers**
  - `checkEcalDiagCuts(...)` applies the usual electron ECAL diagonal sampling-fraction cut.
  - `DCEdgeCuts(...)` applies region-by-region drift-chamber edge cuts for charged tracks.
  - `fillDCdebug(...)` fills drift-chamber debug histograms using stored trajectory coordinates.

- **Neutral-veto helpers**
  - `CalcSolenoidPhiShift(...)` estimates the azimuthal rotation induced by the solenoidal field.
  - `GetShiftedUnitMomentum(...)` builds a solenoid-shifted charged-particle reference vector at the neutral-hit radius.
  - `NeutronECAL_Cut_Veto(...)` preserves the legacy charged-proximity veto used in older studies.
  - `ChargedECALveto(...)` is the main charged-particle ECAL proximity veto for neutral candidates.
  - `NeutralECALveto(...)` applies the solenoid-corrected shifted-track proximity veto used to suppress neutral candidates aligned with charged-track radiation.

Design notes:

- These helpers are intentionally implemented as free functions because they are lightweight and stateless.
- Several functions preserve historical analysis conventions for cross-checking older studies against newer implementations.
- The veto helpers combine direct detector-hit information with simple geometric projections, so they should be interpreted as analysis-level reconstruction tools rather than full detector-transport models.

---

### `histogram_functions` (`histogram_functions.h`)

Helpers for filling histograms consistently by interaction type and for filling `hsPlots` objects.

Main patterns:

- `FillByInt1D(...)` and `FillByInt2D(...)`
  - Always fill the inclusive histogram.
  - Additionally fill one of QEL/MEC/RES/DIS based on boolean flags.

- `FillByInthsPlots(...)`
  - Same logic as above but works on `hsPlots` (your sliced-histogram manager).
  - Supports both `hsPlots::TH1D_TYPE` and `hsPlots::TH2D_TYPE`.

---

### `utilities` (`utilities.h`)

Umbrella header that pulls together the common utilities used throughout the project.

It includes:

- ROOT fundamentals (TFile, TH1, TLorentzVector, etc.)
- YAML loader (`Yaml_loader/Yaml_loader.cpp`)
- math/physics helpers (`analysis_math`, `analysis_physics`)
- basic tools, constants, lists
- reco helpers and histogram helpers

It also provides additional small utility functions such as:

- `ConfigRegion(histName)` which infers FD/CD from a histogram name.
- `ConfigSampleType(sampleName)` which infers simulation vs data from the sample name.
- `GetHistogramFromVector(HistoList, key, option)` to find histograms by name or title.
- `FillHistogramByProcess(...)` which fills QEL/MEC/RES/DIS histograms.

Because `utilities.h` includes a lot, prefer it for:

- analysis applications
- macros
- plotting scripts

and prefer smaller includes for:

- library code
- unit tests
- code that should compile fast

---

## Practical tips

- If you see multiple-definition linker errors, check whether an implementation `.cpp` is being included from more than one place.
- Keep `basic_tools` dependency-free to avoid circular include chains.
- When adding new variables, update `lists::VariableNames` so labels stay consistent.
- When adding new YAML keys, add them as aliases in the loader rather than breaking old configs.