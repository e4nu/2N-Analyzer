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

    DataAnalyzer(const std::string &FileType, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);

    // ConfigureAnalyzerMode function -----------------------------------------------------------------------------------------------------------------------------

    std::string ConfigureAnalyzerMode(const std::string &FileType);

    // RunEventAnalyzer functions ---------------------------------------------------------------------------------------------------------------------------------

    void RunEventAnalyzer(const std::string &AnalyzerMode, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);
};

#endif  // DATAANALYZER_H
