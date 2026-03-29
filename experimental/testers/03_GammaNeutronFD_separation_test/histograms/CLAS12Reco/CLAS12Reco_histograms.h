#pragma once

#include <cmath>
#include <string>
#include <vector>

#include "TH1D.h"
#include "TH2D.h"
#include "TObject.h"
#include "TVector3.h"

struct CLAS12Reco_histograms {
    TH1D *h_reco_P_nFD_clas12_1e_cut, *h_truth_P_nFD_clas12_1e_cut;

    TH1D *h_reco_theta_nFD_clas12_1e_cut, *h_truth_theta_nFD_clas12_1e_cut;

    TH1D *h_reco_phi_nFD_clas12_1e_cut, *h_truth_phi_nFD_clas12_1e_cut;

    TH2D *h_reco_theta_nFD_VS_reco_phi_nFD_clas12_1e_cut, *h_truth_theta_nFD_VS_truth_phi_nFD_clas12_1e_cut;

    TH2D *h_reco_P_e_VS_P_nFD_clas12_1e_cut, *h_reco_theta_nFD_VS_P_nFD_clas12_1e_cut;

    // =======================
    // Constructor
    // =======================
    CLAS12Reco_histograms(std::vector<std::string>& HistSubjects, std::vector<std::string>& HistSubjects2, std::vector<bool>& FirstPrint, std::vector<TH1*>& HistoList_clas12reco,
                          const double& P_upperLim, const double& Ebeam) {
        HistSubjects.push_back("clas12reco");
        HistSubjects2.push_back("#splitline{FD neutron from}{clas12reco}");
        FirstPrint.push_back(true);

        h_reco_P_nFD_clas12_1e_cut = new TH1D("reco_P_nFD_clas12_1e_cut", "P^{reco}_{nFD} in 1e cut (clas12reco);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
        HistoList_clas12reco.push_back(h_reco_P_nFD_clas12_1e_cut);
        h_truth_P_nFD_clas12_1e_cut = new TH1D("truth_P_nFD_clas12_1e_cut", "P^{truth}_{nFD} in 1e cut (clas12reco);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
        HistoList_clas12reco.push_back(h_truth_P_nFD_clas12_1e_cut);

        h_reco_theta_nFD_clas12_1e_cut = new TH1D("reco_theta_nFD_clas12_1e_cut", "#theta^{reco}_{nFD} in 1e cut (clas12reco);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
        HistoList_clas12reco.push_back(h_reco_theta_nFD_clas12_1e_cut);
        h_truth_theta_nFD_clas12_1e_cut = new TH1D("truth_theta_nFD_clas12_1e_cut", "#theta^{truth}_{nFD} in 1e cut (clas12reco);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
        HistoList_clas12reco.push_back(h_truth_theta_nFD_clas12_1e_cut);

        h_reco_phi_nFD_clas12_1e_cut = new TH1D("reco_phi_nFD_clas12_1e_cut", "#phi^{reco}_{nFD} in 1e cut (clas12reco);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
        HistoList_clas12reco.push_back(h_reco_phi_nFD_clas12_1e_cut);
        h_truth_phi_nFD_clas12_1e_cut = new TH1D("truth_phi_nFD_clas12_1e_cut", "#phi^{truth}_{nFD} in 1e cut (clas12reco);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
        HistoList_clas12reco.push_back(h_truth_phi_nFD_clas12_1e_cut);

        h_reco_theta_nFD_VS_reco_phi_nFD_clas12_1e_cut =
            new TH2D("reco_theta_nFD_VS_reco_phi_nFD_clas12_1e_cut", "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (clas12reco);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]",
                     100, -180., 180., 100, 0, 50.);
        HistoList_clas12reco.push_back(h_reco_theta_nFD_VS_reco_phi_nFD_clas12_1e_cut);
        h_truth_theta_nFD_VS_truth_phi_nFD_clas12_1e_cut =
            new TH2D("truth_theta_nFD_VS_truth_phi_nFD_clas12_1e_cut",
                     "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (clas12reco);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);

        HistoList_clas12reco.push_back(h_truth_theta_nFD_VS_truth_phi_nFD_clas12_1e_cut);
        h_reco_P_e_VS_P_nFD_clas12_1e_cut = new TH2D("reco_P_e_VS_P_nFD_clas12_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (clas12reco);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]",
                                                     100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
        HistoList_clas12reco.push_back(h_reco_P_e_VS_P_nFD_clas12_1e_cut);
        h_reco_theta_nFD_VS_P_nFD_clas12_1e_cut =
            new TH2D("reco_theta_nFD_VS_P_nFD_clas12_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (clas12reco);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0.,
                     50., 100, 0., Ebeam * 3.);
        HistoList_clas12reco.push_back(h_reco_theta_nFD_VS_P_nFD_clas12_1e_cut);
    }

    // =========================
    // FILLER FUNCTIONS
    // =========================
    void FillRecoNeutronHistograms(const TVector3& reco_P_nFD, const TVector3& reco_P_e, double weight) {
        const double pn_reco = reco_P_nFD.Mag();
        const double thn_reco = reco_P_nFD.Theta() * 180.0 / M_PI;
        const double phn_reco = reco_P_nFD.Phi() * 180.0 / M_PI;

        h_reco_P_nFD_clas12_1e_cut->Fill(pn_reco, weight);
        h_reco_theta_nFD_clas12_1e_cut->Fill(thn_reco, weight);
        h_reco_phi_nFD_clas12_1e_cut->Fill(phn_reco, weight);
        h_reco_theta_nFD_VS_reco_phi_nFD_clas12_1e_cut->Fill(phn_reco, thn_reco, weight);
        h_reco_P_e_VS_P_nFD_clas12_1e_cut->Fill(reco_P_e.Mag(), pn_reco, weight);
        h_reco_theta_nFD_VS_P_nFD_clas12_1e_cut->Fill(thn_reco, pn_reco, weight);
    }

    void FillTruthNeutronHistograms(const TVector3& truth_P_nFD, double weight) {
        const double pn_truth = truth_P_nFD.Mag();
        const double thn_truth = truth_P_nFD.Theta() * 180.0 / M_PI;
        const double phn_truth = truth_P_nFD.Phi() * 180.0 / M_PI;

        h_truth_P_nFD_clas12_1e_cut->Fill(pn_truth, weight);
        h_truth_theta_nFD_clas12_1e_cut->Fill(thn_truth, weight);
        h_truth_phi_nFD_clas12_1e_cut->Fill(phn_truth, weight);
        h_truth_theta_nFD_VS_truth_phi_nFD_clas12_1e_cut->Fill(phn_truth, thn_truth, weight);
    }
};
