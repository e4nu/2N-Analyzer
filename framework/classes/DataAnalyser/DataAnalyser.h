//
// Created by alons on 05/08/2024.
//

#ifndef DATAANALYSER_H
#define DATAANALYSER_H
// #ifdef DATAANALYSER_H

#include "../../applications/DetectorSimulationAnalyser.h"
// #include "../../applications/framework/applications/TruthLevelAnalyser.cpp"

class DataAnalyser {
   private:
   public:
    // default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    DataAnalyser(const std::string &FileType, const std::string &AnalyseFilePath, const std::string &AnalyseFileSample, const std::string &AnalyseFile);

    // ConfigureAnalyserMode function -----------------------------------------------------------------------------------------------------------------------------

    std::string ConfigureAnalyserMode(const std::string &FileType);

    // RunEventAnalyser functions ---------------------------------------------------------------------------------------------------------------------------------

    void RunEventAnalyser(const std::string &AnalyserMode, const std::string &AnalyseFilePath, const std::string &AnalyseFileSample, const std::string &AnalyseFile);
};

#endif  // DATAANALYSER_H
