#pragma once

#include <cmath>
#include <string>
#include <vector>

#include "TH1D.h"
#include "TH2D.h"
#include "TObject.h"
#include "TVector3.h"

struct RawTruth_histograms {
    // =========================
    // Electron RawTruth Histograms
    // =========================
    TH1D* h_truth_P_e_RawTruth_1e_cut;
    TH1D* h_truth_theta_e_RawTruth_1e_cut;
    TH1D* h_truth_phi_e_RawTruth_1e_cut;
    TH2D* h_truth_theta_e_VS_truth_phi_e_RawTruth_1e_cut;
    TH2D* h_truth_theta_e_VS_truth_P_e_RawTruth_1e_cut;

    // =========================
    // Neutron FD RawTruth Histograms
    // =========================
    TH1D* h_truth_P_nFD_RawTruth_1e_cut;
    TH1D* h_truth_theta_nFD_RawTruth_1e_cut;
    TH1D* h_truth_phi_nFD_RawTruth_1e_cut;
    TH2D* h_truth_theta_nFD_VS_truth_phi_nFD_RawTruth_1e_cut;
    TH2D* h_truth_theta_nFD_VS_truth_P_nFD_RawTruth_1e_cut;

    // =======================
    // Constructor
    // =======================
    RawTruth_histograms(std::vector<std::string>& HistSubjects, std::vector<std::string>& HistSubjects2, std::vector<bool>& FirstPrint, std::vector<TH1*>& HistoList_RawTruth,
                        const double& P_upperLim) {
        HistSubjects.push_back("Raw truth");
        HistSubjects2.push_back("Raw truth");
        FirstPrint.push_back(true);

        // =========================
        // Electron RawTruth Histograms
        // =========================
        h_truth_P_e_RawTruth_1e_cut = new TH1D("truth_P_e_RawTruth_1e_cut", "P^{truth}_{e} in 1e cut (RawTruth);P^{truth}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
        HistoList_RawTruth.push_back(h_truth_P_e_RawTruth_1e_cut);
        h_truth_theta_e_RawTruth_1e_cut = new TH1D("truth_theta_e_RawTruth_1e_cut", "#theta^{truth}_{e} in 1e cut (RawTruth);#theta^{truth}_{e} [#circ];Counts", 50, 0, 45.);
        HistoList_RawTruth.push_back(h_truth_theta_e_RawTruth_1e_cut);
        h_truth_phi_e_RawTruth_1e_cut = new TH1D("truth_phi_e_RawTruth_1e_cut", "#phi^{truth}_{e} in 1e cut (RawTruth);#phi^{truth}_{e} [#circ];Counts", 50, -180, 180.);
        HistoList_RawTruth.push_back(h_truth_phi_e_RawTruth_1e_cut);
        h_truth_theta_e_VS_truth_phi_e_RawTruth_1e_cut =
            new TH2D("truth_theta_e_VS_truth_phi_e_RawTruth_1e_cut", "#theta^{truth}_{e} vs. #phi^{truth}_{e} in 1e cut (RawTruth);#phi^{truth}_{e} [#circ];#theta^{truth}_{e} [#circ]", 100,
                     -180., 180., 100, 0, 50.);
        HistoList_RawTruth.push_back(h_truth_theta_e_VS_truth_phi_e_RawTruth_1e_cut);
        h_truth_theta_e_VS_truth_P_e_RawTruth_1e_cut =
            new TH2D("truth_theta_e_VS_truth_P_e_RawTruth_1e_cut", "#theta^{truth}_{e} vs. P^{truth}_{e} in 1e cut (RawTruth);P^{truth}_{e} [GeV/c];#theta^{truth}_{e} [#circ]", 100, 0,
                     P_upperLim, 100, 0, 50.);
        HistoList_RawTruth.push_back(h_truth_theta_e_VS_truth_P_e_RawTruth_1e_cut);

        // =========================
        // Neutron FD RawTruth Histograms
        // =========================
        h_truth_P_nFD_RawTruth_1e_cut = new TH1D("truth_P_nFD_RawTruth_1e_cut", "P^{truth}_{nFD} in 1e cut (RawTruth);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
        HistoList_RawTruth.push_back(h_truth_P_nFD_RawTruth_1e_cut);
        h_truth_theta_nFD_RawTruth_1e_cut = new TH1D("truth_theta_nFD_RawTruth_1e_cut", "#theta^{truth}_{nFD} in 1e cut (RawTruth);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
        HistoList_RawTruth.push_back(h_truth_theta_nFD_RawTruth_1e_cut);
        h_truth_phi_nFD_RawTruth_1e_cut = new TH1D("truth_phi_nFD_RawTruth_1e_cut", "#phi^{truth}_{nFD} in 1e cut (RawTruth);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
        HistoList_RawTruth.push_back(h_truth_phi_nFD_RawTruth_1e_cut);
        h_truth_theta_nFD_VS_truth_phi_nFD_RawTruth_1e_cut =
            new TH2D("truth_theta_nFD_VS_truth_phi_nFD_RawTruth_1e_cut",
                     "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (RawTruth);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
        HistoList_RawTruth.push_back(h_truth_theta_nFD_VS_truth_phi_nFD_RawTruth_1e_cut);
        h_truth_theta_nFD_VS_truth_P_nFD_RawTruth_1e_cut =
            new TH2D("truth_theta_nFD_VS_truth_P_nFD_RawTruth_1e_cut", "#theta^{truth}_{nFD} vs. P^{truth}_{nFD} in 1e cut (RawTruth);P^{truth}_{nFD} [GeV/c];#theta^{truth}_{nFD} [#circ]",
                     100, 0, P_upperLim, 100, 0, 50.);
        HistoList_RawTruth.push_back(h_truth_theta_nFD_VS_truth_P_nFD_RawTruth_1e_cut);
    }

    // =========================
    // FILLER FUNCTIONS
    // =========================
    // Canonical filler: call once per generated particle (matches the mcpbank loop usage)
    void Fill(const double& pid, const double& truth_P, const double& truth_theta_rad, const double& truth_phi_rad, const double& weight) {
        const double rad2deg = 180.0 / std::acos(-1.0);
        const double truth_theta_deg = truth_theta_rad * rad2deg;
        const double truth_phi_deg = truth_phi_rad * rad2deg;

        if (pid == 11) {
            h_truth_P_e_RawTruth_1e_cut->Fill(truth_P, weight);
            h_truth_theta_e_RawTruth_1e_cut->Fill(truth_theta_deg, weight);
            h_truth_phi_e_RawTruth_1e_cut->Fill(truth_phi_deg, weight);
            h_truth_theta_e_VS_truth_phi_e_RawTruth_1e_cut->Fill(truth_phi_deg, truth_theta_deg, weight);
            h_truth_theta_e_VS_truth_P_e_RawTruth_1e_cut->Fill(truth_P, truth_theta_deg, weight);
        } else if (pid == 2112) {
            h_truth_P_nFD_RawTruth_1e_cut->Fill(truth_P, weight);
            h_truth_theta_nFD_RawTruth_1e_cut->Fill(truth_theta_deg, weight);
            h_truth_phi_nFD_RawTruth_1e_cut->Fill(truth_phi_deg, weight);
            h_truth_theta_nFD_VS_truth_phi_nFD_RawTruth_1e_cut->Fill(truth_phi_deg, truth_theta_deg, weight);
            h_truth_theta_nFD_VS_truth_P_nFD_RawTruth_1e_cut->Fill(truth_P, truth_theta_deg, weight);
        }
    }

    // Convenience overload if you already have a TVector3 momentum
    void Fill(const TVector3& truth_p3, const double& pid, const double& weight) { Fill(pid, truth_p3.Mag(), truth_p3.Theta(), truth_p3.Phi(), weight); }
};
