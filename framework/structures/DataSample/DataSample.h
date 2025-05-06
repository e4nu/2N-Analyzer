//
// Created by Alon Sportes on 23/03/2025.
//

#ifndef DATASAMPLE_H
#define DATASAMPLE_H

#include <iostream>

struct DataSample {
    std::string SampleFilePath = "";
    std::string SampleFileName = "";
    std::string SampleFileDir = "";
    std::string SampleFile = "";
    std::string SampleName = "";

    bool IsSimulationSample = false;
    bool IsDataSample = true;

    bool BeamAt2GeV = false;
    bool BeamAt4GeV = false;
    bool BeamAt6GeV = false;

    std::vector<std::string> SampleRuns;
};

#endif  // DATASAMPLE_H
