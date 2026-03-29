//
// Created by Alon Sportes on 27/01/2023.
//

#include "functions/HistogramPlottingFunctions.h"

#include <TFrame.h>

namespace hf = histogram_functions;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                   histPlotter1D function                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// histPlotter1D function (old) -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter1D function (old) */
void histPlotter1D(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                   std::string HistogramTitleReactions, double titleSize, double labelSizex, double labelSizey, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth,
                   bool LogScalePlot, bool LinearScalePlot, THStack* HistogramStack, std::string HistogramSaveName, const std::string& HistogramSaveNamePath, std::string finalState,
                   int kColor, bool centerTitle, bool AddToStack, bool ShowStats, bool title2, bool apply_plot_cuts, double plot_cuts, double plot_Xmax, bool plot_max) {
    gStyle->SetOptStat("ourmen");

    //  Normalization factor:
    double Histogram_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram_integral = Histogram->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        const std::string title = hf::BuildStandardHistTitle(HistogramTitle, HistogramTitleReactions, finalState, true);
        const char* HistogramTitle = title.c_str();
        Histogram->SetTitle(HistogramTitle);
        Histogram->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Scale(100. / Histogram_integral, "nosw2");
            Histogram->Sumw2();
            Histogram->Draw();
        }
    } else if (!normalize_Histogram) {
        const std::string title = hf::BuildDisplayHistTitle(HistogramTitle, HistogramTitleReactions, finalState, title2);
        const char* HistogramTitle = title.c_str();
        Histogram->SetTitle(HistogramTitle);
        Histogram->GetYaxis()->SetTitle("Number of events");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            //            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Sumw2();
            Histogram->Draw();
        }
    }

    Histogram->GetXaxis()->SetTitleSize(titleSize);
    Histogram->GetXaxis()->SetLabelSize(labelSizex);
    Histogram->GetXaxis()->CenterTitle(centerTitle);
    Histogram->GetYaxis()->SetTitleSize(titleSize);
    Histogram->GetYaxis()->SetLabelSize(labelSizey);
    Histogram->GetYaxis()->CenterTitle(centerTitle);
    Histogram->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram);

    if (!ShowStats) { Histogram->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_cuts + plot_Xmax;
        double Lower_cut = -plot_cuts + plot_Xmax;
        double plot_xmax = plot_Xmax;
        TLine* upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine* lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine* max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.35);

        if (Histogram->Integral() != 0.) {
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

            TLegendEntry* Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut, 3)).c_str(), "l");
            TLegendEntry* Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut, 3)).c_str(), "l");

            if (plot_max) {
                TLegendEntry* Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax, 3)).c_str(), "l");
                //            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + ToStringWithPrecision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        const std::string HistogramSaveNameDir = hf::BuildScaleSaveName(HistogramSaveNamePath, HistogramSaveName, "_log_scale", finalState);

        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        const std::string HistogramSaveNameDir = hf::BuildScaleSaveName(HistogramSaveNamePath, HistogramSaveName, "_linear_scale", finalState);

        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (AddToStack) {
        Histogram->SetLineColor(kColor);
        Histogram->SetStats(0);
        HistogramStack->Add(Histogram);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

#pragma region /* histPlotter1D function (old, stackless) */
void histPlotter1D(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                   std::string HistogramTitleReactions, double titleSize, double labelSizex, double labelSizey, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth,
                   bool LogScalePlot, bool LinearScalePlot, std::string HistogramSaveName, const std::string& HistogramSaveNamePath, std::string finalState, bool centerTitle, bool ShowStats,
                   bool title2, bool apply_plot_cuts, double plot_cuts, double plot_Xmax, bool plot_max) {
    gStyle->SetOptStat("ourmen");

    double Histogram_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram_integral = Histogram->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        const std::string title = hf::BuildStandardHistTitle(HistogramTitle, HistogramTitleReactions, finalState, true);
        const char* HistogramTitle = title.c_str();
        Histogram->SetTitle(HistogramTitle);
        Histogram->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Scale(100. / Histogram_integral, "nosw2");
            Histogram->Sumw2();
            Histogram->Draw();
        }
    } else if (!normalize_Histogram) {
        const std::string title = hf::BuildDisplayHistTitle(HistogramTitle, HistogramTitleReactions, finalState, title2);
        const char* HistogramTitle = title.c_str();
        Histogram->SetTitle(HistogramTitle);
        Histogram->GetYaxis()->SetTitle("Number of events");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Sumw2();
            Histogram->Draw();
        }
    }

    Histogram->GetXaxis()->SetTitleSize(titleSize);
    Histogram->GetXaxis()->SetLabelSize(labelSizex);
    Histogram->GetXaxis()->CenterTitle(centerTitle);
    Histogram->GetYaxis()->SetTitleSize(titleSize);
    Histogram->GetYaxis()->SetLabelSize(labelSizey);
    Histogram->GetYaxis()->CenterTitle(centerTitle);
    Histogram->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram);

    if (!ShowStats) { Histogram->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_cuts + plot_Xmax;
        double Lower_cut = -plot_cuts + plot_Xmax;
        double plot_xmax = plot_Xmax;
        TLine* upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine* lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine* max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.35);

        if (Histogram->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry* Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut, 3)).c_str(), "l");
            TLegendEntry* Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut, 3)).c_str(), "l");

            if (plot_max) { TLegendEntry* Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax, 3)).c_str(), "l"); }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        const std::string HistogramSaveNameDir = hf::BuildScaleSaveName(HistogramSaveNamePath, HistogramSaveName, "_log_scale", finalState);

        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        const std::string HistogramSaveNameDir = hf::BuildScaleSaveName(HistogramSaveNamePath, HistogramSaveName, "_linear_scale", finalState);

        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

