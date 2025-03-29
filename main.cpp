#include <iostream>

#include "framework/namespaces/setup/code_setup/code_setup.h"

#include "framework/classes/DataAnalyser/DataAnalyser.h"
#include "framework/namespaces/general_utilities/basic_tools/basic_tools.h"

// using namespace std;
using namespace code_setup;

int main() {
    code_setup::Ver = "DetSim testings";
    
    code_setup::ConfigSampleChain();
    PDF_canvas::ConfigCanvasPDF();

    auto start = std::chrono::system_clock::now();  // Start counting running time

    int Num_of_analysed_samples = 0;
    
    std::cout << "\nLooping over sample chain...\n";
    
    for (int i = 0; i < AnalyseFilePath_v.size(); i++) {
        std::string AnalyseFilePath0 = AnalyseFilePath_v.at(i);
        std::string AnalyseFileSample0 = AnalyseFileSample_v.at(i);
        std::string AnalyseFileDir0 = AnalyseFileDir_v.at(i);
        std::string AnalyseFile0 = AnalyseFile_v.at(i);
    
        std::string FileType = AnalyseFile_v.at(i).substr(AnalyseFile_v.at(i).find_last_of(".") + 1);
    
        std::cout << "\nAnalyseFilePath:\t" << AnalyseFilePath0 << "\n";
        std::cout << "AnalyseFileSample:\t" << AnalyseFileSample0 << "\n";
        std::cout << "AnalyseFileDir:\t" << AnalyseFileDir0 << "\n";
        std::cout << "AnalyseFile:\t" << AnalyseFile0 << "\n\n";
    
        std::cout << "FileType:\t" << FileType << "\n";
    
        DataAnalyser Analysis(FileType, AnalyseFilePath0, AnalyseFileSample0, AnalyseFile0);
        std::string AnalyserMode = Analysis.ConfigureAnalyserMode(FileType);
    
        std::cout << "Analyser mode:\t'" << AnalyserMode << "'\n";
    
        std::cout << "\n---------------------------------------------------------------------------\n\n";
        std::cout << "Analysis mode:\t'" << AnalyserMode << "'" << "\n";
        std::cout << "Code version:\t" << Ver << "\n";
    
        ++Num_of_analysed_samples;
    
        if (AnalyseFilePath_v.size() > 1) {  // Delete all ROOT objects whose class names start with TH (to prevent a memory leak)
            // gDirectory->Delete("TH*;*");
            gDirectory->Clear();
        }
    }

    std::cout << "#(analysed samples):\t" << Num_of_analysed_samples << "\n";

    auto end = std::chrono::system_clock::now();
    auto elapsed_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    double elapsed_time_minutes = elapsed_time_seconds.count() / 60;

    if (elapsed_time_seconds.count() < 60) {
        std::cout << "Running time:\t" << elapsed_time_seconds.count() << " seconds\n\n";
    } else {
        std::cout << "Running time:\t" << basic_tools::ToStringWithPrecision(elapsed_time_minutes, 3) << " minutes\n\n";
    }
}
