//
// Created by Alon Sportes on 13/03/2025.
//

#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <typeinfo>
#include <utility>
#include <vector>

#include "../basic_tools/basic_tools_include.h"
// #include "../utilities/utilities.h"

// using namespace std;

// TODO: split into header and source files!

namespace data_processor {
// using namespace basic_tools;

// GetFS function -------------------------------------------------------------------------------------------------------------------------------------------------------

std::string GetFS(const std::string &Source);

// GetParticleName function ---------------------------------------------------------------------------------------------------------------------------------------------

std::string GetParticleName(const std::string &Source, const bool &PluralParticles = false);

// GetParticleName1 function --------------------------------------------------------------------------------------------------------------------------------------------

std::string GetParticleName1(const std::string &Source, const bool &PluralParticles);

// GetParticleNameFromSubscript function --------------------------------------------------------------------------------------------------------------------------------

std::string GetParticleNameFromSubscript(const std::string &Source, const bool &PluralParticles);

// GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

std::string GetParticleNameLC(const std::string &Source, const bool &PluralParticles = false);

// GetParticleNameLCFromSubscript function ------------------------------------------------------------------------------------------------------------------------------

std::string GetParticleNameLCFromSubscript(const std::string &Source, const bool &PluralParticles);

// GetParticleNameShort function ----------------------------------------------------------------------------------------------------------------------------------------

std::string GetParticleNameShort(const std::string &Source);

// GetParticleNameShortFromSubscript function ---------------------------------------------------------------------------------------------------------------------------

std::string GetParticleNameShortFromSubscript(const std::string &Source);

// GetTopology function -------------------------------------------------------------------------------------------------------------------------------------------------

std::string GetTopology(const std::string &Source);

// GetType function -----------------------------------------------------------------------------------------------------------------------------------------------------

std::string GetType(const std::string &Source);

// GetDRegion function --------------------------------------------------------------------------------------------------------------------------------------------------

std::string GetDRegion(const std::string &Source, const bool &ReturnGoingRegion = false);

// GetDRegionExplicit function ------------------------------------------------------------------------------------------------------------------------------------------

std::string GetDRegionExplicit(const std::string &Source, const bool &ReturnGoingRegion);

// GetFSRTitle function -------------------------------------------------------------------------------------------------------------------------------------------------

std::string GetFSRTitle(const std::string &Source, const std::string &PlotsT);

// GetTitle function ----------------------------------------------------------------------------------------------------------------------------------------------------

std::string GetTitle(const std::string &Source);

// GetStatsTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

std::string GetStatsTitle(const std::string &Source);

// SetXLabel function ---------------------------------------------------------------------------------------------------------------------------------------------------

std::string SetXLabel(const std::string &Source);

// SetSaveDir function --------------------------------------------------------------------------------------------------------------------------------------------------

std::string SetSaveDir(const std::string &Source, const std::string &BaseSaveDir, const std::string &Mod = "");

void SetSaveDir(std::string &SaveDir, std::string &TestSaveDir, const std::string &RecTitle, const std::string &Histogram1DSaveNamePath, const std::string &PlotsT,
                const std::string &DRegion, const std::string &Particle, const std::string &ParticleLC, const std::string &ParticleShort, const std::string &Type);

// SetFSRatioSaveDir function -------------------------------------------------------------------------------------------------------------------------------------------

void SetFSRatioSaveDir(std::string &SaveDir, std::string &TestSaveDir, const std::string &RecTitle, const std::string &FinalState, const std::string &Histogram1DSaveNamePathOriginal,
                       const std::string &PlotsT, const std::string &DRegion, const std::string &Particle, const std::string &ParticleLC, const std::string &ParticleShort,
                       const std::string &Type);

// SetType function -----------------------------------------------------------------------------------------------------------------------------------------------------

std::string SetType(const std::string &Title);

// SetStatsTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

std::string SetStatsTitle(const std::string &RecTitle);

// SetTitle function ----------------------------------------------------------------------------------------------------------------------------------------------------

std::string SetTitle(const std::string &RecTitle, const std::string &PlotsT, const std::string &DRegion);

// SetXAxisTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

std::string SetXAxisTitle(const std::string &RecTitle);

// SetYAxisTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

std::string SetYAxisTitle(const std::string &PlotT, const std::string &FinalState, const std::string &Numerator, const std::string &Denominator, const std::string &Title = "");

// SettingSaveNames function --------------------------------------------------------------------------------------------------------------------------------------------

void SettingSaveNames(const std::string &SampleName, const std::string &Type, const std::string &Particle, const std::string &SaveDir, const std::string &TestSaveDir,
                      const std::string &PlotsT, std::string &Numerator_Clone_SaveName, std::string &Numerator_Clone_test_SaveName, std::string &Numerator_Clone_test_rebined_SaveName,
                      std::string &Denominator_Clone_SaveName, std::string &Denominator_Clone_test_SaveName, std::string &Denominator_Clone_test_rebined_SaveName, std::string &sNameFlag,
                      std::string &PlotSaveName, const std::string &DRegion = "", const std::string &FinalState = "");

};  // namespace data_processor

#endif  // DATA_PROCESSOR_H