// histPlotter1D function (unsymmetric cuts for SF plots) ---------------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter1D function (unsymmetric cuts for SF plots) */
void histPlotter1D(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                   std::string HistogramTitleReactions, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack* HistogramStack,
                   std::string HistogramSaveName, const std::string& HistogramSaveNamePath, std::string finalState, int kColor, bool AddToStack, bool ShowStats, bool title2,
                   bool apply_plot_cuts, double plot_upper_cut, double plot_lower_cut, double plot_Xmax, bool plot_max) {
    gStyle->SetOptStat("ourmen");

    //  Normalization factor:
    double Histogram_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram_integral = Histogram->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        const std::string title = hf::BuildStandardHistTitle(HistogramTitle, HistogramTitleReactions, finalState, true);
        const char* HistogramTitle = title.c_str();
        Histogram->SetTitle(HistogramTitle);
        Histogram->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Scale(100. / Histogram_integral, "nosw2");
            Histogram->Sumw2();
            Histogram->Draw();
        }
    } else if (!normalize_Histogram) {
        const std::string title = hf::BuildDisplayHistTitle(HistogramTitle, HistogramTitleReactions, finalState, title2);
        const char* HistogramTitle = title.c_str();
        Histogram->SetTitle(HistogramTitle);
        Histogram->GetYaxis()->SetTitle("Number of events");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            //            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Sumw2();
            Histogram->Draw();
        }
    }

    Histogram->GetXaxis()->SetTitleSize(0.06);
    Histogram->GetXaxis()->SetLabelSize(0.0425);
    Histogram->GetXaxis()->CenterTitle(true);
    Histogram->GetYaxis()->SetTitleSize(0.06);
    Histogram->GetYaxis()->SetLabelSize(0.0425);
    Histogram->GetYaxis()->CenterTitle(true);
    Histogram->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram);

    if (!ShowStats) { Histogram->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_upper_cut;
        double Lower_cut = plot_lower_cut;
        double plot_xmax = plot_Xmax;
        TLine* upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine* lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine* max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.35);

        if (Histogram->Integral() != 0.) {
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

            TLegendEntry* Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut)).c_str(), "l");
            TLegendEntry* Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut)).c_str(), "l");

            if (plot_max) {
                TLegendEntry* Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax)).c_str(), "l");
                //            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + ToStringWithPrecision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    //    if (!ShowStats) {
    //        Histogram->SetStats(0);
    //        gStyle->SetOptStat(000001111);
    //        gROOT->ForceStyle();
    ////        gStyle->SetOptStat(111110);
    ////        Histogram->SetOptStat(111110);
    //    } else if (ShowStats == true) {
    ////        gStyle->SetOptStat(000001111);
    //        gStyle->SetOptStat(111110);
    //        gROOT->ForceStyle();
    //    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        const std::string HistogramSaveNameDir = hf::BuildScaleSaveName(HistogramSaveNamePath, HistogramSaveName, "_log_scale", finalState);

        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        const std::string HistogramSaveNameDir = hf::BuildScaleSaveName(HistogramSaveNamePath, HistogramSaveName, "_linear_scale", finalState);

        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (AddToStack) {
        Histogram->SetLineColor(kColor);
        Histogram->SetStats(0);
        HistogramStack->Add(Histogram);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

#pragma region /* histPlotter1D function (unsymmetric cuts for SF plots, stackless) */
void histPlotter1D(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                   std::string HistogramTitleReactions, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                   std::string HistogramSaveName, const std::string& HistogramSaveNamePath, std::string finalState, bool ShowStats, bool title2, bool apply_plot_cuts, double plot_upper_cut,
                   double plot_lower_cut, double plot_Xmax, bool plot_max) {
    gStyle->SetOptStat("ourmen");

    double Histogram_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram_integral = Histogram->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        const std::string title = hf::BuildStandardHistTitle(HistogramTitle, HistogramTitleReactions, finalState, true);
        const char* HistogramTitle = title.c_str();
        Histogram->SetTitle(HistogramTitle);
        Histogram->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Scale(100. / Histogram_integral, "nosw2");
            Histogram->Sumw2();
            Histogram->Draw();
        }
    } else if (!normalize_Histogram) {
        const std::string title = hf::BuildDisplayHistTitle(HistogramTitle, HistogramTitleReactions, finalState, title2);
        const char* HistogramTitle = title.c_str();
        Histogram->SetTitle(HistogramTitle);
        Histogram->GetYaxis()->SetTitle("Number of events");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Sumw2();
            Histogram->Draw();
        }
    }

    Histogram->GetXaxis()->SetTitleSize(0.06);
    Histogram->GetXaxis()->SetLabelSize(0.0425);
    Histogram->GetXaxis()->CenterTitle(true);
    Histogram->GetYaxis()->SetTitleSize(0.06);
    Histogram->GetYaxis()->SetLabelSize(0.0425);
    Histogram->GetYaxis()->CenterTitle(true);
    Histogram->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram);

    if (!ShowStats) { Histogram->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_upper_cut;
        double Lower_cut = plot_lower_cut;
        double plot_xmax = plot_Xmax;
        TLine* upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine* lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine* max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.35);

        if (Histogram->Integral() != 0.) {
            if (plot_upper_cut != -1) { upper_cut->Draw("same"); }
            upper_cut->SetLineColor(kBlue);

            if (plot_lower_cut != -1) { lower_cut->Draw("same"); }
            lower_cut->SetLineColor(kRed);

            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry* Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut)).c_str(), "l");
            TLegendEntry* Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut)).c_str(), "l");

            if (plot_max) { TLegendEntry* Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax)).c_str(), "l"); }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        const std::string HistogramSaveNameDir = hf::BuildScaleSaveName(HistogramSaveNamePath, HistogramSaveName, "_log_scale", finalState);

        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        const std::string HistogramSaveNameDir = hf::BuildScaleSaveName(HistogramSaveNamePath, HistogramSaveName, "_linear_scale", finalState);

        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

