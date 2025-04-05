//
// Created by Alon Sportes on 05/04/2025.
//

// #ifndef RECOANALYZER_H
// #define RECOANALYZER_H
#ifdef RECOANALYZER_H

    #pragma region /* Includes */

    #include <iostream>

    // Include setup:
    #include "../setup/codeSetup.h"

    // Include libraries:
    #include "../framework/namespaces/fitters/fitter_functions.h"
    #include "../framework/namespaces/general_utilities/utilities.h"
    #include "../framework/namespaces/plotters/draw_and_save_functions/draw_and_save_functions.h"

    // Include settings:
    #include "../framework/structures/AcceptanceMapsSettings/AcceptanceMapsSettings.h"
    #include "../framework/structures/AnalysisCutSettings/AnalysisCutSettings.h"
    #include "../framework/structures/CutValueManager/CutValueManager.h"
    #include "../framework/structures/EventSelectionSettings/EventSelectionSettings.h"
    #include "../framework/structures/MomentumResolutionSettings/MomentumResolutionSettings.h"
    #include "../framework/structures/RunParameters/RunParameters.h"
    #include "../framework/namespaces/printers/printers.h"

    // Include classes:
    #include "../framework/classes/AMaps/AMaps.cpp"
    #include "../framework/classes/DEfficiency/DEfficiency.cpp"
    #include "../framework/classes/DSCuts/DSCuts.h"
    #include "../framework/classes/MomentumResolution/MomentumResolution.cpp"
    #include "../framework/classes/ParticleID/ParticleID.cpp"
    #include "../framework/classes/Settings/Settings.cpp"
    #include "../framework/classes/TLCuts/TLCuts.cpp"
    #include "../framework/classes/clas12ana/clas12ana.h"
    #include "../framework/classes/hPlots/hPlot1D.cpp"
    #include "../framework/classes/hPlots/hPlot2D.cpp"

    // Include CLAS12 libraries:
    #include "../framework/includes/clas12_include.h"
    #pragma endregion

using namespace utilities;
using namespace draw_and_save_functions;

class RecoAnalyzer {
   public:
    // RecoAnalyzer constructor ---------------------------------------------------------------------------------------------------------------------------------------------

    RecoAnalyzer(const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);

    ~RecoAnalyzer() = default;
};

#endif  // RECOANALYZER_H
