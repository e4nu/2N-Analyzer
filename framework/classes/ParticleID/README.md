
# The `ParticleID` class (CLAS12 analysis helper)

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

`ParticleID` is a utility class used in the `2N-Analyzer-e4nu` framework to build **event-level particle collections** and apply **analysis PID-style selections** on reconstructed CLAS12 particles. It also provides **monitoring histograms and counters** for specific proton-cleaning logic (duplicate-removal style cuts) and wraps the project’s ECAL veto utilities implemented in `reco_analysis_functions`.

This class is designed to be used inside event loops that already have:
- the event particle list (`allParticles`) as `std::vector<clas12::region_part_ptr>`
- an electron collection (typically `electrons[0]`)
- analysis configuration objects (`DSCuts`)
- optional event-level quantities like `starttime`, `beamE`, and weights

---

## Location

- Header: `framework/classes/ParticleID/ParticleID.h`
- Implementation: `framework/classes/ParticleID/ParticleID.cpp`
- Related utilities:
  - `framework/namespaces/general_utilities/reco_analysis_functions.h`
  - `framework/namespaces/general_utilities/constants.h`
  - `framework/classes/DSCuts/DSCuts.h`
  - `framework/classes/MomentumResolution/MomentumResolution.*`

---

## What the class provides

### 1) FD neutral identification and “redefinition” logic

The class supports identifying FD neutrals (neutrons and photons) using the “Larry” style redefinition:

- **Neutron (redefined):** neutral FD candidate with **no PCAL hit** and **has ECIN or ECOUT hit**
- **Photon (redefined):** neutral FD candidate with a **PCAL hit**

Functions:
- `ReDefFDNeutrals(...)`  
  Splits FD neutrals into redefined neutrons and photons (by indices into `allParticles`).
- `FDNeutralParticleID(...)` (multiple overloads)  
  Applies momentum cuts and optional vetoes / edge cuts to produce “within PID cuts” lists.

---

### 2) ECAL veto wrappers (charged and neutral proximity vetoes)

The actual veto implementations live in `reco_analysis_functions`. `ParticleID` exposes wrapper member functions:

- `NeutronECAL_Cut_Veto(...)`  
  Wrapper of the legacy ECAL-based veto used historically.
- `ChargedECALveto(...)`  
  Wrapper of the newer charged proximity veto for FD neutral candidates.
- `NeutralECALveto(...)`  
  Wrapper of the neutral veto utility.

These are typically used during FD neutral selection to reject neutrals contaminated by charged activity.

---

### 3) Charged particle ID utilities

- `ChargedParticleID(ChargedParticles, FD_mom_th, CD_mom_th)`  
  Returns indices of charged particles that pass region-dependent momentum thresholds.
- `ChargedParticleID(ChargedParticles, Momentum_th)`  
  Same idea but uses a single threshold regardless of region.

---

### 4) FD neutron/photon selectors by definition

- `GetFDNeutrons(allParticles, Momentum_cuts, apply_nucleon_cuts)`
- `GetFDPhotons(allParticles, Momentum_cuts)`
- `GetFDNeutronP(Neutron, apply_nucleon_cuts)`  
  Utility for FD neutron momentum (and historical photon-handling mode).

---

### 5) Generic “good particle” filtering

- `GetGoodParticles(Particle, Momentum_cuts)`  
  Returns indices for particles passing a momentum lower cut.

---

## The “good protons” logic

### Purpose

`GetGoodProtons(...)` takes a list of identified proton indices (into the `protons` vector) and removes likely duplicates or problematic combinations to obtain a “clean” proton set for 2p topologies.

It is used with monitoring in `GPMonitoring(...)`, which fills:
- monitoring histograms
- event counters tracking how often specific proton-removal mechanisms occur

### Main mechanisms

This code implements two main duplicate-removal patterns:

1) **sCTOFhp**  
   Two CD protons share the same CTOF hit position (position difference magnitude equals zero).  
   Interpretation: likely double-counting of a single physical proton.

2) **dCDaFDd**  
   One CD and one FD proton near a region boundary and sufficiently close in `Δφ`.  
   Interpretation: duplicate reconstruction of a single proton across regions.

### Monitoring objects and counters

The class contains:
- multiple `hPlot2D` members and raw `TH2D*` pointers used for monitoring
- integer counters tracking how many events were:
  - removed from 2p by each cut type (RM counters)
  - reduced from 3p, 4p, or >=5p down to 2p by each cut type (AD counters)
  - mixed (both cut types in the same event)

Functions:
- `SetGPMonitoringPlots(...)`
- `GPMonitoring(...)`

Important: if you interpret the mixed counter as “event contains both cut types anywhere”, ensure the implementation uses event-level flags rather than per-proton flags. Otherwise, it can undercount mixed events when the two cut types occur on different protons.

---

## Event particle collection utility

`SetEventParticles(...)` builds the standard per-event particle groups and returns them as a tuple.

Type:

```cpp
using ParticleVectorsTuple = std::tuple<
    std::vector<clas12::region_part_ptr>,  // neutrons
    std::vector<clas12::region_part_ptr>,  // protons
    std::vector<clas12::region_part_ptr>,  // Kplus
    std::vector<clas12::region_part_ptr>,  // Kminus
    std::vector<clas12::region_part_ptr>,  // piplus
    std::vector<clas12::region_part_ptr>,  // piminus
    std::vector<clas12::region_part_ptr>,  // electrons
    std::vector<clas12::region_part_ptr>,  // deuterons
    std::vector<clas12::region_part_ptr>,  // neutrals
    std::vector<clas12::region_part_ptr>   // otherpart
>;
```

Usage:

```cpp
auto [neutrons, protons, Kplus, Kminus, piplus, piminus, electrons, deuterons, neutrals, otherpart] = pid.SetEventParticles(clas12ana_particles, clasAna, c12);
```

---

## Typical usage pattern

Pseudo-flow inside an event loop:

1. Build event particle collections
2. Identify charged particles and FD neutrals
3. Apply vetoes and PID cuts
4. Clean proton list using `GetGoodProtons`
5. If monitoring is enabled, call `GPMonitoring`
6. Fill physics histograms or build final-state objects

Example:

```cpp
ParticleID pid;

auto [neutrons, protons, Kplus, Kminus, piplus, piminus, electrons, deuterons, neutrals, otherpart] = pid.SetEventParticles(use_clas12ana, clasAna, c12);

auto IDProtons_ind = pid.ChargedParticleID(protons, FD_p_th, CD_p_th);

auto GoodProtons_ind = pid.GetGoodProtons(apply_nucleon_cuts, protons, IDProtons_ind, Theta_p1_cuts_2p, Theta_p2_cuts_2p, dphi_p1_p2_2p);

pid.GPMonitoring(enableGoodProtonsMonitoring, protons, IDProtons_ind, GoodProtons_ind, Theta_p1_cuts_2p, Theta_p2_cuts_2p, dphi_p1_p2_2p, weight);
```

---

## Notes

- Many functions return indices rather than particle pointers. Ensure you track which container the indices refer to.
- Monitoring histograms that use raw `TH2D*` must be properly allocated before filling.
- The flag `apply_nucleon_cuts` toggles additional reconstruction-based or physics-driven logic in several functions. Keep its usage consistent across the analysis chain.
