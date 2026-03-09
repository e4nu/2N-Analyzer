//
// Created by Alon Sportes on 05/08/2024.
//

// #ifndef DATAANALYZER_H
// #define DATAANALYZER_H
#ifdef DATAANALYZER_H

    #include "../../../applications/RecoAnalyzer.cpp"
// #include "../../../applications/TruthAnalyzer.cpp"

class DataAnalyzer {
   private:
   public:
    // default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    DataAnalyzer(const std::string& FileType, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles);

    // ConfigureAnalyzerMode function -----------------------------------------------------------------------------------------------------------------------------

    std::string ConfigureAnalyzerMode(const std::string& FileType);

    // RunEventAnalyzer functions ---------------------------------------------------------------------------------------------------------------------------------

    void RunEventAnalyzer(const std::string& AnalyzerMode, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles);
};

#endif  // DATAANALYZER_H
