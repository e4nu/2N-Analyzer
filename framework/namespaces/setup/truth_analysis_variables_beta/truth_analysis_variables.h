//
// Created by Alon Sportes on 28/03/2025.
//

#ifndef TRUTH_ANALYSIS_VARIABLES_H
#define TRUTH_ANALYSIS_VARIABLES_H

#include <iostream>

// TODO: split into header and source files!

namespace truth_analysis_variables {
// ======================================================================================================================================================================
// Histogram ranges
// ======================================================================================================================================================================

//<editor-fold desc="Histogram range variables">

////<editor-fold desc="Chi2 plots">
// extern double Chi2_upper_lim, Chi2_lower_lim;
////</editor-fold>

//<editor-fold desc="Vertex plots">
extern double Vertex_upper_lim, Vertex_lower_lim, dV_upper_lim, dV_lower_lim;
//</editor-fold>

//<editor-fold desc="Theta histograms">

//<editor-fold desc="Theta histograms (2p)">
extern double theta_lp_upper_lim_2p, theta_lp_lower_lim_2p, theta_p1_upper_lim_2p, theta_p1_lower_lim_2p, theta_p2_upper_lim_2p, theta_p2_lower_lim_2p;
extern double dtheta_upper_lim_2p, dtheta_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Theta histograms (1n1p)">
extern double theta_lp_upper_lim_1n1p, theta_lp_lower_lim_1n1p, theta_p_upper_lim_1n1p, theta_p_lower_lim_1n1p, theta_n_upper_lim_1n1p, theta_n_lower_lim_1n1p;
extern double dtheta_upper_lim_1n1p, dtheta_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Phi histograms">

//<editor-fold desc="Phi histograms (2p)">
extern double phi_lp_upper_lim_2p, phi_lp_lower_lim_2p, phi_p1_upper_lim_2p, phi_p1_lower_lim_2p, phi_p2_upper_lim_2p, phi_p2_lower_lim_2p;
extern double dphi_upper_lim_2p, dphi_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Phi histograms (1n1p)">
extern double phi_lp_upper_lim_1n1p, phi_lp_lower_lim_1n1p, phi_p_upper_lim_1n1p, phi_p_lower_lim_1n1p, phi_n_upper_lim_1n1p, phi_n_lower_lim_1n1p;
extern double dphi_upper_lim_1n1p, dphi_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy histograms">

//<editor-fold desc="Energy histograms (2p)">
extern double fsEl_upper_lim_2p, fsEl_lower_lim_2p;
extern double fsEl_QEL_upper_lim_2p, fsEl_QEL_lower_lim_2p, fsEl_MEC_upper_lim_2p, fsEl_MEC_lower_lim_2p;
extern double fsEl_RES_upper_lim_2p, fsEl_RES_lower_lim_2p, fsEl_DIS_upper_lim_2p, fsEl_DIS_lower_lim_2p;
extern double fsEl_VS_theta_lp_upper_lim_2p_x, fsEl_VS_theta_lp_lower_lim_2p_x, fsEl_VS_theta_lp_upper_lim_2p_y, fsEl_VS_theta_lp_lower_lim_2p_y;
//</editor-fold>

//<editor-fold desc="Energy histograms (1n1p)">
extern double fsEl_upper_lim_1n1p, fsEl_lower_lim_1n1p;
extern double fsEl_QEL_upper_lim_1n1p, fsEl_QEL_lower_lim_1n1p, fsEl_MEC_upper_lim_1n1p, fsEl_MEC_lower_lim_1n1p;
extern double fsEl_RES_upper_lim_1n1p, fsEl_RES_lower_lim_1n1p, fsEl_DIS_upper_lim_1n1p, fsEl_DIS_lower_lim_1n1p;
extern double fsEl_VS_theta_lp_upper_lim_1n1p_x, fsEl_VS_theta_lp_lower_lim_1n1p_x, fsEl_VS_theta_lp_upper_lim_1n1p_y, fsEl_VS_theta_lp_lower_lim_1n1p_y;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms">

//<editor-fold desc="Energy Transfer histograms (all interactions)">

//<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
extern double E_Trans_all_ang_all_int_upper_lim_2p, E_Trans_all_ang_all_int_lower_lim_2p;
extern double E_Trans15_all_upper_lim_2p, E_Trans15_all_lower_lim_2p, E_Trans45_all_upper_lim_2p, E_Trans45_all_lower_lim_2p, E_Trans90_all_upper_lim_2p, E_Trans90_all_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
extern double E_Trans_all_ang_all_int_upper_lim_1n1p, E_Trans_all_ang_all_int_lower_lim_1n1p;
extern double E_Trans15_all_upper_lim_1n1p, E_Trans15_all_lower_lim_1n1p, E_Trans45_all_upper_lim_1n1p, E_Trans45_all_lower_lim_1n1p, E_Trans90_all_upper_lim_1n1p,
    E_Trans90_all_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (QEL only)">

//<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
extern double E_Trans15_QEL_upper_lim_2p, E_Trans15_QEL_lower_lim_2p, E_Trans45_QEL_upper_lim_2p, E_Trans45_QEL_lower_lim_2p, E_Trans90_QEL_upper_lim_2p, E_Trans90_QEL_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
extern double E_Trans15_QEL_upper_lim_1n1p, E_Trans15_QEL_lower_lim_1n1p, E_Trans45_QEL_upper_lim_1n1p, E_Trans45_QEL_lower_lim_1n1p, E_Trans90_QEL_upper_lim_1n1p,
    E_Trans90_QEL_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (MEC only)">

//<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
extern double E_Trans15_MEC_upper_lim_2p, E_Trans15_MEC_lower_lim_2p, E_Trans45_MEC_upper_lim_2p, E_Trans45_MEC_lower_lim_2p, E_Trans90_MEC_upper_lim_2p, E_Trans90_MEC_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
extern double E_Trans15_MEC_upper_lim_1n1p, E_Trans15_MEC_lower_lim_1n1p, E_Trans45_MEC_upper_lim_1n1p, E_Trans45_MEC_lower_lim_1n1p, E_Trans90_MEC_upper_lim_1n1p,
    E_Trans90_MEC_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (RES only)">

//<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
extern double E_Trans15_RES_upper_lim_2p, E_Trans15_RES_lower_lim_2p, E_Trans45_RES_upper_lim_2p, E_Trans45_RES_lower_lim_2p, E_Trans90_RES_upper_lim_2p, E_Trans90_RES_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
extern double E_Trans15_RES_upper_lim_1n1p, E_Trans15_RES_lower_lim_1n1p, E_Trans45_RES_upper_lim_1n1p, E_Trans45_RES_lower_lim_1n1p, E_Trans90_RES_upper_lim_1n1p,
    E_Trans90_RES_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (DIS only)">

//<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
extern double E_Trans15_DIS_upper_lim_2p, E_Trans15_DIS_lower_lim_2p, E_Trans45_DIS_upper_lim_2p, E_Trans45_DIS_lower_lim_2p, E_Trans90_DIS_upper_lim_2p, E_Trans90_DIS_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
extern double E_Trans15_DIS_upper_lim_1n1p, E_Trans15_DIS_lower_lim_1n1p, E_Trans45_DIS_upper_lim_1n1p, E_Trans45_DIS_lower_lim_1n1p, E_Trans90_DIS_upper_lim_1n1p,
    E_Trans90_DIS_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//</editor-fold>

//<editor-fold desc="Inclusive Energy Transfer histograms">
extern double E_Trans15_all_inclusive_upper_lim, E_Trans15_all_inclusive_lower_lim;
extern double E_Trans15_QEL_inclusive_upper_lim, E_Trans15_QEL_inclusive_lower_lim, E_Trans15_MEC_inclusive_upper_lim, E_Trans15_MEC_inclusive_lower_lim;
extern double E_Trans15_RES_inclusive_upper_lim, E_Trans15_RES_inclusive_lower_lim, E_Trans15_DIS_inclusive_upper_lim, E_Trans15_DIS_inclusive_lower_lim;
extern double E_Trans_VS_q_all_inclusive_upper_lim_x, E_Trans_VS_q_all_inclusive_lower_lim_x, E_Trans_VS_q_all_inclusive_upper_lim_y, E_Trans_VS_q_all_inclusive_lower_lim_y;
extern double E_Trans_VS_q_QEL_inclusive_upper_lim_x, E_Trans_VS_q_QEL_inclusive_lower_lim_x, E_Trans_VS_q_QEL_inclusive_upper_lim_y, E_Trans_VS_q_QEL_inclusive_lower_lim_y;
extern double E_Trans_VS_q_MEC_inclusive_upper_lim_x, E_Trans_VS_q_MEC_inclusive_lower_lim_x, E_Trans_VS_q_MEC_inclusive_upper_lim_y, E_Trans_VS_q_MEC_inclusive_lower_lim_y;

extern double E_Trans_VS_q3_all_upper_lim_x_2p, E_Trans_VS_q3_all_lower_lim_x_2p, E_Trans_VS_q3_all_upper_lim_y_2p, E_Trans_VS_q3_all_lower_lim_y_2p;
extern double E_Trans_VS_q3_QEL_upper_lim_x_2p, E_Trans_VS_q3_QEL_lower_lim_x_2p, E_Trans_VS_q3_QEL_upper_lim_y_2p, E_Trans_VS_q3_QEL_lower_lim_y_2p;
extern double E_Trans_VS_q3_MEC_upper_lim_x_2p, E_Trans_VS_q3_MEC_lower_lim_x_2p, E_Trans_VS_q3_MEC_upper_lim_y_2p, E_Trans_VS_q3_MEC_lower_lim_y_2p;

extern double E_Trans_VS_q3_all_upper_lim_x_1n1p, E_Trans_VS_q3_all_lower_lim_x_1n1p, E_Trans_VS_q3_all_upper_lim_y_1n1p, E_Trans_VS_q3_all_lower_lim_y_1n1p;
extern double E_Trans_VS_q3_QEL_upper_lim_x_1n1p, E_Trans_VS_q3_QEL_lower_lim_x_1n1p, E_Trans_VS_q3_QEL_upper_lim_y_1n1p, E_Trans_VS_q3_QEL_lower_lim_y_1n1p;
extern double E_Trans_VS_q3_MEC_upper_lim_x_1n1p, E_Trans_VS_q3_MEC_lower_lim_x_1n1p, E_Trans_VS_q3_MEC_upper_lim_y_1n1p, E_Trans_VS_q3_MEC_lower_lim_y_1n1p;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms">

//<editor-fold desc="E_cal range">
extern double E_cal_QEL_upper_lim_range, E_cal_QEL_lower_lim_range, E_cal_MEC_upper_lim_range, E_cal_MEC_lower_lim_range;
extern double E_cal_RES_upper_lim_range, E_cal_RES_lower_lim_range, E_cal_DIS_upper_lim_range, E_cal_DIS_lower_lim_range;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms (2p)">
extern double E_cal_QEL_upper_lim_2p, E_cal_QEL_lower_lim_2p, E_cal_MEC_upper_lim_2p, E_cal_MEC_lower_lim_2p;
extern double E_cal_RES_upper_lim_2p, E_cal_RES_lower_lim_2p, E_cal_DIS_upper_lim_2p, E_cal_DIS_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms (1n1p)">
extern double E_cal_QEL_upper_lim_1n1p, E_cal_QEL_lower_lim_1n1p, E_cal_MEC_upper_lim_1n1p, E_cal_MEC_lower_lim_1n1p;
extern double E_cal_RES_upper_lim_1n1p, E_cal_RES_lower_lim_1n1p, E_cal_DIS_upper_lim_1n1p, E_cal_DIS_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Momentum histograms">

//<editor-fold desc="Momentum histograms (2p)">
extern double P_L_hist_upper_lim_2p, P_L_hist_lower_lim_2p, P_R_hist_upper_lim_2p, P_R_hist_lower_lim_2p, P_lp_hist_upper_lim_2p, P_lp_hist_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Momentum histograms (1n1p)">
extern double P_p_hist_upper_lim_1n1p, P_p_hist_lower_lim_1n1p, P_n_hist_upper_lim_1n1p, P_n_hist_lower_lim_1n1p, P_lp_hist_upper_lim_1n1p, P_lp_hist_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="MicroBooNE plots">

//<editor-fold desc="MicroBooNE gamma plots (unweighted)">
extern double Gamma_Lab_upper_lim, Gamma_Lab_lower_lim, Gamma_mu_p_tot_upper_lim, Gamma_mu_p_tot_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE gamma plots (Q4 weighted)">
extern double Gamma_Lab_weighted_upper_lim, Gamma_Lab_weighted_lower_lim, Gamma_mu_p_tot_weighted_upper_lim, Gamma_mu_p_tot_weighted_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE gamma plots (all int., no pions)">
extern double Gamma_Lab_noPions_All_Int_upper_lim, Gamma_Lab_noPions_All_Int_lower_lim, Gamma_Lab_noPions_All_Int_weighted_upper_lim, Gamma_Lab_noPions_All_Int_weighted_lower_lim;
extern double Gamma_Lab_noPions_QEL_upper_lim, Gamma_Lab_noPions_QEL_lower_lim, Gamma_Lab_noPions_QEL_weighted_upper_lim, Gamma_Lab_noPions_QEL_weighted_lower_lim;
extern double Gamma_Lab_noPions_MEC_upper_lim, Gamma_Lab_noPions_MEC_lower_lim, Gamma_Lab_noPions_MEC_weighted_upper_lim, Gamma_Lab_noPions_MEC_weighted_lower_lim;
extern double Gamma_Lab_noPions_RES_upper_lim, Gamma_Lab_noPions_RES_lower_lim, Gamma_Lab_noPions_RES_weighted_upper_lim, Gamma_Lab_noPions_RES_weighted_lower_lim;
extern double Gamma_Lab_noPions_DIS_upper_lim, Gamma_Lab_noPions_DIS_lower_lim, Gamma_Lab_noPions_DIS_weighted_upper_lim, Gamma_Lab_noPions_DIS_weighted_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE dP_T plots">
extern double dP_T_hist_upper_lim, dP_T_hist_lower_lim, dP_T_hist_weighted_upper_lim, dP_T_hist_weighted_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE momentum plots (for self-examination)">
extern double P_L_hist_upper_lim, P_L_hist_lower_lim, P_R_hist_upper_lim, P_R_hist_lower_lim, P_lp_hist_upper_lim, P_lp_hist_lower_lim, P_pion_hist_upper_lim, P_pion_hist_lower_lim;
//</editor-fold>

//</editor-fold>

//</editor-fold>

//</editor-fold>

};  // namespace truth_analysis_variables

#endif  // TRUTH_ANALYSIS_VARIABLES_H
