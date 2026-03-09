
# The `ExperimentParameters` class

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## What this class does

`ExperimentParameters` is the configuration backbone of the 2N-Analyzer framework.
It derives from `TargetParameters` and turns a reconstructed-sample path (`RecoSamplePath`) into a consistent set of analysis settings.

Given a sample path (and optionally a run directory label), the class configures:

- **Target**: `H1`, `D2`, `C12` (including `Cx4` naming), `Ar40`, or `Uniform`
- **Sample type**: GENIE simulation, farm data, or uniform simulation
- **GENIE tune** (simulation only): `G18` or `SuSa`
- **Beam energy**: bucket (2, 4, 6 GeV) and numeric value
- **Q² category** (simulation only): `Q2_0_02`, `Q2_0_25`, `Q2_0_40`
- **Canonical name**: `SampleName` used throughout the analysis
- **Map selector name**: `VaryingSampleName` for acceptance maps, weights, and resolution inputs
- **Vertex selections**: `Vz` and `dVz` cut objects (global/FD/CD)
- **I/O wiring**: helper methods to populate a `HipoChain`

The design goal is to avoid duplicating sample logic across the codebase.
If directory naming conventions change, the intent is to update parsing rules here rather than in many analysis modules.

## Constructor and configuration pipeline

Constructor signature:

```cpp
ExperimentParameters(const std::string& RecoSamplePath,
                     const std::string& ReconHipoDir,
                     const std::string& HipoFilesPrefix = pd::HipoFilesPrefix);
```

The constructor configures the object by calling a sequence of mutator methods:

1. `ConfigSampleTarget()`
2. `ConfigSampleType()`
3. `ConfigGENIETune()`
4. `ConfigBeamEnergy()`
5. `ConfigQ2Cut()`
6. `ConfigSampleName()`
7. `ConfigureVaryingSampleName()`
8. `ConfigureVz_cuts()`
9. `ConfiguredVz_cuts()`

It also classifies whether the sample is considered **local** or **on ifarm** using a simple path heuristic (checks for "volatile" or "clas12/rg-m/production").

## How path parsing works

All configuration is based on substring matching in `RecoSamplePath`.
The parsing assumes the project’s directory naming conventions are stable.

### Target inference

Configured by `ConfigSampleTarget`.

Matched tokens:

- Hydrogen: `/H1/`, `/H/`, `/H1_`
- Deuterium: `/D2/`, `/D/`, `/D2_`
- Carbon: `/C12/`, `/C/`, `/C12_`, `/Cx4/`, `/Cx4_` (mapped to `C12`)
- Argon: `/Ar40/`, `/Ar/`, `/Ar40_`
- Uniform: `/Uniform`

The method also sets `TargetParameters` fields such as PDG code and A/Z.

### Sample type inference

Configured by `ConfigSampleType`.

Matched tokens:

- GENIE simulation: `GENIE_Reco_Samples`
- Farm data: `clas12/rg-m/production`
- Uniform simulation: `Uniform`

### GENIE tune inference (simulation only)

Configured by `ConfigGENIETune`.

Matched tokens:

- `G18` → `G18_TUNE` and label `G18`
- `GEM21` → `SUSA_TUNE` and label `SuSa`

### Beam energy inference

Configured by `ConfigBeamEnergy`.

Matched tokens (bucket and value):

- `2070` or `2GeV` → 2 GeV bucket, `SampleBeamEnergyStr = 2070MeV`, `BeamEnergyValue = 2.07052`
- `4029` or `4GeV` → 4 GeV bucket, `SampleBeamEnergyStr = 4029MeV`, `BeamEnergyValue = 4.02962`
- `5986` or `6GeV` → 6 GeV bucket, `SampleBeamEnergyStr = 5986MeV`, `BeamEnergyValue = 5.98636`

### Q² category inference (simulation only)

Configured by `ConfigQ2Cut`.

The Q² label is inferred from the beam-energy bucket (GENIE simulation samples only):

- 2 GeV → `GENIE_DEF_CUT` → `Q2_0_02`
- 4 GeV → `Q2_0_25_CUT` → `Q2_0_25`
- 6 GeV → `Q2_0_4_CUT` → `Q2_0_40`

## Sample naming conventions

Configured by `ConfigSampleName`.

### GENIE simulation

Pattern:

```text
<Target>_sim_<Tune>_<Q2Label>_<EnergyLabel>
```

Example:

```text
C12_sim_G18_Q2_0_02_2070MeV
```

### Data

Pattern:

```text
<Target>_data_<EnergyLabel>[_run_<ReconHipoDir>]
```

Example:

```text
C12_data_2070MeV_run_015664
```

### Uniform simulation

Two modes exist in the current implementation:

1. Tester samples (special-cased by the path):

```text
Uniform_sample_e_tester_<EnergyLabel>
```

2. Particle-type derived from tokens (`OutPut_1e`, `OutPut_ep`, `OutPut_en`):

```text
Uniform_<ParticleType>_sample_<EnergyLabel>
```

Example:

```text
Uniform_en_sample_2070MeV
```

## Vertex cuts

Configured by:

- `ConfigureVz_cuts()`
- `ConfiguredVz_cuts()`

The class stores separate cut windows for:

- Global (all)
- FD-only
- CD-only

`Vz` is the reconstructed vertex z coordinate selection.
`dVz` is a delta-vertex-z consistency selection.

Cut windows are keyed primarily off `SampleName` and differ between:

- GENIE simulation samples
- Data samples (including run-specific handling)
- Uniform samples (particle type and beam energy)

## VaryingSampleName

Configured by `ConfigureVaryingSampleName()`.

This string is intended to select sample-dependent inputs such as:

- Acceptance maps
- Weight maps
- Neutron-resolution inputs

In the current implementation it is set to the provided sample name for all supported sample types.

## HipoChain helpers

The class provides two helpers to add input files into a `HipoChain`:

- `AddToHipoChain(...)`
  - Legacy implementation with hard-coded run lists inside the method.

- `AddToHipoChainFromList(...)`
  - Preferred implementation that uses centralized run lists (from the project’s `lists` utilities)
  - Expands run directories only when no specific run directory is requested
  - Falls back to user-provided input globs when needed

## Public API highlights

Commonly used getters:

- `std::string GetSampleName()`
- `std::string GetVaryingSampleName()`
- `double GetBeamEnergy()`
- `bool IsBeamAt2GeV() const`, `IsBeamAt4GeV() const`, `IsBeamAt6GeV() const`
- `bool IsDataSample() const`, `bool IsSimulationSample() const`
- `DSCuts GetVz_cuts()`, `GetVz_cuts_FD()`, `GetVz_cuts_CD()`
- `DSCuts GetdVz_cuts()`, `GetdVz_cuts_FD()`, `GetdVz_cuts_CD()`

## Notes for developers

- All `Config*` methods are mutators and are intended to be called from the constructor body.
- If you add a new sample naming convention or directory pattern, update parsing here.
- Avoid copying sample parsing logic into analysis modules; prefer passing an `ExperimentParameters` instance.
