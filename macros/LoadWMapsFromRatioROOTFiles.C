#include <TFile.h>
#include <TH2.h>
#include <TKey.h>
#include <TList.h>
#include <TObject.h>
#include <TSystem.h>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace {

struct SliceData {
    double low = 0.0;
    double high = 0.0;
    TH2D* hist = nullptr;
};

enum class MapsExportMode { AMaps, WMaps };

std::string EnsureTrailingSlash(std::string path) {
    if (!path.empty() && path.back() != '/') { path.push_back('/'); }
    return path;
}

std::string ToLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

std::string Fmt2(double v) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << v;
    return oss.str();
}

bool ParseLowHighFromName(const std::string& name, double& low, double& high) {
    static const std::regex re(R"(_from_(-?\d+(?:\.\d+)?)_to_(-?\d+(?:\.\d+)?))");
    std::smatch m;
    if (!std::regex_search(name, m, re) || m.size() < 3) { return false; }
    low = std::stod(m[1].str());
    high = std::stod(m[2].str());
    return true;
}

bool ParseLowHighFromTitle(const std::string& title, double& low, double& high) {
    static const std::regex re(R"(([-+]?\d+(?:\.\d+)?)#leqP\^\{truth\}_\{[^}]+\}#leq([-+]?\d+(?:\.\d+)?))");
    std::smatch m;
    if (!std::regex_search(title, m, re) || m.size() < 3) { return false; }
    low = std::stod(m[1].str());
    high = std::stod(m[2].str());
    return true;
}

bool IsSliceHistogram(const std::string& name, const std::string& title) {
    return name.find("_for_P_from_") != std::string::npos || name.find("_from_") != std::string::npos || title.find("P^{truth}_{") != std::string::npos;
}

MapsExportMode ParseMapsMode(const std::string& mapsMode) {
    const std::string mode = ToLower(mapsMode);
    if (mode == "amaps" || mode == "amap" || mode == "a") { return MapsExportMode::AMaps; }
    return MapsExportMode::WMaps;
}

void WriteSliceFile(const std::string& path, const SliceData& s, const std::string& xLabel, const std::string& yLabel, double minKeep, MapsExportMode mode) {
    const int xBins = s.hist->GetNbinsX();
    const int yBins = s.hist->GetNbinsY();

    std::ofstream out(path);
    out << "Lower_P_lim:\t" << s.low << "\n";
    out << "Upper_P_lim:\t" << s.high << "\n\n";
    out << xLabel << ":\t" << xBins << "\n";
    out << yLabel << ":\t" << yBins << "\n\n";

    for (int y = 1; y <= yBins; ++y) {
        out << ((mode == MapsExportMode::AMaps) ? "Line\t" : "Weight\t");
        for (int x = 1; x <= xBins; ++x) {
            const double raw = s.hist->GetBinContent(x, y);
            if (mode == MapsExportMode::AMaps) {
                const int a = (raw >= minKeep) ? 1 : 0;
                out << a;
            } else {
                const double w = (raw >= minKeep) ? raw : 0.0;
                out << w;
            }
            if (x != xBins) out << ":";
        }
        out << "\n";
    }
}

void WriteSliceLimits(const std::string& path, const std::string& tag, const std::vector<SliceData>& slices) {
    std::ofstream out(path);
    for (size_t i = 0; i < slices.size(); ++i) { out << tag << "_slice_" << (i + 1) << "\t" << slices[i].low << ":" << slices[i].high << "\n"; }
    out << "\n";
}

