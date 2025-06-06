//
// Created by Alon Sportes on 27/01/2023.
//

#ifndef HISTOGRAMPLOTTINGFUNCTIONS_H
#define HISTOGRAMPLOTTINGFUNCTIONS_H

#if IndependentCanvasPDF
    #include "../includes/CanvasPDF/CanvasPDF.h"
#endif

#include "../namespaces/general_utilities/utilities.h"

using namespace utilities;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                   histPlotter1D function                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// histPlotter1D function (old) -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region                                          /* histPlotter1D function (old) */
void histPlotter1D(TCanvas *HistogramCanvas,            // The canvas
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle, std::string Histogram1DTitleReactions, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list,
                   vector<TObject *> &HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName,
                   const std::string &Histogram1DSaveNamePath, std::string finalState, int kColor = 1, bool centerTitle = true, bool AddToStack = false, bool ShowStats = true,
                   bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0, bool plot_max = true) {
    // gStyle->SetOptStat("ourmen");

    //  Normalization factor:
    double Histogram1D_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        std::string title;

        if (finalState == "") {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")" + " - Normalized";
        } else {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        }

        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string title;

        if (title2 == false) {
            if (finalState == "") {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")";
            } else {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
            }
        } else {
            if (finalState == "") {
                title = Histogram1DTitle;
            } else {
                title = Histogram1DTitle + " (" + finalState + ")";
            }
        }
        //        std::string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Number of events");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            //            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (!ShowStats) { Histogram1D->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_cuts + plot_Xmax;
        double Lower_cut = -plot_cuts + plot_Xmax;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            //            upper_cut->SetLineColor(kMagenta);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            //            lower_cut->SetLineColor(kMagenta);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
                //            lower_cut->SetLineColor(kMagenta);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut, 3)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut, 3)).c_str(), "l");

            if (plot_max) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax, 3)).c_str(), "l");
                //            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + ToStringWithPrecision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale.pdf";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".pdf";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale.pdf";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".pdf";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (AddToStack) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

#pragma region                                          /* histPlotter1D function (old, stackless) */
void histPlotter1D(TCanvas *HistogramCanvas,            // The canvas
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle, std::string Histogram1DTitleReactions, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list,
                   vector<TObject *> &HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath,
                   std::string finalState, bool centerTitle = true, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0,
                   bool plot_max = true) {
    // gStyle->SetOptStat("ourmen");

    double Histogram1D_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        std::string title;

        if (finalState == "") {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")" + " - Normalized";
        } else {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        }

        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string title;

        if (title2 == false) {
            if (finalState == "") {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")";
            } else {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
            }
        } else {
            if (finalState == "") {
                title = Histogram1DTitle;
            } else {
                title = Histogram1DTitle + " (" + finalState + ")";
            }
        }
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Number of events");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (!ShowStats) { Histogram1D->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_cuts + plot_Xmax;
        double Lower_cut = -plot_cuts + plot_Xmax;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut, 3)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut, 3)).c_str(), "l");

            if (plot_max) { TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax, 3)).c_str(), "l"); }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale.pdf";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".pdf";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale.pdf";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".pdf";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

// histPlotter1D function (unsymmetric cuts for SF plots) ---------------------------------------------------------------------------------------------------------------

#pragma region                                          /* histPlotter1D function (unsymmetric cuts for SF plots) */
void histPlotter1D(TCanvas *HistogramCanvas,            // The canvas
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, vector<TObject *> &HistoList, int lineWidth, bool LogScalePlot,
                   bool LinearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath, std::string finalState, int kColor = 1,
                   bool AddToStack = false, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_upper_cut = 0, double plot_lower_cut = 0,
                   double plot_Xmax = 0, bool plot_max = true) {
    // gStyle->SetOptStat("ourmen");

    //  Normalization factor:
    double Histogram1D_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        std::string title;

        if (finalState == "") {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")" + " - Normalized";
        } else {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        }

        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string title;

        if (title2 == false) {
            if (finalState == "") {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")";
            } else {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
            }
        } else {
            if (finalState == "") {
                title = Histogram1DTitle;
            } else {
                title = Histogram1DTitle + " (" + finalState + ")";
            }
        }
        //        std::string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Number of events");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            //            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (!ShowStats) { Histogram1D->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_upper_cut;
        double Lower_cut = plot_lower_cut;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            if (plot_upper_cut != -1) { upper_cut->Draw("same"); }
            upper_cut->SetLineColor(kBlue);
            //            upper_cut->SetLineColor(kMagenta);

            if (plot_lower_cut != -1) { lower_cut->Draw("same"); }
            lower_cut->SetLineColor(kRed);
            //            lower_cut->SetLineColor(kMagenta);

            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
                //            lower_cut->SetLineColor(kMagenta);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut)).c_str(), "l");

            if (plot_max) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax)).c_str(), "l");
                //            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + ToStringWithPrecision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    //    if (!ShowStats) {
    //        Histogram1D->SetStats(0);
    //        gStyle->SetOptStat(000001111);
    //        gROOT->ForceStyle();
    ////        gStyle->SetOptStat(111110);
    ////        Histogram1D->SetOptStat(111110);
    //    } else if (ShowStats == true) {
    ////        gStyle->SetOptStat(000001111);
    //        gStyle->SetOptStat(111110);
    //        gROOT->ForceStyle();
    //    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale.pdf";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".pdf";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale.pdf";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".pdf";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (AddToStack) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

