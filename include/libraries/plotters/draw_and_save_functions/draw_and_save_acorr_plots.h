//
// Created by Alon Sportes on 26/03/2025.
//

#ifndef DRAW_AND_SAVE_ACORR_PLOTS_H
#define DRAW_AND_SAVE_ACORR_PLOTS_H

#include <TH1D.h>
#include <TList.h>

#include <string>

#include "../../AnalysisTools/hPlot1D.h"
#include "../../libraries/general_utilities/utilities.h"

namespace draw_and_save_acorr_plots {

// DrawAndSaveACorrPlots function ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveACorrPlots function */
void DrawAndSaveACorrPlots(bool save_ACorr_data, const std::string& SampleName, const hPlot1D& TLPlot, const hPlot1D& RPlot, TList* Histogram_list, TList* ACorr_data,
                           std::string& ACorr_data_Dir);
#pragma endregion

// DrawAndSaveACorrPlots function ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveACorrPlots function */
void DrawAndSaveACorrPlots(bool save_ACorr_data, const std::string& SampleName, const hPlot1D& TLPlot, TH1D* RPlot, TList* Histogram_list, TList* ACorr_data, std::string& ACorr_data_Dir);
#pragma endregion

}  // namespace draw_and_save_acorr_plots

#endif  // DRAW_AND_SAVE_ACORR_PLOTS_H
