#include <string>

#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"

struct TruthLevel_histograms {
    // =========================
    // Neutron FD Truth Histograms 1e cut
    // =========================
    TH1D* h_truth_P_nFD_truth_1e_cut;
    TH1D* h_truth_theta_nFD_truth_1e_cut;
    TH1D* h_truth_phi_nFD_truth_1e_cut;

    TH2D* h_truth_theta_nFD_VS_truth_phi_nFD_truth_1e_cut;
    TH2D* h_truth_theta_nFD_VS_truth_P_nFD_truth_1e_cut;

    // =======================
    // Constructor
    // =======================
    TruthLevel_histograms(std::vector<std::string>& HistSubjects, std::vector<std::string>& HistSubjects2, std::vector<bool>& FirstPrint, std::vector<TH1*>& HistoList_truth_1e_cut,
                            const double& P_upperLim) {
        HistSubjects.push_back("Truth 1e cut");
        HistSubjects2.push_back("Truth 1e cut");
        FirstPrint.push_back(true);

        h_truth_P_nFD_truth_1e_cut = new TH1D("truth_P_nFD_truth_1e_cut", "P^{truth}_{nFD} (truth 1e cut);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
        HistoList_truth_1e_cut.push_back(h_truth_P_nFD_truth_1e_cut);
        h_truth_theta_nFD_truth_1e_cut = new TH1D("truth_theta_nFD_truth_1e_cut", "#theta^{truth}_{nFD} (truth 1e cut);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
        HistoList_truth_1e_cut.push_back(h_truth_theta_nFD_truth_1e_cut);
        h_truth_phi_nFD_truth_1e_cut = new TH1D("truth_phi_nFD_truth_1e_cut", "#phi^{truth}_{nFD} (truth 1e cut);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
        HistoList_truth_1e_cut.push_back(h_truth_phi_nFD_truth_1e_cut);
        h_truth_theta_nFD_VS_truth_phi_nFD_truth_1e_cut =
            new TH2D("truth_theta_nFD_VS_truth_phi_nFD_truth_1e_cut", "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} (truth 1e cut);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]",
                     100, -180., 180., 100, 0, 50.);
        HistoList_truth_1e_cut.push_back(h_truth_theta_nFD_VS_truth_phi_nFD_truth_1e_cut);
        h_truth_theta_nFD_VS_truth_P_nFD_truth_1e_cut =
            new TH2D("truth_theta_nFD_VS_truth_P_nFD_truth_1e_cut", "#theta^{truth}_{nFD} vs. P^{truth}_{nFD} (truth 1e cut);P^{truth}_{nFD} [GeV/c];#theta^{truth}_{nFD} [#circ]", 100, 0,
                     P_upperLim, 100, 0, 50.);
        HistoList_truth_1e_cut.push_back(h_truth_theta_nFD_VS_truth_P_nFD_truth_1e_cut);
    }

    // =========================
    // FILLER FUNCTIONS
    // =========================
    void Fill(const TVector3& truth_P_n, const double& weight) {
        const double p = truth_P_n.Mag();
        const double theta = truth_P_n.Theta() * 180.0 / M_PI;
        const double phi = truth_P_n.Phi() * 180.0 / M_PI;

        // =========================
        // Neutron FD Truth Histograms 1e cut
        // =========================
        h_truth_P_nFD_truth_1e_cut->Fill(p, weight);
        h_truth_theta_nFD_truth_1e_cut->Fill(theta, weight);
        h_truth_phi_nFD_truth_1e_cut->Fill(phi, weight);
        h_truth_theta_nFD_VS_truth_phi_nFD_truth_1e_cut->Fill(phi, theta, weight);
        h_truth_theta_nFD_VS_truth_P_nFD_truth_1e_cut->Fill(p, theta, weight);

        // // ECAL veto
        // h_truth_P_nFD_ECALveto_1e_cut->Fill(p, weight);
        // h_truth_theta_nFD_ECALveto_1e_cut->Fill(theta, weight);
        // h_truth_phi_nFD_ECALveto_1e_cut->Fill(phi, weight);
        // h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut->Fill(phi, theta, weight);
    }
};
