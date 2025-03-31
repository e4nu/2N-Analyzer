//
// Created by Alon Sportes on 13/03/2025.
//

#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <typeinfo>
#include <utility>
#include <vector>

#include "../basic_tools/basic_tools.h"
// #include "../utilities/utilities.h"

// using namespace std;

// TODO: split into header and source files!

namespace data_processor {
// using namespace basic_tools;

// GetFS function -------------------------------------------------------------------------------------------------------------------------------------------------------

std::string GetFS(const std::string &Source) {
    static const std::vector<std::pair<std::string, std::string>> patterns = {{"1e_cut", "1e cut"}, {"1e cut", "1e cut"}, {"1e Cut", "1e cut"}, {"(e,e')", "1e cut"}, {"1pFD", "1pFD"},
                                                                              {"1p", "1p"},         {"1nFD", "1n"},       {"1LnFD", "1LnFD"},   {"1n", "1n"},         {"1n1p", "1n1p"},
                                                                              {"1e2p", "1e2p"},     {"2p", "2p"},         {"pFDpCD", "pFDpCD"}, {"nFDpCD", "nFDpCD"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, pattern.first)) { return pattern.second; }
    }

    return "";
}

// GetParticleName function ---------------------------------------------------------------------------------------------------------------------------------------------

std::string GetParticleName(const std::string &Source, const bool &PluralParticles = false) {
    std::string ParticleName, FS = GetFS(Source);

    auto setParticleName = [&](const std::string &singular, const std::string &plural) { ParticleName = PluralParticles ? plural : singular; };

    static const std::vector<std::tuple<std::string, std::string, std::string>> patterns = {{"neutrals|Neutrals|neut.|Neut.", "Neut", "Neut"},
                                                                                            {"Electron|electron", "Electron", "Electrons"},
                                                                                            {"Proton|proton", "Proton", "Protons"},
                                                                                            {"Neutron|neutron", "Neutron", "Neutrons"},
                                                                                            {"#pi^{+}", "Piplus", "Piplus"},
                                                                                            {"#pi^{-}", "Piminus", "Piminus"},
                                                                                            {"K^{+}", "Kplus", "Kplus"},
                                                                                            {"K^{-}", "Kminus", "Kminus"},
                                                                                            {"D", "Deuteron", "Deuterons"},
                                                                                            {"#gamma|photon|Photon", "Photon", "Photons"},
                                                                                            {"nFDpCD|pFDpCD", "Nucleon", "Nucleon"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, std::get<0>(pattern))) {
            setParticleName(std::get<1>(pattern), std::get<2>(pattern));
            return ParticleName;
        }
    }

    ParticleName = "Unknown";
    return ParticleName;
}

// GetParticleName1 function --------------------------------------------------------------------------------------------------------------------------------------------

std::string GetParticleName1(const std::string &Source, const bool &PluralParticles) {
    static const std::vector<std::tuple<std::string, std::string, std::string>> patterns = {{"neutrals|Neutrals|neut.|Neut.", "Neut", "Neut"},
                                                                                            {"Electron|electron", "Electron", "Electrons"},
                                                                                            {"Proton|proton", "Proton", "Protons"},
                                                                                            {"Neutron|neutron", "Neutron", "Neutrons"},
                                                                                            {"#pi^{+}", "Piplus", "Piplus"},
                                                                                            {"#pi^{-}", "Piminus", "Piminus"},
                                                                                            {"K^{+}", "Kplus", "Kplus"},
                                                                                            {"K^{-}", "Kminus", "Kminus"},
                                                                                            {"D", "Deuteron", "Deuterons"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, std::get<0>(pattern))) { return PluralParticles ? std::get<2>(pattern) : std::get<1>(pattern); }
    }

    return "Deuteron";
}

// GetParticleNameFromSubscript function --------------------------------------------------------------------------------------------------------------------------------

std::string GetParticleNameFromSubscript(const std::string &Source, const bool &PluralParticles) {
    static const std::vector<std::tuple<std::string, std::string, std::string>> patterns = {
        {"{e}", "Electron", "Electrons"},           {"{p}|{pFD}|{pCD}", "Proton", "Protons"},   {"{n}|{nFD}", "Neutron", "Neutrons"},
        {"{#pi^{+}}", "#pi^{+}", "Positive pions"}, {"{#pi^{-}}", "#pi^{-}", "Negative pions"}, {"{K^{+}}", "K^{+}", "Positive kaons"},
        {"{K^{-}}", "K^{-}", "Negative kaons"},     {"{D}", "Deuteron", "Deuterons"},           {"{#gamma}", "Photon", "Photons"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, std::get<0>(pattern))) { return PluralParticles ? std::get<2>(pattern) : std::get<1>(pattern); }
    }

    return "Unknown";
}

// GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

std::string GetParticleNameLC(const std::string &Source, const bool &PluralParticles = false) {
    static const std::vector<std::tuple<std::string, std::string, std::string>> patterns = {{"neutrals|Neutrals|neut.|Neut.", "neut", "neut"},
                                                                                            {"Electron|electron", "electron", "electrons"},
                                                                                            {"Proton|proton", "proton", "protons"},
                                                                                            {"Neutron|neutron", "neutron", "neutrons"},
                                                                                            {"#pi^{+}", "piplus", "piplus"},
                                                                                            {"#pi^{-}", "piminus", "piminus"},
                                                                                            {"K^{+}", "Kplus", "Kplus"},
                                                                                            {"K^{-}", "Kminus", "Kminus"},
                                                                                            {"deuteron", "deuteron", "deuterons"},
                                                                                            {"#gamma|photon|Photon", "photon", "photons"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, std::get<0>(pattern))) { return PluralParticles ? std::get<2>(pattern) : std::get<1>(pattern); }
    }

    return "unknown";
}

// GetParticleNameLCFromSubscript function ------------------------------------------------------------------------------------------------------------------------------

std::string GetParticleNameLCFromSubscript(const std::string &Source, const bool &PluralParticles) {
    static const std::vector<std::tuple<std::string, std::string, std::string>> patterns = {
        {"{e}", "electron", "electrons"},           {"{p}|{pFD}|{pCD}", "proton", "protons"},   {"{n}|{nFD}", "neutron", "neutrons"},
        {"{#pi^{+}}", "#pi^{+}", "positive pions"}, {"{#pi^{-}}", "#pi^{-}", "negative pions"}, {"{K^{+}}", "K^{+}", "positive kaons"},
        {"{K^{-}}", "K^{-}", "negative kaons"},     {"{D}", "deuteron", "deuterons"},           {"{#gamma}", "Photon", "Photons"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, std::get<0>(pattern))) { return PluralParticles ? std::get<2>(pattern) : std::get<1>(pattern); }
    }

    return "Unknown";
}

// GetParticleNameShort function ----------------------------------------------------------------------------------------------------------------------------------------

std::string GetParticleNameShort(const std::string &Source) {
    static const std::vector<std::pair<std::string, std::string>> patterns = {{"neutrals|Neutrals|neut.|Neut.", "neut"},
                                                                              {"Electron|electron", "e"},
                                                                              {"Proton|proton", "p"},
                                                                              {"Neutron|neutron", "n"},
                                                                              {"#pi^{+}", "#pi^{+}"},
                                                                              {"#pi^{-}", "#pi^{-}"},
                                                                              {"K^{+}", "K^{+}"},
                                                                              {"K^{-}", "K^{-}"},
                                                                              {"D", "D"},
                                                                              {"#gamma|photon|Photon", "Photon"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, pattern.first)) { return pattern.second; }
    }

    return "";
}

// GetParticleNameShortFromSubscript function ---------------------------------------------------------------------------------------------------------------------------

std::string GetParticleNameShortFromSubscript(const std::string &Source) {
    static const std::vector<std::pair<std::string, std::string>> patterns = {{"{e}", "e"},         {"{p}", "p"},         {"{n}", "n"}, {"{#pi^{+}}", "#pi^{+}"}, {"{#pi^{-}}", "#pi^{-}"},
                                                                              {"{K^{+}}", "K^{+}"}, {"{K^{-}}", "K^{-}"}, {"{D}", "D"}, {"{#gamma}", "#gamma"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, pattern.first)) { return pattern.second; }
    }

    return "";
}

// GetTopology function -------------------------------------------------------------------------------------------------------------------------------------------------

