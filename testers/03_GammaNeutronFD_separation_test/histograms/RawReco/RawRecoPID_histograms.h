#pragma once

#include <cmath>
#include <string>
#include <vector>

#include "TH1D.h"
#include "TH2D.h"
#include "TObject.h"
#include "TVector3.h"

// Include libraries:
#include "../../../../framework/namespaces/general_utilities/utilities.h"

// Include CLAS12 libraries:
#include "../../../../framework/includes/clas12_include.h"

namespace raf = reco_analysis_functions;

struct RawRecoPID_histograms {
    // =========================
    // Electron pre-selection and PID Histograms
    // =========================
    TH1D *h_Vz_e_BC_1e_cut, *h_Vz_e_AC_1e_cut;

    TH2D* h_dc_electron_hit_map_BC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // use 1..3
    TH2D* h_dc_electron_hit_map_AC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // use 1..3

    TH1D *h_nphe_BC_1e_cut, *h_nphe_AC_1e_cut;

    TH2D *h_Edep_PCAL_VS_EC_BC_1e_cut, *h_Edep_PCAL_VS_EC_AC_1e_cut;

    TH2D *h_SF_VS_Edep_PCAL_BC_AllSectors_1e_cut, *h_SF_VS_Edep_PCAL_AC_AllSectors_1e_cut;
    TH2D* h_SF_VS_Edep_PCAL_BC_BySector_1e_cut[7] = {nullptr};  // use 1..6
    TH2D* h_SF_VS_Edep_PCAL_AC_BySector_1e_cut[7] = {nullptr};  // use 1..6

    TH2D *h_SF_VS_P_e_BC_AllSectors_1e_cut, *h_SF_VS_P_e_AC_AllSectors_1e_cut;
    TH2D* h_SF_VS_P_e_BC_BySector_1e_cut[7] = {nullptr};  // use 1..6
    TH2D* h_SF_VS_P_e_AC_BySector_1e_cut[7] = {nullptr};  // use 1..6

    TH2D *h_SF_VS_Lv_BC_1e_cut, *h_SF_VS_Lv_AC_1e_cut;

    TH2D *h_SF_VS_Lw_BC_1e_cut, *h_SF_VS_Lw_AC_1e_cut;

    TH2D *h_SF_VS_Lu_BC_1e_cut, *h_SF_VS_Lu_AC_1e_cut;

    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut, *h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut;

    // =========================
    // Proton pre-selection and PID Histograms
    // =========================
    TH1D *h_Vz_pFD_BC_1e_cut, *h_Vz_pFD_AC_1e_cut;
    TH1D *h_dVz_pFD_BC_1e_cut, *h_dVz_pFD_AC_1e_cut;
    TH1D *h_chi2_pFD_BC_1e_cut, *h_chi2_pFD_AC_1e_cut;

    TH1D *h_Vz_pCD_BC_1e_cut, *h_Vz_pCD_AC_1e_cut;
    TH1D *h_dVz_pCD_BC_1e_cut, *h_dVz_pCD_AC_1e_cut;
    TH1D *h_chi2_pCD_BC_1e_cut, *h_chi2_pCD_AC_1e_cut;

    TH2D* h_dc_pFD_hit_map_BC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // 1..3
    TH2D* h_dc_pFD_hit_map_AC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // 1..3

    // =========================
    // Pi+ PID Histograms
    // =========================
    TH1D *h_Vz_pipFD_BC_1e_cut, *h_Vz_pipFD_AC_1e_cut;
    TH1D *h_dVz_pipFD_BC_1e_cut, *h_dVz_pipFD_AC_1e_cut;
    TH1D *h_chi2_pipFD_BC_1e_cut, *h_chi2_pipFD_AC_1e_cut;

    TH1D *h_Vz_pipCD_BC_1e_cut, *h_Vz_pipCD_AC_1e_cut;
    TH1D *h_dVz_pipCD_BC_1e_cut, *h_dVz_pipCD_AC_1e_cut;
    TH1D *h_chi2_pipCD_BC_1e_cut, *h_chi2_pipCD_AC_1e_cut;

    TH2D* h_dc_pipFD_hit_map_BC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // 1..3
    TH2D* h_dc_pipFD_hit_map_AC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // 1..3

    // =========================
    // Pi- PID Histograms
    // =========================
    TH1D *h_Vz_pimFD_BC_1e_cut, *h_Vz_pimFD_AC_1e_cut;
    TH1D *h_dVz_pimFD_BC_1e_cut, *h_dVz_pimFD_AC_1e_cut;
    TH1D *h_chi2_pimFD_BC_1e_cut, *h_chi2_pimFD_AC_1e_cut;

    TH1D *h_Vz_pimCD_BC_1e_cut, *h_Vz_pimCD_AC_1e_cut;
    TH1D *h_dVz_pimCD_BC_1e_cut, *h_dVz_pimCD_AC_1e_cut;
    TH1D *h_chi2_pimCD_BC_1e_cut, *h_chi2_pimCD_AC_1e_cut;

    TH2D* h_dc_pimFD_hit_map_BC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // 1..3
    TH2D* h_dc_pimFD_hit_map_AC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // 1..3

    // =========================
    // K+ PID Histograms
    // =========================
    TH1D *h_Vz_KpFD_BC_1e_cut, *h_Vz_KpFD_AC_1e_cut;
    TH1D *h_dVz_KpFD_BC_1e_cut, *h_dVz_KpFD_AC_1e_cut;
    TH1D *h_chi2_KpFD_BC_1e_cut, *h_chi2_KpFD_AC_1e_cut;

    TH1D *h_Vz_KpCD_BC_1e_cut, *h_Vz_KpCD_AC_1e_cut;
    TH1D *h_dVz_KpCD_BC_1e_cut, *h_dVz_KpCD_AC_1e_cut;
    TH1D *h_chi2_KpCD_BC_1e_cut, *h_chi2_KpCD_AC_1e_cut;

    TH2D* h_dc_KpFD_hit_map_BC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // 1..3
    TH2D* h_dc_KpFD_hit_map_AC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // 1..3

    // =========================
    // K- PID Histograms
    // =========================
    TH1D *h_Vz_KmFD_BC_1e_cut, *h_Vz_KmFD_AC_1e_cut;
    TH1D *h_dVz_KmFD_BC_1e_cut, *h_dVz_KmFD_AC_1e_cut;
    TH1D *h_chi2_KmFD_BC_1e_cut, *h_chi2_KmFD_AC_1e_cut;

    TH1D *h_Vz_KmCD_BC_1e_cut, *h_Vz_KmCD_AC_1e_cut;
    TH1D *h_dVz_KmCD_BC_1e_cut, *h_dVz_KmCD_AC_1e_cut;
    TH1D *h_chi2_KmCD_BC_1e_cut, *h_chi2_KmCD_AC_1e_cut;

    TH2D* h_dc_KmFD_hit_map_BC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // 1..3
    TH2D* h_dc_KmFD_hit_map_AC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // 1..3

    // =========================
    // D PID Histograms
    // =========================
    TH1D *h_Vz_D_FD_BC_1e_cut, *h_Vz_D_FD_AC_1e_cut;
    TH1D *h_dVz_D_FD_BC_1e_cut, *h_dVz_D_FD_AC_1e_cut;
    TH1D *h_chi2_D_FD_BC_1e_cut, *h_chi2_D_FD_AC_1e_cut;

    TH1D *h_Vz_D_CD_BC_1e_cut, *h_Vz_D_CD_AC_1e_cut;
    TH1D *h_dVz_D_CD_BC_1e_cut, *h_dVz_D_CD_AC_1e_cut;
    TH1D *h_chi2_D_CD_BC_1e_cut, *h_chi2_D_CD_AC_1e_cut;

    TH2D* h_dc_D_FD_hit_map_BC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // 1..3
    TH2D* h_dc_D_FD_hit_map_AC_1e_cut[4] = {nullptr, nullptr, nullptr, nullptr};  // 1..3

