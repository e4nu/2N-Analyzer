//
// Created by Alon Sportes on 13/03/2025.
//

#ifndef DATA_FillHistogramByProcessOR_H
#define DATA_FillHistogramByProcessOR_H

#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "../utilities/utilities.h"

using namespace std;

namespace data_FillHistogramByProcessor {
using namespace utilities;
// GetFS function -------------------------------------------------------------------------------------------------------------------------------------------------------

string GetFS(const string &Source) {
    static const vector<pair<string, string>> patterns = {{"1e_cut", "1e cut"}, {"1e cut", "1e cut"}, {"1e Cut", "1e cut"}, {"(e,e')", "1e cut"}, {"1pFD", "1pFD"},
                                                          {"1p", "1p"},         {"1nFD", "1n"},       {"1LnFD", "1LnFD"},   {"1n", "1n"},         {"1n1p", "1n1p"},
                                                          {"1e2p", "1e2p"},     {"2p", "2p"},         {"pFDpCD", "pFDpCD"}, {"nFDpCD", "nFDpCD"}};

    for (const auto &pattern : patterns) {
        if (FindSubstring(Source, pattern.first)) { return pattern.second; }
    }

    return "";
}

// GetParticleName function -------------------------------------------------------------------------------------------------------------------------------------------

string GetParticleName(const string &Source, const bool &PluralParticles = false) {
    string ParticleName, FS = GetFS(Source);

    auto setParticleName = [&](const string &singular, const string &plural) { ParticleName = PluralParticles ? plural : singular; };

    static const vector<tuple<string, string, string>> patterns = {{"neutrals|Neutrals|neut.|Neut.", "Neut", "Neut"},
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
        if (FindSubstring(Source, get<0>(pattern))) {
            setParticleName(get<1>(pattern), get<2>(pattern));
            return ParticleName;
        }
    }

    ParticleName = "Unknown";
    return ParticleName;
}

// GetParticleName1 function ------------------------------------------------------------------------------------------------------------------------------------------

string GetParticleName1(const string &Source, const bool &PluralParticles) {
    static const vector<tuple<string, string, string>> patterns = {{"neutrals|Neutrals|neut.|Neut.", "Neut", "Neut"},
                                                                   {"Electron|electron", "Electron", "Electrons"},
                                                                   {"Proton|proton", "Proton", "Protons"},
                                                                   {"Neutron|neutron", "Neutron", "Neutrons"},
                                                                   {"#pi^{+}", "Piplus", "Piplus"},
                                                                   {"#pi^{-}", "Piminus", "Piminus"},
                                                                   {"K^{+}", "Kplus", "Kplus"},
                                                                   {"K^{-}", "Kminus", "Kminus"},
                                                                   {"D", "Deuteron", "Deuterons"}};

    for (const auto &pattern : patterns) {
        if (FindSubstring(Source, get<0>(pattern))) { return PluralParticles ? get<2>(pattern) : get<1>(pattern); }
    }

    return "Deuteron";
}

// GetParticleNameFromSubscript function --------------------------------------------------------------------------------------------------------------------------------

string GetParticleNameFromSubscript(const string &Source, const bool &PluralParticles) {
    static const vector<tuple<string, string, string>> patterns = {{"{e}", "Electron", "Electrons"},           {"{p}|{pFD}|{pCD}", "Proton", "Protons"},   {"{n}|{nFD}", "Neutron", "Neutrons"},
                                                                   {"{#pi^{+}}", "#pi^{+}", "Positive pions"}, {"{#pi^{-}}", "#pi^{-}", "Negative pions"}, {"{K^{+}}", "K^{+}", "Positive kaons"},
                                                                   {"{K^{-}}", "K^{-}", "Negative kaons"},     {"{D}", "Deuteron", "Deuterons"},           {"{#gamma}", "Photon", "Photons"}};

    for (const auto &pattern : patterns) {
        if (FindSubstring(Source, get<0>(pattern))) { return PluralParticles ? get<2>(pattern) : get<1>(pattern); }
    }

    return "Unknown";
}

// GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

string GetParticleNameLC(const string &Source, const bool &PluralParticles = false) {
    static const vector<tuple<string, string, string>> patterns = {{"neutrals|Neutrals|neut.|Neut.", "neut", "neut"},
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
        if (FindSubstring(Source, get<0>(pattern))) { return PluralParticles ? get<2>(pattern) : get<1>(pattern); }
    }

    return "unknown";
}

// GetParticleNameLCFromSubscript function --------------------------------------------------------------------------------------------------------------------------------

string GetParticleNameLCFromSubscript(const string &Source, const bool &PluralParticles) {
    static const vector<tuple<string, string, string>> patterns = {{"{e}", "electron", "electrons"},           {"{p}|{pFD}|{pCD}", "proton", "protons"},   {"{n}|{nFD}", "neutron", "neutrons"},
                                                                   {"{#pi^{+}}", "#pi^{+}", "positive pions"}, {"{#pi^{-}}", "#pi^{-}", "negative pions"}, {"{K^{+}}", "K^{+}", "positive kaons"},
                                                                   {"{K^{-}}", "K^{-}", "negative kaons"},     {"{D}", "deuteron", "deuterons"},           {"{#gamma}", "Photon", "Photons"}};

    for (const auto &pattern : patterns) {
        if (FindSubstring(Source, get<0>(pattern))) { return PluralParticles ? get<2>(pattern) : get<1>(pattern); }
    }

    return "Unknown";
}

// GetParticleNameShort function ----------------------------------------------------------------------------------------------------------------------------------------

string GetParticleNameShort(const string &Source) {
    static const vector<pair<string, string>> patterns = {{"neutrals|Neutrals|neut.|Neut.", "neut"},
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
        if (FindSubstring(Source, pattern.first)) { return pattern.second; }
    }

    return "";
}

// GetParticleNameShortFromSubscript function ---------------------------------------------------------------------------------------------------------------------------

string GetParticleNameShortFromSubscript(const string &Source) {
    static const vector<pair<string, string>> patterns = {{"{e}", "e"},         {"{p}", "p"},         {"{n}", "n"}, {"{#pi^{+}}", "#pi^{+}"}, {"{#pi^{-}}", "#pi^{-}"},
                                                          {"{K^{+}}", "K^{+}"}, {"{K^{-}}", "K^{-}"}, {"{D}", "D"}, {"{#gamma}", "#gamma"}};

    for (const auto &pattern : patterns) {
        if (FindSubstring(Source, pattern.first)) { return pattern.second; }
    }

    return "";
}

// GetTopology function -------------------------------------------------------------------------------------------------------------------------------------------------

string GetTopology(const string &Source) {
    static const vector<pair<string, string>> patterns = {{"1n", "1N"}, {"1p", "1N"}, {"1nFD", "1N"}, {"1pFD", "1N"}, {"1n1p", "2N"}, {"2p", "2N"}, {"nFDpCD", "2N"}, {"pFDpCD", "2N"}};

    for (const auto &pattern : patterns) {
        if (FindSubstring(Source, pattern.first)) { return pattern.second; }
    }

    return "";
}

// GetType function -----------------------------------------------------------------------------------------------------------------------------------------------------

string GetType(const string &Source) {
    static const vector<pair<string, string>> patterns = {{"momentum", "momentum"},
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
        if (FindSubstring(Source, pattern.first)) { return pattern.second; }
    }

    return "";
}

// GetDRegion function --------------------------------------------------------------------------------------------------------------------------------------------------

string GetDRegion(const string &Source, const bool &ReturnGoingRegion = false) {
    static const vector<pair<string, string>> patterns = {{", FD)", "FD"}, {"FD ", "FD"}, {"FD", "FD"}, {", CD)", "CD"}, {"CD ", "CD"}, {"CD", "CD"}};

    for (const auto &pattern : patterns) {
        if (FindSubstring(Source, pattern.first)) { return ReturnGoingRegion ? (pattern.second == "FD" ? "forward-going" : "central-going") : pattern.second; }
    }

    return "";
}

// GetDRegionExplicit function ------------------------------------------------------------------------------------------------------------------------------------------

string GetDRegionExplicit(const string &Source, const bool &ReturnGoingRegion) {
    static const vector<pair<string, string>> patterns = {{"FD", "FD"}, {"CD", "CD"}};

    for (const auto &pattern : patterns) {
        if (FindSubstring(Source, pattern.first)) { return ReturnGoingRegion ? (pattern.second == "FD" ? "forward-going" : "central-going") : pattern.second; }
    }

    return "";
}

// GetFSRTitle function -------------------------------------------------------------------------------------------------------------------------------------------------

string GetFSRTitle(const string &Source, const string &PlotsT) {
    string FSRTitle;
    string Particle = GetParticleName(Source), ParticleShort = GetParticleNameShort(Source), Type = GetType(Source), DRegion = GetDRegion(Source);

    if (PlotsT == "FSRatio") {
        if (!FindSubstring(Source, "vs")) {
            static const map<string, string> typeToTitle = {{"Q2", "Q^{2} ratio - "},
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
            static const map<string, string> vsTypeToTitle = {
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

// GetTitle function -----------------------------------------------------------------------------------------------------------------------------------------------------

string GetTitle(const string &Source) {
    string Title, Type = GetType(Source), ParticleShort = GetParticleNameShort(Source), Particle = GetParticleName(Source);

    if (Type == "momentum") {
        Title = Particle + " " + Type;
    } else {
        Title = "#" + Type + "_{" + ParticleShort + "}";
    }

    return Title;
}

// GetStatsTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

string GetStatsTitle(const string &Source) {
    string StatsTitle, StatsType;

    static const vector<pair<string, string>> patterns = {{"#theta", "#theta_{"}, {"#phi", "#phi_{"}};

    string FS = GetFS(Source), ParticleShort = GetParticleNameShort(Source);

    for (const auto &pattern : patterns) {
        if (FindSubstring(Source, pattern.first)) {
            StatsType = pattern.second + ParticleShort + "}";
            break;
        }
    }

    StatsTitle = StatsType + " (" + FS + ")";

    return StatsTitle;
}

// SetXLabel function ---------------------------------------------------------------------------------------------------------------------------------------------------

string SetXLabel(const string &Source) {
    static const vector<pair<string, string>> patterns = {{"momentum", "P_{"}, {"#theta", "#theta_{"}, {"#phi", "#phi_{"}};

    string ParticleShort = GetParticleNameShort(Source);

    for (const auto &pattern : patterns) {
        if (FindSubstring(Source, pattern.first)) { return pattern.second + ParticleShort + "} [" + (pattern.first == "momentum" ? "GeV/c" : "Deg") + "]"; }
    }

    return "";
}

// SetSaveDir function --------------------------------------------------------------------------------------------------------------------------------------------------

string SetSaveDir(const string &Source, const string &BaseSaveDir, const string &Mod) {
    string SaveDir, TestSaveDir;

    string Particle = GetParticleName(Source), ParticleLC = GetParticleNameLC(Source), FS = GetFS(Source), Type = GetType(Source);

    static const vector<pair<string, string>> patterns = {{"Electron|electron", "/00_" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/"},
                                                          {", FD)|FD " + Particle + "|FD " + ParticleLC, "/01_FD_" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/"},
                                                          {", CD)|CD " + Particle + "|CD " + ParticleLC, "/02_CD_" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/"}};

    for (const auto &pattern : patterns) {
        if (FindSubstring(Source, pattern.first)) {
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

}  // namespace data_FillHistogramByProcessor

#endif  // DATA_FillHistogramByProcessOR_H