std::string GetTopology(const std::string &Source) {
    static const std::vector<std::pair<std::string, std::string>> patterns = {{"1n", "1N"},   {"1p", "1N"}, {"1nFD", "1N"},   {"1pFD", "1N"},
                                                                              {"1n1p", "2N"}, {"2p", "2N"}, {"nFDpCD", "2N"}, {"pFDpCD", "2N"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, pattern.first)) { return pattern.second; }
    }

    return "";
}

// GetType function -----------------------------------------------------------------------------------------------------------------------------------------------------

std::string GetType(const std::string &Source) {
    static const std::vector<std::pair<std::string, std::string>> patterns = {{"momentum", "momentum"},
                                                                              {"Leading momentum", "leading_nuc_momentum"},
                                                                              {"Recoil momentum", "recoil_nuc_momentum"},
                                                                              {"Total 3-momentum", "total_3momentum"},
                                                                              {"Relative 3-momentum", "relative_3momentum"},
                                                                              {"Total 4-momentum", "total_4momentum"},
                                                                              {"Relative 4-momentum", "relative_4momentum"},
                                                                              {"#vec{P}_{tot}-#vec{q}", "P_tot_minus_q"},
                                                                              {"W ", "W"},
                                                                              {"Q^{2}", "Q2"},
                                                                              {"E_{e}", "E_e"},
                                                                              {"#omega", "omega"},
                                                                              {"E_{cal}", "Ecal"},
                                                                              {"#deltaP_{T,tot}", "deltaP_T_tot"},
                                                                              {"#deltaP_{T,L}", "deltaP_T_L"},
                                                                              {"#delta#alpha_{T,tot}", "deltaAlpha_T_tot"},
                                                                              {"#delta#alpha_{T,L}", "deltaAlpha_T_L"},
                                                                              {"#delta#phi_{T,tot}", "deltaPhi_T_tot"},
                                                                              {"#delta#phi_{T,L}", "deltaPhi_T_L"},
                                                                              {"#theta", "theta"},
                                                                              {"#phi", "phi"},
                                                                              {"#theta_{tot}", "theta_tot"},
                                                                              {"#phi_{tot}", "phi_tot"},
                                                                              {"#theta_{rel}", "theta_rel"},
                                                                              {"#phi_{rel}", "phi_rel"},
                                                                              {"#theta_{#vec{P}_{e},#vec{P}_{tot}}", "Opening_ang_P_e_P_tot"},
                                                                              {"#theta_{#vec{q},#vec{P}_{tot}}", "Opening_ang_q_P_tot"},
                                                                              {"#theta_{#vec{q},#vec{P}_{pFD}}", "Opening_ang_q_P_nucFD"},
                                                                              {"#theta_{#vec{q},#vec{P}_{pCD}}", "Opening_ang_q_P_nucCD"},
                                                                              {"#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}}", "Opening_ang_P_nucL_minus_q_nucR"},
                                                                              {"#theta_{#vec{q},#vec{P}_{pL}}", "Opening_ang_q_P_nucL"},
                                                                              {"#theta_{#vec{q},#vec{P}_{pR}}", "Opening_ang_q_P_nucR"},
                                                                              {"#theta_{pFD,pCD}", "Opening_ang_P_nucFD_P_nucCD"},
                                                                              {"Total nucleon 3-momentum", "Total_nucleon_3momentum"},
                                                                              {"Total nucleon 4-momentum", "Total_nucleon_4momentum"},
                                                                              {"P_{pL} vs. P_{pR}", "P_nucL_vs_P_nucR"},
                                                                              {"P_{pFD} vs. P_{pCD}", "P_nucFD_vs_P_nucCD"},
                                                                              {"|#vec{P}_{tot}| vs. |#vec{P}_{rel}|", "P_tot_vs_P_rel_3v"},
                                                                              {"P_{tot}^{#mu} vs. P_{rel}^{#mu}", "P_tot_vs_P_rel_4v"},
                                                                              {"#theta_{e} vs. P_{e}", "theta_e_vs_P_e"},
                                                                              {"#theta_{e} vs. W", "theta_e_vs_W"},
                                                                              {"#phi_{e} vs. P_{e}", "phi_e_vs_P_e"},
                                                                              {"#phi_{e} vs. W", "phi_e_vs_W"},
                                                                              {"#theta_{pFD} vs. P_{pFD}", "theta_nucFD_vs_P_nucFD"},
                                                                              {"#theta_{pFD} vs. W", "theta_nucFD_vs_W"},
                                                                              {"#theta_{pCD} vs. P_{pCD}", "theta_nucCD_vs_P_nucCD"},
                                                                              {"#theta_{pCD} vs. W", "theta_nucCD_vs_W"},
                                                                              {"#phi_{pFD} vs. P_{pFD}", "phi_nucFD_vs_P_nucFD"},
                                                                              {"#phi_{pFD} vs. W", "phi_nucFD_vs_W"},
                                                                              {"#phi_{pCD} vs. P_{pCD}", "phi_nucCD_vs_P_nucCD"},
                                                                              {"#phi_{pCD} vs. W", "phi_nucCD_vs_W"},
                                                                              {"#theta_{e} vs. #phi_{e}", "theta_e_vs_phi_e"},
                                                                              {"#theta_{pFD} vs. #phi_{pFD}", "theta_nucFD_vs_theta_nucFD"},
                                                                              {"#theta_{pCD} vs. #phi_{pCD}", "theta_nucCD_vs_theta_nucCD"},
                                                                              {"#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W", "theta_P_e_P_tot_vs_W"},
                                                                              {"#theta_{#vec{q},#vec{P}_{tot}} vs. W", "theta_q_P_tot_vs_W"},
                                                                              {"#theta_{#vec{q},#vec{P}_{pL}} vs. W", "theta_q_P_nucL_vs_W"},
                                                                              {"#theta_{#vec{q},#vec{P}_{pR}} vs. W", "theta_q_P_nucR_vs_W"},
                                                                              {"#theta_{#vec{q},#vec{P}_{pFD}} vs. W", "theta_q_P_nucFD_vs_W"},
                                                                              {"#theta_{#vec{q},#vec{P}_{pCD}} vs. W", "theta_q_P_nucCD_vs_W"},
                                                                              {"#theta_{#vec{q},#vec{P}_{pL}} vs. r_{pL}=|#vec{P}_{pL}|/|#vec{q}|", "theta_q_P_nucCD_vs_r_nucL"},
                                                                              {"#theta_{#vec{q},#vec{P}_{pL}} vs. #theta_{#vec{q},#vec{P}_{pR}}", "theta_q_P_nucL_vs_theta_q_P_nucR"},
                                                                              {"#theta_{#vec{q},#vec{P}_{pFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}}", "theta_q_P_nucFD_vs_theta_q_P_nucCD"},
                                                                              {"#theta_{pFD,pCD} vs. W", "theta_nucFD_nucCD_vs_W"},
                                                                              {"Q^{2} vs. W", "Q2_vs_W"},
                                                                              {"E_{e} vs. #theta_{e}", "E_e_vs_theta_e"},
                                                                              {"E_{cal} vs. #delta#alpha_{T,L}", "E_cal_vs_dalpha_T_L"},
                                                                              {"E_{cal} vs. #delta#alpha_{T,tot}", "E_cal_vs_dalpha_T_tot"},
                                                                              {"E_{cal} vs. #deltaP_{T,L}", "E_cal_vs_dP_T_L"},
                                                                              {"E_{cal} vs. #deltaP_{T,tot}", "E_cal_vs_dP_T_tot"},
                                                                              {"E_{cal} vs. W", "E_cal_vs_W"},
                                                                              {"#deltaP_{T,L} vs. #delta#alpha_{T,L}", "dP_T_L_vs_dalpha_T_L"},
                                                                              {"#deltaP_{T,tot} vs. #delta#alpha_{T,tot}", "dP_T_tot_vs_dalpha_T_tot"},
                                                                              {"#deltaP_{T,L} vs. W", "dP_T_L_vs_W"},
                                                                              {"#deltaP_{T,tot} vs. W", "dP_T_tot_vs_W"},
                                                                              {"#delta#alpha_{T,L} vs. W", "dalpha_T_L_vs_W"},
                                                                              {"#delta#alpha_{T,tot} vs. W", "dalpha_T_tot_vs_W"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, pattern.first)) { return pattern.second; }
    }

    return "";
}

