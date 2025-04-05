//
// Created by alons on 05/04/2025.
//

// #ifndef RECOANALYSER_H
// #define RECOANALYSER_H
#ifdef RECOANALYSER_H

    #pragma region /* Includes */

    // // Include settings:
    // #include "../structures/AcceptanceMapsSettings/AcceptanceMapsSettings.h"
    // #include "../structures/AnalysisCutSettings/CutSettings.h"
    // #include "../structures/CutValueManager/CutValueManager.h"
    // #include "../structures/EventSelectionSettings/EventSelectionSettings.h"
    // #include "../structures/MomentumResolutionSettings/MomentumResolutionSettings.h"
    // #include "../structures/RunParameters/RunParameters.h"
    // #include "../namespaces/printers/printers.h"
    
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
    #include "../framework/functions/FillByInt.h"
    #include "../framework/includes/clas12_include.h"
    #include "../framework/namespaces/fitters/fitter_functions.h"
    #include "../framework/namespaces/general_utilities/utilities.h"
    #include "../framework/namespaces/plotters/draw_and_save_functions/draw_and_save_functions.h"
    #include "../setup/codeSetup.h"

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

#endif  // RECOANALYSER_H