// histPlotter1D function (one-sided cuts for Nphe plots) ---------------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter1D function (one-sided cuts for Nphe plots) */
void histPlotter1D(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                   std::string HistogramTitleReactions, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack* HistogramStack,
                   std::string HistogramSaveName, const std::string& HistogramSaveNamePath, std::string finalState, int kColor, bool AddToStack, bool ShowStats, bool title2,
                   bool apply_plot_cuts, double plot_cut, double plot_Xmax, bool plot_max) {
    gStyle->SetOptStat("ourmen");

    //  Normalization factor:
    double Histogram_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram_integral = Histogram->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        const std::string title = hf::BuildStandardHistTitle(HistogramTitle, HistogramTitleReactions, finalState, true);
        Histogram->SetTitle(title.c_str());
        Histogram->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Scale(100. / Histogram_integral, "nosw2");
            Histogram->Sumw2();
            Histogram->Draw();
        }
    } else if (!normalize_Histogram) {
        const std::string title = hf::BuildDisplayHistTitle(HistogramTitle, HistogramTitleReactions, finalState, title2);
        const char* HistogramTitle = title.c_str();
        Histogram->SetTitle(HistogramTitle);
        Histogram->GetYaxis()->SetTitle("Number of events");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            //            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Sumw2();
            Histogram->Draw();
        }
    }

    Histogram->GetXaxis()->SetTitleSize(0.06);
    Histogram->GetXaxis()->SetLabelSize(0.0425);
    Histogram->GetXaxis()->CenterTitle(true);
    Histogram->GetYaxis()->SetTitleSize(0.06);
    Histogram->GetYaxis()->SetLabelSize(0.0425);
    Histogram->GetYaxis()->CenterTitle(true);
    Histogram->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram);

    if (!ShowStats) { Histogram->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Cut = plot_cut;
        double plot_xmax = plot_Xmax;
        TLine* plot_cut = new TLine(Cut, 0., Cut, gPad->GetFrame()->GetY2());
        plot_cut->SetLineWidth(lineWidth);
        TLine* max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.35);

        if (Histogram->Integral() != 0.) {
            plot_cut->Draw("same");
            plot_cut->SetLineColor(kRed);
            //            plot_cut->SetLineColor(kMagenta);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
                //            plot_cut->SetLineColor(kMagenta);
            }

            TLegendEntry* Cut_legend_lower_lim = Cut_legend->AddEntry(plot_cut, ("Lower cut = " + ToStringWithPrecision(Cut, 0)).c_str(), "l");

            if (plot_max) {
                TLegendEntry* Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax)).c_str(), "l");
                //            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(plot_cut, ("Peak location = " + ToStringWithPrecision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    //    if (!ShowStats) {
    //        Histogram->SetStats(0);
    //        gStyle->SetOptStat(000001111);
    //        gROOT->ForceStyle();
    ////        gStyle->SetOptStat(111110);
    ////        Histogram->SetOptStat(111110);
    //    } else if (ShowStats == true) {
    ////        gStyle->SetOptStat(000001111);
    //        gStyle->SetOptStat(111110);
    //        gROOT->ForceStyle();
    //    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        const std::string HistogramSaveNameDir = hf::BuildScaleSaveName(HistogramSaveNamePath, HistogramSaveName, "_log_scale", finalState);
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        const std::string HistogramSaveNameDir = hf::BuildScaleSaveName(HistogramSaveNamePath, HistogramSaveName, "_linear_scale", finalState);
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (AddToStack) {
        Histogram->SetLineColor(kColor);
        Histogram->SetStats(0);
        HistogramStack->Add(Histogram);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

#pragma region /* histPlotter1D function (one-sided cuts for Nphe plots, stackless) */
void histPlotter1D(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                   std::string HistogramTitleReactions, TList* Histogram_list, vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                   std::string HistogramSaveName, const std::string& HistogramSaveNamePath, std::string finalState, bool ShowStats, bool title2, bool apply_plot_cuts, double plot_cut,
                   double plot_Xmax, bool plot_max) {
    gStyle->SetOptStat("ourmen");

    double Histogram_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram_integral = Histogram->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        const std::string title = hf::BuildStandardHistTitle(HistogramTitle, HistogramTitleReactions, finalState, true);
        Histogram->SetTitle(title.c_str());
        Histogram->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Scale(100. / Histogram_integral, "nosw2");
            Histogram->Sumw2();
            Histogram->Draw();
        }
    } else if (!normalize_Histogram) {
        const std::string title = hf::BuildDisplayHistTitle(HistogramTitle, HistogramTitleReactions, finalState, title2);
        const char* HistogramTitle = title.c_str();
        Histogram->SetTitle(HistogramTitle);
        Histogram->GetYaxis()->SetTitle("Number of events");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Sumw2();
            Histogram->Draw();
        }
    }

    Histogram->GetXaxis()->SetTitleSize(0.06);
    Histogram->GetXaxis()->SetLabelSize(0.0425);
    Histogram->GetXaxis()->CenterTitle(true);
    Histogram->GetYaxis()->SetTitleSize(0.06);
    Histogram->GetYaxis()->SetLabelSize(0.0425);
    Histogram->GetYaxis()->CenterTitle(true);
    Histogram->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram);

    if (!ShowStats) { Histogram->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Cut = plot_cut;
        double plot_xmax = plot_Xmax;
        TLine* plot_cut = new TLine(Cut, 0., Cut, gPad->GetFrame()->GetY2());
        plot_cut->SetLineWidth(lineWidth);
        TLine* max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.35);

        if (Histogram->Integral() != 0.) {
            plot_cut->Draw("same");
            plot_cut->SetLineColor(kRed);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry* Cut_legend_lower_lim = Cut_legend->AddEntry(plot_cut, ("Lower cut = " + ToStringWithPrecision(Cut, 0)).c_str(), "l");

            if (plot_max) { TLegendEntry* Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax)).c_str(), "l"); }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        const std::string HistogramSaveNameDir = hf::BuildScaleSaveName(HistogramSaveNamePath, HistogramSaveName, "_log_scale", finalState);
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        const std::string HistogramSaveNameDir = hf::BuildScaleSaveName(HistogramSaveNamePath, HistogramSaveName, "_linear_scale", finalState);
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

// histPlotter1D function (Chi2 for separate plots) ---------------------------------------------------------------------------------------------------------------------

#pragma region                                 /* histPlotter1D function (Chi2 for separate plots) */
void histPlotter1D(TCanvas* HistogramCanvas1,  // canvas c1 of other histograms
                   TCanvas* HistogramCanvas,   // canvas c2 of chi2 histograms
                   TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle1, std::string HistogramTitle2,
                   std::string HistogramTitle3, std::string Particle, double titleSize, double labelSizex, double labelSizey, TList* Histogram_list, vector<TObject*>& HistoList,
                   int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack* HistogramStack, std::string HistogramSaveName, const std::string& HistogramSaveNamePath, int kColor,
                   bool centerTitle, bool AddToStack, bool ShowStats, bool title2, bool apply_plot_cuts, double plot_cuts, double plot_Xmax, bool plot_max) {
    double TitleFontSize = gStyle->GetTitleFontSize();
    HistogramCanvas->cd();

    gStyle->SetOptStat("ourmen");

    double Histogram_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram_integral = Histogram->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        std::string title = HistogramTitle1 + " (" + HistogramTitle2 + ", " + HistogramTitle3 + ")" + " - Normalized";
        std::string HistogramTitle = "#splitline{" + title + "}{        (|#chi^{2}_{" + Particle + "," + HistogramTitle3 + "} - #bar{#chi^{2}}_{" + Particle + "," + HistogramTitle3 +
                                     "}| #leq " + ToStringWithPrecision(plot_cuts) + ")}";
        Histogram->SetTitle(HistogramTitle.c_str());
        Histogram->GetYaxis()->SetTitle("Probability (%)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Scale(100. / Histogram_integral, "nosw2");
            Histogram->Sumw2();
            Histogram->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string HistogramTitle, title, subtitle;

        if (title2 == false) {
            title = HistogramTitle1 + " (" + HistogramTitle2 + ", " + HistogramTitle3 + ")";
        } else {
            title = HistogramTitle1 + " (" + HistogramTitle2 + ")";
        }

        HistogramTitle = "#splitline{" + title + "}{        (|#chi^{2}_{" + Particle + "," + HistogramTitle3 + "} - (#chi^{2}_{" + Particle + "," + HistogramTitle3 + "})_{max}| #leq " +
                         ToStringWithPrecision(plot_cuts) + ")}";
        Histogram->SetTitle(HistogramTitle.c_str());
        Histogram->GetYaxis()->SetTitle("Number of events");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Sumw2();
            Histogram->Draw();
        }
    }

    Histogram->GetXaxis()->SetTitleSize(titleSize);
    Histogram->GetXaxis()->SetLabelSize(labelSizex);
    Histogram->GetXaxis()->CenterTitle(centerTitle);
    Histogram->GetYaxis()->SetTitleSize(titleSize);
    Histogram->GetYaxis()->SetLabelSize(labelSizey);
    Histogram->GetYaxis()->CenterTitle(centerTitle);
    Histogram->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram);

    if (!ShowStats) { Histogram->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_cuts + plot_Xmax;
        double Lower_cut = -plot_cuts + plot_Xmax;
        double plot_xmax = plot_Xmax;
        TLine* upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine* lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine* max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.35);

        if (Histogram->Integral() != 0.) {
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

            TLegendEntry* Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut)).c_str(), "l");
            TLegendEntry* Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut)).c_str(), "l");

            if (plot_max) {
                TLegendEntry* Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax)).c_str(), "l");
                //            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + ToStringWithPrecision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string HistogramSaveNameDir = HistogramSaveNamePath + HistogramSaveName + "_log_scale_" + HistogramTitle3 + ".pdf";
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string HistogramSaveNameDir = HistogramSaveNamePath + HistogramSaveName + "_linear_scale_" + HistogramTitle3 + ".pdf";
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (AddToStack) {
        Histogram->SetLineColor(kColor);
        Histogram->SetStats(0);
        HistogramStack->Add(Histogram);
    }

    HistogramCanvas->Clear();
    gStyle->SetTitleFontSize(TitleFontSize);
    gROOT->ForceStyle();

    HistogramCanvas1->cd();
}
#pragma endregion