#pragma region                                          /* histPlotter1D function (unsymmetric cuts for SF plots, stackless) */
void histPlotter1D(TCanvas *HistogramCanvas,            // The canvas
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, vector<TObject *> &HistoList, int lineWidth, bool LogScalePlot,
                   bool LinearScalePlot, std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath, std::string finalState, bool ShowStats = true, bool title2 = false,
                   bool apply_plot_cuts = false, double plot_upper_cut = 0, double plot_lower_cut = 0, double plot_Xmax = 0, bool plot_max = true) {
    // gStyle->SetOptStat("ourmen");

    double Histogram1D_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        std::string title;

        if (finalState == "") {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")" + " - Normalized";
        } else {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        }

        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string title;

        if (title2 == false) {
            if (finalState == "") {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")";
            } else {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
            }
        } else {
            if (finalState == "") {
                title = Histogram1DTitle;
            } else {
                title = Histogram1DTitle + " (" + finalState + ")";
            }
        }
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Number of events");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (!ShowStats) { Histogram1D->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_upper_cut;
        double Lower_cut = plot_lower_cut;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            if (plot_upper_cut != -1) { upper_cut->Draw("same"); }
            upper_cut->SetLineColor(kBlue);

            if (plot_lower_cut != -1) { lower_cut->Draw("same"); }
            lower_cut->SetLineColor(kRed);

            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut)).c_str(), "l");

            if (plot_max) { TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax)).c_str(), "l"); }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale.pdf";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".pdf";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale.pdf";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".pdf";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

// histPlotter1D function (one-sided cuts for Nphe plots) ---------------------------------------------------------------------------------------------------------------

#pragma region                                          /* histPlotter1D function (one-sided cuts for Nphe plots) */
void histPlotter1D(TCanvas *HistogramCanvas,            // The canvas
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, vector<TObject *> &HistoList, int lineWidth, bool LogScalePlot,
                   bool LinearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath, std::string finalState, int kColor = 1,
                   bool AddToStack = false, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cut = 0, double plot_Xmax = 0, bool plot_max = true) {
    // gStyle->SetOptStat("ourmen");

    //  Normalization factor:
    double Histogram1D_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        if (finalState == "") {
            std::string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")" + " - Normalized";
            const char *HistogramTitle = title.c_str();
            Histogram1D->SetTitle(HistogramTitle);
        }
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string title;

        if (title2 == false) {
            if (finalState == "") {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")";
            } else {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
            }
        } else {
            if (finalState == "") {
                title = Histogram1DTitle;
            } else {
                title = Histogram1DTitle + " (" + finalState + ")";
            }
        }
        //        std::string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Number of events");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            //            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (!ShowStats) { Histogram1D->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Cut = plot_cut;
        double plot_xmax = plot_Xmax;
        TLine *plot_cut = new TLine(Cut, 0., Cut, gPad->GetFrame()->GetY2());
        plot_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            plot_cut->Draw("same");
            plot_cut->SetLineColor(kRed);
            //            plot_cut->SetLineColor(kMagenta);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
                //            plot_cut->SetLineColor(kMagenta);
            }

            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(plot_cut, ("Lower cut = " + ToStringWithPrecision(Cut, 0)).c_str(), "l");

            if (plot_max) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax)).c_str(), "l");
                //            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(plot_cut, ("Peak location = " + ToStringWithPrecision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    //    if (!ShowStats) {
    //        Histogram1D->SetStats(0);
    //        gStyle->SetOptStat(000001111);
    //        gROOT->ForceStyle();
    ////        gStyle->SetOptStat(111110);
    ////        Histogram1D->SetOptStat(111110);
    //    } else if (ShowStats == true) {
    ////        gStyle->SetOptStat(000001111);
    //        gStyle->SetOptStat(111110);
    //        gROOT->ForceStyle();
    //    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        if (finalState == "") {
            std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale.pdf";
            const char *SaveDir = Histogram1DSaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        } else {
            std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".pdf";
            const char *SaveDir = Histogram1DSaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        }

        HistoList.push_back(Histogram1D);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        if (finalState == "") {
            std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale.pdf";
            const char *SaveDir = Histogram1DSaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        } else {
            std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".pdf";
            const char *SaveDir = Histogram1DSaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        }

        HistoList.push_back(Histogram1D);
    }

    if (AddToStack) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

#pragma region                                          /* histPlotter1D function (one-sided cuts for Nphe plots, stackless) */
void histPlotter1D(TCanvas *HistogramCanvas,            // The canvas
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, vector<TObject *> &HistoList, int lineWidth, bool LogScalePlot,
                   bool LinearScalePlot, std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath, std::string finalState, bool ShowStats = true, bool title2 = false,
                   bool apply_plot_cuts = false, double plot_cut = 0, double plot_Xmax = 0, bool plot_max = true) {
    // gStyle->SetOptStat("ourmen");

    double Histogram1D_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        if (finalState == "") {
            std::string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")" + " - Normalized";
            const char *HistogramTitle = title.c_str();
            Histogram1D->SetTitle(HistogramTitle);
        }
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string title;

        if (title2 == false) {
            if (finalState == "") {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")";
            } else {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
            }
        } else {
            if (finalState == "") {
                title = Histogram1DTitle;
            } else {
                title = Histogram1DTitle + " (" + finalState + ")";
            }
        }
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Number of events");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (!ShowStats) { Histogram1D->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Cut = plot_cut;
        double plot_xmax = plot_Xmax;
        TLine *plot_cut = new TLine(Cut, 0., Cut, gPad->GetFrame()->GetY2());
        plot_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            plot_cut->Draw("same");
            plot_cut->SetLineColor(kRed);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(plot_cut, ("Lower cut = " + ToStringWithPrecision(Cut, 0)).c_str(), "l");

            if (plot_max) { TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax)).c_str(), "l"); }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        if (finalState == "") {
            std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale.pdf";
            const char *SaveDir = Histogram1DSaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        } else {
            std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".pdf";
            const char *SaveDir = Histogram1DSaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        }

        HistoList.push_back(Histogram1D);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        if (finalState == "") {
            std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale.pdf";
            const char *SaveDir = Histogram1DSaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        } else {
            std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".pdf";
            const char *SaveDir = Histogram1DSaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        }

        HistoList.push_back(Histogram1D);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

