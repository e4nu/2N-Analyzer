//
// Created by Alon Sportes on 17/03/2026.
//

#ifndef HISTOGRAMSETTINGS_H
#define HISTOGRAMSETTINGS_H

#include "AcceptanceMapsSettings.h"
#include "AnalysisCutSettings.h"
#include "EventSelectionSettings.h"

struct HistogramSettings {
    // ==================================================================================================================================================================
    // Plot selector variables
    // ==================================================================================================================================================================

    /* Here are boolean variables used to turn ON/OFF the different plots of the code.
       Plot_selector_master must remain true, set it OFF only for debugging. */

    // Plot selector variable declarations
    /* Master plots variable */
    bool Plot_selector_master = true;  // Master plot selector for analysis

    /* Cut variable plots */
    bool Cut_plots_master = true;                                                  // Master cut plots selector
    bool Vertex_plots = true, Vertex_corr_plots = true, DC_edge_plots = true;      // Preselection cuts
    bool Nphe_plots = true, Edep_plots = true, SF_plots = true, ECAL_edge = true;  // Electron quality cuts
    bool Chi2_plots = true, pCD_PID_plots = true, CD_edge_plots = true;            // Hadron PID cuts
    bool Momentum_plots = true;

    /* Reaction monitoring plots */
    bool ReacMon_plots = true;

    /* W plots */
    bool W_plots = true;

    /* Beta plots */
    bool Beta_plots = true;
    bool Beta_vs_P_plots = true;

    /* Angle plots */
    bool Angle_plots_master = true;  // Master angle plots selector
    bool Theta_e_plots = true, Phi_e_plots = true;

    /* Momentum transfer plots */
    bool Momentum_transfer_plots = true;

    /* E_e plots */
    bool E_e_plots = true;

    /* ET plots */
    bool ETrans_plots_master = true;  // Master ET plots selector
    bool ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;

    /* Ecal plots */
    bool Ecal_plots = true;

    /* Transverse variables plots */
    bool TKI_plots = true;

    /* ToF plots */
    bool ToF_plots = true;

    /* Efficiency plots */
    bool Efficiency_plots = true;
    bool TL_after_Acceptance_Maps_plots = true;

    /* Resolution plots */
    bool AMaps_plots = true;
    bool WMaps_plots = true;

    /* Resolution plots */
    bool Resolution_plots = true;

    /* Multiplicity plots */
    bool Multiplicity_plots = true;

    /* Final state ratio plots */
    bool FSR_1D_plots = true, FSR_2D_plots = true;  // FSR_2D_plots is disabled below if HipoChainLength is 2 or lower

    /* Other setup variables */
    bool wider_margin = true;
    bool debug_plots = true;  // Print out clas12ana debugging plots

    bool Log_scale_Vx_plots = true, Log_scale_Vy_plots = true, Log_scale_Vz_plots = true;
    bool Log_scale_dVx_plots = true, Log_scale_dVy_plots = true, Log_scale_dVz_plots = true;

    // ==================================================================================================================================================================
    // Normalization variables
    // ==================================================================================================================================================================

    // Normalization setup
    /* Here are boolean variables used to turn ON/OFF the different plot normalizations of the code.
     * Enable of presentations only, since event count is important otherwise. */

    bool normalize_master = false;

    // Preselection cuts:
    bool norm_Vertex_plots = false, norm_Vertex_corr_plots = false, norm_DC_edge_plots = false;

    // Electron quality cuts:
    bool norm_Nphe_plots = false, norm_Edep_plots = false, norm_SF_plots = false, norm_ECAL_edge = false;

    // Hadron PID cuts:
    bool norm_Chi2_plots = false, norm_pCD_PID_plots = false, norm_CD_edge_plots = false;

    bool norm_Momentum_plots = false;

    bool norm_ReacMon_plots = false;

    bool norm_W_plots = false, norm_Beta_plots = false, norm_Angle_plots_master = false, norm_Momentum_transfer_plots = false, norm_E_e_plots = false, norm_ET_plots = false;
    bool norm_Ecal_plots = false, norm_TKI_plots = false;

