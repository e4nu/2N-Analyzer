#include <iostream>

// Include setup:
#include "framework/namespaces/setup/code_setup.h"

// Include libraries:
#include "framework/namespaces/general_utilities/basic_tools.h"

// Include classes:
#include "framework/classes/DataAnalyzer/DataAnalyzer.cpp"

using namespace code_setup;

namespace env = environment;
namespace bt = basic_tools;

int main() {
    auto start = std::chrono::system_clock::now();  // Start counting running time

    bt::PrintBanner("2N Analyzer");

    ConfigSampleChain();

    TH1::AddDirectory(kFALSE);

    int Num_of_Analyzed_samples = 0;
    constexpr int lineWidth = 91;

    bt::PrintMessage("[main] Looping over sample chain...");

    for (int i = 0; i < RecoSamplePath_v.size(); i++) {
        std::cout << "\n" << std::flush;

        ++Num_of_Analyzed_samples;

        bt::PrintTitleBlock("Processing sample number " + env::RESET_COLOR + bt::ToStringWithPrecision(Num_of_Analyzed_samples, 0), '=', 91, env::COMPLETION_COLOR);

        std::string RecoSamplePath0 = RecoSamplePath_v.at(i);
        std::string ReconHipoDir0 = ReconHipoDir_v.at(i);
        std::string PathToHipoFilesDir0 = PathToHipoFilesDir_v.at(i);
        std::string InputHipoFiles0 = InputHipoFiles_v.at(i);

        std::string FileType = InputHipoFiles_v.at(i).substr(InputHipoFiles_v.at(i).find_last_of(".") + 1);

        bt::PrintTitleBlock("Received parameters");

        bt::PrintWrappedLabelValueColored("RecoSamplePath", RecoSamplePath0, lineWidth, 1);
        bt::PrintAlignedValueColored("ReconHipoDir", ReconHipoDir0, lineWidth, 1);
        bt::PrintWrappedLabelValueColored("PathToHipoFilesDir", PathToHipoFilesDir0, lineWidth, 1);
        bt::PrintWrappedLabelValueColored("InputHipoFiles", InputHipoFiles0, lineWidth, 1);
        bt::PrintAlignedValueColored("FileType", FileType, lineWidth, 1);
        std::cout << "\n" << std::flush;
        bt::PrintLineColored();

        std::cout << std::flush;

        DataAnalyzer Analysis(FileType, RecoSamplePath0, ReconHipoDir0, InputHipoFiles0);

        bt::PrintLineColored('-');
        std::cout << env::SYSTEM_COLOR << "Analysis mode:" << env::RESET_COLOR << "\t\t'" << Analysis.ConfigureAnalyzerMode(FileType) << "'" << "\n" << std::flush;
        std::cout << "\n\n" << std::flush;
    }

    if (RecoSamplePath_v.size() > 1 && zipping::Collect_all_zip_files) { CollectZipFiles(path_definitions::custom_plots_path_prefix); }

    std::cout << env::SYSTEM_COLOR << "#(Analyzed samples):" << env::RESET_COLOR << "\t" << Num_of_Analyzed_samples << "\n" << std::flush;

    auto end = std::chrono::system_clock::now();
    auto elapsed_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    double elapsed_time_minutes = elapsed_time_seconds.count() / 60;

    if (elapsed_time_seconds.count() < 60) {
        std::cout << env::SYSTEM_COLOR << "Running time:" << env::RESET_COLOR << "\t\t" << elapsed_time_seconds.count() << " seconds\n\n" << std::flush;
    } else {
        std::cout << env::SYSTEM_COLOR << "Running time:" << env::RESET_COLOR << "\t\t" << basic_tools::ToStringWithPrecision(elapsed_time_minutes, 3) << " minutes\n\n" << std::flush;
    }
}