// histPlotter1D function (Chi2 for separate plots) ---------------------------------------------------------------------------------------------------------------------

#pragma region                                          /* histPlotter1D function (Chi2 for separate plots) */
void histPlotter1D(TCanvas *HistogramCanvas1,           // canvas c1 of other histograms
                   TCanvas *HistogramCanvas,            // canvas c2 of chi2 histograms
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle1, std::string Histogram1DTitle2, std::string Histogram1DTitle3, std::string Particle, double titleSize, double labelSizex, double labelSizey,
                   TList *Histogram_list, vector<TObject *> &HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName,
                   const std::string &Histogram1DSaveNamePath, int kColor = 1, bool centerTitle = true, bool AddToStack = false, bool ShowStats = true, bool title2 = false,
                   bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0, bool plot_max = true) {
    double TitleFontSize = gStyle->GetTitleFontSize();
    HistogramCanvas->cd();

    // gStyle->SetOptStat("ourmen");

    double Histogram1D_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        std::string title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")" + " - Normalized";
        std::string HistogramTitle = "#splitline{" + title + "}{        (|#chi^{2}_{" + Particle + "," + Histogram1DTitle3 + "} - #bar{#chi^{2}}_{" + Particle + "," + Histogram1DTitle3 +
                                     "}| #leq " + ToStringWithPrecision(plot_cuts) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string HistogramTitle, title, subtitle;

        if (title2 == false) {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")";
        } else {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ")";
        }

        HistogramTitle = "#splitline{" + title + "}{        (|#chi^{2}_{" + Particle + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle + "," + Histogram1DTitle3 + "})_{max}| #leq " +
                         ToStringWithPrecision(plot_cuts) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Number of events");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (!ShowStats) { Histogram1D->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_cuts + plot_Xmax;
        double Lower_cut = -plot_cuts + plot_Xmax;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            //            upper_cut->SetLineColor(kMagenta);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            //            lower_cut->SetLineColor(kMagenta);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
                //            lower_cut->SetLineColor(kMagenta);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut)).c_str(), "l");

            if (plot_max) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax)).c_str(), "l");
                //            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + ToStringWithPrecision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + Histogram1DTitle3 + ".pdf";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + Histogram1DTitle3 + ".pdf";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (AddToStack) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    HistogramCanvas->Clear();
    gStyle->SetTitleFontSize(TitleFontSize);
    gROOT->ForceStyle();

    HistogramCanvas1->cd();
}
#pragma endregion

#pragma region                                          /* histPlotter1D function (Chi2 for separate plots, stackless) */
void histPlotter1D(TCanvas *HistogramCanvas1,           // canvas c1 of other histograms
                   TCanvas *HistogramCanvas,            // canvas c2 of chi2 histograms
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle1, std::string Histogram1DTitle2, std::string Histogram1DTitle3, std::string Particle, double titleSize, double labelSizex, double labelSizey,
                   TList *Histogram_list, vector<TObject *> &HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, std::string Histogram1DSaveName,
                   const std::string &Histogram1DSaveNamePath, bool centerTitle = true, bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0,
                   double plot_Xmax = 0, bool plot_max = true) {
    double TitleFontSize = gStyle->GetTitleFontSize();
    HistogramCanvas->cd();

    // gStyle->SetOptStat("ourmen");

    double Histogram1D_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        std::string title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")" + " - Normalized";
        std::string HistogramTitle = "#splitline{" + title + "}{        (|#chi^{2}_{" + Particle + "," + Histogram1DTitle3 + "} - #bar{#chi^{2}}_{" + Particle + "," + Histogram1DTitle3 +
                                     "}| #leq " + ToStringWithPrecision(plot_cuts) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string HistogramTitle, title, subtitle;

        if (title2 == false) {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")";
        } else {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ")";
        }

        HistogramTitle = "#splitline{" + title + "}{        (|#chi^{2}_{" + Particle + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle + "," + Histogram1DTitle3 + "})_{max}| #leq " +
                         ToStringWithPrecision(plot_cuts) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Number of events");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (!ShowStats) { Histogram1D->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_cuts + plot_Xmax;
        double Lower_cut = -plot_cuts + plot_Xmax;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut)).c_str(), "l");

            if (plot_max) { TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax)).c_str(), "l"); }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + Histogram1DTitle3 + ".pdf";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + Histogram1DTitle3 + ".pdf";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    HistogramCanvas->Clear();
    gStyle->SetTitleFontSize(TitleFontSize);
    gROOT->ForceStyle();

    HistogramCanvas1->cd();
}
#pragma endregion

// histPlotter1D function (applying all Chi2 cuts) ----------------------------------------------------------------------------------------------------------------------