void CollectFromObject(TObject* obj, bool fromConstFile, std::vector<SliceData>& eSlices, std::vector<SliceData>& pSlices, std::vector<SliceData>& nSlices,
                       std::vector<SliceData>& nConstSlices, TH2D*& nCombined) {
    if (!obj) return;

    if (obj->InheritsFrom(TList::Class())) {
        TIter nextListObj(static_cast<TList*>(obj));
        while (TObject* item = nextListObj()) { CollectFromObject(item, fromConstFile, eSlices, pSlices, nSlices, nConstSlices, nCombined); }
        return;
    }

    if (!obj->InheritsFrom(TH2::Class())) return;

    TH2D* h = dynamic_cast<TH2D*>(obj);
    if (!h) return;

    TH2D* hc = dynamic_cast<TH2D*>(h->Clone(h->GetName()));
    hc->SetDirectory(nullptr);

    const std::string name = hc->GetName();
    const std::string titleLower = ToLower(hc->GetTitle());
    const std::string nameLower = ToLower(name);

    const bool isElectron = titleLower.find("electron") != std::string::npos || titleLower.find("{e") != std::string::npos || nameLower.find("_e_") != std::string::npos;
    const bool isProton = titleLower.find("proton") != std::string::npos || titleLower.find("{p") != std::string::npos;
    const bool isNeutron = titleLower.find("neutron") != std::string::npos || titleLower.find("{n") != std::string::npos;

    const std::string title = hc->GetTitle();
    const bool isSlice = IsSliceHistogram(name, title);
    double low = 0.0;
    double high = 0.0;
    bool hasLimits = ParseLowHighFromName(name, low, high);
    if (!hasLimits) { hasLimits = ParseLowHighFromTitle(title, low, high); }

    if (isElectron && isSlice && hasLimits) {
        eSlices.push_back({low, high, hc});
    } else if (isProton && isSlice && hasLimits) {
        pSlices.push_back({low, high, hc});
    } else if (isNeutron && isSlice && hasLimits) {
        if (fromConstFile)
            nConstSlices.push_back({low, high, hc});
        else
            nSlices.push_back({low, high, hc});
    } else if (isNeutron && !isSlice) {
        nCombined = hc;
    }
}

void CollectFromFile(const std::string& rootFilePath, bool fromConstFile, std::vector<SliceData>& eSlices, std::vector<SliceData>& pSlices, std::vector<SliceData>& nSlices,
                     std::vector<SliceData>& nConstSlices, TH2D*& nCombined) {
    if (gSystem->AccessPathName(rootFilePath.c_str())) { return; }

    TFile* f = TFile::Open(rootFilePath.c_str(), "READ");
    if (!f) {
        std::cerr << "[Maps macro] cannot open: " << rootFilePath << "\n";
        return;
    }
    if (f->IsZombie()) { std::cerr << "[Maps macro] warning: ROOT recovered a damaged file, trying to read recovered keys: " << rootFilePath << "\n"; }
    if (!f->GetListOfKeys() || f->GetListOfKeys()->GetSize() == 0) {
        std::cerr << "[Maps macro] no readable keys in: " << rootFilePath << "\n";
        f->Close();
        return;
    }

    TIter nextKey(f->GetListOfKeys());
    while (TKey* key = static_cast<TKey*>(nextKey())) {
        TObject* obj = key->ReadObj();
        if (!obj) continue;
        CollectFromObject(obj, fromConstFile, eSlices, pSlices, nSlices, nConstSlices, nCombined);
    }

    f->Close();
}

