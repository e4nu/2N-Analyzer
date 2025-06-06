//
// Created by Alon Sportes on 07/04/2023.
//

#ifndef HPLOT2D_H
#define HPLOT2D_H

#include "hPlot2D.h"

// Constructor definition -----------------------------------------------------------------------------------------------------------------------------------------------

hPlot2D::hPlot2D(std::string h2DtReactions, std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string yat, double LowerXlim, double UpperXlim,
                 double LowerYlim, double UpperYlim, int hnoXb = 150, int hnoYb = 150) {
    Histogram2DTitles["Histogram2DTitleReactions"] = h2DtReactions, Histogram2DTitles["FinalState"] = fState, Histogram2DTitles["DetectorRegion"] = dRegion;
    Histogram2DTitles["HistogramStatTitle"] = hst, Histogram2DTitles["HistogramTitle"] = ht, Histogram2DTitles["XaxisTitle"] = xat, Histogram2DTitles["YaxisTitle"] = yat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramYAxisLimits.push_back(LowerYlim), HistogramYAxisLimits.push_back(UpperYlim);
    HistogramNumberOfXBins = hnoXb, HistogramNumberOfYBins = hnoYb;
    HistogramStatsTitle = hst;

    Histogram2D = new TH2D((Histogram2DTitles["HistogramStatTitle"] + " (" + Histogram2DTitles["Histogram2DTitleReactions"] + ", " + Histogram2DTitles["FinalState"] + ", " +
                            Histogram2DTitles["DetectorRegion"] + ")")
                               .c_str(),
                           (Histogram2DTitles["HistogramTitle"] + " (" + Histogram2DTitles["Histogram2DTitleReactions"] + ", " + Histogram2DTitles["FinalState"] + ", " +
                            Histogram2DTitles["DetectorRegion"] + ")" + ";" + Histogram2DTitles["XaxisTitle"] + ";" + Histogram2DTitles["YaxisTitle"])
                               .c_str(),
                           HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1), HistogramNumberOfYBins, HistogramYAxisLimits.at(0), HistogramYAxisLimits.at(1));
}

hPlot2D::hPlot2D(std::string h2DtReactions, std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string yat, std::string sPath, std::string sName,
                 double LowerXlim, double UpperXlim, double LowerYlim, double UpperYlim, int hnoXb = 150, int hnoYb = 150) {
    Histogram2DTitles["Histogram2DTitleReactions"] = h2DtReactions, Histogram2DTitles["FinalState"] = fState, Histogram2DTitles["DetectorRegion"] = dRegion;
    Histogram2DTitles["HistogramStatTitle"] = hst, Histogram2DTitles["HistogramTitle"] = ht, Histogram2DTitles["XaxisTitle"] = xat, Histogram2DTitles["YaxisTitle"] = yat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramYAxisLimits.push_back(LowerYlim), HistogramYAxisLimits.push_back(UpperYlim);
    HistogramNumberOfXBins = hnoXb, HistogramNumberOfYBins = hnoYb;
    Histogram2DSaveNamePath = sPath, Histogram2DSaveName = sName;
    HistogramStatsTitle = hst;

    std::string subtitle;

    if (Histogram2DTitles["Histogram2DTitleReactions"] == "" && Histogram2DTitles["DetectorRegion"] == "") {
        subtitle = Histogram2DTitles["FinalState"];
    } else if (Histogram2DTitles["Histogram2DTitleReactions"] != "" && Histogram2DTitles["DetectorRegion"] == "") {
        subtitle = Histogram2DTitles["Histogram2DTitleReactions"] + ", " + Histogram2DTitles["FinalState"];
    } else if (Histogram2DTitles["Histogram2DTitleReactions"] == "" && Histogram2DTitles["DetectorRegion"] != "") {
        subtitle = Histogram2DTitles["FinalState"] + ", " + Histogram2DTitles["DetectorRegion"];
    } else {
        subtitle = Histogram2DTitles["Histogram2DTitleReactions"] + ", " + Histogram2DTitles["FinalState"] + ", " + Histogram2DTitles["DetectorRegion"];
    }

    Histogram2D = new TH2D((Histogram2DTitles["HistogramStatTitle"] + " (" + subtitle + ")").c_str(),
                           (Histogram2DTitles["HistogramTitle"] + " (" + subtitle + ")" + ";" + Histogram2DTitles["XaxisTitle"] + ";" + Histogram2DTitles["YaxisTitle"]).c_str(),
                           HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1), HistogramNumberOfYBins, HistogramYAxisLimits.at(0), HistogramYAxisLimits.at(1));
}