// GetDRegion function --------------------------------------------------------------------------------------------------------------------------------------------------

std::string GetDRegion(const std::string &Source, const bool &ReturnGoingRegion = false) {
    static const std::vector<std::pair<std::string, std::string>> patterns = {{", FD)", "FD"}, {"FD ", "FD"}, {"FD", "FD"}, {", CD)", "CD"}, {"CD ", "CD"}, {"CD", "CD"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, pattern.first)) { return ReturnGoingRegion ? (pattern.second == "FD" ? "forward-going" : "central-going") : pattern.second; }
    }

    return "";
}

// GetDRegionExplicit function ------------------------------------------------------------------------------------------------------------------------------------------

std::string GetDRegionExplicit(const std::string &Source, const bool &ReturnGoingRegion) {
    static const std::vector<std::pair<std::string, std::string>> patterns = {{"FD", "FD"}, {"CD", "CD"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, pattern.first)) { return ReturnGoingRegion ? (pattern.second == "FD" ? "forward-going" : "central-going") : pattern.second; }
    }

    return "";
}

// GetFSRTitle function -------------------------------------------------------------------------------------------------------------------------------------------------

std::string GetFSRTitle(const std::string &Source, const std::string &PlotsT) {
    std::string FSRTitle;
    std::string Particle = GetParticleName(Source), ParticleShort = GetParticleNameShort(Source), Type = GetType(Source), DRegion = GetDRegion(Source);

    if (PlotsT == "FSRatio") {
        if (!basic_tools::FindSubstring(Source, "vs")) {
            static const std::map<std::string, std::string> typeToTitle = {{"Q2", "Q^{2} ratio - "},
                                                                           {"E_e", "E_{e} ratio - "},
                                                                           {"omega", "#omega ratio - "},
                                                                           {"Ecal", "E_{cal} ratio - "},
                                                                           {"deltaP_T_tot", "#deltaP_{T,tot} ratio - "},
                                                                           {"deltaP_T_L", "#deltaP_{T,L} ratio - "},
                                                                           {"deltaAlpha_T_tot", "#delta#alpha_{T,tot} ratio - "},
                                                                           {"deltaAlpha_T_L", "#delta#alpha_{T,L} ratio - "},
                                                                           {"deltaPhi_T_tot", "#delta#phi_{T,tot} ratio - "},
                                                                           {"deltaPhi_T_L", "#delta#phi_{T,L} ratio - "},
                                                                           {"Opening_ang_P_e_P_tot", "Opening angle between P_{e} and P_{tot} ratio - "},
                                                                           {"Opening_ang_q_P_tot", "Opening angle between q and P_{tot} ratio - "},
                                                                           {"Opening_ang_q_P_nucFD", "Opening angle between q and P_{nuc,FD} ratio - "},
                                                                           {"Opening_ang_q_P_nucCD", "Opening angle between q and P_{nuc,CD} ratio - "},
                                                                           {"Opening_ang_q_P_nucL", "Opening angle between q and leading nucleon P_{nucL} ratio - "},
                                                                           {"Opening_ang_q_P_nucR", "Opening angle between q and recoil nucleon P_{nucR} ratio - "},
                                                                           {"Opening_ang_P_nucFD_P_nucCD", "Opening angle between FD and CD nucleons ratio - "},
                                                                           {"leading_nuc_momentum", "Leading nucleon momentum ratio- "},
                                                                           {"recoil_nuc_momentum", "Recoil nucleon momentum ratio - "},
                                                                           {"total_3momentum", "Total nucleon 3-momentum ratio - "},
                                                                           {"relative_3momentum", "Relative nucleon 3-momentum ratio - "},
                                                                           {"total_4momentum", "Total nucleon 4-momentum ratio - "},
                                                                           {"relative_4momentum", "Relative nucleon 4-momentum ratio - "},
                                                                           {"theta_tot", "#theta_{tot} of total 3-momentum ratio - "},
                                                                           {"phi_tot", "#phi_{tot} of total 3-momentum ratio - "},
                                                                           {"theta_rel", "#theta_{rel} of relative 3-momentum ratio - "},
                                                                           {"phi_rel", "#phi_{rel} of relative 3-momentum ratio - "},
                                                                           {"P_tot_minus_q", "#vec{P}_{tot}-#vec{q} ratio - "},
                                                                           {"Opening_ang_P_nucL_minus_q_nucR", "#theta_{#vec{P}_{nucL}-#vec{q},#vec{P}_{nucR}} ratio - "}};

            auto it = typeToTitle.find(Type);
            if (it != typeToTitle.end()) {
                FSRTitle = it->second;
            } else {
                if (Particle == "Electron") {
                    FSRTitle = Particle + (Type == "momentum" ? " " : " #") + Type + " ratio - ";
                } else {
                    FSRTitle = DRegion + " " + (DRegion == "FD" ? "nucleon" : "proton") + (Type == "momentum" ? " " : " #") + Type + " ratio - ";
                }
            }
        } else {
            static const std::map<std::string, std::string> vsTypeToTitle = {
                {"|#vec{P}_{tot}| vs. |#vec{P}_{rel}|", "|#vec{P}_{tot}| vs. |#vec{P}_{rel}| ratio - "},
                {"P_{tot}^{#mu} vs. P_{rel}^{#mu}", "P_{tot}^{#mu} vs. P_{rel}^{#mu} ratio - "},
                {"P_{pL} vs. P_{pR}", "P_{nucL} vs. P_{nucR} ratio - "},
                {"P_{pFD} vs. P_{pCD}", "P_{nucFD} vs. P_{nucCD} ratio - "},
                {"#theta_{e} vs. P_{e}", "#theta_{e} vs. P_{e} ratio - "},
                {"#theta_{e} vs. W", "#theta_{e} vs. W ratio - "},
                {"#phi_{e} vs. P_{e}", "#phi_{e} vs. P_{e} ratio - "},
                {"#phi_{e} vs. W", "#phi_{e} vs. W ratio - "},
                {"#theta_{pFD} vs. P_{pFD}", "#theta_{nucFD} vs. P_{nucFD}"},
                {"#theta_{pFD} vs. W", "#theta_{nucFD} vs. W ratio - "},
                {"#theta_{pCD} vs. P_{pCD}", "#theta_{nucCD} vs. P_{nucCD}"},
                {"#theta_{pCD} vs. W", "#theta_{nucCD} vs. W ratio - "},
                {"#phi_{pFD} vs. P_{pFD}", "#phi_{nucFD} vs. P_{nucFD}"},
                {"#phi_{pFD} vs. W", "#phi_{nucFD} vs. W ratio - "},
                {"#phi_{pCD} vs. P_{pCD}", "#phi_{nucCD} vs. P_{nucCD}"},
                {"#phi_{pCD} vs. W", "#phi_{nucCD} vs. W ratio - "},
                {"#theta_{e} vs. #phi_{e}", "#theta_{e} vs. #phi_{e} ratio - "},
                {"#theta_{pFD} vs. #phi_{pFD}", "#theta_{nucFD} vs. #phi_{nucFD} ratio - "},
                {"#theta_{pCD} vs. #phi_{pCD}", "#theta_{nucCD} vs. #phi_{nucCD} ratio - "},
                {"#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W", "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W ratio - "},
                {"#theta_{#vec{q},#vec{P}_{tot}} vs. W", "#theta_{#vec{q},#vec{P}_{tot}} vs. W ratio - "},
                {"#theta_{#vec{q},#vec{P}_{pL}} vs. W", "#theta_{#vec{q},#vec{P}_{nucL}} vs. W ratio - "},
                {"#theta_{#vec{q},#vec{P}_{pR}} vs. W", "#theta_{#vec{q},#vec{P}_{nucR}} vs. W ratio - "},
                {"#theta_{#vec{q},#vec{P}_{pFD}} vs. W", "#theta_{#vec{q},#vec{P}_{nucFD}} vs. W ratio - "},
                {"#theta_{#vec{q},#vec{P}_{pCD}} vs. W", "#theta_{#vec{q},#vec{P}_{nucCD}} vs. W ratio - "},
                {"#theta_{#vec{q},#vec{P}_{pL}} vs. r_{pL}=|#vec{P}_{pL}|/|#vec{q}|", "#theta_{#vec{q},#vec{P}_{nucL}} vs. r_{pL}=|#vec{P}_{nucL}|/|#vec{q}| ratio - "},
                {"#theta_{#vec{q},#vec{P}_{pL}} vs. #theta_{#vec{q},#vec{P}_{pR}}", "#theta_{#vec{q},#vec{P}_{nucL}} vs. #theta_{#vec{q},#vec{P}_{nucR}} ratio - "},
                {"#theta_{#vec{q},#vec{P}_{pFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}}", "#theta_{#vec{q},#vec{P}_{nucFD}} vs. #theta_{#vec{q},#vec{P}_{nucCD}} ratio - "},
                {"#theta_{pFD,pCD} vs. W", "#theta_{nucFD,nucCD} vs. W ratio - "},
                {"Q^{2} vs. W", "Q^{2} vs. W ratio - "},
                {"E_{e} vs. #theta_{e}", "E_{e} vs. #theta_{e} ratio - "},
                {"E_{cal} vs. #delta#alpha_{T,L}", "E_{cal} vs. #delta#alpha_{T,L} ratio - "},
                {"E_{cal} vs. #delta#alpha_{T,tot}", "E_{cal} vs. #delta#alpha_{T,tot} ratio - "},
                {"E_{cal} vs. #deltaP_{T,L}", "E_{cal} vs. #deltaP_{T,L} ratio - "},
                {"E_{cal} vs. #deltaP_{T,tot}", "E_{cal} vs. #deltaP_{T,tot} ratio - "},
                {"E_{cal} vs. W", "E_{cal} vs. W ratio - "},
                {"#deltaP_{T,L} vs. #delta#alpha_{T,L}", "#deltaP_{T,L} vs. #delta#alpha_{T,L} ratio - "},
                {"#deltaP_{T,tot} vs. #delta#alpha_{T,tot}", "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} ratio - "},
                {"#deltaP_{T,L} vs. W", "#deltaP_{T,L} vs. W ratio - "},
                {"#deltaP_{T,tot} vs. W", "#deltaP_{T,tot} vs. W ratio - "},
                {"#delta#alpha_{T,L} vs. W", "#delta#alpha_{T,L} vs. W ratio - "},
                {"#delta#alpha_{T,tot} vs. W", "#delta#alpha_{T,tot} vs. W ratio - "}};

            auto it = vsTypeToTitle.find(Source);
            if (it != vsTypeToTitle.end()) { FSRTitle = it->second; }
        }
    } else {
        FSRTitle = (Type == "momentum" ? Particle + " " : "#" + Type + "_{" + ParticleShort + "}");
    }

    return FSRTitle;
}

