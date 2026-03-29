#pragma once

// Put this somewhere shared (e.g. header or top of your .cpp)
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "TH1D.h"
#include "TH2D.h"
#include "TObject.h"

// Include libraries:
#include "../../../../framework/classes/NeutronVetoRon/NeutralHit.h"
#include "../../../../framework/namespaces/general_utilities/utilities.h"
#include "../../variables/NeutronSelection_variables.h"
#include "HistContext.h"

namespace hf = histogram_functions;

struct NeutronSelectionBase_histograms {
    HistContext Context;

    // =======================
    // Electron histograms
    // =======================
    TH1D *h_reco_P_e, *h_truth_P_e;
    TH1D *h_reco_theta_e, *h_truth_theta_e;
    TH1D *h_reco_phi_e, *h_truth_phi_e;

    TH2D* h_reco_theta_e_VS_reco_phi_e;
    TH2D* h_truth_theta_e_VS_truth_phi_e;

    // =======================
    // nFD basic histograms
    // =======================
    TH1D* h_reco_P_nFD;

    // Sub-detector specific histograms according to clas12particle_hit_in_PCAL, clas12particle_hit_in_ECIN, clas12particle_hit_in_ECOUT:
    TH1D* h_reco_P_nFD_bool_PCAL_test_only;
    TH1D* h_reco_P_nFD_bool_ECIN_test_only;
    TH1D* h_reco_P_nFD_bool_ECOUT_test_only;

    // Sub-detector specific histograms according to clas12detlayer:
    TH1D* h_reco_P_nFD_PCAL_only_clas12detlayer;
    TH1D* h_reco_P_nFD_ECIN_only_clas12detlayer;
    TH1D* h_reco_P_nFD_ECOUT_only_clas12detlayer;

    TH1D* h_truth_P_nFD;
    TH1D* h_reco_theta_nFD;
    TH1D* h_truth_theta_nFD;
    TH1D* h_reco_phi_nFD;
    TH1D* h_truth_phi_nFD;

    TH2D* h_reco_theta_nFD_VS_reco_phi_nFD;
    TH2D* h_truth_theta_nFD_VS_truth_phi_nFD;
    TH2D* h_truth_P_nFD_VS_reco_P_nFD;
    TH2D* h_truth_theta_nFD_VS_reco_theta_nFD;
    TH2D* h_truth_phi_nFD_VS_reco_phi_nFD;

    // =======================
    // FD photons
    // =======================
    TH1D* h_reco_P_phFD_BPID;
    TH1D* h_reco_P_phFD_APID;

    // =======================
    // ECAL energy & beta
    // =======================
    TH1D* h_Edep_ECAL_nFD;
    TH1D* h_beta_nFD;
    TH2D* h_Edep_ECAL_nFD_VS_beta_nFD;

    // =======================
    // Hit correlations
    // =======================
    TH2D* h_nFD_PCAL_hits_VS_nFD_ECIN_hits;
    TH2D* h_nFD_PCAL_hits_VS_nFD_ECOUT_hits;
    TH2D* h_nFD_ECIN_hits_VS_nFD_ECOUT_hits;

    // =======================
    // e–nFD correlations
    // =======================
    TH2D* h_reco_P_e_VS_P_nFD;
    TH2D* h_reco_theta_e_VS_P_nFD;
    TH2D* h_reco_phi_e_VS_P_nFD;
    TH2D* h_reco_P_e_VS_theta_nFD;
    TH2D* h_reco_theta_e_VS_theta_nFD;
    TH2D* h_reco_phi_e_VS_theta_nFD;
    TH2D* h_reco_P_e_VS_phi_nFD;
    TH2D* h_reco_theta_e_VS_phi_nFD;
    TH2D* h_reco_phi_e_VS_phi_nFD;
    TH2D* h_reco_theta_nFD_VS_P_nFD;

    // =======================
    // Deposited energy
    // =======================
    TH2D* h_Edep_ECAL_VS_reco_P_nFD;
    TH2D* h_Edep_ECAL_VS_reco_dPhi_nFD_e;

    // =======================
    // Neutron beta correlations
    // =======================
    TH2D* h_beta_n_VS_reco_P_nFD;
    TH2D* h_beta_n_VS_reco_dPhi_nFD_e;

    // =======================
    // Multiplicities
    // =======================
    TH1D* h_reco_nFD_multi_AllN;
    TH1D* h_reco_nFD_multi_OnlyGood;
    TH1D* h_reco_nFD_multi_OnlyBad;
    TH2D* h_reco_nFD_multi_VS_reco_P_nFD;

    TH1D* h_reco_nFD_multi_AllN_PassMatch;
    TH1D* h_reco_nFD_multi_OnlyGood_PassMatch;
    TH1D* h_reco_nFD_multi_OnlyBad_PassMatch;
    TH2D* h_reco_nFD_multi_VS_reco_P_nFD_PassMatch;

    TH1D* h_reco_nFD_multi_AllN_NotPassMatch;
    TH1D* h_reco_nFD_multi_OnlyGood_NotPassMatch;
    TH1D* h_reco_nFD_multi_OnlyBad_NotPassMatch;
    TH2D* h_reco_nFD_multi_VS_reco_P_nFD_NotPassMatch;

