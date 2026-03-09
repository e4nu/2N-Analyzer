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

struct NeutronSelectionSameSector_histograms {
    HistContext Context;

    // =======================
    // Δθ / Δφ distributions
    // =======================
    TH1D* h_reco_dTheta_nFD_e_Same_e_nFD_Sector;
    TH1D* h_reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin;

    TH1D* h_reco_dPhi_nFD_e_Same_e_nFD_Sector;
    TH1D* h_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin;

    TH1D* h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts;
    TH1D* h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin;

    // =======================
    // Δθ vs Δφ
    // =======================
    TH2D* h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector;
    TH2D* h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin;

    // =======================
    // Electron momentum correlations
    // =======================
    TH2D* h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector;
    TH2D* h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin;

    // =======================
    // Status correlations (Δθ)
    // =======================
    TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector;
    TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector;

    TH2D* h_e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector;
    TH2D* h_e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector;

    // =======================
    // θ(n,e) observables
    // =======================
    TH1D* h_reco_theta_nFD_e_Same_e_nFD_Sector;

    TH2D* h_reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector;
    TH2D* h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector;
    TH2D* h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin;
    TH2D* h_reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector;

    // =======================
    // Status vs θ(n,e)
    // =======================
    TH2D* h_nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector;
    TH2D* h_nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector;

    TH2D* h_e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector;
    TH2D* h_e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector;

