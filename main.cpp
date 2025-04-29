#include <iostream>

// Include setup:
#include "framework/namespaces/setup/code_setup.h"

// Include libraries:
#include "framework/namespaces/general_utilities/basic_tools.h"

// Include classes:
#include "framework/classes/DataAnalyzer/DataAnalyzer.cpp"

using namespace code_setup;

int main() {
    ConfigSampleChain();
    // ConfigCanvasPDF();

    auto start = std::chrono::system_clock::now();  // Start counting running time

    int Num_of_Analyzed_samples = 0;
    std::cout << "\033[33m" << "\nLooping over sample chain...\n" << "\033[0m";

    for (int i = 0; i < AnalyzeFilePath_v.size(); i++) {
        std::string AnalyzeFilePath0 = AnalyzeFilePath_v.at(i);
        std::string AnalyzeFileSample0 = AnalyzeFileSample_v.at(i);
        std::string AnalyzeFileDir0 = AnalyzeFileDir_v.at(i);
        std::string AnalyzeFile0 = AnalyzeFile_v.at(i);

        std::string FileType = AnalyzeFile_v.at(i).substr(AnalyzeFile_v.at(i).find_last_of(".") + 1);

        std::cout << "\033[33m" << "\nAnalyzeFilePath:" << "\033[0m" << "\t" << AnalyzeFilePath0 << "\n";
        std::cout << "\033[33m" << "AnalyzeFileSample:" << "\033[0m" << "\t" << AnalyzeFileSample0 << "\n";
        std::cout << "\033[33m" << "AnalyzeFileDir:" << "\033[0m" << "\t\t" << AnalyzeFileDir0 << "\n";
        std::cout << "\033[33m" << "AnalyzeFile:" << "\033[0m" << "\t\t" << AnalyzeFile0 << "\n\n";

        std::cout << "\033[33m" << "FileType:" << "\033[0m" << "\t\t" << FileType << "\n";

        DataAnalyzer Analysis(FileType, AnalyzeFilePath0, AnalyzeFileSample0, AnalyzeFile0);

        std::cout << "\033[33m" << "\n---------------------------------------------------------------------------\n\n" << "\033[0m";
        std::cout << "\033[33m" << "Analysis mode:" << "\033[0m" << "\t\t'" << Analysis.ConfigureAnalyzerMode(FileType) << "'" << "\n";
        std::cout << "\033[33m" << "Code version:" << "\033[0m" << "\t\t" << Ver << "\n";

        ++Num_of_Analyzed_samples;

        // Delete all ROOT objects whose class names start with TH (to prevent a memory leak):
        if (AnalyzeFilePath_v.size() > 1) { gDirectory->Clear(); }
    }

    if (AnalyzeFilePath_v.size() > 1 && zipping::Collect_all_zip_files) { CollectZipFiles(path_definitions::custom_plots_path_prefix); }

    std::cout << "\033[33m" << "#(Analyzed samples):" << "\033[0m" << "\t" << Num_of_Analyzed_samples << "\n";

    auto end = std::chrono::system_clock::now();
    auto elapsed_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    double elapsed_time_minutes = elapsed_time_seconds.count() / 60;

    if (elapsed_time_seconds.count() < 60) {
        std::cout << "\033[33m" << "Running time:" << "\033[0m" << "\t\t" << elapsed_time_seconds.count() << " seconds\n\n";
    } else {
        std::cout << "\033[33m" << "Running time:" << "\033[0m" << "\t\t" << basic_tools::ToStringWithPrecision(elapsed_time_minutes, 3) << " minutes\n\n";
    }
}
