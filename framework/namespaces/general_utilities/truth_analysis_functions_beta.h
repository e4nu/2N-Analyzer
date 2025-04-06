//
// Created by Alon Sportes on 16/03/2025.
//

#ifndef TRUTH_ANALYSIS_FUNCTIONS_H
#define TRUTH_ANALYSIS_FUNCTIONS_H

#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TTree.h>
#include <math.h>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <typeinfo>
#include <vector>

#include "../analysis_math/analysis_math.h"
#include "../classes/DSCuts/DSCuts.h"
#include "../constants.h"
#include "../lists.h"
#include "GeneralFunctions.h"
#include "clas12reader.h"

using namespace lists;

namespace truth_analysis_functions {
#pragma region /* Particle vectors */
/* Particle index vectors */
vector<int> TL_Electron_ind, TL_Neutrons_ind, TL_Protons_ind, TL_piplus_ind, TL_piminus_ind, TL_pizero_ind, TL_Photons_ind, TL_OtherPart_ind;

/* Particle index vectors (for FD particles) */
vector<int> TL_ElectronFD_ind, TL_IDed_neutrons_FD, TL_ProtonsFD_ind, TL_ProtonsCD_ind, TL_pi0FD_ind, TL_PhotonsFD_ind;

/* Particle index vectors (for particles above momentum threshold) */
vector<int> TL_Electron_mom_ind, TL_Neutrons_mom_ind, TL_Protons_mom_ind, TL_piplus_mom_ind, TL_piminus_mom_ind, TL_pizero_mom_ind, TL_Photons_mom_ind;

/* Particle index vectors (for FD particles above momentum threshold) */
vector<int> TL_ElectronFD_mom_ind, TL_NeutronsFD_mom_ind, TL_ProtonsFD_mom_ind, TL_ProtonsCD_mom_ind, TL_pi0FD_mom_ind, TL_PhotonsFD_mom_ind;
#pragma endregion

#pragma region /* Basic event selection variables */
/* Setting up basic TL event selection */
bool no_TL_cPions, no_TL_OtherPart, no_TL_FDpi0, no_TL_FDPhotons, TL_Event_Selection_1e_cut, TL_Basic_ES;

/* Setting up 1p TL event selection */
bool one_FDproton_1p;

/* Setting up 1n TL event selection */
bool one_FDNeutron_1n, no_protons_1n;

/* Setting up pFDpCD TL event selection */
bool one_FDproton_pFDpCD, one_CDproton_pFDpCD;

/* Setting up nFDpCD TL event selection */
bool one_FDNeutron_nFDpCD, one_proton_nFDpCD, no_FDproton_nFDpCD, one_CDproton_nFDpCD;
#pragma endregion

#pragma region /* Event selection variables */
// 1p = one id. FD proton:
bool TL_Event_Selection_1p;

// 1n = one id. FD neutron & no id. protons:
bool TL_Event_Selection_1n;

// pFDpCD = One id. FD proton & one id. CD proton:
bool TL_Event_Selection_pFDpCD;

// nFDpCD = One id. FD neutron & one id. CD proton:
bool TL_Event_Selection_nFDpCD;
#pragma endregion

};  // namespace truth_analysis_functions

#endif  // TRUTH_ANALYSIS_FUNCTIONS_H