hPlot2D::hPlot2D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string yat, double LowerXlim, double UpperXlim, double LowerYlim,
                 double UpperYlim, int hnoXb = 150, int hnoYb = 150) {
    Histogram2DTitles["FinalState"] = fState, Histogram2DTitles["DetectorRegion"] = dRegion;
    Histogram2DTitles["HistogramStatTitle"] = hst, Histogram2DTitles["HistogramTitle"] = ht, Histogram2DTitles["XaxisTitle"] = xat, Histogram2DTitles["YaxisTitle"] = yat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramYAxisLimits.push_back(LowerYlim), HistogramYAxisLimits.push_back(UpperYlim);
    HistogramNumberOfXBins = hnoXb, HistogramNumberOfYBins = hnoYb;
    HistogramStatsTitle = hst;

    Histogram2D = new TH2D((Histogram2DTitles["HistogramStatTitle"] + " (" + Histogram2DTitles["FinalState"] + ", " + Histogram2DTitles["DetectorRegion"] + ")").c_str(),
                           (Histogram2DTitles["HistogramTitle"] + " (" + Histogram2DTitles["FinalState"] + ", " + Histogram2DTitles["DetectorRegion"] + ")" + ";" +
                            Histogram2DTitles["XaxisTitle"] + ";" + Histogram2DTitles["YaxisTitle"])
                               .c_str(),
                           HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1), HistogramNumberOfYBins, HistogramYAxisLimits.at(0), HistogramYAxisLimits.at(1));
}

hPlot2D::hPlot2D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string yat, std::string sPath, std::string sName, double LowerXlim,
                 double UpperXlim, double LowerYlim, double UpperYlim, int hnoXb = 150, int hnoYb = 150) {
    Histogram2DTitles["FinalState"] = fState, Histogram2DTitles["DetectorRegion"] = dRegion;
    Histogram2DTitles["HistogramStatTitle"] = hst, Histogram2DTitles["HistogramTitle"] = ht, Histogram2DTitles["XaxisTitle"] = xat, Histogram2DTitles["YaxisTitle"] = yat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramYAxisLimits.push_back(LowerYlim), HistogramYAxisLimits.push_back(UpperYlim);
    HistogramNumberOfXBins = hnoXb, HistogramNumberOfYBins = hnoYb;
    Histogram2DSaveNamePath = sPath;
    Histogram2DSaveName = sName;
    HistogramStatsTitle = hst;

    if (fState != "") {
        if (Histogram2DTitles["DetectorRegion"] == "") {
            Histogram2D = new TH2D(
                (Histogram2DTitles["HistogramStatTitle"] + " (" + Histogram2DTitles["FinalState"] + ")").c_str(),
                (Histogram2DTitles["HistogramTitle"] + " (" + Histogram2DTitles["FinalState"] + ")" + ";" + Histogram2DTitles["XaxisTitle"] + ";" + Histogram2DTitles["YaxisTitle"]).c_str(),
                HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1), HistogramNumberOfYBins, HistogramYAxisLimits.at(0), HistogramYAxisLimits.at(1));
        } else {
            Histogram2D =
                new TH2D((Histogram2DTitles["HistogramStatTitle"] + " (" + Histogram2DTitles["FinalState"] + ", " + Histogram2DTitles["DetectorRegion"] + ")").c_str(),
                         (Histogram2DTitles["HistogramTitle"] + " (" + Histogram2DTitles["FinalState"] + ", " + Histogram2DTitles["DetectorRegion"] + ")" + ";" +
                          Histogram2DTitles["XaxisTitle"] + ";" + Histogram2DTitles["YaxisTitle"])
                             .c_str(),
                         HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1), HistogramNumberOfYBins, HistogramYAxisLimits.at(0), HistogramYAxisLimits.at(1));
        }
    } else {
        if (Histogram2DTitles["DetectorRegion"] == "") {
            Histogram2D = new TH2D((Histogram2DTitles["HistogramStatTitle"]).c_str(),
                                   (Histogram2DTitles["HistogramTitle"] + ";" + Histogram2DTitles["XaxisTitle"] + ";" + Histogram2DTitles["YaxisTitle"]).c_str(), HistogramNumberOfXBins,
                                   HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1), HistogramNumberOfYBins, HistogramYAxisLimits.at(0), HistogramYAxisLimits.at(1));
        } else {
            Histogram2D = new TH2D(
                (Histogram2DTitles["HistogramStatTitle"] + " (" + Histogram2DTitles["DetectorRegion"] + ")").c_str(),
                (Histogram2DTitles["HistogramTitle"] + " (" + Histogram2DTitles["DetectorRegion"] + ")" + ";" + Histogram2DTitles["XaxisTitle"] + ";" + Histogram2DTitles["YaxisTitle"])
                    .c_str(),
                HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1), HistogramNumberOfYBins, HistogramYAxisLimits.at(0), HistogramYAxisLimits.at(1));
        }
    }
}