// GetTitle function ----------------------------------------------------------------------------------------------------------------------------------------------------

std::string GetTitle(const std::string &Source) {
    std::string Title, Type = GetType(Source), ParticleShort = GetParticleNameShort(Source), Particle = GetParticleName(Source);

    if (Type == "momentum") {
        Title = Particle + " " + Type;
    } else {
        Title = "#" + Type + "_{" + ParticleShort + "}";
    }

    return Title;
}

// GetStatsTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

std::string GetStatsTitle(const std::string &Source) {
    std::string StatsTitle, StatsType;

    static const std::vector<std::pair<std::string, std::string>> patterns = {{"#theta", "#theta_{"}, {"#phi", "#phi_{"}};

    std::string FS = GetFS(Source), ParticleShort = GetParticleNameShort(Source);

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, pattern.first)) {
            StatsType = pattern.second + ParticleShort + "}";
            break;
        }
    }

    StatsTitle = StatsType + " (" + FS + ")";

    return StatsTitle;
}

// SetXLabel function ---------------------------------------------------------------------------------------------------------------------------------------------------

std::string SetXLabel(const std::string &Source) {
    static const std::vector<std::pair<std::string, std::string>> patterns = {{"momentum", "P_{"}, {"#theta", "#theta_{"}, {"#phi", "#phi_{"}};

    std::string ParticleShort = GetParticleNameShort(Source);

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, pattern.first)) { return pattern.second + ParticleShort + "} [" + (pattern.first == "momentum" ? "GeV/c" : "Deg") + "]"; }
    }

    return "";
}

// SetSaveDir function --------------------------------------------------------------------------------------------------------------------------------------------------

