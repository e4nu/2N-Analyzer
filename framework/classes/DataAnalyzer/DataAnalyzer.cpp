//
// Created by Alon Sportes on 05/08/2024.
//

#ifndef DATAANALYZER_H
#define DATAANALYZER_H

#include "DataAnalyzer.h"

// DataAnalyzer constructors ----------------------------------------------------------------------------------------------------------------------------------

/* Default constructor */
DataAnalyzer::DataAnalyzer(const std::string& FileType, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles) {
    std::string AnalyzerMode = ConfigureAnalyzerMode(FileType);
    this->RunEventAnalyzer(AnalyzerMode, RecoSamplePath, ReconHipoDir, InputHipoFiles);
}

// ConfigureAnalyzerMode function -----------------------------------------------------------------------------------------------------------------------------

string DataAnalyzer::ConfigureAnalyzerMode(const std::string& FileType) {
    std::string AnalyzerMode;

    if (FileType == "root") {
        AnalyzerMode = "Truth level";
    } else if (FileType == "hipo") {
        AnalyzerMode = "Detector Simulation";
    } else {
        cout << "\nCould not find file type!! Aborting...\n\n", exit(1);
    }

    return AnalyzerMode;
}

// RunEventAnalyzer functions ---------------------------------------------------------------------------------------------------------------------------------

void DataAnalyzer::RunEventAnalyzer(const std::string& AnalyzerMode, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles) {
    if (AnalyzerMode == "Detector Simulation") { RecoAnalyzer RecoEventAnalyzer = RecoAnalyzer(RecoSamplePath, ReconHipoDir, InputHipoFiles); }
    // if (AnalyzerMode == "Truth level") {
    //     TruthAnalyzer TruthEventAnalyzer = TruthAnalyzer();
    // } else if (AnalyzerMode == "Detector Simulation") {
    //     RecoAnalyzer RecoEventAnalyzer = RecoAnalyzer(RecoSamplePath, ReconHipoDir, InputHipoFiles);
    // }
};

#endif  // DATAANALYZER_H
