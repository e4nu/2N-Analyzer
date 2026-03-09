//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef VARIABLE_CORRECTORS_H
#define VARIABLE_CORRECTORS_H

#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMath.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>  // for std::tuple
#include <vector>

#include "../basic_tools.h"
#include "../constants.h"

// TODO: move into fitters namespace

namespace bt = basic_tools;

/**
 * @namespace variable_correctors
 * @brief A namespace for functions that perform variable corrections, such as fitting peaks to Gaussians and extracting peak positions from histograms.
 * @details This namespace contains functions that are used to perform variable corrections in the analysis, such as fitting peaks to Gaussian functions and extracting peak positions from
 * histograms. The functions include GetPeakFromHighestBin, which returns the peak position of a histogram by locating the bin with the maximum number of entries, and FitPeakToGaussian,
 * which fits a Gaussian function to the peak of a histogram and returns the mean and its error. These functions are designed to be used in the main code for performing variable corrections
 * based on the characteristics of the histograms being analyzed. The functions can be used to extract peak positions and uncertainties, which can then be used for further analysis or for
 * applying corrections to variables based on the extracted peak positions.
 * @note The functions in this namespace are designed to be used in the main code for performing variable corrections based on the characteristics of the histograms being analyzed. It is
 * important to ensure that the functions are used correctly and that the input histograms are appropriate for the corrections being performed. For example, the FitPeakToGaussian function
 * assumes that the histogram has a well-defined peak that can be fitted with a Gaussian function, and that the fit limits are chosen appropriately based on the characteristics of the
 * histogram. Additionally, the GetPeakFromHighestBin function assumes that the histogram has a well-defined peak that can be identified by locating the bin with the maximum number of
 * entries, and that the binning of the histogram is appropriate for the resolution needed for the analysis.
 */