    // =========================
    // Constructor
    // =========================
    RawRecoPID_histograms(std::vector<TH1*>& HistoList_PID_plots, const double& Ebeam) {
        // =========================
        // Electron pre-selection and PID Histograms
        // =========================
        h_Vz_e_BC_1e_cut = new TH1D("Vz_e_BC_1e_cut", "V_{z}^{e} in 1e cut (before cut);V_{z}^{e} [cm];Counts", 50, -8, 8);
        HistoList_PID_plots.push_back(h_Vz_e_BC_1e_cut);
        h_Vz_e_AC_1e_cut = new TH1D("Vz_e_AC_1e_cut", "V_{z}^{e} in 1e cut (after cut);V_{z}^{e} [cm];Counts", 50, -8, 8);
        HistoList_PID_plots.push_back(h_Vz_e_AC_1e_cut);

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_electron_hit_map_BC_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_BC_%d", i), Form("DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_electron_hit_map_BC_1e_cut[i]);
            h_dc_electron_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_electron_hit_map_AC_%d", i), Form("DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_electron_hit_map_AC_1e_cut[i]);
        }

        h_nphe_BC_1e_cut = new TH1D("nphe_BC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList_PID_plots.push_back(h_nphe_BC_1e_cut);
        h_nphe_AC_1e_cut = new TH1D("nphe_AC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList_PID_plots.push_back(h_nphe_AC_1e_cut);

        h_Edep_PCAL_VS_EC_BC_1e_cut =
            new TH2D("Edep_PCAL_VS_EC_BC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0,
                     0.2, 100, 0, 0.3);
        HistoList_PID_plots.push_back(h_Edep_PCAL_VS_EC_BC_1e_cut);
        h_Edep_PCAL_VS_EC_AC_1e_cut =
            new TH2D("Edep_PCAL_VS_EC_AC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0,
                     0.2, 100, 0, 0.3);
        HistoList_PID_plots.push_back(h_Edep_PCAL_VS_EC_AC_1e_cut);

        h_SF_VS_Edep_PCAL_BC_AllSectors_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_BC_AllSectors_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in 1e cut (all sectors, before cut);E_{dep}^{PCAL} [GeV/c];Electron sampling fraction", 100, 0, 1.25, 100, 0.16, 0.3);
        HistoList_PID_plots.push_back(h_SF_VS_Edep_PCAL_BC_AllSectors_1e_cut);
        h_SF_VS_Edep_PCAL_AC_AllSectors_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_AC_AllSectors_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in 1e cut (all sectors, after cut);E_{dep}^{PCAL} [GeV/c];Electron sampling fraction", 100, 0, 1.25, 100, 0.16, 0.3);
        HistoList_PID_plots.push_back(h_SF_VS_Edep_PCAL_AC_AllSectors_1e_cut);

        for (int i = 1; i <= 6; i++) {
            h_SF_VS_Edep_PCAL_BC_BySector_1e_cut[i] = new TH2D(
                Form("SF_VS_Edep_PCAL_BC_Sector%d_1e_cut", i),
                Form("Electron sampling fraction vs. E_{dep}^{PCAL} in 1e cut (sector %d, before cut);E_{dep}^{PCAL} [GeV];Electron sampling fraction", i), 100, 0, 1.25, 100, 0.16, 0.3);
            HistoList_PID_plots.push_back(h_SF_VS_Edep_PCAL_BC_BySector_1e_cut[i]);

            h_SF_VS_Edep_PCAL_AC_BySector_1e_cut[i] = new TH2D(
                Form("SF_VS_Edep_PCAL_AC_Sector%d_1e_cut", i),
                Form("Electron sampling fraction vs. E_{dep}^{PCAL} in 1e cut (sector %d, after cut);E_{dep}^{PCAL} [GeV];Electron sampling fraction", i), 100, 0, 1.25, 100, 0.16, 0.3);
            HistoList_PID_plots.push_back(h_SF_VS_Edep_PCAL_AC_BySector_1e_cut[i]);
        }

        h_SF_VS_P_e_BC_AllSectors_1e_cut =
            new TH2D("SF_VS_P_e_BC_AllSectors_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (all sectors, before cut);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.16, 0.3);
        HistoList_PID_plots.push_back(h_SF_VS_P_e_BC_AllSectors_1e_cut);
        h_SF_VS_P_e_AC_AllSectors_1e_cut =
            new TH2D("SF_VS_P_e_AC_AllSectors_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (all sectors, after cut);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.16, 0.3);
        HistoList_PID_plots.push_back(h_SF_VS_P_e_AC_AllSectors_1e_cut);

        for (int i = 1; i <= 6; i++) {
            h_SF_VS_P_e_BC_BySector_1e_cut[i] =
                new TH2D(Form("SF_VS_P_e_BC_Sector%d_1e_cut", i), Form("Electron sampling fraction vs. P_{e} in 1e cut (sector %d, before cut);P_{e} [GeV/c];Electron sampling fraction", i),
                         100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
            HistoList_PID_plots.push_back(h_SF_VS_P_e_BC_BySector_1e_cut[i]);

            h_SF_VS_P_e_AC_BySector_1e_cut[i] =
                new TH2D(Form("SF_VS_P_e_AC_Sector%d_1e_cut", i), Form("Electron sampling fraction vs. P_{e} in 1e cut (sector %d, after cut);P_{e} [GeV/c];Electron sampling fraction", i),
                         100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
            HistoList_PID_plots.push_back(h_SF_VS_P_e_AC_BySector_1e_cut[i]);
        }

        h_SF_VS_Lv_BC_1e_cut = new TH2D("SF_VS_Lv_BC_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (before cut);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
        HistoList_PID_plots.push_back(h_SF_VS_Lv_BC_1e_cut);
        h_SF_VS_Lv_AC_1e_cut = new TH2D("SF_VS_Lv_AC_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (after cut);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
        HistoList_PID_plots.push_back(h_SF_VS_Lv_AC_1e_cut);

        h_SF_VS_Lw_BC_1e_cut = new TH2D("SF_VS_Lw_BC_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (before cut);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
        HistoList_PID_plots.push_back(h_SF_VS_Lw_BC_1e_cut);
        h_SF_VS_Lw_AC_1e_cut = new TH2D("SF_VS_Lw_AC_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (after cut);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
        HistoList_PID_plots.push_back(h_SF_VS_Lw_AC_1e_cut);

        h_SF_VS_Lu_BC_1e_cut = new TH2D("SF_VS_Lu_BC_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (before cut);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
        HistoList_PID_plots.push_back(h_SF_VS_Lu_BC_1e_cut);
        h_SF_VS_Lu_AC_1e_cut = new TH2D("SF_VS_Lu_AC_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (after cut);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
        HistoList_PID_plots.push_back(h_SF_VS_Lu_AC_1e_cut);

        h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut = new TH2D(
            "E_PCALoP_e_VS_E_PCALoP_e_BC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (before cut);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3, 100, 0, 0.35);
        HistoList_PID_plots.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut);
        h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut = new TH2D(
            "E_PCALoP_e_VS_E_PCALoP_e_AC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (after cut);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3, 100, 0, 0.35);
        HistoList_PID_plots.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut);

        // =========================
        // Proton pre-selection and PID Histograms
        // =========================
        h_Vz_pFD_BC_1e_cut = new TH1D("Vz_pFD_BC_1e_cut", "V_{z}^{pFD} in 1e cut (before cut);V_{z}^{pFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_pFD_BC_1e_cut);
        h_Vz_pFD_AC_1e_cut = new TH1D("Vz_pFD_AC_1e_cut", "V_{z}^{pFD} in 1e cut (after cut);V_{z}^{pFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_pFD_AC_1e_cut);

        h_dVz_pFD_BC_1e_cut = new TH1D("dVz_pFD_BC_1e_cut", "dV_{z}^{pFD} in 1e cut (before cut);dV_{z}^{pFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_pFD_BC_1e_cut);
        h_dVz_pFD_AC_1e_cut = new TH1D("dVz_pFD_AC_1e_cut", "dV_{z}^{pFD} in 1e cut (after cut);dV_{z}^{pFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_pFD_AC_1e_cut);

        h_chi2_pFD_BC_1e_cut = new TH1D("chi2_pFD_BC_1e_cut", "#chi^{2}_{pFD} in 1e cut (before cut);#chi^{2}_{pFD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_pFD_BC_1e_cut);
        h_chi2_pFD_AC_1e_cut = new TH1D("chi2_pFD_AC_1e_cut", "#chi^{2}_{pFD} in 1e cut (after cut);#chi^{2}_{pFD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_pFD_AC_1e_cut);

        h_Vz_pCD_BC_1e_cut = new TH1D("Vz_pCD_BC_1e_cut", "V_{z}^{pCD} in 1e cut (before cut);V_{z}^{pCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_pCD_BC_1e_cut);
        h_Vz_pCD_AC_1e_cut = new TH1D("Vz_pCD_AC_1e_cut", "V_{z}^{pCD} in 1e cut (after cut);V_{z}^{pCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_pCD_AC_1e_cut);

        h_dVz_pCD_BC_1e_cut = new TH1D("dVz_pCD_BC_1e_cut", "dV_{z}^{pCD} in 1e cut (before cut);dV_{z}^{pCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_pCD_BC_1e_cut);
        h_dVz_pCD_AC_1e_cut = new TH1D("dVz_pCD_AC_1e_cut", "dV_{z}^{pCD} in 1e cut (after cut);dV_{z}^{pCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_pCD_AC_1e_cut);

        h_chi2_pCD_BC_1e_cut = new TH1D("chi2_pCD_BC_1e_cut", "#chi^{2}_{pCD} in 1e cut (before cut);#chi^{2}_{pCD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_pCD_BC_1e_cut);
        h_chi2_pCD_AC_1e_cut = new TH1D("chi2_pCD_AC_1e_cut", "#chi^{2}_{pCD} in 1e cut (after cut);#chi^{2}_{pCD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_pCD_AC_1e_cut);

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pFD_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_pFD_hit_map_BC_%d", i), Form("pFD DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_pFD_hit_map_BC_1e_cut[i]);
            h_dc_pFD_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_pFD_hit_map_AC_%d", i), Form("pFD DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_pFD_hit_map_AC_1e_cut[i]);
        }

        // =========================
        // Pi+ pre-selection and PID Histograms
        // =========================

        // =========================
        // Pi+ pre-selection and PID Histograms
        // =========================
        h_Vz_pipFD_BC_1e_cut = new TH1D("Vz_pipFD_BC_1e_cut", "V_{z}^{pipFD} in 1e cut (before cut);V_{z}^{pipFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_pipFD_BC_1e_cut);
        h_Vz_pipFD_AC_1e_cut = new TH1D("Vz_pipFD_AC_1e_cut", "V_{z}^{pipFD} in 1e cut (after cut);V_{z}^{pipFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_pipFD_AC_1e_cut);

        h_dVz_pipFD_BC_1e_cut = new TH1D("dVz_pipFD_BC_1e_cut", "dV_{z}^{pipFD} in 1e cut (before cut);dV_{z}^{pipFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_pipFD_BC_1e_cut);
        h_dVz_pipFD_AC_1e_cut = new TH1D("dVz_pipFD_AC_1e_cut", "dV_{z}^{pipFD} in 1e cut (after cut);dV_{z}^{pipFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_pipFD_AC_1e_cut);

        h_chi2_pipFD_BC_1e_cut = new TH1D("chi2_pipFD_BC_1e_cut", "#chi^{2}_{pipFD} in 1e cut (before cut);#chi^{2}_{pipFD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_pipFD_BC_1e_cut);
        h_chi2_pipFD_AC_1e_cut = new TH1D("chi2_pipFD_AC_1e_cut", "#chi^{2}_{pipFD} in 1e cut (after cut);#chi^{2}_{pipFD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_pipFD_AC_1e_cut);

        h_Vz_pipCD_BC_1e_cut = new TH1D("Vz_pipCD_BC_1e_cut", "V_{z}^{pipCD} in 1e cut (before cut);V_{z}^{pipCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_pipCD_BC_1e_cut);
        h_Vz_pipCD_AC_1e_cut = new TH1D("Vz_pipCD_AC_1e_cut", "V_{z}^{pipCD} in 1e cut (after cut);V_{z}^{pipCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_pipCD_AC_1e_cut);

        h_dVz_pipCD_BC_1e_cut = new TH1D("dVz_pipCD_BC_1e_cut", "dV_{z}^{pipCD} in 1e cut (before cut);dV_{z}^{pipCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_pipCD_BC_1e_cut);
        h_dVz_pipCD_AC_1e_cut = new TH1D("dVz_pipCD_AC_1e_cut", "dV_{z}^{pipCD} in 1e cut (after cut);dV_{z}^{pipCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_pipCD_AC_1e_cut);

        h_chi2_pipCD_BC_1e_cut = new TH1D("chi2_pipCD_BC_1e_cut", "#chi^{2}_{pipCD} in 1e cut (before cut);#chi^{2}_{pipCD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_pipCD_BC_1e_cut);
        h_chi2_pipCD_AC_1e_cut = new TH1D("chi2_pipCD_AC_1e_cut", "#chi^{2}_{pipCD} in 1e cut (after cut);#chi^{2}_{pipCD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_pipCD_AC_1e_cut);

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pipFD_hit_map_BC_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_BC_%d", i), Form("pipFD DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_pipFD_hit_map_BC_1e_cut[i]);
            h_dc_pipFD_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_pipFD_hit_map_AC_%d", i), Form("pipFD DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_pipFD_hit_map_AC_1e_cut[i]);
        }

        // =========================
        // Pi- pre-selection and PID Histograms
        // =========================
        h_Vz_pimFD_BC_1e_cut = new TH1D("Vz_pimFD_BC_1e_cut", "V_{z}^{pimFD} in 1e cut (before cut);V_{z}^{pimFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_pimFD_BC_1e_cut);
        h_Vz_pimFD_AC_1e_cut = new TH1D("Vz_pimFD_AC_1e_cut", "V_{z}^{pimFD} in 1e cut (after cut);V_{z}^{pimFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_pimFD_AC_1e_cut);

        h_dVz_pimFD_BC_1e_cut = new TH1D("dVz_pimFD_BC_1e_cut", "dV_{z}^{pimFD} in 1e cut (before cut);dV_{z}^{pimFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_pimFD_BC_1e_cut);
        h_dVz_pimFD_AC_1e_cut = new TH1D("dVz_pimFD_AC_1e_cut", "dV_{z}^{pimFD} in 1e cut (after cut);dV_{z}^{pimFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_pimFD_AC_1e_cut);

        h_chi2_pimFD_BC_1e_cut = new TH1D("chi2_pimFD_BC_1e_cut", "#chi^{2}_{pimFD} in 1e cut (before cut);#chi^{2}_{pimFD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_pimFD_BC_1e_cut);
        h_chi2_pimFD_AC_1e_cut = new TH1D("chi2_pimFD_AC_1e_cut", "#chi^{2}_{pimFD} in 1e cut (after cut);#chi^{2}_{pimFD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_pimFD_AC_1e_cut);

        h_Vz_pimCD_BC_1e_cut = new TH1D("Vz_pimCD_BC_1e_cut", "V_{z}^{pimCD} in 1e cut (before cut);V_{z}^{pimCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_pimCD_BC_1e_cut);
        h_Vz_pimCD_AC_1e_cut = new TH1D("Vz_pimCD_AC_1e_cut", "V_{z}^{pimCD} in 1e cut (after cut);V_{z}^{pimCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_pimCD_AC_1e_cut);

        h_dVz_pimCD_BC_1e_cut = new TH1D("dVz_pimCD_BC_1e_cut", "dV_{z}^{pimCD} in 1e cut (before cut);dV_{z}^{pimCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_pimCD_BC_1e_cut);
        h_dVz_pimCD_AC_1e_cut = new TH1D("dVz_pimCD_AC_1e_cut", "dV_{z}^{pimCD} in 1e cut (after cut);dV_{z}^{pimCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_pimCD_AC_1e_cut);

        h_chi2_pimCD_BC_1e_cut = new TH1D("chi2_pimCD_BC_1e_cut", "#chi^{2}_{pimCD} in 1e cut (before cut);#chi^{2}_{pimCD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_pimCD_BC_1e_cut);
        h_chi2_pimCD_AC_1e_cut = new TH1D("chi2_pimCD_AC_1e_cut", "#chi^{2}_{pimCD} in 1e cut (after cut);#chi^{2}_{pimCD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_pimCD_AC_1e_cut);

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pimFD_hit_map_BC_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_BC_%d", i), Form("pimFD DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_pimFD_hit_map_BC_1e_cut[i]);
            h_dc_pimFD_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_pimFD_hit_map_AC_%d", i), Form("pimFD DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_pimFD_hit_map_AC_1e_cut[i]);
        }

        // =========================
        // K+ pre-selection and PID Histograms
        // =========================
        h_Vz_KpFD_BC_1e_cut = new TH1D("Vz_KpFD_BC_1e_cut", "V_{z}^{KpFD} in 1e cut (before cut);V_{z}^{KpFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_KpFD_BC_1e_cut);
        h_Vz_KpFD_AC_1e_cut = new TH1D("Vz_KpFD_AC_1e_cut", "V_{z}^{KpFD} in 1e cut (after cut);V_{z}^{KpFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_KpFD_AC_1e_cut);

        h_dVz_KpFD_BC_1e_cut = new TH1D("dVz_KpFD_BC_1e_cut", "dV_{z}^{KpFD} in 1e cut (before cut);dV_{z}^{KpFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_KpFD_BC_1e_cut);
        h_dVz_KpFD_AC_1e_cut = new TH1D("dVz_KpFD_AC_1e_cut", "dV_{z}^{KpFD} in 1e cut (after cut);dV_{z}^{KpFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_KpFD_AC_1e_cut);

        h_chi2_KpFD_BC_1e_cut = new TH1D("chi2_KpFD_BC_1e_cut", "#chi^{2}_{KpFD} in 1e cut (before cut);#chi^{2}_{KpFD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_KpFD_BC_1e_cut);
        h_chi2_KpFD_AC_1e_cut = new TH1D("chi2_KpFD_AC_1e_cut", "#chi^{2}_{KpFD} in 1e cut (after cut);#chi^{2}_{KpFD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_KpFD_AC_1e_cut);

        h_Vz_KpCD_BC_1e_cut = new TH1D("Vz_KpCD_BC_1e_cut", "V_{z}^{KpCD} in 1e cut (before cut);V_{z}^{KpCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_KpCD_BC_1e_cut);
        h_Vz_KpCD_AC_1e_cut = new TH1D("Vz_KpCD_AC_1e_cut", "V_{z}^{KpCD} in 1e cut (after cut);V_{z}^{KpCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_KpCD_AC_1e_cut);

        h_dVz_KpCD_BC_1e_cut = new TH1D("dVz_KpCD_BC_1e_cut", "dV_{z}^{KpCD} in 1e cut (before cut);dV_{z}^{KpCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_KpCD_BC_1e_cut);
        h_dVz_KpCD_AC_1e_cut = new TH1D("dVz_KpCD_AC_1e_cut", "dV_{z}^{KpCD} in 1e cut (after cut);dV_{z}^{KpCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_KpCD_AC_1e_cut);

        h_chi2_KpCD_BC_1e_cut = new TH1D("chi2_KpCD_BC_1e_cut", "#chi^{2}_{KpCD} in 1e cut (before cut);#chi^{2}_{KpCD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_KpCD_BC_1e_cut);
        h_chi2_KpCD_AC_1e_cut = new TH1D("chi2_KpCD_AC_1e_cut", "#chi^{2}_{KpCD} in 1e cut (after cut);#chi^{2}_{KpCD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_KpCD_AC_1e_cut);

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_KpFD_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_KpFD_hit_map_BC_%d", i), Form("KpFD DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_KpFD_hit_map_BC_1e_cut[i]);
            h_dc_KpFD_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_KpFD_hit_map_AC_%d", i), Form("KpFD DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_KpFD_hit_map_AC_1e_cut[i]);
        }

        // =========================
        // K- pre-selection and PID Histograms
        // =========================
        h_Vz_KmFD_BC_1e_cut = new TH1D("Vz_KmFD_BC_1e_cut", "V_{z}^{KmFD} in 1e cut (before cut);V_{z}^{KmFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_KmFD_BC_1e_cut);
        h_Vz_KmFD_AC_1e_cut = new TH1D("Vz_KmFD_AC_1e_cut", "V_{z}^{KmFD} in 1e cut (after cut);V_{z}^{KmFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_KmFD_AC_1e_cut);

        h_dVz_KmFD_BC_1e_cut = new TH1D("dVz_KmFD_BC_1e_cut", "dV_{z}^{KmFD} in 1e cut (before cut);dV_{z}^{KmFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_KmFD_BC_1e_cut);
        h_dVz_KmFD_AC_1e_cut = new TH1D("dVz_KmFD_AC_1e_cut", "dV_{z}^{KmFD} in 1e cut (after cut);dV_{z}^{KmFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_KmFD_AC_1e_cut);

        h_chi2_KmFD_BC_1e_cut = new TH1D("chi2_KmFD_BC_1e_cut", "#chi^{2}_{KmFD} in 1e cut (before cut);#chi^{2}_{KmFD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_KmFD_BC_1e_cut);
        h_chi2_KmFD_AC_1e_cut = new TH1D("chi2_KmFD_AC_1e_cut", "#chi^{2}_{KmFD} in 1e cut (after cut);#chi^{2}_{KmFD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_KmFD_AC_1e_cut);

        h_Vz_KmCD_BC_1e_cut = new TH1D("Vz_KmCD_BC_1e_cut", "V_{z}^{KmCD} in 1e cut (before cut);V_{z}^{KmCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_KmCD_BC_1e_cut);
        h_Vz_KmCD_AC_1e_cut = new TH1D("Vz_KmCD_AC_1e_cut", "V_{z}^{KmCD} in 1e cut (after cut);V_{z}^{KmCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_KmCD_AC_1e_cut);

        h_dVz_KmCD_BC_1e_cut = new TH1D("dVz_KmCD_BC_1e_cut", "dV_{z}^{KmCD} in 1e cut (before cut);dV_{z}^{KmCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_KmCD_BC_1e_cut);
        h_dVz_KmCD_AC_1e_cut = new TH1D("dVz_KmCD_AC_1e_cut", "dV_{z}^{KmCD} in 1e cut (after cut);dV_{z}^{KmCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_KmCD_AC_1e_cut);

        h_chi2_KmCD_BC_1e_cut = new TH1D("chi2_KmCD_BC_1e_cut", "#chi^{2}_{KmCD} in 1e cut (before cut);#chi^{2}_{KmCD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_KmCD_BC_1e_cut);
        h_chi2_KmCD_AC_1e_cut = new TH1D("chi2_KmCD_AC_1e_cut", "#chi^{2}_{KmCD} in 1e cut (after cut);#chi^{2}_{KmCD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_KmCD_AC_1e_cut);

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_KmFD_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_KmFD_hit_map_BC_%d", i), Form("KmFD DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_KmFD_hit_map_BC_1e_cut[i]);
            h_dc_KmFD_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_KmFD_hit_map_AC_%d", i), Form("KmFD DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_KmFD_hit_map_AC_1e_cut[i]);
        }

        // =========================
        // D pre-selection and PID Histograms
        // =========================
        h_Vz_D_FD_BC_1e_cut = new TH1D("Vz_D_FD_BC_1e_cut", "V_{z}^{dFD} in 1e cut (before cut);V_{z}^{dFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_D_FD_BC_1e_cut);
        h_Vz_D_FD_AC_1e_cut = new TH1D("Vz_D_FD_AC_1e_cut", "V_{z}^{dFD} in 1e cut (after cut);V_{z}^{dFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_D_FD_AC_1e_cut);

        h_dVz_D_FD_BC_1e_cut = new TH1D("dVz_D_FD_BC_1e_cut", "dV_{z}^{dFD} in 1e cut (before cut);dV_{z}^{dFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_D_FD_BC_1e_cut);
        h_dVz_D_FD_AC_1e_cut = new TH1D("dVz_D_FD_AC_1e_cut", "dV_{z}^{dFD} in 1e cut (after cut);dV_{z}^{dFD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_D_FD_AC_1e_cut);

        h_chi2_D_FD_BC_1e_cut = new TH1D("chi2_D_FD_BC_1e_cut", "#chi^{2}_{dFD} in 1e cut (before cut);#chi^{2}_{dFD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_D_FD_BC_1e_cut);
        h_chi2_D_FD_AC_1e_cut = new TH1D("chi2_D_FD_AC_1e_cut", "#chi^{2}_{dFD} in 1e cut (after cut);#chi^{2}_{dFD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_D_FD_AC_1e_cut);

        h_Vz_D_CD_BC_1e_cut = new TH1D("Vz_D_CD_BC_1e_cut", "V_{z}^{dCD} in 1e cut (before cut);V_{z}^{dCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_D_CD_BC_1e_cut);
        h_Vz_D_CD_AC_1e_cut = new TH1D("Vz_D_CD_AC_1e_cut", "V_{z}^{dCD} in 1e cut (after cut);V_{z}^{dCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_Vz_D_CD_AC_1e_cut);

        h_dVz_D_CD_BC_1e_cut = new TH1D("dVz_D_CD_BC_1e_cut", "dV_{z}^{dCD} in 1e cut (before cut);dV_{z}^{dCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_D_CD_BC_1e_cut);
        h_dVz_D_CD_AC_1e_cut = new TH1D("dVz_D_CD_AC_1e_cut", "dV_{z}^{dCD} in 1e cut (after cut);dV_{z}^{dCD} [cm];Counts", 50, -10, 10);
        HistoList_PID_plots.push_back(h_dVz_D_CD_AC_1e_cut);

        h_chi2_D_CD_BC_1e_cut = new TH1D("chi2_D_CD_BC_1e_cut", "#chi^{2}_{dCD} in 1e cut (before cut);#chi^{2}_{dCD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_D_CD_BC_1e_cut);
        h_chi2_D_CD_AC_1e_cut = new TH1D("chi2_D_CD_AC_1e_cut", "#chi^{2}_{dCD} in 1e cut (after cut);#chi^{2}_{dCD};Counts", 50, -5, 5);
        HistoList_PID_plots.push_back(h_chi2_D_CD_AC_1e_cut);

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_D_FD_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_D_FD_hit_map_BC_%d", i), Form("D FD DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_D_FD_hit_map_BC_1e_cut[i]);
            h_dc_D_FD_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_D_FD_hit_map_AC_%d", i), Form("D FD DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList_PID_plots.push_back(h_dc_D_FD_hit_map_AC_1e_cut[i]);
        }
    }

    // =========================
    // FILLER FUNCTIONS
    // =========================

    // =========================
    // Functions to fill histograms for electrons (and grouped functions to fill all electron histograms at once)
    // =========================
    void FillElectronVzBC(const double& vz, const double& w) { h_Vz_e_BC_1e_cut->Fill(vz, w); }
    void FillElectronVzAC(const double& vz, const double& w) { h_Vz_e_AC_1e_cut->Fill(vz, w); }

    void FillElectronDCHitBC(const region_part_ptr& electron, const double& w) { raf::fillDCdebug(electron, h_dc_electron_hit_map_BC_1e_cut, w); }
    void FillElectronDCHitAC(const region_part_ptr& electron, const double& w) { raf::fillDCdebug(electron, h_dc_electron_hit_map_AC_1e_cut, w); }

    void FillElectronNpheBC(const double& nphe, const double& w) { h_nphe_BC_1e_cut->Fill(nphe, w); }
    void FillElectronNpheAC(const double& nphe, const double& w) { h_nphe_AC_1e_cut->Fill(nphe, w); }

    void FillElectronEdepPCALvsECBC(const double& Epc, const double& Eec, const double& w) { h_Edep_PCAL_VS_EC_BC_1e_cut->Fill(Epc, Eec, w); }
    void FillElectronEdepPCALvsECAC(const double& Epc, const double& Eec, const double& w) { h_Edep_PCAL_VS_EC_AC_1e_cut->Fill(Epc, Eec, w); }

    void FillElectronSFvsEdepPCALBC(const double& p, const double& sf, const int& sector, const double& w) {
        if (h_SF_VS_Edep_PCAL_BC_AllSectors_1e_cut) { h_SF_VS_Edep_PCAL_BC_AllSectors_1e_cut->Fill(p, sf, w); }
        if (sector >= 1 && sector <= 6 && h_SF_VS_Edep_PCAL_BC_BySector_1e_cut[sector]) { h_SF_VS_Edep_PCAL_BC_BySector_1e_cut[sector]->Fill(p, sf, w); }
    }

    void FillElectronSFvsEdepPCALAC(const double& p, const double& sf, const int& sector, const double& w) {
        if (h_SF_VS_Edep_PCAL_AC_AllSectors_1e_cut) { h_SF_VS_Edep_PCAL_AC_AllSectors_1e_cut->Fill(p, sf, w); }
        if (sector >= 1 && sector <= 6 && h_SF_VS_Edep_PCAL_AC_BySector_1e_cut[sector]) { h_SF_VS_Edep_PCAL_AC_BySector_1e_cut[sector]->Fill(p, sf, w); }
    }

    void FillElectronSFvsPBC(const double& p, const double& sf, const int& sector, const double& w) {
        if (h_SF_VS_P_e_BC_AllSectors_1e_cut) { h_SF_VS_P_e_BC_AllSectors_1e_cut->Fill(p, sf, w); }
        if (sector >= 1 && sector <= 6 && h_SF_VS_P_e_BC_BySector_1e_cut[sector]) { h_SF_VS_P_e_BC_BySector_1e_cut[sector]->Fill(p, sf, w); }
    }

    void FillElectronSFvsPAC(const double& p, const double& sf, const int& sector, const double& w) {
        if (h_SF_VS_P_e_AC_AllSectors_1e_cut) { h_SF_VS_P_e_AC_AllSectors_1e_cut->Fill(p, sf, w); }
        if (sector >= 1 && sector <= 6 && h_SF_VS_P_e_AC_BySector_1e_cut[sector]) { h_SF_VS_P_e_AC_BySector_1e_cut[sector]->Fill(p, sf, w); }
    }

    void FillElectronSFvsLvBC(const double& Lv, const double& sf, const double& w) { h_SF_VS_Lv_BC_1e_cut->Fill(Lv, sf, w); }
    void FillElectronSFvsLvAC(const double& Lv, const double& sf, const double& w) { h_SF_VS_Lv_AC_1e_cut->Fill(Lv, sf, w); }

    void FillElectronSFvsLwBC(const double& Lw, const double& sf, const double& w) { h_SF_VS_Lw_BC_1e_cut->Fill(Lw, sf, w); }
    void FillElectronSFvsLwAC(const double& Lw, const double& sf, const double& w) { h_SF_VS_Lw_AC_1e_cut->Fill(Lw, sf, w); }

    void FillElectronSFvsLuBC(const double& Lu, const double& sf, const double& w) { h_SF_VS_Lu_BC_1e_cut->Fill(Lu, sf, w); }
    void FillElectronSFvsLuAC(const double& Lu, const double& sf, const double& w) { h_SF_VS_Lu_AC_1e_cut->Fill(Lu, sf, w); }

    void FillElectronEcalDiagBC(const double& Epc_over_p, const double& Eecin_over_p, const double& w) { h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut->Fill(Epc_over_p, Eecin_over_p, w); }
    void FillElectronEcalDiagAC(const double& Epc_over_p, const double& Eecin_over_p, const double& w) { h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut->Fill(Epc_over_p, Eecin_over_p, w); }

    // Grouped electron fillers (BC = before cuts, AC = after cuts)
    void FillElectronBC(const region_part_ptr& electron, const double& vz, const double& nphe, const double& Epc, const double& Eec, const double& p, const double& sf, const double& Lv,
                        const double& Lw, const double& Lu, const double& Epc_over_p, const double& Eecin_over_p, const double& w, const int& sector = -1) {
        FillElectronVzBC(vz, w);
        FillElectronDCHitBC(electron, w);
        FillElectronNpheBC(nphe, w);
        FillElectronEdepPCALvsECBC(Epc, Eec, w);
        FillElectronSFvsPBC(p, sf, sector, w);
        FillElectronSFvsLvBC(Lv, sf, w);
        FillElectronSFvsLwBC(Lw, sf, w);
        FillElectronSFvsLuBC(Lu, sf, w);
        FillElectronEcalDiagBC(Epc_over_p, Eecin_over_p, w);
    }

    // Convenience wrapper: fill all "BC" electron PID histograms for every electron candidate in `electrons_det` in a single pass.
    void FillElectronCandidatesBC(const std::vector<region_part_ptr>& electrons_det, const double& weight) {
        // BC: fill for every electron candidate in electrons_det, in a single pass.
        for (auto el : electrons_det) {
            // Vz
            const double vz = el->par()->getVz();
            FillElectronVzBC(vz, weight);

            // DC hit maps
            FillElectronDCHitBC(el, weight);

            // HTCC nphe
            const double nphe_local = el->che(clas12::HTCC)->getNphe();
            FillElectronNpheBC(nphe_local, weight);

            // ECAL energy deposits
            const double Edep_PCAL_local = el->cal(clas12::PCAL)->getEnergy();
            const double Edep_EC_local = el->cal(clas12::ECIN)->getEnergy() + el->cal(clas12::ECOUT)->getEnergy();
            FillElectronEdepPCALvsECBC(Edep_PCAL_local, Edep_EC_local, weight);

            // Reco momentum and sampling fraction
            TVector3 recoP_local;
            recoP_local.SetMagThetaPhi(el->getP(), el->getTheta(), el->getPhi());

            const double p_local = recoP_local.Mag();
            if (p_local > 0.0) {
                const double Ecal_sum_local = Edep_PCAL_local + Edep_EC_local;
                const double EoP_local = Ecal_sum_local / p_local;

                FillElectronSFvsPBC(p_local, EoP_local, el->getSector(), weight);
                FillElectronSFvsEdepPCALBC(Edep_PCAL_local, EoP_local, el->getSector(), weight);

                // PCAL edge variables vs SF
                const double Lv_local = el->cal(clas12::PCAL)->getLv();
                const double Lw_local = el->cal(clas12::PCAL)->getLw();
                const double Lu_local = el->cal(clas12::PCAL)->getLu();
                FillElectronSFvsLvBC(Lv_local, EoP_local, weight);
                FillElectronSFvsLwBC(Lw_local, EoP_local, weight);
                FillElectronSFvsLuBC(Lu_local, EoP_local, weight);

                // ECAL diagonal variables
                const double E_PCALoP_local = Edep_PCAL_local / p_local;
                const double E_ECINoP_local = el->cal(clas12::ECIN)->getEnergy() / p_local;
                FillElectronEcalDiagBC(E_PCALoP_local, E_ECINoP_local, weight);
            }
        }
    }

    void FillElectronAC(const region_part_ptr& electron, const double& vz, const double& nphe, const double& Epc, const double& Eec, const double& p, const double& sf, const double& Lv,
                        const double& Lw, const double& Lu, const double& Epc_over_p, const double& Eecin_over_p, const double& w, const int& sector = -1) {
        FillElectronVzAC(vz, w);
        FillElectronDCHitAC(electron, w);
        FillElectronNpheAC(nphe, w);
        FillElectronEdepPCALvsECAC(Epc, Eec, w);
        FillElectronSFvsPAC(p, sf, sector, w);
        FillElectronSFvsLvAC(Lv, sf, w);
        FillElectronSFvsLwAC(Lw, sf, w);
        FillElectronSFvsLuAC(Lu, sf, w);
        FillElectronEcalDiagAC(Epc_over_p, Eecin_over_p, w);
    }

    // =========================
    // Functions to fill histograms for protons
    // =========================
    void FillProtonFDVzBC(const double& vz, const double& w) {
        if (h_Vz_pFD_BC_1e_cut) { h_Vz_pFD_BC_1e_cut->Fill(vz, w); }
    }
    void FillProtonFDVzAC(const double& vz, const double& w) {
        if (h_Vz_pFD_AC_1e_cut) { h_Vz_pFD_AC_1e_cut->Fill(vz, w); }
    }
    void FillProtonFDdVzBC(const double& dvz, const double& w) {
        if (h_dVz_pFD_BC_1e_cut) { h_dVz_pFD_BC_1e_cut->Fill(dvz, w); }
    }
    void FillProtonFDdVzAC(const double& dvz, const double& w) {
        if (h_dVz_pFD_AC_1e_cut) { h_dVz_pFD_AC_1e_cut->Fill(dvz, w); }
    }
    void FillProtonFDChi2BC(const double& chi2, const double& w) {
        if (h_chi2_pFD_BC_1e_cut) { h_chi2_pFD_BC_1e_cut->Fill(chi2, w); }
    }
    void FillProtonFDChi2AC(const double& chi2, const double& w) {
        if (h_chi2_pFD_AC_1e_cut) { h_chi2_pFD_AC_1e_cut->Fill(chi2, w); }
    }
    void FillProtonFDDCHitBC(const clas12::region_part_ptr& p, const double& w) { raf::fillDCdebug(p, h_dc_pFD_hit_map_BC_1e_cut, w); }
    void FillProtonFDDCHitAC(const clas12::region_part_ptr& p, const double& w) { raf::fillDCdebug(p, h_dc_pFD_hit_map_AC_1e_cut, w); }

    void FillProtonCDVzBC(const double& vz, const double& w) {
        if (h_Vz_pCD_BC_1e_cut) { h_Vz_pCD_BC_1e_cut->Fill(vz, w); }
    }
    void FillProtonCDVzAC(const double& vz, const double& w) {
        if (h_Vz_pCD_AC_1e_cut) { h_Vz_pCD_AC_1e_cut->Fill(vz, w); }
    }
    void FillProtonCDdVzBC(const double& dvz, const double& w) {
        if (h_dVz_pCD_BC_1e_cut) { h_dVz_pCD_BC_1e_cut->Fill(dvz, w); }
    }
    void FillProtonCDdVzAC(const double& dvz, const double& w) {
        if (h_dVz_pCD_AC_1e_cut) { h_dVz_pCD_AC_1e_cut->Fill(dvz, w); }
    }
    void FillProtonCDChi2BC(const double& chi2, const double& w) {
        if (h_chi2_pCD_BC_1e_cut) { h_chi2_pCD_BC_1e_cut->Fill(chi2, w); }
    }
    void FillProtonCDChi2AC(const double& chi2, const double& w) {
        if (h_chi2_pCD_AC_1e_cut) { h_chi2_pCD_AC_1e_cut->Fill(chi2, w); }
    }

    void FillProtonAC(const std::vector<int>& IDed_Protons_ind, std::vector<region_part_ptr> protons, const double& vtz_e, const double& weight) {
        for (auto& i : IDed_Protons_ind) {
            // loop over protons vector
            if (protons[i]->getRegion() == FD) {
                h_Vz_pFD_AC_1e_cut->Fill(protons[i]->par()->getVz(), weight);
                // bool bad_Vz_pFD_CutCond = (protons[i]->par()->getVz() < -4. || protons[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pFD_CutCond) { h_Vz_pFD_AC_1e_cut->Fill(protons[i]->par()->getVz(), weight); }

                h_dVz_pFD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight);
                // bool bad_dVz_pFD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
                // if (!bad_dVz_pFD_CutCond) { h_Vz_pFD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

                h_chi2_pFD_AC_1e_cut->Fill(protons[i]->par()->getChi2Pid(), weight);
                // bool bad_Vz_pFD_CutCond = (protons[i]->par()->getVz() < -4. || protons[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pFD_CutCond) { h_Vz_pFD_AC_1e_cut->Fill(protons[i]->par()->getVz(), weight); }

                raf::fillDCdebug(protons[i], h_dc_pFD_hit_map_AC_1e_cut, weight);
                // bool bad_DC_edge_CutCond = (!DCEdgeCuts(protons[i]));
                // if (!bad_DC_edge_CutCond) {raf::fillDCdebug(protons[i], h_dc_electron_hit_map_AC_1e_cut, weight); }
            } else if (protons[i]->getRegion() == CD) {
                h_Vz_pCD_AC_1e_cut->Fill(protons[i]->par()->getVz(), weight);
                // bool bad_Vz_pCD_CutCond = (protons[i]->par()->getVz() < -4. || protons[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pCD_CutCond) { h_Vz_pCD_AC_1e_cut->Fill(protons[i]->par()->getVz(), weight); }

                h_dVz_pCD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight);
                // bool bad_dVz_pCD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
                // if (!bad_dVz_pCD_CutCond) { h_Vz_pCD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

                h_chi2_pCD_AC_1e_cut->Fill(protons[i]->par()->getChi2Pid(), weight);
                // bool bad_Vz_pCD_CutCond = (protons[i]->par()->getVz() < -4. || protons[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pCD_CutCond) { h_Vz_pCD_AC_1e_cut->Fill(protons[i]->par()->getVz(), weight); }
            }

        }  // end of ToF loop over protons vector
    }

    // =========================
    // Functions to fill histograms for Pi+
    // =========================
    void FillPiPlusFDVzBC(const double& vz, const double& w) {
        if (h_Vz_pipFD_BC_1e_cut) { h_Vz_pipFD_BC_1e_cut->Fill(vz, w); }
    }
    void FillPiPlusFDVzAC(const double& vz, const double& w) {
        if (h_Vz_pipFD_AC_1e_cut) { h_Vz_pipFD_AC_1e_cut->Fill(vz, w); }
    }
    void FillPiPlusFDdVzBC(const double& dvz, const double& w) {
        if (h_dVz_pipFD_BC_1e_cut) { h_dVz_pipFD_BC_1e_cut->Fill(dvz, w); }
    }
    void FillPiPlusFDdVzAC(const double& dvz, const double& w) {
        if (h_dVz_pipFD_AC_1e_cut) { h_dVz_pipFD_AC_1e_cut->Fill(dvz, w); }
    }
    void FillPiPlusFDChi2BC(const double& chi2, const double& w) {
        if (h_chi2_pipFD_BC_1e_cut) { h_chi2_pipFD_BC_1e_cut->Fill(chi2, w); }
    }
    void FillPiPlusFDChi2AC(const double& chi2, const double& w) {
        if (h_chi2_pipFD_AC_1e_cut) { h_chi2_pipFD_AC_1e_cut->Fill(chi2, w); }
    }
    void FillPiPlusFDDCHitBC(const clas12::region_part_ptr& p, const double& w) { raf::fillDCdebug(p, h_dc_pipFD_hit_map_BC_1e_cut, w); }
    void FillPiPlusFDDCHitAC(const clas12::region_part_ptr& p, const double& w) { raf::fillDCdebug(p, h_dc_pipFD_hit_map_AC_1e_cut, w); }

    void FillPiPlusCDVzBC(const double& vz, const double& w) {
        if (h_Vz_pipCD_BC_1e_cut) { h_Vz_pipCD_BC_1e_cut->Fill(vz, w); }
    }
    void FillPiPlusCDVzAC(const double& vz, const double& w) {
        if (h_Vz_pipCD_AC_1e_cut) { h_Vz_pipCD_AC_1e_cut->Fill(vz, w); }
    }
    void FillPiPlusCDdVzBC(const double& dvz, const double& w) {
        if (h_dVz_pipCD_BC_1e_cut) { h_dVz_pipCD_BC_1e_cut->Fill(dvz, w); }
    }
    void FillPiPlusCDdVzAC(const double& dvz, const double& w) {
        if (h_dVz_pipCD_AC_1e_cut) { h_dVz_pipCD_AC_1e_cut->Fill(dvz, w); }
    }
    void FillPiPlusCDChi2BC(const double& chi2, const double& w) {
        if (h_chi2_pipCD_BC_1e_cut) { h_chi2_pipCD_BC_1e_cut->Fill(chi2, w); }
    }
    void FillPiPlusCDChi2AC(const double& chi2, const double& w) {
        if (h_chi2_pipCD_AC_1e_cut) { h_chi2_pipCD_AC_1e_cut->Fill(chi2, w); }
    }

    void FillPiPlusAC(const std::vector<int>& Piplus_ind, std::vector<region_part_ptr> piplus, const double& vtz_e, const double& weight) {
        for (auto& i : Piplus_ind) {
            // loop over piplus vector
            if (piplus[i]->getRegion() == FD) {
                h_Vz_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // bool bad_Vz_pipFD_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pipFD_CutCond) { h_Vz_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight); }

                h_dVz_pipFD_AC_1e_cut->Fill(vtz_e - piplus[i]->par()->getVz(), weight);
                // bool bad_dVz_pipFD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
                // if (!bad_dVz_pipFD_CutCond) { h_Vz_pipFD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

                h_chi2_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getChi2Pid(), weight);
                // bool bad_Vz_pipFD_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pipFD_CutCond) { h_Vz_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight); }

                raf::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_1e_cut, weight);
                // bool bad_DC_edge_CutCond = (!DCEdgeCuts(piplus[i]));
                // if (!bad_DC_edge_CutCond) {raf::fillDCdebug(piplus[i], h_dc_electron_hit_map_AC_1e_cut, weight); }
            } else if (piplus[i]->getRegion() == CD) {
                h_Vz_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // bool bad_Vz_pipCD_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pipCD_CutCond) { h_Vz_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight); }

                h_dVz_pipCD_AC_1e_cut->Fill(vtz_e - piplus[i]->par()->getVz(), weight);
                // bool bad_dVz_pipCD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
                // if (!bad_dVz_pipCD_CutCond) { h_Vz_pipCD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

                h_chi2_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getChi2Pid(), weight);
                // bool bad_Vz_pipCD_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pipCD_CutCond) { h_Vz_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight); }
            }

        }  // end of ToF loop over piplus vector
    }

    // =========================
    // Functions to fill histograms for Pi-
    // =========================
    void FillPiMinusFDVzBC(const double& vz, const double& w) {
        if (h_Vz_pimFD_BC_1e_cut) { h_Vz_pimFD_BC_1e_cut->Fill(vz, w); }
    }
    void FillPiMinusFDVzAC(const double& vz, const double& w) {
        if (h_Vz_pimFD_AC_1e_cut) { h_Vz_pimFD_AC_1e_cut->Fill(vz, w); }
    }
    void FillPiMinusFDdVzBC(const double& dvz, const double& w) {
        if (h_dVz_pimFD_BC_1e_cut) { h_dVz_pimFD_BC_1e_cut->Fill(dvz, w); }
    }
    void FillPiMinusFDdVzAC(const double& dvz, const double& w) {
        if (h_dVz_pimFD_AC_1e_cut) { h_dVz_pimFD_AC_1e_cut->Fill(dvz, w); }
    }
    void FillPiMinusFDChi2BC(const double& chi2, const double& w) {
        if (h_chi2_pimFD_BC_1e_cut) { h_chi2_pimFD_BC_1e_cut->Fill(chi2, w); }
    }
    void FillPiMinusFDChi2AC(const double& chi2, const double& w) {
        if (h_chi2_pimFD_AC_1e_cut) { h_chi2_pimFD_AC_1e_cut->Fill(chi2, w); }
    }
    void FillPiMinusFDDCHitBC(const clas12::region_part_ptr& p, const double& w) { raf::fillDCdebug(p, h_dc_pimFD_hit_map_BC_1e_cut, w); }
    void FillPiMinusFDDCHitAC(const clas12::region_part_ptr& p, const double& w) { raf::fillDCdebug(p, h_dc_pimFD_hit_map_AC_1e_cut, w); }

    void FillPiMinusCDVzBC(const double& vz, const double& w) {
        if (h_Vz_pimCD_BC_1e_cut) { h_Vz_pimCD_BC_1e_cut->Fill(vz, w); }
    }
    void FillPiMinusCDVzAC(const double& vz, const double& w) {
        if (h_Vz_pimCD_AC_1e_cut) { h_Vz_pimCD_AC_1e_cut->Fill(vz, w); }
    }
    void FillPiMinusCDdVzBC(const double& dvz, const double& w) {
        if (h_dVz_pimCD_BC_1e_cut) { h_dVz_pimCD_BC_1e_cut->Fill(dvz, w); }
    }
    void FillPiMinusCDdVzAC(const double& dvz, const double& w) {
        if (h_dVz_pimCD_AC_1e_cut) { h_dVz_pimCD_AC_1e_cut->Fill(dvz, w); }
    }
    void FillPiMinusCDChi2BC(const double& chi2, const double& w) {
        if (h_chi2_pimCD_BC_1e_cut) { h_chi2_pimCD_BC_1e_cut->Fill(chi2, w); }
    }
    void FillPiMinusCDChi2AC(const double& chi2, const double& w) {
        if (h_chi2_pimCD_AC_1e_cut) { h_chi2_pimCD_AC_1e_cut->Fill(chi2, w); }
    }

    void FillPiMinusAC(const std::vector<int>& Piminus_ind, std::vector<region_part_ptr> piminus, const double& vtz_e, const double& weight) {
        for (auto& i : Piminus_ind) {
            // loop over piminus vector
            if (piminus[i]->getRegion() == FD) {
                h_Vz_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // bool bad_Vz_pimFD_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pimFD_CutCond) { h_Vz_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight); }

                h_dVz_pimFD_AC_1e_cut->Fill(vtz_e - piminus[i]->par()->getVz(), weight);
                // bool bad_dVz_pimFD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
                // if (!bad_dVz_pimFD_CutCond) { h_Vz_pimFD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

                h_chi2_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getChi2Pid(), weight);
                // bool bad_Vz_pimFD_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pimFD_CutCond) { h_Vz_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight); }

                raf::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_1e_cut, weight);
                // bool bad_DC_edge_CutCond = (!DCEdgeCuts(piminus[i]));
                // if (!bad_DC_edge_CutCond) {raf::fillDCdebug(piminus[i], h_dc_electron_hit_map_AC_1e_cut, weight); }
            } else if (piminus[i]->getRegion() == CD) {
                h_Vz_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // bool bad_Vz_pimCD_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pimCD_CutCond) { h_Vz_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight); }

                h_dVz_pimCD_AC_1e_cut->Fill(vtz_e - piminus[i]->par()->getVz(), weight);
                // bool bad_dVz_pimCD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
                // if (!bad_dVz_pimCD_CutCond) { h_Vz_pimCD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

                h_chi2_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getChi2Pid(), weight);
                // bool bad_Vz_pimCD_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pimCD_CutCond) { h_Vz_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight); }
            }

        }  // end of ToF loop over piminus vector
    }

    // =========================
    // Functions to fill histograms for K+
    // =========================
    void FillKPlusFDVzBC(const double& vz, const double& w) {
        if (h_Vz_KpFD_BC_1e_cut) { h_Vz_KpFD_BC_1e_cut->Fill(vz, w); }
    }
    void FillKPlusFDVzAC(const double& vz, const double& w) {
        if (h_Vz_KpFD_AC_1e_cut) { h_Vz_KpFD_AC_1e_cut->Fill(vz, w); }
    }
    void FillKPlusFDdVzBC(const double& dvz, const double& w) {
        if (h_dVz_KpFD_BC_1e_cut) { h_dVz_KpFD_BC_1e_cut->Fill(dvz, w); }
    }
    void FillKPlusFDdVzAC(const double& dvz, const double& w) {
        if (h_dVz_KpFD_AC_1e_cut) { h_dVz_KpFD_AC_1e_cut->Fill(dvz, w); }
    }
    void FillKPlusFDChi2BC(const double& chi2, const double& w) {
        if (h_chi2_KpFD_BC_1e_cut) { h_chi2_KpFD_BC_1e_cut->Fill(chi2, w); }
    }
    void FillKPlusFDChi2AC(const double& chi2, const double& w) {
        if (h_chi2_KpFD_AC_1e_cut) { h_chi2_KpFD_AC_1e_cut->Fill(chi2, w); }
    }
    void FillKPlusFDDCHitBC(const clas12::region_part_ptr& p, const double& w) { raf::fillDCdebug(p, h_dc_KpFD_hit_map_BC_1e_cut, w); }
    void FillKPlusFDDCHitAC(const clas12::region_part_ptr& p, const double& w) { raf::fillDCdebug(p, h_dc_KpFD_hit_map_AC_1e_cut, w); }

    void FillKPlusCDVzBC(const double& vz, const double& w) {
        if (h_Vz_KpCD_BC_1e_cut) { h_Vz_KpCD_BC_1e_cut->Fill(vz, w); }
    }
    void FillKPlusCDVzAC(const double& vz, const double& w) {
        if (h_Vz_KpCD_AC_1e_cut) { h_Vz_KpCD_AC_1e_cut->Fill(vz, w); }
    }
    void FillKPlusCDdVzBC(const double& dvz, const double& w) {
        if (h_dVz_KpCD_BC_1e_cut) { h_dVz_KpCD_BC_1e_cut->Fill(dvz, w); }
    }
    void FillKPlusCDdVzAC(const double& dvz, const double& w) {
        if (h_dVz_KpCD_AC_1e_cut) { h_dVz_KpCD_AC_1e_cut->Fill(dvz, w); }
    }
    void FillKPlusCDChi2BC(const double& chi2, const double& w) {
        if (h_chi2_KpCD_BC_1e_cut) { h_chi2_KpCD_BC_1e_cut->Fill(chi2, w); }
    }
    void FillKPlusCDChi2AC(const double& chi2, const double& w) {
        if (h_chi2_KpCD_AC_1e_cut) { h_chi2_KpCD_AC_1e_cut->Fill(chi2, w); }
    }

    // =========================
    // Functions to fill histograms for K-
    // =========================
    void FillKMinusFDVzBC(const double& vz, const double& w) {
        if (h_Vz_KmFD_BC_1e_cut) { h_Vz_KmFD_BC_1e_cut->Fill(vz, w); }
    }
    void FillKMinusFDVzAC(const double& vz, const double& w) {
        if (h_Vz_KmFD_AC_1e_cut) { h_Vz_KmFD_AC_1e_cut->Fill(vz, w); }
    }
    void FillKMinusFDdVzBC(const double& dvz, const double& w) {
        if (h_dVz_KmFD_BC_1e_cut) { h_dVz_KmFD_BC_1e_cut->Fill(dvz, w); }
    }
    void FillKMinusFDdVzAC(const double& dvz, const double& w) {
        if (h_dVz_KmFD_AC_1e_cut) { h_dVz_KmFD_AC_1e_cut->Fill(dvz, w); }
    }
    void FillKMinusFDChi2BC(const double& chi2, const double& w) {
        if (h_chi2_KmFD_BC_1e_cut) { h_chi2_KmFD_BC_1e_cut->Fill(chi2, w); }
    }
    void FillKMinusFDChi2AC(const double& chi2, const double& w) {
        if (h_chi2_KmFD_AC_1e_cut) { h_chi2_KmFD_AC_1e_cut->Fill(chi2, w); }
    }
    void FillKMinusFDDCHitBC(const clas12::region_part_ptr& p, const double& w) { raf::fillDCdebug(p, h_dc_KmFD_hit_map_BC_1e_cut, w); }
    void FillKMinusFDDCHitAC(const clas12::region_part_ptr& p, const double& w) { raf::fillDCdebug(p, h_dc_KmFD_hit_map_AC_1e_cut, w); }

    void FillKMinusCDVzBC(const double& vz, const double& w) {
        if (h_Vz_KmCD_BC_1e_cut) { h_Vz_KmCD_BC_1e_cut->Fill(vz, w); }
    }
    void FillKMinusCDVzAC(const double& vz, const double& w) {
        if (h_Vz_KmCD_AC_1e_cut) { h_Vz_KmCD_AC_1e_cut->Fill(vz, w); }
    }
    void FillKMinusCDdVzBC(const double& dvz, const double& w) {
        if (h_dVz_KmCD_BC_1e_cut) { h_dVz_KmCD_BC_1e_cut->Fill(dvz, w); }
    }
    void FillKMinusCDdVzAC(const double& dvz, const double& w) {
        if (h_dVz_KmCD_AC_1e_cut) { h_dVz_KmCD_AC_1e_cut->Fill(dvz, w); }
    }
    void FillKMinusCDChi2BC(const double& chi2, const double& w) {
        if (h_chi2_KmCD_BC_1e_cut) { h_chi2_KmCD_BC_1e_cut->Fill(chi2, w); }
    }
    void FillKMinusCDChi2AC(const double& chi2, const double& w) {
        if (h_chi2_KmCD_AC_1e_cut) { h_chi2_KmCD_AC_1e_cut->Fill(chi2, w); }
    }

    // =========================
    // Functions to fill histograms for D
    // =========================
    void FillDeuteronFDVzBC(const double& vz, const double& w) {
        if (h_Vz_D_FD_BC_1e_cut) { h_Vz_D_FD_BC_1e_cut->Fill(vz, w); }
    }
    void FillDeuteronFDVzAC(const double& vz, const double& w) {
        if (h_Vz_D_FD_AC_1e_cut) { h_Vz_D_FD_AC_1e_cut->Fill(vz, w); }
    }
    void FillDeuteronFDdVzBC(const double& dvz, const double& w) {
        if (h_dVz_D_FD_BC_1e_cut) { h_dVz_D_FD_BC_1e_cut->Fill(dvz, w); }
    }
    void FillDeuteronFDdVzAC(const double& dvz, const double& w) {
        if (h_dVz_D_FD_AC_1e_cut) { h_dVz_D_FD_AC_1e_cut->Fill(dvz, w); }
    }
    void FillDeuteronFDChi2KdBC(const double& chi2kd, const double& w) {
        if (h_chi2_D_FD_BC_1e_cut) { h_chi2_D_FD_BC_1e_cut->Fill(chi2kd, w); }
    }
    void FillDeuteronFDChi2KdAC(const double& chi2kd, const double& w) {
        if (h_chi2_D_FD_AC_1e_cut) { h_chi2_D_FD_AC_1e_cut->Fill(chi2kd, w); }
    }
    void FillDeuteronFDDCHitBC(const clas12::region_part_ptr& p, const double& w) { raf::fillDCdebug(p, h_dc_D_FD_hit_map_BC_1e_cut, w); }
    void FillDeuteronFDDCHitAC(const clas12::region_part_ptr& p, const double& w) { raf::fillDCdebug(p, h_dc_D_FD_hit_map_AC_1e_cut, w); }

    void FillDeuteronCDVzBC(const double& vz, const double& w) {
        if (h_Vz_D_CD_BC_1e_cut) { h_Vz_D_CD_BC_1e_cut->Fill(vz, w); }
    }
    void FillDeuteronCDVzAC(const double& vz, const double& w) {
        if (h_Vz_D_CD_AC_1e_cut) { h_Vz_D_CD_AC_1e_cut->Fill(vz, w); }
    }
    void FillDeuteronCDdVzBC(const double& dvz, const double& w) {
        if (h_dVz_D_CD_BC_1e_cut) { h_dVz_D_CD_BC_1e_cut->Fill(dvz, w); }
    }
    void FillDeuteronCDdVzAC(const double& dvz, const double& w) {
        if (h_dVz_D_CD_AC_1e_cut) { h_dVz_D_CD_AC_1e_cut->Fill(dvz, w); }
    }
    void FillDeuteronCDChi2KdBC(const double& chi2kd, const double& w) {
        if (h_chi2_D_CD_BC_1e_cut) { h_chi2_D_CD_BC_1e_cut->Fill(chi2kd, w); }
    }
    void FillDeuteronCDChi2KdAC(const double& chi2kd, const double& w) {
        if (h_chi2_D_CD_AC_1e_cut) { h_chi2_D_CD_AC_1e_cut->Fill(chi2kd, w); }
    }
};
