//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef VARIABLE_CORRECTORS_H
#define VARIABLE_CORRECTORS_H

#include <TH1D.h>
#include <TH2D.h>
#include <TObject.h>

#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "../basic_tools.h"
#include "../constants.h"

class TGraphErrors;

namespace bt = basic_tools;

namespace variable_correctors {

// GetPeakFromHighestBin function ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetPeakFromHighestBin function */
std::pair<double, double> GetPeakFromHighestBin(TH1D* hist);
#pragma endregion

// FitPeakToGaussian function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* FitPeakToGaussian function */
std::pair<double, double> FitPeakToGaussian(TH1D* hist, std::vector<double> fitLimits = {});
#pragma endregion

// FitVertexVsPhi function -----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* FitVertexVsPhi function */
std::tuple<double, double, double, TGraphErrors*> FitVertexVsPhi(std::string Particle, std::string SampleName, TObject* h_Vz_VS_phi_AllSectors,
                                                                 const std::vector<TH1D*>& Zrec_BySector_HistList, const std::vector<TH1D*>& Phi_BySector_HistList = {},
                                                                 const std::pair<double, double>& theta_slice = {-1, -1});
#pragma endregion

}  // namespace variable_correctors

#endif  // VARIABLE_CORRECTORS_H
