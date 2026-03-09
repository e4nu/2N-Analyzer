#pragma once

#include <cmath>
#include <tuple>
#include <vector>

#include "TVector3.h"

// Include libraries:
#include "../../../framework/namespaces/general_utilities/utilities.h"

// Include classes:
#include "../../../framework/classes/NeutronVetoRon/nfdveto.h"

// =================================================================================================
// Helper class: FDNeutralHelpers
// =================================================================================================
// Purpose:
//   Provide a small, self-contained, reusable implementation for building FD neutral-hit collections
//   (Ron-style NeutralHit veto output) and for computing basic FD-neutron kinematic variables.
//
// Why a class here (instead of only lambdas):
//   - Keeps the build logic and the kinematics logic in one place.
//   - Lets us reuse the exact same implementation both inside and outside local lambdas.
//   - Makes it easier to unit-test or to move this logic into a shared utilities module later.
//
// Notes:
//   - This class is intentionally lightweight and stores references to event-scope variables.
//   - It does NOT own the underlying particle containers.
//   - The public API mirrors the prior lambdas' return values to avoid changing downstream code.
// =================================================================================================

class FDNeutralHelpers {
   public:
    // -----------------------------------------------------------------------------
    // Constructor
    // -----------------------------------------------------------------------------
    // Stores references to the current event's particle containers and configuration.
    FDNeutralHelpers(FDNeutralVeto& veto_in, std::vector<region_part_ptr>& allParticles_in, std::vector<region_part_ptr>& electrons_in, const double& Ebeam_in, const double& starttime_in,
                     DSCuts& nFD_mom_th_in, const bool& apply_cPart_ECAL_veto_in, const bool& apply_nPart_ECAL_veto_in, const double& cPart_veto_radius_in,
                     const double& nPart_veto_radius_in, const bool& Recalc_Path_nFD_in)
        : veto(veto_in),
          allParticles(allParticles_in),
          electrons(electrons_in),
          Ebeam(Ebeam_in),
          starttime(starttime_in),
          nFD_mom_th(nFD_mom_th_in),
          apply_cPart_ECAL_veto(apply_cPart_ECAL_veto_in),
          apply_nPart_ECAL_veto(apply_nPart_ECAL_veto_in),
          cPart_veto_radius(cPart_veto_radius_in),
          nPart_veto_radius(nPart_veto_radius_in),
          Recalc_Path_nFD(Recalc_Path_nFD_in) {}

