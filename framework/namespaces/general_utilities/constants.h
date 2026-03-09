#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "../../classes/DSCuts/DSCuts.h"

/**
 * @namespace constants
 *
 * Central namespace for analysis-wide physical, particle-ID, and detector-configuration constants.
 *
 * Purpose:
 *   Provide a single shared location for immutable values and standard configuration objects
 *   that are reused throughout the analysis code.
 *
 * Main contents:
 *   - particle rest masses in GeV
 *   - the speed of light in cm/ns
 *   - PDG codes for common particles used in the analysis
 *   - CLAS12 analysis constants such as the nominal solenoid parameters
 *   - forward-detector and central-detector theta acceptance ranges as `DSCuts` objects
 *
 * Design notes:
 *   Most values here are intended to behave like global constants and should be referenced
 *   consistently across the project so that kinematic calculations, particle selection,
 *   and detector-region cuts all use the same definitions.
 *
 * Conventions:
 *   - masses are stored in GeV
 *   - the speed of light is stored in cm/ns to match reconstruction timing/path units
 *   - PDG codes follow standard PDG numbering conventions
 *   - detector angular ranges are encoded as `DSCuts` helpers for direct reuse in selections
 */
namespace constants {

// ======================================================================================================================================================================
// Physical constants
// ======================================================================================================================================================================

// Particle masses (in GeV).
// These are the standard rest-mass values used throughout the analysis when building
// energies, invariant masses, missing quantities, and other kinematic observables.
constexpr double m_n = 0.939565;       // neutron mass
constexpr double m_p = 0.938272;       // proton mass
constexpr double m_Kminus = 0.493677;  // charged kaon mass (K-)
constexpr double m_Kplus = 0.493677;   // charged kaon mass (K+)
constexpr double m_Kzero = 0.497614;   // neutral kaon mass
constexpr double m_piplus = 0.13957;   // charged pion mass (pi+)
constexpr double m_piminus = 0.13957;  // charged pion mass (pi-)
constexpr double m_pizero = 0.134977;  // neutral pion mass
constexpr double m_e = 0.000511;       // electron mass
constexpr double m_d = 1.8756;         // deuteron mass

// Speed of light in vacuum (cm/ns).
// This unit choice matches the common CLAS12 convention where path lengths are in cm
// and timing quantities are in ns.
constexpr double c = 29.9792458;

// ======================================================================================================================================================================
// Particle PDG codes
// ======================================================================================================================================================================

/**
 * @brief PDG codes for neutron and proton.
 *
 * These are used whenever particle identity must be checked or assigned explicitly.
 */
constexpr int nPDG = 2112, pPDG = 2212;

/**
 * @brief PDG codes for kaons.
 *
 * Includes charged and neutral kaon identifiers.
 */
constexpr int KplusPDG = 321, KminusPDG = -321, KzeroPDG = 311;

/**
 * @brief PDG codes for pions.
 *
 * Includes charged and neutral pion identifiers.
 */
constexpr int piplusPDG = 211, piminusPDG = -211, pizeroPDG = 111;

/**
 * @brief PDG codes for the electron and electron neutrino.
 */
constexpr int ePDG = 11, nuePDG = 12;

/**
 * @brief PDG codes for the muon and muon neutrino.
 */
constexpr int muPDG = 13, numuPDG = 14;

/**
 * @brief PDG codes for the tau and tau neutrino.
 */
constexpr int tauPDG = 15, nutauPDG = 16;

/**
 * @brief PDG code for the photon.
 */
constexpr int phPDG = 22;

// ======================================================================================================================================================================
// CLAS12 analysis constants
// ======================================================================================================================================================================

/**
 * @brief Nominal solenoid magnetic-field strength used by simplified geometric veto helpers.
 *
 * Units: Tesla.
 *
 * Note:
 *   This is used as an effective field parameter in compact analysis-level shift models,
 *   not as a full detector field-map replacement.
 */
double B = 5.0;

/**
 * @brief Effective solenoid path-length parameter used in simplified geometric veto helpers.
 *
 * Units: meters.
 *
 * Note:
 *   This represents an approximate effective distance through the solenoid region and is
 *   primarily used in compact phi-shift estimates rather than detailed detector transport.
 */
double L = 0.5;

/**
 * @brief Forward-detector polar-angle acceptance range.
 *
 * Encoded as a `DSCuts` object so it can be reused directly in region-based selections.
 * The numerical range is approximately 5 to 40 degrees.
 */
DSCuts ThetaFD = DSCuts("Theta FD range", "FD", "", "", 1, 5., 40.);

/**
 * @brief Central-detector polar-angle acceptance range.
 *
 * Encoded as a `DSCuts` object so it can be reused directly in region-based selections.
 * The numerical range is approximately 40 to 135 degrees.
 */
DSCuts ThetaCD = DSCuts("Theta CD range", "CD", "", "", 1, 40., 135.);

};  // namespace constants

#endif  // CONSTANTS_H