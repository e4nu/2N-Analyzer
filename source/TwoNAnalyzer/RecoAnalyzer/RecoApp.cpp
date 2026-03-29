//
// Created by Alon Sportes on 09/03/2026.
//

#include "TwoNAnalyzer/RecoAnalyzer/RecoApp.h"

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

// Include setup:
#include "Settings/setup/code_setup.h"

// Include io:
#include "io/reco_cli.h"

// Include libraries:
#include "libraries/general_utilities/basic_tools.h"

// Include classes:
#include "AnalysisTools/DataAnalyzer.h"

using namespace code_setup;

namespace env = environment;
namespace bt = basic_tools;
namespace rcli = reco_cli;

int RunTwoNAnalyzerReco(int argc, char** argv) {
    auto start = std::chrono::system_clock::now();  // Start counting running time

    std::cout << env::RESET_COLOR;  // Reset printout color before running over the code

    rcli::RecoCliOptions cli = rcli::ParseRecoCliOptions(argc, argv);

    if (cli.help) {
        rcli::PrintRecoUsage(argv[0]);
        return 0;
    }

    if (cli.n_events == 0 || cli.n_events < -1) {
        std::cerr << "Invalid value for --n-events: " << cli.n_events << ". Use -1 for all events, or a positive integer." << std::endl;
        return 1;
    }

    if (cli.verbose) { rcli::PrintRecoCliOptions(cli); }

    LoadSampleChainFromYaml(cli.sample_chain_file);

    if (cli.sample_index >= static_cast<int>(RecoSamplePath_v.size())) {
        std::cerr << "sample-index out of range: " << cli.sample_index << ". Loaded sample chain has " << RecoSamplePath_v.size() << " entries." << std::endl;
        return 1;
    }

    TH1::AddDirectory(kFALSE);

    int Num_of_Analyzed_samples = 0;
    constexpr int lineWidth = 100;

    bt::PrintMessage("[RunTwoNAnalyzerReco] Looping over sample chain...", false);

    for (int i = 0; i < RecoSamplePath_v.size(); i++) {
        if (cli.sample_index >= 0 && i != cli.sample_index) continue;
        std::cout << "\n" << std::flush;

        ++Num_of_Analyzed_samples;

        bt::PrintTitleBlock("[RunTwoNAnalyzerReco] Processing sample number " + env::RESET_COLOR + bt::ToStringWithPrecision(Num_of_Analyzed_samples, 0) + env::SYSTEM_COLOR + " of " +
                                env::RESET_COLOR + bt::ToStringWithPrecision(RecoSamplePath_v.size(), 0),
                            '=', 100, env::COMPLETION_COLOR);

        std::string RecoSamplePath0 = RecoSamplePath_v.at(i);
        std::string ReconHipoDir0 = ReconHipoDir_v.at(i);
        std::string PathToHipoFilesDir0 = PathToHipoFilesDir_v.at(i);
        std::string InputHipoFiles0 = InputHipoFiles_v.at(i);

        std::string FileType = InputHipoFiles_v.at(i).substr(InputHipoFiles_v.at(i).find_last_of(".") + 1);

        bt::PrintTitleBlock("[RunTwoNAnalyzerReco] Received parameters");

        bt::PrintWrappedLabelValueColored("RecoSamplePath", RecoSamplePath0, lineWidth, 1);
        bt::PrintAlignedValueColored("ReconHipoDir", ReconHipoDir0, lineWidth, 1);
        bt::PrintWrappedLabelValueColored("PathToHipoFilesDir", PathToHipoFilesDir0, lineWidth, 1);
        bt::PrintWrappedLabelValueColored("InputHipoFiles", InputHipoFiles0, lineWidth, 1);
        bt::PrintAlignedValueColored("FileType", FileType, lineWidth, 1);
        std::cout << "\n" << std::flush;
        bt::PrintLineColored();

        std::cout << std::flush;

        DataAnalyzer Analysis(FileType, RecoSamplePath0, ReconHipoDir0, InputHipoFiles0, cli);

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

    return 0;
}
