//
// Created by Alon Sportes on 16/03/2025.
//

#ifndef RECO_ANALYSIS_FUNCTIONS_H
#define RECO_ANALYSIS_FUNCTIONS_H

#include <TH2.h>
#include <TVector3.h>

#include <tuple>
#include <vector>

// Include libraries:
#include "analysis_math/analysis_math.h"
#include "constants.h"

// Include CLAS12 libraries:
#include "CLAS12/clas12_include.h"

namespace reco_analysis_functions {

// GetFDNeutronP function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetFDNeutronP function */
double GetFDNeutronP(const clas12::region_part_ptr& Neutron, const bool& apply_nucleon_cuts);
#pragma endregion

// GetECALHitFlags function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetECALHitFlags function */
std::tuple<bool, bool, bool> GetECALHitFlags(const clas12::region_part_ptr& ParticleFD);
#pragma endregion

// CheckForNeutralFDECALHits function ------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CheckForNeutralFDECALHits function */
void CheckForNeutralFDECALHits(bool& ParticleInPCAL, bool& ParticleInECIN, bool& ParticleInECOUT, short& NeutralFD_ECAL_detlayer, const clas12::region_part_ptr& NeutralFD);
#pragma endregion

// CheckForECALHits function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CheckForECALHits function */
void CheckForECALHits(bool& ParticleInPCAL, bool& ParticleInECIN, bool& ParticleInECOUT, short& NeutralFD_ECAL_detlayer, const std::vector<clas12::region_part_ptr>& allParticles,
                      const int& i);
#pragma endregion

// CheckForECALHits function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CheckForECALHits function */
std::tuple<bool, bool, bool, short> CheckForECALHits(const clas12::region_part_ptr& ParticleFD);
#pragma endregion

// GetFTOFHitFlags function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetFTOFHitFlags function */
std::tuple<bool, bool, bool> GetFTOFHitFlags(const clas12::region_part_ptr& ParticleFD);
#pragma endregion

// CalcPathnFD function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcPathnFD function */
double CalcPathnFD(const clas12::region_part_ptr& NeutronFD, const clas12::region_part_ptr& electron, const bool& Recalc_Path_nFD = true);
#pragma endregion

// CalcPathnFD3v function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcPathnFD3v function */
TVector3 CalcPathnFD3v(const clas12::region_part_ptr& NeutronFD, const clas12::region_part_ptr& electron, const bool& Recalc_Path_nFD = true);
#pragma endregion

// CalcToFnFD function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcToFnFD function */
double CalcToFnFD(const clas12::region_part_ptr& NeutronFD, const double& starttime = 9999, const bool& Recalc_ToF_nFD = true);
#pragma endregion

// CalcBetanFD function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcBetanFD function */
double CalcBetanFD(const clas12::region_part_ptr& NeutronFD, const clas12::region_part_ptr& electron, const double& starttime = 9999, const bool& Recalc_Beta_nFD = true);
#pragma endregion

// CalcPnFD function -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcPnFD function */
double CalcPnFD(const clas12::region_part_ptr& NeutronFD, const clas12::region_part_ptr& electron, const double& starttime = 9999, const bool& Recalc_P_nFD = true);
#pragma endregion

// CalcPnFD3v function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcPnFD3v function */
TVector3 CalcPnFD3v(const clas12::region_part_ptr& NeutronFD, const clas12::region_part_ptr& electron, const double& starttime = 9999, const bool& Recalc_P_nFD = true);
#pragma endregion

// checkEcalDiagCuts function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* checkEcalDiagCuts function */
bool checkEcalDiagCuts(const clas12::region_part_ptr& electrons);
#pragma endregion

// DCEdgeCuts function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DCEdgeCuts function */
bool DCEdgeCuts(clas12::region_part_ptr p);
#pragma endregion

// fillDCdebug function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* fillDCdebug function */
void fillDCdebug(clas12::region_part_ptr p, TH2D** h, const double& weight);
#pragma endregion

// CalcSolenoidPhiShift function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcSolenoidPhiShift function */
double CalcSolenoidPhiShift(const clas12::region_part_ptr& charged_particle, const double& B, const double& L);
#pragma endregion

// GetShiftedUnitMomentum function ---------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetShiftedUnitMomentum function */
TVector3 GetShiftedUnitMomentum(const clas12::region_part_ptr& charged_particle, const TVector3& v_nhit_3v, const double& B, const double& L);
#pragma endregion

// NeutronECAL_Cut_Veto function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* NeutronECAL_Cut_Veto function */
bool NeutronECAL_Cut_Veto(const std::vector<clas12::region_part_ptr>& allParticles, const std::vector<clas12::region_part_ptr>& electrons, const double& beamE, const int& index,
                          const double& veto_cut);
#pragma endregion

// ChargedECALveto function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ChargedECALveto function */
bool ChargedECALveto(const std::vector<clas12::region_part_ptr>& allParticles, clas12::region_part_ptr& electron, const clas12::region_part_ptr& NeutronFD, const double& veto_cut,
                     const double& starttime = 9999, const bool& Recalc_P_nFD = false, const bool& apply_PCAL_veto = false);
#pragma endregion

// NeutralECALveto function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* NeutralECALveto function */
bool NeutralECALveto(const std::vector<clas12::region_part_ptr>& allParticles, clas12::region_part_ptr& electron, const clas12::region_part_ptr& NeutronFD, const double& veto_cut,
                     const double& starttime = 9999, const bool& Recalc_P_nFD = false, const bool& apply_PCAL_veto = false, const double B = constants::B, const double L = constants::L);
#pragma endregion

}  // namespace reco_analysis_functions

#endif  // RECO_ANALYSIS_FUNCTIONS_H
