//
// Created by Alon Sportes on 30/03/2026.
//

#ifndef FINALSTATECONFIGURATION_H
#define FINALSTATECONFIGURATION_H

#include <iostream>

// Include libraries:
#include "libraries/general_utilities/basic_tools.h"
#include "libraries/general_utilities/constants.h"

// Include AnalysisTools:
#include "../AnalysisTools/ExperimentParameters/ExperimentParameters.h"

namespace bt = basic_tools;

struct FinalStateConfiguration {
    // Event variant ----------------------------------------------------------------------------------------------------------------------------------------------------

    /**
     * @enum EventVariant
     * Defines the event variant used in final state histograms. These are the following:
     *      (*) RECO_SIM_EVENT: reconstructed events in simulation. Adds reco label to final state histograms.
     *      (*) TRUTH_LEVEL_SIM_EVENT: truth-level events in simulation from mc bank.
     *      (*) RECO_DATA_EVENT: reconstructed events in data. Does not add reco label to final state histograms.
     *      (*) UNSET_EVENT_VARIANT: a default option that aborts operation.
     */
    enum EventVariant { RECO_SIM_EVENT, TRUTH_LEVEL_SIM_EVENT, RECO_DATA_EVENT, UNSET_EVENT_VARIANT };
    const EventVariant eventVariant;

    const std::string EventVariantLabel;

    // Final state signature --------------------------------------------------------------------------------------------------------------------------------------------

    /**
     * @enum FinalStateSignature
     * Defines the final state signature used in final state histograms. These are the following:
     *      (*) FinalState_Inclusive:
     *              (-) Inclusive (e,e') events.
     *              (-) One reco electron and any other particles.
     *      (*) FinalState_1N_1pFD:
     *              (-) Basic event selection (see below).
     *              (-) No CD protons, one FD proton.
     *              (-) Zero or any number of FD neutrons according to event selection settings.
     *      (*) FinalState_1N_1nFD: basic event selection (see below), no CD or FD protons, one FD proton.
     *              (-) Basic event selection (see below).
     *              (-) No CD protons, no FD protons.
     *              (-) One or any number of FD neutrons according to event selection settings.
     *      (*) FinalState_2N_2p:
     *              (-) Basic event selection (see below).
     *              (-) Two protons in the event, regardless of CD or FD.
     *              (-) Zero or any number of FD neutrons according to event selection settings.
     *      (*) FinalState_2N_1pFD1pCD:
     *              (-) Basic event selection (see below).
     *              (-) Two protons in the event - one CD proton and one FD proton.
     *              (-) Zero or any number of FD neutrons according to event selection settings.
     *      (*) FinalState_2N_1nFD1pCD:
     *              (-) Basic event selection (see below).
     *              (-) One CD proton, no FD protons.
     *              (-) One or any number of FD neutrons according to event selection settings.
     *      (*) UNSET_SIGNATURE: a default option that aborts operation.
     *
     * Basic event selection:
     *      (*) Charged particles:
     *              (-) No charged pions above momentum threshold.
     *              (-) No charged kaons whatsoever.
     *              (-) No deuterons whatsoever.
     *      (*) Neutral particles:
     *              (-) No limit on CD neutrals.
     *              (-) FD photons above momentum threshold are removed or enabled according to event selection settings.
     *              (-) Any number of other neutrals and particles with pdg=0.
     */
    enum FinalStateSignature { FinalState_Inclusive, FinalState_1N_1pFD, FinalState_1N_1nFD, FinalState_2N_2p, FinalState_2N_1pFD1pCD, FinalState_2N_1nFD1pCD, UNSET_SIGNATURE };
    const FinalStateSignature finalStateSignature;

    const std::string FinalStateSignatureLabel;

    // Nucleon labels ---------------------------------------------------------------------------------------------------------------------------------------------------

    const std::string NucFD_label;
    const std::string NucCD_label = "pCD";

    // ==================================================================================================================================================================
    // Constructor
    // ==================================================================================================================================================================

    FinalStateConfiguration(const ExperimentParameters& Experiment, const EventVariant& eventVariant, const FinalStateSignature& finalStateSignature)
        : eventVariant(InitEventVariant(Experiment, eventVariant)),  //
          finalStateSignature(finalStateSignature),                  //
          EventVariantLabel(InitEventVariantLabel()),                //
          FinalStateSignatureLabel(InitSignatureLabel()),            //
          NucFD_label(InitNucFDLabel())                              //
    {
        if (this->eventVariant == UNSET_EVENT_VARIANT) { bt::ExitWithError(__func__, __FILE__, __LINE__, "eventVariant is not set!"); }
        if (this->finalStateSignature == UNSET_SIGNATURE) { bt::ExitWithError(__func__, __FILE__, __LINE__, "finalStateSignature is not set!"); }

        if (EventVariantLabel == "") { bt::ExitWithError(__func__, __FILE__, __LINE__, "Unable to set EventVariantLabel!"); }
        if (FinalStateSignatureLabel == "") { bt::ExitWithError(__func__, __FILE__, __LINE__, "Unable to set FinalStateSignatureLabel!"); }
    }

    // ==================================================================================================================================================================
    // Functions
    // ==================================================================================================================================================================

    // InitEventVariant functions ---------------------------------------------------------------------------------------------------------------------------------------

    EventVariant InitEventVariant(const ExperimentParameters& Experiment, const EventVariant& eventVariant) const { return Experiment.IsDataSample() ? RECO_DATA_EVENT : eventVariant; }

    // InitEventVariantLabel functions ----------------------------------------------------------------------------------------------------------------------------------

    std::string InitEventVariantLabel() const { return (eventVariant == RECO_SIM_EVENT) ? "reco" : (eventVariant == TRUTH_LEVEL_SIM_EVENT) ? "truth" : ""; }

    // InitSignatureLabel functions -------------------------------------------------------------------------------------------------------------------------------------

    std::string InitSignatureLabel() const {
        return (finalStateSignature == FinalState_Inclusive)     ? "inclusive (e,e')"
               : (finalStateSignature == FinalState_1N_1pFD)     ? "1pFD"
               : (finalStateSignature == FinalState_1N_1nFD)     ? "1nFD"
               : (finalStateSignature == FinalState_2N_2p)       ? "2p"
               : (finalStateSignature == FinalState_2N_1pFD1pCD) ? "1pFD1pCD"
               : (finalStateSignature == FinalState_2N_1nFD1pCD) ? "1nFD1pCD"
                                                                 : "";
    }

    // InitNucFDLabel functions -----------------------------------------------------------------------------------------------------------------------------------------

    std::string InitNucFDLabel() const {
        return ((finalStateSignature == FinalState_1N_1pFD) || (finalStateSignature == FinalState_2N_1pFD1pCD))   ? "pFD"
               : ((finalStateSignature == FinalState_1N_1nFD) || (finalStateSignature == FinalState_2N_1nFD1pCD)) ? "nFD"
                                                                                                                  : "";
    }
};

#endif  // FINALSTATECONFIGURATION_H
