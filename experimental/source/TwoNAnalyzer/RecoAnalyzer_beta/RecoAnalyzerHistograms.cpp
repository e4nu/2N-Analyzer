//
// Created by Alon Sportes on 05/03/2026.
//

#ifndef RECOANALYZERHISTOGRAMS_H
#define RECOANALYZERHISTOGRAMS_H

#include "RecoAnalyzerHistograms.h"

void RecoAnalyzerHistograms::InitializeNpheHistograms(const AnalysisCutSettings& cut_settings, const Directories& directories, const HistogramSettings& settings) {
    const double nphe_lim = settings.Nphe_lim;
    const int num_th1d_bins = settings.numTH1Dbins;
    if (!cut_settings.apply_cuts) {
        hNphe_1e_cut_BC_FD = hPlot1D("1e cut", "", "N_{phe} in HTCC BC", "#Photo-electrons in HTCC - N_{phe} - BC", "N_{phe}", directories.Nphe_dir_map.at("Nphe_1e_cut_BC"),
                                     "01_Nphe_1e_cut_BC", 0, nphe_lim, num_th1d_bins);
        hNphe_1e_cut_AC_FD = hPlot1D("1e cut", "", "N_{phe} in HTCC AC", "#Photo-electrons in HTCC - N_{phe} - AC", "N_{phe}", directories.Nphe_dir_map.at("Nphe_1e_cut_AC"),
                                     "02_Nphe_1e_cut_AC", 0, nphe_lim, num_th1d_bins);
    } else {
        hNphe_1e_cut_BC_FD = hPlot1D("1e cut", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}", directories.Nphe_dir_map.at("Nphe_1e_cut_BC"),
                                     "01_Nphe_1e_cut", 0, nphe_lim, num_th1d_bins);
    }

    hNphe_1p_FD = hPlot1D("1p", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}", directories.Nphe_dir_map.at("Nphe_1p"), "01_Nphe_1p", 0, nphe_lim,
                          num_th1d_bins);

    hNphe_1n_FD = hPlot1D("1n", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}", directories.Nphe_dir_map.at("Nphe_1n"), "01_Nphe_1n", 0, nphe_lim,
                          num_th1d_bins);

    hNphe_2p_FD = hPlot1D("2p", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}", directories.Nphe_dir_map.at("Nphe_2p"), "01_Nphe_2p", 0, nphe_lim,
                          num_th1d_bins);

    hNphe_pFDpCD_FD = hPlot1D("pFDpCD", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}", directories.Nphe_dir_map.at("Nphe_pFDpCD"), "01_Nphe_pFDpCD", 0,
                              nphe_lim, num_th1d_bins);

    hNphe_nFDpCD_FD = hPlot1D("nFDpCD", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}", directories.Nphe_dir_map.at("Nphe_nFDpCD"), "01_Nphe_nFDpCD", 0,
                              nphe_lim, num_th1d_bins);
}