    bool norm_MomRes_plots = false, norm_Multi_plots = false;

    void ConfigurePlotSelectors(bool TestRun) {
        // Set enabled plots
        const bool FullRun = !TestRun;

        /* Master plots variable */
        Plot_selector_master = FullRun ? true : true;  // Master plot selector for analysis
        // Plot_selector_master = FullRun ? true : false;  // Master plot selector for analysis

        /* Cut variable plots */
        Cut_plots_master = FullRun ? true : true;  // Master cut plots selector
        // Cut_plots_master = FullRun ? true : false;  // Master cut plots selector

        // Preselection cuts:
        Vertex_plots = FullRun ? true : true, Vertex_corr_plots = FullRun ? true : true, DC_edge_plots = FullRun ? true : true;
        // Vertex_plots = FullRun ? true : false, Vertex_corr_plots = FullRun ? true : false, DC_edge_plots = FullRun ? true : false;

        // Electron quality cuts:
        Nphe_plots = FullRun ? true : true, Edep_plots = FullRun ? true : true, SF_plots = FullRun ? true : true, ECAL_edge = FullRun ? true : true;
        // Nphe_plots = FullRun ? true : false, Edep_plots = FullRun ? true : false, SF_plots = FullRun ? true : false, ECAL_edge = FullRun ? true : false;

        // Hadron PID cuts:
        Chi2_plots = FullRun ? true : true, pCD_PID_plots = FullRun ? true : true, CD_edge_plots = FullRun ? true : true;
        // Chi2_plots = FullRun ? true : false, pCD_PID_plots = FullRun ? true : false, CD_edge_plots = FullRun ? true : false;

        // Momentum_plots = FullRun ? true : true;
        Momentum_plots = FullRun ? true : false;

        /* Reaction monitoring plots */
        // ReacMon_plots = FullRun ? true : true;
        ReacMon_plots = FullRun ? true : false;

        /* W plots */
        // W_plots = FullRun ? true : true;
        W_plots = FullRun ? true : false;

        /* Beta plots */
        // Beta_plots = FullRun ? true : true;
        Beta_plots = FullRun ? true : false;
        // Beta_vs_P_plots = FullRun ? true : true;
        Beta_vs_P_plots = FullRun ? true : false;

        /* Angle plots */
        // Angle_plots_master = FullRun ? true : true; // Master angle plots selector
        // Theta_e_plots = FullRun ? true : true, Phi_e_plots = FullRun ? true : true;
        Angle_plots_master = FullRun ? true : false;  // Master angle plots selector
        Theta_e_plots = FullRun ? true : false, Phi_e_plots = FullRun ? true : false;

        /* Momentum transfer plots */
        // Momentum_transfer_plots = FullRun ? true : true;
        Momentum_transfer_plots = FullRun ? true : false;

        /* E_e plots */
        // E_e_plots = FullRun ? true : true;
        E_e_plots = FullRun ? true : false;

        /* ET plots */
        // ETrans_plots_master = FullRun ? true : true; // Master ET plots selector
        ETrans_plots_master = FullRun ? true : false;  // Master ET plots selector
        ETrans_all_plots = FullRun ? true : true;
        ETrans_All_Int_plots = FullRun ? true : true;
        ETrans_QEL_plots = FullRun ? true : true;
        ETrans_MEC_plots = FullRun ? true : true;
        ETrans_RES_plots = FullRun ? true : true;
        ETrans_DIS_plots = FullRun ? true : true;

        /* Ecal plots */
        // Ecal_plots = FullRun ? true : true;
        Ecal_plots = FullRun ? true : false;

        /* Transverse variables plots */
        // TKI_plots = FullRun ? true : true;
        TKI_plots = FullRun ? true : false;

        /* ToF plots */
        // ToF_plots = FullRun ? true : true;
        ToF_plots = FullRun ? true : false;

        /* Efficiency plots */
        // Efficiency_plots = FullRun ? true : true;
        Efficiency_plots = FullRun ? true : false;
        // TL_after_Acceptance_Maps_plots = FullRun ? true : true;
        TL_after_Acceptance_Maps_plots = FullRun ? true : false;

        /* Resolution plots */
        // AMaps_plots = FullRun ? true : true;
        AMaps_plots = FullRun ? true : false;
        // WMaps_plots = FullRun ? true : true;
        WMaps_plots = FullRun ? true : false;

        /* Resolution plots */
        Resolution_plots = FullRun ? true : true;
        // Resolution_plots = FullRun ? true : false;

        /* Multiplicity plots */
        // Multiplicity_plots = FullRun ? true : true;
        Multiplicity_plots = FullRun ? true : false;

        /* Final state ratio plots */
        // FSR_1D_plots = FullRun ? true : true;
        // FSR_2D_plots = FullRun ? true : true;  // disabled below if HipoChainLength is 2 or lower
        FSR_1D_plots = FullRun ? true : false;
        FSR_2D_plots = FullRun ? true : false;  // disabled below if HipoChainLength is less than 100
    }

