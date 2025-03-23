#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "../../classes/DSCuts/DSCuts.h"

namespace constants {
// Physical constants ---------------------------------------------------------------------------------------------------------------------------------------------------

// Particle masses (in GeV):
constexpr double m_n = 0.939565;
constexpr double m_p = 0.938272;
constexpr double m_Kminus = 0.493677;
constexpr double m_Kplus = 0.493677;
constexpr double m_Kzero = 0.497614;
constexpr double m_piplus = 0.13957;
constexpr double m_piminus = 0.13957;
constexpr double m_pizero = 0.134977;
constexpr double m_e = 0.000510999;
constexpr double m_d = 1.8756;

// Speed of light in vacuum (in cm/ns):
constexpr double c = 29.9792458;

// Particle PDG ---------------------------------------------------------------------------------------------------------------------------------------------------------
constexpr int nPDG = 2112, pPDG = 2212;
constexpr int KplusPDG = 321, KminusPDG = -321, KzeroPDG = 311;
constexpr int piplusPDG = 211, piminusPDG = -211, pizeroPDG = 111;
constexpr int ePDG = 11, nuePDG = 12;
constexpr int muPDG = 13, numuPDG = 14;
constexpr int tauPDG = 15, nutauPDG = 16;
constexpr int phPDG = 22;

// CLAS12 constants -----------------------------------------------------------------------------------------------------------------------------------------------------
/* FD & CD theta range */
extern DSCuts ThetaFD;
extern DSCuts ThetaCD;

};  // namespace constants

#endif  // CONSTANTS_H