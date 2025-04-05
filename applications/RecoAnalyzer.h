//
// Created by alons on 05/04/2025.
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

    // // Include settings:
    // #include "../structures/AcceptanceMapsSettings/AcceptanceMapsSettings.h"
    // #include "../structures/AnalysisCutSettings/CutSettings.h"
    // #include "../structures/CutValueManager/CutValueManager.h"
    // #include "../structures/EventSelectionSettings/EventSelectionSettings.h"
    // #include "../structures/MomentumResolutionSettings/MomentumResolutionSettings.h"
    // #include "../structures/RunParameters/RunParameters.h"
    // #include "../namespaces/printers/printers.h"
    
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

// #include "HipoChain.h"
// #include "clas12reader.h"

// using namespace std;
// using namespace clas12;
using namespace utilities;
using namespace draw_and_save_functions;
    #pragma endregion

class RecoAnalyzer {
   public:
    // RecoAnalyzer constructor ---------------------------------------------------------------------------------------------------------------------------------------------

    RecoAnalyzer(const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);

    ~RecoAnalyzer() = default;
};

#endif  // RECOANALYZER_H
