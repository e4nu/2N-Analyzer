//
// Created by Alon Sportes on 26/03/2025.
//

#ifndef DRAW_AND_SAVE_EFFICIENCY_PLOTS_H
#define DRAW_AND_SAVE_EFFICIENCY_PLOTS_H

#include <TH1D.h>
#include <TList.h>

#include <string>

#include "../../AnalysisTools/hPlot1D.h"
#include "../../libraries/general_utilities/utilities.h"

namespace draw_and_save_efficiency_plots {

// DrawAndSaveEfficiencyPlots function -----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveEfficiencyPlots function */
void DrawAndSaveEfficiencyPlots(const std::string& SampleName, const hPlot1D& TLPlot, const hPlot1D& RPlot, TList* Histogram_list);
#pragma endregion

// DrawAndSaveEfficiencyPlots function -----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveEfficiencyPlots function */
void DrawAndSaveEfficiencyPlots(const std::string& SampleName, const hPlot1D& TLPlot, TH1D* RPlot, TList* Histogram_list);
#pragma endregion

}  // namespace draw_and_save_efficiency_plots

#endif  // DRAW_AND_SAVE_EFFICIENCY_PLOTS_H
