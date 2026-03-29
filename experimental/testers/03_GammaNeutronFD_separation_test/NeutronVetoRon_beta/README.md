
# Forward Detector (FD) Neutral Veto  

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Ron Wagner implementation — refactored and documented by Alon Sportes

---

## Quick-Start Summary (One-Page Overview)

This section provides a high-level overview of the FD neutral veto for readers who want to
understand *what the code does* and *how it is used* without reading the full technical details
below.

### What problem does this code solve?

In the CLAS12 Forward Detector, neutral particles (neutrons or photons) frequently produce
multiple detector hits:
- across different ECAL layers (PCAL, ECIN, ECOUT),
- within the same ECAL layer due to segmentation,
- or in combination with upstream detectors (FTOF).

Without a dedicated veto, this leads to double counting, charged-particle contamination, and
poorly controlled detector artifacts.

### What is the basic idea of the veto?

1. Build *hit-level objects* for all FD detector responses (not particle-level objects).
2. Consider **only ECAL neutral hits** as veto candidates.
3. For each ECAL neutral hit, check whether it should be rejected based on:
   - upstream neutral activity (FTOF),
   - nearby charged-particle hits,
   - duplication across ECAL layers,
   - duplication within the same ECAL layer.

Only one representative ECAL hit per physical neutral particle is allowed to survive.

### What does the veto keep?

- The **earliest ECAL layer** hit (PCAL preferred over ECIN, ECIN over ECOUT).
- A **single clustered hit** per layer when multiple related hits occur in the same layer.

### What does the veto reject?

- ECAL neutral hits associated with FTOF neutral hits.
- ECAL neutral hits associated with charged-particle hits.
- Later-layer ECAL hits when an earlier-layer hit is present.
- Pathological ECAL responses consistent with misreconstructed charged particles.
- Many 3-layer ECAL responses (PCAL+ECIN+ECOUT) via indirect Step-3 bookkeeping (`num_of_ECAL_hits_ahead`).

### What are the key functions?

- `GenerateHits`  
  Builds `NeutralHit` and `ChargedHit` objects from reconstructed CLAS12 particles.

- `neutSelection`  
  Organizes hits by detector/layer and drives the veto sequence.

- `AddNeutrons`  
  Implements the actual veto logic (Steps 1–4).

### How is the veto applied in practice?

For each event:
1. Call `GenerateHits`.
2. Call `neutSelection` on the returned hits.
3. Use the surviving `NeutralHit` objects as your FD neutral candidates.

### When should I read the rest of this README?

If you need to:
- modify veto cuts,
- understand or debug veto decisions,
- validate the physics behavior,
- or extend the veto logic,

then you **must** read the detailed sections below.

---

---

## 1. Purpose and Physics Motivation

This code implements a **neutral-particle veto for the CLAS12 Forward Detector (FD)**.

### Why a Dedicated Neutral Veto Is Needed

In the CLAS12 Forward Detector, neutral particles frequently produce:
- multiple ECAL hits across different layers,
- fragmented showers within the same layer,
- signatures that overlap with charged-particle responses.

A naïve neutral selection (e.g. “any ECAL hit without a charged track”) leads to double counting of the same neutral particle, contamination from misreconstructed charged tracks, and poor control over detector-induced artifacts.

The veto implemented here addresses these issues explicitly and deterministically.

The veto is designed to identify **clean neutral FD candidates** (neutrons or photons) reconstructed in the ECAL, while rejecting backgrounds caused by charged particles misidentified as neutral, neutral particles that already interacted upstream (e.g. in FTOF), duplicated detector responses of the *same* neutral particle across ECAL layers, and pathological multi-layer ECAL responses inconsistent with neutral physics.

The veto operates **at the hit level**, not the particle level.  
This distinction is crucial and underlies the entire design.

---

## 2. Credit and Provenance

**Original veto logic:**  
> Ron Wagner, Tel Aviv University (obtained November 2025)

**Refactoring, restructuring, debugging, documentation:**  
> Alon Sportes, Tel Aviv University

The core veto philosophy, ordering logic, and matching criteria originate from Ron Wagner’s implementation.  
This codebase preserves that logic while making it:

- explicit,
- debuggable,
- maintainable,
- and safe against silent logic drift.

Any deviations from Ron’s original intent are documented explicitly in comments and TODOs.

---

## 3. High-Level Conceptual Picture

