#include <iostream>

#include "framework/namespaces/setup/code_setup/code_setup.h"

#include "framework/classes/DataAnalyzer/DataAnalyzer.cpp"
#include "framework/namespaces/general_utilities/basic_tools/basic_tools.h"

// using namespace std;
using namespace code_setup;

int main() {
    code_setup::ConfigSampleChain();
    ConfigCanvasPDF();

    auto start = std::chrono::system_clock::now();  // Start counting running time

    int Num_of_Analyzed_samples = 0;
    
    cout << "\nLooping over sample chain...\n";
    
    for (int i = 0; i < AnalyzeFilePath_v.size(); i++) {
        std::string AnalyzeFilePath0 = AnalyzeFilePath_v.at(i);
        std::string AnalyzeFileSample0 = AnalyzeFileSample_v.at(i);
        std::string AnalyzeFileDir0 = AnalyzeFileDir_v.at(i);
        std::string AnalyzeFile0 = AnalyzeFile_v.at(i);
    
        std::string FileType = AnalyzeFile_v.at(i).substr(AnalyzeFile_v.at(i).find_last_of(".") + 1);
    
        cout << "\nAnalyzeFilePath:\t" << AnalyzeFilePath0 << "\n";
        cout << "AnalyzeFileSample:\t" << AnalyzeFileSample0 << "\n";
        cout << "AnalyzeFileDir:\t" << AnalyzeFileDir0 << "\n";
        cout << "AnalyzeFile:\t" << AnalyzeFile0 << "\n\n";
    
        cout << "FileType:\t" << FileType << "\n";
    
        DataAnalyzer Analysis(FileType, AnalyzeFilePath0, AnalyzeFileSample0, AnalyzeFile0);
        std::string AnalyzerMode = Analysis.ConfigureAnalyzerMode(FileType);
    
        cout << "Analyzer mode:\t'" << AnalyzerMode << "'\n";
    
        cout << "\n---------------------------------------------------------------------------\n\n";
        cout << "Analysis mode:\t'" << AnalyzerMode << "'" << "\n";
        cout << "Code version:\t" << Ver << "\n";
    
        ++Num_of_Analyzed_samples;
    
        if (AnalyzeFilePath_v.size() > 1) {  // Delete all ROOT objects whose class names start with TH (to prevent a memory leak)
            // gDirectory->Delete("TH*;*");
            gDirectory->Clear();
        }
    }

    cout << "#(Analyzed samples):\t" << Num_of_Analyzed_samples << "\n";

    auto end = std::chrono::system_clock::now();
    auto elapsed_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    double elapsed_time_minutes = elapsed_time_seconds.count() / 60;

    if (elapsed_time_seconds.count() < 60) {
        std::cout << "Running time:\t" << elapsed_time_seconds.count() << " seconds\n\n";
    } else {
        std::cout << "Running time:\t" << basic_tools::ToStringWithPrecision(elapsed_time_minutes, 3) << " minutes\n\n";
    }
}