#pragma region                                 /* histPlotter1D function (Chi2 for separate plots, stackless) */
void histPlotter1D(TCanvas* HistogramCanvas1,  // canvas c1 of other histograms
                   TCanvas* HistogramCanvas,   // canvas c2 of chi2 histograms
                   TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle1, std::string HistogramTitle2,
                   std::string HistogramTitle3, std::string Particle, double titleSize, double labelSizex, double labelSizey, TList* Histogram_list, vector<TObject*>& HistoList,
                   int lineWidth, bool LogScalePlot, bool LinearScalePlot, std::string HistogramSaveName, const std::string& HistogramSaveNamePath, bool centerTitle, bool ShowStats,
                   bool title2, bool apply_plot_cuts, double plot_cuts, double plot_Xmax, bool plot_max) {
    double TitleFontSize = gStyle->GetTitleFontSize();
    HistogramCanvas->cd();

    gStyle->SetOptStat("ourmen");

    double Histogram_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram_integral = Histogram->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        std::string title = HistogramTitle1 + " (" + HistogramTitle2 + ", " + HistogramTitle3 + ")" + " - Normalized";
        std::string HistogramTitle = "#splitline{" + title + "}{        (|#chi^{2}_{" + Particle + "," + HistogramTitle3 + "} - #bar{#chi^{2}}_{" + Particle + "," + HistogramTitle3 +
                                     "}| #leq " + ToStringWithPrecision(plot_cuts) + ")}";
        Histogram->SetTitle(HistogramTitle.c_str());
        Histogram->GetYaxis()->SetTitle("Probability (%)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Scale(100. / Histogram_integral, "nosw2");
            Histogram->Sumw2();
            Histogram->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string HistogramTitle, title, subtitle;

        if (title2 == false) {
            title = HistogramTitle1 + " (" + HistogramTitle2 + ", " + HistogramTitle3 + ")";
        } else {
            title = HistogramTitle1 + " (" + HistogramTitle2 + ")";
        }

        HistogramTitle = "#splitline{" + title + "}{        (|#chi^{2}_{" + Particle + "," + HistogramTitle3 + "} - (#chi^{2}_{" + Particle + "," + HistogramTitle3 + "})_{max}| #leq " +
                         ToStringWithPrecision(plot_cuts) + ")}";
        Histogram->SetTitle(HistogramTitle.c_str());
        Histogram->GetYaxis()->SetTitle("Number of events");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Sumw2();
            Histogram->Draw();
        }
    }

    Histogram->GetXaxis()->SetTitleSize(titleSize);
    Histogram->GetXaxis()->SetLabelSize(labelSizex);
    Histogram->GetXaxis()->CenterTitle(centerTitle);
    Histogram->GetYaxis()->SetTitleSize(titleSize);
    Histogram->GetYaxis()->SetLabelSize(labelSizey);
    Histogram->GetYaxis()->CenterTitle(centerTitle);
    Histogram->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram);

    if (!ShowStats) { Histogram->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_cuts + plot_Xmax;
        double Lower_cut = -plot_cuts + plot_Xmax;
        double plot_xmax = plot_Xmax;
        TLine* upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine* lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine* max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.35);

        if (Histogram->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry* Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut)).c_str(), "l");
            TLegendEntry* Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut)).c_str(), "l");

            if (plot_max) { TLegendEntry* Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax)).c_str(), "l"); }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string HistogramSaveNameDir = HistogramSaveNamePath + HistogramSaveName + "_log_scale_" + HistogramTitle3 + ".pdf";
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string HistogramSaveNameDir = HistogramSaveNamePath + HistogramSaveName + "_linear_scale_" + HistogramTitle3 + ".pdf";
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    HistogramCanvas->Clear();
    gStyle->SetTitleFontSize(TitleFontSize);
    gROOT->ForceStyle();

    HistogramCanvas1->cd();
}
#pragma endregion

// histPlotter1D function (applying all Chi2 cuts) ----------------------------------------------------------------------------------------------------------------------