### 3.1 Particle vs Hit (critical distinction)

In CLAS12 reconstruction:

- **One reconstructed particle** may produce **multiple detector hits**.
- Example: a neutron can fire:
  - PCAL,
  - ECIN,
  - ECOUT,
  → producing **three separate ECAL hits**.

This code **does not assume**:
> one particle = one hit

Instead, it explicitly handles:
> one particle → many hits → choose which hit(s) survive

This decision is made *after* all hits are constructed and never during reconstruction.

**Concrete example**

A single reconstructed neutron can produce:
- 1 hit in PCAL,
- 1 hit in ECIN,
- 1 hit in ECOUT.

This results in **three NeutralHit objects**.
The veto logic decides which of these represents the physical neutron.

---

### 3.2 Detector ordering along the beamline

The Forward Detector elements relevant here are ordered as:

```
(target) → FTOF → PCAL → ECIN → ECOUT
```

This ordering is not arbitrary.

Earlier (upstream) layers are closer to the interaction point and are therefore
considered more physically representative of the neutral particle’s first interaction.

Later layers are more susceptible to:
- shower leakage,
- secondary interactions,
- duplicated detector responses.

For this reason, the veto always prefers earlier ECAL layers over later ones.

Definitions used throughout the code:

- **EC**   = ECIN + ECOUT  
- **ECAL** = PCAL + ECIN + ECOUT

This ordering is essential for the veto logic.

---

## 4. Files and Their Roles

### 4.1 `NeutralHitStruct.cpp`

Defines the `NeutralHit` struct.

#### What a NeutralHit represents

A `NeutralHit` is a **single detector response** (not a particle):

- One hit in **FTOF** or **one ECAL layer**
- With geometry, timing, energy, and bookkeeping info
- Independent of other hits from the same particle

NeutralHit objects are immutable with respect to detector content; veto logic only modifies bookkeeping flags.

#### Stored information includes

- Detector identity (`detector`, `layer`, `sector`)
- Absolute hit position (`HitPos`)
- Direction from electron vertex (`HitPath`)
- Timing (`HitTime`, `Time`)
- ECAL energy deposition
- Bookkeeping flags:
  - `blocking`
  - `num_of_ECAL_hits_ahead`

#### Why both HitPos *and* HitPath?

- `HitPos`: where the detector fired
- `HitPath = HitPos - Vtx_e`: direction of the hit relative to the electron vertex

**All angular matching in the veto uses `HitPath`.**  
This makes the veto sensitive to physics direction rather than detector geometry.

#### The `num_of_ECAL_hits_ahead` Counter

This counter is incremented on an earlier-layer ECAL hit whenever a related
later-layer ECAL hit is found and vetoed.

Its purpose is to identify pathological ECAL responses where a single neutral
produces signals in *all three* ECAL layers (PCAL + ECIN + ECOUT).

Because Step 3 vetoes later layers, this counter is most relevant for PCAL hits:
- A PCAL hit matched to both ECIN and ECOUT will have `num_of_ECAL_hits_ahead == 2`.
- Such cases are considered unlikely for true neutrals and can be rejected.

This logic originates from Ron Wagner’s original veto design.

---

### 4.2 `ChargedHitStruct.cpp`

Defines the `ChargedHit` struct.

This is analogous to `NeutralHit`, but for **charged particles**.

Key differences:

- Contains charge and PID
- Uses particle vertex instead of electron vertex
- Used **only** in the charged veto (Step 2)

This separation is intentional:
- neutral logic never depends on charged internals
- charged logic never mutates neutral state

---

### 4.3 `newVeto_functions.cpp`

This file contains **all active veto logic**.

Main components:

1. `GenerateHits`
2. `neutSelection`
3. `AddNeutrons`

It also contains a **high-level veto description block**, which should be kept consistent with this README.

---

### 4.4 Execution Flow (Event-Level Call Order)

For each event:
1. `GenerateHits` builds hit-level objects from reconstructed particles.
2. `neutSelection` organizes hits by detector and ECAL layer.
3. `AddNeutrons` is called three times (ECOUT → ECIN → PCAL) to apply the veto.
4. Surviving `NeutralHit` objects are returned to the analysis.

---

### 4.5 `FDNeutralVeto.h` / `FDNeutralVeto.cpp`

These files define the `FDNeutralVeto` class, which is the primary entry point for the FD neutral veto.

