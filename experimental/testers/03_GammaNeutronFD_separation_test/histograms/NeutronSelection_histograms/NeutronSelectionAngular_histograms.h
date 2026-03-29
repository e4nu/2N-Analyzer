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
#include "../../ECAL_vetoes.cpp"
#include "../../variables/NeutronSelection_variables.h"
#include "HistContext.h"

struct NeutronSelectionAngular_histograms {
    HistContext Context;

    // =======================
    // Δθ / Δφ histograms
    // =======================
    TH1D* h_reco_dTheta_nFD_e;
    TH1D* h_reco_dPhi_nFD_e;

    TH2D* h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e;
    TH2D* h_reco_P_nFD_VS_reco_dTheta_nFD_e;
    TH2D* h_reco_P_nFD_VS_reco_dPhi_nFD_e;
    TH2D* h_reco_P_e_VS_reco_dPhi_nFD_e;

    // =======================
    // Status correlations
    // =======================
    TH2D* h_nFD_status_VS_reco_dTheta_nFD_e;
    TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_zoomin;

    TH2D* h_e_status_VS_reco_dTheta_nFD_e;
    TH2D* h_e_status_VS_reco_dTheta_nFD_e_zoomin;

    // =======================
    // ECAL hit angular maps
    // =======================
    TH2D* h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV;

    // =======================
    // Δr histograms
    // =======================
    TH1D* h_dr_n;
    TH2D* h_dr_n_VS_dTheta_n_e;
    TH2D* h_dr_n_VS_dPhi_n_e;

    // =======================
    // Sliced plots
    // =======================
    std::vector<TObject*> Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList;
    hsPlots h_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n;

    // =======================
    // Vertex-referenced angular map
    // =======================
    TH2D* dTheta_nFD_V_VS_dPhi_nFD_V_Electrons_AcEV;