void RecoAnalyzerHistograms::InitializeChi2Histograms(const Directories& directories, const HistogramSettings& settings) {
    const double chi2_lim = settings.Chi2_lim;
    const int num_th1d_bins = settings.numTH1Dbins;
    hChi2_Electron_FD = hPlot1D("no #(e) cut", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}", directories.Chi2_dir_map.at("Chi2_All_e"), "01_Electron_chi2",
                                -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Proton_CD = hPlot1D("no #(e) cut", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_dir_map.at("Chi2_All_e"), "02_Proton_chi2",
                              -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Proton_FD = hPlot1D("no #(e) cut", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_dir_map.at("Chi2_All_e"), "02_Proton_chi2",
                              -chi2_lim, chi2_lim, num_th1d_bins);

    hChi2_Electron_1e_cut_FD = hPlot1D("1e cut", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}", directories.Chi2_dir_map.at("Chi2_1e_cut"), "01_Electron_chi2",
                                       -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Proton_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_dir_map.at("Chi2_1e_cut"), "02_Proton_chi2",
                                     -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Proton_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_dir_map.at("Chi2_1e_cut"), "02_Proton_chi2",
                                     -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{K^{+}}", "#chi^{2}_{K^{+}} histogram", "#chi^{2}_{K^{+}}", directories.Chi2_dir_map.at("Chi2_1e_cut"),
                                    "03_Kplus_chi2", -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{K^{+}}", "#chi^{2}_{K^{+}} histogram", "#chi^{2}_{K^{+}}", directories.Chi2_dir_map.at("Chi2_1e_cut"),
                                    "03_Kplus_chi2", -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{K^{-}}", "#chi^{2}_{K^{-}} histogram", "#chi^{2}_{K^{-}}", directories.Chi2_dir_map.at("Chi2_1e_cut"),
                                     "04_Kminus_chi2", -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{K^{-}}", "#chi^{2}_{K^{-}} histogram", "#chi^{2}_{K^{-}}", directories.Chi2_dir_map.at("Chi2_1e_cut"),
                                     "04_Kminus_chi2", -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{#pi^{+}}", "#chi^{2}_{#pi^{+}} histogram", "#chi^{2}_{#pi^{+}}", directories.Chi2_dir_map.at("Chi2_1e_cut"),
                                     "05_piplus_chi2", -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{#pi^{+}}", "#chi^{2}_{#pi^{+}} histogram", "#chi^{2}_{#pi^{+}}", directories.Chi2_dir_map.at("Chi2_1e_cut"),
                                     "05_piplus_chi2", -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{#pi^{-}}", "#chi^{2}_{#pi^{-}} histogram", "#chi^{2}_{#pi^{-}}", directories.Chi2_dir_map.at("Chi2_1e_cut"),
                                      "06_piminus_chi2", -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{#pi^{-}}", "#chi^{2}_{#pi^{-}} histogram", "#chi^{2}_{#pi^{-}}", directories.Chi2_dir_map.at("Chi2_1e_cut"),
                                      "06_piminus_chi2", -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_deuteron_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{D}", "#chi^{2}_{D} histogram", "#chi^{2}_{D}", directories.Chi2_dir_map.at("Chi2_1e_cut"), "07_deuteron_chi2",
                                       -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_deuteron_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{D}", "#chi^{2}_{D} histogram", "#chi^{2}_{D}", directories.Chi2_dir_map.at("Chi2_1e_cut"), "07_deuteron_chi2",
                                       -chi2_lim, chi2_lim, num_th1d_bins);

    hChi2_Electron_1p_FD = hPlot1D("1p", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}", directories.Chi2_dir_map.at("Chi2_1p"), "01_Electron_Chi2_1p",
                                   -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Proton_1p_CD = hPlot1D("1p", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_dir_map.at("Chi2_1p"), "02_Proton_Chi2_1p", -chi2_lim,
                                 chi2_lim, num_th1d_bins);
    hChi2_Proton_1p_FD = hPlot1D("1p", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_dir_map.at("Chi2_1p"), "02_Proton_Chi2_1p", -chi2_lim,
                                 chi2_lim, num_th1d_bins);

    hChi2_Electron_1n_FD = hPlot1D("1n", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}", directories.Chi2_dir_map.at("Chi2_1n"), "01_Electron_Chi2_1n",
                                   -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Proton_1n_CD = hPlot1D("1n", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_dir_map.at("Chi2_1n"), "02_Proton_Chi2_1n", -chi2_lim,
                                 chi2_lim, num_th1d_bins);
    hChi2_Proton_1n_FD = hPlot1D("1n", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_dir_map.at("Chi2_1n"), "02_Proton_Chi2_1n", -chi2_lim,
                                 chi2_lim, num_th1d_bins);

    hChi2_Electron_2p_FD = hPlot1D("2p", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}", directories.Chi2_dir_map.at("Chi2_2p"), "01_Electron_Chi2_2p",
                                   -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Proton_2p_CD = hPlot1D("2p", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_dir_map.at("Chi2_2p"), "02_Proton_Chi2_2p", -chi2_lim,
                                 chi2_lim, num_th1d_bins);
    hChi2_Proton_2p_FD = hPlot1D("2p", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_dir_map.at("Chi2_2p"), "02_Proton_Chi2_2p", -chi2_lim,
                                 chi2_lim, num_th1d_bins);

    hChi2_Electron_pFDpCD_FD = hPlot1D("pFDpCD", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}", directories.Chi2_dir_map.at("Chi2_pFDpCD"),
                                       "01_Electron_Chi2_pFDpCD", -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Proton_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "#chi^{2}_{pCD}", "#chi^{2}_{pCD} histogram", "#chi^{2}_{pCD}", directories.Chi2_dir_map.at("Chi2_pFDpCD"),
                                     "02_Proton_Chi2_pFDpCD", -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Proton_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "#chi^{2}_{pFD}", "#chi^{2}_{pFD} histogram", "#chi^{2}_{pFD}", directories.Chi2_dir_map.at("Chi2_pFDpCD"),
                                     "02_Proton_Chi2_pFDpCD", -chi2_lim, chi2_lim, num_th1d_bins);

    hChi2_Electron_nFDpCD_FD = hPlot1D("nFDpCD", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}", directories.Chi2_dir_map.at("Chi2_nFDpCD"),
                                       "01_Electron_Chi2_nFDpCD", -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Proton_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "#chi^{2}_{pCD}", "#chi^{2}_{pCD} histogram", "#chi^{2}_{pCD}", directories.Chi2_dir_map.at("Chi2_nFDpCD"),
                                     "02_Proton_Chi2_nFDpCD", -chi2_lim, chi2_lim, num_th1d_bins);
    hChi2_Proton_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_dir_map.at("Chi2_nFDpCD"), "02_Proton_Chi2_nFDpCD",
                                     -chi2_lim, chi2_lim, num_th1d_bins);
}

void RecoAnalyzerHistograms::InitializeVertexHistograms(const Directories& directories, const HistogramSettings& settings) {
    const double Vz_FD_llim = settings.Vz_FD_llim;
    const double Vz_FD_ulim = settings.Vz_FD_ulim;
    const double Vz_CD_llim = settings.Vz_CD_llim;
    const double Vz_CD_ulim = settings.Vz_CD_ulim;
    const double Momentum_llim = settings.Momentum_llim;
    const double Momentum_ulim = settings.Momentum_ulim;
    const double Theta_llim_FD = settings.Theta_llim_FD;
    const double Theta_ulim_FD = settings.Theta_ulim_FD;
    const double Theta_llim_CD = settings.Theta_llim_CD;
    const double Theta_ulim_CD = settings.Theta_ulim_CD;
    const double Phi_llim = settings.Phi_llim;
    const double Phi_ulim = settings.Phi_ulim;
    const double dV_llim_FD = settings.dV_llim_FD;
    const double dV_ulim_FD = settings.dV_ulim_FD;
    const double dV_llim_CD = settings.dV_llim_CD;
    const double dV_ulim_CD = settings.dV_ulim_CD;
    const double dV_lim = settings.dV_lim;
    const int numTH1Dbins = settings.numTH1Dbins;
    const int numTH2Dbins = settings.numTH2Dbins;
    const bool Log_scale_Vx_plots = settings.Log_scale_Vx_plots;
    const bool Log_scale_Vy_plots = settings.Log_scale_Vy_plots;
    const bool Log_scale_Vz_plots = settings.Log_scale_Vz_plots;
    const bool Log_scale_dVz_plots = settings.Log_scale_dVz_plots;
#pragma region /* Vertex plots */

    // Vertex plots (1e cut)
    hVx_Electron_1e_cut_FD = hPlot1D("1e cut", "", "V_{x}^{e}", "V_{x}^{e} histogram", "V_{x}^{e} [cm]", directories.Vertex_dir_map.at("Vertex_Electron_1e_cut_Vtx"), "01_Electron_Vx",
                                     Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVx_Electron_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hVy_Electron_1e_cut_FD = hPlot1D("1e cut", "", "V_{y}^{e}", "V_{y}^{e} histogram", "V_{y}^{e} [cm]", directories.Vertex_dir_map.at("Vertex_Electron_1e_cut_Vtx"), "02_Electron_Vy",
                                     Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVy_Electron_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hVz_Electron_1e_cut_FD = hPlot1D("1e cut", "", "V_{z}^{e}", "V_{z}^{e} histogram", "V_{z}^{e} [cm]", directories.Vertex_dir_map.at("Vertex_Electron_1e_cut_Vtx"), "03_Electron_Vz",
                                     Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVz_Electron_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);
    hVz_Electron_vs_P_e_1e_cut_FD =
        hPlot2D("1e cut", "FD", "V_{z}^{e} vs. P_{e}", "V_{z}^{e} vs. P_{e}", "P_{e} [GeV/c]", "V_{z}^{e} [cm]", directories.Vertex_dir_map.at("Vertex_Electron_1e_cut_Vtx"),
                "04_Vz_Electron_vs_P_e_1e_cut_FD", Momentum_llim, Momentum_ulim, Vz_FD_llim, Vz_FD_ulim, numTH2Dbins, numTH2Dbins);
    hVz_Electron_vs_Theta_e_1e_cut_FD = hPlot2D("1e cut", "FD", "V_{z}^{e} vs. #theta_{e}", "V_{z}^{e} vs. #theta_{e}", "#theta_{e} [#circ]", "V_{z}^{e} [cm]",
                                                directories.Vertex_dir_map.at("Vertex_Electron_1e_cut_Vtx"), "05_Vz_Electron_vs_Theta_e_1e_cut_FD", Theta_llim_FD,
                                                Theta_ulim_FD, Vz_FD_llim, Vz_FD_ulim, numTH2Dbins, numTH2Dbins);
    hVz_Electron_vs_Phi_e_1e_cut_FD = hPlot2D("1e cut", "FD", "V_{z}^{e} vs. #phi_{e}", "V_{z}^{e} vs. #theta_{e}", "#phi_{e} [#circ]", "V_{z}^{e} [cm]",
                                              directories.Vertex_dir_map.at("Vertex_Electron_1e_cut_Vtx"), "06_Vz_Electron_vs_Phi_e_1e_cut_FD", Phi_llim, Phi_ulim,
                                              Vz_FD_llim, Vz_FD_ulim, numTH2Dbins, numTH2Dbins);
    hVx_Proton_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{p}", "V_{x}^{p} histogram", "V_{x}^{p} [cm]", directories.Vertex_dir_map.at("Vertex_Proton_1e_cut_Vtx"), "01_Proton_Vx",
                                   Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVx_Proton_1e_cut_CD.SetLogScalePlot(Log_scale_Vx_plots);
    hVx_Proton_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{p}", "V_{x}^{p} histogram", "V_{x}^{p} [cm]", directories.Vertex_dir_map.at("Vertex_Proton_1e_cut_Vtx"), "01_Proton_Vx",
                                   Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVx_Proton_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hVy_Proton_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{p}", "V_{y}^{p} histogram", "V_{y}^{p} [cm]", directories.Vertex_dir_map.at("Vertex_Proton_1e_cut_Vtx"), "02_Proton_Vy",
                                   Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVy_Proton_1e_cut_CD.SetLogScalePlot(Log_scale_Vy_plots);
    hVy_Proton_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{p}", "V_{y}^{p} histogram", "V_{y}^{p} [cm]", directories.Vertex_dir_map.at("Vertex_Proton_1e_cut_Vtx"), "02_Proton_Vy",
                                   Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVy_Proton_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hVz_Proton_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{p}", "V_{z}^{p} histogram", "V_{z}^{p} [cm]", directories.Vertex_dir_map.at("Vertex_Proton_1e_cut_Vtx"), "03_Proton_Vz",
                                   Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVz_Proton_1e_cut_CD.SetLogScalePlot(Log_scale_Vz_plots);
    hVz_Proton_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{p}", "V_{z}^{p} histogram", "V_{z}^{p} [cm]", directories.Vertex_dir_map.at("Vertex_Proton_1e_cut_Vtx"), "03_Proton_Vz",
                                   Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVz_Proton_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);
    hVz_Proton_vs_P_p_1e_cut_FD =
        hPlot2D("1e cut", "FD", "V_{z}^{p} vs. P_{p}", "V_{z}^{p} vs. P_{p}", "P_{p} [GeV/c]", "V_{z}^{p} [cm]", directories.Vertex_dir_map.at("Vertex_Proton_1e_cut_Vtx"),
                "04a_Vz_Proton_vs_P_p_1e_cut_FD", Momentum_llim, Momentum_ulim, Vz_FD_llim, Vz_FD_ulim, numTH2Dbins, numTH2Dbins);
    hVz_Proton_vs_Theta_p_1e_cut_FD = hPlot2D("1e cut", "FD", "V_{z}^{p} vs. #theta_{p}", "V_{z}^{p} vs. #theta_{p}", "#theta_{p} [#circ]", "V_{z}^{p} [cm]",
                                              directories.Vertex_dir_map.at("Vertex_Proton_1e_cut_Vtx"), "05a_Vz_Proton_vs_Theta_p_1e_cut_FD", Theta_llim_FD, Theta_ulim_FD,
                                              Vz_FD_llim, Vz_FD_ulim, numTH2Dbins, numTH2Dbins);
    hVz_Proton_vs_Phi_p_1e_cut_FD =
        hPlot2D("1e cut", "FD", "V_{z}^{p} vs. #phi_{p}", "V_{z}^{p} vs. #theta_{p}", "#phi_{p} [#circ]", "V_{z}^{p} [cm]", directories.Vertex_dir_map.at("Vertex_Proton_1e_cut_Vtx"),
                "06a_Vz_Proton_vs_Phi_p_1e_cut_FD", Phi_llim, Phi_ulim, Vz_FD_llim, Vz_FD_ulim, numTH2Dbins, numTH2Dbins);
    hVz_Proton_vs_P_p_1e_cut_CD =
        hPlot2D("1e cut", "CD", "V_{z}^{p} vs. P_{p}", "V_{z}^{p} vs. P_{p}", "P_{p} [GeV/c]", "V_{z}^{p} [cm]", directories.Vertex_dir_map.at("Vertex_Proton_1e_cut_Vtx"),
                "04b_Vz_Proton_vs_P_p_1e_cut_CD", Momentum_llim, Momentum_ulim, Vz_CD_llim, Vz_CD_ulim, numTH2Dbins, numTH2Dbins);
    hVz_Proton_vs_Theta_p_1e_cut_CD = hPlot2D("1e cut", "CD", "V_{z}^{p} vs. #theta_{p}", "V_{z}^{p} vs. #theta_{p}", "#theta_{p} [#circ]", "V_{z}^{p} [cm]",
                                              directories.Vertex_dir_map.at("Vertex_Proton_1e_cut_Vtx"), "05b_Vz_Proton_vs_Theta_p_1e_cut_CD", Theta_llim_CD, Theta_ulim_CD,
                                              Vz_CD_llim, Vz_CD_ulim, numTH2Dbins, numTH2Dbins);
    hVz_Proton_vs_Phi_p_1e_cut_CD =
        hPlot2D("1e cut", "CD", "V_{z}^{p} vs. #phi_{p}", "V_{z}^{p} vs. #theta_{p}", "#phi_{p} [#circ]", "V_{z}^{p} [cm]", directories.Vertex_dir_map.at("Vertex_Proton_1e_cut_Vtx"),
                "06b_Vz_Proton_vs_Phi_p_1e_cut_CD", Phi_llim, Phi_ulim, Vz_CD_llim, Vz_CD_ulim, numTH2Dbins, numTH2Dbins);
    hVx_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{K^{+}}", "V_{x}^{K^{+}} histogram", "V_{x}^{K^{+}} [cm]", directories.Vertex_dir_map.at("Vertex_Kplus_1e_cut_Vtx"),
                                  "01_Kplus_Vx", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVx_Kplus_1e_cut_CD.SetLogScalePlot(Log_scale_Vx_plots);
    hVx_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{K^{+}}", "V_{x}^{K^{+}} histogram", "V_{x}^{K^{+}} [cm]", directories.Vertex_dir_map.at("Vertex_Kplus_1e_cut_Vtx"),
                                  "01_Kplus_Vx", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVx_Kplus_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hVy_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{K^{+}}", "V_{y}^{K^{+}} histogram", "V_{y}^{K^{+}} [cm]", directories.Vertex_dir_map.at("Vertex_Kplus_1e_cut_Vtx"),
                                  "02_Kplus_Vy", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVy_Kplus_1e_cut_CD.SetLogScalePlot(Log_scale_Vy_plots);
    hVy_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{K^{+}}", "V_{y}^{K^{+}} histogram", "V_{y}^{K^{+}} [cm]", directories.Vertex_dir_map.at("Vertex_Kplus_1e_cut_Vtx"),
                                  "02_Kplus_Vy", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVy_Kplus_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hVz_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{K^{+}}", "V_{z}^{K^{+}} histogram", "V_{z}^{K^{+}} [cm]", directories.Vertex_dir_map.at("Vertex_Kplus_1e_cut_Vtx"),
                                  "03_Kplus_Vz", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVz_Kplus_1e_cut_CD.SetLogScalePlot(Log_scale_Vz_plots);
    hVz_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{K^{+}}", "V_{z}^{K^{+}} histogram", "V_{z}^{K^{+}} [cm]", directories.Vertex_dir_map.at("Vertex_Kplus_1e_cut_Vtx"),
                                  "03_Kplus_Vz", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVz_Kplus_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);
    hVx_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{K^{-}}", "V_{x}^{K^{-}} histogram", "V_{x}^{K^{-}} [cm]", directories.Vertex_dir_map.at("Vertex_Kminus_1e_cut_Vtx"),
                                   "01_Kminus_Vx", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVx_Kminus_1e_cut_CD.SetLogScalePlot(Log_scale_Vx_plots);
    hVx_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{K^{-}}", "V_{x}^{K^{-}} histogram", "V_{x}^{K^{-}} [cm]", directories.Vertex_dir_map.at("Vertex_Kminus_1e_cut_Vtx"),
                                   "01_Kminus_Vx", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVx_Kminus_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hVy_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{K^{-}}", "V_{y}^{K^{-}} histogram", "V_{y}^{K^{-}} [cm]", directories.Vertex_dir_map.at("Vertex_Kminus_1e_cut_Vtx"),
                                   "02_Kminus_Vy", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVy_Kminus_1e_cut_CD.SetLogScalePlot(Log_scale_Vy_plots);
    hVy_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{K^{-}}", "V_{y}^{K^{-}} histogram", "V_{y}^{K^{-}} [cm]", directories.Vertex_dir_map.at("Vertex_Kminus_1e_cut_Vtx"),
                                   "02_Kminus_Vy", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVy_Kminus_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hVz_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{K^{-}}", "V_{z}^{K^{-}} histogram", "V_{z}^{K^{-}} [cm]", directories.Vertex_dir_map.at("Vertex_Kminus_1e_cut_Vtx"),
                                   "03_Kminus_Vz", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVz_Kminus_1e_cut_CD.SetLogScalePlot(Log_scale_Vz_plots);
    hVz_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{K^{-}}", "V_{z}^{K^{-}} histogram", "V_{z}^{K^{-}} [cm]", directories.Vertex_dir_map.at("Vertex_Kminus_1e_cut_Vtx"),
                                   "03_Kminus_Vz", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVz_Kminus_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);
    hVx_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{#pi^{+}}", "V_{x}^{#pi^{+}} histogram", "V_{x}^{#pi^{+}} [cm]", directories.Vertex_dir_map.at("Vertex_piplus_1e_cut_Vtx"),
                                   "01_piplus_Vx", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVx_piplus_1e_cut_CD.SetLogScalePlot(Log_scale_Vx_plots);
    hVx_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{#pi^{+}}", "V_{x}^{#pi^{+}} histogram", "V_{x}^{#pi^{+}} [cm]", directories.Vertex_dir_map.at("Vertex_piplus_1e_cut_Vtx"),
                                   "01_piplus_Vx", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVx_piplus_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hVy_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{#pi^{+}}", "V_{y}^{#pi^{+}} histogram", "V_{y}^{#pi^{+}} [cm]", directories.Vertex_dir_map.at("Vertex_piplus_1e_cut_Vtx"),
                                   "02_piplus_Vy", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVy_piplus_1e_cut_CD.SetLogScalePlot(Log_scale_Vy_plots);
    hVy_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{#pi^{+}}", "V_{y}^{#pi^{+}} histogram", "V_{y}^{#pi^{+}} [cm]", directories.Vertex_dir_map.at("Vertex_piplus_1e_cut_Vtx"),
                                   "02_piplus_Vy", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVy_piplus_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hVz_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{#pi^{+}}", "V_{z}^{#pi^{+}} histogram", "V_{z}^{#pi^{+}} [cm]", directories.Vertex_dir_map.at("Vertex_piplus_1e_cut_Vtx"),
                                   "03_piplus_Vz", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVz_piplus_1e_cut_CD.SetLogScalePlot(Log_scale_Vz_plots);
    hVz_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{#pi^{+}}", "V_{z}^{#pi^{+}} histogram", "V_{z}^{#pi^{+}} [cm]", directories.Vertex_dir_map.at("Vertex_piplus_1e_cut_Vtx"),
                                   "03_piplus_Vz", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVz_piplus_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);
    hVx_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{#pi^{-}}", "V_{x}^{#pi^{-}} histogram", "V_{x}^{#pi^{-}} [cm]", directories.Vertex_dir_map.at("Vertex_piminus_1e_cut_Vtx"),
                                    "01_piminus_Vx", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVx_piminus_1e_cut_CD.SetLogScalePlot(Log_scale_Vx_plots);
    hVx_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{#pi^{-}}", "V_{x}^{#pi^{-}} histogram", "V_{x}^{#pi^{-}} [cm]", directories.Vertex_dir_map.at("Vertex_piminus_1e_cut_Vtx"),
                                    "01_piminus_Vx", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVx_piminus_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hVy_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{#pi^{-}}", "V_{y}^{#pi^{-}} histogram", "V_{y}^{#pi^{-}} [cm]", directories.Vertex_dir_map.at("Vertex_piminus_1e_cut_Vtx"),
                                    "02_piminus_Vy", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVy_piminus_1e_cut_CD.SetLogScalePlot(Log_scale_Vy_plots);
    hVy_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{#pi^{-}}", "V_{y}^{#pi^{-}} histogram", "V_{y}^{#pi^{-}} [cm]", directories.Vertex_dir_map.at("Vertex_piminus_1e_cut_Vtx"),
                                    "02_piminus_Vy", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVy_piminus_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hVz_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{#pi^{-}}", "V_{z}^{#pi^{-}} histogram", "V_{z}^{#pi^{-}} [cm]", directories.Vertex_dir_map.at("Vertex_piminus_1e_cut_Vtx"),
                                    "03_piminus_Vz", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVz_piminus_1e_cut_CD.SetLogScalePlot(Log_scale_Vz_plots);
    hVz_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{#pi^{-}}", "V_{z}^{#pi^{-}} histogram", "V_{z}^{#pi^{-}} [cm]", directories.Vertex_dir_map.at("Vertex_piminus_1e_cut_Vtx"),
                                    "03_piminus_Vz", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVz_piminus_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);
    hVx_Deuteron_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{D}", "V_{x}^{D} histogram", "V_{x}^{D} [cm]", directories.Vertex_dir_map.at("Vertex_deuteron_1e_cut_Vtx"),
                                     "01_Deuteron_Vx", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVx_Deuteron_1e_cut_CD.SetLogScalePlot(Log_scale_Vx_plots);
    hVx_Deuteron_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{D}", "V_{x}^{D} histogram", "V_{x}^{D} [cm]", directories.Vertex_dir_map.at("Vertex_deuteron_1e_cut_Vtx"),
                                     "01_Deuteron_Vx", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVx_Deuteron_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hVy_Deuteron_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{D}", "V_{y}^{D} histogram", "V_{y}^{D} [cm]", directories.Vertex_dir_map.at("Vertex_deuteron_1e_cut_Vtx"),
                                     "02_Deuteron_Vy", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVy_Deuteron_1e_cut_CD.SetLogScalePlot(Log_scale_Vy_plots);
    hVy_Deuteron_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{D}", "V_{y}^{D} histogram", "V_{y}^{D} [cm]", directories.Vertex_dir_map.at("Vertex_deuteron_1e_cut_Vtx"),
                                     "02_Deuteron_Vy", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVy_Deuteron_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hVz_Deuteron_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{D}", "V_{z}^{D} histogram", "V_{z}^{D} [cm]", directories.Vertex_dir_map.at("Vertex_deuteron_1e_cut_Vtx"),
                                     "03_Deuteron_Vz", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hVz_Deuteron_1e_cut_CD.SetLogScalePlot(Log_scale_Vz_plots);
    hVz_Deuteron_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{D}", "V_{z}^{D} histogram", "V_{z}^{D} [cm]", directories.Vertex_dir_map.at("Vertex_deuteron_1e_cut_Vtx"),
                                     "03_Deuteron_Vz", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hVz_Deuteron_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);

    // dV plots (1e cut)
    hdVx_Proton_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{p}_{x}", "Vertex correlation dV^{p}_{x}=V^{p}_{x}-V^{e}_{x}", "dV^{p}_{x} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_Proton_1e_cut_Vtx"), "01_dVx_d", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVy_Proton_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{p}_{y}", "Vertex correlation dV^{p}_{y}=V^{p}_{y}-V^{e}_{y}", "dV^{p}_{y} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_Proton_1e_cut_Vtx"), "02_dVy_d", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVz_Proton_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{p}_{z}", "Vertex correlation dV^{p}_{z}=V^{p}_{z}-V^{e}_{z}", "dV^{p}_{z} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_Proton_1e_cut_Vtx"), "03_dVz_d", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVz_Proton_FD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hdVz_Proton_vs_P_p_1e_cut_FD =
        hPlot2D("1e cut", "FD", "dV^{p}_{z} vs. P_{p}", "dV^{p}_{z} vs. P_{p}", "P_{p} [GeV/c]", "dV^{p}_{z} [cm]", directories.Vertex_dir_map.at("Vertex_corr_Proton_1e_cut_Vtx"),
                "04a_dVz_Proton_vs_P_p_1e_cut_FD", Momentum_llim, Momentum_ulim, dV_llim_FD, dV_ulim_FD, numTH2Dbins, numTH2Dbins);
    hdVz_Proton_vs_Theta_p_1e_cut_FD = hPlot2D("1e cut", "FD", "dV^{p}_{z} vs. #theta_{p}", "dV^{p}_{z} vs. #theta_{p}", "#theta_{p} [#circ]", "dV^{p}_{z} [cm]",
                                               directories.Vertex_dir_map.at("Vertex_corr_Proton_1e_cut_Vtx"), "05a_dVz_Proton_vs_Theta_p_1e_cut_FD", Theta_llim_FD,
                                               Theta_ulim_FD, dV_llim_FD, dV_ulim_FD, numTH2Dbins, numTH2Dbins);
    hdVz_Proton_vs_Phi_p_1e_cut_FD = hPlot2D("1e cut", "FD", "dV^{p}_{z} vs. #phi_{p}", "dV^{p}_{z} vs. #theta_{p}", "#phi_{p} [#circ]", "dV^{p}_{z} [cm]",
                                             directories.Vertex_dir_map.at("Vertex_corr_Proton_1e_cut_Vtx"), "06a_dVz_Proton_vs_Phi_p_1e_cut_FD", Phi_llim, Phi_ulim,
                                             dV_llim_FD, dV_ulim_FD, numTH2Dbins, numTH2Dbins);
    hdVx_Proton_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{p}_{x}", "Vertex correlation dV^{p}_{x}=V^{p}_{x}-V^{e}_{x}", "dV^{p}_{x} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_Proton_1e_cut_Vtx"), "01_dVx_d", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVy_Proton_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{p}_{y}", "Vertex correlation dV^{p}_{y}=V^{p}_{y}-V^{e}_{y}", "dV^{p}_{y} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_Proton_1e_cut_Vtx"), "02_dVy_d", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVz_Proton_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{p}_{z}", "Vertex correlation dV^{p}_{z}=V^{p}_{z}-V^{e}_{z}", "dV^{p}_{z} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_Proton_1e_cut_Vtx"), "03_dVz_d", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVz_Proton_CD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hdVz_Proton_vs_P_p_1e_cut_CD =
        hPlot2D("1e cut", "CD", "dV^{p}_{z} vs. P_{p}", "dV^{p}_{z} vs. P_{p}", "P_{p} [GeV/c]", "dV^{p}_{z} [cm]", directories.Vertex_dir_map.at("Vertex_corr_Proton_1e_cut_Vtx"),
                "04b_dVz_Proton_vs_P_p_1e_cut_CD", Momentum_llim, Momentum_ulim, dV_llim_CD, dV_ulim_CD, numTH2Dbins, numTH2Dbins);
    hdVz_Proton_vs_Theta_p_1e_cut_CD = hPlot2D("1e cut", "CD", "dV^{p}_{z} vs. #theta_{p}", "dV^{p}_{z} vs. #theta_{p}", "#theta_{p} [#circ]", "dV^{p}_{z} [cm]",
                                               directories.Vertex_dir_map.at("Vertex_corr_Proton_1e_cut_Vtx"), "05b_dVz_Proton_vs_Theta_p_1e_cut_CD", Theta_llim_CD,
                                               Theta_ulim_CD, dV_llim_CD, dV_ulim_CD, numTH2Dbins, numTH2Dbins);
    hdVz_Proton_vs_Phi_p_1e_cut_CD = hPlot2D("1e cut", "CD", "dV^{p}_{z} vs. #phi_{p}", "dV^{p}_{z} vs. #theta_{p}", "#phi_{p} [#circ]", "dV^{p}_{z} [cm]",
                                             directories.Vertex_dir_map.at("Vertex_corr_Proton_1e_cut_Vtx"), "06b_dVz_Proton_vs_Phi_p_1e_cut_CD", Phi_llim, Phi_ulim,
                                             dV_llim_CD, dV_ulim_CD, numTH2Dbins, numTH2Dbins);
    hdVx_Kplus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{K^{+}}_{x}", "Vertex correlation dV^{K^{+}}_{x}=V^{K^{+}}_{x}-V^{e}_{x}", "dV^{K^{+}}_{x} [cm]",
                                   directories.Vertex_dir_map.at("Vertex_corr_Kplus_1e_cut_Vtx"), "01_dVx_Kplus", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVy_Kplus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{K^{+}}_{y}", "Vertex correlation dV^{K^{+}}_{y}=V^{K^{+}}_{y}-V^{e}_{y}", "dV^{K^{+}}_{y} [cm]",
                                   directories.Vertex_dir_map.at("Vertex_corr_Kplus_1e_cut_Vtx"), "02_dVy_Kplus", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVz_Kplus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{K^{+}}_{z}", "Vertex correlation dV^{K^{+}}_{z}=V^{K^{+}}_{z}-V^{e}_{z}", "dV^{K^{+}}_{z} [cm]",
                                   directories.Vertex_dir_map.at("Vertex_corr_Kplus_1e_cut_Vtx"), "03_dVz_Kplus", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVz_Kplus_FD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hdVx_Kplus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{K^{+}}_{x}", "Vertex correlation dV^{K^{+}}_{x}=V^{K^{+}}_{x}-V^{e}_{x}", "dV^{K^{+}}_{x} [cm]",
                                   directories.Vertex_dir_map.at("Vertex_corr_Kplus_1e_cut_Vtx"), "01_dVx_Kplus", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVy_Kplus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{K^{+}}_{y}", "Vertex correlation dV^{K^{+}}_{y}=V^{K^{+}}_{y}-V^{e}_{y}", "dV^{K^{+}}_{y} [cm]",
                                   directories.Vertex_dir_map.at("Vertex_corr_Kplus_1e_cut_Vtx"), "02_dVy_Kplus", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVz_Kplus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{K^{+}}_{z}", "Vertex correlation dV^{K^{+}}_{z}=V^{K^{+}}_{z}-V^{e}_{z}", "dV^{K^{+}}_{z} [cm]",
                                   directories.Vertex_dir_map.at("Vertex_corr_Kplus_1e_cut_Vtx"), "03_dVz_Kplus", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVz_Kplus_CD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hdVx_Kminus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{K^{-}}_{x}", "Vertex correlation dV^{K^{-}}_{x}=V^{K^{-}}_{x}-V^{e}_{x}", "dV^{K^{-}}_{x} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_Kminus_1e_cut_Vtx"), "01_dVx_Kminus", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVy_Kminus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{K^{-}}_{y}", "Vertex correlation dV^{K^{-}}_{y}=V^{K^{-}}_{y}-V^{e}_{y}", "dV^{K^{-}}_{y} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_Kminus_1e_cut_Vtx"), "02_dVy_Kminus", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVz_Kminus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{K^{-}}_{z}", "Vertex correlation dV^{K^{-}}_{z}=V^{K^{-}}_{z}-V^{e}_{z}", "dV^{K^{-}}_{z} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_Kminus_1e_cut_Vtx"), "03_dVz_Kminus", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVz_Kminus_FD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hdVx_Kminus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{K^{-}}_{x}", "Vertex correlation dV^{K^{-}}_{x}=V^{K^{-}}_{x}-V^{e}_{x}", "dV^{K^{-}}_{x} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_Kminus_1e_cut_Vtx"), "01_dVx_Kminus", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVy_Kminus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{K^{-}}_{y}", "Vertex correlation dV^{K^{-}}_{y}=V^{K^{-}}_{y}-V^{e}_{y}", "dV^{K^{-}}_{y} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_Kminus_1e_cut_Vtx"), "02_dVy_Kminus", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVz_Kminus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{K^{-}}_{z}", "Vertex correlation dV^{K^{-}}_{z}=V^{K^{-}}_{z}-V^{e}_{z}", "dV^{K^{-}}_{z} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_Kminus_1e_cut_Vtx"), "03_dVz_Kminus", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVz_Kminus_CD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hdVx_piplus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{#pi^{+}}_{x}", "Vertex correlation dV^{#pi^{+}}_{x}=V^{#pi^{+}}_{x}-V^{e}_{x}", "dV^{#pi^{+}}_{x} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_piplus_1e_cut_Vtx"), "01_dVx_Piplus", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVy_piplus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{#pi^{+}}_{y}", "Vertex correlation dV^{#pi^{+}}_{y}=V^{#pi^{+}}_{y}-V^{e}_{y}", "dV^{#pi^{+}}_{y} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_piplus_1e_cut_Vtx"), "02_dVy_Piplus", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVz_piplus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{#pi^{+}}_{z}", "Vertex correlation dV^{#pi^{+}}_{z}=V^{#pi^{+}}_{z}-V^{e}_{z}", "dV^{#pi^{+}}_{z} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_piplus_1e_cut_Vtx"), "03_dVz_Piplus", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVz_piplus_FD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hdVx_piplus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{#pi^{+}}_{x}", "Vertex correlation dV^{#pi^{+}}_{x}=V^{#pi^{+}}_{x}-V^{e}_{x}", "dV^{#pi^{+}}_{x} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_piplus_1e_cut_Vtx"), "01_dVx_Piplus", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVy_piplus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{#pi^{+}}_{y}", "Vertex correlation dV^{#pi^{+}}_{y}=V^{#pi^{+}}_{y}-V^{e}_{y}", "dV^{#pi^{+}}_{y} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_piplus_1e_cut_Vtx"), "02_dVy_Piplus", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVz_piplus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{#pi^{+}}_{z}", "Vertex correlation dV^{#pi^{+}}_{z}=V^{#pi^{+}}_{z}-V^{e}_{z}", "dV^{#pi^{+}}_{z} [cm]",
                                    directories.Vertex_dir_map.at("Vertex_corr_piplus_1e_cut_Vtx"), "03_dVz_Piplus", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVz_piplus_CD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hdVx_piminus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{#pi^{-}}_{x}", "Vertex correlation dV^{#pi^{-}}_{x}=V^{#pi^{-}}_{x}-V^{e}_{x}", "dV^{#pi^{-}}_{x} [cm]",
                                     directories.Vertex_dir_map.at("Vertex_corr_piminus_1e_cut_Vtx"), "01_dVx_Piminus", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVy_piminus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{#pi^{-}}_{y}", "Vertex correlation dV^{#pi^{-}}_{y}=V^{#pi^{-}}_{y}-V^{e}_{y}", "dV^{#pi^{-}}_{y} [cm]",
                                     directories.Vertex_dir_map.at("Vertex_corr_piminus_1e_cut_Vtx"), "02_dVy_Piminus", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVz_piminus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{#pi^{-}}_{z}", "Vertex correlation dV^{#pi^{-}}_{z}=V^{#pi^{-}}_{z}-V^{e}_{z}", "dV^{#pi^{-}}_{z} [cm]",
                                     directories.Vertex_dir_map.at("Vertex_corr_piminus_1e_cut_Vtx"), "03_dVz_Piminus", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVz_piminus_FD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hdVx_piminus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{#pi^{-}}_{x}", "Vertex correlation dV^{#pi^{-}}_{x}=V^{#pi^{-}}_{x}-V^{e}_{x}", "dV^{#pi^{-}}_{x} [cm]",
                                     directories.Vertex_dir_map.at("Vertex_corr_piminus_1e_cut_Vtx"), "01_dVx_Piminus", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVy_piminus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{#pi^{-}}_{y}", "Vertex correlation dV^{#pi^{-}}_{y}=V^{#pi^{-}}_{y}-V^{e}_{y}", "dV^{#pi^{-}}_{y} [cm]",
                                     directories.Vertex_dir_map.at("Vertex_corr_piminus_1e_cut_Vtx"), "02_dVy_Piminus", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVz_piminus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{#pi^{-}}_{z}", "Vertex correlation dV^{#pi^{-}}_{z}=V^{#pi^{-}}_{z}-V^{e}_{z}", "dV^{#pi^{-}}_{z} [cm]",
                                     directories.Vertex_dir_map.at("Vertex_corr_piminus_1e_cut_Vtx"), "03_dVz_Piminus", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVz_piminus_CD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hdVx_Deuteron_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{D}_{x}", "Vertex correlation dV^{D}_{x}=V^{D}_{x}-V^{e}_{x}", "dV^{D}_{x} [cm]",
                                      directories.Vertex_dir_map.at("Vertex_corr_deuteron_1e_cut_Vtx"), "01_dVx_d", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVy_Deuteron_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{D}_{y}", "Vertex correlation dV^{D}_{y}=V^{D}_{y}-V^{e}_{y}", "dV^{D}_{y} [cm]",
                                      directories.Vertex_dir_map.at("Vertex_corr_deuteron_1e_cut_Vtx"), "02_dVy_d", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVz_Deuteron_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{D}_{z}", "Vertex correlation dV^{D}_{z}=V^{D}_{z}-V^{e}_{z}", "dV^{D}_{z} [cm]",
                                      directories.Vertex_dir_map.at("Vertex_corr_deuteron_1e_cut_Vtx"), "03_dVz_d", dV_llim_FD, dV_ulim_FD, numTH1Dbins);
    hdVz_Deuteron_FD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hdVx_Deuteron_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{D}_{x}", "Vertex correlation dV^{D}_{x}=V^{D}_{x}-V^{e}_{x}", "dV^{D}_{x} [cm]",
                                      directories.Vertex_dir_map.at("Vertex_corr_deuteron_1e_cut_Vtx"), "01_dVx_d", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVy_Deuteron_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{D}_{y}", "Vertex correlation dV^{D}_{y}=V^{D}_{y}-V^{e}_{y}", "dV^{D}_{y} [cm]",
                                      directories.Vertex_dir_map.at("Vertex_corr_deuteron_1e_cut_Vtx"), "02_dVy_d", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVz_Deuteron_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{D}_{z}", "Vertex correlation dV^{D}_{z}=V^{D}_{z}-V^{e}_{z}", "dV^{D}_{z} [cm]",
                                      directories.Vertex_dir_map.at("Vertex_corr_deuteron_1e_cut_Vtx"), "03_dVz_d", dV_llim_CD, dV_ulim_CD, numTH1Dbins);
    hdVz_Deuteron_CD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);

    // dV plots (1p)
    hdVx_1p = hPlot1D("1p", "", "Vertex corr. dV^{p}_{x}", "Vertex correlation dV^{p}_{x}=V^{p}_{x}-V^{e}_{x}", "dV^{p}_{x} [cm]", directories.Vertex_dir_map.at("Vertex_dV_1p"),
                      "01_dVx", -dV_lim, dV_lim, numTH1Dbins);
    hdVy_1p = hPlot1D("1p", "", "Vertex corr. dV^{p}_{y}", "Vertex correlation dV^{p}_{y}=V^{p}_{y}-V^{e}_{y}", "dV^{p}_{y} [cm]", directories.Vertex_dir_map.at("Vertex_dV_1p"),
                      "02_dVy", -dV_lim, dV_lim, numTH1Dbins);
    hdVz_1p = hPlot1D("1p", "", "Vertex corr. dV^{p}_{z}", "Vertex correlation dV^{p}_{z}=V^{p}_{z}-V^{e}_{z}", "dV^{p}_{z} [cm]", directories.Vertex_dir_map.at("Vertex_dV_1p"),
                      "03_dVz", -dV_lim, dV_lim, numTH1Dbins);

    // dV plots (1n)
    hdVx_1n = hPlot1D("1n", "", "Vertex corr. dV^{n}_{x}", "Vertex correlation dV^{n}_{x}=V^{n}_{x}-V^{e}_{x}", "dV^{n}_{x} [cm]", directories.Vertex_dir_map.at("Vertex_dV_1n"),
                      "01_dVx", -dV_lim, dV_lim, numTH1Dbins);
    hdVy_1n = hPlot1D("1n", "", "Vertex corr. dV^{n}_{y}", "Vertex correlation dV^{n}_{y}=V^{n}_{y}-V^{e}_{y}", "dV^{n}_{y} [cm]", directories.Vertex_dir_map.at("Vertex_dV_1n"),
                      "02_dVy", -dV_lim, dV_lim, numTH1Dbins);
    hdVz_1n = hPlot1D("1n", "", "Vertex corr. dV^{n}_{z}", "Vertex correlation dV^{n}_{z}=V^{n}_{z}-V^{e}_{z}", "dV^{n}_{z} [cm]", directories.Vertex_dir_map.at("Vertex_dV_1n"),
                      "03_dVz", -dV_lim, dV_lim, numTH1Dbins);

    // dV plots (2p)
    hdVx_2p = hPlot1D("2p", "", "Vertex corr. dV^{p}_{x}", "Vertex correlation dV^{p}_{x}=V^{p}_{x}-V^{e}_{x}", "dV^{p}_{x} [cm]", directories.Vertex_dir_map.at("Vertex_dV_2p"),
                      "01_dVx", -dV_lim, dV_lim, numTH1Dbins);
    hdVy_2p = hPlot1D("2p", "", "Vertex corr. dV^{p}_{y}", "Vertex correlation dV^{p}_{y}=V^{p}_{y}-V^{e}_{y}", "dV^{p}_{y} [cm]", directories.Vertex_dir_map.at("Vertex_dV_2p"),
                      "02_dVy", -dV_lim, dV_lim, numTH1Dbins);
    hdVz_2p = hPlot1D("2p", "", "Vertex corr. dV^{p}_{z}", "Vertex correlation dV^{p}_{z}=V^{p}_{z}-V^{e}_{z}", "dV^{p}_{z} [cm]", directories.Vertex_dir_map.at("Vertex_dV_2p"),
                      "03_dVz", -dV_lim, dV_lim, numTH1Dbins);

    // dV plots (pFDpCD)
    hdVx_pFDpCD = hPlot1D("pFDpCD", "", "Vertex corr. dV^{p}_{x}", "Vertex correlation dV^{p}_{x}=V^{p}_{x}-V^{e}_{x}", "dV^{p}_{x} [cm]",
                          directories.Vertex_dir_map.at("Vertex_dV_pFDpCD"), "01_dVx", -dV_lim, dV_lim, numTH1Dbins);
    hdVy_pFDpCD = hPlot1D("pFDpCD", "", "Vertex corr. dV^{p}_{y}", "Vertex correlation dV^{p}_{y}=V^{p}_{y}-V^{e}_{y}", "dV^{p}_{y} [cm]",
                          directories.Vertex_dir_map.at("Vertex_dV_pFDpCD"), "02_dVy", -dV_lim, dV_lim, numTH1Dbins);
    hdVz_pFDpCD = hPlot1D("pFDpCD", "", "Vertex corr. dV^{p}_{z}", "Vertex correlation dV^{p}_{z}=V^{p}_{z}-V^{e}_{z}", "dV^{p}_{z} [cm]",
                          directories.Vertex_dir_map.at("Vertex_dV_pFDpCD"), "03_dVz", -dV_lim, dV_lim, numTH1Dbins);
    hdVx_pFD_pFDpCD = hPlot1D("pFDpCD", "", "dV^{pFD}_{x} of FD proton", "Vertex corr. dV^{pFD}_{x}=V^{pFD}_{x}-V^{e}_{x} of FD proton", "dV^{pFD}_{x} [cm]",
                              directories.Vertex_dir_map.at("Vertex_dV_pFDpCD"), "04_dVx_pFD", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hdVy_pFD_pFDpCD = hPlot1D("pFDpCD", "", "dV^{pFD}_{y} of FD proton", "Vertex corr. dV^{pFD}_{y}=V^{pFD}_{y}-V^{e}_{y} of FD proton", "dV^{pFD}_{y} [cm]",
                              directories.Vertex_dir_map.at("Vertex_dV_pFDpCD"), "05_dVy_pFD", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hdVz_pFD_pFDpCD = hPlot1D("pFDpCD", "", "dV^{pFD}_{z} of FD proton", "Vertex corr. dV^{pFD}_{z}=V^{pFD}_{z}-V^{e}_{z} of FD proton", "dV^{pFD}_{z} [cm]",
                              directories.Vertex_dir_map.at("Vertex_dV_pFDpCD"), "06_dVz_pFD", Vz_FD_llim, Vz_FD_ulim, numTH1Dbins);
    hdVx_pCD_pFDpCD = hPlot1D("pFDpCD", "", "dV^{pCD}_{x} of CD proton", "Vertex corr. dV^{pCD}_{x}=V^{pCD}_{x}-V^{e}_{x} of CD proton", "dV^{pCD}_{x} [cm]",
                              directories.Vertex_dir_map.at("Vertex_dV_pFDpCD"), "07_dVx_pCD", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hdVy_pCD_pFDpCD = hPlot1D("pFDpCD", "", "dV^{pCD}_{y} of CD proton", "Vertex corr. dV^{pCD}_{y}=V^{pCD}_{y}-V^{e}_{y} of CD proton", "dV^{pCD}_{y} [cm]",
                              directories.Vertex_dir_map.at("Vertex_dV_pFDpCD"), "08_dVy_pCD", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hdVz_pCD_pFDpCD = hPlot1D("pFDpCD", "", "dV^{pCD}_{z} of CD proton", "Vertex corr. dV^{pCD}_{z}=V^{pCD}_{z}-V^{e}_{z} of CD proton", "dV^{pCD}_{z} [cm]",
                              directories.Vertex_dir_map.at("Vertex_dV_pFDpCD"), "09_dVz_pCD", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);

    // dV plots (nFDpCD)
    hdVx_nFDpCD = hPlot1D("nFDpCD", "", "Vertex corr. dV_{x}", "Vertex correlation dV_{x}=V^{p}_{x}-V^{e}_{x}", "dV_{x} [cm]", directories.Vertex_dir_map.at("Vertex_dV_nFDpCD"),
                          "01_dVx", -dV_lim, dV_lim, numTH1Dbins);
    hdVy_nFDpCD = hPlot1D("nFDpCD", "", "Vertex corr. dV_{y}", "Vertex correlation dV_{y}=V^{p}_{y}-V^{e}_{y}", "dV_{y} [cm]", directories.Vertex_dir_map.at("Vertex_dV_nFDpCD"),
                          "02_dVy", -dV_lim, dV_lim, numTH1Dbins);
    hdVz_nFDpCD = hPlot1D("nFDpCD", "", "Vertex corr. dV_{z}", "Vertex correlation dV_{z}=V^{p}_{z}-V^{e}_{z}", "dV_{z} [cm]", directories.Vertex_dir_map.at("Vertex_dV_nFDpCD"),
                          "03_dVz", -dV_lim, dV_lim, numTH1Dbins);
    hdVx_pCD_nFDpCD = hPlot1D("nFDpCD", "", "dV^{pCD}_{x} of CD proton", "Vertex corr. dV^{pCD}_{x}=V^{pCD}_{x}-V^{e}_{x} of CD proton", "dV^{pCD}_{x} [cm]",
                              directories.Vertex_dir_map.at("Vertex_dV_nFDpCD"), "07_dVx_pCD", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hdVy_pCD_nFDpCD = hPlot1D("nFDpCD", "", "dV^{pCD}_{y} of CD proton", "Vertex corr. dV^{pCD}_{y}=V^{pCD}_{y}-V^{e}_{y} of CD proton", "dV^{pCD}_{y} [cm]",
                              directories.Vertex_dir_map.at("Vertex_dV_nFDpCD"), "08_dVy_pCD", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);
    hdVz_pCD_nFDpCD = hPlot1D("nFDpCD", "", "dV^{pCD}_{z} of CD proton", "Vertex corr. dV^{pCD}_{z}=V^{pCD}_{z}-V^{e}_{z} of CD proton", "dV^{pCD}_{z} [cm]",
                              directories.Vertex_dir_map.at("Vertex_dV_nFDpCD"), "09_dVz_pCD", Vz_CD_llim, Vz_CD_ulim, numTH1Dbins);

#pragma endregion
}

void RecoAnalyzerHistograms::InitializeSamplingFractionHistograms(const AnalysisCutSettings& cut_settings, const Directories& directories, const HistogramSettings& settings,
                                                                  const double beamE) {
    const double SF_llim = settings.SF_llim;
    const double SF_ulim = settings.SF_ulim;
    const int numTH1Dbins = settings.numTH1Dbins;
    const int numTH2Dbins = settings.numTH2Dbins;
    if (!cut_settings.apply_cuts) {
        hSF_1e_cut_BC_FD = hPlot1D("1e cut", "FD", "SF BC", "Sampling fraction f_{e} - before cuts", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                   directories.SF_dir_map.at("SF_1e_cut"), "01_SF_1e_cut_before_SF_cuts", SF_llim, SF_ulim, numTH1Dbins);
        hSF_1e_cut_AC_FD = hPlot1D("1e cut", "FD", "SF AC", "Sampling fraction f_{e} - after cuts", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                   directories.SF_dir_map.at("SF_1e_cut"), "02_SF_1e_cut_after_SF_cuts", SF_llim, SF_ulim, numTH1Dbins);
    } else {
        hSF_1e_cut_BC_FD = hPlot1D("1e cut", "FD", "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_dir_map.at("SF_1e_cut"),
                                   "01_SF_1e_cut", SF_llim, SF_ulim, numTH1Dbins);
    }

    if (!cut_settings.apply_cuts) {
        hSF_VS_P_e_1e_cut_BC_FD =
            hPlot2D("1e cut", "FD", "SF vs. P_{e} BC", "Sampling fraction f_{e} vs. P_{e} - before cuts", "P_{e} [GeV/c]", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                    directories.SF_dir_map.at("SF_VS_P_e_1e_cut"), "01_SF_VS_P_e_1e_cut_BC_FD", 0, beamE * 1.1, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
        hSF_VS_P_e_1e_cut_AC_FD =
            hPlot2D("1e cut", "FD", "SF vs. P_{e} AC", "Sampling fraction f_{e} vs. P_{e} - after cuts", "P_{e} [GeV/c]", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                    directories.SF_dir_map.at("SF_VS_P_e_1e_cut_AC"), "01_SF_VS_P_e_1e_cut_AC_FD", 0, beamE * 1.1, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
    } else {
        hSF_VS_P_e_1e_cut_BC_FD =
            hPlot2D("1e cut", "FD", "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                    directories.SF_dir_map.at("SF_VS_P_e_1e_cut"), "01_SF_VS_P_e_1e_cut_FD", 0, beamE * 1.1, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
    }

    hSF_1p_FD = hPlot1D("1p", "FD", "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_dir_map.at("SF_1p"), "01_SF_1p", SF_llim,
                        SF_ulim, numTH1Dbins);
    hSF_VS_P_e_1p_FD = hPlot2D("1p", "FD", "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                               directories.SF_dir_map.at("SF_VS_P_e_1p"), "01_SF_VS_P_e_1p_FD", 0, beamE * 1.1, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);

    hSF_1n_FD = hPlot1D("1n", "FD", "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_dir_map.at("SF_1n"), "01_SF_1n", SF_llim,
                        SF_ulim, numTH1Dbins);
    hSF_VS_P_e_1n_FD = hPlot2D("1n", "FD", "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                               directories.SF_dir_map.at("SF_VS_P_e_1n"), "01_SF_VS_P_e_1n_FD", 0, beamE * 1.1, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);

    hSF_2p_FD = hPlot1D("2p", "FD", "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_dir_map.at("SF_2p"), "01_SF_2p", SF_llim,
                        SF_ulim, numTH1Dbins);
    hSF_VS_P_e_2p_FD = hPlot2D("2p", "FD", "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                               directories.SF_dir_map.at("SF_VS_P_e_2p"), "01_SF_VS_P_e_2p_FD", 0, beamE * 1.1, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);

    hSF_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_dir_map.at("SF_pFDpCD"), "01_SF_pFDpCD",
                            SF_llim, SF_ulim, numTH1Dbins);
    hSF_VS_P_e_pFDpCD_FD = hPlot2D("pFDpCD", "FD", "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                   directories.SF_dir_map.at("SF_VS_P_e_pFDpCD"), "01_SF_VS_P_e_pFDpCD_FD", 0, beamE * 1.1, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);

    hSF_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_dir_map.at("SF_nFDpCD"), "01_SF_nFDpCD",
                            SF_llim, SF_ulim, numTH1Dbins);
    hSF_VS_P_e_nFDpCD_FD = hPlot2D("nFDpCD", "FD", "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                   directories.SF_dir_map.at("SF_VS_P_e_nFDpCD"), "01_SF_VS_P_e_nFDpCD_FD", 0, beamE * 1.1, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
}

void RecoAnalyzerHistograms::InitializeEcalEdgeHistograms(const AnalysisCutSettings& cut_settings, const Directories& directories, const HistogramSettings& settings) {
    const double SF_llim = settings.SF_llim;
    const double SF_ulim = settings.SF_ulim;
    const int numTH2Dbins = settings.numTH2Dbins;
    if (!cut_settings.apply_cuts) {
        hVcal_VS_EoP_1e_cut_BC_PCAL =
            hPlot2D("1e cut", "PCAL", "Vcal vs. SF BC", "ECAL V coordinate vs. SF - before cuts", "ECAL V coordinate [cm]", "Sampling Fraction (SF)",
                    directories.ECAL_fiducial_dir_map.at("Edge_1e_BC_PCAL"), "01_Vcal_VS_EoP_PCAL_1e_cut_BC", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
        hVcal_VS_EoP_1e_cut_AC_PCAL =
            hPlot2D("1e cut", "PCAL", "Vcal vs. SF AC", "ECAL V coordinate vs. SF - after cuts", "ECAL V coordinate [cm]", "Sampling Fraction (SF)",
                    directories.ECAL_fiducial_dir_map.at("Edge_1e_BC_PCAL"), "01_Vcal_VS_EoP_PCAL_1e_cut_AC", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);

        hWcal_VS_EoP_1e_cut_BC_PCAL =
            hPlot2D("1e cut", "PCAL", "Wcal vs. SF BC", "ECAL W coordinate vs. SF - before cuts", "ECAL W coordinate [cm]", "Sampling Fraction (SF)",
                    directories.ECAL_fiducial_dir_map.at("Edge_1e_BC_PCAL"), "02_Wcal_VS_EoP_PCAL_1e_cut_BC", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
        hWcal_VS_EoP_1e_cut_AC_PCAL =
            hPlot2D("1e cut", "PCAL", "Wcal vs. SF AC", "ECAL W coordinate vs. SF - after cuts", "ECAL W coordinate [cm]", "Sampling Fraction (SF)",
                    directories.ECAL_fiducial_dir_map.at("Edge_1e_BC_PCAL"), "02_Wcal_VS_EoP_PCAL_1e_cut_AC", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);

        hUcal_VS_EoP_1e_cut_BC_PCAL = hPlot2D("1e cut", "PCAL", "Ucal vs. SF BC", "ECAL U coordinate vs. SF - before cuts", "ECAL U coordinate [cm]", "Sampling Fraction (SF)",
                                              directories.ECAL_fiducial_dir_map.at("Edge_1e_BC_PCAL"), "03_Ucal_VS_EoP_PCAL_1e_cut_BC", 0, 50, SF_llim, SF_ulim, 100, 100);
        hUcal_VS_EoP_1e_cut_AC_PCAL = hPlot2D("1e cut", "PCAL", "Ucal vs. SF AC", "ECAL U coordinate vs. SF - after cuts", "ECAL U coordinate [cm]", "Sampling Fraction (SF)",
                                              directories.ECAL_fiducial_dir_map.at("Edge_1e_BC_PCAL"), "03_Ucal_VS_EoP_PCAL_1e_cut_AC", 0, 50, SF_llim, SF_ulim, 100, 100);
    } else {
        hVcal_VS_EoP_1e_cut_BC_PCAL =
            hPlot2D("1e cut", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]", "Sampling Fraction (SF)",
                    directories.ECAL_fiducial_dir_map.at("Edge_1e_BC_PCAL"), "01_Vcal_VS_EoP_PCAL_1e_cut", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
        hWcal_VS_EoP_1e_cut_BC_PCAL =
            hPlot2D("1e cut", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]", "Sampling Fraction (SF)",
                    directories.ECAL_fiducial_dir_map.at("Edge_1e_BC_PCAL"), "02_Wcal_VS_EoP_PCAL_1e_cut", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
        hUcal_VS_EoP_1e_cut_BC_PCAL = hPlot2D("1e cut", "PCAL", "Ucal vs. SF", "ECAL U coordinate vs. SF", "ECAL U coordinate [cm]", "Sampling Fraction (SF)",
                                              directories.ECAL_fiducial_dir_map.at("Edge_1e_BC_PCAL"), "03_Ucal_VS_EoP_PCAL_1e_cut", 0, 50, SF_llim, SF_ulim, 100, 100);
    }

    hPCAL_acceptance_1e_cut = hPlot2D("1e cut", "PCAL", "Electron PCAL acceptance", "Electron PCAL acceptance", "x [cm]", "y [cm]",
                                      directories.ECAL_fiducial_dir_map.at("Edge_1e_BC_PCAL"), "04_Electron_PCAL_acceptance_1e_cut", -400, 400, -400, 400, 100, 100);

    hVcal_VS_EoP_1p_PCAL = hPlot2D("1p", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]", "Sampling Fraction (SF)",
                                   directories.ECAL_fiducial_dir_map.at("Edge_1p_PCAL"), "01_Vcal_VS_EoP_PCAL_1p", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
    hWcal_VS_EoP_1p_PCAL = hPlot2D("1p", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]", "Sampling Fraction (SF)",
                                   directories.ECAL_fiducial_dir_map.at("Edge_1p_PCAL"), "02_Wcal_VS_EoP_PCAL_1p", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);

    hVcal_VS_EoP_1n_PCAL = hPlot2D("1n", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]", "Sampling Fraction (SF)",
                                   directories.ECAL_fiducial_dir_map.at("Edge_1n_PCAL"), "01_Vcal_VS_EoP_PCAL_1n", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
    hWcal_VS_EoP_1n_PCAL = hPlot2D("1n", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]", "Sampling Fraction (SF)",
                                   directories.ECAL_fiducial_dir_map.at("Edge_1n_PCAL"), "02_Wcal_VS_EoP_PCAL_1n", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);

    hVcal_VS_EoP_2p_PCAL = hPlot2D("2p", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]", "Sampling Fraction (SF)",
                                   directories.ECAL_fiducial_dir_map.at("Edge_2p_PCAL"), "01_Vcal_VS_EoP_PCAL_2p", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
    hWcal_VS_EoP_2p_PCAL = hPlot2D("2p", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]", "Sampling Fraction (SF)",
                                   directories.ECAL_fiducial_dir_map.at("Edge_2p_PCAL"), "02_Wcal_VS_EoP_PCAL_2p", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);

    hVcal_VS_EoP_pFDpCD_PCAL =
        hPlot2D("pFDpCD", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]", "Sampling Fraction (SF)",
                directories.ECAL_fiducial_dir_map.at("Edge_pFDpCD_PCAL"), "01_Vcal_VS_EoP_PCAL_pFDpCD", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
    hWcal_VS_EoP_pFDpCD_PCAL =
        hPlot2D("pFDpCD", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]", "Sampling Fraction (SF)",
                directories.ECAL_fiducial_dir_map.at("Edge_pFDpCD_PCAL"), "02_Wcal_VS_EoP_PCAL_pFDpCD", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);

    hVcal_VS_EoP_nFDpCD_PCAL =
        hPlot2D("nFDpCD", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]", "Sampling Fraction (SF)",
                directories.ECAL_fiducial_dir_map.at("Edge_nFDpCD_PCAL"), "01_Vcal_VS_EoP_PCAL_nFDpCD", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
    hWcal_VS_EoP_nFDpCD_PCAL =
        hPlot2D("nFDpCD", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]", "Sampling Fraction (SF)",
                directories.ECAL_fiducial_dir_map.at("Edge_nFDpCD_PCAL"), "02_Wcal_VS_EoP_PCAL_nFDpCD", 0, 50, SF_llim, SF_ulim, numTH2Dbins, numTH2Dbins);
}

void RecoAnalyzerHistograms::InitializeMomentumThresholdHistograms(const Directories& directories, const HistogramSettings& settings, const double beamE) {
    const int numTH1Dbins = settings.numTH1Dbins;
    const int numTH2Dbins = settings.numTH2Dbins;
    const int numTH1Dbins_Mom_eff_Plots = settings.numTH1Dbins_Mom_eff_Plots;
    const int numTH2Dbins_Mom_Plots = settings.numTH2Dbins_Mom_Plots;
    const int numTH2Dbins_Momentum_transfer_plots = settings.numTH2Dbins_Momentum_transfer_plots;
    const int numTH2Dbins_ReacMon_plots = settings.numTH2Dbins_ReacMon_plots;
    const double Momentum_llim = settings.Momentum_llim;
    const double Momentum_ulim = settings.Momentum_ulim;
    const double FDMomentum_llim = settings.FDMomentum_llim;
    const double FDMomentum_ulim = settings.FDMomentum_ulim;
    const double CDMomentum_llim = settings.CDMomentum_llim;
    const double CDMomentum_ulim = settings.CDMomentum_ulim;
    const double P_nucFD_llim = settings.P_nucFD_llim;
    const double P_nucFD_ulim = settings.P_nucFD_ulim;
    const double P_nucCD_llim = settings.P_nucCD_llim;
    const double P_nucCD_ulim = settings.P_nucCD_ulim;
    const double Theta_llim_FD = settings.Theta_llim_FD;
    const double Theta_ulim_FD = settings.Theta_ulim_FD;
    const double Theta_llim_CD = settings.Theta_llim_CD;
    const double Theta_ulim_CD = settings.Theta_ulim_CD;
    const double Phi_llim = settings.Phi_llim;
    const double Phi_ulim = settings.Phi_ulim;
    const double W_llim = settings.W_llim;
    const double W_ulim = settings.W_ulim;
#pragma region /* Momentum threshold histograms */
    /* Here are the definitions for the Momentum threshold plots. These are used to check that momentum cuts were applied in 1e_cut and 2p plots. */

#pragma region /* Momentum threshold plots (1e cut) */

    // Momentum threshold plots (1e cut)
    hP_e_1e_cut_FD = hPlot1D("1e cut", "", "Electron momentum", "Electron momentum P_{e}", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"), "01_P_e_1e_cut_FD", 0,
                             beamE * 1.1, numTH1Dbins);

    hP_p_1e_cut_CD = hPlot1D("1e cut", "CD", "Proton momentum", "Proton momentum P_{p}", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"), "02_P_p_1e_cut_CD", 0,
                             beamE * 1.1, numTH1Dbins);
    hP_p_1e_cut_FD = hPlot1D("1e cut", "FD", "Proton momentum", "Proton momentum P_{p}", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"), "02_P_p_1e_cut_FD", 0,
                             beamE * 1.1, numTH1Dbins);

    hP_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "#pi^{+} momentum", "#pi^{+} momentum P_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                                  "03_P_piplus_1e_cut_CD", 0, beamE * 1.1, numTH1Dbins);
    hP_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "#pi^{+} momentum", "#pi^{+} momentum P_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                                  "03_P_piplus_1e_cut_FD", 0, beamE * 1.1, numTH1Dbins);

    hP_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "#pi^{-} momentum", "#pi^{-} momentum P_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                                   "04_P_piminus_1e_cut_CD", 0, beamE * 1.1, numTH1Dbins);
    hP_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "#pi^{-} momentum", "#pi^{-} momentum P_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                                   "04_P_piminus_1e_cut_FD", 0, beamE * 1.1, numTH1Dbins);

    hP_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "K^{+} momentum", "K^{+} momentum P_{K^{+}}", "P_{K^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                                 "05_P_Kplus_1e_cut_CD", 0, beamE * 1.1, numTH1Dbins);
    hP_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "K^{+} momentum", "K^{+} momentum P_{K^{+}}", "P_{K^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                                 "05_P_Kplus_1e_cut_FD", 0, beamE * 1.1, numTH1Dbins);

    hP_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "K^{-} momentum", "K^{-} momentum P_{K^{-}}", "P_{K^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                                  "06_P_Kminus_1e_cut_CD", 0, beamE * 1.1, numTH1Dbins);
    hP_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "K^{-} momentum", "K^{-} momentum P_{K^{-}}", "P_{K^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                                  "06_P_Kminus_1e_cut_FD", 0, beamE * 1.1, numTH1Dbins);

    hP_deuteron_1e_cut_CD = hPlot1D("1e cut", "CD", "Deuterons momentum", "Deuterons momentum P_{D}", "P_{D} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                                    "07_P_deuteron_1e_cut_CD", 0, beamE * 1.1, numTH1Dbins);
    hP_deuteron_1e_cut_FD = hPlot1D("1e cut", "FD", "Deuterons momentum", "Deuterons momentum P_{D}", "P_{D} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                                    "07_P_deuteron_1e_cut_FD", 0, beamE * 1.1, numTH1Dbins);

    hP_LnFD_APID_1e_cut_FD =
        hPlot1D("1e_cut", "FD", "Leading FD neutron momentum APID", "Leading FD neutron momentum P_{n} APID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                "08a_P_LnFD_APID_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);  // Leading nFD
    hP_LnFD_APID_1e_cut_ZOOMOUT_FD =
        hPlot1D("1e_cut", "FD", "Leading FD neutron momentum APID - ZOOMOUT", "Leading FD neutron momentum P_{n} APID - ZOOMOUT", "P_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_1e_cut"), "08b_P_LnFD_APID_1e_cut_ZOOMOUT_FD", Momentum_llim, 15., numTH1Dbins);  // Leading nFD
    hP_LnFD_BPID_1e_cut_FD =
        hPlot1D("1e_cut", "FD", "Leading FD neutron momentum BPID", "Leading FD neutron momentum P_{n} BPID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                "08c_P_LnFD_BPID_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);  // Leading nFD
    hP_LnFD_BPID_1e_cut_ZOOMOUT_FD =
        hPlot1D("1e_cut", "FD", "Leading FD neutron momentum BPID - ZOOMOUT", "Leading FD neutron momentum P_{n} BPID - ZOOMOUT", "P_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_1e_cut"), "08d_P_LnFD_BPID_1e_cut_ZOOMOUT_FD", Momentum_llim, 15., numTH1Dbins);  // Leading nFD
    hP_LnFD_APIDandNS_1e_cut_FD =
        hPlot1D("1e_cut", "FD", "Leading FD neutron momentum APID&NC", "Leading FD neutron momentum P_{n} APID&NC", "P_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_1e_cut"), "08e_P_LnFD_APIDandNS_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);  // Leading nFD
    hP_LnFD_APIDandNS_1e_cut_ZOOMOUT_FD =
        hPlot1D("1e_cut", "FD", "Leading FD neutron momentum APID&NC - ZOOMOUT", "Leading FD neutron momentum P_{n} APID&NC - ZOOMOUT", "P_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_1e_cut"), "08f_P_LnFD_APIDandNS_1e_cut_ZOOMOUT_FD", Momentum_llim, 15., numTH1Dbins);  // Leading nFD

    hP_nFD_APID_1e_cut_FD = hPlot1D("1e_cut", "FD", "FD neutron momentum APID", "FD neutron momentum P_{n} APID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                                    "09a_P_nFD_APID_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);  // All nFD
    hP_nFD_APID_1e_cut_ZOOMOUT_FD = hPlot1D("1e_cut", "FD", "FD neutron momentum APID - ZOOMOUT", "FD neutron momentum P_{n} APID - ZOOMOUT", "P_{n} [GeV/c]",
                                            directories.Momentum_dir_map.at("Momentum_1e_cut"), "09b_P_nFD_APID_1e_cut_ZOOMOUT_FD", Momentum_llim, 15., numTH1Dbins);  // All nFD
    hP_nFD_BPID_1e_cut_FD = hPlot1D("1e_cut", "FD", "FD neutron momentum BPID", "FD neutron momentum P_{n} BPID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                                    "09c_P_nFD_BPID_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);  // All nFD
    hP_nFD_BPID_1e_cut_ZOOMOUT_FD = hPlot1D("1e_cut", "FD", "FD neutron momentum BPID - ZOOMOUT", "FD neutron momentum P_{n} BPID - ZOOMOUT", "P_{n} [GeV/c]",
                                            directories.Momentum_dir_map.at("Momentum_1e_cut"), "09d_P_nFD_BPID_1e_cut_ZOOMOUT_FD", Momentum_llim, 15., numTH1Dbins);  // All nFD
    hP_nFD_APIDandNS_1e_cut_FD =
        hPlot1D("1e_cut", "FD", "FD neutron momentum APID&NC", "FD neutron momentum P_{n} APID&NC", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1e_cut"),
                "09e_P_nFD_APIDandNS_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);  // All nFD
    hP_nFD_APIDandNS_1e_cut_ZOOMOUT_FD =
        hPlot1D("1e_cut", "FD", "FD neutron momentum APID&NC - ZOOMOUT", "FD neutron momentum P_{n} APID&NC - ZOOMOUT", "P_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_1e_cut"), "09f_P_nFD_APIDandNS_1e_cut_ZOOMOUT_FD", Momentum_llim, 15., numTH1Dbins);  // All nFD

    // Reco momentum plots for momentum thresholds (1e cut)
    hP_e_reco_1e_cut_FD =
        hPlot1D("1e cut", "FD", "Reco FD Electron momentum", "FD Electron momentum P^{reco}_{e}", "P^{reco}_{e} [GeV/c]", directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"),
                "01a_P_e_reco_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_e_reco_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD Electron momentum - ZOOMIN", "FD Electron momentum P^{reco}_{e} - ZOOMIN", "P^{reco}_{e} [GeV/c]",
                                         directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "01b_P_e_reco_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_e_vs_Theta_e_reco_1e_cut_FD =
        hPlot2D("1e cut", "FD", "P_{e} vs. #theta_{e}", "P_{e} vs. #theta_{e}", "P_{e} [GeV/c]", "#theta_{e} [#circ]", directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"),
                "01c_P_e_vs_Theta_e_reco_1e_cut_FD", Momentum_llim, Momentum_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    hP_p_reco_1e_cut_FD =
        hPlot1D("1e cut", "FD", "Reco FD Proton momentum", "FD Proton momentum P^{reco}_{p}", "P^{reco}_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"),
                "02a_P_p_reco_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_p_reco_1e_cut_CD =
        hPlot1D("1e cut", "CD", "Reco CD Proton momentum", "CD Proton momentum P^{reco}_{p}", "P^{reco}_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"),
                "02b_P_p_reco_1e_cut_CD", CDMomentum_llim, CDMomentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_p_reco_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD Proton momentum - ZOOMIN", "FD Proton momentum P^{reco}_{p} - ZOOMIN", "P^{reco}_{p} [GeV/c]",
                                         directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "02aa_P_p_reco_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_p_reco_1e_cut_CD_ZOOMIN = hPlot1D("1e cut", "CD", "Reco CD Proton momentum - ZOOMIN", "CD Proton momentum P^{reco}_{p} - ZOOMIN", "P^{reco}_{p} [GeV/c]",
                                         directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "02bb_P_p_reco_1e_cut_CD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_p_vs_Theta_p_reco_1e_cut_FD =
        hPlot2D("1e cut", "FD", "P_{p} vs. #theta_{p}", "P_{p} vs. #theta_{p}", "P_{p} [GeV/c]", "#theta_{p} [#circ]", directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"),
                "02c_P_p_vs_Theta_p_reco_1e_cut_FD", Momentum_llim, Momentum_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);
    hP_p_vs_Theta_p_reco_1e_cut_CD =
        hPlot2D("1e cut", "CD", "P_{p} vs. #theta_{p}", "P_{p} vs. #theta_{p}", "P_{p} [GeV/c]", "#theta_{p} [#circ]", directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"),
                "02d_P_p_vs_Theta_p_reco_1e_cut_CD", CDMomentum_llim, CDMomentum_ulim, Theta_llim_CD, Theta_ulim_CD, numTH2Dbins * 2, numTH2Dbins * 2);

    // leading nFD (APID):
    hP_LnFD_reco_APID_1e_cut_FD =
        hPlot1D("1e cut", "FD", "Reco leading FD neutron momentum APID", "Leading FD neutron momentum P^{reco}_{n} APID", "P^{reco}_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03a_P_LnFD_reco_APID_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_LnFD_reco_APID_1e_cut_FD_ZOOMIN =
        hPlot1D("1e cut", "FD", "Reco leading FD neutron momentum APID - ZOOMIN", "Leading FD neutron momentum P^{reco}_{n} APID - ZOOMIN", "P^{reco}_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03b_P_LnFD_reco_APID_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_LnFD_reco_APID_1e_cut_FD_ZOOMOUT =
        hPlot1D("1e cut", "FD", "Reco leading FD neutron momentum APID - ZOOMOUT", "Leading FD neutron momentum P^{reco}_{n} APID - ZOOMOUT", "P^{reco}_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03c_P_LnFD_reco_APID_1e_cut_FD_ZOOMOUT", 0, 15., numTH1Dbins_Mom_eff_Plots);
    hP_LnFD_vs_Theta_LnFD_reco_APID_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{LnFD} vs. #theta_{LnFD} APID", "P_{LnFD} vs. #theta_{LnFD} APID", "P_{LnFD} [GeV/c]", "#theta_{LnFD} [#circ]",
                                                        directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03d_P_LnFD_vs_Theta_LnFD_reco_APID_1e_cut_FD", Momentum_llim,
                                                        Momentum_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    // leading nFD (BPID):
    hP_LnFD_reco_BPID_1e_cut_FD =
        hPlot1D("1e cut", "FD", "Reco leading FD neutron momentum BPID", "Leading FD neutron momentum P^{reco}_{n} BPID", "P^{reco}_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03a_P_LnFD_reco_BPID_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_LnFD_reco_BPID_1e_cut_FD_ZOOMIN =
        hPlot1D("1e cut", "FD", "Reco leading FD neutron momentum BPID - ZOOMIN", "Leading FD neutron momentum P^{reco}_{n} BPID - ZOOMIN", "P^{reco}_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03b_P_LnFD_reco_BPID_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_LnFD_reco_BPID_1e_cut_FD_ZOOMOUT =
        hPlot1D("1e cut", "FD", "Reco leading FD neutron momentum BPID - ZOOMOUT", "Leading FD neutron momentum P^{reco}_{n} BPID - ZOOMOUT", "P^{reco}_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03c_P_LnFD_reco_BPID_1e_cut_FD_ZOOMOUT", 0, 15., numTH1Dbins_Mom_eff_Plots);
    hP_LnFD_vs_Theta_LnFD_reco_BPID_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{LnFD} vs. #theta_{LnFD} BPID", "P_{LnFD} vs. #theta_{LnFD} BPID", "P_{LnFD} [GeV/c]", "#theta_{LnFD} [#circ]",
                                                        directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03d_P_LnFD_vs_Theta_LnFD_reco_BPID_1e_cut_FD", Momentum_llim,
                                                        Momentum_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    // all nFD (APID):
    hP_nFD_reco_APID_1e_cut_FD =
        hPlot1D("1e cut", "FD", "Reco FD neutrons momentum APID", "FD neutrons momentum P^{reco}_{n} APID", "P^{reco}_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03a_P_nFD_reco_APID_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_nFD_reco_APID_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD neutrons momentum APID - ZOOMIN", "FD neutrons momentum P^{reco}_{n} APID - ZOOMIN", "P^{reco}_{n} [GeV/c]",
                                                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03b_P_nFD_reco_APID_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_nFD_reco_APID_1e_cut_FD_ZOOMOUT =
        hPlot1D("1e cut", "FD", "Reco FD neutron momentum APID - ZOOMOUT", "FD neutrons momentum P^{reco}_{n} APID - ZOOMOUT", "P^{reco}_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03c_P_nFD_reco_APID_1e_cut_FD_ZOOMOUT", 0, 15., numTH1Dbins_Mom_eff_Plots);
    hP_nFD_vs_Theta_nFD_reco_APID_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{nFD} vs. #theta_{nFD} APID", "P_{nFD} vs. #theta_{nFD} APID", "P_{nFD} [GeV/c]", "#theta_{nFD} [#circ]",
                                                      directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03d_P_nFD_vs_Theta_nFD_reco_APID_1e_cut_FD", Momentum_llim,
                                                      Momentum_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    // all nFD (BPID):
    hP_nFD_reco_BPID_1e_cut_FD =
        hPlot1D("1e cut", "FD", "Reco FD neutrons momentum BPID", "FD neutrons momentum P^{reco}_{n} BPID", "P^{reco}_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03a_P_nFD_reco_BPID_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_nFD_reco_BPID_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD neutrons momentum BPID - ZOOMIN", "FD neutrons momentum P^{reco}_{n} BPID - ZOOMIN", "P^{reco}_{n} [GeV/c]",
                                                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03b_P_nFD_reco_BPID_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_nFD_reco_BPID_1e_cut_FD_ZOOMOUT =
        hPlot1D("1e cut", "FD", "Reco FD neutron momentum BPID - ZOOMOUT", "FD neutrons momentum P^{reco}_{n} BPID - ZOOMOUT", "P^{reco}_{n} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03c_P_nFD_reco_BPID_1e_cut_FD_ZOOMOUT", 0, 15., numTH1Dbins_Mom_eff_Plots);
    hP_nFD_vs_Theta_nFD_reco_BPID_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{nFD} vs. #theta_{nFD} BPID", "P_{nFD} vs. #theta_{nFD} BPID", "P_{nFD} [GeV/c]", "#theta_{nFD} [#circ]",
                                                      directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "03d_P_nFD_vs_Theta_nFD_reco_BPID_1e_cut_FD", Momentum_llim,
                                                      Momentum_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    hP_piplus_reco_1e_cut =
        hPlot1D("1e cut", "CD & FD", "Reco #pi^{+} momentum", "#pi^{+} momentum P^{reco}_{#pi^{+}}", "P^{reco}_{#pi^{+}} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "04a_P_piplus_reco_1e_cut", Momentum_llim, Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_piplus_reco_1e_cut_ZOOMIN = hPlot1D("1e cut", "CD & FD", "Reco #pi^{+} momentum - ZOOMIN", "#pi^{+} momentum P^{reco}_{#pi^{+}} - ZOOMIN", "P^{reco}_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "04b_P_piplus_reco_1e_cut_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_piplus_reco_1e_cut_FD =
        hPlot1D("1e cut", "FD", "Reco FD #pi^{+} momentum", "FD #pi^{+} momentum P^{reco}_{#pi^{+}}", "P^{reco}_{#pi^{+}} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "04c_P_piplus_reco_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_piplus_reco_1e_cut_CD =
        hPlot1D("1e cut", "CD", "Reco CD #pi^{+} momentum", "CD #pi^{+} momentum P^{reco}_{#pi^{+}}", "P^{reco}_{#pi^{+}} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "04d_P_piplus_reco_1e_cut_CD", CDMomentum_llim, CDMomentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_piplus_reco_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD #pi^{+} momentum - ZOOMIN", "FD #pi^{+} momentum P^{reco}_{#pi^{+}} - ZOOMIN", "P^{reco}_{#pi^{+}} [GeV/c]",
                                              directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "04e_P_piplus_reco_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_piplus_reco_1e_cut_CD_ZOOMIN = hPlot1D("1e cut", "CD", "Reco CD #pi^{+} momentum - ZOOMIN", "CD #pi^{+} momentum P^{reco}_{#pi^{+}} - ZOOMIN", "P^{reco}_{#pi^{+}} [GeV/c]",
                                              directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "04f_P_piplus_reco_1e_cut_CD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_piplus_vs_Theta_piplus_reco_1e_cut_FD =
        hPlot2D("1e cut", "FD", "P_{#pi^{+}} vs. #theta_{#pi^{+}}", "P_{#pi^{+}} vs. #theta_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]", "#theta_{#pi^{+}} [#circ]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "04g_P_piplus_vs_Theta_piplus_reco_1e_cut_FD", Momentum_llim, Momentum_ulim, Theta_llim_FD,
                Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);
    hP_piplus_vs_Theta_piplus_reco_1e_cut_CD =
        hPlot2D("1e cut", "CD", "P_{#pi^{+}} vs. #theta_{#pi^{+}}", "P_{#pi^{+}} vs. #theta_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]", "#theta_{#pi^{+}} [#circ]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "04h_P_piplus_vs_Theta_piplus_reco_1e_cut_CD", CDMomentum_llim, CDMomentum_ulim,
                Theta_llim_CD, Theta_ulim_CD, numTH2Dbins * 2, numTH2Dbins * 2);

    hP_piminus_reco_1e_cut =
        hPlot1D("1e cut", "CD & FD", "Reco #pi^{-} momentum", "#pi^{-} momentum P^{reco}_{#pi^{-}}", "P^{reco}_{#pi^{-}} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "05a_P_piminus_reco_1e_cut", Momentum_llim, Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_piminus_reco_1e_cut_ZOOMIN = hPlot1D("1e cut", "CD & FD", "Reco #pi^{-} momentum - ZOOMIN", "#pi^{-} momentum P^{reco}_{#pi^{-}} - ZOOMIN", "P^{reco}_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "05b_P_piminus_reco_1e_cut_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_piminus_reco_1e_cut_FD =
        hPlot1D("1e cut", "FD", "Reco FD #pi^{-} momentum", "FD #pi^{-} momentum P^{reco}_{#pi^{-}}", "P^{reco}_{#pi^{-}} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "05c_P_piminus_reco_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_piminus_reco_1e_cut_CD =
        hPlot1D("1e cut", "CD", "Reco CD #pi^{-} momentum", "CD #pi^{-} momentum P^{reco}_{#pi^{-}}", "P^{reco}_{#pi^{-}} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "05d_P_piminus_reco_1e_cut_CD", CDMomentum_llim, CDMomentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_piminus_reco_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD #pi^{-} momentum - ZOOMIN", "FD #pi^{-} momentum P^{reco}_{#pi^{-}} - ZOOMIN", "P^{reco}_{#pi^{-}} [GeV/c]",
                                               directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "05e_P_piminus_reco_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_piminus_reco_1e_cut_CD_ZOOMIN = hPlot1D("1e cut", "CD", "Reco CD #pi^{-} momentum - ZOOMIN", "CD #pi^{-} momentum P^{reco}_{#pi^{-}} - ZOOMIN", "P^{reco}_{#pi^{-}} [GeV/c]",
                                               directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "05f_P_piminus_reco_1e_cut_CD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_piminus_vs_Theta_piminus_reco_1e_cut_FD =
        hPlot2D("1e cut", "FD", "P_{#pi^{-}} vs. #theta_{#pi^{-}}", "P_{#pi^{-}} vs. #theta_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]", "#theta_{#pi^{-}} [#circ]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "05g_P_piminus_vs_Theta_piminus_reco_1e_cut_FD", Momentum_llim, Momentum_ulim,
                Theta_llim_FD, Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);
    hP_piminus_vs_Theta_piminus_reco_1e_cut_CD =
        hPlot2D("1e cut", "CD", "P_{#pi^{-}} vs. #theta_{#pi^{-}}", "P_{#pi^{-}} vs. #theta_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]", "#theta_{#pi^{-}} [#circ]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "05h_P_piminus_vs_Theta_piminus_reco_1e_cut_CD", CDMomentum_llim, CDMomentum_ulim,
                Theta_llim_CD, Theta_ulim_CD, numTH2Dbins * 2, numTH2Dbins * 2);

    hP_ph_reco_APID_1e_cut_FD =
        hPlot1D("1e cut", "FD", "Reco FD #gamma momentum APID", "FD #gamma momentum P^{reco}_{#gamma} APID", "P^{reco}_{#gamma} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "06a_P_ph_reco_APID_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_ph_reco_APID_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD #gamma momentum APID - ZOOMIN", "FD #gamma momentum P^{reco}_{#gamma} APID - ZOOMIN", "P^{reco}_{#gamma} [GeV/c]",
                                               directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "06b_P_ph_reco_APID_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_ph_reco_BPID_1e_cut_FD =
        hPlot1D("1e cut", "FD", "Reco FD #gamma momentum BPID", "FD #gamma momentum P^{reco}_{#gamma} BPID", "P^{reco}_{#gamma} [GeV/c]",
                directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "06a_P_ph_reco_BPID_1e_cut_FD", Momentum_llim, Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_ph_reco_BPID_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD #gamma momentum BPID - ZOOMIN", "FD #gamma momentum P^{reco}_{#gamma} BPID - ZOOMIN", "P^{reco}_{#gamma} [GeV/c]",
                                               directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "06b_P_ph_reco_BPID_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_ph_vs_Theta_ph_reco_APID_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{#gamma} vs. #theta_{#gamma} APID", "P_{#gamma} vs. #theta_{#gamma} APID", "P_{#gamma} [GeV/c]",
                                                    "#theta_{#gamma} [#circ]", directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "06e_P_ph_vs_Theta_ph_reco_APID_1e_cut_FD",
                                                    Momentum_llim, Momentum_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);
    hP_ph_vs_Theta_ph_reco_BPID_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{#gamma} vs. #theta_{#gamma} BPID", "P_{#gamma} vs. #theta_{#gamma} BPID", "P_{#gamma} [GeV/c]",
                                                    "#theta_{#gamma} [#circ]", directories.Momentum_dir_map.at("Momentum_th_reco_1e_cut"), "06e_P_ph_vs_Theta_ph_reco_BPID_1e_cut_FD",
                                                    Momentum_llim, Momentum_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);

#pragma endregion

#pragma region /* Momentum threshold plots (1p) */

#pragma region /* Analysis plots (1p) */
    hP_pFD_APID_1p = hPlot1D("1p", "FD", "FD proton momentum APID", "FD proton momentum APID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_1p"),
                             "01a_P_pFD_APID_1p_FD", FDMomentum_llim, FDMomentum_ulim, numTH1Dbins);
    hP_pFD_APIDandPS_1p = hPlot1D("1p", "FD", "FD proton momentum APID&PS", "FD proton momentum APID&PS", "P_{p} [GeV/c]",
                                  directories.Momentum_dir_map.at("Analysis_plots_momentum_1p"), "01b_P_pFD_APIDandPS_1p_FD", FDMomentum_llim, FDMomentum_ulim, numTH1Dbins);

    hP_pFD_APIDandPS_VS_W_1p =
        new TH2D("FD proton momentum APID vs. W (All Int., 1p)", "FD proton momentum APID vs. W (All Int., 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{p} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    hP_pFD_APIDandPS_VS_W_QEL_1p =
        new TH2D("FD proton momentum APID vs. W (QEL only, 1p)", "FD proton momentum APID vs. W (QEL only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{p} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    hP_pFD_APIDandPS_VS_W_MEC_1p =
        new TH2D("FD proton momentum APID vs. W (MEC only, 1p)", "FD proton momentum APID vs. W (MEC only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{p} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    hP_pFD_APIDandPS_VS_W_RES_1p =
        new TH2D("FD proton momentum APID vs. W (RES only, 1p)", "FD proton momentum APID vs. W (RES only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{p} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    hP_pFD_APIDandPS_VS_W_DIS_1p =
        new TH2D("FD proton momentum APID vs. W (DIS only, 1p)", "FD proton momentum APID vs. W (DIS only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{p} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    hP_pFD_APIDandPS_VS_W_1p_Dir = directories.Momentum_dir_map.at("Analysis_plots_momentum_1p");

    hP_e_vs_Theta_e_reco_All_int_1p = new TH2D("P_e_vs_Theta_e_reco_All_int_1p", "P_{e} vs. #theta_{e} (All Int., 1p);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                               Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_QEL_1p = new TH2D("P_e_vs_Theta_e_reco_QEL_1p", "P_{e} vs. #theta_{e} (QEL only, 1p);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                           Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_MEC_1p = new TH2D("P_e_vs_Theta_e_reco_MEC_1p", "P_{e} vs. #theta_{e} (MEC only, 1p);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                           Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_RES_1p = new TH2D("P_e_vs_Theta_e_reco_RES_1p", "P_{e} vs. #theta_{e} (RES only, 1p);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                           Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_DIS_1p = new TH2D("P_e_vs_Theta_e_reco_DIS_1p", "P_{e} vs. #theta_{e} (DIS only, 1p);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                           Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_1p_Dir = directories.Momentum_dir_map.at("Analysis_plots_momentum_1p");

#pragma endregion

#pragma region /* Other momentum threshold plots (1p) */
    hP_e_APID_1p_FD = hPlot1D("1p", "", "Electron momentum APID", "Electron momentum P_{e} APID", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                              "01a_P_e_APID_1p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_e_BPID_1p_FD = hPlot1D("1p", "", "Electron momentum BPID", "Electron momentum P_{e} BPID", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                              "01b_P_e_BPID_1p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_p_APID_1p_CD = hPlot1D("1p", "CD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"), "02a_P_p_APID_1p_CD",
                              Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_APIDandPS_1p_CD = hPlot1D("1p", "CD", "Proton momentum APID&PS", "Proton momentum P_{p} APID&PS", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                                   "02b_P_p_APIDandPS_1p_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_BPID_1p_CD = hPlot1D("1p", "CD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"), "02c_P_p_BPID_1p_CD",
                              Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_APID_1p_FD = hPlot1D("1p", "FD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"), "02d_P_p_APID_1p_FD",
                              Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_APIDandPS_1p_FD = hPlot1D("1p", "FD", "Proton momentum APID&PS", "Proton momentum P_{p} APID&PS", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                                   "02e_P_p_APIDandPS_1p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_BPID_1p_FD = hPlot1D("1p", "FD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"), "02f_P_p_BPID_1p_FD",
                              Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_piplus_APID_1p_CD = hPlot1D("1p", "CD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                                   "03a_P_piplus_APID_1p_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_BPID_1p_CD = hPlot1D("1p", "CD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                                   "03b_P_piplus_BPID_1p_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_APID_1p_FD = hPlot1D("1p", "FD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                                   "03c_P_piplus_APID_1p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_BPID_1p_FD = hPlot1D("1p", "FD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                                   "03d_P_piplus_BPID_1p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_piminus_APID_1p_CD = hPlot1D("1p", "CD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                                    "04a_P_piminus_APID_1p_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_BPID_1p_CD = hPlot1D("1p", "CD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                                    "04b_P_piminus_BPID_1p_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_APID_1p_FD = hPlot1D("1p", "FD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                                    "04c_P_piminus_APID_1p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_BPID_1p_FD = hPlot1D("1p", "FD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                                    "04d_P_piminus_BPID_1p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_ph_APID_1p_FD = hPlot1D("1p", "FD", "Photon momentum APID", "Photon momentum P_{#gamma} APID", "P_{#gamma} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                               "05c_P_ph_APID_1p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_ph_BPID_1p_FD = hPlot1D("1p", "FD", "Photon momentum BPID", "Photon momentum P_{#gamma} BPID", "P_{#gamma} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                               "05d_P_ph_BPID_1p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_n_APID_1p_FD = hPlot1D("1p", "FD", "Neutron momentum APID", "Neutron momentum P_{n} APID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                              "06c_P_n_APID_1p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_n_BPID_1p_FD = hPlot1D("1p", "FD", "Neutron momentum BPID", "Neutron momentum P_{n} BPID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1p"),
                              "06d_P_n_BPID_1p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

#pragma endregion

#pragma endregion

#pragma region /* Momentum threshold plots (1n) */

#pragma region /* Analysis plots (1n) */
    hP_nFD_APID_1n = hPlot1D("1n", "FD", "Leading FD neutron momentum APID", "Leading FD neutron momentum APID", "P_{n} [GeV/c]",
                             directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"), "01a_P_nFD_APID_1n_FD", FDMomentum_llim, FDMomentum_ulim, numTH1Dbins);
    hP_nFD_APIDandNS_1n = hPlot1D("1n", "FD", "Leading FD neutron momentum APID&NC", "Leading FD neutron momentum APID&NC", "P_{n} [GeV/c]",
                                  directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"), "01b_P_nFD_APIDandNS_1n_FD", FDMomentum_llim, FDMomentum_ulim, numTH1Dbins);
    hP_nFD_APID_1n_ZOOMOUT = hPlot1D("1n", "FD", "Leading FD neutron momentum APID - ZOOMOUT", "Leading FD neutron momentum APID - ZOOMOUT", "P_{n} [GeV/c]",
                                     directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"), "01c_P_nFD_APID_1n_ZOOMOUT_FD", Momentum_llim, 15., numTH1Dbins);
    hP_nFD_APIDandNS_1n_ZOOMOUT = hPlot1D("1n", "FD", "Leading FD neutron momentum APID&NC - ZOOMOUT", "Leading FD neutron momentum APID&NC - ZOOMOUT", "P_{n} [GeV/c]",
                                          directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"), "01d_P_nFD_APIDandNS_1n_ZOOMOUT_FD", Momentum_llim, 15., numTH1Dbins);

    hP_nFD_vs_P_e_1n = hPlot2D("1n", "", "P_{nFD} vs. P_{e}", "P_{nFD} vs. P_{e}", "P_{nFD} [GeV/c]", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"),
                               "05aa_P_nFD_vs_P_e", P_nucFD_llim, P_nucFD_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hP_nFD_vs_P_e_ZoomOut_1n = hPlot2D("1n", "", "P_{nFD} vs. P_{e} - ZoomOut", "P_{nFD} vs. P_{e} - ZoomOut", "P_{nFD} [GeV/c]", "P_{e} [GeV/c]",
                                       directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"), "05ab_P_nFD_vs_P_e_ZoomOut", P_nucFD_llim, beamE * 3., Momentum_llim,
                                       Momentum_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hP_nFD_vs_theta_e_1n =
        hPlot2D("1n", "", "P_{nFD} vs. #theta_{e}", "P_{nFD} vs. #theta_{e}", "P_{nFD} [GeV/c]", "#theta_{e} [#circ]", directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"),
                "05ba_P_nFD_vs_theta_e", P_nucFD_llim, P_nucFD_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hP_nFD_vs_theta_e_ZoomOut_1n = hPlot2D("1n", "", "P_{nFD} vs. #theta_{e} - ZoomOut", "P_{nFD} vs. #theta_{e} - ZoomOut", "P_{nFD} [GeV/c]", "#theta_{e} [#circ]",
                                           directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"), "05bb_P_nFD_vs_theta_e_ZoomOut", P_nucFD_llim, beamE * 3.,
                                           Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hP_nFD_vs_phi_e_1n =
        hPlot2D("1n", "", "P_{nFD} vs. #phi_{e}", "P_{nFD} vs. #phi_{e}", "P_{nFD} [GeV/c]", "#phi_{e} [#circ]", directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"),
                "05ca_P_nFD_vs_phi_e", P_nucFD_llim, P_nucFD_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hP_nFD_vs_phi_e_ZoomOut_1n = hPlot2D("1n", "", "P_{nFD} vs. #phi_{e} - ZoomOut", "P_{nFD} vs. #phi_{e} - ZoomOut", "P_{nFD} [GeV/c]", "#phi_{e} [#circ]",
                                         directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"), "05cb_P_nFD_vs_phi_e_ZoomOut", P_nucFD_llim, beamE * 3., Phi_llim,
                                         Phi_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    htheta_nFD_vs_P_e_1n =
        hPlot2D("1n", "", "#theta_{nFD} vs. P_{e}", "#theta_{nFD} vs. P_{e}", "#theta_{nFD} [#circ]", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"),
                "05d_theta_nFD_vs_P_e", Theta_llim_FD, Theta_ulim_FD, Momentum_llim, Momentum_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    htheta_nFD_vs_theta_e_1n = hPlot2D("1n", "", "#theta_{nFD} vs. #theta_{e}", "#theta_{nFD} vs. #theta_{e}", "#theta_{nFD} [#circ]", "#theta_{e} [#circ]",
                                       directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"), "05e_theta_nFD_vs_theta_e", Theta_llim_FD, Theta_ulim_FD,
                                       Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    htheta_nFD_vs_phi_e_1n = hPlot2D("1n", "", "#theta_{nFD} vs. #phi_{e}", "#theta_{nFD} vs. #phi_{e}", "#theta_{nFD} [#circ]", "#phi_{e} [#circ]",
                                     directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"), "05f_theta_nFD_vs_phi_e", Theta_llim_FD, Theta_ulim_FD, Phi_llim,
                                     Phi_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hphi_nFD_vs_P_e_1n =
        hPlot2D("1n", "", "#phi_{nFD} vs. P_{e}", "#phi_{nFD} vs. P_{e}", "#phi_{nFD} [#circ]", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"),
                "05g_phi_nFD_vs_P_e", Phi_llim, Phi_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hphi_nFD_vs_theta_e_1n = hPlot2D("1n", "", "#phi_{nFD} vs. #theta_{e}", "#phi_{nFD} vs. #theta_{e}", "#phi_{nFD} [#circ]", "#theta_{e} [#circ]",
                                     directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"), "05h_phi_nFD_vs_theta_e", Phi_llim, Phi_ulim, Theta_llim_FD,
                                     Theta_ulim_FD, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hphi_nFD_vs_phi_e_1n = hPlot2D("1n", "", "#phi_{nFD} vs. #phi_{e}", "#phi_{nFD} vs. #phi_{e}", "#phi_{nFD} [#circ]", "#phi_{e} [#circ]",
                                   directories.Momentum_dir_map.at("Analysis_plots_momentum_1n"), "05i_phi_nFD_vs_phi_e", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                   numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);

    hP_nFD_APIDandNS_VS_W_1n = new TH2D("Leading FD neutron momentum APID&NC vs. W (All Int., 1n)",
                                        "Leading FD neutron momentum APID&NC vs. W (All Int., 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{n} [GeV/c]",
                                        numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    hP_nFD_APIDandNS_VS_W_QEL_1n = new TH2D("Leading FD neutron momentum APID&NC vs. W (QEL only, 1n)",
                                            "Leading FD neutron momentum APID&NC vs. W (QEL only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{n} [GeV/c]",
                                            numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    hP_nFD_APIDandNS_VS_W_MEC_1n = new TH2D("Leading FD neutron momentum APID&NC vs. W (MEC only, 1n)",
                                            "Leading FD neutron momentum APID&NC vs. W (MEC only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{n} [GeV/c]",
                                            numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    hP_nFD_APIDandNS_VS_W_RES_1n = new TH2D("Leading FD neutron momentum APID&NC vs. W (RES only, 1n)",
                                            "Leading FD neutron momentum APID&NC vs. W (RES only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{n} [GeV/c]",
                                            numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    hP_nFD_APIDandNS_VS_W_DIS_1n = new TH2D("Leading FD neutron momentum APID&NC vs. W (DIS only, 1n)",
                                            "Leading FD neutron momentum APID&NC vs. W (DIS only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{n} [GeV/c]",
                                            numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    hP_nFD_APIDandNS_VS_W_1n_Dir = directories.Momentum_dir_map.at("Analysis_plots_momentum_1n");

    hP_e_vs_Theta_e_reco_All_int_1n = new TH2D("P_e_vs_Theta_e_reco_All_int_1n", "P_{e} vs. #theta_{e} (All Int., 1n);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                               Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_QEL_1n = new TH2D("P_e_vs_Theta_e_reco_QEL_1n", "P_{e} vs. #theta_{e} (QEL only, 1n);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                           Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_MEC_1n = new TH2D("P_e_vs_Theta_e_reco_MEC_1n", "P_{e} vs. #theta_{e} (MEC only, 1n);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                           Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_RES_1n = new TH2D("P_e_vs_Theta_e_reco_RES_1n", "P_{e} vs. #theta_{e} (RES only, 1n);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                           Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_DIS_1n = new TH2D("P_e_vs_Theta_e_reco_DIS_1n", "P_{e} vs. #theta_{e} (DIS only, 1n);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                           Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_1n_Dir = directories.Momentum_dir_map.at("Analysis_plots_momentum_1n");

#pragma endregion

#pragma region /* Other momentum threshold plots (1n) */

    hP_e_APID_1n_FD = hPlot1D("1n", "", "Electron momentum APID", "Electron momentum P_{e} APID", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                              "01a_P_e_APID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_e_BPID_1n_FD = hPlot1D("1n", "", "Electron momentum BPID", "Electron momentum P_{e} BPID", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                              "01b_P_e_BPID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_n_APID_1n_FD = hPlot1D("1n", "FD", "Neutron momentum APID", "Neutron momentum P_{n} APID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                              "02a_P_n_APID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_n_APID_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron momentum APID - ZOOMOUT", "Neutron momentum P_{n} APID - ZOOMOUT", "P_{n} [GeV/c]",
                                      directories.Momentum_dir_map.at("Momentum_1n"), "02b_P_n_APID_1n_ZOOMOUT_FD", Momentum_llim, 15., numTH1Dbins);
    hP_n_BPID_1n_FD = hPlot1D("1n", "FD", "Neutron momentum BPID", "Neutron momentum P_{n} BPID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                              "02c_P_n_BPID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_n_BPID_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron momentum BPID - ZOOMOUT", "Neutron momentum P_{n} BPID - ZOOMOUT", "P_{n} [GeV/c]",
                                      directories.Momentum_dir_map.at("Momentum_1n"), "02d_P_n_BPID_1n_ZOOMOUT_FD", Momentum_llim, 15., numTH1Dbins);
    hP_n_APIDandNS_1n_FD = hPlot1D("1n", "FD", "Neutron momentum APID&NC", "Neutron momentum P_{n} APID&NC", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                                   "02e_P_n_APIDandNS_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_n_APIDandNS_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron momentum APID&NC - ZOOMOUT", "Neutron momentum P_{n} APID&NC - ZOOMOUT", "P_{n} [GeV/c]",
                                           directories.Momentum_dir_map.at("Momentum_1n"), "02f_P_n_APIDandNS_1n_ZOOMOUT_FD", Momentum_llim, 15., numTH1Dbins);

    hP_p_APID_1n_CD = hPlot1D("1n", "CD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"), "03a_P_p_APID_1n_CD",
                              Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_BPID_1n_CD = hPlot1D("1n", "CD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"), "03b_P_p_BPID_1n_CD",
                              Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_APID_1n_FD = hPlot1D("1n", "FD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"), "03c_P_p_APID_1n_FD",
                              Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_BPID_1n_FD = hPlot1D("1n", "FD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"), "03d_P_p_BPID_1n_FD",
                              Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_piplus_APID_1n_CD = hPlot1D("1n", "CD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                                   "04a_P_piplus_APID_1n_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_BPID_1n_CD = hPlot1D("1n", "CD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                                   "04b_P_piplus_BPID_1n_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_APID_1n_FD = hPlot1D("1n", "FD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                                   "04c_P_piplus_APID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_BPID_1n_FD = hPlot1D("1n", "FD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                                   "04d_P_piplus_BPID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_piminus_APID_1n_CD = hPlot1D("1n", "CD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                                    "05a_P_piminus_APID_1n_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_BPID_1n_CD = hPlot1D("1n", "CD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                                    "05b_P_piminus_BPID_1n_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_APID_1n_FD = hPlot1D("1n", "FD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                                    "05c_P_piminus_APID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_BPID_1n_FD = hPlot1D("1n", "FD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                                    "05d_P_piminus_BPID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_n_VN_BPID_1n_FD = hPlot1D("1n", "FD", "P_{n} (from v. neutrons) BPID", "Neutron Momentum (from v. neutrons) BPID", "P_{n} [GeV/c]",
                                 directories.Momentum_dir_map.at("Momentum_1n"), "06a_P_n_VNeutrons_BPID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_n_VN_APID_1n_FD = hPlot1D("1n", "FD", "P_{n} (from v. neutrons) APID", "Neutron Momentum (from v. neutrons) APID", "P_{n} [GeV/c]",
                                 directories.Momentum_dir_map.at("Momentum_1n"), "06b_P_n_VNeutrons_APID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_n_Ph_BPID_1n_FD = hPlot1D("1n", "FD", "P_{n} (from 'Photons') BPID", "Neutron Momentum (from 'Photons') BPID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                                 "07a_P_n_From_Photons_BPID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_n_Ph_APID_1n_FD = hPlot1D("1n", "FD", "P_{n} (from 'Photons') APID", "Neutron Momentum (from 'Photons') APID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                                 "07b_P_n_From_Photons_APID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_ph_APID_1n_FD = hPlot1D("1n", "FD", "Photon momentum APID", "Photon momentum P_{#gamma} APID", "P_{#gamma} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                               "08a_P_ph_APID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_ph_BPID_1n_FD = hPlot1D("1n", "FD", "Photon momentum BPID", "Photon momentum P_{#gamma} BPID", "P_{#gamma} [GeV/c]", directories.Momentum_dir_map.at("Momentum_1n"),
                               "08b_P_ph_BPID_1n_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

#pragma endregion

#pragma endregion

#pragma region /* Momentum threshold plots (2p) */

    hP_e_APID_2p_FD = hPlot1D("2p", "", "Electron momentum APID", "Electron momentum P_{e} APID", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                              "01a_P_e_APID_2p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_e_BPID_2p_FD = hPlot1D("2p", "", "Electron momentum BPID", "Electron momentum P_{e} BPID", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                              "01b_P_e_BPID_2p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_p_APID_2p_CD = hPlot1D("2p", "CD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"), "02a_P_p_APID_2p_CD",
                              Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_BPID_2p_CD = hPlot1D("2p", "CD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"), "02b_P_p_BPID_2p_CD",
                              Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_APID_2p_FD = hPlot1D("2p", "FD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"), "03c_P_p_APID_2p_FD",
                              Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_BPID_2p_FD = hPlot1D("2p", "FD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"), "03d_P_p_BPID_2p_FD",
                              Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_piplus_APID_2p_CD = hPlot1D("2p", "CD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                                   "04a_P_piplus_APID_2p_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_BPID_2p_CD = hPlot1D("2p", "CD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                                   "04b_P_piplus_BPID_2p_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_APID_2p_FD = hPlot1D("2p", "FD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                                   "04c_P_piplus_APID_2p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_BPID_2p_FD = hPlot1D("2p", "FD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                                   "04d_P_piplus_BPID_2p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_piminus_APID_2p_CD = hPlot1D("2p", "CD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                                    "05a_P_piminus_APID_2p_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_BPID_2p_CD = hPlot1D("2p", "CD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                                    "05b_P_piminus_BPID_2p_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_APID_2p_FD = hPlot1D("2p", "FD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                                    "05c_P_piminus_APID_2p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_BPID_2p_FD = hPlot1D("2p", "FD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                                    "05d_P_piminus_BPID_2p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_ph_APID_2p_FD = hPlot1D("2p", "FD", "Photon momentum APID", "Photon momentum P_{#gamma} APID", "P_{#gamma} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                               "06c_P_ph_APID_2p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_ph_BPID_2p_FD = hPlot1D("2p", "FD", "Photon momentum BPID", "Photon momentum P_{#gamma} BPID", "P_{#gamma} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                               "06d_P_ph_BPID_2p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_n_APID_2p_FD = hPlot1D("2p", "FD", "Neutron momentum APID", "Neutron momentum P_{n} APID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                              "07c_P_n_APID_2p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_n_BPID_2p_FD = hPlot1D("2p", "FD", "Neutron momentum BPID", "Neutron momentum P_{n} BPID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"),
                              "07d_P_n_BPID_2p_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    // Leading (P_1) and recoil (P_2) Momentum threshold plots (2p)
    hP_p_1_2p = hPlot1D("2p", "", "Leading proton P_{1}", "Leading proton momentum P_{1}", "P_{1} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"), "07_P_p_1_2p",
                        Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_2_2p = hPlot1D("2p", "", "Recoil proton P_{2}", "Recoil proton momentum P_{2}", "P_{2} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"), "07_P_p_2_2p",
                        Momentum_llim, Momentum_ulim, numTH1Dbins);

    // P1 vs P2 (2p, CD & FD)
    hP_p_1_vs_P_p_2_2p = hPlot2D("2p", "", "P_{1} vs. P_{2}", "P_{1} vs. P_{2}", "P_{1} [GeV/c]", "P_{2} [GeV/c]", directories.Momentum_dir_map.at("Momentum_2p"), "08_P_p_1_vs_P_p_2",
                                 Momentum_llim, Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins, numTH2Dbins);

#pragma endregion

#pragma region /* Momentum threshold plots (pFDpCD) */

#pragma region /* Analysis plots (pFDpCD) */
    hP_pFD_pFDpCD = hPlot1D("pFDpCD", "", "FD proton momentum", "FD proton momentum P_{pFD}", "P_{pFD} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"),
                            "01a_P_pFD_pFDpCD", FDMomentum_llim, FDMomentum_ulim, numTH1Dbins);
    hP_pCD_pFDpCD = hPlot1D("pFDpCD", "", "CD proton momentum", "CD proton momentum P_{pCD}", "P_{pCD} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"),
                            "01b_P_pCD_pFDpCD", CDMomentum_llim, CDMomentum_ulim, numTH1Dbins);
    hP_pFD_vs_P_pCD_pFDpCD =
        hPlot2D("pFDpCD", "", "P_{pFD} vs. P_{pCD}", "P_{pFD} vs. P_{pCD}", "P_{pFD} [GeV/c]", "P_{pCD} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"),
                "05_P_pFD_vs_P_pCD", P_nucFD_llim, P_nucFD_ulim, P_nucCD_llim, P_nucCD_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);

    hP_pFD_VS_W_pFDpCD =
        new TH2D("FD proton momentum vs. W (All Int., pFDpCD)", "FD proton momentum vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pFD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucFD_llim, P_nucFD_ulim);
    hP_pFD_VS_W_QEL_pFDpCD =
        new TH2D("FD proton momentum vs. W (QEL only, pFDpCD)", "FD proton momentum vs. W (QEL only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pFD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucFD_llim, P_nucFD_ulim);
    hP_pFD_VS_W_MEC_pFDpCD =
        new TH2D("FD proton momentum vs. W (MEC only, pFDpCD)", "FD proton momentum vs. W (MEC only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pFD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucFD_llim, P_nucFD_ulim);
    hP_pFD_VS_W_RES_pFDpCD =
        new TH2D("FD proton momentum vs. W (RES only, pFDpCD)", "FD proton momentum vs. W (RES only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pFD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucFD_llim, P_nucFD_ulim);
    hP_pFD_VS_W_DIS_pFDpCD =
        new TH2D("FD proton momentum vs. W (DIS only, pFDpCD)", "FD proton momentum vs. W (DIS only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pFD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucFD_llim, P_nucFD_ulim);
    hP_pFD_VS_W_pFDpCD_Dir = directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD");

    hP_pCD_VS_W_pFDpCD =
        new TH2D("CD proton momentum vs. W (All Int., pFDpCD)", "CD proton momentum vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pCD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucCD_llim, P_nucCD_ulim);
    hP_pCD_VS_W_QEL_pFDpCD =
        new TH2D("CD proton momentum vs. W (QEL only, pFDpCD)", "CD proton momentum vs. W (QEL only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pCD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucCD_llim, P_nucCD_ulim);
    hP_pCD_VS_W_MEC_pFDpCD =
        new TH2D("CD proton momentum vs. W (MEC only, pFDpCD)", "CD proton momentum vs. W (MEC only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pCD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucCD_llim, P_nucCD_ulim);
    hP_pCD_VS_W_RES_pFDpCD =
        new TH2D("CD proton momentum vs. W (RES only, pFDpCD)", "CD proton momentum vs. W (RES only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pCD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucCD_llim, P_nucCD_ulim);
    hP_pCD_VS_W_DIS_pFDpCD =
        new TH2D("CD proton momentum vs. W (DIS only, pFDpCD)", "CD proton momentum vs. W (DIS only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pCD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucCD_llim, P_nucCD_ulim);
    hP_pCD_VS_W_pFDpCD_Dir = directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD");

    // Total and relative nucleon momenta (pFDpCD)
    hP_tot_pFDpCD = hPlot1D("pFDpCD", "", "Total nucleon momentum (3-vector)", "Total nucleon 3-momentum", "|#font[62]{P}_{tot}| = |#font[62]{P}_{pL} + #font[62]{P}_{pR}| [GeV/c]",
                            directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"), "02a_P_tot_pFDpCD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_rel_pFDpCD = hPlot1D("pFDpCD", "", "Relative nucleon momentum (3-vector)", "Relative nucleon 3-momentum", "|#font[62]{P}_{rel}| = |#font[62]{P}_{pL} - #font[62]{P}_{pR}|/2 [GeV/c]",
                            directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"), "02b_P_rel_pFDpCD", CDMomentum_llim, CDMomentum_ulim, numTH1Dbins);
    hP_tot_vs_P_rel_pFDpCD = hPlot2D("pFDpCD", "", "|#font[62]{P}_{tot}| vs. |#font[62]{P}_{rel}|", "|#font[62]{P}_{tot}| vs. |#font[62]{P}_{rel}|",
                                     "|#font[62]{P}_{tot}| = |#font[62]{P}_{pL} + #font[62]{P}_{pR}| [GeV/c]", "|#font[62]{P}_{rel}| = |#font[62]{P}_{pL} - #font[62]{P}_{pR}|/2 [GeV/c]",
                                     directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"), "02c_P_tot_vs_P_rel", Momentum_llim, Momentum_ulim,
                                     Momentum_llim, Momentum_ulim, numTH2Dbins, numTH2Dbins);

    hP_tot_mu_pFDpCD = hPlot1D("pFDpCD", "", "Total nucleon momentum (4-vector)", "Total nucleon 4-momentum", "P_{tot}^{#mu} = (P_{pL}^{#mu} + P_{pR}^{#mu}) [GeV/c]",
                               directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"), "02d_P_tot_pFDpCD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_rel_mu_pFDpCD = hPlot1D("pFDpCD", "", "Relative nucleon momentum (4-vector)", "Relative nucleon 4-momentum", "P_{rel}^{#mu} = (P_{pL}^{#mu} - P_{pR}^{#mu})/2 [GeV/c]",
                               directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"), "02e_P_rel_pFDpCD", -CDMomentum_ulim, CDMomentum_llim, numTH1Dbins);
    //                                       "02e_P_rel_pFDpCD", CDMomentum_llim, CDMomentum_ulim, numTH1Dbins);
    hP_tot_mu_vs_P_rel_mu_pFDpCD = hPlot2D("pFDpCD", "", "P_{tot}^{#mu} vs. P_{rel}^{#mu}", "P_{tot}^{#mu} vs. P_{rel}^{#mu}", "P_{tot}^{#mu} = P_{pL}^{#mu} + P_{pR}^{#mu} [GeV/c]",
                                           "P_{rel}^{#mu} = (P_{pL}^{#mu} - P_{pR}^{#mu})/2 [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"),
                                           "02f_P_tot_vs_P_rel", Momentum_llim, Momentum_ulim, -CDMomentum_ulim, CDMomentum_llim, numTH2Dbins, numTH2Dbins);
    //                                                   Momentum_llim, Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins, numTH2Dbins);

    hP_tot_minus_q_pFDpCD = hPlot1D("pFDpCD", "", "#font[62]{P}_{tot}-#font[62]{q} distribution", "#font[62]{P}_{tot}-#font[62]{q} distribution",
                                    "|#font[62]{P}_{tot} - #font[62]{q}| = |#font[62]{P}_{pL} + #font[62]{P}_{pR}- #font[62]{q}| [GeV/c]",
                                    directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"), "06_P_tot_minus_q_pFDpCD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_tot_minus_q_vs_P_tot_pFDpCD = hPlot2D("pFDpCD", "", "#font[62]{P}_{tot}-#font[62]{q} vs. #font[62]{P}_{tot}", "#font[62]{P}_{tot}-#font[62]{q} vs. #font[62]{P}_{tot}",
                                             "|#font[62]{P}_{tot}| = |#font[62]{P}_{nL} + #font[62]{P}_{nR}| [GeV/c]",
                                             "|#font[62]{P}_{tot} - #font[62]{q}| = |#font[62]{P}_{nL} + #font[62]{P}_{nR}- #font[62]{q}| [GeV/c]",
                                             directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"), "07a_P_tot_minus_q_vs_P_tot_pFDpCD", Momentum_llim,
                                             Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins, numTH2Dbins);
    hP_tot_minus_q_vs_q_pFDpCD = hPlot2D("pFDpCD", "", "|#font[62]{P}_{tot} - #font[62]{q}| vs. |#font[62]{q}|", "|#font[62]{P}_{tot} - #font[62]{q}| vs. |#font[62]{q}|",
                                         "|#font[62]{q}| [GeV/c]", "|#font[62]{P}_{tot} - #font[62]{q}| = |#font[62]{P}_{nL} + #font[62]{P}_{nR}- #font[62]{q}| [GeV/c]",
                                         directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"), "07b_P_tot_minus_q_vs_q_pFDpCD", Momentum_llim, Momentum_ulim,
                                         Momentum_llim, Momentum_ulim, numTH2Dbins, numTH2Dbins);

    // Leading and recoil nucleon momentum plots (pFDpCD)
    hP_pL_pFDpCD = hPlot1D("pFDpCD", "", "Leading proton momentum", "Leading proton momentum", "P_{pL} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"),
                           "03a_P_pL_pFDpCD", FDMomentum_llim, FDMomentum_ulim, numTH1Dbins);
    hP_pR_pFDpCD = hPlot1D("pFDpCD", "", "Recoil proton momentum", "Recoil proton momentum", "P_{pR} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"),
                           "03b_P_pR_pFDpCD", CDMomentum_llim, CDMomentum_ulim, numTH1Dbins);
    hP_pL_vs_P_pR_pFDpCD =
        hPlot2D("pFDpCD", "", "P_{pL} vs. P_{pR}", "P_{pL} vs. P_{pR}", "P_{pL} [GeV/c]", "P_{pR} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD"),
                "04_P_pL_vs_P_pR", P_nucFD_llim, P_nucFD_ulim, P_nucCD_llim, P_nucCD_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);

    hP_e_vs_Theta_e_reco_All_int_pFDpCD = new TH2D("P_e_vs_Theta_e_reco_All_int_pFDpCD", "P_{e} vs. #theta_{e} (All Int., pFDpCD);P_{e} [GeV/c];#theta_{e} [#circ];",
                                                   numTH2Dbins_ReacMon_plots, Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_QEL_pFDpCD = new TH2D("P_e_vs_Theta_e_reco_QEL_pFDpCD", "P_{e} vs. #theta_{e} (QEL only, pFDpCD);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                               Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_MEC_pFDpCD = new TH2D("P_e_vs_Theta_e_reco_MEC_pFDpCD", "P_{e} vs. #theta_{e} (MEC only, pFDpCD);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                               Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_RES_pFDpCD = new TH2D("P_e_vs_Theta_e_reco_RES_pFDpCD", "P_{e} vs. #theta_{e} (RES only, pFDpCD);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                               Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_DIS_pFDpCD = new TH2D("P_e_vs_Theta_e_reco_DIS_pFDpCD", "P_{e} vs. #theta_{e} (DIS only, pFDpCD);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                               Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_pFDpCD_Dir = directories.Momentum_dir_map.at("Analysis_plots_momentum_pFDpCD");

#pragma endregion

#pragma region /* Other momentum threshold plots (pFDpCD) */

    hP_e_APID_pFDpCD_FD = hPlot1D("pFDpCD", "", "Electron momentum APID", "Electron momentum P_{e} APID", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Momentum_pFDpCD"),
                                  "01a_P_e_APID_pFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_e_BPID_pFDpCD_FD = hPlot1D("pFDpCD", "", "Electron momentum BPID", "Electron momentum P_{e} BPID", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Momentum_pFDpCD"),
                                  "01b_P_e_BPID_pFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_p_APID_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_pFDpCD"),
                                  "02a_P_p_APID_pFDpCD_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_BPID_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_pFDpCD"),
                                  "02b_P_p_BPID_pFDpCD_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_APID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_pFDpCD"),
                                  "03c_P_p_APID_pFDpCD_FD", FDMomentum_llim, FDMomentum_ulim, numTH1Dbins);
    hP_p_APIDandPS_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Proton momentum APID&PS", "Proton momentum P_{p} APID&PS", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_pFDpCD"),
                                       "03d_P_p_APIDandPS_pFDpCD_FD", FDMomentum_llim, FDMomentum_ulim, numTH1Dbins);
    hP_p_BPID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_pFDpCD"),
                                  "03e_P_p_BPID_pFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_piplus_APID_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                       directories.Momentum_dir_map.at("Momentum_pFDpCD"), "04a_P_piplus_APID_pFDpCD_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_BPID_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                       directories.Momentum_dir_map.at("Momentum_pFDpCD"), "04b_P_piplus_BPID_pFDpCD_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_APID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                       directories.Momentum_dir_map.at("Momentum_pFDpCD"), "04c_P_piplus_APID_pFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_BPID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                       directories.Momentum_dir_map.at("Momentum_pFDpCD"), "04d_P_piplus_BPID_pFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_piminus_APID_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                        directories.Momentum_dir_map.at("Momentum_pFDpCD"), "05a_P_piminus_APID_pFDpCD_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_BPID_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                        directories.Momentum_dir_map.at("Momentum_pFDpCD"), "05b_P_piminus_BPID_pFDpCD_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_APID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                        directories.Momentum_dir_map.at("Momentum_pFDpCD"), "05c_P_piminus_APID_pFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_BPID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                        directories.Momentum_dir_map.at("Momentum_pFDpCD"), "05d_P_piminus_BPID_pFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_ph_APID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Photon momentum APID", "Photon momentum P_{#gamma} APID", "P_{#gamma} [GeV/c]", directories.Momentum_dir_map.at("Momentum_pFDpCD"),
                                   "06c_P_ph_APID_pFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_ph_BPID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Photon momentum BPID", "Photon momentum P_{#gamma} BPID", "P_{#gamma} [GeV/c]", directories.Momentum_dir_map.at("Momentum_pFDpCD"),
                                   "06d_P_ph_BPID_pFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_n_APID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Neutron momentum APID", "Neutron momentum P_{n} APID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_pFDpCD"),
                                  "07c_P_n_APID_pFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_n_BPID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Neutron momentum BPID", "Neutron momentum P_{n} BPID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_pFDpCD"),
                                  "07d_P_n_BPID_pFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

#pragma endregion

#pragma endregion

#pragma region /* Momentum threshold plots (nFDpCD) */

#pragma region /* Analysis plots (nFDpCD) */
    hP_nFD_nFDpCD = hPlot1D("nFDpCD", "", "FD neutron momentum", "FD neutron momentum P_{nFD}", "P_{nFD} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"),
                            "01aa_P_nFD_nFDpCD", FDMomentum_llim, FDMomentum_ulim, numTH1Dbins);
    hP_nFD_nFDpCD_ZoomOut = hPlot1D("nFDpCD", "", "FD neutron momentum - ZoomOut", "FD neutron momentum P_{nFD} - ZoomOut", "P_{nFD} [GeV/c]",
                                    directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "01ab_P_nFD_nFDpCD_ZoomOut", FDMomentum_llim, beamE * 3., numTH1Dbins);
    hP_pCD_nFDpCD = hPlot1D("nFDpCD", "", "CD proton momentum", "CD proton momentum P_{pCD}", "P_{pCD} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"),
                            "01b_P_pCD_nFDpCD", CDMomentum_llim, CDMomentum_ulim, numTH1Dbins);
    hP_nFD_vs_P_pCD_nFDpCD =
        hPlot2D("nFDpCD", "", "P_{nFD} vs. P_{pCD}", "P_{nFD} vs. P_{pCD}", "P_{nFD} [GeV/c]", "P_{pCD} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"),
                "05_P_nFD_vs_P_pCD", P_nucFD_llim, P_nucFD_ulim, P_nucCD_llim, P_nucCD_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hP_nFD_vs_P_e_nFDpCD =
        hPlot2D("nFDpCD", "", "P_{nFD} vs. P_{e}", "P_{nFD} vs. P_{e}", "P_{nFD} [GeV/c]", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"),
                "05aa_P_nFD_vs_P_e", P_nucFD_llim, P_nucFD_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hP_nFD_vs_P_e_ZoomOut_nFDpCD = hPlot2D("nFDpCD", "", "P_{nFD} vs. P_{e} - ZoomOut", "P_{nFD} vs. P_{e} - ZoomOut", "P_{nFD} [GeV/c]", "P_{e} [GeV/c]",
                                           directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "05ab_P_nFD_vs_P_e_ZoomOut", P_nucFD_llim, beamE * 3.,
                                           Momentum_llim, Momentum_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hP_nFD_vs_theta_e_nFDpCD = hPlot2D("nFDpCD", "", "P_{nFD} vs. #theta_{e}", "P_{nFD} vs. #theta_{e}", "P_{nFD} [GeV/c]", "#theta_{e} [#circ]",
                                       directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "05ba_P_nFD_vs_theta_e", P_nucFD_llim, P_nucFD_ulim,
                                       Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hP_nFD_vs_theta_e_ZoomOut_nFDpCD = hPlot2D("nFDpCD", "", "P_{nFD} vs. #theta_{e} - ZoomOut", "P_{nFD} vs. #theta_{e} - ZoomOut", "P_{nFD} [GeV/c]", "#theta_{e} [#circ]",
                                               directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "05bb_P_nFD_vs_theta_e_ZoomOut", P_nucFD_llim, beamE * 3.,
                                               Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hP_nFD_vs_phi_e_nFDpCD =
        hPlot2D("nFDpCD", "", "P_{nFD} vs. #phi_{e}", "P_{nFD} vs. #phi_{e}", "P_{nFD} [GeV/c]", "#phi_{e} [#circ]", directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"),
                "05ca_P_nFD_vs_phi_e", P_nucFD_llim, P_nucFD_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hP_nFD_vs_phi_e_ZoomOut_nFDpCD = hPlot2D("nFDpCD", "", "P_{nFD} vs. #phi_{e} - ZoomOut", "P_{nFD} vs. #phi_{e} - ZoomOut", "P_{nFD} [GeV/c]", "#phi_{e} [#circ]",
                                             directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "05cb_P_nFD_vs_phi_e_ZoomOut", P_nucFD_llim, beamE * 3.,
                                             Phi_llim, Phi_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    htheta_nFD_vs_P_e_nFDpCD = hPlot2D("nFDpCD", "", "#theta_{nFD} vs. P_{e}", "#theta_{nFD} vs. P_{e}", "#theta_{nFD} [#circ]", "P_{e} [GeV/c]",
                                       directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "05d_theta_nFD_vs_P_e", Theta_llim_FD, Theta_ulim_FD,
                                       Momentum_llim, Momentum_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    htheta_nFD_vs_theta_e_nFDpCD = hPlot2D("nFDpCD", "", "#theta_{nFD} vs. #theta_{e}", "#theta_{nFD} vs. #theta_{e}", "#theta_{nFD} [#circ]", "#theta_{e} [#circ]",
                                           directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "05e_theta_nFD_vs_theta_e", Theta_llim_FD, Theta_ulim_FD,
                                           Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    htheta_nFD_vs_phi_e_nFDpCD = hPlot2D("nFDpCD", "", "#theta_{nFD} vs. #phi_{e}", "#theta_{nFD} vs. #phi_{e}", "#theta_{nFD} [#circ]", "#phi_{e} [#circ]",
                                         directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "05f_theta_nFD_vs_phi_e", Theta_llim_FD, Theta_ulim_FD,
                                         Phi_llim, Phi_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hphi_nFD_vs_P_e_nFDpCD =
        hPlot2D("nFDpCD", "", "#phi_{nFD} vs. P_{e}", "#phi_{nFD} vs. P_{e}", "#phi_{nFD} [#circ]", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"),
                "05g_phi_nFD_vs_P_e", Phi_llim, Phi_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hphi_nFD_vs_theta_e_nFDpCD = hPlot2D("nFDpCD", "", "#phi_{nFD} vs. #theta_{e}", "#phi_{nFD} vs. #theta_{e}", "#phi_{nFD} [#circ]", "#theta_{e} [#circ]",
                                         directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "05h_phi_nFD_vs_theta_e", Phi_llim, Phi_ulim, Theta_llim_FD,
                                         Theta_ulim_FD, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);
    hphi_nFD_vs_phi_e_nFDpCD = hPlot2D("nFDpCD", "", "#phi_{nFD} vs. #phi_{e}", "#phi_{nFD} vs. #phi_{e}", "#phi_{nFD} [#circ]", "#phi_{e} [#circ]",
                                       directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "05i_phi_nFD_vs_phi_e", Phi_llim, Phi_ulim, Phi_llim,
                                       Phi_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);

    hP_nFD_VS_W_nFDpCD = new TH2D("FD neutron momentum vs. W (All Int., nFDpCD)",
                                  "FD neutron momentum vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{nFD} [GeV/c]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucFD_llim, P_nucFD_ulim);
    hP_nFD_VS_W_QEL_nFDpCD = new TH2D("FD neutron momentum vs. W (QEL only, nFDpCD)",
                                      "FD neutron momentum vs. W (QEL only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{nFD} [GeV/c]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucFD_llim, P_nucFD_ulim);
    hP_nFD_VS_W_MEC_nFDpCD = new TH2D("FD neutron momentum vs. W (MEC only, nFDpCD)",
                                      "FD neutron momentum vs. W (MEC only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{nFD} [GeV/c]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucFD_llim, P_nucFD_ulim);
    hP_nFD_VS_W_RES_nFDpCD = new TH2D("FD neutron momentum vs. W (RES only, nFDpCD)",
                                      "FD neutron momentum vs. W (RES only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{nFD} [GeV/c]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucFD_llim, P_nucFD_ulim);
    hP_nFD_VS_W_DIS_nFDpCD = new TH2D("FD neutron momentum vs. W (DIS only, nFDpCD)",
                                      "FD neutron momentum vs. W (DIS only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{nFD} [GeV/c]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucFD_llim, P_nucFD_ulim);
    hP_nFD_VS_W_nFDpCD_Dir = directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD");

    hP_pCD_VS_W_nFDpCD =
        new TH2D("CD proton momentum vs. W (All Int., nFDpCD)", "CD proton momentum vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pCD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucCD_llim, P_nucCD_ulim);
    hP_pCD_VS_W_QEL_nFDpCD =
        new TH2D("CD proton momentum vs. W (QEL only, nFDpCD)", "CD proton momentum vs. W (QEL only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pCD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucCD_llim, P_nucCD_ulim);
    hP_pCD_VS_W_MEC_nFDpCD =
        new TH2D("CD proton momentum vs. W (MEC only, nFDpCD)", "CD proton momentum vs. W (MEC only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pCD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucCD_llim, P_nucCD_ulim);
    hP_pCD_VS_W_RES_nFDpCD =
        new TH2D("CD proton momentum vs. W (RES only, nFDpCD)", "CD proton momentum vs. W (RES only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pCD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucCD_llim, P_nucCD_ulim);
    hP_pCD_VS_W_DIS_nFDpCD =
        new TH2D("CD proton momentum vs. W (DIS only, nFDpCD)", "CD proton momentum vs. W (DIS only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];P_{pCD} [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, P_nucCD_llim, P_nucCD_ulim);
    hP_pCD_VS_W_nFDpCD_Dir = directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD");

    // Total and Relative nucleon momenta (nFDpCD)
    hP_tot_nFDpCD = hPlot1D("nFDpCD", "", "Total nucleon momentum (3-vector)", "Total nucleon 3-momentum", "|#font[62]{P}_{tot}| = |#font[62]{P}_{nL} + #font[62]{P}_{nR}| [GeV/c]",
                            directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "02a_P_tot_nFDpCD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_rel_nFDpCD = hPlot1D("nFDpCD", "", "Relative nucleon momentum (3-vector)", "Relative nucleon 3-momentum", "|#font[62]{P}_{rel}| = |#font[62]{P}_{nL} - #font[62]{P}_{nR}|/2 [GeV/c]",
                            directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "02b_P_rel_nFDpCD", CDMomentum_llim, CDMomentum_ulim, numTH1Dbins);
    hP_tot_vs_P_rel_nFDpCD = hPlot2D("nFDpCD", "", "|#font[62]{P}_{tot}| vs. |#font[62]{P}_{rel}|", "|#font[62]{P}_{tot}| vs. |#font[62]{P}_{rel}|",
                                     "|#font[62]{P}_{tot}| = |#font[62]{P}_{nL} + #font[62]{P}_{nR}| [GeV/c]", "|#font[62]{P}_{rel}| = |#font[62]{P}_{nL} - #font[62]{P}_{nR}|/2 [GeV/c]",
                                     directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "02c_P_tot_vs_P_rel", Momentum_llim, Momentum_ulim,
                                     Momentum_llim, Momentum_ulim, numTH2Dbins, numTH2Dbins);

    hP_tot_mu_nFDpCD = hPlot1D("nFDpCD", "", "Total nucleon momentum (4-vector)", "Total nucleon 4-momentum", "P_{tot}^{#mu} = (P_{nL}^{#mu} + P_{nR}^{#mu}) [GeV/c]",
                               directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "02d_P_tot_nFDpCD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_rel_mu_nFDpCD = hPlot1D("nFDpCD", "", "Relative nucleon momentum (4-vector)", "Relative nucleon 4-momentum", "P_{rel}^{#mu} = (P_{nL}^{#mu} - P_{nR}^{#mu})/2 [GeV/c]",
                               directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "02e_P_rel_nFDpCD", -CDMomentum_ulim, CDMomentum_llim, numTH1Dbins);
    //                                       "02e_P_rel_nFDpCD", CDMomentum_llim, CDMomentum_ulim, numTH1Dbins);
    hP_tot_mu_vs_P_rel_mu_nFDpCD = hPlot2D("nFDpCD", "", "P_{tot}^{#mu} vs. P_{rel}^{#mu}", "P_{tot}^{#mu} vs. P_{rel}^{#mu}", "P_{tot}^{#mu} = P_{nL}^{#mu} + P_{nR}^{#mu} [GeV/c]",
                                           "P_{rel}^{#mu} = (P_{nL}^{#mu} - P_{nR}^{#mu})/2 [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"),
                                           "02f_P_tot_vs_P_rel", Momentum_llim, Momentum_ulim, -CDMomentum_ulim, CDMomentum_llim, numTH2Dbins, numTH2Dbins);

    hP_tot_minus_q_nFDpCD = hPlot1D("nFDpCD", "", "#font[62]{P}_{tot}-#font[62]{q} distribution", "#font[62]{P}_{tot}-#font[62]{q} distribution",
                                    "|#font[62]{P}_{tot} - #font[62]{q}| = |#font[62]{P}_{nL} + #font[62]{P}_{nR}- #font[62]{q}| [GeV/c]",
                                    directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "06_P_tot_minus_q_nFDpCD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_tot_minus_q_vs_P_tot_nFDpCD = hPlot2D("nFDpCD", "", "#font[62]{P}_{tot}-#font[62]{q} vs. #font[62]{P}_{tot}", "#font[62]{P}_{tot}-#font[62]{q} vs. #font[62]{P}_{tot}",
                                             "|#font[62]{P}_{tot}| = |#font[62]{P}_{nL} + #font[62]{P}_{nR}| [GeV/c]",
                                             "|#font[62]{P}_{tot} - #font[62]{q}| = |#font[62]{P}_{nL} + #font[62]{P}_{nR}- #font[62]{q}| [GeV/c]",
                                             directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "07a_P_tot_minus_q_vs_P_tot_nFDpCD", Momentum_llim,
                                             Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins, numTH2Dbins);
    hP_tot_minus_q_vs_q_nFDpCD = hPlot2D("nFDpCD", "", "|#font[62]{P}_{tot} - #font[62]{q}| vs. |#font[62]{q}|", "|#font[62]{P}_{tot} - #font[62]{q}| vs. |#font[62]{q}|",
                                         "|#font[62]{q}| [GeV/c]", "|#font[62]{P}_{tot} - #font[62]{q}| = |#font[62]{P}_{nL} + #font[62]{P}_{nR}- #font[62]{q}| [GeV/c]",
                                         directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"), "07b_P_tot_minus_q_vs_q_nFDpCD", Momentum_llim, Momentum_ulim,
                                         Momentum_llim, Momentum_ulim, numTH2Dbins, numTH2Dbins);

    // Leading and recoil nucleon momentum plots (nFDpCD)
    hP_nL_nFDpCD = hPlot1D("nFDpCD", "", "Leading nucleon momentum", "Leading nucleon momentum", "P_{nL} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"),
                           "03a_P_nL_nFDpCD", FDMomentum_llim, FDMomentum_ulim, numTH1Dbins);
    hP_nR_nFDpCD = hPlot1D("nFDpCD", "", "Recoil nucleon momentum", "Recoil nucleon momentum", "P_{nR} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"),
                           "03b_P_nR_nFDpCD", CDMomentum_llim, CDMomentum_ulim, numTH1Dbins);
    hP_nL_vs_P_nR_nFDpCD =
        hPlot2D("nFDpCD", "", "P_{nL} vs. P_{nR}", "P_{nL} vs. P_{nR}", "P_{nL} [GeV/c]", "P_{nR} [GeV/c]", directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD"),
                "04_P_nL_vs_P_nR", P_nucFD_llim, P_nucFD_ulim, P_nucCD_llim, P_nucCD_ulim, numTH2Dbins_Mom_Plots, numTH2Dbins_Mom_Plots);

    hP_e_vs_Theta_e_reco_All_int_nFDpCD = new TH2D("P_e_vs_Theta_e_reco_All_int_nFDpCD", "P_{e} vs. #theta_{e} (All Int., nFDpCD);P_{e} [GeV/c];#theta_{e} [#circ];",
                                                   numTH2Dbins_ReacMon_plots, Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_QEL_nFDpCD = new TH2D("P_e_vs_Theta_e_reco_QEL_nFDpCD", "P_{e} vs. #theta_{e} (QEL only, nFDpCD);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                               Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_MEC_nFDpCD = new TH2D("P_e_vs_Theta_e_reco_MEC_nFDpCD", "P_{e} vs. #theta_{e} (MEC only, nFDpCD);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                               Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_RES_nFDpCD = new TH2D("P_e_vs_Theta_e_reco_RES_nFDpCD", "P_{e} vs. #theta_{e} (RES only, nFDpCD);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                               Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_DIS_nFDpCD = new TH2D("P_e_vs_Theta_e_reco_DIS_nFDpCD", "P_{e} vs. #theta_{e} (DIS only, nFDpCD);P_{e} [GeV/c];#theta_{e} [#circ];", numTH2Dbins_ReacMon_plots,
                                               Momentum_llim, Momentum_ulim, numTH2Dbins_ReacMon_plots, Theta_llim_FD, Theta_ulim_FD);
    hP_e_vs_Theta_e_reco_nFDpCD_Dir = directories.Momentum_dir_map.at("Analysis_plots_momentum_nFDpCD");

#pragma endregion

#pragma region /* Other momentum threshold plots (nFDpCD) */

    hP_e_APID_nFDpCD_FD = hPlot1D("nFDpCD", "", "Electron momentum APID", "Electron momentum P_{e} APID", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Momentum_nFDpCD"),
                                  "01a_P_e_APID_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_e_BPID_nFDpCD_FD = hPlot1D("nFDpCD", "", "Electron momentum BPID", "Electron momentum P_{e} BPID", "P_{e} [GeV/c]", directories.Momentum_dir_map.at("Momentum_nFDpCD"),
                                  "01b_P_e_BPID_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_p_APID_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_nFDpCD"),
                                  "02a_P_p_APID_nFDpCD_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_BPID_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_nFDpCD"),
                                  "02b_P_p_BPID_nFDpCD_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_APID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_nFDpCD"),
                                  "03c_P_p_APID_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_p_BPID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]", directories.Momentum_dir_map.at("Momentum_nFDpCD"),
                                  "03d_P_p_BPID_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_piplus_APID_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                       directories.Momentum_dir_map.at("Momentum_nFDpCD"), "04a_P_piplus_APID_nFDpCD_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_BPID_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                       directories.Momentum_dir_map.at("Momentum_nFDpCD"), "04b_P_piplus_BPID_nFDpCD_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_APID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                       directories.Momentum_dir_map.at("Momentum_nFDpCD"), "04c_P_piplus_APID_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piplus_BPID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                       directories.Momentum_dir_map.at("Momentum_nFDpCD"), "04d_P_piplus_BPID_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_piminus_APID_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                        directories.Momentum_dir_map.at("Momentum_nFDpCD"), "05a_P_piminus_APID_nFDpCD_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_BPID_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                        directories.Momentum_dir_map.at("Momentum_nFDpCD"), "05b_P_piminus_BPID_nFDpCD_CD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_APID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                        directories.Momentum_dir_map.at("Momentum_nFDpCD"), "05c_P_piminus_APID_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_piminus_BPID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                        directories.Momentum_dir_map.at("Momentum_nFDpCD"), "05d_P_piminus_BPID_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_ph_APID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Photon momentum APID", "Photon momentum P_{#gamma} APID", "P_{#gamma} [GeV/c]", directories.Momentum_dir_map.at("Momentum_nFDpCD"),
                                   "06c_P_ph_APID_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_ph_BPID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Photon momentum BPID", "Photon momentum P_{#gamma} BPID", "P_{#gamma} [GeV/c]", directories.Momentum_dir_map.at("Momentum_nFDpCD"),
                                   "06d_P_ph_BPID_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_n_APID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Neutron momentum APID", "Neutron momentum P_{n} APID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_nFDpCD"),
                                  "07a_P_n_APID_nFDpCD_FD", FDMomentum_llim, FDMomentum_ulim, numTH1Dbins);
    hP_n_APIDandNS_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Neutron momentum APID&NC", "Neutron momentum P_{n} APID&NC", "P_{n} [GeV/c]",
                                       directories.Momentum_dir_map.at("Momentum_nFDpCD"), "07b_P_n_APIDandNS_nFDpCD_FD", FDMomentum_llim, FDMomentum_ulim, numTH1Dbins);
    hP_n_BPID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Neutron momentum BPID", "Neutron momentum P_{n} BPID", "P_{n} [GeV/c]", directories.Momentum_dir_map.at("Momentum_nFDpCD"),
                                  "07c_P_n_BPID_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

    hP_nL_APID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Leading neutron momentum APID", "Leading neutron momentum P_{n} APID", "P_{n} [GeV/c]",
                                   directories.Momentum_dir_map.at("Momentum_nFDpCD"), "07a_P_n_APID_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);
    hP_nL_APIDandNS_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Leading neutron momentum APID&NC", "Leading neutron momentum P_{n} APID&NC", "P_{n} [GeV/c]",
                                        directories.Momentum_dir_map.at("Momentum_nFDpCD"), "07b_P_n_APIDandNS_nFDpCD_FD", Momentum_llim, Momentum_ulim, numTH1Dbins);

#pragma endregion

#pragma endregion

#pragma endregion

#pragma endregion
}

#endif  // RECOANALYZERHISTOGRAMS_H