hPlot2D::hPlot2D(std::string hst, std::string ht, std::string xat, std::string yat, double LowerXlim, double UpperXlim, double LowerYlim, double UpperYlim, int hnoXb = 150,
                 int hnoYb = 150) {
    Histogram2DTitles["HistogramStatTitle"] = hst, Histogram2DTitles["HistogramTitle"] = ht, Histogram2DTitles["XaxisTitle"] = xat, Histogram2DTitles["YaxisTitle"] = yat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramYAxisLimits.push_back(LowerYlim), HistogramYAxisLimits.push_back(UpperYlim);
    HistogramNumberOfXBins = hnoXb, HistogramNumberOfYBins = hnoYb;
    HistogramStatsTitle = hst;

    Histogram2D = new TH2D((Histogram2DTitles["HistogramStatTitle"]).c_str(),
                           (Histogram2DTitles["HistogramTitle"] + ";" + Histogram2DTitles["XaxisTitle"] + ";" + Histogram2DTitles["YaxisTitle"]).c_str(), HistogramNumberOfXBins,
                           HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1), HistogramNumberOfYBins, HistogramYAxisLimits.at(0), HistogramYAxisLimits.at(1));
}

// histPlotter2D function -----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (regular) */

// histPlotter2D function (regular) -------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (regular) */
void hPlot2D::histPlotter2D(const std::string &SampleName, TCanvas *HistogramCanvas, TH2D *Histogram2D, TList *Histogram_list, vector<TObject *> &HistoList,
                            std::string Histogram2DSaveNameDir, std::string Histogram2DSaveName, bool showStats = true) {
    HistogramCanvas->cd();

    gStyle->SetOptStat("ourmen");

    std::string sNameFlag;

    if (basic_tools::FindSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (basic_tools::FindSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    //    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;
    //    double diplayTextSize = 0.1;

    Histogram2D->SetTitleSize(Histogram2DTitleSizes.at(0), "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(Histogram2DTitleSizes.at(1));
    Histogram2D->GetXaxis()->CenterTitle(CenterTitle);
    Histogram2D->GetYaxis()->SetLabelSize(Histogram2DTitleSizes.at(2));
    Histogram2D->GetYaxis()->CenterTitle(CenterTitle);
    Histogram2D->GetZaxis()->SetLabelSize(Histogram2DTitleSizes.at(3));
    Histogram_list->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        Histogram2D->SetStats(0);
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->AddText("Empty histogram");
        displayText->SetTextAlign(22);
        Histogram2D->Draw();
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        Histogram2D->Draw("colz");
    }

    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.875);

    if (!showStats) { Histogram2D->SetStats(0); }

    if (zLogScalePlot) {
        HistogramCanvas->SetLogz(1);
        system(("mkdir -p " + Histogram2DSaveNameDir + "/z_Log_Scale_plots").c_str());
        HistogramCanvas->SaveAs((Histogram2DSaveNameDir + "/z_Log_Scale_plots/" + sNameFlag + Histogram2DSaveName + "_zLogScale.pdf").c_str());
        //        HistogramCanvas->SetLogz(1);
        //        HistogramCanvas->SaveAs((Histogram2DSaveNameDir + sNameFlag + Histogram2DSaveName + "_zLogScale.pdf").c_str());

        HistoList.push_back(Histogram2D);
    }

    if (ZLinearScalePlot) {
        HistogramCanvas->SetLogz(0);
        //        system(("mkdir -p " + Histogram2DSaveNameDir + "/z_Linear_Scale_plots").c_str());
        //        HistogramCanvas->SaveAs((Histogram2DSaveNameDir + "/z_Linear_Scale_plots/" + sNameFlag + Histogram2DSaveName + "_zLinearScale.pdf").c_str());
        //        HistogramCanvas->SetLogz(0);
        HistogramCanvas->SaveAs((Histogram2DSaveNameDir + sNameFlag + Histogram2DSaveName + "_zLinearScale.pdf").c_str());

        HistoList.push_back(Histogram2D);
    }

    // gStyle->SetStatX(DefStatX);
    // gStyle->SetStatY(DefStatY);

    HistogramCanvas->Clear();  //    if (zLogScalePlot) { HistogramCanvas->SetLogz(1); }
                               //
                               //    if (!ShowStats) { Histogram2D->SetStats(0); }
                               //
                               //    gStyle->SetStatX(0.87);
                               //    gStyle->SetStatY(0.875);
                               //    HistogramCanvas->SaveAs((Histogram2DSaveNameDir + sNameFlag + Histogram2DSaveName + ".pdf").c_str());
                               //    // gStyle->SetStatX(DefStatX);
                               //    // gStyle->SetStatY(DefStatY);
                               //    Histogram2D->Write(); HistogramCanvas->Clear();
}
#pragma endregion

#pragma region /* hDrawAndSave function (regular) */
void hPlot2D::hDrawAndSave(const std::string &SampleName, TCanvas *h2DCanvas, TList *hList, vector<TObject *> &HistoList, bool showStats) {
    histPlotter2D(SampleName, h2DCanvas, Histogram2D, hList, HistoList, Histogram2DSaveNamePath, Histogram2DSaveName, showStats);
}
#pragma endregion

#pragma endregion

#pragma region /* histPlotter2D function (Beta vs. P plots) */

// histPlotter2D function (Beta vs. P plots, all particles) -------------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (Beta vs. P plots, all particles) */
void hPlot2D::histPlotter2D(const std::string &SampleName, TCanvas *HistogramCanvas, TH2D *Histogram2D, TList *Histogram_list, vector<TObject *> &HistoList,
                            std::string Histogram2DSaveNameDir, std::string Histogram2DSaveName, TF1 *Beta_function1, TF1 *Beta_function2, TF1 *Beta_function3, TF1 *Beta_function4,
                            TF1 *Beta_function5, TF1 *Beta_function6, TF1 *Beta_function7, TF1 *Beta_function8, TF1 *Beta_function9, bool showStats = true) {
    HistogramCanvas->cd();

    gStyle->SetOptStat("ourmen");

    std::string sNameFlag;

    if (basic_tools::FindSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (basic_tools::FindSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    //    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Histogram2D->SetTitleSize(Histogram2DTitleSizes.at(0), "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(Histogram2DTitleSizes.at(1));
    Histogram2D->GetXaxis()->CenterTitle(CenterTitle);
    Histogram2D->GetYaxis()->SetLabelSize(Histogram2DTitleSizes.at(2));
    Histogram2D->GetYaxis()->CenterTitle(CenterTitle);
    Histogram2D->GetZaxis()->SetLabelSize(Histogram2DTitleSizes.at(3));
    Histogram_list->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->AddText("Empty histogram");
        displayText->SetTextAlign(22);
        Histogram2D->Draw();
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        Histogram2D->Draw("colz");

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

    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.4);

    if (!ShowStats || Histogram2D->Integral() == 0.) { Histogram2D->SetStats(0); }

    if (zLogScalePlot) {
        HistogramCanvas->SetLogz(1);
        HistogramCanvas->SaveAs((Histogram2DSaveNameDir + sNameFlag + Histogram2DSaveName + "_zLogScale.pdf").c_str());

        HistoList.push_back(Histogram2D);
    }

    if (ZLinearScalePlot) {
        HistogramCanvas->SetLogz(0);
        system(("mkdir -p " + Histogram2DSaveNameDir + "/z_Linear_Scale_plots").c_str());
        HistogramCanvas->SaveAs((Histogram2DSaveNameDir + "/z_Linear_Scale_plots/" + sNameFlag + Histogram2DSaveName + "_zLinearScale.pdf").c_str());

        HistoList.push_back(Histogram2D);
    }

    // gStyle->SetStatX(DefStatX);
    // gStyle->SetStatY(DefStatY);

    HistogramCanvas->Clear();
}
#pragma endregion

#pragma region /* hDrawAndSave function (Beta vs. P plots, all particles) */
void hPlot2D::hDrawAndSave(const std::string &SampleName, TCanvas *h2DCanvas, TList *hList, vector<TObject *> &HistoList, TF1 *Beta_function1, TF1 *Beta_function2, TF1 *Beta_function3,
                           TF1 *Beta_function4, TF1 *Beta_function5, TF1 *Beta_function6, TF1 *Beta_function7, TF1 *Beta_function8, TF1 *Beta_function9, bool showStats) {
    histPlotter2D(SampleName, h2DCanvas, Histogram2D, hList, HistoList, Histogram2DSaveNamePath, Histogram2DSaveName, Beta_function1, Beta_function2, Beta_function3, Beta_function4,
                  Beta_function5, Beta_function6, Beta_function7, Beta_function8, Beta_function9, showStats);
}
#pragma endregion

// histPlotter2D function (Beta vs. P plots, single particle) -----------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (Beta vs. P plots, single particle) */
void hPlot2D::histPlotter2D(const std::string &SampleName, TCanvas *HistogramCanvas, TH2D *Histogram2D, TList *Histogram_list, vector<TObject *> &HistoList,
                            std::string Histogram2DSaveNameDir, std::string Histogram2DSaveName, TF1 *Beta_function1, std::string particle1, bool showStats = true, bool plot_legend = true) {
    HistogramCanvas->cd();

    gStyle->SetOptStat("ourmen");

    std::string sNameFlag;

    if (basic_tools::FindSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (basic_tools::FindSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    //    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Histogram2D->SetTitleSize(Histogram2DTitleSizes.at(0), "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(Histogram2DTitleSizes.at(1));
    Histogram2D->GetXaxis()->CenterTitle(CenterTitle);
    Histogram2D->GetYaxis()->SetLabelSize(Histogram2DTitleSizes.at(2));
    Histogram2D->GetYaxis()->CenterTitle(CenterTitle);
    Histogram2D->GetZaxis()->SetLabelSize(Histogram2DTitleSizes.at(3));
    Histogram_list->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->AddText("Empty histogram");
        displayText->SetTextAlign(22);
        Histogram2D->Draw();
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        Histogram2D->Draw("colz");

        Beta_function1->SetLineColor(kRed);
        Beta_function1->Draw("same");
    }

    auto Beta_vs_P_legend = new TLegend(0.87, 0.725 - 0.25, 0.87 - 0.2, 0.725 - 0.3);

    if ((plot_legend == true) && (Histogram2D->Integral() != 0.)) {
        TLegendEntry *Beta_function1_entry = Beta_vs_P_legend->AddEntry(Beta_function1, particle1.c_str(), "l");

        Beta_vs_P_legend->Draw("same");
    }

    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.4);

    if (!ShowStats || Histogram2D->Integral() == 0.) { Histogram2D->SetStats(0); }

    if (zLogScalePlot) {
        HistogramCanvas->SetLogz(1);
        HistogramCanvas->SaveAs((Histogram2DSaveNameDir + sNameFlag + Histogram2DSaveName + "_zLogScale.pdf").c_str());

        HistoList.push_back(Histogram2D);
    }

    if (ZLinearScalePlot) {
        HistogramCanvas->SetLogz(0);
        system(("mkdir -p " + Histogram2DSaveNameDir + "/z_Linear_Scale_plots").c_str());
        HistogramCanvas->SaveAs((Histogram2DSaveNameDir + "/z_Linear_Scale_plots/" + sNameFlag + Histogram2DSaveName + "_zLinearScale.pdf").c_str());

        HistoList.push_back(Histogram2D);
    }

    // gStyle->SetStatX(DefStatX);
    // gStyle->SetStatY(DefStatY);

    HistogramCanvas->Clear();
}
#pragma endregion

#pragma region /* hDrawAndSave function (Beta vs. P plots, single particle) */
void hPlot2D::hDrawAndSave(const std::string &SampleName, TCanvas *h2DCanvas, TList *hList, vector<TObject *> &HistoList, TF1 *Beta_function1, std::string particle1, bool showStats,
                           bool plot_legend) {
    histPlotter2D(SampleName, h2DCanvas, Histogram2D, hList, HistoList, Histogram2DSaveNamePath, Histogram2DSaveName, Beta_function1, particle1, showStats, plot_legend);
}
#pragma endregion

// histPlotter2D function (Beta vs. P plots, 3 particles (by charge)) -----------------------------------------------------------------------------------------------------------

#pragma region /* histPlotter2D function (Beta vs. P plots, 3 particles (by charge)) */
void hPlot2D::histPlotter2D(const std::string &SampleName, TCanvas *HistogramCanvas, TH2D *Histogram2D, TList *Histogram_list, vector<TObject *> &HistoList,
                            std::string Histogram2DSaveNameDir, std::string Histogram2DSaveName, TF1 *Beta_function1, std::string particle1, TF1 *Beta_function2, std::string particle2,
                            TF1 *Beta_function3, std::string particle3, bool showStats, bool plot_legend) {
    HistogramCanvas->cd();

    gStyle->SetOptStat("ourmen");

    std::string sNameFlag;

    if (basic_tools::FindSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (basic_tools::FindSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Histogram2D->SetTitleSize(Histogram2DTitleSizes.at(0), "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(Histogram2DTitleSizes.at(1));
    Histogram2D->GetXaxis()->CenterTitle(CenterTitle);
    Histogram2D->GetYaxis()->SetLabelSize(Histogram2DTitleSizes.at(2));
    Histogram2D->GetYaxis()->CenterTitle(CenterTitle);
    Histogram2D->GetZaxis()->SetLabelSize(Histogram2DTitleSizes.at(3));
    Histogram_list->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->AddText("Empty histogram");
        displayText->SetTextAlign(22);
        Histogram2D->Draw();
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        Histogram2D->Draw("colz");

        Beta_function1->SetLineColor(kBlue);
        Beta_function1->Draw("same");
        Beta_function2->SetLineColor(kGreen);
        Beta_function2->Draw("same");
        Beta_function3->SetLineColor(kRed);
        Beta_function3->Draw("same");
    }

    //    auto Beta_vs_P_legend = new TLegend(0.87, 0.725 - 0.15, 0.87 - 0.2, 0.725 - 0.3);
    auto Beta_vs_P_legend = new TLegend(0.87, 0.725 - 0.2, 0.87 - 0.2, 0.725 - 0.3);

    if ((plot_legend == true) && (Histogram2D->Integral() != 0.)) {
        TLegendEntry *Beta_function1_entry = Beta_vs_P_legend->AddEntry(Beta_function1, particle1.c_str(), "l");
        TLegendEntry *Beta_function2_entry = Beta_vs_P_legend->AddEntry(Beta_function2, particle2.c_str(), "l");
        TLegendEntry *Beta_function3_entry = Beta_vs_P_legend->AddEntry(Beta_function3, particle3.c_str(), "l");

        Beta_vs_P_legend->Draw("same");
    }

    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.4);

    if (!ShowStats || Histogram2D->Integral() == 0.) { Histogram2D->SetStats(0); }

    if (zLogScalePlot) {
        HistogramCanvas->SetLogz(1);
        HistogramCanvas->SaveAs((Histogram2DSaveNameDir + sNameFlag + Histogram2DSaveName + "_zLogScale.pdf").c_str());

        HistoList.push_back(Histogram2D);
    }

    if (ZLinearScalePlot) {
        HistogramCanvas->SetLogz(0);
        system(("mkdir -p " + Histogram2DSaveNameDir + "/z_Linear_Scale_plots").c_str());
        HistogramCanvas->SaveAs((Histogram2DSaveNameDir + "/z_Linear_Scale_plots/" + sNameFlag + Histogram2DSaveName + "_zLinearScale.pdf").c_str());

        HistoList.push_back(Histogram2D);
    }

    // gStyle->SetStatX(DefStatX);
    // gStyle->SetStatY(DefStatY);

    HistogramCanvas->Clear();
}
#pragma endregion

#pragma region /* hDrawAndSave function (Beta vs. P plots, 3 particles (by charge)) */
void hPlot2D::hDrawAndSave(const std::string &SampleName, TCanvas *h2DCanvas, TList *hList, vector<TObject *> &HistoList, TF1 *Beta_function1, std::string particle1, TF1 *Beta_function2,
                           std::string particle2, TF1 *Beta_function3, std::string particle3, bool showStats, bool plot_legend) {
    histPlotter2D(SampleName, h2DCanvas, Histogram2D, hList, HistoList, Histogram2DSaveNamePath, Histogram2DSaveName, Beta_function1, particle1, Beta_function2, particle2, Beta_function3,
                  particle3, showStats, plot_legend);
}
#pragma endregion

#pragma endregion

// hDivision function -----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* hDivision function */
void hPlot2D::hDivision(TH2D *hDenominator, bool SetLogZLimits, double MaxZLim) {
    Histogram2D->Divide(hDenominator);

    //    if (SetLogZLimits) {
    //        zLogScalePlot = true;
    //    }
}
#pragma endregion

#endif  // HPLOT2D_H