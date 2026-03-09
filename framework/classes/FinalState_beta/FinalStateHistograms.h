//
// Created by Alon Sportes on 30/04/2025.
//

#ifndef FINALSTATEHISTOGRAMS_H
#define FINALSTATEHISTOGRAMS_H

#include "TH1D.h"
#include "TH2D.h"

// Dedicated histogram class for pFDpCD
class FinalStateHistograms {
   public:
    FinalStateHistograms();
    virtual ~FinalStateHistograms();
    // TH1D histograms
    TH1D* hNphe_pFDpCD_FD = nullptr;
    TH1D* hChi2_Electron_pFDpCD_FD = nullptr;
    TH1D* hChi2_Proton_pFDpCD_FD = nullptr;
    TH1D* hChi2_Proton_pFDpCD_CD = nullptr;
    TH1D* hP_pFDpCD_FD = nullptr;
    TH1D* hP_pFDpCD_CD = nullptr;
    TH1D* hTheta_pFDpCD_FD = nullptr;
    TH1D* hTheta_pFDpCD_CD = nullptr;
    TH1D* hPhi_pFDpCD_FD = nullptr;
    TH1D* hPhi_pFDpCD_CD = nullptr;
    TH1D* hPmiss_pFDpCD = nullptr;
    TH1D* hEmiss_pFDpCD = nullptr;
    TH1D* hQ2_pFDpCD = nullptr;
    TH1D* hxB_pFDpCD = nullptr;
    TH1D* hW_pFDpCD = nullptr;
    TH1D* hTheta_q_pFDpCD = nullptr;
    TH1D* hAlpha_pFDpCD = nullptr;
    TH1D* hPtot_pFDpCD = nullptr;
    TH1D* hPrel_pFDpCD = nullptr;
    TH1D* hTheta_p_e_p_tot_pFDpCD = nullptr;
    TH1D* hTheta_q_p_tot_pFDpCD = nullptr;
    TH1D* hEcal_pFDpCD = nullptr;
    TH1D* hTheta_pFD_pCD_pFDpCD = nullptr;
    TH1D* hVz_e_pFDpCD = nullptr;
    TH1D* hVz_pFD_pFDpCD = nullptr;
    TH1D* hVz_pCD_pFDpCD = nullptr;
    TH1D* hEoP_e_pFDpCD = nullptr;
    TH1D* hP_pL_pFDpCD = nullptr;
    TH1D* hP_pR_pFDpCD = nullptr;
    TH1D* hTheta_pL_pFDpCD = nullptr;
    TH1D* hTheta_pR_pFDpCD = nullptr;
    TH1D* hPhi_pL_pFDpCD = nullptr;
    TH1D* hPhi_pR_pFDpCD = nullptr;
    TH1D* hVz_pL_pFDpCD = nullptr;
    TH1D* hVz_pR_pFDpCD = nullptr;
    TH1D* hTheta_q_pL_pFDpCD = nullptr;
    TH1D* hTheta_q_pR_pFDpCD = nullptr;
    TH1D* hAlpha_pL_pFDpCD = nullptr;
    TH1D* hAlpha_pR_pFDpCD = nullptr;
    TH1D* hP_T_pL_pFDpCD = nullptr;
    TH1D* hP_T_pR_pFDpCD = nullptr;
    TH1D* hP_T_tot_pFDpCD = nullptr;
    TH1D* hdP_T_L_pFDpCD = nullptr;
    TH1D* hdP_T_tot_pFDpCD = nullptr;
    TH1D* hdAlpha_T_L_pFDpCD = nullptr;
    TH1D* hdAlpha_T_tot_pFDpCD = nullptr;
    TH1D* hdPhi_T_L_pFDpCD = nullptr;
    TH1D* hdPhi_T_tot_pFDpCD = nullptr;
    TH1D* hPhi_L_pFDpCD = nullptr;
    TH1D* hPhi_R_pFDpCD = nullptr;
    TH1D* hP_miss_in_QE_range_pFDpCD = nullptr;
    TH1D* hP_miss_in_MECandSRC_range_pFDpCD = nullptr;
    TH1D* hE_miss_in_QE_range_pFDpCD = nullptr;
    TH1D* hE_miss_in_MECandSRC_range_pFDpCD = nullptr;
    TH1D* hxB_in_QE_range_pFDpCD = nullptr;
    TH1D* htheta_q_pCD_in_lower_FSI_range_pFDpCD = nullptr;
    TH1D* hPass_ReacMon_cuts_pFDpCD = nullptr;
    // TH2D histograms (names containing VS)
    TH2D* hQ2_VS_W_pFDpCD = nullptr;
    TH2D* hQ2_VS_xB_pFDpCD = nullptr;
    TH2D* hW_VS_xB_pFDpCD = nullptr;
    TH2D* hPmiss_VS_Emiss_pFDpCD = nullptr;
    TH2D* hPmiss_VS_Q2_pFDpCD = nullptr;
    TH2D* hPmiss_VS_xB_pFDpCD = nullptr;
    TH2D* hEmiss_VS_Q2_pFDpCD = nullptr;
    TH2D* hEmiss_VS_xB_pFDpCD = nullptr;
    TH2D* hPmiss_VS_W_pFDpCD = nullptr;
    TH2D* hEmiss_VS_W_pFDpCD = nullptr;
    TH2D* hTheta_q_pFDpCD_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hAlpha_pFDpCD_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hTheta_p_e_p_tot_pFDpCD_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hTheta_q_p_tot_pFDpCD_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hPtot_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hPrel_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hTheta_pFD_pCD_pFDpCD_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hVz_e_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hVz_pFD_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hVz_pCD_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hEoP_e_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hP_pL_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hP_pR_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hTheta_pL_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hTheta_pR_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hPhi_pL_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hPhi_pR_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hVz_pL_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hVz_pR_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hTheta_q_pL_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hTheta_q_pR_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hAlpha_pL_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hAlpha_pR_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hP_T_pL_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hP_T_pR_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hP_T_tot_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hdP_T_L_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hdP_T_tot_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hdAlpha_T_L_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hdAlpha_T_tot_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hdPhi_T_L_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hdPhi_T_tot_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hPhi_L_VS_Pmiss_pFDpCD = nullptr;
    TH2D* hPhi_R_VS_Pmiss_pFDpCD = nullptr;
};

#endif  // FINALSTATEHISTOGRAMS_H
