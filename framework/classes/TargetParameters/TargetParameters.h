#ifndef TARGETPARAMETERS_H
#define TARGETPARAMETERS_H

#include <iostream>
#include <string>

/**
 * @class TargetParameters
 *
 * Base class that stores target (nuclear) information used throughout the analysis.
 *
 * In particular, `TargetElementPDG` follows the GENIE / PDG2006 ion-code convention:
 *
 *   PDG ion code: 10LZZZAAAI
 *
 * where:
 *   - AAA  : total baryon number (A)
 *   - ZZZ  : total charge number (Z)
 *   - L    : number of strange quarks
 *   - I    : isomer number (I = 0 is the ground state)
 *
 * Examples:
 *   - O16  : 1000080160
 *   - Fe56 : 1000260560
 *
 * Common targets in this project:
 *   - Hydrogen  (H1): 1000010010
 *   - Deuterium (D2): 1000010020
 *   - Carbon-12 (C12): 1000060120
 *   - Argon-40 (Ar40): 1000180400
 */
class TargetParameters {
    // Protected so derived classes (e.g. ExperimentParameters) can configure these values.
   protected:
    // Human-readable target label used by the analysis (e.g. "H1", "D2", "C12", "Ar40").
    std::string TargetElement;

    // Target nucleus PDG code used by GENIE (PDG2006 ion code: 10LZZZAAAI).
    // Examples: H1 = 1000010010, D2 = 1000010020, C12 = 1000060120, Ar40 = 1000180400,
    //           O16 = 1000080160, Fe56 = 1000260560.
    int TargetElementPDG;

    // Incident lepton PDG (e.g. electron = 11).
    int ProbePDG;

    // Effective binding energy used by parts of the analysis / generator configuration.
    double BindingEnergyToNucleus;

    // Expanded PDG ion-code components (10LZZZAAAI):
    //   - AAA: total baryon number (A)
    //   - ZZZ: total charge number (Z)
    //   - L  : number of strange quarks
    //   - I  : isomer number (I=0 is the ground state)
    int TotalBaryonNumber_A;
    int TotalChargeNumber_Z;
    int StrangeQuarksNumber_L;
    int IsomerNumber_I;

   public:
    // Set the human-readable target label (e.g. "H1", "D2", "C12").
    void SetTargetElement(std::string TargetElementName) {
        // Store the target label for downstream configuration and naming.
        TargetElement = TargetElementName;
    }

    // Set the GENIE/PDG2006 ion PDG code for the target.
    void SetTargetElementPDG(int TargetElementPDGNumber) {
        // This should follow the 10LZZZAAAI convention described above.
        TargetElementPDG = TargetElementPDGNumber;
    }

    // Set the probe PDG code (incident lepton).
    void SetProbePDG(int ProbePDGNumber) {
        // Example: electron PDG = 11.
        ProbePDG = ProbePDGNumber;
    }

    // Set the (effective) binding energy used by the analysis/generator configuration.
    void SetBindingEnergyToNucleus(double BindingEnergy) { BindingEnergyToNucleus = BindingEnergy; }

    // Return the human-readable target label.
    std::string GetTargetElement() { return TargetElement; }

    // Return the target PDG ion code used by GENIE.
    int GetTargetElementPDG() { return TargetElementPDG; }

    // Return the probe PDG code.
    int GetProbePDG() { return ProbePDG; }

    // Return the configured binding energy.
    double GetBindingEnergyToNucleus() { return BindingEnergyToNucleus; }
};

#endif  // TARGETPARAMETERS_H