void BuildRatioSlicesFromRecoAndTL(const std::vector<SliceData>& recoSlices, const std::vector<SliceData>& tlSlices, std::vector<SliceData>& outRatioSlices,
                                   const std::string& label) {
    if (recoSlices.empty() || tlSlices.empty()) return;

    if (recoSlices.size() != tlSlices.size()) {
        std::cerr << "[Maps macro] warning: fallback " << label << " Reco/TL slice count mismatch (" << recoSlices.size() << " vs " << tlSlices.size()
                  << "), using common subset\n";
    }

    const size_t nCommon = std::min(recoSlices.size(), tlSlices.size());
    for (size_t i = 0; i < nCommon; ++i) {
        const SliceData& reco = recoSlices[i];
        const SliceData& tl = tlSlices[i];
        if (!reco.hist || !tl.hist) continue;

        TH2D* ratio = dynamic_cast<TH2D*>(reco.hist->Clone((std::string(reco.hist->GetName()) + "_fallback_ratio").c_str()));
        if (!ratio) continue;
        ratio->SetDirectory(nullptr);

        const int xBins = ratio->GetNbinsX();
        const int yBins = ratio->GetNbinsY();
        for (int y = 1; y <= yBins; ++y) {
            for (int x = 1; x <= xBins; ++x) {
                const double denom = tl.hist->GetBinContent(x, y);
                const double numer = reco.hist->GetBinContent(x, y);
                const double value = (denom > 0.0) ? (numer / denom) : 0.0;
                ratio->SetBinContent(x, y, value);
            }
        }

        outRatioSlices.push_back({reco.low, reco.high, ratio});
    }
}

bool RebuildRatioSlicesFromRecoTL(const std::string& inputDir, const std::string& sampleName, std::vector<SliceData>& eSlices, std::vector<SliceData>& pSlices,
                                  std::vector<SliceData>& nSlices, std::vector<SliceData>& nConstSlices) {
    std::vector<SliceData> recoESlices, recoPSlices, recoNSlices, recoNConstSlices;
    std::vector<SliceData> tlESlices, tlPSlices, tlNSlices, tlNConstSlices;
    TH2D* dummyRecoCombined = nullptr;
    TH2D* dummyTLCombined = nullptr;

    const std::string recoRoot = inputDir + sampleName + "/02_AMap_Reco_-_" + sampleName + ".root";
    const std::string recoConstRoot = inputDir + sampleName + "/02_AMap_Reco_-__const_" + sampleName + ".root";
    const std::string tlRoot = inputDir + sampleName + "/01_AMap_TL_-_" + sampleName + ".root";
    const std::string tlConstRoot = inputDir + sampleName + "/01_AMap_TL_-__const_" + sampleName + ".root";

    CollectFromFile(recoRoot, false, recoESlices, recoPSlices, recoNSlices, recoNConstSlices, dummyRecoCombined);
    CollectFromFile(recoConstRoot, true, recoESlices, recoPSlices, recoNSlices, recoNConstSlices, dummyRecoCombined);
    CollectFromFile(tlRoot, false, tlESlices, tlPSlices, tlNSlices, tlNConstSlices, dummyTLCombined);
    CollectFromFile(tlConstRoot, true, tlESlices, tlPSlices, tlNSlices, tlNConstSlices, dummyTLCombined);

    auto byLow = [](const SliceData& a, const SliceData& b) { return a.low < b.low; };
    std::sort(recoESlices.begin(), recoESlices.end(), byLow);
    std::sort(recoPSlices.begin(), recoPSlices.end(), byLow);
    std::sort(recoNSlices.begin(), recoNSlices.end(), byLow);
    std::sort(recoNConstSlices.begin(), recoNConstSlices.end(), byLow);
    std::sort(tlESlices.begin(), tlESlices.end(), byLow);
    std::sort(tlPSlices.begin(), tlPSlices.end(), byLow);
    std::sort(tlNSlices.begin(), tlNSlices.end(), byLow);
    std::sort(tlNConstSlices.begin(), tlNConstSlices.end(), byLow);

    eSlices.clear();
    pSlices.clear();
    nSlices.clear();
    nConstSlices.clear();
    BuildRatioSlicesFromRecoAndTL(recoESlices, tlESlices, eSlices, "electron");
    BuildRatioSlicesFromRecoAndTL(recoPSlices, tlPSlices, pSlices, "proton");
    BuildRatioSlicesFromRecoAndTL(recoNSlices, tlNSlices, nSlices, "neutron");
    BuildRatioSlicesFromRecoAndTL(recoNConstSlices, tlNConstSlices, nConstSlices, "neutron const");

    return !(eSlices.empty() && pSlices.empty() && nSlices.empty() && nConstSlices.empty());
}