    void ApplyPlotSelectorDependencies(const EventSelectionSettings& ESSettings, const AcceptanceMapsSettings& AMapsSettings, const AnalysisCutSettings& CutSettings) {
        // Auto-disable plot selector variables
        if (!Plot_selector_master) {
            Cut_plots_master = ReacMon_plots = W_plots = Beta_plots = Beta_vs_P_plots = Angle_plots_master = Momentum_transfer_plots = E_e_plots = ETrans_plots_master = Ecal_plots = false;
            TKI_plots = ToF_plots = Efficiency_plots = AMaps_plots = WMaps_plots = Resolution_plots = Multiplicity_plots = false;
            FSR_1D_plots = FSR_2D_plots = false;
        }

        if (!Cut_plots_master) {
            Vertex_plots = Vertex_corr_plots = DC_edge_plots = false;  // Preselection cuts
            Nphe_plots = Edep_plots = SF_plots = ECAL_edge = false;    // Electron quality cuts
            Chi2_plots = pCD_PID_plots = CD_edge_plots = false;        // Hadron PID cuts
            Momentum_plots = false;
        }

        if (!Angle_plots_master) { Theta_e_plots = Phi_e_plots = false; }

        if (!ETrans_plots_master) { ETrans_all_plots = ETrans_QEL_plots = ETrans_MEC_plots = ETrans_RES_plots = ETrans_DIS_plots = false; }

        if (!ESSettings.fill_TL_plots) { Efficiency_plots = TL_after_Acceptance_Maps_plots = false; }

        if (!AMapsSettings.Generate_Electron_AMaps && !AMapsSettings.Generate_Nucleon_AMaps) { AMaps_plots = false; }

        if (!AMapsSettings.Generate_WMaps) { WMaps_plots = false; }

        if (!CutSettings.apply_nucleon_cuts || (AMapsSettings.Electron_single_slice_test || AMapsSettings.Nucleon_single_slice_test)) { FSR_1D_plots = FSR_2D_plots = false; }
    }

    void ApplyNormalizationDependencies() {
        // Auto-disable all plot normalization variables if normalize_master is false:
        if (!normalize_master) {
            norm_Vertex_plots = norm_Vertex_corr_plots = norm_DC_edge_plots = false;     // Preselection cuts
            norm_Nphe_plots = norm_Edep_plots = norm_SF_plots = norm_ECAL_edge = false;  // Electron quality cuts
            norm_Chi2_plots = norm_pCD_PID_plots = norm_CD_edge_plots = false;           // Hadron PID cuts
            norm_Momentum_plots = false;
            norm_ReacMon_plots = false;
            norm_W_plots = norm_Beta_plots = norm_Angle_plots_master = norm_Momentum_transfer_plots = norm_E_e_plots = norm_ET_plots = norm_Ecal_plots = norm_TKI_plots = false;
            norm_MomRes_plots = norm_Multi_plots = false;
        }
    }
};

#endif  // HISTOGRAMSETTINGS_H
