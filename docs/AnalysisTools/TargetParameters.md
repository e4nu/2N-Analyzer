
# The `TargetParameters` class

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

`TargetParameters` is a lightweight base class that stores nuclear target metadata used throughout the 2N-Analyzer framework.

It does **not** perform path parsing or configuration logic. Instead, it provides a structured container for:

- A human-readable target label (e.g. `H1`, `D2`, `C12`, `Ar40`)
- The GENIE PDG ion code of the target nucleus
- The probe PDG (incident lepton)
- An effective binding energy parameter
- The decomposed PDG ion-code components (A, Z, L, I)

This class is intended to be inherited by higher-level configuration classes such as `ExperimentParameters`, which determine the target from directory structure and configure these fields.

---

## GENIE / PDG2006 Ion Code Convention

GENIE follows the PDG2006 ion-code convention:

```
10LZZZAAAI
```

Where:

- `AAA`  → total baryon number (A)
- `ZZZ`  → total charge number (Z)
- `L`    → number of strange quarks
- `I`    → isomer number (I = 0 corresponds to the ground state)

### Examples

| Nucleus | PDG Code |
|----------|----------|
| Hydrogen (H1) | `1000010010` |
| Deuterium (D2) | `1000010020` |
| Carbon-12 (C12) | `1000060120` |
| Argon-40 (Ar40) | `1000180400` |
| O16 | `1000080160` |
| Fe56 | `1000260560` |

These codes are stored in `TargetElementPDG`.

---

## Data Members

### Core Properties

- `std::string TargetElement`
  - Human-readable label (e.g. `"C12"`).

- `int TargetElementPDG`
  - GENIE/PDG2006 ion code.

- `int ProbePDG`
  - PDG code of the incoming lepton (e.g. 11 for electron).

- `double BindingEnergyToNucleus`
  - Effective binding energy parameter used in analysis or generator configuration.

### Decomposed Ion-Code Components

The following members store the individual components of the `10LZZZAAAI` structure:

- `TotalBaryonNumber_A`
- `TotalChargeNumber_Z`
- `StrangeQuarksNumber_L`
- `IsomerNumber_I`

These are primarily used for clarity and consistency when configuring or validating nuclear properties.

---

## Public Interface

### Setters

- `SetTargetElement(std::string)`
- `SetTargetElementPDG(int)`
- `SetProbePDG(int)`
- `SetBindingEnergyToNucleus(double)`

These methods are typically called by derived configuration classes.

### Getters

- `GetTargetElement()`
- `GetTargetElementPDG()`
- `GetProbePDG()`
- `GetBindingEnergyToNucleus()`

---

## Intended Usage Pattern

A derived class (for example `ExperimentParameters`) will:

1. Parse a sample path
2. Identify the target element
3. Set:
   - `TargetElement`
   - `TargetElementPDG`
   - A/Z/L/I values
   - Probe PDG
   - Binding energy (if required)

After configuration, analysis code accesses these values only via getters.

---

## Design Principles

- Minimal responsibility: stores target metadata only.
- No path parsing logic.
- No analysis logic.
- Safe base class for reuse across multiple configuration systems.

If a new target nucleus is introduced, prefer configuring it in the derived configuration layer rather than modifying this base class logic.
