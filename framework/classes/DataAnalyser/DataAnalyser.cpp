//
// Created by alons on 05/08/2024.
//

#ifndef DATAANALYSER_H
#define DATAANALYSER_H

#include "DataAnalyser.h"

// DataAnalyser constructors ----------------------------------------------------------------------------------------------------------------------------------

/* Default constructor */
DataAnalyser::DataAnalyser(const std::string &FileType, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile) {
    std::string AnalyserMode = ConfigureAnalyserMode(FileType);
    RunEventAnalyser(AnalyserMode, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFile);
}

// ConfigureAnalyserMode function -----------------------------------------------------------------------------------------------------------------------------

string DataAnalyser::ConfigureAnalyserMode(const std::string &FileType) {
    std::string AnalyserMode;

    if (FileType == "root") {
        AnalyserMode = "Truth level";
    } else if (FileType == "hipo") {
        AnalyserMode = "Detector Simulation";
    } else {
        cout << "\nCould not find file type!! Exiting...\n\n", exit(0);
    }

    return AnalyserMode;
}

// RunEventAnalyser functions ---------------------------------------------------------------------------------------------------------------------------------

void DataAnalyser::RunEventAnalyser(const std::string &AnalyserMode, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile) {
    if (AnalyserMode == "Truth level") {
        TruthAnalyser TruthEventAnalyser = TruthAnalyser();
    } else if (AnalyserMode == "Detector Simulation") {
        RecoAnalyser RecoEventAnalyser = RecoAnalyser(AnalyzeFilePath, AnalyzeFileSample, AnalyzeFile);
    }
};

#endif  // DATAANALYSER_H
