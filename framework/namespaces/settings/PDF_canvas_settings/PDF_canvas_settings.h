//#ifndef PDF_CANVAS_SETTINGS_H
//#define PDF_CANVAS_SETTINGS_H
#ifdef PDF_CANVAS_H

// ======================================================================================================================================================================
// PDF file canvas settings
// ======================================================================================================================================================================

namespace PDF_canvas_settings {
// Canvas variables -----------------------------------------------------------------------------------------------------------------------------------------------------

int num_of_hist_per_row = 4;
int num_of_hist_per_col = 3;

int CanvasPDF_ind = 1;

TCanvas* CanvasPDF = new TCanvas("CanvasPDF", "CanvasPDF", 1000 * num_of_hist_per_row, 750 * num_of_hist_per_col);  // normal res
}

#endif  // PDF_CANVAS_SETTINGS_H