void WriteSlicesToDir(const std::string& baseDir, const std::string& tag, const std::vector<SliceData>& slices, const std::string& xLabel, const std::string& yLabel, double minKeep,
                      MapsExportMode mode) {
    if (slices.empty()) return;
    gSystem->mkdir(baseDir.c_str(), true);

    for (const auto& s : slices) {
        const std::string modeTag = (mode == MapsExportMode::AMaps) ? "AMap" : "WMap";
        const std::string file = tag + "_" + modeTag + "_file_from_" + Fmt2(s.low) + "_to_" + Fmt2(s.high) + ".par";
        WriteSliceFile(baseDir + file, s, xLabel, yLabel, minKeep, mode);
    }

    WriteSliceLimits(baseDir + tag + "_slice_limits.par", tag, slices);
}

std::vector<std::pair<double, double>> ReadSliceLimitsFile(const std::string& path) {
    std::vector<std::pair<double, double>> limits;
    std::ifstream in(path);
    if (!in.is_open()) return limits;

    std::string line;
    while (std::getline(in, line)) {
        if (line.find("_slice_") == std::string::npos) continue;
        const auto tabPos = line.find('\t');
        if (tabPos == std::string::npos) continue;
        const std::string values = line.substr(tabPos + 1);
        const auto colonPos = values.find(':');
        if (colonPos == std::string::npos) continue;
        try {
            const double low = std::stod(values.substr(0, colonPos));
            const double high = std::stod(values.substr(colonPos + 1));
            limits.emplace_back(low, high);
        } catch (...) {}
    }
    return limits;
}

void ApplyLimitsByIndex(std::vector<SliceData>& slices, const std::vector<std::pair<double, double>>& limits) {
    if (slices.empty() || limits.empty() || slices.size() != limits.size()) return;
    for (size_t i = 0; i < slices.size(); ++i) {
        slices[i].low = limits[i].first;
        slices[i].high = limits[i].second;
    }
}

}  // namespace

