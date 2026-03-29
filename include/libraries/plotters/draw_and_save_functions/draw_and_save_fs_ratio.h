//
// Created by Alon Sportes on 26/03/2025.
//

#ifndef DRAW_AND_SAVE_FS_RATIO_H
#define DRAW_AND_SAVE_FS_RATIO_H

#include <TH1D.h>
#include <TH2D.h>
#include <TList.h>

#include <string>

#include "../../AnalysisTools/hPlot1D.h"
#include "../../AnalysisTools/hPlot2D.h"
#include "../../libraries/general_utilities/utilities.h"

namespace draw_and_save_fs_ratio {

// DrawAndSaveFSRatio function -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveFSRatio function */
void DrawAndSaveFSRatio(const std::string& SampleName, const hPlot1D& pFDpCD_Plot, const hPlot1D& nFDpCD_Plot, TList* Histogram_list);
#pragma endregion

// DrawAndSaveFSRatio function -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveFSRatio function */
void DrawAndSaveFSRatio(const std::string& SampleName, const hPlot1D& pFDpCD_Plot, TH1D* nFDpCD_Plot, TList* Histogram_list);
#pragma endregion

// DrawAndSaveFSRatio function -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveFSRatio function */
void DrawAndSaveFSRatio(const std::string& SampleName, TH1D* pFDpCD_Plot, const std::string& pFDpCD_PlotSaveNamePath, TH1D* nFDpCD_Plot, TList* Histogram_list);
#pragma endregion

// DrawAndSaveFSRatio function -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveFSRatio function */
void DrawAndSaveFSRatio(const std::string& SampleName, const hPlot2D& pFDpCD_Plot, const hPlot2D& nFDpCD_Plot, TList* Histogram_list);
#pragma endregion

// DrawAndSaveFSRatio function -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveFSRatio function */
void DrawAndSaveFSRatio(const std::string& SampleName, const hPlot2D& pFDpCD_Plot, TH2D* nFDpCD_Plot, TList* Histogram_list);
#pragma endregion

// DrawAndSaveFSRatio function -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveFSRatio function */
void DrawAndSaveFSRatio(const std::string& SampleName, TH2D* pFDpCD_Plot, const std::string& pFDpCD_PlotSaveNamePath, TH2D* nFDpCD_Plot, TList* Histogram_list);
#pragma endregion

}  // namespace draw_and_save_fs_ratio

#endif  // DRAW_AND_SAVE_FS_RATIO_H
