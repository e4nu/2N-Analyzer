//
// Created by Alon Sportes on 27/01/2023.
//

#ifndef HISTOGRAMPLOTTINGFUNCTIONS_H
#define HISTOGRAMPLOTTINGFUNCTIONS_H

#include <TCanvas.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <THStack.h>
#include <TList.h>
#include <TObject.h>

#include <string>
#include <vector>

#include "../libraries/general_utilities/utilities.h"

using namespace utilities;

// histPlotter1D function (old) -----------------------------------------------------------------------------------------------------------------------------------------

// histPlotter1D function (old)
void histPlotter1D(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                   std::string HistogramTitleReactions, double titleSize, double labelSizex, double labelSizey, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth,
                   bool LogScalePlot, bool LinearScalePlot, THStack* HistogramStack, std::string HistogramSaveName, const std::string& HistogramSaveNamePath, std::string finalState,
                   int kColor = 1, bool centerTitle = true, bool AddToStack = false, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0,
                   double plot_Xmax = 0, bool plot_max = true);

//    histPlotter1D function (old, stackless)
void histPlotter1D(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                   std::string HistogramTitleReactions, double titleSize, double labelSizex, double labelSizey, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth,
                   bool LogScalePlot, bool LinearScalePlot, std::string HistogramSaveName, const std::string& HistogramSaveNamePath, std::string finalState, bool centerTitle = true,
                   bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0, bool plot_max = true);

// histPlotter1D function (unsymmetric cuts for SF plots) ---------------------------------------------------------------------------------------------------------------

// histPlotter1D function (unsymmetric cuts for SF plots)
void histPlotter1D(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                   std::string HistogramTitleReactions, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack* HistogramStack,
                   std::string HistogramSaveName, const std::string& HistogramSaveNamePath, std::string finalState, int kColor = 1, bool AddToStack = false, bool ShowStats = true,
                   bool title2 = false, bool apply_plot_cuts = false, double plot_upper_cut = 0, double plot_lower_cut = 0, double plot_Xmax = 0, bool plot_max = true);

//    histPlotter1D function (unsymmetric cuts for SF plots, stackless)
void histPlotter1D(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                   std::string HistogramTitleReactions, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                   std::string HistogramSaveName, const std::string& HistogramSaveNamePath, std::string finalState, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false,
                   double plot_upper_cut = 0, double plot_lower_cut = 0, double plot_Xmax = 0, bool plot_max = true);

// histPlotter1D function (one-sided cuts for Nphe plots) ---------------------------------------------------------------------------------------------------------------

// histPlotter1D function (one-sided cuts for Nphe plots)
void histPlotter1D(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                   std::string HistogramTitleReactions, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack* HistogramStack,
                   std::string HistogramSaveName, const std::string& HistogramSaveNamePath, std::string finalState, int kColor = 1, bool AddToStack = false, bool ShowStats = true,
                   bool title2 = false, bool apply_plot_cuts = false, double plot_cut = 0, double plot_Xmax = 0, bool plot_max = true);

// histPlotter1D function (one-sided cuts for Nphe plots, stackless)
void histPlotter1D(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                   std::string HistogramTitleReactions, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                   std::string HistogramSaveName, const std::string& HistogramSaveNamePath, std::string finalState, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false,
                   double plot_cut = 0, double plot_Xmax = 0, bool plot_max = true);

// histPlotter1D function (Chi2 for separate plots) ---------------------------------------------------------------------------------------------------------------------

// histPlotter1D function (Chi2 for separate plots)
void histPlotter1D(TCanvas* HistogramCanvas1, TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                   std::string HistogramTitle1, std::string HistogramTitle2, std::string HistogramTitle3, std::string Particle, double titleSize, double labelSizex, double labelSizey,
                   TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack* HistogramStack, std::string HistogramSaveName,
                   const std::string& HistogramSaveNamePath, int kColor = 1, bool centerTitle = true, bool AddToStack = false, bool ShowStats = true, bool title2 = false,
                   bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0, bool plot_max = true);

// histPlotter1D function (Chi2 for separate plots, stackless)
void histPlotter1D(TCanvas* HistogramCanvas1, TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                   std::string HistogramTitle1, std::string HistogramTitle2, std::string HistogramTitle3, std::string Particle, double titleSize, double labelSizex, double labelSizey,
                   TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, std::string HistogramSaveName,
                   const std::string& HistogramSaveNamePath, bool centerTitle = true, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0,
                   double plot_Xmax = 0, bool plot_max = true);