#pragma region                                          /* histPlotter1D function (applying all Chi2 cuts) */
void histPlotter1D(TCanvas *HistogramCanvas1,           // canvas c1 of other histograms
                   TCanvas *HistogramCanvas,            // canvas c2 of chi2 histograms
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle1, std::string Histogram1DTitle2, std::string Histogram1DTitle3, std::string Particle1, std::string Particle2, double titleSize,
                   double labelSizex, double labelSizey, TList *Histogram_list, vector<TObject *> &HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                   THStack *Histogram1DStack, std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath, int kColor = 1, bool centerTitle = true, bool AddToStack = false,
                   bool ShowStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts1 = 0, double plot_Xmax1 = 0, double plot_cuts2 = 0, double plot_Xmax2 = 0,
                   bool plot_max = true) {
    double TitleFontSize = gStyle->GetTitleFontSize();
    HistogramCanvas->cd();

    // gStyle->SetOptStat("ourmen");

    double Histogram1D_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram1D_integral = custom_normalization_factor;
    }

    //    TPaveText *PrametesrBox1 = new TPaveText(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 - 0.4, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 - 0.05, "NDC");
    //    PrametesrBox1->SetTextSize(12);
    //    PrametesrBox1->SetFillColor(0);
    ////            PrametesrBox1->SetTextAlign(12);
    //    PrametesrBox1->AddText(("|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - ({#chi^{2}}_{" + Particle1 + "," +
    //                           Histogram1DTitle3 + "})_{max}| #leq " + ToStringWithPrecision(plot_cuts1)).c_str());
    //    PrametesrBox1->AddText(("|#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "} - ({#chi^{2}}_{" + Particle2 + "," +
    //                           Histogram1DTitle3 + "})_{max}| #leq " + ToStringWithPrecision(plot_cuts2)).c_str());

    //    TPaveText *PrametesrBox = new TPaveText(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 - 0.4, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 - 0.05, "NDC");
    //    PrametesrBox->SetTextSize(12);
    //    PrametesrBox->SetFillColor(0);
    ////            PrametesrBox->SetTextAlign(12);
    //    PrametesrBox->AddText(("|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - ({#chi^{2}}_{" + Particle1 + "," +
    //                           Histogram1DTitle3 + "})_{max}| #leq " + ToStringWithPrecision(plot_cuts1)).c_str());
    //    PrametesrBox->AddText(("|#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "} - ({#chi^{2}}_{" + Particle2 + "," +
    //                           Histogram1DTitle3 + "})_{max}| #leq " + ToStringWithPrecision(plot_cuts2)).c_str());

    if (normalize_Histogram) {
        std::string title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")" + " - Normalized";
        std::string HistogramTitle = "#splitline{" + title + "}{" + "(|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 +
                                     "})_{max}| #leq " + ToStringWithPrecision(plot_cuts1) + ", " + "|#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "} -(#chi^{2}_{" + Particle2 + "," +
                                     Histogram1DTitle3 + "})_{max}| #leq " + ToStringWithPrecision(plot_cuts2) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            //            PrametesrBox1->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string HistogramTitle, title, subtitle;

        if (title2 == false) {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")";
        } else {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ")";
        }

        HistogramTitle = "#splitline{" + title + "}{" + "(|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "})_{max}| #leq " +
                         ToStringWithPrecision(plot_cuts1) + ", " + "|#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 +
                         "})_{max}| #leq " + ToStringWithPrecision(plot_cuts2) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Number of events");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            //            PrametesrBox1->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (!ShowStats) { Histogram1D->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_cuts1 + plot_Xmax1;
        double Lower_cut = -plot_cuts1 + plot_Xmax1;
        double plot_xmax1 = plot_Xmax1;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax1, 0., plot_Xmax1, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut)).c_str(), "l");

            if (plot_max) { TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax1)).c_str(), "l"); }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + Histogram1DTitle3 + ".pdf";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + Histogram1DTitle3 + ".pdf";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (AddToStack) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    HistogramCanvas->Clear();
    gStyle->SetTitleFontSize(TitleFontSize);
    gROOT->ForceStyle();

    HistogramCanvas1->cd();
}
#pragma endregion

