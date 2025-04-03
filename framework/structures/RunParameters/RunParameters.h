//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef RUNPARAMETERS_H
#define RUNPARAMETERS_H

#include <iostream>

#include "../../classes/ExperimentParameters/ExperimentParameters.cpp"

struct RunParameters {
    std::string SampleName;
    std::string VaryingSampleName;
    double beamE;
    std::string Target;
    int TargetPDG;
    bool isLocal;
    bool isMC;
    bool isData;
    bool is2GeVSample;
    bool is4GeVSample;
    bool is6GeVSample;

    RunParameters(const ExperimentParameters& Experiment, const std::string& AnalyzeFilePath, const std::string& AnalyzeFileSample)
        : SampleName(Experiment.ConfigureSampleName(AnalyzeFilePath, AnalyzeFileSample)),
          VaryingSampleName(Experiment.GetVaryingSampleName()),
          beamE(Experiment.GetBeamEnergy()),
          Target(Experiment.GetTargetElement()),
          TargetPDG(Experiment.GetTargetElementPDG()),
          isLocal(Experiment.SLocal()),
          isMC(Experiment.SSample()),
          isData(Experiment.DSample()),
          is2GeVSample(Experiment.IsBeamAt2GeV()),
          is4GeVSample(Experiment.IsBeamAt4GeV()),
          is6GeVSample(Experiment.IsBeamAt6GeV()) {}
};

#endif  // RUNPARAMETERS_H
