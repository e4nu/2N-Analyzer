//
// Created by alons on 05/08/2024.
//

// #ifndef DATAANALYSER_H
// #define DATAANALYSER_H
#ifdef DATAANALYSER_H

    #include "../../../applications/RecoAnalyser.cpp"
    #include "../../../applications/TruthAnalyser.cpp"

class DataAnalyser {
   private:
   public:
    // default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    DataAnalyser(const std::string &FileType, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);

    // ConfigureAnalyserMode function -----------------------------------------------------------------------------------------------------------------------------

    std::string ConfigureAnalyserMode(const std::string &FileType);

    // RunEventAnalyser functions ---------------------------------------------------------------------------------------------------------------------------------

    void RunEventAnalyser(const std::string &AnalyserMode, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);
};

#endif  // DATAANALYSER_H