std::string SetSaveDir(const std::string &Source, const std::string &BaseSaveDir, const std::string &Mod) {
    std::string SaveDir, TestSaveDir;

    std::string Particle = GetParticleName(Source), ParticleLC = GetParticleNameLC(Source), FS = GetFS(Source), Type = GetType(Source);

    static const std::vector<std::pair<std::string, std::string>> patterns = {
        {"Electron|electron", "/00_" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/"},
        {", FD)|FD " + Particle + "|FD " + ParticleLC, "/01_FD_" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/"},
        {", CD)|CD " + Particle + "|CD " + ParticleLC, "/02_CD_" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/"}};

    for (const auto &pattern : patterns) {
        if (basic_tools::FindSubstring(Source, pattern.first)) {
            SaveDir = BaseSaveDir + pattern.second;
            TestSaveDir = SaveDir + "Cloned_hist_test/";
            break;
        }
    }

    if (SaveDir.empty()) {
        SaveDir = BaseSaveDir + "/" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/";
        TestSaveDir = SaveDir + "Cloned_hist_test/";
    }

    if ((Mod == "test") || (Mod == "Test")) {
        system(("mkdir -p " + TestSaveDir).c_str());
        return TestSaveDir;
    } else {
        system(("mkdir -p " + SaveDir).c_str());
        return SaveDir;
    }
}

void SetSaveDir(std::string &SaveDir, std::string &TestSaveDir, const std::string &RecTitle, const std::string &Histogram1DSaveNamePath, const std::string &PlotsT,
                const std::string &DRegion, const std::string &Particle, const std::string &ParticleLC, const std::string &ParticleShort, const std::string &Type) {
    if (PlotsT == "FSRatio") {
        if (!basic_tools::FindSubstring(Type, "vs") && !basic_tools::FindSubstring(Type, "vs.") && !basic_tools::FindSubstring(Type, "VS") && !basic_tools::FindSubstring(Type, "VS.")) {
            if (Type == "W" || Type == "Q2" || Type == "E_e" || Type == "omega" || Type == "Ecal" || Type == "deltaP_T_tot" || Type == "deltaP_T_L" || Type == "deltaAlpha_T_tot" ||
                Type == "deltaAlpha_T_L" || Type == "deltaPhi_T_tot" || Type == "deltaPhi_T_L" || Type == "total_3momentum" || Type == "relative_3momentum" || Type == "total_4momentum" ||
                Type == "relative_4momentum" || Type == "leading_nuc_momentum" || Type == "recoil_nuc_momentum" || Type == "theta_tot" || Type == "phi_tot" || Type == "theta_rel" ||
                Type == "phi_rel" || Type == "Opening_ang_P_nucL_minus_q_nucR" || Type == "P_tot_minus_q") {
                SaveDir = Histogram1DSaveNamePath + "/";
                TestSaveDir = SaveDir + "Cloned_hist_test/";
            } else {
                if (basic_tools::FindSubstring(RecTitle, "Electron") || basic_tools::FindSubstring(RecTitle, "electron")) {
                    SaveDir = Histogram1DSaveNamePath + "/00_" + Particle + "_" + Type + "_plots/";
                    TestSaveDir = SaveDir + "Cloned_hist_test/";
                } else {
                    if (DRegion == "FD") {
                        SaveDir = Histogram1DSaveNamePath + "/01_FD_Nucleon_" + Type + "_plots/";
                        TestSaveDir = SaveDir + "Cloned_hist_test/";
                    } else if (DRegion == "CD") {
                        SaveDir = Histogram1DSaveNamePath + "/02_CD_Nucleon_" + Type + "_plots/";
                        TestSaveDir = SaveDir + "Cloned_hist_test/";
                    } else {
                        SaveDir = Histogram1DSaveNamePath + "/" + Particle + "_" + Type + "_plots/";
                        TestSaveDir = SaveDir + "Cloned_hist_test" + "/";
                    }
                }
            }
        } else {
            SaveDir = Histogram1DSaveNamePath + "/" + Type + "_plots/";
            TestSaveDir = SaveDir + "Cloned_hist_test/";
        }
    } else {
        if (basic_tools::FindSubstring(RecTitle, "Electron") || basic_tools::FindSubstring(RecTitle, "electron")) {
            SaveDir = Histogram1DSaveNamePath + "/00_" + Particle + "_" + Type + "_plots/";
            TestSaveDir = SaveDir + "Cloned_hist_test/";
        } else {
            if (basic_tools::FindSubstring(RecTitle, ", FD)") || basic_tools::FindSubstring(RecTitle, "FD " + Particle) || basic_tools::FindSubstring(RecTitle, "FD " + ParticleLC)) {
                SaveDir = Histogram1DSaveNamePath + "/01_FD_" + Particle + "_" + Type + "_plots/";
                TestSaveDir = SaveDir + "Cloned_hist_test/";
            } else if (basic_tools::FindSubstring(RecTitle, ", CD)") || basic_tools::FindSubstring(RecTitle, "CD " + Particle) || basic_tools::FindSubstring(RecTitle, "CD " + ParticleLC)) {
                SaveDir = Histogram1DSaveNamePath + "/02_CD_" + Particle + "_" + Type + "_plots/";
                TestSaveDir = SaveDir + "Cloned_hist_test/";
            } else {
                SaveDir = Histogram1DSaveNamePath + "/" + Particle + "_" + Type + "_plots/";
                TestSaveDir = SaveDir + "Cloned_hist_test" + "/";
            }
        }
    }

    system(("mkdir -p " + SaveDir).c_str());
    system(("mkdir -p " + TestSaveDir).c_str());
}

// SetFSRatioSaveDir function -------------------------------------------------------------------------------------------------------------------------------------------

void SetFSRatioSaveDir(std::string &SaveDir, std::string &TestSaveDir, const std::string &RecTitle, const std::string &FinalState, const std::string &Histogram1DSaveNamePathOriginal,
                       const std::string &PlotsT, const std::string &DRegion, const std::string &Particle, const std::string &ParticleLC, const std::string &ParticleShort,
                       const std::string &Type) {
    std::string Histogram1DSaveNamePath, FStoFind, FSTopology;  // TODO: reconfirm what is a topology

    if (FinalState == "1p" || FinalState == "1n") {
        FSTopology = "_1N", FStoFind = "02_1p";
    } else if (FinalState == "pFDpCD" || FinalState == "nFDpCD") {
        FSTopology = "_2N", FStoFind = "05_pFDpCD";
    }

    if (!basic_tools::FindSubstring(RecTitle, "vs") && !basic_tools::FindSubstring(RecTitle, "vs.") && !basic_tools::FindSubstring(RecTitle, "VS") &&
        !basic_tools::FindSubstring(RecTitle, "VS.")) {
        if (Type == "deltaP_T_tot" || Type == "deltaP_T_L" || Type == "deltaAlpha_T_tot" || Type == "deltaAlpha_T_L" || Type == "deltaPhi_T_tot" || Type == "deltaPhi_T_L") {  // TKI ratios
            Histogram1DSaveNamePath =
                Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) + "/FS_TKI_ratio_plots/FS_" + Type + "_ratio_plots" + FSTopology;
        } else if (basic_tools::FindSubstring(Type, "Opening_ang") || basic_tools::FindSubstring(Type, "theta") || basic_tools::FindSubstring(Type, "phi")) {
            if (basic_tools::FindSubstring(Type, "Opening_ang")) {  // Angle ratios
                Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) +
                                          "/FS_ang_ratio_plots/FS_Opening_ang_ratio_plots/FS_" + Type + "_ratio_plots" + FSTopology;
            } else if (basic_tools::FindSubstring(Type, "theta")) {
                Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) + "/FS_ang_ratio_plots/FS_theta_ratio_plots/FS_" +
                                          Type + "_ratio_plots" + FSTopology;
            } else if (basic_tools::FindSubstring(Type, "phi")) {
                Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) + "/FS_ang_ratio_plots/FS_phi_ratio_plots/FS_" +
                                          Type + "_ratio_plots" + FSTopology;
            }
        } else if (basic_tools::FindSubstring(Type, "momentum") || basic_tools::FindSubstring(Type, "P_tot_minus_q")) {  // Momentum ratios
            Histogram1DSaveNamePath =
                Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) + "/FS_momentum_ratio_plots/FS_" + Type + "_ratio_plots" + FSTopology;
        } else {
            Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) + "/FS_" + Type + "_ratio_plots" + FSTopology;
        }
    } else {
        Histogram1DSaveNamePath =
            Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) + "/FS_2D_hist_ratios/FS_" + Type + "_ratio_plots" + FSTopology;
    }

    //<editor-fold desc="original">
    //    std::string Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find("05_pFDpCD") - 1) + "/FS_" + Type + "_ratio_plots";

    /*
    if (Type == "W" || Type == "Q2") {
    Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find("05_pFDpCD") - 1) + "/FS_" + Type + "_ratio_plots";
    } else {
    Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find("05_pFDpCD") - 1) + "/06a_FS_" + Type + "_ratio_plots";
    }
    */
    //</editor-fold>

    SetSaveDir(SaveDir, TestSaveDir, RecTitle, Histogram1DSaveNamePath, PlotsT, DRegion, Particle, ParticleLC, ParticleShort, Type);
}

// SetType function -----------------------------------------------------------------------------------------------------------------------------------------------------

std::string SetType(const std::string &Title) {
    std::string Type;

    if (basic_tools::FindSubstring(Title, "momentum")) {
        Type = "momentum";
    } else if (basic_tools::FindSubstring(Title, "#theta")) {
        Type = "theta";
    } else if (basic_tools::FindSubstring(Title, "#phi")) {
        Type = "phi";
    }

    return Type;
}

// SetStatsTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

std::string SetStatsTitle(const std::string &RecTitle) {
    std::string StatsTitle;

    std::string ParticleShort = GetParticleNameShort(RecTitle);

    if (basic_tools::FindSubstring(RecTitle, "momentum")) {
        StatsTitle = "P_{" + ParticleShort + "}";
    } else if (basic_tools::FindSubstring(RecTitle, "#theta")) {
        StatsTitle = "#theta_{" + ParticleShort + "}";
    } else if (basic_tools::FindSubstring(RecTitle, "#phi")) {
        StatsTitle = "#phi_{" + ParticleShort + "}";
    }

    return StatsTitle;
}

// SetTitle function ----------------------------------------------------------------------------------------------------------------------------------------------------

std::string SetTitle(const std::string &RecTitle, const std::string &PlotsT, const std::string &DRegion) {
    std::string Title;

    std::string Particle = GetParticleName(RecTitle);
    std::string ParticleShort = GetParticleNameShort(RecTitle);
    std::string Type = SetType(RecTitle);

    if (PlotsT == "FSRatio") {
        if (Particle == "Electron") {
            if (Type == "momentum") {
                Title = Particle + " " + Type + " ratio";
            } else {
                Title = Particle + " #" + Type + " ratio";
            }
        } else {
            if (DRegion == "FD") {
                if (Type == "momentum") {
                    Title = DRegion + " nucleon " + Type + " ratio";
                } else {
                    Title = DRegion + " nucleon #" + Type + " ratio";
                }
            } else if (DRegion == "CD") {
                if (Type == "momentum") {
                    Title = DRegion + " proton " + Type + " ratio";
                } else {
                    Title = DRegion + " proton #" + Type + " ratio";
                }
            }
        }
    } else {
        if (Type == "momentum") {
            Title = Particle + " " + Type;
        } else {
            Title = "#" + Type + "_{" + ParticleShort + "}";
        }
    }

    return Title;
}

// SetXAxisTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