    // -----------------------------------------------------------------------------
    // BuildFDNeutrals
    // -----------------------------------------------------------------------------
    // Purpose:
    //   Build the FD neutral-hit collections using the Ron-style veto output (NeutralHit objects).
    //   Applies additional analysis cuts used by this tester (momentum threshold, ECAL edge cuts,
    //   optional charged/neutral ECAL vetoes, optional PCAL exclusion).
    //
    // Output tuple (same ordering as the prior lambda):
    //   (Reco_P_LnFD_mag_local, Reco_P_LnFD_ind_local,
    //    Local_reco_nFD, Local_reco_nFD_OnlyGood, Local_reco_nFD_OnlyBad,
    //    Local_reco_LnFD, Local_gammaFD, Local_reco_nFD_ind)
    //
    // Line-by-line behavior summary:
    //   - Pull NeutralHit candidates from GetNewNeutronFDVetoHits(...)
    //   - For each hit, compute momentum and basic geometry
    //   - Apply analysis cuts
    //   - Partition into "OnlyGood" / "OnlyBad" by sector relation to the electron
    //   - Pick the leading neutron by momentum
    auto BuildFDNeutrals() {
        double Reco_P_LnFD_mag_local = -1;  // Leading neutron momentum (reco)
        int Reco_P_LnFD_ind_local = -1;     // Leading neutron particle index (global)

        NeutralHit Local_reco_LnFD;  // Leading neutron hit (only valid if Reco_P_LnFD_ind_local != -1)
        std::vector<region_part_ptr> Local_gammaFD;
        std::vector<int> Local_reco_nFD_ind;

        std::vector<NeutralHit> Local_reco_nFD;           // All neutrons (after all cuts below)
        std::vector<NeutralHit> Local_reco_nFD_OnlyGood;  // Subset tagged "good" by sector relation to the electron
        std::vector<NeutralHit> Local_reco_nFD_OnlyBad;   // Complement of "OnlyGood"

        // Ron-style hit-level veto output
        std::vector<NeutralHit> Temp_local_reco_nFD = veto.GetNewNeutronFDVetoHits(allParticles, electrons[0]);

        // Apply analysis-level cuts and build output vectors
        for (int i = 0; i < (int)Temp_local_reco_nFD.size(); i++) {
            // Momentum from hit momentum components
            const auto [ToF, beta, gamma, p_vec] = nfdveto::Calc_nFD_vars(Temp_local_reco_nFD[i], starttime);
            // const auto [ToF, beta, gamma, p_vec] = nfdveto::Calc_nFD_vars(Temp_local_reco_nFD[i], starttime, Recalc_Path_nFD);
            (void)ToF;
            (void)beta;
            (void)gamma;
            const double Momentum = p_vec.Mag();

            // Angle computed from hit position relative to electron vertex (used in optional debugging)
            const double Theta_deg = p_vec.Theta() * 180.0 / M_PI;
            (void)Theta_deg;  // Theta_deg is kept for optional debugging; suppress unused warnings in some builds.

            // Core analysis cuts
            // const bool PassMomTh = true;
            const bool PassMomTh = (Momentum >= nFD_mom_th.GetLowerCut());
            const bool PassMinimalThetanFDTh = true;
            const bool PassECALeadgeCuts = (Temp_local_reco_nFD[i].Lv > 14. && Temp_local_reco_nFD[i].Lw > 14.);

            // const bool PassEdepECALCut = (Temp_local_reco_nFD[i].Edep_ECAL <= (gamma - 1) * constants::m_n);

            // Optional ECAL vetoes (charged / neutral activity near the neutron)
            const bool PassCVeto = nfdveto::ChargedECALveto(allParticles, electrons, Ebeam, Temp_local_reco_nFD[i], cPart_veto_radius);
            const bool PassNVeto = nfdveto::NeutralECALveto(allParticles, electrons, Ebeam, Temp_local_reco_nFD[i], nPart_veto_radius);

            // Final keep decision for this neutral hit
            if (PassMomTh && PassECALeadgeCuts && PassMinimalThetanFDTh && (!apply_cPart_ECAL_veto || PassCVeto) && (!apply_nPart_ECAL_veto || PassNVeto)
                //  && (Temp_local_reco_nFD[i].clas12particle_hit_in_PCAL == false)
            ) {
                Local_reco_nFD.push_back(Temp_local_reco_nFD[i]);
                Local_reco_nFD_ind.push_back(Temp_local_reco_nFD[i].pindex);

                // Partition by sector relation to the electron at the neutron's first ECAL layer
                const int nFD_nSector = Temp_local_reco_nFD[i].sector;
                const int e_nSector = electrons[0]->getSector();

                if (std::abs(nFD_nSector - e_nSector) == 3) {
                    Local_reco_nFD_OnlyGood.push_back(Temp_local_reco_nFD[i]);
                } else {
                    Local_reco_nFD_OnlyBad.push_back(Temp_local_reco_nFD[i]);
                }
            }
        }

        // Pick the leading neutron by momentum among the kept hits
        int Temp_Reco_P_LnFD_ind_local = -1;  // index within Local_reco_nFD
        for (size_t k = 0; k < Local_reco_nFD.size(); ++k) {
            const int idx = Local_reco_nFD[k].pindex;  // global index
            const double Momentum = am::RadCalc(Local_reco_nFD[k].Px, Local_reco_nFD[k].Py, Local_reco_nFD[k].Pz);

            if (Momentum >= Reco_P_LnFD_mag_local) {
                Reco_P_LnFD_mag_local = Momentum;
                Reco_P_LnFD_ind_local = idx;
                Temp_Reco_P_LnFD_ind_local = (int)k;
            }
        }

        if (Reco_P_LnFD_ind_local != -1) { Local_reco_LnFD = Local_reco_nFD[Temp_Reco_P_LnFD_ind_local]; }

        return std::make_tuple(Reco_P_LnFD_mag_local, Reco_P_LnFD_ind_local, std::move(Local_reco_nFD), std::move(Local_reco_nFD_OnlyGood), std::move(Local_reco_nFD_OnlyBad),
                               std::move(Local_reco_LnFD), std::move(Local_gammaFD), std::move(Local_reco_nFD_ind));
    }

   private:
    FDNeutralVeto& veto;
    std::vector<region_part_ptr>& allParticles;
    std::vector<region_part_ptr>& electrons;
    const double& Ebeam;
    const double& starttime;

    DSCuts& nFD_mom_th;

    const bool& apply_cPart_ECAL_veto;
    const bool& apply_nPart_ECAL_veto;
    const double& cPart_veto_radius;
    const double& nPart_veto_radius;

    bool Recalc_Path_nFD;
};