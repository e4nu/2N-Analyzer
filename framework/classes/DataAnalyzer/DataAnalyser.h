//
// Created by alons on 05/08/2024.
//

// #ifndef DATAAnalyzeR_H
// #define DATAAnalyzeR_H
#ifdef DATAAnalyzeR_H

#include "../../../../applications/DetectorSimulationAnalyzer.cpp"
#include "../../../../applications/TruthLevelAnalyzer.cpp"

class DataAnalyzer {
   private:
   public:
    // default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    DataAnalyzer(const std::string &FileType, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);

    // ConfigureAnalyzerMode function -----------------------------------------------------------------------------------------------------------------------------

    std::string ConfigureAnalyzerMode(const std::string &FileType);

    // RunEventAnalyzer functions ---------------------------------------------------------------------------------------------------------------------------------

    void RunEventAnalyzer(const std::string &AnalyzerMode, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);
};

#endif  // DATAAnalyzeR_H