std::string SetXAxisTitle(const std::string &RecTitle) {
    std::string XAxisTitle;

    std::string Particle = GetParticleName(RecTitle);
    std::string ParticleShort = GetParticleNameShort(RecTitle);

    if (!basic_tools::FindSubstring(RecTitle, "vs") && !basic_tools::FindSubstring(RecTitle, "vs.") && !basic_tools::FindSubstring(RecTitle, "VS") &&
        !basic_tools::FindSubstring(RecTitle, "VS.")) {
        if (basic_tools::FindSubstring(RecTitle, "momentum") && !basic_tools::FindSubstring(RecTitle, "-momentum") && !basic_tools::FindSubstring(RecTitle, "Total") &&
            !basic_tools::FindSubstring(RecTitle, "Relative") && !basic_tools::FindSubstring(RecTitle, "Leading") && !basic_tools::FindSubstring(RecTitle, "Recoil") &&
            !basic_tools::FindSubstring(RecTitle, "FD proton") && !basic_tools::FindSubstring(RecTitle, "FD neutron") && !basic_tools::FindSubstring(RecTitle, "CD proton")) {
            XAxisTitle = "Momentum [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "momentum") && (basic_tools::FindSubstring(RecTitle, "FD proton") || basic_tools::FindSubstring(RecTitle, "FD neutron"))) {
            XAxisTitle = "P_{nucFD} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "momentum") && basic_tools::FindSubstring(RecTitle, "CD proton")) {
            XAxisTitle = "P_{nucCD} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "momentum") && basic_tools::FindSubstring(RecTitle, "Leading")) {
            XAxisTitle = "P_{nucL} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "momentum") && basic_tools::FindSubstring(RecTitle, "Recoil")) {
            XAxisTitle = "P_{nucR} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "3-momentum") && basic_tools::FindSubstring(RecTitle, "Total")) {
            XAxisTitle = "|#vec{P}_{tot}| = |#vec{P}_{nucL} + #vec{P}_{nucR}| [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "4-momentum") && basic_tools::FindSubstring(RecTitle, "Total")) {
            XAxisTitle = "P_{tot}^{#mu} = (P_{nucL}^{#mu} + P_{nucR}^{#mu}) [GeV/c] [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "3-momentum") && basic_tools::FindSubstring(RecTitle, "Relative")) {
            XAxisTitle = "|#vec{P}_{rel}| = |#vec{P}_{nucL} - #vec{P}_{nucR}|/2 [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "4-momentum") && basic_tools::FindSubstring(RecTitle, "Relative")) {
            XAxisTitle = "P_{rel}^{#mu} = (P_{nucL}^{#mu} - P_{nucR}^{#mu})/2 [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "#vec{P}_{tot}-#vec{q}")) {
            XAxisTitle = "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{pL} + #vec{P}_{pR}- #vec{q}| [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "Q^{2}")) {
            XAxisTitle = "Q^{2} [GeV^{2}/c^{2}]";
        } else if (basic_tools::FindSubstring(RecTitle, "E_{e}")) {
            XAxisTitle = "E_{e} [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#omega")) {
            XAxisTitle = "#omega = E_{beam}-E_{e} [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "E_{cal}")) {
            XAxisTitle = "E_{cal} = E_{e} + T_{nuc,FD} + T_{nuc,CD} [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#deltaP_{T,tot}")) {
            XAxisTitle = "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nucFD} + #vec{p}_{T,nucCD}| [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "#deltaP_{T,L}")) {
            XAxisTitle = "#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,nucL}| [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "#delta#alpha_{T,tot}")) {
            XAxisTitle = "#delta#alpha_{T,tot} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#delta#alpha_{T,L}")) {
            XAxisTitle = "#delta#alpha_{T,L} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#delta#phi_{T,tot}")) {
            XAxisTitle = "#delta#phi_{T,tot} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#delta#phi_{T,L}")) {
            XAxisTitle = "#delta#phi_{T,L} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta") && !basic_tools::FindSubstring(RecTitle, "#theta_{#vec{") &&
                   !(basic_tools::FindSubstring(RecTitle, "#theta_{pFD,pCD}") || basic_tools::FindSubstring(RecTitle, "#theta_{nFD,pCD}")) &&
                   !(basic_tools::FindSubstring(RecTitle, "#theta_{tot}") || basic_tools::FindSubstring(RecTitle, "#theta_{rel}"))) {
            XAxisTitle = "#theta [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#phi") && !(basic_tools::FindSubstring(RecTitle, "#phi_{tot}") || basic_tools::FindSubstring(RecTitle, "#phi_{rel}"))) {
            XAxisTitle = "#phi [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{tot}")) {
            XAxisTitle = "#theta_{tot} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#phi_{tot}")) {
            XAxisTitle = "#phi_{tot} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{rel}")) {
            XAxisTitle = "#theta_{rel} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#phi_{rel}")) {
            XAxisTitle = "phi_{rel} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{P}_{e},#vec{P}_{tot}}")) {
            XAxisTitle = "#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{tot}}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{tot}} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pFD}}") || basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nFD}}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{nuc,FD}} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pCD}}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{p,CD}} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}}")) {
            XAxisTitle = "#theta_{#vec{P}_{nucL}-#vec{q},#vec{P}_{nucR}} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pL}}") || basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nL}}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{nucL}} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pR}}") || basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nR}}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{nucR}} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{pFD,pCD}") || basic_tools::FindSubstring(RecTitle, "#theta_{nFD,pCD}")) {
            XAxisTitle = "#theta_{nucFD,nucCD} [Deg]";
        }
    } else {
        if (basic_tools::FindSubstring(RecTitle, "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|")) {
            XAxisTitle = "|#vec{P}_{tot}| = |#vec{P}_{nucL} + #vec{P}_{nucR}| [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "P_{tot}^{#mu} vs. P_{rel}^{#mu}")) {
            XAxisTitle = "P_{tot}^{#mu} = P_{pL}^{#mu} + P_{pR}^{#mu} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "P_{pL} vs. P_{pR}") || basic_tools::FindSubstring(RecTitle, "P_{nL} vs. P_{nR}")) {
            XAxisTitle = "P_{nucL} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "P_{pFD} vs. P_{pCD}") || basic_tools::FindSubstring(RecTitle, "P_{nFD} vs. P_{pCD}")) {
            XAxisTitle = "P_{nucFD} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{e} vs. P_{e}")) {
            XAxisTitle = "P_{e} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{e} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#phi_{e} vs. P_{e}")) {
            XAxisTitle = "P_{e} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "#phi_{e} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{e} vs. #phi_{e}")) {
            XAxisTitle = "#phi_{e} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{pFD} vs. #phi_{pFD}") || basic_tools::FindSubstring(RecTitle, "#theta_{nFD} vs. #phi_{nFD}")) {
            XAxisTitle = "#phi_{nucFD} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{pCD} vs. #phi_{pCD}")) {
            XAxisTitle = "#phi_{nucCD} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{pFD} vs. W") || basic_tools::FindSubstring(RecTitle, "#theta_{nFD} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{pFD} vs. P_{pFD}") || basic_tools::FindSubstring(RecTitle, "#theta_{nFD} vs. P_{nFD}")) {
            XAxisTitle = "P_{nucFD} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{pCD} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{pCD} vs. P_{pCD}")) {
            XAxisTitle = "P_{pCD} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "#phi_{pFD} vs. W") || basic_tools::FindSubstring(RecTitle, "#phi_{nFD} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#phi_{pFD} vs. P_{pFD}") || basic_tools::FindSubstring(RecTitle, "#phi_{nFD} vs. P_{nFD}")) {
            XAxisTitle = "P_{nucFD} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "#phi_{pCD} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#phi_{pCD} vs. P_{pCD}")) {
            XAxisTitle = "P_{pCD} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{tot}} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pL}} vs. W") || basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nL}} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pR}} vs. W") || basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nR}} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pFD}} vs. W") || basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nFD}} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pCD}} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pL}} vs. r_{pL}=|#vec{P}_{pL}|/|#vec{q}|") ||
                   basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nL}} vs. r_{nL}=|#vec{P}_{nL}|/|#vec{q}|")) {
            XAxisTitle = "r_{nucL}";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pL}} vs. #theta_{#vec{q},#vec{P}_{pR}}") ||
                   basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nL}} vs. #theta_{#vec{q},#vec{P}_{nR}}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{nucL}} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}}") ||
                   basic_tools::FindSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{nucFD}} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#theta_{pFD,pCD} vs. W") || basic_tools::FindSubstring(RecTitle, "#theta_{nFD,pCD} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "Q^{2} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "E_{e} vs. #theta_{e}")) {
            XAxisTitle = "#theta_{e} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "E_{cal} vs. #delta#alpha_{T,L}")) {
            XAxisTitle = "#delta#alpha_{T,L} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "E_{cal} vs. #delta#alpha_{T,tot}")) {
            XAxisTitle = "#delta#alpha_{T,tot} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "E_{cal} vs. #deltaP_{T,L}")) {
            XAxisTitle = "#deltaP_{T,L} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "E_{cal} vs. #deltaP_{T,tot}")) {
            XAxisTitle = "#deltaP_{T,tot} [GeV/c]";
        } else if (basic_tools::FindSubstring(RecTitle, "E_{cal} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#deltaP_{T,L} vs. #delta#alpha_{T,L}")) {
            XAxisTitle = "#delta#alpha_{T,L} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#deltaP_{T,tot} vs. #delta#alpha_{T,tot}")) {
            XAxisTitle = "#delta#alpha_{T,tot} [Deg]";
        } else if (basic_tools::FindSubstring(RecTitle, "#deltaP_{T,L} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#deltaP_{T,tot} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#delta#alpha_{T,L} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (basic_tools::FindSubstring(RecTitle, "#delta#alpha_{T,tot} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        }
    }

    return XAxisTitle;
}

// SetYAxisTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

std::string SetYAxisTitle(const std::string &PlotT, const std::string &FinalState, const std::string &Numerator, const std::string &Denominator, const std::string &Title = "") {
    std::string YAxisTitle;

    std::string xLabelNum = Numerator.substr(0, Numerator.find_last_of('[') - 1);
    std::string xLabelDem = Denominator.substr(0, Denominator.find_last_of('[') - 1);

    if (PlotT == "efficiency" || PlotT == "Efficiency" || PlotT == "eff" || PlotT == "Eff") {
        YAxisTitle = "#epsilon_{eff} = " + xLabelNum + "^{rec}" + "/" + xLabelDem;
    } else if (PlotT == "acceptance correction" || PlotT == "Acceptance Correction" || PlotT == "acorrection" || PlotT == "ACorrection" || PlotT == "acorr" || PlotT == "ACorr") {
        YAxisTitle = std::string("#alpha = ") + "#frac{1}{#epsilon_{eff}} = " + xLabelNum + "/" + xLabelDem + "^{rec}";
    } else if (PlotT == "FSRatio") {
        if (!basic_tools::FindSubstring(Title, "vs") && !basic_tools::FindSubstring(Title, "vs.") && !basic_tools::FindSubstring(Title, "VS") && !basic_tools::FindSubstring(Title, "VS.")) {
            if (FinalState == "1p" || FinalState == "1n") {
                YAxisTitle = "1nFD/1pFD";
            } else if (FinalState == "pFDpCD" || FinalState == "nFDpCD") {
                YAxisTitle = "nFDpCD/pFDpCD";
            }
        } else {
            if (basic_tools::FindSubstring(Title, "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|")) {
                YAxisTitle = "|#vec{P}_{rel}| = |#vec{P}_{nucL} - #vec{P}_{nucR}|/2 [GeV/c]";
            } else if (basic_tools::FindSubstring(Title, "P_{tot}^{#mu} vs. P_{rel}^{#mu}")) {
                YAxisTitle = "P_{rel}^{#mu} = (P_{pL}^{#mu} - P_{pR}^{#mu})/2 [GeV/c]";
            } else if (basic_tools::FindSubstring(Title, "P_{pL} vs. P_{pR}") || basic_tools::FindSubstring(Title, "P_{nL} vs. P_{nR}")) {
                YAxisTitle = "P_{nucR} [GeV/c]";
            } else if (basic_tools::FindSubstring(Title, "P_{pFD} vs. P_{pCD}") || basic_tools::FindSubstring(Title, "P_{nFD} vs. P_{pCD}")) {
                YAxisTitle = "P_{nucCD} [GeV/c]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{e} vs. P_{e}")) {
                YAxisTitle = "#theta_{e} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{e} vs. W")) {
                YAxisTitle = "#theta_{e} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#phi_{e} vs. P_{e}")) {
                YAxisTitle = "#phi_{e} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#phi_{e} vs. W")) {
                YAxisTitle = "#phi_{e} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{e} vs. #phi_{e}")) {
                YAxisTitle = "#theta_{e} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{pFD} vs. #phi_{pFD}") || basic_tools::FindSubstring(Title, "#theta_{nFD} vs. #phi_{nFD}")) {
                YAxisTitle = "#theta_{nucFD} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{pCD} vs. #phi_{pCD}")) {
                YAxisTitle = "#theta_{nucCD} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{pFD} vs. W") || basic_tools::FindSubstring(Title, "#theta_{nFD} vs. W")) {
                YAxisTitle = "#theta_{nucFD} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{pFD} vs. P_{pFD}") || basic_tools::FindSubstring(Title, "#theta_{nFD} vs. P_{nFD}")) {
                YAxisTitle = "#theta_{nucFD} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{pCD} vs. W") || basic_tools::FindSubstring(Title, "#theta_{nCD} vs. W")) {
                YAxisTitle = "#theta_{nucCD} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{pCD} vs. P_{pCD}") || basic_tools::FindSubstring(Title, "#theta_{nCD} vs. P_{nCD}")) {
                YAxisTitle = "#theta_{nucCD} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#phi_{pFD} vs. W") || basic_tools::FindSubstring(Title, "#phi_{nFD} vs. W")) {
                YAxisTitle = "#phi_{nucFD} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#phi_{pFD} vs. P_{pFD}") || basic_tools::FindSubstring(Title, "#phi_{nFD} vs. P_{nFD}")) {
                YAxisTitle = "#phi_{nucFD} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#phi_{pCD} vs. W") || basic_tools::FindSubstring(Title, "#phi_{nCD} vs. W")) {
                YAxisTitle = "#phi_{nucCD} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#phi_{pCD} vs. P_{pCD}") || basic_tools::FindSubstring(Title, "#phi_{nCD} vs. P_{nCD}")) {
                YAxisTitle = "#phi_{nucFD} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W")) {
                YAxisTitle = "#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{tot}} vs. W")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{tot}} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{pL}} vs. W") || basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{nL}} vs. W")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucL}} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{pR}} vs. W") || basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{nR}} vs. W")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucR}} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{pFD}} vs. W") || basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{nFD}} vs. W")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucFD}} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{pCD}} vs. W") || basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{nCD}} vs. W")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucCD}} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{pL}} vs. r_{pL}=|#vec{P}_{pL}|/|#vec{q}|") ||
                       basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{nL}} vs. r_{nL}=|#vec{P}_{nL}|/|#vec{q}|")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucL}} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{pL}} vs. #theta_{#vec{q},#vec{P}_{pR}}") ||
                       basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{nL}} vs. #theta_{#vec{q},#vec{P}_{nR}}")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucR}} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{pFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}}") ||
                       basic_tools::FindSubstring(Title, "#theta_{#vec{q},#vec{P}_{nFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}}")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucCD}} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#theta_{pFD,pCD} vs. W") || basic_tools::FindSubstring(Title, "#theta_{nFD,pCD} vs. W")) {
                YAxisTitle = "#theta_{nucFD,nucCD} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "Q^{2} vs. W")) {
                YAxisTitle = "Q^{2} [GeV^{2}/c^{2}]";
            } else if (basic_tools::FindSubstring(Title, "E_{e} vs. #theta_{e}")) {
                YAxisTitle = "E_{e} [GeV]";
            } else if (basic_tools::FindSubstring(Title, "E_{cal} vs. #delta#alpha_{T,L}")) {
                YAxisTitle = "E_{cal} [GeV]";
            } else if (basic_tools::FindSubstring(Title, "E_{cal} vs. #delta#alpha_{T,tot}")) {
                YAxisTitle = "E_{cal} [GeV]";
            } else if (basic_tools::FindSubstring(Title, "E_{cal} vs. #deltaP_{T,L}")) {
                YAxisTitle = "E_{cal} [GeV]";
            } else if (basic_tools::FindSubstring(Title, "E_{cal} vs. #deltaP_{T,tot}")) {
                YAxisTitle = "E_{cal} [GeV]";
            } else if (basic_tools::FindSubstring(Title, "E_{cal} vs. W")) {
                YAxisTitle = "E_{cal} [GeV]";
            } else if (basic_tools::FindSubstring(Title, "#deltaP_{T,L} vs. #delta#alpha_{T,L}")) {
                YAxisTitle = "#deltaP_{T,L} [GeV/c]";
            } else if (basic_tools::FindSubstring(Title, "#deltaP_{T,tot} vs. #delta#alpha_{T,tot}")) {
                YAxisTitle = "#deltaP_{T,tot} [GeV/c]";
            } else if (basic_tools::FindSubstring(Title, "#deltaP_{T,L} vs. W")) {
                YAxisTitle = "#deltaP_{T,L} [GeV/c]";
            } else if (basic_tools::FindSubstring(Title, "#deltaP_{T,tot} vs. W")) {
                YAxisTitle = "#deltaP_{T,tot} [GeV/c]";
            } else if (basic_tools::FindSubstring(Title, "#delta#alpha_{T,L} vs. W")) {
                YAxisTitle = "#delta#alpha_{T,L} [Deg]";
            } else if (basic_tools::FindSubstring(Title, "#delta#alpha_{T,tot} vs. W")) {
                YAxisTitle = "#delta#alpha_{T,tot} [Deg]";
            }
        }
    }

    return YAxisTitle;
}

