#pragma once

#include <cmath>
#include <string>
#include <vector>

#include "TH1D.h"
#include "TH2D.h"
#include "TObject.h"
#include "TVector3.h"

struct RawRecoKinematics_histograms {
    TH1D *h_reco_P_e_1e_cut, *h_truth_P_e_1e_cut;
    TH1D *h_reco_theta_e_1e_cut, *h_truth_theta_e_1e_cut;
    TH1D *h_reco_phi_e_1e_cut, *h_truth_phi_e_1e_cut;
    TH2D *h_reco_theta_e_VS_reco_phi_e_1e_cut, *h_truth_theta_e_VS_truth_phi_e_1e_cut;

    TH1D *h_reco_P_n_1e_cut, *h_truth_P_n_1e_cut;
    TH1D *h_reco_theta_n_1e_cut, *h_truth_theta_n_1e_cut;
    TH1D *h_reco_phi_n_1e_cut, *h_truth_phi_n_1e_cut;
    TH2D *h_reco_theta_n_VS_reco_phi_n_1e_cut, *h_truth_theta_n_VS_truth_phi_n_1e_cut;

    // =======================
    // Constructor
    // =======================
    RawRecoKinematics_histograms(std::vector<TH1*>& HistoList_raw, const double P_upperLim) {
        h_reco_P_e_1e_cut = new TH1D("reco_P_e_1e_cut", "P^{reco}_{e} in 1e cut;P^{reco}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
        HistoList_raw.push_back(h_reco_P_e_1e_cut);
        h_truth_P_e_1e_cut = new TH1D("truth_P_e_1e_cut", "P^{truth}_{e} in 1e cut;P^{truth}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
        HistoList_raw.push_back(h_truth_P_e_1e_cut);
        h_reco_theta_e_1e_cut = new TH1D("reco_theta_e_1e_cut", "#theta^{reco}_{e} in 1e cut;#theta^{reco}_{e} [#circ];Counts", 50, 0, 45.);
        HistoList_raw.push_back(h_reco_theta_e_1e_cut);
        h_truth_theta_e_1e_cut = new TH1D("truth_theta_e_1e_cut", "#theta^{truth}_{e} in 1e cut;#theta^{truth}_{e} [#circ];Counts", 50, 0, 45.);
        HistoList_raw.push_back(h_truth_theta_e_1e_cut);
        h_reco_phi_e_1e_cut = new TH1D("reco_phi_e_1e_cut", "#phi^{reco}_{e} in 1e cut;#phi^{reco}_{e} [#circ];Counts", 50, -180, 180.);
        HistoList_raw.push_back(h_reco_phi_e_1e_cut);
        h_truth_phi_e_1e_cut = new TH1D("truth_phi_e_1e_cut", "#phi^{truth}_{e} in 1e cut;#phi^{truth}_{e} [#circ];Counts", 50, -180, 180.);
        HistoList_raw.push_back(h_truth_phi_e_1e_cut);
        h_reco_theta_e_VS_reco_phi_e_1e_cut =
            new TH2D("reco_theta_e_VS_reco_phi_e_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{e} in 1e cut;#phi^{reco}_{e} [#circ];#theta^{reco}_{e} [#circ]", 100, -180., 180., 100, 0, 50.);
        HistoList_raw.push_back(h_reco_theta_e_VS_reco_phi_e_1e_cut);
        h_truth_theta_e_VS_truth_phi_e_1e_cut = new TH2D(
            "truth_theta_e_VS_truth_phi_e_1e_cut", "#theta^{truth}_{e} vs. #phi^{truth}_{e} in 1e cut;#phi^{truth}_{e} [#circ];#theta^{truth}_{e} [#circ]", 100, -180., 180., 100, 0, 50.);
        HistoList_raw.push_back(h_truth_theta_e_VS_truth_phi_e_1e_cut);

        h_reco_P_n_1e_cut = new TH1D("reco_P_n_1e_cut", "P^{reco}_{n} in 1e cut;P^{reco}_{n} [GeV/c];Counts", 50, 0, P_upperLim);
        HistoList_raw.push_back(h_reco_P_n_1e_cut);
        h_truth_P_n_1e_cut = new TH1D("truth_P_n_1e_cut", "P^{truth}_{n} in 1e cut;P^{truth}_{n} [GeV/c];Counts", 50, 0, P_upperLim);
        HistoList_raw.push_back(h_truth_P_n_1e_cut);
        h_reco_theta_n_1e_cut = new TH1D("reco_theta_n_1e_cut", "#theta^{reco}_{n} in 1e cut;#theta^{reco}_{n} [#circ];Counts", 50, 0, 45.);
        HistoList_raw.push_back(h_reco_theta_n_1e_cut);
        h_truth_theta_n_1e_cut = new TH1D("truth_theta_n_1e_cut", "#theta^{truth}_{n} in 1e cut;#theta^{truth}_{n} [#circ];Counts", 50, 0, 45.);
        HistoList_raw.push_back(h_truth_theta_n_1e_cut);
        h_reco_phi_n_1e_cut = new TH1D("reco_phi_n_1e_cut", "#phi^{reco}_{n} in 1e cut;#phi^{reco}_{n} [#circ];Counts", 50, -180, 180.);
        HistoList_raw.push_back(h_reco_phi_n_1e_cut);
        h_truth_phi_n_1e_cut = new TH1D("truth_phi_n_1e_cut", "#phi^{truth}_{n} in 1e cut;#phi^{truth}_{n} [#circ];Counts", 50, -180, 180.);
        HistoList_raw.push_back(h_truth_phi_n_1e_cut);
        h_reco_theta_n_VS_reco_phi_n_1e_cut =
            new TH2D("reco_theta_n_VS_reco_phi_n_1e_cut", "#theta^{reco}_{n} vs. #phi^{reco}_{n} in 1e cut;#phi^{reco}_{n} [#circ];#theta^{reco}_{n} [#circ]", 100, -180., 180., 100, 0, 50.);
        HistoList_raw.push_back(h_reco_theta_n_VS_reco_phi_n_1e_cut);
        h_truth_theta_n_VS_truth_phi_n_1e_cut = new TH2D(
            "truth_theta_n_VS_truth_phi_n_1e_cut", "#theta^{truth}_{n} vs. #phi^{truth}_{n} in 1e cut;#phi^{truth}_{n} [#circ];#theta^{truth}_{n} [#circ]", 100, -180., 180., 100, 0, 50.);
        HistoList_raw.push_back(h_truth_theta_n_VS_truth_phi_n_1e_cut);
    }

    // =========================
    // FILLER FUNCTIONS
    // =========================
    void FillRecoElectronKinematics(const TVector3& reco_P_e, double weight) {
        const double pre = reco_P_e.Mag();
        const double tre = reco_P_e.Theta() * 180.0 / M_PI;
        const double phre = reco_P_e.Phi() * 180.0 / M_PI;

        h_reco_P_e_1e_cut->Fill(pre, weight);
        h_reco_theta_e_1e_cut->Fill(tre, weight);
        h_reco_phi_e_1e_cut->Fill(phre, weight);
        h_reco_theta_e_VS_reco_phi_e_1e_cut->Fill(phre, tre, weight);
    }

    void FillTruthElectronKinematics(const TVector3& truth_P_e, double weight) {
        const double ptr = truth_P_e.Mag();
        const double ttr = truth_P_e.Theta() * 180.0 / M_PI;
        const double phtr = truth_P_e.Phi() * 180.0 / M_PI;

        h_truth_P_e_1e_cut->Fill(ptr, weight);
        h_truth_theta_e_1e_cut->Fill(ttr, weight);
        h_truth_phi_e_1e_cut->Fill(phtr, weight);
        h_truth_theta_e_VS_truth_phi_e_1e_cut->Fill(phtr, ttr, weight);
    }

    void FillRecoNeutronKinematics(const TVector3& reco_P_n, double weight) {
        const double pre = reco_P_n.Mag();
        const double tre = reco_P_n.Theta() * 180.0 / M_PI;
        const double phre = reco_P_n.Phi() * 180.0 / M_PI;

        h_reco_P_n_1e_cut->Fill(pre, weight);
        h_reco_theta_n_1e_cut->Fill(tre, weight);
        h_reco_phi_n_1e_cut->Fill(phre, weight);
        h_reco_theta_n_VS_reco_phi_n_1e_cut->Fill(phre, tre, weight);
    }

    void FillTruthNeutronKinematics(const TVector3& truth_P_n, double weight) {
        const double ptr = truth_P_n.Mag();
        const double ttr = truth_P_n.Theta() * 180.0 / M_PI;
        const double phtr = truth_P_n.Phi() * 180.0 / M_PI;

        h_truth_P_n_1e_cut->Fill(ptr, weight);
        h_truth_theta_n_1e_cut->Fill(ttr, weight);
        h_truth_phi_n_1e_cut->Fill(phtr, weight);
        h_truth_theta_n_VS_truth_phi_n_1e_cut->Fill(phtr, ttr, weight);
    }
};
