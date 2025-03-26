//
// Created by alons on 05/08/2024.
//

#include "DataAnalyser.h"

// DataAnalyser constructors ----------------------------------------------------------------------------------------------------------------------------------

/* Default constructor */
DataAnalyser::DataAnalyser(const std::string &FileType, const std::string &AnalyseFilePath, const std::string &AnalyseFileSample, const std::string &AnalyseFile) {
    std::string AnalyserMode = ConfigureAnalyserMode(FileType);
    RunEventAnalyser(AnalyserMode, AnalyseFilePath, AnalyseFileSample, AnalyseFile);
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

void DataAnalyser::RunEventAnalyser(const std::string &AnalyserMode, const std::string &AnalyseFilePath, const std::string &AnalyseFileSample, const std::string &AnalyseFile) {

    if (AnalyserMode == "Truth level") {
        gst g;
        g.Loop();
    } else if (AnalyserMode == "Detector Simulation") {
        EventAnalyser(AnalyseFilePath, AnalyseFileSample, AnalyseFile);
    }
};