#pragma region                                 /* histPlotter1D function (applying all Chi2 cuts) */
void histPlotter1D(TCanvas* HistogramCanvas1,  // canvas c1 of other histograms
                   TCanvas* HistogramCanvas,   // canvas c2 of chi2 histograms
                   TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle1, std::string HistogramTitle2,
                   std::string HistogramTitle3, std::string Particle1, std::string Particle2, double titleSize, double labelSizex, double labelSizey, TList* Histogram_list,
                   vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack* HistogramStack, std::string HistogramSaveName,
                   const std::string& HistogramSaveNamePath, int kColor, bool centerTitle, bool AddToStack, bool ShowStats, bool title2, bool apply_plot_cuts, double plot_cuts1,
                   double plot_Xmax1, double plot_cuts2, double plot_Xmax2, bool plot_max) {
    double TitleFontSize = gStyle->GetTitleFontSize();
    HistogramCanvas->cd();

    gStyle->SetOptStat("ourmen");

    double Histogram_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram_integral = Histogram->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram_integral = custom_normalization_factor;
    }

    //    TPaveText *PrametesrBox1 = new TPaveText(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 - 0.4, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 - 0.05, "NDC");
    //    PrametesrBox1->SetTextSize(12);
    //    PrametesrBox1->SetFillColor(0);
    ////            PrametesrBox1->SetTextAlign(12);
    //    PrametesrBox1->AddText(("|#chi^{2}_{" + Particle1 + "," + HistogramTitle3 + "} - ({#chi^{2}}_{" + Particle1 + "," +
    //                           HistogramTitle3 + "})_{max}| #leq " + ToStringWithPrecision(plot_cuts1)).c_str());
    //    PrametesrBox1->AddText(("|#chi^{2}_{" + Particle2 + "," + HistogramTitle3 + "} - ({#chi^{2}}_{" + Particle2 + "," +
    //                           HistogramTitle3 + "})_{max}| #leq " + ToStringWithPrecision(plot_cuts2)).c_str());

    //    TPaveText *PrametesrBox = new TPaveText(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 - 0.4, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 - 0.05, "NDC");
    //    PrametesrBox->SetTextSize(12);
    //    PrametesrBox->SetFillColor(0);
    ////            PrametesrBox->SetTextAlign(12);
    //    PrametesrBox->AddText(("|#chi^{2}_{" + Particle1 + "," + HistogramTitle3 + "} - ({#chi^{2}}_{" + Particle1 + "," +
    //                           HistogramTitle3 + "})_{max}| #leq " + ToStringWithPrecision(plot_cuts1)).c_str());
    //    PrametesrBox->AddText(("|#chi^{2}_{" + Particle2 + "," + HistogramTitle3 + "} - ({#chi^{2}}_{" + Particle2 + "," +
    //                           HistogramTitle3 + "})_{max}| #leq " + ToStringWithPrecision(plot_cuts2)).c_str());

    if (normalize_Histogram) {
        std::string title = HistogramTitle1 + " (" + HistogramTitle2 + ", " + HistogramTitle3 + ")" + " - Normalized";
        std::string HistogramTitle = "#splitline{" + title + "}{" + "(|#chi^{2}_{" + Particle1 + "," + HistogramTitle3 + "} - (#chi^{2}_{" + Particle1 + "," + HistogramTitle3 +
                                     "})_{max}| #leq " + ToStringWithPrecision(plot_cuts1) + ", " + "|#chi^{2}_{" + Particle2 + "," + HistogramTitle3 + "} -(#chi^{2}_{" + Particle2 + "," +
                                     HistogramTitle3 + "})_{max}| #leq " + ToStringWithPrecision(plot_cuts2) + ")}";
        Histogram->SetTitle(HistogramTitle.c_str());
        Histogram->GetYaxis()->SetTitle("Probability (%)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Scale(100. / Histogram_integral, "nosw2");
            Histogram->Sumw2();
            Histogram->Draw();
            //            PrametesrBox1->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string HistogramTitle, title, subtitle;

        if (title2 == false) {
            title = HistogramTitle1 + " (" + HistogramTitle2 + ", " + HistogramTitle3 + ")";
        } else {
            title = HistogramTitle1 + " (" + HistogramTitle2 + ")";
        }

        HistogramTitle = "#splitline{" + title + "}{" + "(|#chi^{2}_{" + Particle1 + "," + HistogramTitle3 + "} - (#chi^{2}_{" + Particle1 + "," + HistogramTitle3 + "})_{max}| #leq " +
                         ToStringWithPrecision(plot_cuts1) + ", " + "|#chi^{2}_{" + Particle2 + "," + HistogramTitle3 + "} - (#chi^{2}_{" + Particle2 + "," + HistogramTitle3 +
                         "})_{max}| #leq " + ToStringWithPrecision(plot_cuts2) + ")}";
        Histogram->SetTitle(HistogramTitle.c_str());
        Histogram->GetYaxis()->SetTitle("Number of events");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Sumw2();
            Histogram->Draw();
            //            PrametesrBox1->Draw();
        }
    }

    Histogram->GetXaxis()->SetTitleSize(titleSize);
    Histogram->GetXaxis()->SetLabelSize(labelSizex);
    Histogram->GetXaxis()->CenterTitle(centerTitle);
    Histogram->GetYaxis()->SetTitleSize(titleSize);
    Histogram->GetYaxis()->SetLabelSize(labelSizey);
    Histogram->GetYaxis()->CenterTitle(centerTitle);
    Histogram->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram);

    if (!ShowStats) { Histogram->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_cuts1 + plot_Xmax1;
        double Lower_cut = -plot_cuts1 + plot_Xmax1;
        double plot_xmax1 = plot_Xmax1;
        TLine* upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine* lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine* max_location = new TLine(plot_Xmax1, 0., plot_Xmax1, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.35);

        if (Histogram->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry* Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut)).c_str(), "l");
            TLegendEntry* Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut)).c_str(), "l");

            if (plot_max) { TLegendEntry* Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax1)).c_str(), "l"); }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string HistogramSaveNameDir = HistogramSaveNamePath + HistogramSaveName + "_log_scale_" + HistogramTitle3 + ".pdf";
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string HistogramSaveNameDir = HistogramSaveNamePath + HistogramSaveName + "_linear_scale_" + HistogramTitle3 + ".pdf";
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (AddToStack) {
        Histogram->SetLineColor(kColor);
        Histogram->SetStats(0);
        HistogramStack->Add(Histogram);
    }

    HistogramCanvas->Clear();
    gStyle->SetTitleFontSize(TitleFontSize);
    gROOT->ForceStyle();

    HistogramCanvas1->cd();
}
#pragma endregion

