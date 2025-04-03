//
// Created by alons on 05/08/2024.
//

#ifndef DATAANALYZER_H
#define DATAANALYZER_H
// // #ifndef DATAANALYZER_H
// // #define DATAANALYZER_H
// #ifdef DATAANALYZER_H

// Include settings:
#include "../structures/AcceptanceMapsSettings/AcceptanceMapsSettings.h"
#include "../structures/AnalysisCutSettings/AnalysisCutSettings.h"
#include "../structures/CutValueManager/CutValueManager.h"
#include "../structures/EventSelectionSettings/EventSelectionSettings.h"
#include "../structures/MomentumResolutionSettings/MomentumResolutionSettings.h"
#include "../structures/RunParameters/RunParameters.h"

// #include "../../applications/DetectorSimulationAnalyser.cpp"
#include "../../applications/TruthLevelAnalyser.cpp"

class DataAnalyzer {
   private:
    // RecoEventAnalyser function ---------------------------------------------------------------------------------------------------------------------------------------

    void RecoEventAnalyser(const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);

    // RecoEventAnalyser function ---------------------------------------------------------------------------------------------------------------------------------------

    void RecoCutsPrintOut(const std::string &workingDirectory, const std::string &run_plots_path, const std::string &analyseFilePath, const std::string &analyseFileSample,
                          const std::string &analyseFile, const std::string &fileName, const RunParameters &parameters, const AnalysisCutSettings &CutSettings);

   public:
    // default constructor ----------------------------------------------------------------------------------------------------------------------------------------------

    DataAnalyzer(const std::string &FileType, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);

    // ConfigureAnalyserMode function -----------------------------------------------------------------------------------------------------------------------------------

    std::string ConfigureAnalyserMode(const std::string &FileType);

    // RunEventAnalyser function ----------------------------------------------------------------------------------------------------------------------------------------

    void RunEventAnalyser(const std::string &AnalyserMode, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);
};

#endif  // DATAANALYZER_H
