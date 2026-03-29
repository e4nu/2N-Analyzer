//
// Created by Alon Sportes on 05/08/2024.
//

#ifndef DATAANALYZER_H
#define DATAANALYZER_H

#include <string>

// Include io:
#include "io/reco_cli.h"

// Include TwoNAnalyzer:
#include "TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.h"

namespace rcli = reco_cli;

class DataAnalyzer {
   private:
   public:
    // default constructor ----------------------------------------------------------------------------------------------------------------------------------------------

    DataAnalyzer(const std::string& FileType, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles, const rcli::RecoCliOptions& cli);

    // ConfigureAnalyzerMode function -----------------------------------------------------------------------------------------------------------------------------------

    std::string ConfigureAnalyzerMode(const std::string& FileType);

    // RunEventAnalyzer functions ---------------------------------------------------------------------------------------------------------------------------------------

    void RunEventAnalyzer(const std::string& AnalyzerMode, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles,
                          const rcli::RecoCliOptions& cli);
};

#endif  // DATAANALYZER_H