#pragma region                                 /* histPlotter1D function (applying all Chi2 cuts, stackless) */
void histPlotter1D(TCanvas* HistogramCanvas1,  // canvas c1 of other histograms
                   TCanvas* HistogramCanvas,   // canvas c2 of chi2 histograms
                   TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle1, std::string HistogramTitle2,
                   std::string HistogramTitle3, std::string Particle1, std::string Particle2, double titleSize, double labelSizex, double labelSizey, TList* Histogram_list,
                   vector<TObject*>& HistoList, int lineWidth, bool LogScalePlot, bool LinearScalePlot, std::string HistogramSaveName, const std::string& HistogramSaveNamePath,
                   bool centerTitle, bool ShowStats, bool title2, bool apply_plot_cuts, double plot_cuts1, double plot_Xmax1, double plot_cuts2, double plot_Xmax2, bool plot_max) {
    double TitleFontSize = gStyle->GetTitleFontSize();
    HistogramCanvas->cd();

    gStyle->SetOptStat("ourmen");

    double Histogram_integral;  // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    if (normalize_Histogram && !custom_normalization) {
        Histogram_integral = Histogram->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        std::string title = HistogramTitle1 + " (" + HistogramTitle2 + ", " + HistogramTitle3 + ")" + " - Normalized";
        std::string HistogramTitle = "#splitline{" + title + "}{" + "(|#chi^{2}_{" + Particle1 + "," + HistogramTitle3 + "} - (#chi^{2}_{" + Particle1 + "," + HistogramTitle3 +
                                     "})_{max}| #leq " + ToStringWithPrecision(plot_cuts1) + ", " + "|#chi^{2}_{" + Particle2 + "," + HistogramTitle3 + "} -(#chi^{2}_{" + Particle2 + "," +
                                     HistogramTitle3 + "})_{max}| #leq " + ToStringWithPrecision(plot_cuts2) + ")}";
        Histogram->SetTitle(HistogramTitle.c_str());
        Histogram->GetYaxis()->SetTitle("Probability (%)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Scale(100. / Histogram_integral, "nosw2");
            Histogram->Sumw2();
            Histogram->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string HistogramTitle, title, subtitle;

        if (title2 == false) {
            title = HistogramTitle1 + " (" + HistogramTitle2 + ", " + HistogramTitle3 + ")";
        } else {
            title = HistogramTitle1 + " (" + HistogramTitle2 + ")";
        }

        HistogramTitle = "#splitline{" + title + "}{" + "(|#chi^{2}_{" + Particle1 + "," + HistogramTitle3 + "} - (#chi^{2}_{" + Particle1 + "," + HistogramTitle3 + "})_{max}| #leq " +
                         ToStringWithPrecision(plot_cuts1) + ", " + "|#chi^{2}_{" + Particle2 + "," + HistogramTitle3 + "} - (#chi^{2}_{" + Particle2 + "," + HistogramTitle3 +
                         "})_{max}| #leq " + ToStringWithPrecision(plot_cuts2) + ")}";
        Histogram->SetTitle(HistogramTitle.c_str());
        Histogram->GetYaxis()->SetTitle("Number of events");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Sumw2();
            Histogram->Draw();
        }
    }

    Histogram->GetXaxis()->SetTitleSize(titleSize);
    Histogram->GetXaxis()->SetLabelSize(labelSizex);
    Histogram->GetXaxis()->CenterTitle(centerTitle);
    Histogram->GetYaxis()->SetTitleSize(titleSize);
    Histogram->GetYaxis()->SetLabelSize(labelSizey);
    Histogram->GetYaxis()->CenterTitle(centerTitle);
    Histogram->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram);

    if (!ShowStats) { Histogram->SetStats(0); }

    if (apply_plot_cuts) {
        gPad->Update();
        double Upper_cut = plot_cuts1 + plot_Xmax1;
        double Lower_cut = -plot_cuts1 + plot_Xmax1;
        double plot_xmax1 = plot_Xmax1;
        TLine* upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine* lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine* max_location = new TLine(plot_Xmax1, 0., plot_Xmax1, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.35);

        if (Histogram->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry* Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + ToStringWithPrecision(Upper_cut)).c_str(), "l");
            TLegendEntry* Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + ToStringWithPrecision(Lower_cut)).c_str(), "l");

            if (plot_max) { TLegendEntry* Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + ToStringWithPrecision(plot_xmax1)).c_str(), "l"); }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string HistogramSaveNameDir = HistogramSaveNamePath + HistogramSaveName + "_log_scale_" + HistogramTitle3 + ".pdf";
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string HistogramSaveNameDir = HistogramSaveNamePath + HistogramSaveName + "_linear_scale_" + HistogramTitle3 + ".pdf";
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
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
void histPlotter1DwFit(TCanvas* HistogramCanvas, TH1D* Histogram, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor, std::string HistogramTitle,
                       std::string HistogramTitleReactions, TList* Histogram_list, vector<TObject*>& HistoList, std::string HistogramSaveName, const std::string& HistogramSaveNamePath,
                       std::string finalState, double& plot_Xmax, double& plot_lcut, double& plot_ucut, double factor, bool plot_max, std::string particle) {
    double Histogram_integral;  // To be calculated only if normalize_Histogram
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
        Histogram_integral = Histogram->Integral();
    } else if (normalize_Histogram && custom_normalization) {
        Histogram_integral = custom_normalization_factor;
    }

    if (normalize_Histogram) {
        std::string title = HistogramTitle + " (" + HistogramTitleReactions + ", " + finalState + ")" + " - Normalized";
        const char* HistogramTitle = title.c_str();
        Histogram->SetTitle(HistogramTitle);
        Histogram->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Scale(100. / Histogram_integral, "nosw2");
            Histogram->Sumw2();
            Histogram->Draw();
        }
    } else if (!normalize_Histogram) {
        std::string title;

        if (title2 == false) {
            title = HistogramTitle + " (" + HistogramTitleReactions + ", " + finalState + ")";
        } else {
            title = HistogramTitle + " (" + finalState + ")";
        }

        const char* HistogramTitle = title.c_str();
        Histogram->SetTitle(HistogramTitle);
        Histogram->GetYaxis()->SetTitle("Number of events");

        if (Histogram->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Histogram->Sumw2();
            Histogram->Draw();
            displayText->Draw();
        } else if (Histogram->Integral() != 0.) {
            Histogram->Sumw2();
            Histogram->Draw();
        }
    }

    Histogram->GetXaxis()->SetTitleSize(titleSize);
    Histogram->GetXaxis()->SetLabelSize(labelSizex);
    Histogram->GetXaxis()->CenterTitle(centerTitle);
    Histogram->GetYaxis()->SetTitleSize(titleSize);
    Histogram->GetYaxis()->SetLabelSize(labelSizey);
    Histogram->GetYaxis()->CenterTitle(centerTitle);
    Histogram->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram);

    if (!ShowStats) { Histogram->SetStats(0); }

    if (particle != "") { cout << "\n\nFit results for " << particle << " in " << finalState << " are (" << HistogramTitleReactions << "):\n\n"; }

    if (apply_plot_cuts) {
        /* A fit to a gaussian with 3 parameters: f(x) = p0*exp(-0.5((x-p1)/p2)^2)). */
        Histogram->Fit("gaus");

        /* Get fitted function to TF1 plot */
        TF1* fit = Histogram->GetFunction("gaus");
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
        Histogram->GetListOfFunctions()->Remove(Histogram->GetFunction("gaus"));

        /* Draw TF1 plot as curved function */
        fit->SetLineColor(kMagenta);
        fit->Draw("same && C");

        gPad->Update();

        double Upper_cut = plot_ucut + plot_Xmax, Lower_cut = plot_lcut + plot_Xmax, plot_xmax = plot_Xmax;
        TLine* upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine* lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine* max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);

        double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
        double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
        auto Cut_legend = new TLegend(x_1_Cut_legend, y_1_Cut_legend, x_2_Cut_legend, y_2_Cut_legend);

        if (Histogram->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry* Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("UC = " + ToStringWithPrecision(Upper_cut, 3)).c_str(), "l");
            TLegendEntry* Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("LC = " + ToStringWithPrecision(Lower_cut, 3)).c_str(), "l");

            if (plot_max) { TLegendEntry* Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Mean = " + ToStringWithPrecision(plot_xmax, 3)).c_str(), "l"); }

            Cut_legend->Draw("same");

            double x_1_FitParam = gStyle->GetStatX(), y_1_FitParam = y_1_Cut_legend - 0.14;
            double x_2_FitParam = gStyle->GetStatX() - 0.2, y_2_FitParam = y_1_Cut_legend - 0.245;

            TPaveText* FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
            FitParam->SetBorderSize(1);
            FitParam->SetTextFont(0);
            FitParam->SetFillColor(0);
            FitParam->SetTextAlign(11);
            FitParam->AddText(("Fit amp = " + ToStringWithPrecision(Amp, 8)).c_str());
            FitParam->AddText(("Fit std = " + ToStringWithPrecision(Std, 8)).c_str());
            FitParam->AddText(("Fit mean = " + ToStringWithPrecision(Mean, 8)).c_str());
            ((TText*)FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
            FitParam->AddText(("Cuts = std * " + ToStringWithPrecision(factor, 2) + " = " + ToStringWithPrecision(plot_ucut, 3)).c_str());
            ((TText*)FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
            FitParam->Draw("same");
        }
    }

    if (particle != "") { cout << "\n"; }

    if (LogScalePlot) {
        HistogramCanvas->SetLogy(1);
        std::string HistogramSaveNameDir = HistogramSaveNamePath + HistogramSaveName + "_log_scale_" + finalState + ".pdf";
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    if (LinearScalePlot) {
        HistogramCanvas->SetLogy(0);
        std::string HistogramSaveNameDir = HistogramSaveNamePath + HistogramSaveName + "_linear_scale_" + finalState + ".pdf";
        HistogramCanvas->SaveAs(HistogramSaveNameDir.c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir);

        HistoList.push_back(Histogram);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                   stackPlotter1D function                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// stackPlotter1D function (regular) -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* stackPlotter1D (regular) */
void stackPlotter1D(TCanvas* HistogramCanvas, THStack* HistogramStack, bool normalize_Histogram, std::string HistogramTitle, std::string HistogramTitleReactions, TList* Histogram_list,
                    vector<TObject*>& HistoList, TH1D* H1D_All_Int, TH1D* H1D_QEL, TH1D* H1D_MEC, TH1D* H1D_RES, TH1D* H1D_DIS, std::string HistogramStackSaveName,
                    std::string HistogramStackSaveNamePath, std::string finalState) {
    gStyle->SetOptStat("ourmen");

    //  Normalization factor:
    double Histogram_integral;  // To be calculated only if normalize_Histogram
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

    HistogramStack->Draw("nostack");
    HistogramStack->GetHistogram()->GetXaxis()->SetTitleSize(TitleSize);
    HistogramStack->GetHistogram()->GetXaxis()->SetLabelSize(LabelSize);
    HistogramStack->GetHistogram()->GetXaxis()->CenterTitle(centerTitle);
    HistogramStack->GetHistogram()->GetYaxis()->SetLabelSize(LabelSize);
    HistogramStack->GetHistogram()->GetYaxis()->CenterTitle(centerTitle);

    if (normalize_Histogram) {
        std::string title;

        if (finalState == "") {
            title = HistogramTitle + " (" + HistogramTitleReactions + ")" + " - Normalized";
        } else {
            title = HistogramTitle + " (" + HistogramTitleReactions + ", " + finalState + ")" + " - Normalized";
        }

        const char* HistogramTitle = title.c_str();
        HistogramStack->SetTitle(HistogramTitle);
        //        HistogramStack->GetXaxis()->SetTitle(xLable.c_str());
        HistogramStack->GetYaxis()->SetTitle("Probability (%)");
        HistogramStack->GetHistogram()->GetYaxis()->SetTitleSize(TitleSize);

        if (H1D_All_Int->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            HistogramStack->Draw();
            displayText->Draw();
        }
    } else {
        std::string title;

        if (finalState == "") {
            title = HistogramTitle + " (" + HistogramTitleReactions + ")";
        } else {
            title = HistogramTitle + " (" + HistogramTitleReactions + ", " + finalState + ")";
        }

        const char* HistogramTitle = title.c_str();
        HistogramStack->SetTitle(HistogramTitle);

        if (H1D_All_Int->Integral() == 0.) {
            TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            HistogramStack->Draw();
            displayText->Draw();
        } else if (H1D_All_Int->Integral() != 0.) {
            //            HistogramStack->GetXaxis()->SetTitle(xLable.c_str());
            HistogramStack->GetYaxis()->SetTitle("Number of events");
            HistogramStack->GetHistogram()->GetYaxis()->SetTitleSize(TitleSize);
        }
    }

    auto HistogramStackLegend = new TLegend(0.76, 0.624, 0.865, 0.89);  // TLA
                                                                        //    auto HistogramStackLegend = new TLegend(0.77, 0.624, 0.875, 0.89); // DSA
                                                                        //    auto HistogramStackLegend = new TLegend(0.775, 0.625, 0.9, 0.9);

    if (H1D_All_Int->Integral() != 0.) {
        TLegendEntry* H1D_All_Int_Entry = HistogramStackLegend->AddEntry(H1D_All_Int, "All int.", "l");
        TLegendEntry* H1D_QEL_Entry = HistogramStackLegend->AddEntry(H1D_QEL, "QE", "l");
        TLegendEntry* H1D_MEC_Entry = HistogramStackLegend->AddEntry(H1D_MEC, "MEC", "l");
        TLegendEntry* H1D_RES_Entry = HistogramStackLegend->AddEntry(H1D_RES, "RES", "l");
        TLegendEntry* H1D_DIS_Entry = HistogramStackLegend->AddEntry(H1D_DIS, "DIS", "l");
        HistogramStackLegend->Draw();
    }

    Histogram_list->Add(HistogramStack);

    if (finalState == "") {
        HistogramCanvas->SaveAs((HistogramStackSaveNamePath + HistogramStackSaveName + "_" + HistogramTitleReactions + ".pdf").c_str());
        hf::FixPDFOrientation(HistogramStackSaveNamePath + HistogramStackSaveName + "_" + HistogramTitleReactions + ".pdf");

        HistoList.push_back(HistogramStack);
    } else {
        HistogramCanvas->SaveAs((HistogramStackSaveNamePath + HistogramStackSaveName + "_" + HistogramTitleReactions + "_" + finalState + ".pdf").c_str());
        hf::FixPDFOrientation(HistogramStackSaveNamePath + HistogramStackSaveName + "_" + HistogramTitleReactions + "_" + finalState + ".pdf");

        HistoList.push_back(HistogramStack);
    }

    HistogramCanvas->Clear();
}
#pragma endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                   histPlotter2D function                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// histPlotter2D function (regular) -------------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (regular) */
void histPlotter2D(TCanvas* HistogramCanvas, TH2D* Histogram, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList* Histogram_list,
                   vector<TObject*>& HistoList, bool zLogScalePlot, std::string HistogramSaveNameDir, std::string HistogramSaveName, bool ShowStats) {
    gStyle->SetOptStat("ourmen");

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Histogram->SetTitleSize(titleSize, "xyz");
    Histogram->GetXaxis()->SetLabelSize(labelSizex);
    Histogram->GetXaxis()->CenterTitle(centerTitle);
    Histogram->GetYaxis()->SetLabelSize(labelSizey);
    Histogram->GetYaxis()->CenterTitle(centerTitle);
    Histogram->GetZaxis()->SetLabelSize(labelSizez);
    Histogram_list->Add(Histogram);

    if (Histogram->Integral() == 0.) {
        Histogram->SetStats(0);
        TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->SetTextAlign(12);
        displayText->AddText("Empty histogram");
        Histogram->Draw();
        displayText->Draw();
    } else if (Histogram->Integral() != 0.) {
        Histogram->Draw("colz");

        gPad->Update();
        TPaletteAxis* palette = (TPaletteAxis*)Histogram->GetListOfFunctions()->FindObject("palette");
        if (palette) { palette->SetY2NDC(0.475), gPad->Modified(), gPad->Update(); }
    }

    if (zLogScalePlot) {
        HistogramCanvas->SetLogz(1);
        system(("mkdir -p " + HistogramSaveNameDir + "/z_Log_Scale_plots").c_str());
        HistogramCanvas->SaveAs((HistogramSaveNameDir + "/z_Log_Scale_plots/" + HistogramSaveName + "_zLogScale.pdf").c_str());
        hf::FixPDFOrientation(HistogramSaveNameDir + "/z_Log_Scale_plots/" + HistogramSaveName + "_zLogScale.pdf");

        HistoList.push_back(Histogram);
    }

    if (!ShowStats) { Histogram->SetStats(0); }

    // gStyle->SetStatX(0.98);  // // gStyle->SetStatX(0.87);
    // gStyle->SetStatY(0.875);

    HistogramCanvas->SetLogz(0);
    HistogramCanvas->SaveAs((HistogramSaveNameDir + HistogramSaveName + ".pdf").c_str());
    hf::FixPDFOrientation(HistogramSaveNameDir + HistogramSaveName + ".pdf");

    HistogramCanvas->Clear();
}
#pragma endregion

// histPlotter2D function (Beta vs. P plots, all particles) -------------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (Beta vs. P plots, all particles) */
void histPlotter2D(TCanvas* HistogramCanvas, TH2D* Histogram, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList* Histogram_list,
                   vector<TObject*>& HistoList, bool zLogScalePlot, std::string HistogramSaveNameDir, std::string HistogramSaveName, TF1* Beta_function1, TF1* Beta_function2,
                   TF1* Beta_function3, TF1* Beta_function4, TF1* Beta_function5, TF1* Beta_function6, TF1* Beta_function7, TF1* Beta_function8, TF1* Beta_function9) {
    gStyle->SetOptStat("ourmen");

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Histogram->SetTitleSize(titleSize, "xyz");
    Histogram->GetXaxis()->SetLabelSize(labelSizex);
    Histogram->GetXaxis()->CenterTitle(centerTitle);
    Histogram->GetYaxis()->SetLabelSize(labelSizey);
    Histogram->GetYaxis()->CenterTitle(centerTitle);
    Histogram->GetZaxis()->SetLabelSize(labelSizez);
    Histogram_list->Add(Histogram);

    if (Histogram->Integral() == 0.) {
        TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->SetTextAlign(12);
        displayText->AddText("Empty histogram");
        Histogram->Draw();
        displayText->Draw();
    } else if (Histogram->Integral() != 0.) {
        Histogram->Draw("colz");

        gPad->Update();
        TPaletteAxis* palette = (TPaletteAxis*)Histogram->GetListOfFunctions()->FindObject("palette");
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

    // gStyle->SetStatX(0.98);   // // gStyle->SetStatX(0.87);
    // gStyle->SetStatY(0.935);  // // gStyle->SetStatY(0.4);
    HistogramCanvas->SaveAs((HistogramSaveNameDir + HistogramSaveName).c_str());
    hf::FixPDFOrientation(HistogramSaveNameDir + HistogramSaveName);

    HistoList.push_back(Histogram);

    HistogramCanvas->Clear();
}
#pragma endregion

// histPlotter2D function (Beta vs. P plots, single particle) -----------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (Beta vs. P plots, single particle) */
void histPlotter2D(TCanvas* HistogramCanvas, TH2D* Histogram, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList* Histogram_list,
                   vector<TObject*>& HistoList, bool zLogScalePlot, std::string HistogramSaveNameDir, std::string HistogramSaveName, TF1* Beta_function1, std::string particle1,
                   bool plot_legend) {
    gStyle->SetOptStat("ourmen");

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.165, y_1 = 0.3, x_2 = 0.865, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Histogram->SetTitleSize(titleSize, "xyz");
    Histogram->GetXaxis()->SetLabelSize(labelSizex);
    Histogram->GetXaxis()->CenterTitle(centerTitle);
    Histogram->GetYaxis()->SetLabelSize(labelSizey);
    Histogram->GetYaxis()->CenterTitle(centerTitle);
    Histogram->GetZaxis()->SetLabelSize(labelSizez);
    Histogram_list->Add(Histogram);

    if (Histogram->Integral() == 0.) {
        Histogram->SetStats(0);
        TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->SetTextAlign(12);
        displayText->AddText("Empty histogram");
        Histogram->Draw("colz");
        displayText->Draw();
    } else if (Histogram->Integral() != 0.) {
        Histogram->Draw("colz");

        gPad->Update();
        TPaletteAxis* palette = (TPaletteAxis*)Histogram->GetListOfFunctions()->FindObject("palette");
        if (palette) { palette->SetY2NDC(0.475), gPad->Modified(), gPad->Update(); }

        Beta_function1->SetLineColor(kRed);
        Beta_function1->Draw("same");
    }

    auto Beta_vs_P_legend = new TLegend(0.87, 0.725 - 0.25, 0.87 - 0.2, 0.725 - 0.3);

    if ((plot_legend == true) && (Histogram->Integral() != 0.)) {
        TLegendEntry* Beta_function1_entry = Beta_vs_P_legend->AddEntry(Beta_function1, particle1.c_str(), "l");

        Beta_vs_P_legend->Draw("same");
    }

    if (zLogScalePlot) { HistogramCanvas->SetLogz(1); }

    // gStyle->SetStatX(0.98);   // // gStyle->SetStatX(0.87);
    // gStyle->SetStatY(0.935);  // // gStyle->SetStatY(0.4);
    HistogramCanvas->SaveAs((HistogramSaveNameDir + HistogramSaveName + ".pdf").c_str());
    hf::FixPDFOrientation(HistogramSaveNameDir + HistogramSaveName + ".pdf");

    HistoList.push_back(Histogram);

    HistogramCanvas->Clear();
}
#pragma endregion

// histPlotter2D function (Beta vs. P plots, by charge) -----------------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (Beta vs. P plots, by charge) */
void histPlotter2D(TCanvas* HistogramCanvas, TH2D* Histogram, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez, TList* Histogram_list,
                   vector<TObject*>& HistoList, bool zLogScalePlot, std::string HistogramSaveNameDir, std::string HistogramSaveName, TF1* Beta_function1, std::string particle1,
                   TF1* Beta_function2, std::string particle2, TF1* Beta_function3, std::string particle3, bool plot_legend) {
    gStyle->SetOptStat("ourmen");

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.165, y_1 = 0.3, x_2 = 0.865, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Histogram->SetTitleSize(titleSize, "xyz");
    Histogram->GetXaxis()->SetLabelSize(labelSizex);
    Histogram->GetXaxis()->CenterTitle(centerTitle);
    Histogram->GetYaxis()->SetLabelSize(labelSizey);
    Histogram->GetYaxis()->CenterTitle(centerTitle);
    Histogram->GetZaxis()->SetLabelSize(labelSizez);
    Histogram_list->Add(Histogram);

    if (Histogram->Integral() == 0.) {
        Histogram->SetStats(0);
        TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->SetTextAlign(12);
        displayText->AddText("Empty histogram");
        Histogram->Draw("colz");
        displayText->Draw();
    } else if (Histogram->Integral() != 0.) {
        Histogram->Draw("colz");

        gPad->Update();
        TPaletteAxis* palette = (TPaletteAxis*)Histogram->GetListOfFunctions()->FindObject("palette");
        if (palette) { palette->SetY2NDC(0.475), gPad->Modified(), gPad->Update(); }

        Beta_function1->SetLineColor(kBlue);
        Beta_function1->Draw("same");
        Beta_function2->SetLineColor(kGreen);
        Beta_function2->Draw("same");
        Beta_function3->SetLineColor(kRed);
        Beta_function3->Draw("same");
    }

    auto Beta_vs_P_legend = new TLegend(0.87, 0.725 - 0.2, 0.87 - 0.2, 0.725 - 0.3);

    if ((plot_legend == true) && (Histogram->Integral() != 0.)) {
        TLegendEntry* Beta_function1_entry = Beta_vs_P_legend->AddEntry(Beta_function1, particle1.c_str(), "l");
        TLegendEntry* Beta_function2_entry = Beta_vs_P_legend->AddEntry(Beta_function2, particle2.c_str(), "l");
        TLegendEntry* Beta_function3_entry = Beta_vs_P_legend->AddEntry(Beta_function3, particle3.c_str(), "l");

        Beta_vs_P_legend->Draw("same");
    }

    if (zLogScalePlot) { HistogramCanvas->SetLogz(1); }

    // gStyle->SetStatX(0.98);   // // gStyle->SetStatX(0.87);
    // gStyle->SetStatY(0.935);  // // gStyle->SetStatY(0.4);
    HistogramCanvas->SaveAs((HistogramSaveNameDir + HistogramSaveName + ".pdf").c_str());
    hf::FixPDFOrientation(HistogramSaveNameDir + HistogramSaveName + ".pdf");

    HistoList.push_back(Histogram);

    HistogramCanvas->Clear();
}
#pragma endregion