**What the class provides**
- A single place that packages Ron Wagner’s veto logic (Steps 1–4) plus Alon Sportes’ refactoring, documentation, and debugging instrumentation.
- Optional ROOT histogram instrumentation to audit the veto (general kinematics and step-by-step comparisons).
- A per-event start time interface (`SetStartTime`) used for TOF-derived neutral kinematics (`Calc_nFD_vars`), which is also used by Reformatted Step 5 classification.

**Public API**
- `GenerateHits(...)`: build `NeutralHit` and `ChargedHit` objects from reconstructed particles.
- `neutSelection(...)`: orchestrate the veto passes across ECAL layers.
- `AddNeutrons(...)`: apply the Ron-style veto logic to a specific ECAL layer (Steps 1–4).
- `GetNewNeutronFDVetoHits(...)`: convenience wrapper that returns veto-passing neutral hits.
- `EnableDebugHists(...)` / `FinalizeDebugHists(...)`: enable and write the debug histograms.
- `SetStartTime(...)`: set per-event start time for TOF-based quantities.

**Reformatted Step 5 in class terms**
Reformatted Step 5 conditionally applies the *indirect* three-layer rejection (implemented via `num_of_ECAL_hits_ahead < 2`) based on TOF-derived beta classification of the earliest-layer ECAL hit:
- photon-like: keep strict topology rejection
- neutron-like and ambiguous: relax the indirect three-layer rejection to protect neutron efficiency at high momentum

---

## 5. GenerateHits — Hit Construction Stage

### Purpose

`GenerateHits` converts reconstructed CLAS12 particles into **hit-level objects**:

- `NeutralHit` objects
- `ChargedHit` objects

No veto is applied here.

### Important properties

- Each detector response becomes **one hit object**
- A particle can produce:
  - 0 hits (ignored)
  - 1 hit
  - multiple hits (e.g. PCAL + ECIN)

### Guarantees After Hit Construction

After `GenerateHits`:
- All hit objects are internally consistent and validated.
- All geometry vectors are cached and synchronized.
- No hit has veto-related state set (blocking=false, counters reset).

---

## 6. Definition of “Related Hits”

Two hits are considered **related** if all of the following hold:

1. **Sector match**
   - Minimal sector distance equals `dSector_cut`
   - Wrap-around on 6 sectors is handled explicitly

2. **Angular match**
   - Opening angle between `HitPath` vectors  
   - Must be `< Opening_ang_cut_deg`

3. **Timing match**
   - `|ΔHitTime| < dHitTime_cut`

These matching criteria are symmetric and applied identically regardless of detector or layer.

These criteria are used **consistently in all veto steps**.

---

## 7. Veto Algorithm Overview

The veto operates **only on ECAL neutral hits**.

Processing order: ECOUT → ECIN → PCAL

This ensures that **earlier (upstream) layers dominate** later ones.

The logic is implemented in `AddNeutrons`, orchestrated by `neutSelection`.

---

## 8. Detailed Veto Steps

The veto is applied sequentially to each ECAL neutral hit using the following steps:

### Step 1 — FTOF Neutral Veto

**Question:**  
> Is there a neutral hit in FTOF related to this ECAL hit?

If **yes** → veto the ECAL hit.

**Motivation:**  
A neutral particle should not fire ECAL *after* already interacting in FTOF.

---

### Step 2 — Charged Particle Veto

**Question:**  
> Is there a charged hit related to this ECAL hit?

Charged hits include:
- FTOF
- ECAL layers up to the tested layer

If **yes** → veto the ECAL hit.

**Motivation:**  
Reject charged particles misidentified as neutral.

---

### Step 3 — Earliest ECAL Layer Wins (Ron Wagner logic)

This step is the reason ECAL layers are processed in reverse order (ECOUT → ECIN → PCAL).

**Question:**  
> Is there a related neutral hit in an earlier ECAL layer?

If **yes**:
- veto the *later* hit
- increment `num_of_ECAL_hits_ahead` on the *earlier* hit

**Example:**
- PCAL + ECOUT → keep PCAL, veto ECOUT

**Important detail:**  
This bookkeeping persists across layers because pointer views reference the same hit objects.

This exactly preserves Ron Wagner’s original logic.

---

### Step 4 — Same-Layer Clustering

**Question:**  
> Are there multiple related neutral hits in the same ECAL layer?

If **yes**:
- combine them into one hit
- average `HitPath`
- mark neighbors as `blocking = true`

