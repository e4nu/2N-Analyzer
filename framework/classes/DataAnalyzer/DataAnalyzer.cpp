//
// Created by alons on 05/08/2024.
//

#ifndef DATAANALYZER_H
#define DATAANALYZER_H

#include "DataAnalyzer.h"

// DataAnalyzer constructors ----------------------------------------------------------------------------------------------------------------------------------

/* Default constructor */
DataAnalyzer::DataAnalyzer(const std::string &FileType, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile) {
    std::string AnalyzerMode = ConfigureAnalyzerMode(FileType);
    RunEventAnalyzer(AnalyzerMode, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFile);
}

// ConfigureAnalyzerMode function -----------------------------------------------------------------------------------------------------------------------------

string DataAnalyzer::ConfigureAnalyzerMode(const std::string &FileType) {
    std::string AnalyzerMode;

    if (FileType == "root") {
        AnalyzerMode = "Truth level";
    } else if (FileType == "hipo") {
        AnalyzerMode = "Detector Simulation";
    } else {
        cout << "\nCould not find file type!! Exiting...\n\n", exit(0);
    }

    return AnalyzerMode;
}

// RunEventAnalyzer functions ---------------------------------------------------------------------------------------------------------------------------------

void DataAnalyzer::RunEventAnalyzer(const std::string &AnalyzerMode, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile) {
    if (AnalyzerMode == "Detector Simulation") { RecoAnalyzer RecoEventAnalyzer = RecoAnalyzer(AnalyzeFilePath, AnalyzeFileSample, AnalyzeFile); }
    // if (AnalyzerMode == "Truth level") {
    //     TruthAnalyzer TruthEventAnalyzer = TruthAnalyzer();
    // } else if (AnalyzerMode == "Detector Simulation") {
    //     RecoAnalyzer RecoEventAnalyzer = RecoAnalyzer(AnalyzeFilePath, AnalyzeFileSample, AnalyzeFile);
    // }
};

#endif  // DATAANALYZER_H
