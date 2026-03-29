
# The `DSCuts` class

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

`DSCuts` is a lightweight configuration container used to define and manage a single analysis cut within the `2N-Analyzer-e4nu` framework.

It stores:

- The cut variable name (e.g. momentum, beta, resolution)
- Detector region label (CD / FD / combined)
- Particle label and corresponding PDG code
- Applied-cuts description string
- Mean value and lower/upper cut boundaries
- Fit-related parameters (fit mean, sigma factor, sigma value)
- Optional slice information (slice index and bounds)

This class does **not** perform physics calculations. It acts purely as a structured data holder for analysis configuration.

## Design

The class is intentionally simple:

- No external dependencies beyond the C++ standard library
- Deterministic member initialization
- Public state preserved for backward compatibility with existing analysis code

## Constructor

```cpp
DSCuts(std::string cv = "", std::string r = "", std::string p = "", std::string ac = "", double mean = 0, double llim = -9999, double ulim = 9999);
```

### Constructor behavior

- If `r == ""`, `Region` is set to `"CD & FD"`.
- `FitStdFactor` is determined from the **input** region argument `r` (legacy behavior):
  - `"CD"` -> `3`
  - `"FD"` -> `3`
  - otherwise -> `1`
- The internal vector `Cuts` is always structured as:

```text
Cuts = { mean, lower_cut, upper_cut }
```

### Particle label mapping to PDG codes

If the provided particle label does not match any supported alias, `particlePDG` remains `0`.

| Particle label aliases | PDG |
|---|---:|
| `electron`, `Electron`, `electrons`, `Electrons` | `11` |
| `neutron`, `Neutron`, `neutrons`, `Neutrons` | `2112` |
| `proton`, `Proton`, `protons`, `Protons` | `2212` |
| `kplus`, `Kplus`, `k+`, `K+` | `321` |
| `kminus`, `Kminus`, `k-`, `K-` | `-321` |
| `piplus`, `Piplus`, `pi+`, `Pi+` | `211` |
| `piminus`, `Piminus`, `pi-`, `Pi-` | `-211` |
| `pizero`, `Pizero`, `pi0`, `Pi0` | `111` |

## Stored values

### Cut triplet

The cut parameters are stored in `Cuts` as a 3-element vector:

- `Cuts[0]`: mean
- `Cuts[1]`: lower cut
- `Cuts[2]`: upper cut

### Fit-related quantities

- `MeanFromHistogram`: cached mean extracted from a histogram
- `MeanFromFit`: cached mean extracted from a fit (can be used by higher-level code)
- `FitStdFactor`: sigma multiplier (region dependent)
- `FitStd`: cached sigma from a fit

### Slice bookkeeping

Slice fields are intended for workflows that fit or apply cuts in bins (slices) of another variable:

- `SliceNumber`
- `SliceLowerb`
- `SliceUpperb`

## Setters

### Mean and fit

```cpp
void SetMeanHist(double mh);
void SetMeanFit(double mf);
void SetStdFactor(double sf);
```

Note: `SetMeanFit()` currently writes to `MeanFromHistogram` (legacy behavior preserved to avoid changing existing functionality).

### Cut parameters

```cpp
void SetCutPram(double mean, double lcut, double ucut);
void SetMean(double mean);
void SetLowerCut(double lcut);
void SetUpperCut(double ucut);
```

### Metadata

```cpp
void SetCutVariable(std::string cv);
void SetRegion(std::string r);
void SetPart(std::string p);
void SetAppliedCuts(std::string ac);
```

### Slice information

```cpp
void SetSliceNumber(int sNumber);
void SetSliceUpperb(double sUpperb);
void SetSliceLowerb(double sLowerb);
```

## Getters

### Fit and cut values

```cpp
double GetMeanHist();
double GetMeanFit();
double GetStdFactor();
double GetMean();
double GetLowerCut();
double GetUpperCut();
```

Const-safe variants:

```cpp
double GetMeanConst() const;
double GetLowerCutConst() const;
double GetUpperCutConst() const;
```

### Metadata

```cpp
std::string GetCutVariable();
std::string GetRegion();
std::string GetPart();
int GetPartPDG();
std::string GetAppliedCuts();
```

### Slice information

```cpp
int GetSliceNumber();
double GetSliceUpperb();
double GetSliceLowerb();
```

## Example

```cpp
DSCuts protonBetaCut("beta", "FD", "proton", "Preselection", 0.95, 0.80, 1.10);

// Access cut limits
const double lower = protonBetaCut.GetLowerCut();
const double upper = protonBetaCut.GetUpperCut();

// Update cut limits
protonBetaCut.SetLowerCut(0.85);
protonBetaCut.SetUpperCut(1.05);
```

## Notes and caveats

- `DSCuts` does not validate inputs.
- Particle matching is string-based and only supports the explicit aliases listed above.
- Legacy behavior is preserved (notably `SetMeanFit()` writing to `MeanFromHistogram`).