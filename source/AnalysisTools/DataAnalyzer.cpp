//
// Created by Alon Sportes on 05/08/2024.
//

#include "DataAnalyzer.h"

#include <cstdlib>
#include <iostream>

// DataAnalyzer constructors ----------------------------------------------------------------------------------------------------------------------------------

/* Default constructor */
DataAnalyzer::DataAnalyzer(const std::string& FileType, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles,
                           const rcli::RecoCliOptions& cli) {
    std::string AnalyzerMode = ConfigureAnalyzerMode(FileType);
    this->RunEventAnalyzer(AnalyzerMode, RecoSamplePath, ReconHipoDir, InputHipoFiles, cli);
}

// ConfigureAnalyzerMode function -----------------------------------------------------------------------------------------------------------------------------

std::string DataAnalyzer::ConfigureAnalyzerMode(const std::string& FileType) {
    std::string AnalyzerMode;

    if (FileType == "root") {
        AnalyzerMode = "Truth level";
    } else if (FileType == "hipo") {
        AnalyzerMode = "Detector Simulation";
    } else {
        std::cout << "\nCould not find file type!! Aborting...\n\n", std::exit(1);
    }

    return AnalyzerMode;
}

// RunEventAnalyzer functions ---------------------------------------------------------------------------------------------------------------------------------

void DataAnalyzer::RunEventAnalyzer(const std::string& AnalyzerMode, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles,
                                    const rcli::RecoCliOptions& cli) {
    if (AnalyzerMode == "Detector Simulation") { RecoAnalyzer RecoEventAnalyzer = RecoAnalyzer(RecoSamplePath, ReconHipoDir, InputHipoFiles, cli); }
    // if (AnalyzerMode == "Truth level") {
    //     TruthAnalyzer TruthEventAnalyzer = TruthAnalyzer();
    // } else if (AnalyzerMode == "Detector Simulation") {
    //     RecoAnalyzer RecoEventAnalyzer = RecoAnalyzer(RecoSamplePath, ReconHipoDir, InputHipoFiles);
    // }
};
