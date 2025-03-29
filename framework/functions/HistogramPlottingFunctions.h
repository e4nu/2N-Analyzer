//
// Created by alons on 27/01/2023.
//

#ifndef HISTOGRAMPLOTTINGFUNCTIONS_H
#define HISTOGRAMPLOTTINGFUNCTIONS_H

#include <TCanvas.h>
#include <TF1.h>
#include <TFrame.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TLine.h>
#include <TList.h>
#include <TPaveText.h>

#include <iostream>

#if IndependentCanvasPDF
#include "../includes/CanvasPDF/CanvasPDF.h"
#endif

#include "../namespaces/general_utilities/utilities/utilities.h"
// #include "GeneralFunctions.h"

using namespace utilities;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                   histPlotter1D function                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// histPlotter1D function (old) -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (old)">
void histPlotter1D(TCanvas *HistogramCanvas,            // The canvas
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle, std::string Histogram1DTitleReactions, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list,
                   const char *Histogram_OutPDF, int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName,
                   const std::string &Histogram1DSaveNamePath, std::string finalState, int kColor = 1, bool centerTitle = true, bool AddToStack = false, bool ShowStats = true,
                   bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0, bool plot_max = true);
//</editor-fold>

//<editor-fold desc="histPlotter1D function (old, stackless)">
void histPlotter1D(TCanvas *HistogramCanvas,            // The canvas
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle, std::string Histogram1DTitleReactions, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list,
                   const char *Histogram_OutPDF, int lineWidth, bool LogScalePlot, bool LinearScalePlot, std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath,
                   std::string finalState, bool centerTitle = true, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0,
                   bool plot_max = true);
//</editor-fold>

// histPlotter1D function (unsymmetric cuts for SF plots) ---------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (unsymmetric cuts for SF plots)">
void histPlotter1D(TCanvas *HistogramCanvas,            // The canvas
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, const char *Histogram_OutPDF, int lineWidth, bool LogScalePlot,
                   bool LinearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath, std::string finalState, int kColor = 1,
                   bool AddToStack = false, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_upper_cut = 0, double plot_lower_cut = 0,
                   double plot_Xmax = 0, bool plot_max = true);
//</editor-fold>

//<editor-fold desc="histPlotter1D function (unsymmetric cuts for SF plots, stackless)">
void histPlotter1D(TCanvas *HistogramCanvas,            // The canvas
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, const char *Histogram_OutPDF, int lineWidth, bool LogScalePlot,
                   bool LinearScalePlot, std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath, std::string finalState, bool ShowStats = true, bool title2 = false,
                   bool apply_plot_cuts = false, double plot_upper_cut = 0, double plot_lower_cut = 0, double plot_Xmax = 0, bool plot_max = true);
//</editor-fold>

// histPlotter1D function (one-sided cuts for Nphe plots) ---------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (one-sided cuts for Nphe plots)">
void histPlotter1D(TCanvas *HistogramCanvas,            // The canvas
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, const char *Histogram_OutPDF, int lineWidth, bool LogScalePlot,
                   bool LinearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath, std::string finalState, int kColor = 1,
                   bool AddToStack = false, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cut = 0, double plot_Xmax = 0, bool plot_max = true);
//</editor-fold>

//<editor-fold desc="histPlotter1D function (one-sided cuts for Nphe plots, stackless)">
void histPlotter1D(TCanvas *HistogramCanvas,            // The canvas
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, const char *Histogram_OutPDF, int lineWidth, bool LogScalePlot,
                   bool LinearScalePlot, std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath, std::string finalState, bool ShowStats = true, bool title2 = false,
                   bool apply_plot_cuts = false, double plot_cut = 0, double plot_Xmax = 0, bool plot_max = true);
//</editor-fold>

// histPlotter1D function (Chi2 for separate plots) ---------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (Chi2 for separate plots)">
void histPlotter1D(TCanvas *HistogramCanvas1,           // canvas c1 of other histograms
                   TCanvas *HistogramCanvas,            // canvas c2 of chi2 histograms
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle1, std::string Histogram1DTitle2, std::string Histogram1DTitle3, std::string Particle, double titleSize, double labelSizex, double labelSizey,
                   TList *Histogram_list, const char *Histogram_OutPDF, int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName,
                   const std::string &Histogram1DSaveNamePath, int kColor = 1, bool centerTitle = true, bool AddToStack = false, bool ShowStats = true, bool title2 = false,
                   bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0, bool plot_max = true);
//</editor-fold>

//<editor-fold desc="histPlotter1D function (Chi2 for separate plots, stackless)">
void histPlotter1D(TCanvas *HistogramCanvas1,           // canvas c1 of other histograms
                   TCanvas *HistogramCanvas,            // canvas c2 of chi2 histograms
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle1, std::string Histogram1DTitle2, std::string Histogram1DTitle3, std::string Particle, double titleSize, double labelSizex, double labelSizey,
                   TList *Histogram_list, const char *Histogram_OutPDF, int lineWidth, bool LogScalePlot, bool LinearScalePlot, std::string Histogram1DSaveName,
                   const std::string &Histogram1DSaveNamePath, bool centerTitle = true, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0,
                   double plot_Xmax = 0, bool plot_max = true);
