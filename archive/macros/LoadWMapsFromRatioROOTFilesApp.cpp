#include <iostream>
#include <string>

#ifndef AMAPS_H
    #define AMAPS_H
#endif
#include "../framework/classes/AMaps/AMaps.h"

namespace {
std::string EnsureTrailingSlash(std::string path) {
    if (!path.empty() && path.back() != '/') { path.push_back('/'); }
    return path;
}
}  // namespace

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <SampleName> <InputAcceptanceMapsDir> [OutputAcceptanceMapsDir]\n";
        return 2;
    }

    const std::string sampleName = argv[1];
    const std::string inputDir = EnsureTrailingSlash(argv[2]);
    const std::string outputDir = EnsureTrailingSlash((argc >= 4) ? argv[3] : argv[2]);

    std::cout << "[LoadWMapsFromRatioROOTFilesApp] sample: " << sampleName << "\n";
    std::cout << "[LoadWMapsFromRatioROOTFilesApp] input:  " << inputDir << "\n";
    std::cout << "[LoadWMapsFromRatioROOTFilesApp] output: " << outputDir << "\n";

    AMaps maps;
    maps.LoadWMapsFromRatioROOTFiles(sampleName, inputDir, outputDir);

    std::cout << "[LoadWMapsFromRatioROOTFilesApp] finished\n";
    return 0;
}