#pragma region                                          /* histPlotter1D function (applying all Chi2 cuts, stackless) */
void histPlotter1D(TCanvas *HistogramCanvas1,           // canvas c1 of other histograms
                   TCanvas *HistogramCanvas,            // canvas c2 of chi2 histograms
                   TH1D *Histogram1D,                   // The histogram
                   bool normalize_Histogram,            // Normalize histogram or not
                   bool custom_normalization,           // Normalize histogram or not
                   double custom_normalization_factor,  // Normalize histogram or not
                   std::string Histogram1DTitle1, std::string Histogram1DTitle2, std::string Histogram1DTitle3, std::string Particle1, std::string Particle2, double titleSize,
                   double labelSizex, double labelSizey, TList *Histogram_list, vector<TObject *> &HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                   std::string Histogram1DSaveName, const std::string &Histogram1DSaveNamePath, bool centerTitle = true, bool ShowStats = true, bool title2 = false,
                   bool apply_plot_cuts = false, double plot_cuts1 = 0, double plot_Xmax1 = 0, double plot_cuts2 = 0, double plot_Xmax2 = 0, bool plot_max = true) {
    double TitleFontSize = gStyle->GetTitleFontSize();
    HistogramCanvas->cd();

    // gStyle->SetOptStat("ourmen");

    double Histogram1D_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        std::string title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")" + " - Normalized";
        std::string HistogramTitle = "#splitline{" + title + "}{" + "(|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 +
                                     "})_{max}| #leq " + ToStringWithPrecision(plot_cuts1) + ", " + "|#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "} -(#chi^{2}_{" + Particle2 + "," +
                                     Histogram1DTitle3 + "})_{max}| #leq " + ToStringWithPrecision(plot_cuts2) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string HistogramTitle, title, subtitle;

        if (title2 == false) {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")";
        } else {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ")";
        }

        HistogramTitle = "#splitline{" + title + "}{" + "(|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "})_{max}| #leq " +
                         ToStringWithPrecision(plot_cuts1) + ", " + "|#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 +
                         "})_{max}| #leq " + ToStringWithPrecision(plot_cuts2) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Number of events");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (!ShowStats) { Histogram1D->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_cuts1 + plot_Xmax1;
        double Lower_cut = -plot_cuts1 + plot_Xmax1;
        double plot_xmax1 = plot_Xmax1;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax1, 0., plot_Xmax1, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut)).c_str(), "l");

            if (plot_max) { TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax1)).c_str(), "l"); }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + Histogram1DTitle3 + ".pdf";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + Histogram1DTitle3 + ".pdf";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    HistogramCanvas->Clear();
    gStyle->SetTitleFontSize(TitleFontSize);
    gROOT->ForceStyle();

    HistogramCanvas1->cd();
}
#pragma endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                 histPlotter1DwFit function                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// histPlotter1DwFit function (old) -------------------------------------------------------------------------------------------------------------------------------------

// #pragma region /* FitFunction function */
///* FitFunction is used for custom plot fits */
//
// Double_t FitFunction(Double_t *v, Double_t *par) {
//    Double_t arg = 0;
//    if (par[2] != 0) arg = (v[0] - par[1]) / par[2];
//
//    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
//    return fitval;
//}
// #pragma endregion

