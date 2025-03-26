//
// Created by Alon Sportes on 13/03/2025.
//

#ifndef LISTS_H
#define LISTS_H

#include <map>

#include "../constants/constants.h"
#include "../../../structures/DataSample/DataSample.h"

namespace lists {
// VariableNames map ----------------------------------------------------------------------------------------------------------------------------------------------------

std::map<std::string, std::map<std::string, std::string>> VariableNames = {
    {"P_e", {{"VarName", "P_e"}, {"VarLabel", "|#font[62]{P}_{e}|"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"theta_e", {{"VarName", "theta_e"}, {"VarLabel", "#theta_{e}"}, {"VarDim", "#left[#circ#right]"}}},
    {"phi_e", {{"VarName", "phi_e"}, {"VarLabel", "#phi_{e}"}, {"VarDim", "#left[#circ#right]"}}},
    //
    {"P_p", {{"VarName", "P_p"}, {"VarLabel", "|#font[62]{P}_{p}|"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"theta_p", {{"VarName", "theta_p"}, {"VarLabel", "#theta_{p}"}, {"VarDim", "#left[#circ#right]"}}},
    {"phi_p", {{"VarName", "phi_p"}, {"VarLabel", "#phi_{p}"}, {"VarDim", "#left[#circ#right]"}}},
    //
    {"P_n", {{"VarName", "P_n"}, {"VarLabel", "|#font[62]{P}_{n}|"}, {"VarDim", "#left[GeV/c##right]"}}},
    {"theta_n", {{"VarName", "theta_n"}, {"VarLabel", "#theta_{n}"}, {"VarDim", "#left[#circ#right]"}}},
    {"phi_n", {{"VarName", "phi_n"}, {"VarLabel", "#phi_{n}"}, {"VarDim", "#left[#circ#right]"}}},
    //
    {"P_pFD", {{"VarName", "P_pFD"}, {"VarLabel", "|#font[62]{P}_{pFD}|"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"theta_pFD", {{"VarName", "theta_pFD"}, {"VarLabel", "#theta_{pFD}"}, {"VarDim", "#left[#circ#right]"}}},
    {"phi_pFD", {{"VarName", "phi_pFD"}, {"VarLabel", "#phi_{pFD}"}, {"VarDim", "#left[#circ#right]"}}},
    //
    {"P_nFD", {{"VarName", "P_nFD"}, {"VarLabel", "|#font[62]{P}_{nFD}|"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"theta_nFD", {{"VarName", "theta_nFD"}, {"VarLabel", "#theta_{nFD}"}, {"VarDim", "#left[#circ#right]"}}},
    {"phi_nFD", {{"VarName", "phi_nFD"}, {"VarLabel", "#phi_{nFD}"}, {"VarDim", "#left[#circ#right]"}}},
    //
    {"P_pCD", {{"VarName", "P_pCD"}, {"VarLabel", "|#font[62]{P}_{pCD}|"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"theta_pCD", {{"VarName", "theta_pCD"}, {"VarLabel", "#theta_{pCD}"}, {"VarDim", "#left[#circ#right]"}}},
    {"phi_pCD", {{"VarName", "phi_pCD"}, {"VarLabel", "#phi_{pCD}"}, {"VarDim", "#left[#circ#right]"}}},
    //
    {"P_nCD", {{"VarName", "P_nCD"}, {"VarLabel", "|#font[62]{P}_{nCD}|"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"theta_nCD", {{"VarName", "theta_nCD"}, {"VarLabel", "#theta_{nCD}"}, {"VarDim", "#left[#circ#right]"}}},
    {"phi_nCD", {{"VarName", "phi_nCD"}, {"VarLabel", "#phi_{nCD}"}, {"VarDim", "#left[#circ#right]"}}},
    //
    {"momentum", {{"VarName", "momentum"}, {"VarLabel", "momentum"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"leading_nuc_momentum", {{"VarName", "leading_nuc_momentum"}, {"VarLabel", "|#font[62]{P}_{nucL}|"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"recoil_nuc_momentum", {{"VarName", "recoil_nuc_momentum"}, {"VarLabel", "|#font[62]{P}_{nucR}|"}, {"VarDim", "#left[GeV/c#right]"}}},
    //
    {"total_3momentum", {{"VarName", "total_3momentum"}, {"VarLabel", "Total 3-momentum"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"theta_tot", {{"VarName", "theta_tot"}, {"VarLabel", "#theta_{tot}"}, {"VarDim", "#left[#circ#right]"}}},
    {"phi_tot", {{"VarName", "phi_tot"}, {"VarLabel", "#phi_{tot}"}, {"VarDim", "#left[#circ#right]"}}},
    //
    {"relative_3momentum", {{"VarName", "relative_3momentum"}, {"VarLabel", "Relative 3-momentum"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"theta_rel", {{"VarName", "theta_rel"}, {"VarLabel", "#theta_{rel}"}, {"VarDim", "#left[#circ#right]"}}},
    {"phi_rel", {{"VarName", "phi_rel"}, {"VarLabel", "#phi_{rel}"}, {"VarDim", "#left[#circ#right]"}}},
    //
    {"total_4momentum", {{"VarName", "total_4momentum"}, {"VarLabel", "Total 4-momentum"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"relative_4momentum", {{"VarName", "relative_4momentum"}, {"VarLabel", "Relative 4-momentum"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"P_tot_minus_q", {{"VarName", "P_tot_minus_q"}, {"VarLabel", "|#font[62]{P}_{tot}-#font[62]{q}|"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"Opening_ang_P_nucL_minus_q_nucR",
     {{"VarName", "Opening_ang_P_nucL_minus_q_nucR"}, {"VarLabel", "#theta_{#font[62]{P}_{nucL}-#font[62]{q},#font[62]{P}_{nucR}}"}, {"VarDim", "#left[#circ#right]"}}},
    //
    {"P_tot_minus_q", {{"VarName", "P_tot_minus_q"}, {"VarLabel", "#vec{P}_{tot}-#vec{q}"}, {"VarDim", "#left[#circ#right]"}}},
    //
    {"q", {{"VarName", "q"}, {"VarLabel", "|#font[62]{q}|"}, {"VarDim", "#left[GeV/c#right]"}}},
    {"omega", {{"VarName", "omega"}, {"VarLabel", "#omega"}, {"VarDim", "#left[GeV#right]"}}},
    {"Q2", {{"VarName", "Q2"}, {"VarLabel", "Q^{2}"}, {"VarDim", "#left[GeV^{2}/c^{2}#right]"}}},
    {"xB", {{"VarName", "xB"}, {"VarLabel", "x_{B}"}, {"VarDim", ""}}},
    {"W", {{"VarName", "W"}, {"VarLabel", "W"}, {"VarDim", "#left[GeV/c^{2}#right]"}}},
    //
    {"tof", {{"VarName", "ToF"}, {"VarLabel", "t_{ToF}"}, {"VarDim", "#left[ns#right]"}}},
    //
    {"E_e", {{"VarName", "E_e"}, {"VarLabel", "E_{e}"}, {"VarDim", "#left[GeV#right]"}}},
    {"Ecal", {{"VarName", "Ecal"}, {"VarLabel", "E_{cal}"}, {"VarDim", "#left[GeV#right]"}}},
    {"Ecal_ext_1N", {{"VarName", "Ecal"}, {"VarLabel", "E_{cal} = E_{e} + T_{nuc}"}, {"VarDim", "#left[GeV#right]"}}},
    {"Ecal_ext_2N", {{"VarName", "Ecal"}, {"VarLabel", "E_{cal} = E_{e} + T_{nuc,1} + T_{nuc,2}"}, {"VarDim", "#left[GeV#right]"}}},
    //
    {"deltaP_T_tot", {{"VarName", "deltaP_T_tot"}, {"VarLabel", "#deltaP_{T,tot}"}, {"VarDim", ""}}},
    {"deltaP_T_L", {{"VarName", "deltaP_T_L"}, {"VarLabel", "#deltaP_{T,L}"}, {"VarDim", ""}}},
    {"deltaAlpha_T_tot", {{"VarName", "deltaAlpha_T_tot"}, {"VarLabel", "#delta#alpha_{T,tot}"}, {"VarDim", ""}}},
};

// HistogramLimits map --------------------------------------------------------------------------------------------------------------------------------------------------

std::map<std::string, std::map<std::string, double>> HistogramLimits;

// Histogram limits -----------------------------------------------------------------------------------------------------------------------------------------------------

// Histogram limits for analysis plots
// Default beam energy is 5.98636 GeV, which is changed in the analysis code
double beamE_def = 5.98636;  // Default beam energy

/* Nphe boundaries */
const double Nphe_boundary = 40;

/* Chi2 boundaries */
double Chi2_boundary = 20;
//    double Chi2_boundary = 30;

/* Vertex boundaries */
double Vertex_boundary = 20., Vertex_uboundary = Vertex_boundary, Vertex_lboundary = -Vertex_boundary;
double Vertex_boundary_FD = Vertex_boundary, Vertex_uboundary_FD = Vertex_uboundary, Vertex_lboundary_FD = Vertex_lboundary;
double Vertex_boundary_CD = Vertex_boundary, Vertex_uboundary_CD = Vertex_uboundary, Vertex_lboundary_CD = Vertex_lboundary;
double dV_boundary = 8., dV_uboundary = dV_boundary, dV_lboundary = -dV_boundary;
double dV_boundary_FD = dV_boundary, dV_uboundary_FD = dV_uboundary, dV_lboundary_FD = dV_lboundary;
double dV_boundary_CD = dV_boundary, dV_uboundary_CD = dV_uboundary, dV_lboundary_CD = dV_lboundary;

/* SF boundries */
const double SF_uboundary = 0.31, SF_lboundary = 0.16;

/* Momentum boundries */
const double Momentum_lboundary = 0., Momentum_uboundary = beamE_def * 1.1;                   // Default
double FDMomentum_lboundary = Momentum_lboundary, FDMomentum_uboundary = Momentum_uboundary;  // FD nucleons (1nFD, 1pFD, pFDpCD and nFDpCD)
const double CDMomentum_lboundary = 0., CDMomentum_uboundary = beamE_def / 2;                 // CD nucleons (pFDpCD & nFDpCD)
double P_nucFD_lboundary = 0., P_nucFD_uboundary = beamE_def * 1.1;                           // Default
double P_nucCD_lboundary = 0., P_nucCD_uboundary = beamE_def / 2;                             // CD nucleons (pFDpCD & nFDpCD)

/* W boundries */
const double W_lboundary = 0.35;
const double W_uboundary = 1.1 * sqrt((beamE_def + constants::m_p) * (beamE_def + constants::m_p) - beamE_def * beamE_def);  // Default

/* Beta boundries */
const double dBeta_sigma_boundary = 0.1;
const double Beta_dist_uboundary = 1 + dBeta_sigma_boundary, Beta_dist_lboundary = 1 - dBeta_sigma_boundary;

const double dBeta_sigma_ZOOMOUT_boundary = 0.1;
const double Beta_dist_ZOOMOUT_uboundary = 1 + dBeta_sigma_ZOOMOUT_boundary;
const double Beta_dist_ZOOMOUT_lboundary = 0.9;

double Beta_boundary_const = 3., Beta_boundary = 3., P_boundary = beamE_def * 1.425;

/* Angle boundries */
const double Theta_lboundary_FD = 0., Theta_uboundary_FD = 50.;
const double Theta_lboundary_CD = 30., Theta_uboundary_CD = 155.;
const double Opening_Ang_wide_lboundary = 0, Opening_Ang_wide_uboundary = 180.;
const double Opening_Ang_narrow_lboundary = 0, Opening_Ang_narrow_uboundary = 100.;
const double Phi_lboundary = -180., Phi_uboundary = 180.;

/* Momentum transfer boundries */
double Q2_lboundary_FD = 0., Q2_uboundary_FD = beamE_def * 1.1;

/* TKI boundries */
const double dP_T_boundary = 3.;

// Data runs (2 GeV) ----------------------------------------------------------------------------------------------------------------------------------------------------

DataSample H1_data_2GeV;
// DataSample H1_data_2GeV.SampleDir = "/lustre24/expphy/cache/clas12/rg-m/production/pass1/2gev/H/dst/recon";
// DataSample H1_data_2GeV.SampleDir = "/lustre24/expphy/cache/clas12/rg-m/production/pass1/2gev/H/dst/recon";

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/2gev/H/dst/recon
vector<string> H1_data_2GeV_runs = {"015550", "015552", "015555", "015560", "015562", "015628", "015631", "015633", "015635",
                                    "015551", "015554", "015556", "015561", "015563", "015629", "015632", "015634", "015636"};

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/2gev/D/dst/recon
vector<string> D2_data_2GeV_runs = {"015567", "015573", "015578", "015583", "015590", "015595", "015602", "015608", "015613", "015618", "015624", "015568", "015574", "015579",
                                    "015586", "015591", "015598", "015603", "015609", "015614", "015619", "015625", "015569", "015575", "015580", "015587", "015592", "015599",
                                    "015604", "015610", "015615", "015620", "015626", "015570", "015576", "015581", "015588", "015593", "015600", "015606", "015611", "015616",
                                    "015622", "015627", "015572", "015577", "015582", "015589", "015594", "015601", "015607", "015612", "015617", "015623"};

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/2gev/C/dst/recon
vector<string> C12_data_2GeV_runs = {"015644", "015646", "015650", "015652", "015654", "015656", "015658", "015660", "015662", "015664", "015666", "015668", "015670",
                                     "015645", "015647", "015651", "015653", "015655", "015657", "015659", "015661", "015663", "015665", "015667", "015669"};

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/2gev/Ar/dst/recon
vector<string> Ar40_data_2GeV_runs = {"015672", "015674", "015676", "015678", "015680", "015682", "015684", "015686", "015692", "015694", "015696", "015700",
                                      "015702", "015704", "015707", "015709", "015711", "015713", "015715", "015717", "015719", "015721", "015723", "015673",
                                      "015675", "015677", "015679", "015681", "015683", "015685", "015691", "015693", "015695", "015698", "015701", "015703",
                                      "015706", "015708", "015710", "015712", "015714", "015716", "015718", "015720", "015722", "015724"};

// Data runs (4 GeV) ----------------------------------------------------------------------------------------------------------------------------------------------------

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/4gev/C/dst/recon
vector<string> C12_data_4GeV_runs = {"015733", "015768", "015770", "015772", "015775", "015779", "015781", "015783",
                                     "015766", "015769", "015771", "015773", "015778", "015780", "015782", "015784"};

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon
vector<string> Ar40_data_4GeV_runs = {"015743", "015744", "015745", "015749", "015750", "015751", "015752", "015753", "015754", "015755", "015756", "015759", "015764"};

// Data runs (6 GeV) ----------------------------------------------------------------------------------------------------------------------------------------------------

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/6gev/H/dst/recon
vector<string> H1_data_6GeV_runs = {"015019", "015022", "015024", "015026", "015028", "015030", "015032", "015034", "015036",
                                    "015020", "015023", "015025", "015027", "015029", "015031", "015033", "015035"};

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/6gev/D/dst/recon
vector<string> D2_data_6GeV_runs = {"015045", "015051", "015056", "015061", "015072", "015078", "015094", "015099", "015104", "015437", "015444", "015450", "015456",
                                    "015046", "015052", "015057", "015062", "015073", "015079", "015095", "015100", "015105", "015439", "015445", "015451", "015047",
                                    "015053", "015058", "015065", "015074", "015081", "015096", "015101", "015106", "015441", "015447", "015452", "015049", "015054",
                                    "015059", "015066", "015075", "015082", "015097", "015102", "015435", "015442", "015448", "015454", "015050", "015055", "015060",
                                    "015067", "015077", "015093", "015098", "015103", "015436", "015443", "015449", "015455"};

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/6gev/Cx4/dst/recon
vector<string> C12x4_data_6GeV_runs = {"015186", "015187", "015188", "015189", "015190", "015191", "015192", "015193", "015194", "015196", "015199", "015200", "015202", "015203", "015204",
                                       "015205", "015206", "015207", "015210", "015212", "015213", "015214", "015215", "015217", "015219", "015220", "015221", "015223", "015224", "015225",
                                       "015226", "015228", "015234", "015235", "015236", "015238", "015239", "015240", "015241", "015242", "015243", "015245", "015246", "015247", "015248",
                                       "015249", "015250", "015252", "015253", "015254", "015255", "015257", "015258", "015259", "015260", "015261", "015262", "015263", "015264", "015265",
                                       "015266", "015269", "015270", "015271", "015272", "015273", "015274", "015275", "015278", "015279", "015280", "015282", "015283", "015284", "015286",
                                       "015287", "015288", "015289", "015290", "015291", "015292", "015293", "015294", "015295", "015296", "015298", "015300", "015301", "015302", "015303",
                                       "015304", "015305", "015306", "015307", "015308", "015309", "015310", "015311", "015312", "015313", "015314", "015316", "015317"};

// dir: /lustre24/expphy/cache/clas12/rg-m/production/pass1/6gev/Ar/dst/recon
vector<string> Ar40_data_6GeV_runs = {"015792", "015793", "015794", "015795", "015796", "015797", "015798", "015800", "015801", "015802"};

}  // namespace lists

#endif  // LISTS_H