    // =======================
    // Constructor
    // =======================
    NeutronSelectionBase_histograms(std::vector<TH1*>& HistoList, const double& P_upperLim, const double& Ebeam, const HistContext& ctx) : Context(ctx) {
        // =======================
        // Electron histograms
        // =======================
        if (Context.nTag == "nFD") {
            h_reco_P_e = new TH1D(("reco_P_e_" + Context.selection + "_1e_cut").c_str(), ("P^{reco}_{e} in 1e cut (" + Context.selection + ");P^{reco}_{e} [GeV/c];Counts").c_str(), 50, 0,
                                  P_upperLim);
            HistoList.push_back(h_reco_P_e);
            h_truth_P_e = new TH1D(("truth_P_e_" + Context.selection + "_1e_cut").c_str(), ("P^{truth}_{e} in 1e cut (" + Context.selection + ");P^{truth}_{e} [GeV/c];Counts").c_str(), 50,
                                   0, P_upperLim);
            HistoList.push_back(h_truth_P_e);
            h_reco_theta_e = new TH1D(("reco_theta_e_" + Context.selection + "_1e_cut").c_str(),
                                      ("#theta^{reco}_{e} in 1e cut (" + Context.selection + ");#theta^{reco}_{e} [#circ];Counts").c_str(), 50, 0, 45.);
            HistoList.push_back(h_reco_theta_e);
            h_truth_theta_e = new TH1D(("truth_theta_e_" + Context.selection + "_1e_cut").c_str(),
                                       ("#theta^{truth}_{e} in 1e cut (" + Context.selection + ");#theta^{truth}_{e} [#circ];Counts").c_str(), 50, 0, 45.);
            HistoList.push_back(h_truth_theta_e);
            h_reco_phi_e = new TH1D(("reco_phi_e_" + Context.selection + "_1e_cut").c_str(), ("#phi^{reco}_{e} in 1e cut (" + Context.selection + ");#phi^{reco}_{e} [#circ];Counts").c_str(),
                                    50, -180, 180.);
            HistoList.push_back(h_reco_phi_e);
            h_truth_phi_e = new TH1D(("truth_phi_e_" + Context.selection + "_1e_cut").c_str(),
                                     ("#phi^{truth}_{e} in 1e cut (" + Context.selection + ");#phi^{truth}_{e} [#circ];Counts").c_str(), 50, -180, 180.);
            HistoList.push_back(h_truth_phi_e);

            h_reco_theta_e_VS_reco_phi_e = new TH2D(("reco_theta_e_VS_reco_phi_e_" + Context.selection + "_1e_cut").c_str(),
                                                    ("#theta^{reco}_{e} vs. #phi^{reco}_{e} in 1e cut (" + Context.selection + ");#phi^{reco}_{e} [#circ];#theta^{reco}_{e} [#circ]").c_str(),
                                                    100, -180., 180., 100, 0, 50.);
            HistoList.push_back(h_reco_theta_e_VS_reco_phi_e);
            h_truth_theta_e_VS_truth_phi_e = new TH2D(
                ("truth_theta_e_VS_truth_phi_e_" + Context.selection + "_1e_cut").c_str(),
                ("#theta^{truth}_{e} vs. #phi^{truth}_{e} in 1e cut (" + Context.selection + ");#phi^{truth}_{e} [#circ];#theta^{truth}_{e} [#circ]").c_str(), 100, -180., 180., 100, 0, 50.);
            HistoList.push_back(h_truth_theta_e_VS_truth_phi_e);
        }

        // =======================
        // nFD basic histograms
        // =======================
        h_reco_P_nFD = new TH1D(("reco_P_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                ("P^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");P^{reco}_{" + Context.nTag + "} [GeV/c];Counts").c_str(), 50, 0, P_upperLim);
        HistoList.push_back(h_reco_P_nFD);

        // Sub-detector specific histograms according to clas12particle_hit_in_PCAL, clas12particle_hit_in_ECIN, clas12particle_hit_in_ECOUT:
        h_reco_P_nFD_bool_PCAL_test_only = new TH1D(
            ("reco_P_" + Context.nTag + "_bool_PCAL_test_only_" + Context.selection + "_1e_cut").c_str(),
            ("P^{reco}_{" + Context.nTag + "} in 1e cut, bool PCAL test only (" + Context.selection + ");P^{reco}_{" + Context.nTag + "} [GeV/c];Counts").c_str(), 50, 0, P_upperLim);
        HistoList.push_back(h_reco_P_nFD_bool_PCAL_test_only);
        h_reco_P_nFD_bool_ECIN_test_only = new TH1D(
            ("reco_P_" + Context.nTag + "_bool_ECIN_test_only_" + Context.selection + "_1e_cut").c_str(),
            ("P^{reco}_{" + Context.nTag + "} in 1e cut, bool ECIN test only (" + Context.selection + ");P^{reco}_{" + Context.nTag + "} [GeV/c];Counts").c_str(), 50, 0, P_upperLim);
        HistoList.push_back(h_reco_P_nFD_bool_ECIN_test_only);
        h_reco_P_nFD_bool_ECOUT_test_only = new TH1D(
            ("reco_P_" + Context.nTag + "_bool_ECOUT_test_only_" + Context.selection + "_1e_cut").c_str(),
            ("P^{reco}_{" + Context.nTag + "} in 1e cut, bool ECOUT test only (" + Context.selection + ");P^{reco}_{" + Context.nTag + "} [GeV/c];Counts").c_str(), 50, 0, P_upperLim);
        HistoList.push_back(h_reco_P_nFD_bool_ECOUT_test_only);

        // Sub-detector specific histograms according to clas12detlayer:
        h_reco_P_nFD_PCAL_only_clas12detlayer =
            new TH1D(("reco_P_" + Context.nTag + "_PCAL_only_clas12detlayer_" + Context.selection + "_1e_cut").c_str(),
                     ("P^{reco}_{" + Context.nTag + "} in 1e cut, PCAL only with clas12detlayer (" + Context.selection + ");P^{reco}_{" + Context.nTag + "} [GeV/c];Counts").c_str(), 50, 0,
                     P_upperLim);
        HistoList.push_back(h_reco_P_nFD_PCAL_only_clas12detlayer);
        h_reco_P_nFD_ECIN_only_clas12detlayer =
            new TH1D(("reco_P_" + Context.nTag + "_ECIN_only_clas12detlayer_" + Context.selection + "_1e_cut").c_str(),
                     ("P^{reco}_{" + Context.nTag + "} in 1e cut, ECIN only with clas12detlayer (" + Context.selection + ");P^{reco}_{" + Context.nTag + "} [GeV/c];Counts").c_str(), 50, 0,
                     P_upperLim);
        HistoList.push_back(h_reco_P_nFD_ECIN_only_clas12detlayer);
        h_reco_P_nFD_ECOUT_only_clas12detlayer =
            new TH1D(("reco_P_" + Context.nTag + "_ECOUT_only_clas12detlayer_" + Context.selection + "_1e_cut").c_str(),
                     ("P^{reco}_{" + Context.nTag + "} in 1e cut, ECOUT only with clas12detlayer (" + Context.selection + ");P^{reco}_{" + Context.nTag + "} [GeV/c];Counts").c_str(), 50, 0,
                     P_upperLim);
        HistoList.push_back(h_reco_P_nFD_ECOUT_only_clas12detlayer);

        // if (Context.nTag == "nFD") {
        h_truth_P_nFD = new TH1D(("truth_P_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                 ("P^{truth}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");P^{truth}_{" + Context.nTag + "} [GeV/c];Counts").c_str(), 50, 0, P_upperLim);
        HistoList.push_back(h_truth_P_nFD);
        // }

        h_reco_theta_nFD = new TH1D(("reco_theta_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                    ("#theta^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#theta^{reco}_{" + Context.nTag + "} [#circ];Counts").c_str(), 50, 0, 45.);
        HistoList.push_back(h_reco_theta_nFD);

        // if (Context.nTag == "nFD") {
        h_truth_theta_nFD =
            new TH1D(("truth_theta_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                     ("#theta^{truth}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#theta^{truth}_{" + Context.nTag + "} [#circ];Counts").c_str(), 50, 0, 45.);
        HistoList.push_back(h_truth_theta_nFD);
        // }

        h_reco_phi_nFD = new TH1D(("reco_phi_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                  ("#phi^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#phi^{reco}_{" + Context.nTag + "} [#circ];Counts").c_str(), 50, -180, 180.);
        HistoList.push_back(h_reco_phi_nFD);

        // if (Context.nTag == "nFD") {
        h_truth_phi_nFD = new TH1D(("truth_phi_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                   ("#phi^{truth}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#phi^{truth}_{" + Context.nTag + "} [#circ];Counts").c_str(), 50, -180, 180.);
        HistoList.push_back(h_truth_phi_nFD);
        // }

        h_reco_theta_nFD_VS_reco_phi_nFD = new TH2D(("reco_theta_" + Context.nTag + "_VS_reco_phi_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                                    ("#theta^{reco}_{" + Context.nTag + "} vs. #phi^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#phi^{reco}_{" +
                                                     Context.nTag + "} [#circ];#theta^{reco}_{" + Context.nTag + "} [#circ]")
                                                        .c_str(),
                                                    100, -180., 180., 100, 0, 50.);
        HistoList.push_back(h_reco_theta_nFD_VS_reco_phi_nFD);

        // if (Context.nTag == "nFD") {
        h_truth_theta_nFD_VS_truth_phi_nFD = new TH2D(("truth_theta_" + Context.nTag + "_VS_truth_phi_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                                      ("#theta^{truth}_{" + Context.nTag + "} vs. #phi^{truth}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#phi^{truth}_{" +
                                                       Context.nTag + "} [#circ];#theta^{truth}_{" + Context.nTag + "} [#circ]")
                                                          .c_str(),
                                                      100, -180., 180., 100, 0, 50.);
        HistoList.push_back(h_truth_theta_nFD_VS_truth_phi_nFD);
        h_truth_P_nFD_VS_reco_P_nFD = new TH2D(("truth_P_" + Context.nTag + "_VS_reco_P_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                               ("P^{truth}_{" + Context.nTag + "} vs. P^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");P^{truth}_{" + Context.nTag +
                                                "} [GeV/c];P^{reco}_{" + Context.nTag + "} [GeV/c]")
                                                   .c_str(),
                                               100, -180., 180., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_truth_P_nFD_VS_reco_P_nFD);
        h_truth_theta_nFD_VS_reco_theta_nFD = new TH2D(("truth_theta_" + Context.nTag + "_VS_reco_theta_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                                       ("#theta^{truth}_{" + Context.nTag + "} vs. #theta^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection +
                                                        ");#theta^{truth}_{" + Context.nTag + "} [#circ];#theta^{reco}_{" + Context.nTag + "} [#circ]")
                                                           .c_str(),
                                                       100, 0, 50., 100, 0, 50.);
        HistoList.push_back(h_truth_theta_nFD_VS_reco_theta_nFD);
        h_truth_phi_nFD_VS_reco_phi_nFD = new TH2D(("truth_phi_" + Context.nTag + "_VS_reco_phi_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                                   ("#phi^{truth}_{" + Context.nTag + "} vs. #phi^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#phi^{truth}_{" +
                                                    Context.nTag + "} [#circ];#phi^{reco}_{" + Context.nTag + "} [#circ]")
                                                       .c_str(),
                                                   100, -180., 180., 100, -180., 180.);
        HistoList.push_back(h_truth_phi_nFD_VS_reco_phi_nFD);
        // }

        // =======================
        // FD photons
        // =======================
        if (Context.nTag == "nFD") {
            h_reco_P_phFD_BPID = new TH1D(("reco_P_phFD_BPID_" + Context.selection + "_1e_cut").c_str(),
                                          ("P^{reco}_{#gammaFD} in 1e cut BPID (" + Context.selection + ");P^{reco}_{#gammaFD} [GeV/c];Counts").c_str(), 50, 0, P_upperLim);
            HistoList.push_back(h_reco_P_phFD_BPID);
            h_reco_P_phFD_APID = new TH1D(("reco_P_phFD_APID_" + Context.selection + "_1e_cut").c_str(),
                                          ("P^{reco}_{#gammaFD} in 1e cut APID (" + Context.selection + ");P^{reco}_{#gammaFD} [GeV/c];Counts").c_str(), 50, 0, P_upperLim);
            HistoList.push_back(h_reco_P_phFD_APID);
        }

        // =======================
        // ECAL energy & beta
        // =======================
        h_Edep_ECAL_nFD = new TH1D(("Edep_ECAL_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                   ("E^{ECAL}_{dep} in 1e cut (" + Context.selection + ");E^{ECAL}_{dep} [GeV];Counts").c_str(), 50, 0., 1.5);
        HistoList.push_back(h_Edep_ECAL_nFD);
        h_beta_nFD = new TH1D(("beta_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                              ("#beta_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#beta_{" + Context.nTag + "};Counts").c_str(), 50, -0.25, 1.5);
        HistoList.push_back(h_beta_nFD);
        h_Edep_ECAL_nFD_VS_beta_nFD =
            new TH2D(("Edep_ECAL_" + Context.nTag + "_VS_beta_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                     ("E^{ECAL}_{dep} vs. #beta_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#beta_{" + Context.nTag + "};E^{ECAL}_{dep} [GeV]").c_str(), 100, -0.25, 1.5,
                     100, 0., 1.5);
        HistoList.push_back(h_Edep_ECAL_nFD_VS_beta_nFD);

        // =======================
        // Hit correlations
        // =======================
        h_nFD_PCAL_hits_VS_nFD_ECIN_hits = new TH2D(("" + Context.nTag + "_PCAL_hits_VS_" + Context.nTag + "_ECIN_hits_" + Context.selection + "_1e_cut").c_str(),
                                                    ("#(" + Context.nTag + " hits in PCAL) vs. #(" + Context.nTag + " hits in ECIN) in 1e cut (" + Context.selection + ");#(" + Context.nTag +
                                                     " hits in PCAL);#(" + Context.nTag + " hits in ECIN)")
                                                        .c_str(),
                                                    2, -0.25, 1.5, 2, -0.25, 1.5);
        HistoList.push_back(h_nFD_PCAL_hits_VS_nFD_ECIN_hits);
        h_nFD_PCAL_hits_VS_nFD_ECOUT_hits = new TH2D(("" + Context.nTag + "_PCAL_hits_VS_" + Context.nTag + "_ECOUT_hits_" + Context.selection + "_1e_cut").c_str(),
                                                     ("#(" + Context.nTag + " hits in PCAL) vs. #(" + Context.nTag + " hits in ECOUT) in 1e cut (" + Context.selection + ");#(" +
                                                      Context.nTag + " hits in PCAL);#(" + Context.nTag + " hits in ECOUT)")
                                                         .c_str(),
                                                     2, -0.25, 1.5, 2, -0.25, 1.5);
        HistoList.push_back(h_nFD_PCAL_hits_VS_nFD_ECOUT_hits);
        h_nFD_ECIN_hits_VS_nFD_ECOUT_hits = new TH2D(("" + Context.nTag + "_ECIN_hits_VS_" + Context.nTag + "_ECOUT_hits_" + Context.selection + "_1e_cut").c_str(),
                                                     ("#(" + Context.nTag + " hits in ECIN) vs. #(" + Context.nTag + " hits in ECOUT) in 1e cut (" + Context.selection + ");#(" +
                                                      Context.nTag + " hits in ECIN);#(" + Context.nTag + " hits in ECOUT)")
                                                         .c_str(),
                                                     2, -0.25, 1.5, 2, -0.25, 1.5);
        HistoList.push_back(h_nFD_ECIN_hits_VS_nFD_ECOUT_hits);

        // =======================
        // e–nFD correlations
        // =======================
        h_reco_P_e_VS_P_nFD =
            new TH2D(("reco_P_e_VS_P_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                     ("P^{reco}_{e} vs. P^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");P^{reco}_{e} [GeV/c];P^{reco}_{" + Context.nTag + "} [GeV/c]").c_str(), 100, 0.,
                     Ebeam * 2., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_P_e_VS_P_nFD);
        h_reco_theta_e_VS_P_nFD = new TH2D(
            ("reco_theta_e_VS_P_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
            ("#theta^{reco}_{e} vs. P^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#theta^{reco}_{e} [#circ];P^{reco}_{" + Context.nTag + "} [GeV/c]").c_str(), 100, 0,
            45., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_theta_e_VS_P_nFD);
        h_reco_phi_e_VS_P_nFD =
            new TH2D(("reco_phi_e_VS_P_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                     ("#phi^{reco}_{e} vs. P^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#phi^{reco}_{e} [#circ];P^{reco}_{" + Context.nTag + "} [GeV/c]").c_str(),
                     100, -180., 180., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_phi_e_VS_P_nFD);
        h_reco_P_e_VS_theta_nFD = new TH2D(
            ("reco_P_e_VS_theta_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
            ("P^{reco}_{e} vs. #theta^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");P^{reco}_{e} [GeV/c];#theta^{reco}_{" + Context.nTag + "} [#circ]").c_str(), 100,
            0., P_upperLim, 100, 0, 45.);
        HistoList.push_back(h_reco_P_e_VS_theta_nFD);
        h_reco_theta_e_VS_theta_nFD = new TH2D(
            ("reco_theta_e_VS_theta_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
            ("#theta^{reco}_{e} vs. #theta^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#theta^{reco}_{e} [#circ];#theta^{reco}_{" + Context.nTag + "} [#circ]")
                .c_str(),
            100, 0, 45., 100, 0, 45.);
        HistoList.push_back(h_reco_theta_e_VS_theta_nFD);
        h_reco_phi_e_VS_theta_nFD = new TH2D(
            ("reco_phi_e_VS_theta_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
            ("#phi^{reco}_{e} vs. #theta^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#phi^{reco}_{e} [#circ];#theta^{reco}_{" + Context.nTag + "} [#circ]").c_str(),
            100, -180., 180., 100, 0, 45.);
        HistoList.push_back(h_reco_phi_e_VS_theta_nFD);
        h_reco_P_e_VS_phi_nFD =
            new TH2D(("reco_P_e_VS_phi_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                     ("P^{reco}_{e} vs. #phi^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");P^{reco}_{e} [GeV/c];#phi^{reco}_{" + Context.nTag + "} [#circ]").c_str(),
                     100, 0., P_upperLim, 100, -180., 180.);
        HistoList.push_back(h_reco_P_e_VS_phi_nFD);
        h_reco_theta_e_VS_phi_nFD = new TH2D(
            ("reco_theta_e_VS_phi_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
            ("#theta^{reco}_{e} vs. #phi^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#theta^{reco}_{e} [#circ];#phi^{reco}_{" + Context.nTag + "} [#circ]").c_str(),
            100, 0, 45., 100, -180., 180.);
        HistoList.push_back(h_reco_theta_e_VS_phi_nFD);
        h_reco_phi_e_VS_phi_nFD = new TH2D(
            ("reco_phi_e_VS_phi_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
            ("#phi^{reco}_{e} vs. #phi^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#phi^{reco}_{e} [#circ];#phi^{reco}_{" + Context.nTag + "} [#circ]").c_str(), 100,
            -180., 180., 100, -180., 180.);
        HistoList.push_back(h_reco_phi_e_VS_phi_nFD);
        h_reco_theta_nFD_VS_P_nFD = new TH2D(("reco_theta_" + Context.nTag + "_VS_P_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                             ("#theta^{reco}_{" + Context.nTag + "} vs. P^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");#theta^{reco}_{" +
                                              Context.nTag + "} [#circ];P^{reco}_{" + Context.nTag + "} [GeV/c]")
                                                 .c_str(),
                                             100, 0., 45., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_theta_nFD_VS_P_nFD);

        // =======================
        // Deposited energy
        // =======================
        h_Edep_ECAL_VS_reco_P_nFD =
            new TH2D(("Edep_ECAL_VS_reco_P_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                     ("E^{ECAL}_{dep} vs. P^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");P^{reco}_{" + Context.nTag + "} [GeV/c];E^{ECAL}_{dep} [GeV]").c_str(), 100,
                     0., Ebeam * 3., 100, 0., 1.5);
        HistoList.push_back(h_Edep_ECAL_VS_reco_P_nFD);
        h_Edep_ECAL_VS_reco_dPhi_nFD_e = new TH2D(("Edep_ECAL_VS_reco_dPhi_" + Context.nTag + "_e_" + Context.selection + "_1e_cut").c_str(),
                                                  ("E^{ECAL}_{dep} vs. #delta#phi^{reco}_{" + Context.nTag + ",e} in 1e cut (" + Context.selection + ");#delta#phi^{reco}_{" + Context.nTag +
                                                   ",e} = |#phi^{reco}_{" + Context.nTag + "} - #phi^{reco}_{e}| [#circ]];E^{ECAL}_{dep} [GeV]")
                                                      .c_str(),
                                                  100, -180., 180., 100, 0., 1.5);
        HistoList.push_back(h_Edep_ECAL_VS_reco_dPhi_nFD_e);

        // =======================
        // Neutron beta correlations
        // =======================
        h_beta_n_VS_reco_P_nFD =
            new TH2D(("beta_n_VS_reco_P_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                     ("#beta_{nFD} vs. P^{reco}_{nFD} in 1e cut (" + Context.selection + ");P^{reco}_{nFD} [GeV/c];#beta_{nFD}").c_str(), 100, 0., Ebeam * 3., 100, -0.25, 1.5);
        HistoList.push_back(h_beta_n_VS_reco_P_nFD);
        h_beta_n_VS_reco_dPhi_nFD_e = new TH2D(("beta_n_VS_reco_dPhi_" + Context.nTag + "_e_" + Context.selection + "_1e_cut").c_str(),
                                               ("#beta_{nFD} vs. #delta#phi^{reco}_{" + Context.nTag + ",e} in 1e cut (" + Context.selection + ");#delta#phi^{reco}_{" + Context.nTag +
                                                ",e} = |#phi^{reco}_{" + Context.nTag + "} - #phi^{reco}_{e}| [#circ]];#beta_{nFD}")
                                                   .c_str(),
                                               100, -180., 180., 100, -0.25, 1.5);
        HistoList.push_back(h_beta_n_VS_reco_dPhi_nFD_e);

        // =======================
        // Multiplicities
        // =======================
        if (Context.nTag == "nFD") {
            h_reco_nFD_multi_AllN = new TH1D(
                ("reco_" + Context.nTag + "_multi_AllN_" + Context.selection + "_1e_cut").c_str(),
                ("reco " + Context.nTag + " multiplicity in 1e cut (all " + Context.nTag + "s, " + Context.selection + ");" + Context.nTag + " multiplicity;Counts").c_str(), 9, 1, 10);
            HistoList.push_back(h_reco_nFD_multi_AllN);
            h_reco_nFD_multi_OnlyGood = new TH1D(
                ("reco_" + Context.nTag + "_multi_OnlyGood_" + Context.selection + "_1e_cut").c_str(),
                ("reco " + Context.nTag + " multiplicity in 1e cut (Only good " + Context.nTag + ", " + Context.selection + ");Good " + Context.nTag + " multiplicity;Counts").c_str(), 9, 1,
                10);
            HistoList.push_back(h_reco_nFD_multi_OnlyGood);
            h_reco_nFD_multi_OnlyBad =
                new TH1D(("reco_" + Context.nTag + "_multi_OnlyBad_" + Context.selection + "_1e_cut").c_str(),
                         ("reco " + Context.nTag + " multiplicity in 1e cut (Only bad " + Context.nTag + ", " + Context.selection + ");Bad " + Context.nTag + " multiplicity;Counts").c_str(),
                         9, 1, 10);
            HistoList.push_back(h_reco_nFD_multi_OnlyBad);
            h_reco_nFD_multi_VS_reco_P_nFD = new TH2D(("reco_" + Context.nTag + "_multi_VS_reco_P_" + Context.nTag + "_" + Context.selection + "_1e_cut").c_str(),
                                                      ("reco " + Context.nTag + " multiplicity vs. P^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");P^{reco}_{" +
                                                       Context.nTag + "} [GeV/c];" + Context.nTag + " multiplicity;Counts")
                                                          .c_str(),
                                                      100, 0., Ebeam * 3., 9, 1, 10);
            HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD);

            if (Context.selection == "Matched") {
                h_reco_nFD_multi_AllN_PassMatch = new TH1D(
                    ("reco_" + Context.nTag + "_multi_AllN_PassMatch_" + Context.selection + "_1e_cut").c_str(),
                    ("Multiplicity of " + Context.nTag + "s not passing match cuts in 1e cut (all " + Context.nTag + "s, " + Context.selection + ");" + Context.nTag + " multiplicity;Counts")
                        .c_str(),
                    9, 1, 10);
                h_reco_nFD_multi_OnlyGood_PassMatch = new TH1D(("reco_" + Context.nTag + "_multi_OnlyGood_PassMatch_" + Context.selection + "_1e_cut").c_str(),
                                                               ("Multiplicity of " + Context.nTag + "s passing match cuts in 1e cut (Only good " + Context.nTag + ", " + Context.selection +
                                                                ");Good " + Context.nTag + " multiplicity;Counts")
                                                                   .c_str(),
                                                               9, 1, 10);
                HistoList.push_back(h_reco_nFD_multi_OnlyGood_PassMatch);
                h_reco_nFD_multi_OnlyBad_PassMatch = new TH1D(("reco_" + Context.nTag + "_multi_OnlyBad_PassMatch_" + Context.selection + "_1e_cut").c_str(),
                                                              ("Multiplicity of " + Context.nTag + "s passing match cuts in 1e cut (Only bad " + Context.nTag + ", " + Context.selection +
                                                               ");Bad " + Context.nTag + " multiplicity;Counts")
                                                                  .c_str(),
                                                              9, 1, 10);
                HistoList.push_back(h_reco_nFD_multi_OnlyBad_PassMatch);
                h_reco_nFD_multi_VS_reco_P_nFD_PassMatch = new TH2D(("reco_" + Context.nTag + "_multi_VS_reco_P_" + Context.nTag + "_PassMatch_" + Context.selection + "_1e_cut").c_str(),
                                                                    ("Multiplicity of " + Context.nTag + "s passing match cuts vs. P^{reco}_{" + Context.nTag + "} in 1e cut (" +
                                                                     Context.selection + ");P^{reco}_{" + Context.nTag + "} [GeV/c];" + Context.nTag + " multiplicity")
                                                                        .c_str(),
                                                                    100, 0., Ebeam * 3., 9, 1, 10);
                HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD_PassMatch);

                h_reco_nFD_multi_AllN_NotPassMatch = new TH1D(
                    ("reco_" + Context.nTag + "_multi_AllN_NotPassMatch_" + Context.selection + "_1e_cut").c_str(),
                    ("Multiplicity of " + Context.nTag + "s not passing match cuts in 1e cut (all " + Context.nTag + "s, " + Context.selection + ");" + Context.nTag + " multiplicity;Counts")
                        .c_str(),
                    9, 1, 10);
                HistoList.push_back(h_reco_nFD_multi_AllN_NotPassMatch);
                h_reco_nFD_multi_OnlyGood_NotPassMatch = new TH1D(("reco_" + Context.nTag + "_multi_OnlyGood_NotPassMatch_" + Context.selection + "_1e_cut").c_str(),
                                                                  ("Multiplicity of " + Context.nTag + "s not passing match cuts in 1e cut (Only good " + Context.nTag + ", " +
                                                                   Context.selection + ");Good " + Context.nTag + " multiplicity;Counts")
                                                                      .c_str(),
                                                                  9, 1, 10);
                HistoList.push_back(h_reco_nFD_multi_OnlyGood_NotPassMatch);
                h_reco_nFD_multi_OnlyBad_NotPassMatch = new TH1D(("reco_" + Context.nTag + "_multi_OnlyBad_NotPassMatch_" + Context.selection + "_1e_cut").c_str(),
                                                                 ("Multiplicity of " + Context.nTag + "s not passing match cuts in 1e cut (Only bad " + Context.nTag + ", " +
                                                                  Context.selection + ");Bad " + Context.nTag + " multiplicity;Counts")
                                                                     .c_str(),
                                                                 9, 1, 10);
                HistoList.push_back(h_reco_nFD_multi_OnlyBad_NotPassMatch);
                h_reco_nFD_multi_VS_reco_P_nFD_NotPassMatch =
                    new TH2D(("reco_" + Context.nTag + "_multi_VS_reco_P_" + Context.nTag + "_NotPassMatch_" + Context.selection + "_1e_cut").c_str(),
                             ("Multiplicity of " + Context.nTag + "s not passing match cuts vs. P^{reco}_{" + Context.nTag + "} in 1e cut (" + Context.selection + ");P^{reco}_{" +
                              Context.nTag + "} [GeV/c];" + Context.nTag + " multiplicity")
                                 .c_str(),
                             100, 0., Ebeam * 3., 9, 1, 10);
                HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD_NotPassMatch);
            }
        }
    }

    // =========================
    // FILLER FUNCTIONS
    // =========================
    void FillRecoElectronKinematics(const TVector3& reco_P_e, const double& weight) {
        if (Context.nTag == "nFD") {
            const double pre = reco_P_e.Mag();
            const double tre = reco_P_e.Theta() * 180.0 / M_PI;
            const double phre = reco_P_e.Phi() * 180.0 / M_PI;

            h_reco_P_e->Fill(pre, weight);
            h_reco_theta_e->Fill(tre, weight);
            h_reco_phi_e->Fill(phre, weight);
            h_reco_theta_e_VS_reco_phi_e->Fill(phre, tre, weight);
        }
    }

    void FillTruthElectronKinematics(const TVector3& truth_P_e, const double& weight) {
        if (Context.nTag == "nFD") {
            const double ptr = truth_P_e.Mag();
            const double ttr = truth_P_e.Theta() * 180.0 / M_PI;
            const double phtr = truth_P_e.Phi() * 180.0 / M_PI;

            h_truth_P_e->Fill(ptr, weight);
            h_truth_theta_e->Fill(ttr, weight);
            h_truth_phi_e->Fill(phtr, weight);
            h_truth_theta_e_VS_truth_phi_e->Fill(phtr, ttr, weight);
        }
    }

    void FillRecoPhotonKinematicsBPID(const double& reco_P_ph_mag, const double& weight) {
        if (Context.nTag == "nFD") { h_reco_P_phFD_BPID->Fill(reco_P_ph_mag, weight); }
    }

    void FillRecoPhotonKinematicsAPID(const double& reco_P_ph_mag, const double& weight) {
        if (Context.nTag == "nFD") { h_reco_P_phFD_APID->Fill(reco_P_ph_mag, weight); }
    }

    void FillRecoNeutronKinematics(const NeutralHit& nFD, const NeutronSelection_variables& NS_variables, const int& reco_nFD_multi, const double& weight) {
        bool ParticleInPCAL = nFD.clas12particle_hit_in_PCAL;
        bool ParticleInECIN = nFD.clas12particle_hit_in_ECIN;
        bool ParticleInECOUT = nFD.clas12particle_hit_in_ECOUT;

        const double pre = NS_variables.reco_P_nFD.Mag();
        const double tre = NS_variables.reco_P_nFD.Theta() * 180.0 / M_PI;
        const double phre = NS_variables.reco_P_nFD.Phi() * 180.0 / M_PI;

        h_reco_P_nFD->Fill(NS_variables.reco_P_nFD.Mag(), weight);

        if (ParticleInPCAL) { h_reco_P_nFD_bool_PCAL_test_only->Fill(NS_variables.reco_P_nFD.Mag(), weight); }
        if (ParticleInECIN) { h_reco_P_nFD_bool_ECIN_test_only->Fill(NS_variables.reco_P_nFD.Mag(), weight); }
        if (ParticleInECOUT) { h_reco_P_nFD_bool_ECOUT_test_only->Fill(NS_variables.reco_P_nFD.Mag(), weight); }

        if (nFD.particle_clas12detlayer == clas12::PCAL) { h_reco_P_nFD_PCAL_only_clas12detlayer->Fill(NS_variables.reco_P_nFD.Mag(), weight); }
        if (nFD.particle_clas12detlayer == clas12::ECIN) { h_reco_P_nFD_ECIN_only_clas12detlayer->Fill(NS_variables.reco_P_nFD.Mag(), weight); }
        if (nFD.particle_clas12detlayer == clas12::ECOUT) { h_reco_P_nFD_ECOUT_only_clas12detlayer->Fill(NS_variables.reco_P_nFD.Mag(), weight); }

        h_reco_theta_nFD->Fill(NS_variables.reco_P_nFD.Theta() * 180 / M_PI, weight);
        h_reco_phi_nFD->Fill(NS_variables.reco_P_nFD.Phi() * 180 / M_PI, weight);
        h_reco_theta_nFD_VS_reco_phi_nFD->Fill(NS_variables.reco_P_nFD.Phi() * 180 / M_PI, NS_variables.reco_P_nFD.Theta() * 180 / M_PI, weight);

        h_Edep_ECAL_nFD->Fill(NS_variables.Edep_ECAL_nFD, weight);
        h_beta_nFD->Fill(NS_variables.reco_Beta_nFD, weight);
        h_Edep_ECAL_nFD_VS_beta_nFD->Fill(NS_variables.reco_Beta_nFD, NS_variables.Edep_ECAL_nFD, weight);

        h_nFD_PCAL_hits_VS_nFD_ECIN_hits->Fill(NS_variables.PCAL_hit, NS_variables.ECIN_hit, weight);
        h_nFD_PCAL_hits_VS_nFD_ECOUT_hits->Fill(NS_variables.PCAL_hit, NS_variables.ECOUT_hit, weight);
        h_nFD_ECIN_hits_VS_nFD_ECOUT_hits->Fill(NS_variables.ECIN_hit, NS_variables.ECOUT_hit, weight);

        h_reco_P_e_VS_P_nFD->Fill(NS_variables.reco_P_e.Mag(), NS_variables.reco_P_nFD.Mag(), weight);
        h_reco_theta_e_VS_P_nFD->Fill(NS_variables.reco_P_e.Theta() * 180 / M_PI, NS_variables.reco_P_nFD.Mag(), weight);
        h_reco_phi_e_VS_P_nFD->Fill(NS_variables.reco_P_e.Phi() * 180 / M_PI, NS_variables.reco_P_nFD.Mag(), weight);
        h_reco_P_e_VS_theta_nFD->Fill(NS_variables.reco_P_e.Mag(), NS_variables.reco_P_nFD.Theta() * 180 / M_PI, weight);
        h_reco_theta_e_VS_theta_nFD->Fill(NS_variables.reco_P_e.Theta() * 180 / M_PI, NS_variables.reco_P_nFD.Theta() * 180 / M_PI, weight);
        h_reco_phi_e_VS_theta_nFD->Fill(NS_variables.reco_P_e.Phi() * 180 / M_PI, NS_variables.reco_P_nFD.Theta() * 180 / M_PI, weight);
        h_reco_P_e_VS_phi_nFD->Fill(NS_variables.reco_P_e.Mag(), NS_variables.reco_P_nFD.Phi() * 180 / M_PI, weight);
        h_reco_theta_e_VS_phi_nFD->Fill(NS_variables.reco_P_e.Theta() * 180 / M_PI, NS_variables.reco_P_nFD.Phi() * 180 / M_PI, weight);
        h_reco_phi_e_VS_phi_nFD->Fill(NS_variables.reco_P_e.Phi() * 180 / M_PI, NS_variables.reco_P_nFD.Phi() * 180 / M_PI, weight);
        h_reco_theta_nFD_VS_P_nFD->Fill(NS_variables.reco_P_nFD.Theta() * 180 / M_PI, NS_variables.reco_P_nFD.Mag(), weight);

        h_Edep_ECAL_VS_reco_P_nFD->Fill(NS_variables.reco_P_nFD.Mag(), NS_variables.Edep_ECAL_nFD, weight);
        h_Edep_ECAL_VS_reco_dPhi_nFD_e->Fill(NS_variables.dPhi, NS_variables.Edep_ECAL_nFD, weight);
        h_beta_n_VS_reco_P_nFD->Fill(NS_variables.reco_P_nFD.Mag(), NS_variables.reco_Beta_nFD, weight);
        h_beta_n_VS_reco_dPhi_nFD_e->Fill(NS_variables.dPhi, NS_variables.reco_Beta_nFD, weight);

        if (Context.nTag == "nFD") { h_reco_nFD_multi_VS_reco_P_nFD->Fill(NS_variables.reco_P_nFD.Mag(), reco_nFD_multi, weight); }
    }

    void FillTruthNeutronKinematics(const TVector3& truth_P_nFD, const double& weight) {
        // if (Context.nTag == "nFD") {
        const double ptr = truth_P_nFD.Mag();
        const double ttr = truth_P_nFD.Theta() * 180.0 / M_PI;
        const double phtr = truth_P_nFD.Phi() * 180.0 / M_PI;

        h_truth_P_nFD->Fill(ptr, weight);
        h_truth_theta_nFD->Fill(ttr, weight);
        h_truth_phi_nFD->Fill(phtr, weight);
        h_truth_theta_nFD_VS_truth_phi_nFD->Fill(phtr, ttr, weight);
        // }
    }

    void FillTruthNeutronKinematics2(const TVector3& truth_P_nFD, const TVector3& reco_P_nFD, const double& weight) {
        // if (Context.nTag == "nFD") {
        const double ptr = truth_P_nFD.Mag();
        const double ttr = truth_P_nFD.Theta() * 180.0 / M_PI;
        const double phtr = truth_P_nFD.Phi() * 180.0 / M_PI;

        const double pre = reco_P_nFD.Mag();
        const double tre = reco_P_nFD.Theta() * 180.0 / M_PI;
        const double phre = reco_P_nFD.Phi() * 180.0 / M_PI;

        h_truth_P_nFD_VS_reco_P_nFD->Fill(ptr, pre, weight);
        h_truth_theta_nFD_VS_reco_theta_nFD->Fill(ttr, tre, weight);
        h_truth_phi_nFD_VS_reco_phi_nFD->Fill(phtr, phre, weight);
        // }
    }

    void FillNeutronMultiplicity(const std::vector<NeutralHit>& neutrons_FD_NeutronSelection, const std::vector<NeutralHit>& neutrons_FD_NeutronSelection_OnlyGood,
                                 const std::vector<NeutralHit>& neutrons_FD_NeutronSelection_OnlyBad, const double& weight, const std::string selection = "ECALveto") {
        if (Context.nTag == "nFD") {
            if (Context.selection == "Matching" && selection == "PassMatch") {
                if (neutrons_FD_NeutronSelection.size() != 0) { h_reco_nFD_multi_AllN_PassMatch->Fill(neutrons_FD_NeutronSelection.size(), weight); }
                if (neutrons_FD_NeutronSelection_OnlyGood.size() != 0) { h_reco_nFD_multi_OnlyGood_PassMatch->Fill(neutrons_FD_NeutronSelection_OnlyGood.size(), weight); }
                if (neutrons_FD_NeutronSelection_OnlyBad.size() != 0) { h_reco_nFD_multi_OnlyBad_PassMatch->Fill(neutrons_FD_NeutronSelection_OnlyBad.size(), weight); }
            } else if (Context.selection == "Matching" && selection == "NotPassMatch") {
                if (neutrons_FD_NeutronSelection.size() != 0) { h_reco_nFD_multi_AllN_NotPassMatch->Fill(neutrons_FD_NeutronSelection.size(), weight); }
                if (neutrons_FD_NeutronSelection_OnlyGood.size() != 0) { h_reco_nFD_multi_OnlyGood_NotPassMatch->Fill(neutrons_FD_NeutronSelection_OnlyGood.size(), weight); }
                if (neutrons_FD_NeutronSelection_OnlyBad.size() != 0) { h_reco_nFD_multi_OnlyBad_NotPassMatch->Fill(neutrons_FD_NeutronSelection_OnlyBad.size(), weight); }
            } else {
                if (neutrons_FD_NeutronSelection.size() != 0) { h_reco_nFD_multi_AllN->Fill(neutrons_FD_NeutronSelection.size(), weight); }
                if (neutrons_FD_NeutronSelection_OnlyGood.size() != 0) { h_reco_nFD_multi_OnlyGood->Fill(neutrons_FD_NeutronSelection_OnlyGood.size(), weight); }
                if (neutrons_FD_NeutronSelection_OnlyBad.size() != 0) { h_reco_nFD_multi_OnlyBad->Fill(neutrons_FD_NeutronSelection_OnlyBad.size(), weight); }
            }
        }
    }
};