    // =======================
    // Constructor
    // =======================
    NeutronSelectionAngular_histograms(std::vector<TH1*>& HistoList, const std::vector<std::vector<double>>& dr_n_SliceLimits,
                                       std::vector<TObject*>& HistoList_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n, const double& Ebeam, const HistContext& ctx)
        : Context(ctx),
          h_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n(dr_n_SliceLimits, hsPlots::TH2D_TYPE, HistoList_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n,
                                                  &Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList,
                                                  "dTheta_" + ctx.nTag + "_e_VS_dPhi_" + ctx.nTag + "_e_BysliceOf_dr_" + ctx.nTag + "_" + ctx.selection + "_1e_cut",
                                                  "#delta#theta^{reco}_{" + ctx.nTag + ",e} vs. #delta#phi^{reco}_{" + ctx.nTag + ",e} in slices of |#Deltar_{" + ctx.nTag +
                                                      "}| in 1e cut (" + ctx.selection + ");#delta#phi^{reco}_{" + ctx.nTag + ",e} [#circ];#delta#theta^{reco}_{" + ctx.nTag + ",e} [#circ]",
                                                  100, -180., 180., 100, -25., 10., "|#Deltar_{" + ctx.nTag + "}| [cm]") {
        // =======================
        // Δθ / Δφ histograms
        // =======================
        h_reco_dTheta_nFD_e = new TH1D(("reco_dTheta_" + Context.nTag + "_e_" + ctx.selection + "_1e_cut").c_str(),
                                       ("#delta#theta^{reco}_{" + Context.nTag + ",e} in 1e cut (" + ctx.selection + ");#delta#theta^{reco}_{" + Context.nTag + ",e} = #theta^{reco}_{" +
                                        Context.nTag + "} - #theta^{reco}_{e} [#circ];Counts ")
                                           .c_str(),
                                       50, -25., 10.);
        HistoList.push_back(h_reco_dTheta_nFD_e);
        h_reco_dPhi_nFD_e = new TH1D(("reco_dPhi_" + Context.nTag + "_e_" + ctx.selection + "_1e_cut").c_str(),
                                     ("#delta#phi^{reco}_{" + Context.nTag + ",e} in 1e cut (" + ctx.selection + ");#delta#phi^{reco}_{" + Context.nTag + ",e} = |#phi^{reco}_{" +
                                      Context.nTag + "} - #phi^{reco}_{e}| [#circ];Counts")
                                         .c_str(),
                                     50, -180., 180.);
        HistoList.push_back(h_reco_dPhi_nFD_e);

        h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e =
            new TH2D(("reco_dTheta_" + Context.nTag + "_e_VS_reco_dPhi_" + Context.nTag + "_e_" + ctx.selection + "_1e_cut").c_str(),
                     ("#delta#theta^{reco}_{" + Context.nTag + ",e} vs. #delta#phi^{reco}_{" + Context.nTag + ",e} in 1e cut (" + ctx.selection + ");#delta#phi^{reco}_{" + Context.nTag +
                      ",e} = |#phi^{reco}_{" + Context.nTag + "} - #phi^{reco}_{e}| [#circ];#delta#theta^{reco}_{" + Context.nTag + ",e} = #theta^{reco}_{" + Context.nTag +
                      "} - #theta^{reco}_{e} [#circ]")
                         .c_str(),
                     100, -180., 180., 100, -25., 10.);
        HistoList.push_back(h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e);
        h_reco_P_nFD_VS_reco_dTheta_nFD_e =
            new TH2D(("reco_P_" + Context.nTag + "_VS_reco_dTheta_" + Context.nTag + "_e_" + ctx.selection + "_1e_cut").c_str(),
                     ("P^{reco}_{" + Context.nTag + "} vs. #delta#theta^{reco}_{" + Context.nTag + ",e} in 1e cut (" + ctx.selection + ");#delta#theta^{reco}_{" + Context.nTag +
                      ",e} = #theta^{reco}_{" + Context.nTag + "} - #theta^{reco}_{e} [#circ];P^{reco}_{" + Context.nTag + "} [GeV/c]")
                         .c_str(),
                     100, -25., 10., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_P_nFD_VS_reco_dTheta_nFD_e);
        h_reco_P_nFD_VS_reco_dPhi_nFD_e = new TH2D(("reco_P_" + Context.nTag + "_VS_reco_dPhi_" + Context.nTag + "_e_" + ctx.selection + "_1e_cut").c_str(),
                                                   ("P^{reco}_{" + Context.nTag + "} vs. #delta#phi^{reco}_{" + Context.nTag + ",e} in 1e cut (" + ctx.selection + ");#delta#phi^{reco}_{" +
                                                    Context.nTag + ",e} = |#phi^{reco}_{" + Context.nTag + "} - #phi^{reco}_{e}| [#circ];P^{reco}_{" + Context.nTag + "} [GeV/c]")
                                                       .c_str(),
                                                   100, -180., 180., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_P_nFD_VS_reco_dPhi_nFD_e);
        h_reco_P_e_VS_reco_dPhi_nFD_e = new TH2D(("reco_P_e_VS_reco_dPhi_" + Context.nTag + "_e_" + ctx.selection + "_1e_cut").c_str(),
                                                 ("P^{reco}_{e} vs. #delta#phi^{reco}_{" + Context.nTag + ",e} in 1e cut (" + ctx.selection + ");P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{" +
                                                  Context.nTag + ",e} = |#phi^{reco}_{" + Context.nTag + "} - #phi^{reco}_{e}| [#circ]")
                                                     .c_str(),
                                                 100, 0., Ebeam * 1.1, 100, -180., 180.);
        HistoList.push_back(h_reco_P_e_VS_reco_dPhi_nFD_e);

        // =======================
        // Status correlations
        // =======================
        h_nFD_status_VS_reco_dTheta_nFD_e = new TH2D(("" + Context.nTag + "_status_VS_reco_dTheta_" + Context.nTag + "_e_" + ctx.selection + "_1e_cut").c_str(),
                                                     ("" + Context.nTag + " status vs. #delta#theta^{reco}_{" + Context.nTag + ",e} in 1e cut (" + ctx.selection + ");#delta#theta^{reco}_{" +
                                                      Context.nTag + ",e} = #theta^{reco}_{" + Context.nTag + "} - #theta^{reco}_{e} [#circ];" + Context.nTag + " status")
                                                         .c_str(),
                                                     100, -25., 10., 100, 1990, 3000.);
        HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e);
        h_nFD_status_VS_reco_dTheta_nFD_e_zoomin =
            new TH2D(("" + Context.nTag + "_status_VS_reco_dTheta_" + Context.nTag + "_e_zoomin_" + ctx.selection + "_1e_cut").c_str(),
                     ("" + Context.nTag + " status vs. #delta#theta^{reco}_{" + Context.nTag + ",e} - zoomin - in 1e cut (" + ctx.selection + ");#delta#theta^{reco}_{" + Context.nTag +
                      ",e} = #theta^{reco}_{" + Context.nTag + "} - #theta^{reco}_{e} [#circ];" + Context.nTag + " status")
                         .c_str(),
                     100, -10., 5., 100, 1990, 2250.);
        HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_zoomin);

        h_e_status_VS_reco_dTheta_nFD_e = new TH2D(("e_status_VS_reco_dTheta_" + Context.nTag + "_e_" + ctx.selection + "_1e_cut").c_str(),
                                                   ("e status vs. #delta#theta^{reco}_{" + Context.nTag + ",e} in 1e cut (" + ctx.selection + ");#delta#theta^{reco}_{" + Context.nTag +
                                                    ",e} = #theta^{reco}_{" + Context.nTag + "} - #theta^{reco}_{e} [#circ];e status")
                                                       .c_str(),
                                                   100, -25., 10., 100, -2250., -1990.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e);
        h_e_status_VS_reco_dTheta_nFD_e_zoomin = new TH2D(("e_status_VS_reco_dTheta_" + Context.nTag + "_e_zoomin_" + ctx.selection + "_1e_cut").c_str(),
                                                          ("e status vs. #delta#theta^{reco}_{" + Context.nTag + ",e} - zoomin - in 1e cut (" + ctx.selection + ");#delta#theta^{reco}_{" +
                                                           Context.nTag + ",e} = #theta^{reco}_{" + Context.nTag + "} - #theta^{reco}_{e} [#circ];e status")
                                                              .c_str(),
                                                          100, -10., 5., 100, -2250., -0.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_zoomin);

        // =======================
        // ECAL hit angular maps
        // =======================
        h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV = new TH2D(("dTheta_" + ctx.nTag + "_e_VS_dPhi_" + ctx.nTag + "_e_Electrons_AcEV_" + ctx.selection + "_1e_cut").c_str(),
                                                           ("'Neutron Hits' vs. Electron Hits - After Veto in 1e cut (" + ctx.selection + ");#Delta#phi_{" + ctx.nTag + ",e} = #phi_{" +
                                                            ctx.nTag + "}^{ECAL} - #phi_{e}^{ECAL} [#circ];#Delta#theta_{" + ctx.nTag +
                                                            ",e} = "
                                                            "#theta_{" +
                                                            ctx.nTag + "}^{ECAL} - #theta_{e}^{ECAL} [#circ]")
                                                               .c_str(),
                                                           100, -180, 180, 100, -25, 10);
        HistoList.push_back(h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV);

        // =======================
        // Δr histograms
        // =======================
        h_dr_n = new TH1D(("dr_" + ctx.nTag + "_" + ctx.selection + "_1e_cut").c_str(),
                          ("|#Delta#font[62]{r}_{" + ctx.nTag + "}| in 1e cut (" + ctx.selection + ");|#Delta#font[62]{r}_{" + ctx.nTag + "}| = |#font[62]{r}^{hit}_{" + ctx.nTag +
                           "} - #font[62]{R}_{e}| [cm];Counts")
                              .c_str(),
                          50, 0, 400);
        HistoList.push_back(h_dr_n);
        h_dr_n_VS_dTheta_n_e =
            new TH2D(("dr_" + ctx.nTag + "_VS_dTheta_" + ctx.nTag + "_e_" + ctx.selection + "_1e_cut").c_str(),
                     ("|#Delta#font[62]{r}_{" + ctx.nTag + "}| vs. #delta#theta^{reco}_{" + Context.nTag + ",e} - in 1e cut (" + ctx.selection + ");|#Delta#font[62]{r}_{" + ctx.nTag +
                      "}| = |#font[62]{r}^{hit}_{" + ctx.nTag + "} - #font[62]{R}_{e}| [cm];#Delta#theta_{" + Context.nTag + ",e} = #theta_{V}^{ECAL} - #theta_{e}^{ECAL} [#circ]")
                         .c_str(),
                     100, 0, 400, 100, -25, 10);
        HistoList.push_back(h_dr_n_VS_dTheta_n_e);
        h_dr_n_VS_dPhi_n_e =
            new TH2D(("dr_" + ctx.nTag + "_VS_dPhi_" + ctx.nTag + "_e_" + ctx.selection + "_1e_cut").c_str(),
                     ("|#Delta#font[62]{r}_{" + ctx.nTag + "}| vs. #delta#phi^{reco}_{" + Context.nTag + ",e} - in 1e cut (" + ctx.selection + ");|#Delta#font[62]{r}_{" + ctx.nTag +
                      "}| = |#font[62]{r}^{hit}_{" + ctx.nTag + "} - #font[62]{R}_{e}| [cm];#Delta#phi_{" + Context.nTag + ",e} = #phi_{V}^{ECAL} - #phi_{e}^{ECAL} [#circ]")
                         .c_str(),
                     100, 0, 400, 100, -25, 10);
        HistoList.push_back(h_dr_n_VS_dPhi_n_e);

        // =======================
        // Vertex-referenced angular map
        // =======================
        dTheta_nFD_V_VS_dPhi_nFD_V_Electrons_AcEV =
            new TH2D(("dTheta_" + ctx.nTag + "_V_VS_dPhi_" + ctx.nTag + "_V_Electrons_AcEV_" + ctx.selection + "_1e_cut").c_str(),
                     ("#Delta#theta_{" + Context.nTag + ",V} vs. #Delta#phi_{" + Context.nTag + ",V} - After Veto in 1e cut (" + ctx.selection + ");#Delta#phi_{" + ctx.nTag +
                      ",V} = #phi_{" + ctx.nTag + "}^{ECAL} - #phi_{V} [#circ];#Delta#theta_{" + Context.nTag + ",V} = #theta_{" + Context.nTag + "}^{ECAL} - #theta_{V} [#circ]")
                         .c_str(),
                     100, -180, 180, 100, -25, 10);
        HistoList.push_back(dTheta_nFD_V_VS_dPhi_nFD_V_Electrons_AcEV);
    }

    // =========================
    // FILLER FUNCTIONS
    // =========================
    void Fill(const NeutralHit& nFD, NeutronSelection_variables& NS_variables, const vector<region_part_ptr>& electrons, const double& B, const double& L, const double& weight) {
        h_reco_dTheta_nFD_e->Fill(NS_variables.dTheta, weight);
        h_reco_dPhi_nFD_e->Fill(NS_variables.dPhi, weight);

        h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e->Fill(NS_variables.dPhi, NS_variables.dTheta, weight);
        h_reco_P_nFD_VS_reco_dTheta_nFD_e->Fill(NS_variables.dTheta, NS_variables.reco_P_nFD.Mag(), weight);
        h_reco_P_nFD_VS_reco_dPhi_nFD_e->Fill(NS_variables.dPhi, NS_variables.reco_P_nFD.Mag(), weight);
        h_reco_P_e_VS_reco_dPhi_nFD_e->Fill(NS_variables.reco_P_e.Mag(), NS_variables.dPhi, weight);

        // h_nFD_status_VS_reco_dTheta_nFD_e->Fill(NS_variables.dTheta, NS_variables.nFD_status, weight);
        // h_nFD_status_VS_reco_dTheta_nFD_e_zoomin->Fill(NS_variables.dTheta, NS_variables.nFD_status, weight);

        h_e_status_VS_reco_dTheta_nFD_e->Fill(NS_variables.dTheta, NS_variables.e_status, weight);
        h_e_status_VS_reco_dTheta_nFD_e_zoomin->Fill(NS_variables.dTheta, NS_variables.e_status, weight);

        NS_variables.v_nhit = TVector3(nFD.xHit, nFD.yHit, nFD.zHit);
        NS_variables.shifted_e_unit_v = nfdveto::Get_shifted_e_unit_v_NeutVeto(electrons, NS_variables.v_nhit, B, L);
        NS_variables.v_dist = NS_variables.v_nhit - NS_variables.shifted_e_unit_v;

        h_dr_n->Fill(NS_variables.v_dist.Mag(), weight);
        h_dr_n_VS_dTheta_n_e->Fill(NS_variables.v_dist.Mag(), NS_variables.dTheta, weight);
        h_dr_n_VS_dPhi_n_e->Fill(NS_variables.v_dist.Mag(), NS_variables.dPhi, weight);

        h_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n.Fill(NS_variables.v_dist.Mag(), NS_variables.dPhi, NS_variables.dTheta, weight);
        dTheta_nFD_V_VS_dPhi_nFD_V_Electrons_AcEV->Fill(NS_variables.v_dist.Phi() * 180 / M_PI, NS_variables.v_dist.Theta() * 180 / M_PI, weight);

        // h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV->Fill(NS_variables.dPhi, NS_variables.dTheta, weight);
    }
};
