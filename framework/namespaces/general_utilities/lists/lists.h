//
// Created by Alon Sportes on 13/03/2025.
//

#ifndef LISTS_H
#define LISTS_H

#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "../../../structures/DataSample/DataSample.h"
#include "../constants/constants_include.h"

namespace lists {
// VariableNames map ----------------------------------------------------------------------------------------------------------------------------------------------------

extern std::map<std::string, std::map<std::string, std::string>> VariableNames;

// HistogramLimits map --------------------------------------------------------------------------------------------------------------------------------------------------

inline std::map<std::string, std::map<std::string, double>> HistogramLimits;

// Histogram limits -----------------------------------------------------------------------------------------------------------------------------------------------------

// Histogram limits for analysis plots
// Default beam energy is 5.98636 GeV, which is changed in the analysis code
extern double beamE_def;  // Default beam energy

/* Nphe boundaries */
extern const double Nphe_boundary;

/* Chi2 boundaries */
extern double Chi2_boundary;

/* Vertex boundaries */
extern double Vertex_boundary, Vertex_uboundary, Vertex_lboundary;
extern double Vertex_boundary_FD, Vertex_uboundary_FD, Vertex_lboundary_FD;
extern double Vertex_boundary_CD, Vertex_uboundary_CD, Vertex_lboundary_CD;
extern double dV_boundary, dV_uboundary, dV_lboundary;
extern double dV_boundary_FD, dV_uboundary_FD, dV_lboundary_FD;
extern double dV_boundary_CD, dV_uboundary_CD, dV_lboundary_CD;

/* SF boundaries */
extern const double SF_uboundary, SF_lboundary;

/* Momentum boundaries */
extern const double Momentum_lboundary, Momentum_uboundary;
extern double FDMomentum_lboundary, FDMomentum_uboundary;
extern const double CDMomentum_lboundary, CDMomentum_uboundary;
extern double P_nucFD_lboundary, P_nucFD_uboundary;
extern double P_nucCD_lboundary, P_nucCD_uboundary;

/* W boundaries */
extern const double W_lboundary;
extern const double W_uboundary;

/* Beta boundaries */
extern const double dBeta_sigma_boundary;
extern const double Beta_dist_uboundary, Beta_dist_lboundary;
extern const double dBeta_sigma_ZOOMOUT_boundary;
extern const double Beta_dist_ZOOMOUT_uboundary, Beta_dist_ZOOMOUT_lboundary;

extern double Beta_boundary_const, Beta_boundary, P_boundary;

/* Angle boundaries */
extern const double Theta_lboundary_FD, Theta_uboundary_FD;
extern const double Theta_lboundary_CD, Theta_uboundary_CD;
extern const double Opening_Ang_wide_lboundary, Opening_Ang_wide_uboundary;
extern const double Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary;
extern const double Phi_lboundary, Phi_uboundary;

/* Momentum transfer boundaries */
extern double Q2_lboundary_FD, Q2_uboundary_FD;

/* TKI boundaries */
extern const double dP_T_boundary;

// Data runs (2 GeV) ----------------------------------------------------------------------------------------------------------------------------------------------------

// DataSample H1_data_2GeV;
// DataSample H1_data_2GeV.SampleDir = "/lustre24/expphy/cache/clas12/rg-m/production/pass1/2gev/H/dst/recon";
// DataSample H1_data_2GeV.SampleDir = "/lustre24/expphy/cache/clas12/rg-m/production/pass1/2gev/H/dst/recon";

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/2gev/H/dst/recon
extern std::vector<std::string> H1_data_2GeV_runs;

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/2gev/D/dst/recon
extern std::vector<std::string> D2_data_2GeV_runs;

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/2gev/C/dst/recon
extern std::vector<std::string> C12_data_2GeV_runs;

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/2gev/Ar/dst/recon
extern std::vector<std::string> Ar40_data_2GeV_runs;

// Data runs (4 GeV) ----------------------------------------------------------------------------------------------------------------------------------------------------

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/4gev/C/dst/recon
extern std::vector<std::string> C12_data_4GeV_runs;

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon
extern std::vector<std::string> Ar40_data_4GeV_runs;

// Data runs (6 GeV) ----------------------------------------------------------------------------------------------------------------------------------------------------

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/6gev/H/dst/recon
extern std::vector<std::string> H1_data_6GeV_runs;

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/6gev/D/dst/recon
extern std::vector<std::string> D2_data_6GeV_runs;

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/6gev/Cx4/dst/recon
extern std::vector<std::string> C12x4_data_6GeV_runs;

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/6gev/Ar/dst/recon
extern std::vector<std::string> Ar40_data_6GeV_runs;

};  // namespace lists

#endif  // LISTS_H