**Motivation:**  
Handle segmentation and shower splitting within a layer.

Note: only spatial information is averaged during clustering; timing information
is taken from the cluster seed hit.

---

### Step 5 — Three-Layer ECAL Veto (implicit / indirect)

Ron Wagner’s original veto included an explicit Step 5 block that rejected candidates consistent with a
**3-of-3 ECAL response** (PCAL + ECIN + ECOUT), motivated by the fact that true neutrals are unlikely to
produce clean, related hits in all three ECAL layers.

In the refactored code, the explicit Step 5 block has been removed. However, a closely related rejection
still occurs **indirectly** via Step 3 bookkeeping and the final acceptance cut on:

- `num_of_ECAL_hits_ahead < 2`

Mechanism:

- During the ECOUT and ECIN passes, Step 3 vetoes later-layer hits if an earlier-layer match exists.
- Each time an earlier-layer ECAL hit matches a later-layer test hit, the earlier-layer hit increments
  `num_of_ECAL_hits_ahead`.
- Because `neutSelection` uses pointer views into stable local hit storage, these increments persist across
  passes (ECOUT → ECIN → PCAL).

Therefore, if a candidate truly produced matching hits in **all three ECAL layers**, the **PCAL** hit will
accumulate two "ahead" matches (ECIN and ECOUT):

- `PCAL.num_of_ECAL_hits_ahead == 2`

and will be rejected during the PCAL pass by the `< 2` requirement.

**Important:** this is a structural, ordering-based rejection ("PCAL saw both later layers") rather than an
explicit particle-level rule ("this particle fired all three layers").

---

## 9. neutSelection — Veto Driver

`neutSelection`:

1. Splits neutral hits into:
   - FTOF
   - ECAL layers
2. Builds pointer views into the same hit objects
3. Calls `AddNeutrons` layer-by-layer
4. Collects surviving neutral hits

This design ensures:
- bookkeeping survives across layers
- no hit duplication
- no accidental state resets

`neutSelection` itself performs no physics decisions; all veto logic resides in `AddNeutrons`.

---

## 10. Design Philosophy

- **Strict correctness over silent failure**
- **Explicit geometry and timing logic**
- **No hidden assumptions**
- **Debug output available at every decision point**
- **Logic locality**: each veto decision is made in exactly one place in the code.

The code is intentionally verbose and defensive to prevent physics-level mistakes.

---

## 11. Summary in One Sentence

> This veto identifies clean FD neutral candidates by rejecting hits associated with upstream activity, charged tracks, or redundant ECAL responses, while preserving the earliest physically meaningful hit per neutral particle — exactly as originally designed by Ron Wagner.

---

## 12. Algorithmic Difference: Ron Wagner Veto vs Reformatted Step 5

The **original Ron Wagner veto** is purely topology- and matching-based:  
- It unconditionally rejects ECAL neutral candidates that produce matching hits in all three ECAL layers (PCAL, ECIN, ECOUT), via the indirect acceptance cut  
  `num_of_ECAL_hits_ahead < 2`.  
- Any ECAL neutral candidate whose earliest ECAL-layer hit is associated with matching hits in both later ECAL layers is vetoed.  
- This rejection is applied **independently of particle kinematics**, timing interpretation, or momentum regime.

In contrast, **Reformatted Step 5** introduces a *conditional*, topology-aware extension to this logic:  
- The indirect three-layer rejection (`num_of_ECAL_hits_ahead < 2`) is applied **only** to candidates classified as *photon-like*, based on the TOF-derived beta of the **earliest ECAL-layer hit**.  
- For *neutron-like* and *ambiguous* candidates, the indirect three-layer rejection is **relaxed**, allowing multi-layer ECAL responses to survive when they are consistent with high-momentum neutron kinematics.  
- All other veto steps (FTOF neutral veto, charged veto, earliest-layer ordering, same-layer clustering) remain **identical** to Ron Wagner’s original algorithm.

In summary:  
- **Ron Wagner veto:** unconditional, topology-based rejection of multi-layer ECAL responses.  
- **Reformatted Step 5:** topology + kinematics; indirect three-layer rejection is applied selectively using TOF beta, preserving high-momentum neutron efficiency while retaining strong photon background suppression.

---

## Maintainers

- **Ron Wagner** — original veto logic and concept  
- **Alon Sportes** — refactoring, validation, documentation

---
</file>