#pragma region /* histPlotter1DwFit function (old) */
/* histPlotter1DwFit is used for chi2 plots with fit */
void histPlotter1DwFit(TCanvas *HistogramCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string Histogram1DTitle,
                       std::string Histogram1DTitleReactions, TList *Histogram_list, vector<TObject *> &HistoList, std::string Histogram1DSaveName,
                       const std::string &Histogram1DSaveNamePath, std::string finalState, double &plot_Xmax, double &plot_lcut, double &plot_ucut, double factor, bool plot_max = true,
                       std::string particle = "") {
    double Histogram1D_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    double titleSize = 0.06;
    double labelSizex = 0.0425;
    double labelSizey = 0.0425;

    int lineWidth = 2;
    bool LogScalePlot = false;
    bool LinearScalePlot = true;

    int kColor = kBlack;
    bool centerTitle = true;
    bool ShowStats = true;
    bool title2 = false;
    bool apply_plot_cuts = true;

    if (normalize_Histogram && !custom_normalization) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        std::string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string title;

        if (title2 == false) {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        } else {
            title = Histogram1DTitle + " (" + finalState + ")";
        }

        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Number of events");

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1D->Sumw2();
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Sumw2();
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (!ShowStats) { Histogram1D->SetStats(0); }

    if (particle != "") { cout << "\n\nFit results for " << particle << " in " << finalState << " are (" << Histogram1DTitleReactions << "):\n\n"; }

    if (apply_plot_cuts) {
        /* A fit to a gaussian with 3 parameters: f(x) = p0*exp(-0.5((x-p1)/p2)^2)). */
        Histogram1D->Fit("gaus");

        /* Get fitted function to TF1 plot */
        TF1 *fit = Histogram1D->GetFunction("gaus");
        fit->SetLineColor(kMagenta);

        /* Set fitted plot parameters from TF1 plot */
        double Amp = fit->GetParameter(0);   // get p0
        double Mean = fit->GetParameter(1);  // get p1
        double Std = fit->GetParameter(2);   // get p2

        /* Set cut parameters from TF1 plot */
        plot_Xmax = Mean;
        plot_ucut = Std * factor;     // Cut up to 2 Std from mean
        plot_lcut = -(Std * factor);  // Cut up to 2 Std from mean

        /* Remove TF1 plot from histogram */
        Histogram1D->GetListOfFunctions()->Remove(Histogram1D->GetFunction("gaus"));

        /* Draw TF1 plot as curved function */
        fit->SetLineColor(kMagenta);
        fit->Draw("same && C");

        gPad->Update();

        double Upper_cut = plot_ucut + plot_Xmax, Lower_cut = plot_lcut + plot_Xmax, plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);

        double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
        double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
        auto Cut_legend = new TLegend(x_1_Cut_legend, y_1_Cut_legend, x_2_Cut_legend, y_2_Cut_legend);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("UC = " + ToStringWithPrecision(Upper_cut, 3)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("LC = " + ToStringWithPrecision(Lower_cut, 3)).c_str(), "l");

            if (plot_max) { TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Mean = " + ToStringWithPrecision(plot_xmax, 3)).c_str(), "l"); }

            Cut_legend->Draw("same");

            double x_1_FitParam = gStyle->GetStatX(), y_1_FitParam = y_1_Cut_legend - 0.14;
            double x_2_FitParam = gStyle->GetStatX() - 0.2, y_2_FitParam = y_1_Cut_legend - 0.245;

            TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
            FitParam->SetBorderSize(1);
            FitParam->SetTextFont(0);
            FitParam->SetFillColor(0);
            FitParam->SetTextAlign(11);
            FitParam->AddText(("Fit amp = " + ToStringWithPrecision(Amp, 8)).c_str());
            FitParam->AddText(("Fit std = " + ToStringWithPrecision(Std, 8)).c_str());
            FitParam->AddText(("Fit mean = " + ToStringWithPrecision(Mean, 8)).c_str());
            ((TText *)FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
            FitParam->AddText(("Cuts = std * " + ToStringWithPrecision(factor, 2) + " = " + ToStringWithPrecision(plot_ucut, 3)).c_str());
            ((TText *)FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
            FitParam->Draw("same");
        }
    }

    if (particle != "") { cout << "\n"; }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".pdf";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".pdf";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        HistogramCanvas->SaveAs(SaveDir);

        HistoList.push_back(Histogram1D);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                   stackPlotter1D function                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// stackPlotter1D function (regular) -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* stackPlotter1D (regular) */
void stackPlotter1D(TCanvas *HistogramCanvas, THStack *Histogram1DStack, bool normalize_Histogram, std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list,
                    vector<TObject *> &HistoList, TH1D *H1D_All_Int, TH1D *H1D_QEL, TH1D *H1D_MEC, TH1D *H1D_RES, TH1D *H1D_DIS, std::string Histogram1DStackSaveName,
                    std::string Histogram1DStackSaveNamePath, std::string finalState) {
    // gStyle->SetOptStat("ourmen");

    //  Normalization factor:
    double Histogram1D_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    //    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1225, TitleSize = 0.06, LabelSize = 0.0425;
    bool centerTitle = true;
    std::string xLable = H1D_All_Int->GetXaxis()->GetTitle();

    H1D_All_Int->SetLineWidth(4), H1D_All_Int->SetLineColor(kBlack), H1D_All_Int->SetLineStyle(5);
    H1D_QEL->SetLineWidth(2), H1D_QEL->SetLineColor(kBlue);
    //    H1D_MEC->SetLineWidth(2), H1D_MEC->SetLineColor(kRed);
    H1D_MEC->SetLineWidth(2), H1D_MEC->SetLineColor(kRed + 1);
    H1D_RES->SetLineWidth(2), H1D_RES->SetLineColor(kGreen);
    H1D_DIS->SetLineWidth(2), H1D_DIS->SetLineColor(kOrange + 6);
    //    H1D_DIS->SetLineWidth(2), H1D_DIS->SetLineColor(kMagenta);
    //    H1D_DIS->SetLineColor(9);
    //    H1D_DIS->SetLineColor(kCyan);

    Histogram1DStack->Draw("nostack");
    Histogram1DStack->GetHistogram()->GetXaxis()->SetTitleSize(TitleSize);
    Histogram1DStack->GetHistogram()->GetXaxis()->SetLabelSize(LabelSize);
    Histogram1DStack->GetHistogram()->GetXaxis()->CenterTitle(centerTitle);
    Histogram1DStack->GetHistogram()->GetYaxis()->SetLabelSize(LabelSize);
    Histogram1DStack->GetHistogram()->GetYaxis()->CenterTitle(centerTitle);

    if (normalize_Histogram) {
        std::string title;

        if (finalState == "") {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")" + " - Normalized";
        } else {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        }

        const char *HistogramTitle = title.c_str();
        Histogram1DStack->SetTitle(HistogramTitle);
        //        Histogram1DStack->GetXaxis()->SetTitle(xLable.c_str());
        Histogram1DStack->GetYaxis()->SetTitle("Probability (%)");
        Histogram1DStack->GetHistogram()->GetYaxis()->SetTitleSize(TitleSize);

        if (H1D_All_Int->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1DStack->Draw();
            displayText->Draw();
        }
    } else {
        std::string title;

        if (finalState == "") {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")";
        } else {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        }

        const char *HistogramTitle = title.c_str();
        Histogram1DStack->SetTitle(HistogramTitle);

        if (H1D_All_Int->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram1DStack->Draw();
            displayText->Draw();
        } else if (H1D_All_Int->Integral() != 0.) {
            //            Histogram1DStack->GetXaxis()->SetTitle(xLable.c_str());
            Histogram1DStack->GetYaxis()->SetTitle("Number of events");
            Histogram1DStack->GetHistogram()->GetYaxis()->SetTitleSize(TitleSize);
        }
    }

    auto Histogram1DStackLegend = new TLegend(0.76, 0.624, 0.865, 0.89);  // TLA
                                                                          //    auto Histogram1DStackLegend = new TLegend(0.77, 0.624, 0.875, 0.89); // DSA
                                                                          //    auto Histogram1DStackLegend = new TLegend(0.775, 0.625, 0.9, 0.9);

    if (H1D_All_Int->Integral() != 0.) {
        TLegendEntry *H1D_All_Int_Entry = Histogram1DStackLegend->AddEntry(H1D_All_Int, "All int.", "l");
        TLegendEntry *H1D_QEL_Entry = Histogram1DStackLegend->AddEntry(H1D_QEL, "QE", "l");
        TLegendEntry *H1D_MEC_Entry = Histogram1DStackLegend->AddEntry(H1D_MEC, "MEC", "l");
        TLegendEntry *H1D_RES_Entry = Histogram1DStackLegend->AddEntry(H1D_RES, "RES", "l");
        TLegendEntry *H1D_DIS_Entry = Histogram1DStackLegend->AddEntry(H1D_DIS, "DIS", "l");
        Histogram1DStackLegend->Draw();
    }

    Histogram_list->Add(Histogram1DStack);

    if (finalState == "") {
        HistogramCanvas->SaveAs((Histogram1DStackSaveNamePath + Histogram1DStackSaveName + "_" + Histogram1DTitleReactions + ".pdf").c_str());

        HistoList.push_back(Histogram1DStack);
    } else {
        HistogramCanvas->SaveAs((Histogram1DStackSaveNamePath + Histogram1DStackSaveName + "_" + Histogram1DTitleReactions + "_" + finalState + ".pdf").c_str());

        HistoList.push_back(Histogram1DStack);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                   histPlotter2D function                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// histPlotter2D function (regular) -------------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (regular) */
void histPlotter2D(TCanvas *HistogramCanvas, TH2D *Histogram2D, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList *Histogram_list,
                   vector<TObject *> &HistoList, bool zLogScalePlot, std::string Histogram1DSaveNameDir, std::string Histogram1DSaveName, bool ShowStats = true) {
    // gStyle->SetOptStat("ourmen");

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Histogram2D->SetTitleSize(titleSize, "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram2D->GetXaxis()->CenterTitle(centerTitle);
    Histogram2D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram2D->GetYaxis()->CenterTitle(centerTitle);
    Histogram2D->GetZaxis()->SetLabelSize(labelSizez);
    Histogram_list->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        Histogram2D->SetStats(0);
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->SetTextAlign(12);
        displayText->AddText("Empty histogram");
        Histogram2D->Draw();
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        Histogram2D->Draw("colz");

        gPad->Update();
        TPaletteAxis *palette = (TPaletteAxis *)Histogram2D->GetListOfFunctions()->FindObject("palette");
        if (palette) { palette->SetY2NDC(0.475), gPad->Modified(), gPad->Update(); }
    }

    if (zLogScalePlot) {
        HistogramCanvas->SetLogz(1);
        system(("mkdir -p " + Histogram1DSaveNameDir + "/z_Log_Scale_plots").c_str());
        HistogramCanvas->SaveAs((Histogram1DSaveNameDir + "/z_Log_Scale_plots/" + Histogram1DSaveName + "_zLogScale.pdf").c_str());
        // HistogramCanvas->SaveAs((Histogram1DSaveNameDir + "/z_Log_Scale_plots/" + sNameFlag + Histogram1DSaveName + "_zLogScale.pdf").c_str());
        //        HistogramCanvas->SetLogz(1);
        //        HistogramCanvas->SaveAs((Histogram2DSaveNameDir + sNameFlag + Histogram2DSaveName + "_zLogScale.pdf").c_str());

        HistoList.push_back(Histogram2D);
    }
    // if (zLogScalePlot) {
    //     HistogramCanvas->SetLogz(1);

    //     HistoList.push_back(Histogram2D);
    // } else if (zLogScalePlot == false) {
    //     HistogramCanvas->SetLogz(0);

    //     HistoList.push_back(Histogram2D);
    // }

    if (!ShowStats) { Histogram2D->SetStats(0); }

    gStyle->SetStatX(0.98);  // gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.875);

    HistogramCanvas->SetLogz(0);
    HistogramCanvas->SaveAs((Histogram1DSaveNameDir + Histogram1DSaveName + ".pdf").c_str());

    // gStyle->SetStatX(DefStatX);
    // gStyle->SetStatY(DefStatY);

    HistogramCanvas->Clear();
}
#pragma endregion

// histPlotter2D function (Beta vs. P plots, all particles) -------------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (Beta vs. P plots, all particles) */
void histPlotter2D(TCanvas *HistogramCanvas, TH2D *Histogram2D, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList *Histogram_list,
                   vector<TObject *> &HistoList, bool zLogScalePlot, std::string Histogram1DSaveNameDir, std::string Histogram1DSaveName, TF1 *Beta_function1, TF1 *Beta_function2,
                   TF1 *Beta_function3, TF1 *Beta_function4, TF1 *Beta_function5, TF1 *Beta_function6, TF1 *Beta_function7, TF1 *Beta_function8, TF1 *Beta_function9) {
    // gStyle->SetOptStat("ourmen");

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Histogram2D->SetTitleSize(titleSize, "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram2D->GetXaxis()->CenterTitle(centerTitle);
    Histogram2D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram2D->GetYaxis()->CenterTitle(centerTitle);
    Histogram2D->GetZaxis()->SetLabelSize(labelSizez);
    Histogram_list->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->SetTextAlign(12);
        displayText->AddText("Empty histogram");
        Histogram2D->Draw();
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        Histogram2D->Draw("colz");

        gPad->Update();
        TPaletteAxis *palette = (TPaletteAxis *)Histogram2D->GetListOfFunctions()->FindObject("palette");
        if (palette) { palette->SetY2NDC(0.475), gPad->Modified(), gPad->Update(); }

        Beta_function1->Draw("same");
        Beta_function2->Draw("same");
        Beta_function3->Draw("same");
        Beta_function4->Draw("same");
        Beta_function5->Draw("same");
        Beta_function6->Draw("same");
        Beta_function7->Draw("same");
        Beta_function8->Draw("same");
        Beta_function9->Draw("same");
    }

    if (zLogScalePlot) { HistogramCanvas->SetLogz(1); }

    gStyle->SetStatX(0.98);   // gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.935);  // gStyle->SetStatY(0.4);
    HistogramCanvas->SaveAs((Histogram1DSaveNameDir + Histogram1DSaveName).c_str());
    // gStyle->SetStatX(DefStatX);
    // gStyle->SetStatY(DefStatY);

    HistoList.push_back(Histogram2D);

    HistogramCanvas->Clear();
}
#pragma endregion

// histPlotter2D function (Beta vs. P plots, single particle) -----------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (Beta vs. P plots, single particle) */
void histPlotter2D(TCanvas *HistogramCanvas, TH2D *Histogram2D, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList *Histogram_list,
                   vector<TObject *> &HistoList, bool zLogScalePlot, std::string Histogram1DSaveNameDir, std::string Histogram1DSaveName, TF1 *Beta_function1, std::string particle1,
                   bool plot_legend) {
    // gStyle->SetOptStat("ourmen");

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.165, y_1 = 0.3, x_2 = 0.865, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Histogram2D->SetTitleSize(titleSize, "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram2D->GetXaxis()->CenterTitle(centerTitle);
    Histogram2D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram2D->GetYaxis()->CenterTitle(centerTitle);
    Histogram2D->GetZaxis()->SetLabelSize(labelSizez);
    Histogram_list->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        Histogram2D->SetStats(0);
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->SetTextAlign(12);
        displayText->AddText("Empty histogram");
        Histogram2D->Draw("colz");
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        Histogram2D->Draw("colz");

        gPad->Update();
        TPaletteAxis *palette = (TPaletteAxis *)Histogram2D->GetListOfFunctions()->FindObject("palette");
        if (palette) { palette->SetY2NDC(0.475), gPad->Modified(), gPad->Update(); }

        Beta_function1->SetLineColor(kRed);
        Beta_function1->Draw("same");
    }

    auto Beta_vs_P_legend = new TLegend(0.87, 0.725 - 0.25, 0.87 - 0.2, 0.725 - 0.3);

    if ((plot_legend == true) && (Histogram2D->Integral() != 0.)) {
        TLegendEntry *Beta_function1_entry = Beta_vs_P_legend->AddEntry(Beta_function1, particle1.c_str(), "l");

        Beta_vs_P_legend->Draw("same");
    }

    if (zLogScalePlot) { HistogramCanvas->SetLogz(1); }

    gStyle->SetStatX(0.98);   // gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.935);  // gStyle->SetStatY(0.4);
    HistogramCanvas->SaveAs((Histogram1DSaveNameDir + Histogram1DSaveName).c_str());
    // gStyle->SetStatX(DefStatX);
    // gStyle->SetStatY(DefStatY);

    HistoList.push_back(Histogram2D);

    HistogramCanvas->Clear();
}
#pragma endregion

// histPlotter2D function (Beta vs. P plots, by charge) -----------------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (Beta vs. P plots, by charge) */
void histPlotter2D(TCanvas *HistogramCanvas, TH2D *Histogram2D, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList *Histogram_list,
                   vector<TObject *> &HistoList, bool zLogScalePlot, std::string Histogram1DSaveNameDir, std::string Histogram1DSaveName, TF1 *Beta_function1, std::string particle1,
                   TF1 *Beta_function2, std::string particle2, TF1 *Beta_function3, std::string particle3, bool plot_legend) {
    // gStyle->SetOptStat("ourmen");

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.165, y_1 = 0.3, x_2 = 0.865, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Histogram2D->SetTitleSize(titleSize, "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram2D->GetXaxis()->CenterTitle(centerTitle);
    Histogram2D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram2D->GetYaxis()->CenterTitle(centerTitle);
    Histogram2D->GetZaxis()->SetLabelSize(labelSizez);
    Histogram_list->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        Histogram2D->SetStats(0);
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->SetTextAlign(12);
        displayText->AddText("Empty histogram");
        Histogram2D->Draw("colz");
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        Histogram2D->Draw("colz");

        gPad->Update();
        TPaletteAxis *palette = (TPaletteAxis *)Histogram2D->GetListOfFunctions()->FindObject("palette");
        if (palette) { palette->SetY2NDC(0.475), gPad->Modified(), gPad->Update(); }

        Beta_function1->SetLineColor(kBlue);
        Beta_function1->Draw("same");
        Beta_function2->SetLineColor(kGreen);
        Beta_function2->Draw("same");
        Beta_function3->SetLineColor(kRed);
        Beta_function3->Draw("same");
    }

    auto Beta_vs_P_legend = new TLegend(0.87, 0.725 - 0.2, 0.87 - 0.2, 0.725 - 0.3);

    if ((plot_legend == true) && (Histogram2D->Integral() != 0.)) {
        TLegendEntry *Beta_function1_entry = Beta_vs_P_legend->AddEntry(Beta_function1, particle1.c_str(), "l");
        TLegendEntry *Beta_function2_entry = Beta_vs_P_legend->AddEntry(Beta_function2, particle2.c_str(), "l");
        TLegendEntry *Beta_function3_entry = Beta_vs_P_legend->AddEntry(Beta_function3, particle3.c_str(), "l");

        Beta_vs_P_legend->Draw("same");
    }

    if (zLogScalePlot) { HistogramCanvas->SetLogz(1); }

    gStyle->SetStatX(0.98);   // gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.935);  // gStyle->SetStatY(0.4);
    HistogramCanvas->SaveAs((Histogram1DSaveNameDir + Histogram1DSaveName).c_str());
    // gStyle->SetStatX(DefStatX);
    // gStyle->SetStatY(DefStatY);

    HistoList.push_back(Histogram2D);

    HistogramCanvas->Clear();
}
#pragma endregion

#endif  // HISTOGRAMPLOTTINGFUNCTIONS_H