//</editor-fold>

// histPlotter1D function (applying all Chi2 cuts) ----------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (applying all Chi2 cuts)">
void histPlotter1D(TCanvas *HistogramCanvas1,           // canvas c1 of other histograms
                   TCanvas *HistogramCanvas,            // canvas c2 of chi2 histograms
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle1, std::string Histogram1DTitle2, std::string Histogram1DTitle3, std::string Particle1, std::string Particle2, double titleSize,
                   double labelSizex, double labelSizey, TList *Histogram_list, const char *Histogram_OutPDF, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                   THStack *Histogram1DStack, std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath, int kColor = 1, bool centerTitle = true, bool AddToStack = false,
                   bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts1 = 0, double plot_Xmax1 = 0, double plot_cuts2 = 0, double plot_Xmax2 = 0,
                   bool plot_max = true);
//</editor-fold>

//<editor-fold desc="histPlotter1D function (applying all Chi2 cuts, stackless)">
void histPlotter1D(TCanvas *HistogramCanvas1,           // canvas c1 of other histograms
                   TCanvas *HistogramCanvas,            // canvas c2 of chi2 histograms
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle1, std::string Histogram1DTitle2, std::string Histogram1DTitle3, std::string Particle1, std::string Particle2, double titleSize,
                   double labelSizex, double labelSizey, TList *Histogram_list, const char *Histogram_OutPDF, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                   std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath, bool centerTitle = true, bool ShowStats = true, bool title2 = false,
                   bool apply_plot_cuts = false, double plot_cuts1 = 0, double plot_Xmax1 = 0, double plot_cuts2 = 0, double plot_Xmax2 = 0, bool plot_max = true);
//</editor-fold>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                 histPlotter1DwFit function                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//<editor-fold desc="histPlotter1DwFit function (old)">
/* histPlotter1DwFit is used for chi2 plots with fit */
void histPlotter1DwFit(TCanvas *HistogramCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string Histogram1DTitle,
                       std::string Histogram1DTitleReactions, TList *Histogram_list, const char *Histogram_OutPDF, std::string Histogram1DSaveName,
                       const std::string &Histogram1DSaveNamePath, std::string finalState, double &plot_Xmax, double &plot_lcut, double &plot_ucut, double factor, bool plot_max = true,
                       std::string particle = "");
//</editor-fold>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                   stackPlotter1D function                                                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// stackPlotter1D function (regular) ------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="stackPlotter1D (regular)">
void stackPlotter1D(TCanvas *HistogramCanvas, THStack *Histogram1DStack, bool normalize_Histogram, std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list,
                    const char *Histogram_OutPDF, TH1D *H1D_All_Int, TH1D *H1D_QEL, TH1D *H1D_MEC, TH1D *H1D_RES, TH1D *H1D_DIS, std::string Histogram1DStackSaveName,
                    std::string Histogram1DStackSaveNamePath, std::string finalState);
//</editor-fold>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                   histPlotter2D function                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// histPlotter2D function (regular) -------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter2D function (regular)">
void histPlotter2D(TCanvas *HistogramCanvas, TH2D *Histogram2D, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList *Histogram_list,
                   const char *Histogram_OutPDF, bool zLogScalePlot, std::string Histogram1DSaveNameDir, std::string Histogram1DSaveName, bool ShowStats = true);
//</editor-fold>

// histPlotter2D function (Beta vs. P plots, all particles) -------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter2D function (Beta vs. P plots, all particles)">
void histPlotter2D(TCanvas *HistogramCanvas, TH2D *Histogram2D, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList *Histogram_list,
                   const char *Histogram_OutPDF, bool zLogScalePlot, std::string Histogram1DSaveNameDir, std::string Histogram1DSaveName, TF1 *Beta_function1, TF1 *Beta_function2,
                   TF1 *Beta_function3, TF1 *Beta_function4, TF1 *Beta_function5, TF1 *Beta_function6, TF1 *Beta_function7, TF1 *Beta_function8, TF1 *Beta_function9);
//</editor-fold>

// histPlotter2D function (Beta vs. P plots, single particle) -----------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter2D function (Beta vs. P plots, single particle)">
void histPlotter2D(TCanvas *HistogramCanvas, TH2D *Histogram2D, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList *Histogram_list,
                   const char *Histogram_OutPDF, bool zLogScalePlot, std::string Histogram1DSaveNameDir, std::string Histogram1DSaveName, TF1 *Beta_function1, std::string particle1,
                   bool plot_legend);
//</editor-fold>

// histPlotter2D function (Beta vs. P plots, by charge) -----------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter2D function (Beta vs. P plots, by charge)">
void histPlotter2D(TCanvas *HistogramCanvas, TH2D *Histogram2D, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList *Histogram_list,
                   const char *Histogram_OutPDF, bool zLogScalePlot, std::string Histogram1DSaveNameDir, std::string Histogram1DSaveName, TF1 *Beta_function1, std::string particle1,
                   TF1 *Beta_function2, std::string particle2, TF1 *Beta_function3, std::string particle3, bool plot_legend);
//</editor-fold>

#endif  // HISTOGRAMPLOTTINGFUNCTIONS_H