// histPlotter1D function (applying all Chi2 cuts) ----------------------------------------------------------------------------------------------------------------------

// histPlotter1D function (applying all Chi2 cuts)
void histPlotter1D(TCanvas* HistogramCanvas1, TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                   std::string HistogramTitle1, std::string HistogramTitle2, std::string HistogramTitle3, std::string Particle1, std::string Particle2, double titleSize, double labelSizex,
                   double labelSizey, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack* HistogramStack,
                   std::string HistogramSaveName, const std::string& HistogramSaveNamePath, int kColor = 1, bool centerTitle = true, bool AddToStack = false, bool ShowStats = true,
                   bool title2 = false, bool apply_plot_cuts = false, double plot_cuts1 = 0, double plot_Xmax1 = 0, double plot_cuts2 = 0, double plot_Xmax2 = 0, bool plot_max = true);

// histPlotter1D function (applying all Chi2 cuts, stackless)
void histPlotter1D(TCanvas* HistogramCanvas1, TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                   std::string HistogramTitle1, std::string HistogramTitle2, std::string HistogramTitle3, std::string Particle1, std::string Particle2, double titleSize, double labelSizex,
                   double labelSizey, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, std::string HistogramSaveName,
                   const std::string& HistogramSaveNamePath, bool centerTitle = true, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts1 = 0,
                   double plot_Xmax1 = 0, double plot_cuts2 = 0, double plot_Xmax2 = 0, bool plot_max = true);

// histPlotter1DwFit function (old) -------------------------------------------------------------------------------------------------------------------------------------

void histPlotter1DwFit(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                       std::string HistogramTitleReactions, TList* Histogram_list, vector<TObject*>& HistoList, std::string HistogramSaveName, const std::string& HistogramSaveNamePath,
                       std::string finalState, double& plot_Xmax, double& plot_lcut, double& plot_ucut, double factor, bool plot_max = true, std::string particle = "");

// stackPlotter1D function (regular) -----------------------------------------------------------------------------------------------------------------------------------------

void stackPlotter1D(TCanvas* HistogramCanvas, THStack* HistogramStack, bool normalize_Histogram, std::string HistogramTitle, std::string HistogramTitleReactions, TList* Histogram_list,
                    vector<TObject*>& HistoList, TH1D* H1D_All_Int, TH1D* H1D_QEL, TH1D* H1D_MEC, TH1D* H1D_RES, TH1D* H1D_DIS, std::string HistogramStackSaveName,
                    std::string HistogramStackSaveNamePath, std::string finalState);

// histPlotter2D function (regular) -------------------------------------------------------------------------------------------------------------

void histPlotter2D(TCanvas* HistogramCanvas, TH2D* Histogram, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList* Histogram_list,
                   vector<TObject*>& HistoList, bool zLogScalePlot, std::string HistogramSaveNameDir, std::string HistogramSaveName, bool ShowStats = true);

// histPlotter2D function (Beta vs. P plots, all particles) -------------------------------------------------------------------------------------------------------------

void histPlotter2D(TCanvas* HistogramCanvas, TH2D* Histogram, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList* Histogram_list,
                   vector<TObject*>& HistoList, bool zLogScalePlot, std::string HistogramSaveNameDir, std::string HistogramSaveName, TF1* Beta_function1, TF1* Beta_function2,
                   TF1* Beta_function3, TF1* Beta_function4, TF1* Beta_function5, TF1* Beta_function6, TF1* Beta_function7, TF1* Beta_function8, TF1* Beta_function9);

// histPlotter2D function (Beta vs. P plots, single particle) -----------------------------------------------------------------------------------------------------------

void histPlotter2D(TCanvas* HistogramCanvas, TH2D* Histogram, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList* Histogram_list,
                   vector<TObject*>& HistoList, bool zLogScalePlot, std::string HistogramSaveNameDir, std::string HistogramSaveName, TF1* Beta_function1, std::string particle1,
                   bool plot_legend);

// histPlotter2D function (Beta vs. P plots, by charge) -----------------------------------------------------------------------------------------------------------------

void histPlotter2D(TCanvas* HistogramCanvas, TH2D* Histogram, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList* Histogram_list,
                   vector<TObject*>& HistoList, bool zLogScalePlot, std::string HistogramSaveNameDir, std::string HistogramSaveName, TF1* Beta_function1, std::string particle1,
                   TF1* Beta_function2, std::string particle2, TF1* Beta_function3, std::string particle3, bool plot_legend);

#endif  // HISTOGRAMPLOTTINGFUNCTIONS_H
