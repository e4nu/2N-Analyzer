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
#include "../../variables/NeutronSelection_variables.h"
#include "HistContext.h"

struct NeutronSelectionMisc_histograms {
    HistContext Context;

    TH1D* h_v_dist_nFD;
    TH2D* h_v_dist_nFD_VS_reco_P_nFD;
    TH2D* h_v_dist_nFD_VS_reco_P_e;
    TH2D* h_v_dist_nFD_VS_reco_dPhi_nFD_e;

    // =======================
    // Constructor
    // =======================
    NeutronSelectionMisc_histograms(std::vector<TH1*>& HistoList, const double& Ebeam, const HistContext& ctx) : Context(ctx) {
        h_v_dist_nFD = new TH1D((std::string("v_dist_") + ctx.nTag + "_" + ctx.selection + "_1e_cut").c_str(),
                                (std::string("v_dist in 1e cut (") + ctx.selection + ");v_dist [cm];Counts").c_str(), 50, 0., 1000.);
        HistoList.push_back(h_v_dist_nFD);

        h_v_dist_nFD_VS_reco_P_nFD =
            new TH2D((std::string("v_dist_") + ctx.nTag + "_VS_reco_P_" + ctx.nTag + "_" + ctx.selection + "_1e_cut").c_str(),
                     (std::string("v_dist vs. P^{reco}_{") + ctx.nTag + "} in 1e cut (" + ctx.selection + ");v_dist [cm];P^{reco}_{" + ctx.nTag + "} [GeV/c]").c_str(), 100, 0., 1000., 100,
                     0., Ebeam * 3.);
        HistoList.push_back(h_v_dist_nFD_VS_reco_P_nFD);

        h_v_dist_nFD_VS_reco_P_e =
            new TH2D((std::string("v_dist_") + ctx.nTag + "_VS_reco_P_e_" + ctx.selection + "_1e_cut").c_str(),
                     (std::string("v_dist vs. P^{reco}_{e} in 1e cut (") + ctx.selection + ");v_dist [cm];P^{reco}_{e} [GeV/c]").c_str(), 100, 0., 1000., 100, 0., Ebeam);
        HistoList.push_back(h_v_dist_nFD_VS_reco_P_e);

        h_v_dist_nFD_VS_reco_dPhi_nFD_e = new TH2D((std::string("v_dist_") + ctx.nTag + "_VS_reco_dPhi_" + ctx.nTag + "_e_" + ctx.selection + "_1e_cut").c_str(),
                                                   (std::string("v_dist vs. #delta#phi^{reco}_{") + ctx.nTag + ",e} in 1e cut (" + ctx.selection + ");v_dist [cm];#delta#phi^{reco}_{" +
                                                    ctx.nTag + ",e} = |#phi^{reco}_{" + ctx.nTag + "} - #phi^{reco}_{e}| [#circ]")
                                                       .c_str(),
                                                   100, 0., 1000., 100, -180., 180.);
        HistoList.push_back(h_v_dist_nFD_VS_reco_dPhi_nFD_e);
    }

    // =========================
    // FILLER FUNCTIONS
    // =========================
    void Fill(const TVector3& v_dist, const NeutronSelection_variables& NS_variables, const double& weight) {
        h_v_dist_nFD->Fill(v_dist.Mag(), weight);
        h_v_dist_nFD_VS_reco_P_nFD->Fill(v_dist.Mag(), NS_variables.reco_P_nFD.Mag(), weight);
        h_v_dist_nFD_VS_reco_P_e->Fill(v_dist.Mag(), NS_variables.reco_P_e.Mag(), weight);
        h_v_dist_nFD_VS_reco_dPhi_nFD_e->Fill(v_dist.Mag(), NS_variables.dPhi, weight);
    }
};