// SettingSaveNames function --------------------------------------------------------------------------------------------------------------------------------------------

void SettingSaveNames(const std::string &SampleName, const std::string &Type, const std::string &Particle, const std::string &SaveDir, const std::string &TestSaveDir,
                      const std::string &PlotsT, std::string &Numerator_Clone_SaveName, std::string &Numerator_Clone_test_SaveName, std::string &Numerator_Clone_test_rebined_SaveName,
                      std::string &Denominator_Clone_SaveName, std::string &Denominator_Clone_test_SaveName, std::string &Denominator_Clone_test_rebined_SaveName, std::string &sNameFlag,
                      std::string &PlotSaveName, const std::string &DRegion = "", const std::string &FinalState = "") {
    std::string FinalState1, FinalState2;

    if (basic_tools::FindSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (basic_tools::FindSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    if (FinalState == "1p" || FinalState == "1n") {
        FinalState1 = "_1n_", FinalState2 = "_1p_";
    } else if (FinalState == "pFDpCD" || FinalState == "nFDpCD") {
        FinalState1 = "_nFDpCD_", FinalState2 = "_pFDpCD_";
    }

    if (basic_tools::FindSubstring(PlotsT, "FSRatio")) {  // for FSRation plots
        if (!basic_tools::FindSubstring(Type, "vs") && !basic_tools::FindSubstring(Type, "vs.") && !basic_tools::FindSubstring(Type, "VS") && !basic_tools::FindSubstring(Type, "VS.")) {
            if (Type == "W" || Type == "Q2" || Type == "E_e" || Type == "omega" || Type == "Ecal" || Type == "deltaP_T_tot" || Type == "deltaP_T_L" || Type == "deltaAlpha_T_tot" ||
                Type == "deltaAlpha_T_L" || Type == "deltaPhi_T_tot" || Type == "deltaPhi_T_L" || Type == "Opening_ang_P_e_P_tot" || Type == "Opening_ang_q_P_tot" ||
                Type == "Opening_ang_q_P_nucFD" || Type == "Opening_ang_q_P_nucCD" || Type == "Opening_ang_q_P_nucL" || Type == "Opening_ang_q_P_nucR" ||
                Type == "Opening_ang_P_nucFD_P_nucCD" || Type == "total_3momentum" || Type == "relative_3momentum" || Type == "total_4momentum" || Type == "relative_4momentum" ||
                Type == "theta_tot" || Type == "phi_tot" || Type == "theta_rel" || Type == "phi_rel" || Type == "leading_nuc_momentum" || Type == "recoil_nuc_momentum" ||
                Type == "P_tot_minus_q" || Type == "Opening_ang_P_nucL_minus_q_nucR") {
                Numerator_Clone_SaveName = SaveDir + sNameFlag + "01_" + Type + FinalState1 + "Clone.png";
                Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_" + Type + FinalState1 + "Clone_test.png";
                Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_" + Type + FinalState1 + "Clone_test_rebined.png";

                Denominator_Clone_SaveName = SaveDir + sNameFlag + "02_" + Type + FinalState2 + "Clone.png";
                Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_" + Type + FinalState2 + "Clone_test.png";
                Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_" + Type + FinalState2 + "Clone_test_rebined.png";

                PlotSaveName = SaveDir + sNameFlag + "03_" + Type + "_ratio.png";
            } else {
                if (Particle == "Electron") {
                    Numerator_Clone_SaveName = SaveDir + sNameFlag + "01_" + Particle + "_" + Type + FinalState1 + "Clone.png";
                    Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_" + Particle + "_" + Type + FinalState1 + "Clone_test.png";
                    Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_" + Particle + "_" + Type + FinalState1 + "Clone_test_rebined.png";

                    Denominator_Clone_SaveName = SaveDir + sNameFlag + "02_" + Particle + "_" + Type + FinalState2 + "Clone.png";
                    Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_" + Particle + "_" + Type + FinalState2 + "Clone_test.png";
                    Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_" + Particle + "_" + Type + FinalState2 + "Clone_test_rebined.png";

                    PlotSaveName = SaveDir + sNameFlag + "03_" + Particle + "_" + Type + "_ratio.png";
                } else {
                    if (DRegion == "FD") {
                        Numerator_Clone_SaveName = SaveDir + sNameFlag + "01_FD_Neutron_" + Type + FinalState1 + "Clone.png";
                        Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_FD_Neutron_" + Type + FinalState1 + "Clone_test.png";
                        Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_FD_Neutron_" + Type + FinalState1 + "Clone_test_rebined.png";

                        Denominator_Clone_SaveName = SaveDir + sNameFlag + "02_FD_Proton_" + Type + FinalState2 + "Clone.png";
                        Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_FD_Proton_" + Type + FinalState2 + "Clone_test.png";
                        Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_FD_Proton_" + Type + FinalState2 + "Clone_test_rebined.png";

                        PlotSaveName = SaveDir + sNameFlag + "03_FD_Nucleon_" + Type + "_ratio.png";
                    } else if (DRegion == "CD") {
                        Numerator_Clone_SaveName = SaveDir + sNameFlag + "01_CD_Proton_" + Type + FinalState1 + "Clone.png";
                        Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_CD_Proton_" + Type + FinalState1 + "Clone_test.png";
                        Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_CD_Proton_" + Type + FinalState1 + "Clone_test_rebined.png";

                        Denominator_Clone_SaveName = SaveDir + sNameFlag + "02_CD_Proton_" + Type + FinalState2 + "Clone.png";
                        Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_CD_Proton_" + Type + FinalState2 + "Clone_test.png";
                        Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_CD_Proton_" + Type + FinalState2 + "Clone_test_rebined.png";

                        PlotSaveName = SaveDir + sNameFlag + "03_CD_Nucleon_" + Type + "_ratio.png";
                    }
                }
            }
        } else {
            Numerator_Clone_SaveName = SaveDir + sNameFlag + "01_" + Type + FinalState1 + "Clone.png";
            Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_" + Type + FinalState1 + "Clone_test.png";
            Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_" + Type + FinalState1 + "Clone_test_rebined.png";

            Denominator_Clone_SaveName = SaveDir + sNameFlag + "02_" + Type + FinalState2 + "Clone.png";
            Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_" + Type + FinalState2 + "Clone_test.png";
            Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_" + Type + FinalState2 + "Clone_test_rebined.png";

            PlotSaveName = SaveDir + sNameFlag + "03_" + Type + "_ratio.png";
        }
    } else {  // for efficiency and acceptance correction plots
        Numerator_Clone_SaveName = SaveDir + sNameFlag + "01_" + Particle + "_" + Type + "_Rec_Clone.png";
        Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_" + Particle + "_" + Type + "_Rec_Clone_test.png";
        Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_" + Particle + "_" + Type + "_Rec_Clone_test_rebined.png";

        Denominator_Clone_SaveName = SaveDir + sNameFlag + "02_" + Particle + "_" + Type + "_Truth_Clone.png";
        Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_" + Particle + "_" + Type + "_Truth_Clone_test.png";
        Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_" + Particle + "_" + Type + "_Truth_Clone_test_rebined.png";

        PlotSaveName = SaveDir + sNameFlag + "03_" + Particle + "_" + Type + "_" + PlotsT + ".png";
    }
}

};  // namespace data_processor

#endif  // DATA_PROCESSOR_H