void LoadMapsFromRatioROOTFiles(const std::string& sampleName, const std::string& inputAcceptanceMapsDirectory = "data/AcceptanceMaps",
                                const std::string& outputAcceptanceMapsDirectory = "data/AcceptanceWeights", const std::string& mapsMode = "WMaps") {
    const std::string inputDir = EnsureTrailingSlash(inputAcceptanceMapsDirectory);
    const std::string outputDir = EnsureTrailingSlash(outputAcceptanceMapsDirectory.empty() ? inputAcceptanceMapsDirectory : outputAcceptanceMapsDirectory);
    const MapsExportMode mode = ParseMapsMode(mapsMode);
    const std::string modeLabel = (mode == MapsExportMode::AMaps) ? "AMaps" : "WMaps";

    const std::string ratioRoot = inputDir + sampleName + "/03_AMap_Ratio_-_" + sampleName + ".root";
    const std::string constRatioRoot = inputDir + sampleName + "/03_AMap_Ratio_-__const_" + sampleName + ".root";

    std::vector<SliceData> eSlices;
    std::vector<SliceData> pSlices;
    std::vector<SliceData> nSlices;
    std::vector<SliceData> nConstSlices;
    TH2D* nCombined = nullptr;
    bool usedFallbackFromRecoTL = false;

    CollectFromFile(ratioRoot, false, eSlices, pSlices, nSlices, nConstSlices, nCombined);
    CollectFromFile(constRatioRoot, true, eSlices, pSlices, nSlices, nConstSlices, nCombined);

    if (eSlices.empty() && pSlices.empty() && nSlices.empty() && nConstSlices.empty()) {
        std::cerr << "[Maps macro] ratio ROOT files are empty/unreadable, rebuilding ratios from Reco/TL ROOT files\n";
        usedFallbackFromRecoTL = RebuildRatioSlicesFromRecoTL(inputDir, sampleName, eSlices, pSlices, nSlices, nConstSlices);
    }

    auto byLow = [](const SliceData& a, const SliceData& b) { return a.low < b.low; };
    std::sort(eSlices.begin(), eSlices.end(), byLow);
    std::sort(pSlices.begin(), pSlices.end(), byLow);
    std::sort(nSlices.begin(), nSlices.end(), byLow);
    std::sort(nConstSlices.begin(), nConstSlices.end(), byLow);

    const auto eLimits = ReadSliceLimitsFile(inputDir + sampleName + "/e_AMaps_by_slice/e_slice_limits.par");
    const auto pLimits = ReadSliceLimitsFile(inputDir + sampleName + "/p_AMaps_by_slice/p_slice_limits.par");
    const auto nLimits = ReadSliceLimitsFile(inputDir + sampleName + "/n_AMaps_by_slice/n_slice_limits.par");
    ApplyLimitsByIndex(eSlices, eLimits);
    ApplyLimitsByIndex(pSlices, pLimits);
    ApplyLimitsByIndex(nSlices, nLimits);
    ApplyLimitsByIndex(nConstSlices, nLimits);

    const std::string sampleOutDir = outputDir + sampleName + "/";
    gSystem->mkdir(sampleOutDir.c_str(), true);

    const std::string modeSuffix = (mode == MapsExportMode::AMaps) ? "AMaps" : "WMap";
    WriteSlicesToDir(sampleOutDir + "e_" + modeSuffix + "_by_slice/", "e", eSlices, "HistElectronSliceNumOfXBins", "HistElectronSliceNumOfYBins", 0.7, mode);
    WriteSlicesToDir(sampleOutDir + "p_" + modeSuffix + "_by_slice/", "p", pSlices, "HistNucSliceNumOfXBins", "HistNucSliceNumOfYBins", 0.7, mode);
    WriteSlicesToDir(sampleOutDir + "n_" + modeSuffix + "_by_slice/", "n", nSlices, "HistNucSliceNumOfXBins", "HistNucSliceNumOfYBins", 0.2, mode);
    WriteSlicesToDir(sampleOutDir + "n_" + modeSuffix + "_by_ConstSlice/", "n", nConstSlices, "HistNucSliceNumOfXBins", "HistNucSliceNumOfYBins", 0.2, mode);

    std::cout << "[Maps macro] mode:   " << modeLabel << "\n";
    std::cout << "[Maps macro] sample: " << sampleName << "\n";
    std::cout << "[Maps macro] input:  " << inputDir << "\n";
    std::cout << "[Maps macro] output: " << sampleOutDir << "\n";
    std::cout << "[Maps macro] source: " << (usedFallbackFromRecoTL ? "Reco/TL fallback" : "Ratio ROOT files") << "\n";
    std::cout << "[Maps macro] e slices: " << eSlices.size() << ", p slices: " << pSlices.size() << ", n slices: " << nSlices.size() << ", n const slices: " << nConstSlices.size() << "\n";
}

void LoadWMapsFromRatioROOTFiles(const std::string& sampleName, const std::string& inputAcceptanceMapsDirectory = "data/AcceptanceMaps",
                                 const std::string& outputAcceptanceMapsDirectory = "data/AcceptanceWeights") {
    LoadMapsFromRatioROOTFiles(sampleName, inputAcceptanceMapsDirectory, outputAcceptanceMapsDirectory, "WMaps");
}

void LoadWMapsFromRatioROOTFiles(const std::string& sampleName, const std::string& inputAcceptanceMapsDirectory,
                                 const std::string& outputAcceptanceMapsDirectory, const std::string& mapsMode) {
    LoadMapsFromRatioROOTFiles(sampleName, inputAcceptanceMapsDirectory, outputAcceptanceMapsDirectory, mapsMode);
}