    // =======================
    // Constructor
    // =======================
    NeutronSelectionSameSector_histograms(std::vector<TH1*>& HistoList, DSCuts dTheta_cuts, const double& Ebeam, const HistContext& ctx) : Context(ctx) {
        // =======================
        // Δθ / Δφ distributions
        // =======================
        h_reco_dTheta_nFD_e_Same_e_nFD_Sector = new TH1D(("reco_dTheta_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                                                         ("#delta#theta^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection +
                                                          "};#delta#theta^{reco}_{" + ctx.nTag + ",e} = #theta^{reco}_{" + ctx.nTag + "} - #theta^{reco}_{e} [#circ];Counts")
                                                             .c_str(),
                                                         50, -25., 10.);
        HistoList.push_back(h_reco_dTheta_nFD_e_Same_e_nFD_Sector);
        h_reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin = new TH1D(("reco_dTheta_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_zoomin_" + ctx.selection + "_1e_cut").c_str(),
                                                                ("#delta#theta^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} - zoomin (" + ctx.selection +
                                                                 "};#delta#theta^{reco}_{" + ctx.nTag + ",e} = #theta^{reco}_{" + ctx.nTag + "} - #theta^{reco}_{e} [#circ];Counts")
                                                                    .c_str(),
                                                                50, -10., 5.);
        HistoList.push_back(h_reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin);

        h_reco_dPhi_nFD_e_Same_e_nFD_Sector = new TH1D(("reco_dPhi_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                                                       ("#delta#phi^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection + "};#delta#phi^{reco}_{" +
                                                        ctx.nTag + ",e} = |#phi^{reco}_{" + ctx.nTag + "} - #phi^{reco}_{e}| [#circ];Counts")
                                                           .c_str(),
                                                       50, -180., 180.);
        HistoList.push_back(h_reco_dPhi_nFD_e_Same_e_nFD_Sector);
        h_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin = new TH1D(("reco_dPhi_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_zoomin_" + ctx.selection + "_1e_cut").c_str(),
                                                              ("#delta#phi^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} - zoomin (" + ctx.selection +
                                                               "};#delta#phi^{reco}_{" + ctx.nTag + ",e} = |#phi^{reco}_{" + ctx.nTag + "} - #phi^{reco}_{e}| [#circ];Counts")
                                                                  .c_str(),
                                                              50, -50., 10.);
        HistoList.push_back(h_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin);

        h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts =
            new TH1D(("reco_dPhi_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_for_dTheta_cuts_" + ctx.selection + "_1e_cut").c_str(),
                     ("#delta#phi^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} and " + bt::ToStringWithPrecision(dTheta_cuts.GetLowerCut()) +
                      " #leq #delta#theta^{reco}_{" + ctx.nTag + ",e} #leq " + bt::ToStringWithPrecision(dTheta_cuts.GetUpperCut()) + " (" + ctx.selection + "};#delta#phi^{reco}_{" +
                      ctx.nTag + ",e} = |#phi^{reco}_{" + ctx.nTag + "} - #phi^{reco}_{e}| [#circ];Counts")
                         .c_str(),
                     50, -180., 180.);
        HistoList.push_back(h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts);
        h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin =
            new TH1D(("reco_dPhi_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_for_dTheta_cuts_zoomin_" + ctx.selection + "_1e_cut").c_str(),
                     ("#delta#phi^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} and " + bt::ToStringWithPrecision(dTheta_cuts.GetLowerCut()) +
                      " #leq #delta#theta^{reco}_{" + ctx.nTag + ",e} #leq " + bt::ToStringWithPrecision(dTheta_cuts.GetUpperCut()) + " - zoomin (" + ctx.selection +
                      "};#delta#phi^{reco}_{" + ctx.nTag + ",e} = |#phi^{reco}_{" + ctx.nTag + "} - #phi^{reco}_{e}| [#circ];Counts")
                         .c_str(),
                     50, -50., 10.);
        HistoList.push_back(h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin);

        // =======================
        // Δθ vs Δφ
        // =======================
        h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector =
            new TH2D(("reco_dTheta_" + ctx.nTag + "_e_VS_reco_dPhi_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                     ("#delta#theta^{reco}_{" + ctx.nTag + ",e} vs. #delta#phi^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection +
                      "};#delta#phi^{reco}_{" + ctx.nTag + ",e} = |#phi^{reco}_{" + ctx.nTag + "} - #phi^{reco}_{e}| [#circ];#delta#theta^{reco}_{" + ctx.nTag + ",e} = #theta^{reco}_{" +
                      ctx.nTag + "} - #theta^{reco}_{e} [#circ]")
                         .c_str(),
                     100, -180., 180., 100, -25., 10.);
        HistoList.push_back(h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector);
        h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin =
            new TH2D(("reco_dTheta_" + ctx.nTag + "_e_VS_reco_dPhi_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_zoomin_" + ctx.selection + "_1e_cut").c_str(),
                     ("#delta#theta^{reco}_{" + ctx.nTag + ",e} vs. #delta#phi^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} - zoomin (" + ctx.selection +
                      "};#delta#phi^{reco}_{" + ctx.nTag + ",e} = |#phi^{reco}_{" + ctx.nTag + "} - #phi^{reco}_{e}| [#circ];#delta#theta^{reco}_{" + ctx.nTag + ",e} = #theta^{reco}_{" +
                      ctx.nTag + "} - #theta^{reco}_{e} [#circ]")
                         .c_str(),
                     100, -50., 10., 100, -10., 5.);
        HistoList.push_back(h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin);

        // =======================
        // Electron momentum correlations
        // =======================
        h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector =
            new TH2D(("reco_P_e_VS_reco_dPhi_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                     ("P^{reco}_{e} vs. #delta#phi^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection + "};P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{" +
                      ctx.nTag + ",e} = |#phi^{reco}_{" + ctx.nTag + "} - #phi^{reco}_{e}| [#circ]")
                         .c_str(),
                     100, 0., Ebeam * 1.1, 100, -180., 180.);
        HistoList.push_back(h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector);
        h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin =
            new TH2D(("reco_P_e_VS_reco_dPhi_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_zoomin_" + ctx.selection + "_1e_cut").c_str(),
                     ("P^{reco}_{e} vs. #delta#phi^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} - zoomin (" + ctx.selection +
                      "};P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{" + ctx.nTag + ",e} = |#phi^{reco}_{" + ctx.nTag + "} - #phi^{reco}_{e}| [#circ]")
                         .c_str(),
                     100, 0., Ebeam * 1.1, 100, -50., 10);
        HistoList.push_back(h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin);

        // =======================
        // Status correlations (Δθ)
        // =======================
        h_nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector =
            new TH2D(("" + ctx.nTag + "_status_VS_reco_dTheta_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                     ("" + ctx.nTag + " status vs. #delta#theta^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection + "};#delta#theta^{reco}_{" +
                      ctx.nTag + ",e} = #theta^{reco}_{" + ctx.nTag + "} - #theta^{reco}_{e} [#circ];" + ctx.nTag + " status")
                         .c_str(),
                     100, -25., 10., 100, 1990, 3000.);
        HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector);
        h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector =
            new TH2D(("" + ctx.nTag + "_status_VS_reco_dTheta_" + ctx.nTag + "_e_zoomin_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                     ("" + ctx.nTag + " status vs. #delta#theta^{reco}_{" + ctx.nTag + ",e} - zoomin - in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection +
                      "};#delta#theta^{reco}_{" + ctx.nTag + ",e} = #theta^{reco}_{" + ctx.nTag + "} - #theta^{reco}_{e} [#circ];" + ctx.nTag + " status")
                         .c_str(),
                     100, -10., 5., 100, 1990, 2250.);
        //  100, -25., 10., 100, 1990, 2250.);
        HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector);

        h_e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector = new TH2D(("e_status_VS_reco_dTheta_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                                                                     ("e status vs. #delta#theta^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection +
                                                                      "};#delta#theta^{reco}_{" + ctx.nTag + ",e} = #theta^{reco}_{" + ctx.nTag + "} - #theta^{reco}_{e} [#circ];e status")
                                                                         .c_str(),
                                                                     100, -25., 10., 100, -2250., -1990.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector);
        h_e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector =
            new TH2D(("e_status_VS_reco_dTheta_" + ctx.nTag + "_e_zoomin_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                     ("e status vs. #delta#theta^{reco}_{" + ctx.nTag + ",e} - zoomin - in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection + "};#delta#theta^{reco}_{" + ctx.nTag +
                      ",e} = #theta^{reco}_{" + ctx.nTag + "} - #theta^{reco}_{e} [#circ];e status")
                         .c_str(),
                     100, -10., 5., 100, -2250., -2100.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector);

        // =======================
        // θ(n,e) observables
        // =======================
        h_reco_theta_nFD_e_Same_e_nFD_Sector = new TH1D(
            ("reco_theta_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
            ("#theta^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection + "};#theta^{reco}_{" + ctx.nTag + ",e} [#circ];Counts").c_str(), 50, 0., 50.);
        HistoList.push_back(h_reco_theta_nFD_e_Same_e_nFD_Sector);

        h_reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector =
            new TH2D(("reco_theta_" + ctx.nTag + "_e_VS_reco_dTheta_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                     ("#theta^{reco}_{" + ctx.nTag + ",e} vs. #delta#theta^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection +
                      "};#delta#theta^{reco}_{" + ctx.nTag + ",e} = #theta^{reco}_{" + ctx.nTag + "} - #theta^{reco}_{e} [#circ];#theta^{reco}_{" + ctx.nTag + ",e} [#circ]")
                         .c_str(),
                     100, -25., 10., 100, 0., 50.);
        HistoList.push_back(h_reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector);
        h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector =
            new TH2D(("reco_theta_" + ctx.nTag + "_e_VS_reco_dPhi_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                     ("#theta^{reco}_{" + ctx.nTag + ",e} vs. #delta#phi^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection + "};#delta#phi^{reco}_{" +
                      ctx.nTag + ",e} = |#phi^{reco}_{" + ctx.nTag + "} - #phi^{reco}_{e}| [#circ];#theta^{reco}_{" + ctx.nTag + ",e} [#circ]")
                         .c_str(),
                     100, -180., 180., 100, 0., 50.);
        HistoList.push_back(h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector);
        h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin =
            new TH2D(("reco_theta_" + ctx.nTag + "_e_VS_reco_dPhi_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_zoomin_" + ctx.selection + "_1e_cut").c_str(),
                     ("#theta^{reco}_{" + ctx.nTag + ",e} vs. #delta#phi^{reco}_{" + ctx.nTag + ",e} - zoomin - in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection +
                      "};#delta#phi^{reco}_{" + ctx.nTag + ",e} = |#phi^{reco}_{" + ctx.nTag + "} - #phi^{reco}_{e}| [#circ];#theta^{reco}_{" + ctx.nTag + ",e} [#circ]")
                         .c_str(),
                     100, -50., 10., 100, 0., 25.);
        HistoList.push_back(h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin);
        h_reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector =
            new TH2D(("reco_theta_" + ctx.nTag + "_e_VS_reco_P_" + ctx.nTag + "_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                     ("#theta^{reco}_{" + ctx.nTag + ",e} vs. P^{reco}_{" + ctx.nTag + "} in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection + "};P^{reco}_{" + ctx.nTag +
                      "} [GeV/c];#theta^{reco}_{" + ctx.nTag + ",e} [#circ]")
                         .c_str(),
                     100, 0., Ebeam * 3., 100, 0., 50.);
        HistoList.push_back(h_reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector);

        // =======================
        // Status vs θ(n,e)
        // =======================
        h_nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector =
            new TH2D(("" + ctx.nTag + "_status_VS_reco_theta_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                     ("" + ctx.nTag + " status vs. #theta^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection + "};#theta^{reco}_{" + ctx.nTag +
                      ",e} [#circ];" + ctx.nTag + " status")
                         .c_str(),
                     100, 0., 50., 100, 1990, 3000.);
        HistoList.push_back(h_nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector);
        h_nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector =
            new TH2D(("" + ctx.nTag + "_status_VS_reco_theta_" + ctx.nTag + "_e_zoomin_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                     ("" + ctx.nTag + " status vs. #theta^{reco}_{" + ctx.nTag + ",e} - zoomin - in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection + "};#theta^{reco}_{" +
                      ctx.nTag + ",e} [#circ];" + ctx.nTag + " status")
                         .c_str(),
                     100, 0., 20., 100, 1990, 2250.);
        HistoList.push_back(h_nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector);

        h_e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector = new TH2D(
            ("e_status_VS_reco_theta_" + ctx.nTag + "_e_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
            ("e status vs. #theta^{reco}_{" + ctx.nTag + ",e} in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection + "};#theta^{reco}_{" + ctx.nTag + ",e} [#circ];e status")
                .c_str(),
            100, 0., 50., 100, -2250., -1990.);
        HistoList.push_back(h_e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector);
        h_e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector =
            new TH2D(("e_status_VS_reco_theta_" + ctx.nTag + "_e_zoomin_Same_e_" + ctx.nTag + "_Sector_" + ctx.selection + "_1e_cut").c_str(),
                     ("e status vs. #theta^{reco}_{" + ctx.nTag + ",e} - zoomin - in 1e cut for S_{" + ctx.nTag + "} = S_{e} (" + ctx.selection + "};#theta^{reco}_{" + ctx.nTag +
                      ",e} [#circ];e status")
                         .c_str(),
                     100, 0., 20., 100, -2250., -0.);
        HistoList.push_back(h_e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector);
    }

    // =======================
    // Fill function
    // =======================
    void Fill(const NeutronSelection_variables& NS_variables, const bool& Pass_dTheta_window, const double& weight) {
        h_reco_dTheta_nFD_e_Same_e_nFD_Sector->Fill(NS_variables.dTheta, weight);
        h_reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin->Fill(NS_variables.dTheta, weight);

        h_reco_dPhi_nFD_e_Same_e_nFD_Sector->Fill(NS_variables.dPhi, weight);
        h_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin->Fill(NS_variables.dPhi, weight);

        if (Pass_dTheta_window) {
            h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts->Fill(NS_variables.dPhi, weight);
            h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin->Fill(NS_variables.dPhi, weight);
        }

        h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector->Fill(NS_variables.dPhi, NS_variables.dTheta, weight);
        h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin->Fill(NS_variables.dPhi, NS_variables.dTheta, weight);

        h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector->Fill(NS_variables.reco_P_e.Mag(), NS_variables.dPhi, weight);
        h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin->Fill(NS_variables.reco_P_e.Mag(), NS_variables.dPhi, weight);

        // h_nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector->Fill(NS_variables.dTheta, NS_variables.nFD_status, weight);
        // h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector->Fill(NS_variables.dTheta, NS_variables.nFD_status, weight);

        h_e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector->Fill(NS_variables.dTheta, NS_variables.e_status, weight);
        h_e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector->Fill(NS_variables.dTheta, NS_variables.e_status, weight);

        h_reco_theta_nFD_e_Same_e_nFD_Sector->Fill(NS_variables.reco_P_nFD.Theta() * 180 / pi, weight);

        h_reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector->Fill(NS_variables.dTheta, NS_variables.reco_P_nFD.Theta() * 180 / pi, weight);
        h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector->Fill(NS_variables.dPhi, NS_variables.reco_P_nFD.Theta() * 180 / pi, weight);
        h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin->Fill(NS_variables.dPhi, NS_variables.reco_P_nFD.Theta() * 180 / pi, weight);
        h_reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector->Fill(NS_variables.reco_P_nFD.Mag(), NS_variables.reco_P_nFD.Theta() * 180 / pi, weight);

        // h_nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector->Fill(NS_variables.reco_P_nFD.Theta() * 180 / pi, NS_variables.nFD_status, weight);
        // h_nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector->Fill(NS_variables.reco_P_nFD.Theta() * 180 / pi, NS_variables.nFD_status, weight);

        h_e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector->Fill(NS_variables.reco_P_nFD.Theta() * 180 / pi, NS_variables.e_status, weight);
        h_e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector->Fill(NS_variables.reco_P_nFD.Theta() * 180 / pi, NS_variables.e_status, weight);
    }
};