namespace variable_correctors {

// GetPeakFromHighestBin function ----------------------------------------------------------------------------------------------------------------------

/**
 * @brief Returns the peak position of a histogram by locating the bin with the maximum number of entries.
 *
 * This function does not perform a fit — it simply identifies the bin with the highest content,
 * and returns its central x-value and an estimated uncertainty.
 *
 * The uncertainty is estimated as half the bin width at the peak bin, providing a simple measure
 * of positional resolution based on binning.
 *
 * This is useful for a quick, rough estimate of the peak position but is less robust than a Gaussian fit,
 * especially in the presence of statistical fluctuations or background.
 *
 * @param hist Pointer to the TH1D histogram to be analyzed.
 * @return std::pair<double, double> where the first value is the peak position (x of the max bin),
 *         and the second is an estimated uncertainty (half the bin width at the peak).
 *
 * @example
 *   auto [peak, err] = GetPeakFromHighestBin(h1);
 *   std::cout << "Peak bin center: " << peak << " ± " << err << std::endl;
 */
std::pair<double, double> GetPeakFromHighestBin(TH1D* hist) {
    int peakBin = hist->GetMaximumBin();
    return {hist->GetBinCenter(peakBin), 0.5 * hist->GetBinWidth(peakBin)};
}

// FitPeakToGaussian functions ------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Fits a Gaussian function to the peak of a histogram and returns the mean and its error.
 *
 * If fit limits are not provided, they are inferred based on the histogram peak and Ebeam status.
 *
 * @param hist Pointer to the TH1D histogram to be fitted.
 * @param fitLimits Optional vector of two doubles specifying fit range [min, max].
 * @return std::pair<double, double> containing the fitted mean and its uncertainty.
 *
 * @example
 *   auto [mean, error] = fit_peak_gaussian(h1);
 *   std::cout << "Peak at: " << mean << " ± " << error << std::endl;
 */
std::pair<double, double> FitPeakToGaussian(TH1D* hist, std::vector<double> fitLimits = {}) {
    double fitMin, fitMax;

    // Return NaNs if histogram is empty
    if (hist->GetEntries() == 0) {
        std::cerr << "Histogram is empty. Returning NaN." << std::endl;
        return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
    }

    // Use default fit limits based on histogram characteristics if none provided
    if (fitLimits.empty()) {
        // If no limits are provided, use the histogram's peak center
        double peakCenter = hist->GetBinCenter(hist->GetMaximumBin());

        if (bt::FindSubstring(hist->GetName(), "_e_") || bt::FindSubstring(hist->GetName(), "_pipCD_") || bt::FindSubstring(hist->GetName(), "_pimCD_")) {
            if (peakCenter < 0) {
                // If peak is negative, set limits accordingly
                fitMin = -std::fabs(peakCenter * 1.1);
                fitMax = -std::fabs(peakCenter * 0.9);
            } else {
                // If peak is positive, set limits accordingly
                fitMin = std::fabs(peakCenter * 0.9);
                fitMax = std::fabs(peakCenter * 1.1);
            }
        } else {
            fitMin = peakCenter - 0.5;
            fitMax = peakCenter + 0.5;
            // if (peakCenter < 0) {
            //     fitMin = -std::fabs(peakCenter * 1.6);
            //     fitMax = -std::fabs(peakCenter * 0.4);
            // } else {
            //     fitMin = std::fabs(peakCenter * 0.8);
            //     fitMax = std::fabs(peakCenter * 1.2);
            // }
        }
    } else if (fitLimits.size() == 2) {
        // Use provided fit limits
        fitMin = fitLimits[0];
        fitMax = fitLimits[1];
    } else {
        // Return NaNs if fitLimits vector is invalid
        std::cerr << "Error: fitLimits must contain exactly two elements." << std::endl;
        return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
    }

    // Construct and fit a Gaussian
    TF1* fit = new TF1("fit", "gaus", fitMin, fitMax);
    hist->Fit(fit, "RQ");  // R = use range, Q = quiet

    // Set visual color and attach to histogram
    fit->SetLineColor(kViolet);
    hist->GetListOfFunctions()->Clear();
    hist->GetListOfFunctions()->Add(fit);

    // Extract fit results
    double mean = fit->GetParameter(1);
    double error = fit->GetParError(1);
    // auto result = std::make_pair(error, mean);
    auto result = std::make_pair(mean, error);

    return result;
}

// FitPeakToGaussian functions ------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Fits the average reconstructed vertex positions from 6 CLAS12 sectors
 *        to a sinusoidal function to extract the beam offset direction and magnitude.
 *
 * This version takes lists of TH1D* histograms for Zrec and phi, fits their peaks,
 * and uses the extracted means and errors to construct the TGraphErrors.
 *
 * @param Zrec_BySector_HistList   Vector of TH1D* histograms for Zrec per sector (size must be 6).
 * @param Phi_BySector_HistList    Vector of TH1D* histograms for phi per sector (size must be 6).
 * @param theta_slice              Optional pair of theta limits in degrees (first < second). Used to fit with A = r / tan(theta).
 * @return std::tuple<double, double, double, TGraphErrors*> with (A or r, φ_beam, Vz_true, graph with fit and legend).
 */
std::tuple<double, double, double, TGraphErrors*> FitVertexVsPhi(std::string Particle, std::string SampleName, TObject* h_Vz_VS_phi_AllSectors,
                                                                 const std::vector<TH1D*>& Zrec_BySector_HistList, const std::vector<TH1D*>& Phi_BySector_HistList = {},
                                                                 const std::pair<double, double>& theta_slice = {-1, -1}) {
    if (Zrec_BySector_HistList.size() != 6) { throw std::runtime_error("FitVertexVsPhi: expected 6 sector histograms (Zrec_BySector_HistList.size() != 6)"); }
    if (Phi_BySector_HistList.size() != 6) { throw std::runtime_error("FitVertexVsPhi: expected 6 sector histograms (Phi_BySector_HistList.size() != 6)"); }

    std::string sector_label[6] = {"Sector 1", "Sector 2", "Sector 3", "Sector 4", "Sector 5", "Sector 6"};

    double phi_deg[6], z_vals[6], phi_err[6], z_err[6];

    for (int i = 0; i < 6; ++i) {
        auto [z_mean, z_error] = FitPeakToGaussian(Zrec_BySector_HistList[i]);
        z_vals[i] = z_mean;
        z_err[i] = z_error;
    }

    for (int i = 0; i < 6; ++i) {
        auto [phi_mean, phi_error] = GetPeakFromHighestBin(Phi_BySector_HistList[i]);
        phi_deg[i] = phi_mean;
        phi_err[i] = phi_error;
    }

    double minZ = *std::min_element(z_vals, z_vals + 6), maxZ = *std::max_element(z_vals, z_vals + 6);

    bool useThetaSlice = (theta_slice.first > 0.0 && theta_slice.second > 0.0 && theta_slice.second > theta_slice.first);

    double mean_theta = useThetaSlice ? 0.5 * (theta_slice.first + theta_slice.second) : 0.0;  // mean in degrees
    double mean_theta_rad = mean_theta * TMath::DegToRad();                                    // mean in radians
    // double mean_theta = 0.0;
    // double mean_theta_rad = 0.0;
    // if (useThetaSlice) {
    //     mean_theta = 0.5 * (theta_slice.first + theta_slice.second);  // mean in degrees
    //     mean_theta_rad = mean_theta * TMath::DegToRad();
    // }

    TGraphErrors* g = new TGraphErrors(6, phi_deg, z_vals, phi_err, z_err);

    if (useThetaSlice) {
        g->SetTitle(("V_{z,rec}^{" + Particle + "} peaks vs. #phi_{" + Particle + "} peaks for " + bt::ToStringWithPrecision(theta_slice.first) + "#circ #leq #theta_{" + Particle +
                     "} #leq " + bt::ToStringWithPrecision(theta_slice.second) + "#circ;#phi_{" + Particle + "} [#circ];V_{z,rec}^{" + Particle + "} peaks [cm]")
                        .c_str());
    } else {
        g->SetTitle(("V_{z,rec}^{" + Particle + "} peaks vs. #phi_{" + Particle + "} peaks;#phi_{" + Particle + "} peaks [#circ];V_{z,rec}^{" + Particle + "} peaks [cm]").c_str());
    }

    g->SetMarkerStyle(21);
    g->SetMarkerSize(1.5);
    g->GetXaxis()->CenterTitle();
    g->GetYaxis()->CenterTitle();

    g->GetXaxis()->SetLimits(-180, 180);

    // double margin = 0.5 * std::abs(maxZ - minZ);
    double margin = 0.1 * std::abs(maxZ - minZ);
    g->GetYaxis()->SetRangeUser(minZ - margin, maxZ + 10 * margin);
    // g->GetYaxis()->SetRangeUser(minZ - margin, maxZ + margin);

    TF1* fitFunc = nullptr;  // Fit function: argument in degrees, convert to radians in the formula

    if (!useThetaSlice) {
        fitFunc = new TF1("fitFunc", "(-[0]) * cos((x - [1]) * TMath::DegToRad()) + [2]", -180, 180);
        fitFunc->SetParNames("Amplitude", "Phi_beam", "Vz_true");
    } else {
        // mean_theta is provided in degrees; convert to radians for trigonometric use
        fitFunc = new TF1("fitFunc", ([=](double* x, double* par) { return (-par[0] / tan(mean_theta_rad)) * cos((x[0] - par[1]) * TMath::DegToRad()) + par[2]; }), -180, 180, 3);
        fitFunc->SetParNames("r", "Phi_beam", "Vz_true");
    }

    double ampGuess = 0.5 * (maxZ - minZ), meanGuess = 0.5 * (maxZ + minZ);
    fitFunc->SetParameters(ampGuess, 0.0, meanGuess);

    fitFunc->SetParLimits(0, 0, 9999);  // Set amplitude (or r) limits to be non-negative

    // double minPhi_beam = 30;
    // double maxPhi_beam = 45;
    // fitFunc->SetParLimits(1, minPhi_beam, maxPhi_beam);

    // double minVz_true = -5.6;
    // double maxVz_true = -5.4;
    // fitFunc->SetParLimits(2, minVz_true, maxVz_true);

    g->Fit(fitFunc);
    // g->Fit(fitFunc, "F");

    // Retrieve fit parameters
    double A = fitFunc->GetParameter(0);
    double phi_beam = fitFunc->GetParameter(1);
    double Vz_true = fitFunc->GetParameter(2);

    // Retrieve parameter errors before using them below
    double err_A = fitFunc->GetParError(0);
    double err_phi_beam = fitFunc->GetParError(1);
    double err_Vz_true = fitFunc->GetParError(2);

    // convert back from r to amplitude and propagate error
    if (useThetaSlice) {
        A = A / tan(mean_theta_rad);
        err_A = err_A / tan(mean_theta_rad);  // propagate error from r to A
    }

    // Converts phi_beam into range [-180^\circ, 180^\circ]
    phi_beam = fmod(phi_beam, 360.0);
    if (phi_beam < -180.0) {
        phi_beam += 360.0;
    } else if (phi_beam > 180.0) {
        phi_beam -= 360.0;
    }

    std::ostringstream legendText;

    if (useThetaSlice) {
        legendText << "V_{z,rec}^{" << Particle << "}(#phi_{" << Particle << "}) = V_{z,true}^{" << Particle << "} - #left[#frac{r #times cos(#phi_{" << Particle
                   << "} - #phi_{beam})}{tan(#theta_{" << Particle << "}^{average})}#right]_{#theta_{" << Particle << "}^{average} = " << bt::ToStringWithPrecision(mean_theta) << "#circ}";
    } else {
        legendText << "V_{z,rec}^{" << Particle << "}(#phi_{" << Particle << "}) = V_{z,true}^{" << Particle << "} - A*cos(#phi_{" << Particle << "} - #phi_{beam})";
    }

    TLegend* legend = new TLegend(0.18, 0.77, 0.74, 0.88);
    legend->AddEntry(fitFunc, legendText.str().c_str(), "l");
    legend->SetTextAlign(12);
    legend->SetTextSize(0.025);
    g->GetListOfFunctions()->Add(legend);

    TPaveText* FitParam1 = new TPaveText(0.18, 0.65, 0.35, 0.75, "NDC");
    TPaveText* FitParam2 = new TPaveText(0.35, 0.65, 0.75, 0.75, "NDC");

    for (auto* box : {FitParam1, FitParam2}) {
        box->SetBorderSize(1);
        box->SetFillColor(0);
        box->SetTextAlign(12);
        box->SetTextFont(42);
        box->SetTextSize(0.025);
    }

    double chi2_ndf = fitFunc->GetChisquare() / fitFunc->GetNDF();
    FitParam1->AddText(("#chi^{2}/NDF = " + bt::ToStringWithPrecision(chi2_ndf, 3)).c_str());

    if (useThetaSlice) {
        FitParam1->AddText(("r = " + bt::ToStringWithPrecision(A) + " #pm " + bt::ToStringWithPrecision(err_A) + " cm").c_str());
    } else {
        FitParam1->AddText(("A = " + bt::ToStringWithPrecision(A) + " #pm " + bt::ToStringWithPrecision(err_A) + " cm").c_str());
    }

    FitParam2->AddText(
        ("#phi_{beam} = " + bt::ToStringWithPrecision(phi_beam) + " #pm " + bt::ToStringWithPrecision(err_phi_beam) + "#circ (#phi_{beam} #in [-180#circ, 180#circ])").c_str());
    FitParam2->AddText(("V_{z,true}^{" + Particle + "} = " + bt::ToStringWithPrecision(Vz_true) + " #pm " + bt::ToStringWithPrecision(err_Vz_true) + " cm").c_str());

    g->GetListOfFunctions()->Add(fitFunc);
    g->GetListOfFunctions()->Add(FitParam1);
    g->GetListOfFunctions()->Add(FitParam2);

    std::cout << std::endl;

    for (int i = 0; i < 6; ++i) {
        double x = phi_deg[i];
        double y = z_vals[i];
        std::ostringstream label;
        label << "#splitline{#font[62]{" << sector_label[i] << "}}{(" << bt::ToStringWithPrecision(x) << "#circ, " << bt::ToStringWithPrecision(y) << " cm)}";
        std::cout << label.str() << std::endl;
        TLatex* latex = new TLatex(x + 3, y + 0.075, label.str().c_str());  // offset to avoid overlap
        latex->SetTextFont(42);
        latex->SetTextSize(0.025);
        latex->SetTextAlign(12);
        g->GetListOfFunctions()->Add(latex);
    }

    std::cout << std::endl;

    if (h_Vz_VS_phi_AllSectors->InheritsFrom("TH2D")) {
        auto* Temo_h2 = (TH2D*)h_Vz_VS_phi_AllSectors;

        Temo_h2->GetListOfFunctions()->Add(g);
        Temo_h2->GetListOfFunctions()->Add(fitFunc);
        Temo_h2->GetListOfFunctions()->Add(legend);
        Temo_h2->GetListOfFunctions()->Add(FitParam1);
        Temo_h2->GetListOfFunctions()->Add(FitParam2);
    }

    return std::make_tuple(A, phi_beam, Vz_true, g);
}
};  // namespace variable_correctors

#endif  // VARIABLE_CORRECTORS_H
