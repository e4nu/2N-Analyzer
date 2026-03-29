//
// Created by Alon Sportes on 13/03/2025.
//

#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "TString.h"

namespace data_processor {

using std::get;
using std::map;
using std::pair;
using std::tuple;

// FindAnySubstring function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* FindAnySubstring function */
bool FindAnySubstring(const std::string& source, const std::string& patternList);
#pragma endregion

// GetFS function --------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetFS function */
std::string GetFS(const std::string& Source);
#pragma endregion

// GetParticleName function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetParticleName function */
std::string GetParticleName(const std::string& Source, const bool& PluralParticles = false);
#pragma endregion

// GetParticleName1 function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetParticleName1 function */
std::string GetParticleName1(const std::string& Source, const bool& PluralParticles);
#pragma endregion

// GetParticleNameFromSubscript function ---------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetParticleNameFromSubscript function */
std::string GetParticleNameFromSubscript(const std::string& Source, const bool& PluralParticles);
#pragma endregion

// GetParticleNameLC function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetParticleNameLC function */
std::string GetParticleNameLC(const std::string& Source, const bool& PluralParticles = false);
#pragma endregion

// GetParticleNameLCFromSubscript function -------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetParticleNameLCFromSubscript function */
std::string GetParticleNameLCFromSubscript(const std::string& Source, const bool& PluralParticles);
#pragma endregion

// GetParticleNameShort function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetParticleNameShort function */
std::string GetParticleNameShort(const std::string& Source);
#pragma endregion

// GetParticleNameShortFromSubscript function ----------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetParticleNameShortFromSubscript function */
std::string GetParticleNameShortFromSubscript(const std::string& Source);
#pragma endregion

// GetTopology function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetTopology function */
std::string GetTopology(const std::string& Source);
#pragma endregion

// GetType function ------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetType function */
std::string GetType(const std::string& Source);
#pragma endregion

// GetDRegion function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetDRegion function */
std::string GetDRegion(const std::string& Source, const bool& ReturnGoingRegion = false);
#pragma endregion

// GetDRegionExplicit function -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetDRegionExplicit function */
std::string GetDRegionExplicit(const std::string& Source, const bool& ReturnGoingRegion);
#pragma endregion

// GetFSRTitle function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetFSRTitle function */
std::string GetFSRTitle(const std::string& Source, const std::string& PlotsT);
#pragma endregion

// GetTitle function -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetTitle function */
std::string GetTitle(const std::string& Source);
#pragma endregion

// GetStatsTitle function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetStatsTitle function */
std::string GetStatsTitle(const std::string& Source);
#pragma endregion

// SetXLabel function ----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetXLabel function */
std::string SetXLabel(const std::string& Source);
#pragma endregion

// SetSaveDir function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetSaveDir function */
std::string SetSaveDir(const std::string& Source, const std::string& BaseSaveDir, const std::string& Mod = "");
#pragma endregion

// SetSaveDir function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetSaveDir function */
void SetSaveDir(std::string& SaveDir, std::string& TestSaveDir, const std::string& RecTitle, const std::string& Histogram1DSaveNamePath, const std::string& PlotsT,
                const std::string& DRegion, const std::string& Particle, const std::string& ParticleLC, const std::string& ParticleShort, const std::string& Type);
#pragma endregion

// SetFSRatioSaveDir function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetFSRatioSaveDir function */
void SetFSRatioSaveDir(std::string& SaveDir, std::string& TestSaveDir, const std::string& RecTitle, const std::string& FinalState, const std::string& Histogram1DSaveNamePathOriginal,
                       const std::string& PlotsT, const std::string& DRegion, const std::string& Particle, const std::string& ParticleLC, const std::string& ParticleShort,
                       const std::string& Type);
#pragma endregion

// SetType function ------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetType function */
std::string SetType(const std::string& Title);
#pragma endregion

// SetStatsTitle function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetStatsTitle function */
std::string SetStatsTitle(const std::string& RecTitle);
#pragma endregion

// SetTitle function -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetTitle function */
std::string SetTitle(const std::string& RecTitle, const std::string& PlotsT, const std::string& DRegion);
#pragma endregion

// SetXAxisTitle function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetXAxisTitle function */
std::string SetXAxisTitle(const std::string& RecTitle);
#pragma endregion

// SetYAxisTitle function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetYAxisTitle function */
std::string SetYAxisTitle(const std::string& PlotT, const std::string& FinalState, const std::string& Numerator, const std::string& Denominator, const std::string& Title = "");
#pragma endregion

// SettingSaveNames function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SettingSaveNames function */
void SettingSaveNames(const std::string& SampleName, const std::string& Type, const std::string& Particle, const std::string& SaveDir, const std::string& TestSaveDir,
                      const std::string& PlotsT, std::string& Numerator_Clone_SaveName, std::string& Numerator_Clone_test_SaveName, std::string& Numerator_Clone_test_rebined_SaveName,
                      std::string& Denominator_Clone_SaveName, std::string& Denominator_Clone_test_SaveName, std::string& Denominator_Clone_test_rebined_SaveName, std::string& sNameFlag,
                      std::string& PlotSaveName, const std::string& DRegion = "", const std::string& FinalState = "");
#pragma endregion

}  // namespace data_processor

#endif  // DATA_PROCESSOR_H
