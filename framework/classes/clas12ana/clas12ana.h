#ifndef CLAS12ANA_HH
#define CLAS12ANA_HH

#include <TCanvas.h>  // My addition
#include <TF1.h>
#include <TFile.h>           // My addition
#include <TH1D.h>            // My addition
#include <TH2D.h>            // My addition
#include <TLorentzVector.h>  // My addition
#include <TVector3.h>        // My addition
#include <math.h>

#include <iostream>
#include <map>
#include <vector>

#include "../../namespaces/general_utilities/analysis_math/analysis_math_include.h"  // My addition
//
#include "clas12reader.h"
#include "region_particle.h"

using namespace std;
using namespace clas12;

// TODO: check which clas12ana version is this! It might be from the MSc analysis, and was altered for the next gen analysis

struct cutpar {
    std::string id;
    std::vector<double> par = {};  // pi- parameters
};

// My edit!
// // helper function for DC fiducials
// TVector3 rotate(TVector3 vec, int sector) {
//     double rot_ang = -(sector - 1) * 60 * TMath::DegToRad();

//     vec.RotateZ(rot_ang);

//     return vec;
// }

// #################################################################### //
// Analysis suite for CLAS12 analysis                                   //
// #################################################################### //

class clas12ana : public clas12reader {
   public:
    clas12ana() { Init(); };

    void Init();

    void InitSFEcalCuts();

    void InitSFPCuts();

    void readInputParam(const char *inFile);

    void readEcalPPar(const char *filename);

    void readEcalSFPar(const char *filename);

    void printParams();

    void InitDebugPlots();

    void WriteDebugPlots();

    void Clear();

    void Run(const std::unique_ptr<clas12::clas12reader> &c12);

    void plotDebug();

    void setdebug_fileName(TString db_fileName) { debug_fileName = db_fileName; }

    void set_hit_map_ref_fileName(TString db_fileName) { hit_map_ref_Out_filename = db_fileName; }

    double getSF(region_part_ptr p);

    //   void pidCuts(std::vector<region_part_ptr> &particles);
    //   void pidCuts(std::vector<std::vector<region_part_ptr>> &particles);

    void setEcalPCuts(bool flag = true) { f_ecalPCuts = flag; };  // option to have several cuts

    void setEcalSFCuts(bool flag = true) { f_ecalSFCuts = flag; };  // option to have several cuts

    void setDCEdgeCuts(bool flag = true) { f_DCEdgeCuts = flag; };

    void set_e_DCEdgeCuts(bool flag = true) { f_e_DCEdgeCuts = flag; };

    void setEcalEdgeCuts(bool flag = true) { f_ecalEdgeCuts = flag; };

    void setPidCuts(bool flag = true) { f_pidCuts = flag; };

    void set_e_VertexCuts(bool flag = true) { f_e_vertexCuts = flag; };

    void setVertexCuts(bool flag = true) { f_vertexCuts = flag; };

    void setVertexCorrCuts(bool flag = true) { f_corr_vertexCuts = flag; };

    TVector3 getCOM(TLorentzVector l, TLorentzVector r, TLorentzVector q);

    std::vector<region_part_ptr> getByPid(int pid) {
        if (pid == 11)
            return electrons;
        else if (pid == 2212)
            return protons;
        else if (pid == 2112)
            return neutrons;
        else if (pid == 45)
            return deuterons;
        else if (pid == 211)
            return piplus;
        else if (pid == -211)
            return piminus;
        else if (pid == 321)
            return kplus;
        else if (pid == -321)
            return kminus;
        else if (pid == 0)
            return neutrals;
        else
            return otherpart;
    }

    void setByPid(region_part_ptr p) {
        int pid = p->par()->getPid();
        if (pid == 11)
            electrons.push_back(p);
        else if (pid == 2212)
            protons.push_back(p);
        else if (pid == 2112)
            neutrons.push_back(p);
        else if (pid == 45)
            deuterons.push_back(p);
        else if (pid == 211)
            piplus.push_back(p);
        else if (pid == -211)
            piminus.push_back(p);
        else if (pid == 321)
            kplus.push_back(p);
        else if (pid == -321)
            kminus.push_back(p);
        else if (pid == 0)
            neutrals.push_back(p);
        else
            otherpart.push_back(p);
    }

    double getEventMult() { return event_mult; };

    void debugByPid(region_part_ptr p);

    bool EcalEdgeCuts(region_part_ptr p);

    bool checkEcalPCuts(region_part_ptr p);

    bool checkEcalSFCuts(region_part_ptr p);

    bool checkPidCut(region_part_ptr p);

    bool checkVertex(region_part_ptr p);

    bool checkVertexCorrelation(region_part_ptr el, region_part_ptr p);

    bool DCEdgeCuts(region_part_ptr p);

    void setVxcuts(double min, double max) {
        vertex_x_cuts.at(0) = min;
        vertex_x_cuts.at(1) = max;
    };

    void setVycuts(double min, double max) {
        vertex_y_cuts.at(0) = min;
        vertex_y_cuts.at(1) = max;
    };

    void setVzcuts(double min, double max) {
        vertex_z_cuts.at(0) = min;
        vertex_z_cuts.at(1) = max;
    };

    void setVzcutsFD(double min, double max) {
        // My addition!
        vertex_z_cuts_FD.at(0) = min;
        vertex_z_cuts_FD.at(1) = max;
    };

    void setVzcutsCD(double min, double max) {
        // My addition!
        vertex_z_cuts_CD.at(0) = min;
        vertex_z_cuts_CD.at(1) = max;
    };

    void setVertexCorrCutsLim(double min, double max) {
        vertex_corr_cuts.at(0) = min;
        vertex_corr_cuts.at(1) = max;
    };

    void setVertexCorrCutsLimFD(double min, double max) {
        // My addition!
        vertex_corr_cuts_FD.at(0) = min;
        vertex_corr_cuts_FD.at(1) = max;
    };

    void setVertexCorrCutsLimCD(double min, double max) {
        // My addition!
        vertex_corr_cuts_CD.at(0) = min;
        vertex_corr_cuts_CD.at(1) = max;
    };

    void fillDCdebug(region_part_ptr p, TH2D **h);

    void getLeadRecoilSRC(TLorentzVector beam, TLorentzVector target, TLorentzVector el);

    std::vector<region_part_ptr> getLeadSRC() { return lead_proton; };

    std::vector<region_part_ptr> getRecoilSRC() { return recoil_proton; };

    //  void getByChi2Pid(std::vector<region_part_ptr> &p,double mean, double sigma);
    std::vector<region_part_ptr> getByPid(std::vector<region_part_ptr> particles, int pid);
    //  std::vector<region_part_ptr> getByPidChi2(int pid, double chi2);

    //<editor-fold desc="My addition (methods)">
    double getEcalSFUpperCut() { return SF_max_cut; };  // My addition

    double getEcalSFLowerCut() { return SF_min_cut; };  // My addition

    double getDCEdgeCuts() { return dc_edge_cut; };  // My addition

    double getEcalEdgeCuts() { return ecal_edge_cut; };  // My addition

    void setNpheCuts(bool flag = true) { f_NpheCuts = flag; };  // My addition

    double getNpheCuts() { return htcc_Nphe_cut; };  // My addition

    double getNeutronMomentumCut() { return Neutron_Momentum_cut; };  // My addition

    double getNeutralBetaCut() { return FD_Neutral_Beta_cut; };  // My addition

    double getNeutralBetaCutMean() { return FD_Neutral_Beta_Mean; };  // My addition

    double getdPhiCutMean() {
        if (dPhi_p1_p2_Mean == 9999.) {
            return 0;
        } else {
            return dPhi_p1_p2_Mean;
        }
    };  // My addition

    std::vector<region_part_ptr> getParticles() { return allparticles; }  // My addition

    void addToAllParticles(region_part_ptr p) { allparticles.push_back(p); }  // My addition

    bool HTCCNpheCuts(region_part_ptr p);  // My addition

    double GetPidCutSigma(int Pid, std::string region) {
        // My addition?
        if (region == "CD") {
            auto itter_CD = pid_cuts_cd.find(Pid);

            return itter_CD->second.at(1);
        } else if (region == "FD") {
            auto itter_FD = pid_cuts_fd.find(Pid);

            return itter_FD->second.at(1);
        } else {
            // TODO: figure out what to do in this case
            return -9999;
        }
    }

    double GetPidCutMean(int Pid, std::string region) {
        // My addition?
        if (region == "CD") {
            auto itter_CD = pid_cuts_cd.find(Pid);

            return itter_CD->second.at(0);
        } else if (region == "FD") {
            auto itter_FD = pid_cuts_fd.find(Pid);

            return itter_FD->second.at(0);
        } else {
            // TODO: figure out what to do in this case
            return -9999;
        }
    }

    void FillECALHitMap(region_part_ptr p);  // My addition
                                             //</editor-fold>

   private:
    std::vector<region_part_ptr> electrons;
    std::vector<region_part_ptr> protons;
    std::vector<region_part_ptr> deuterons;
    std::vector<region_part_ptr> neutrals;
    std::vector<region_part_ptr> neutrons;
    std::vector<region_part_ptr> piplus;
    std::vector<region_part_ptr> piminus;
    std::vector<region_part_ptr> kplus;
    std::vector<region_part_ptr> kminus;
    std::vector<region_part_ptr> otherpart;

    // SRC
    std::vector<region_part_ptr> lead_proton;
    std::vector<region_part_ptr> recoil_proton;

    // prototype function for fitting ECAL electron cuts
    TF1 *ecal_p_fcn[2][7];         // 0 upper 1 lower fiducial
    TF1 *ecal_sf_fcn[2][7];        // 0 upper 1 lower fiducial
    double ecal_p_fcn_par[7][6];   // sector, parameter
    double ecal_sf_fcn_par[7][6];  // sector, parameter
    int sigma_cut = 3;
    // vector to hold constants of function for each particle type
    //   std::vector<cutpar> dc_cuts;
    //   std::vector<cutpar> ecal_cuts;

    // pid quality cuts e.g. {2212, {0.,2}} for a proton with chisq2pid mean = 0. and sigma = 2
    //   std::vector<cutpar> pid_cuts;
    // std::vector<cutpar> vertex_cuts;

    bool f_ecalSFCuts = false;
    bool f_ecalPCuts = false;
    bool f_ecalEdgeCuts = false;
    bool f_e_DCEdgeCuts = false;
    bool f_DCEdgeCuts = false;
    bool f_pidCuts = false;
    bool f_e_vertexCuts = false;
    bool f_vertexCuts = false;
    bool f_corr_vertexCuts = false;

    //  map<int,std::vector<double> > test_cuts;
    //  map<int,int> test_cuts;
    map<int, std::vector<double> > pid_cuts_cd;  // map<pid, {min,max cut}> Central Detector (CD)
    map<int, std::vector<double> > pid_cuts_fd;  // map<pid, {min,max cut}> Forward Detector (FD)

    std::vector<double> vertex_x_cuts = {-99, 99};
    std::vector<double> vertex_x_cuts_FD = {-99, 99};  // My addition!
    std::vector<double> vertex_x_cuts_CD = {-99, 99};  // My addition!
    std::vector<double> vertex_y_cuts = {-99, 99};
    std::vector<double> vertex_y_cuts_FD = {-99, 99};  // My addition!
    std::vector<double> vertex_y_cuts_CD = {-99, 99};  // My addition!
    std::vector<double> vertex_z_cuts = {-99, 99};
    std::vector<double> vertex_z_cuts_FD = {-99, 99};     // My addition!
    std::vector<double> vertex_z_cuts_CD = {-99, 99};     // My addition!
    map<string, std::vector<double> > vertex_cuts;        // map< x,y,z, {min,max}>
    std::vector<double> vertex_corr_cuts = {-99, 99};     // electron vertex <-> particle vertex correlation cuts
    std::vector<double> vertex_corr_cuts_FD = {-99, 99};  // electron vertex <-> particle vertex correlation cuts (FD only, my addition!)
    std::vector<double> vertex_corr_cuts_CD = {-99, 99};  // electron vertex <-> particle vertex correlation cuts (CD only, my addition!)

    double ecal_edge_cut = 14;
    //    double dc_edge_cut = 5; // Justin's original

    // SRC Cuts
    double q2_cut = 1.5;               // Q^2 cut
    double xb_cut = 1.2;               // x-borken
    double pmiss_cut = .2;             // missing momentum cut
    double recoil_mom_cut = .3;        // missing momentum cut
    double mmiss_cut[2] = {.84, 1.2};  // missing mass cut
    double pq_cut[2] = {0.6, 0.96};    //|p|/|q| cut
    double theta_pq_cut = 25;          // degrees angle between pLead & q

    // SRC variables
    double q2_e = 0;
    double xb_e = 0;
    double pmiss_e = 0;  // missing momentum
    double mmiss_e = 0;
    double pq_e = 0;
    double theta_pq_e = 0;

    //  map<int,std::vector<int> > pid_cuts_stats;
    //  map<string,std::vector<int> > vertex_cuts_stats;

    // constants
    double mass_proton = 0.938272;  // GeV/c2
    double mass_neutron = 0.939565;
    double mass_pion = 0.13957;
    double mass_deuterium = 1.8756;

    double event_mult = 0;  // charged particle multiplicity

    // debugging tools
    TString debug_fileName = "./debugOutputFile.root";
    TString hit_map_ref_Out_filename = "./hit_map_ref.root";
    bool debug_plots = true;
    TH1D *ecal_sf[7];  // ECAL sampling fraction
    TH1D *dc[4][7];    // DC hit map

    TH2D *sf_e_debug_b[7] = {nullptr};
    TH2D *sf_e_debug_a[7] = {nullptr};
    TH2D *sf_p_debug_b[7] = {nullptr};
    TH2D *sf_p_debug_a[7] = {nullptr};

    TH2D *pid_cd_debug = new TH2D("pid_cd_debug", "PID Uncut CD", 100, 0, 3, 100, 0, 1.2);
    TH2D *pid_fd_debug = new TH2D("pid_fd_debug", "PID Uncut FD", 100, 0, 5, 100, 0, 1.2);

    TH2D *sf_v_ecalIN_debug = new TH2D("sf_v_ecalIN_debug", "", 100, 0, 30, 100, 0, .4);
    TH2D *sf_w_ecalIN_debug = new TH2D("sf_w_ecalIN_debug", "", 100, 0, 30, 100, 0, .4);

    TH2D *sf_v_ecalOUT_debug = new TH2D("sf_v_ecalOUT_debug", "", 100, 0, 30, 100, 0, .4);
    TH2D *sf_w_ecalOUT_debug = new TH2D("sf_w_ecalOUT_debug", "", 100, 0, 30, 100, 0, .4);

    TH2D *sf_v_pcal_debug = new TH2D("sf_v_pcal_debug", "", 100, 0, 30, 100, 0, .4);
    TH2D *sf_w_pcal_debug = new TH2D("sf_w_pcal_debug", "", 100, 0, 30, 100, 0, .4);

    TH2D *sf_v_ecalIN_a_debug = new TH2D("sf_v_ecalIN_a_debug", "", 100, 0, 30, 100, 0, .4);
    TH2D *sf_w_ecalIN_a_debug = new TH2D("sf_w_ecalIN_a_debug", "", 100, 0, 30, 100, 0, .4);

    TH2D *sf_v_ecalOUT_a_debug = new TH2D("sf_v_ecalOUT_a_debug", "", 100, 0, 30, 100, 0, .4);
    TH2D *sf_w_ecalOUT_a_debug = new TH2D("sf_w_ecalOUT_a_debug", "", 100, 0, 30, 100, 0, .4);

    TH2D *sf_v_pcal_a_debug = new TH2D("sf_v_pcal_a_debug", "", 100, 0, 30, 100, 0, .4);
    TH2D *sf_w_pcal_a_debug = new TH2D("sf_w_pcal_a_debug", "", 100, 0, 30, 100, 0, .4);

    TH2D *pid_proton_fd_debug = new TH2D("pid_proton_fd_debug", "PID Cut Proton FD", 100, 0, 5, 100, 0, 1.2);
    TH2D *pid_proton_cd_debug = new TH2D("pid_proton_cd_debug", "PID Cut Proton CD", 100, 0, 5, 100, 0, 1.2);
    TH2D *pid_piplus_fd_debug = new TH2D("pid_piplus_fd_debug", "PID Cut #pi + FD", 100, 0, 5, 100, 0, 1.2);
    TH2D *pid_piplus_cd_debug = new TH2D("pid_piplus_cd_debug", "PID Cut #pi + CD", 100, 0, 5, 100, 0, 1.2);
    TH2D *pid_kplus_fd_debug = new TH2D("pid_kplus_fd_debug", "PID Cut K+ FD", 100, 0, 5, 100, 0, 1.2);
    TH2D *pid_kplus_cd_debug = new TH2D("pid_kplus_cd_debug", "PID Cut K+ CD", 100, 0, 5, 100, 0, 1.2);

    TH2D *pid_piminus_fd_debug = new TH2D("pid_piminus_fd_debug", "PID Cut #pi + FD", 100, 0, 5, 100, 0, 1.2);
    TH2D *pid_piminus_cd_debug = new TH2D("pid_piminus_cd_debug", "PID Cut #pi + CD", 100, 0, 5, 100, 0, 1.2);
    TH2D *pid_kminus_fd_debug = new TH2D("pid_kminus_fd_debug", "PID Cut K+ FD", 100, 0, 5, 100, 0, 1.2);
    TH2D *pid_kminus_cd_debug = new TH2D("pid_kminus_cd_debug", "PID Cut K+ CD", 100, 0, 5, 100, 0, 1.2);
    TH2D *pid_neutrals_fd_debug = new TH2D("pid_neutrals_fd_debug", "PID Cut neutrals FD", 100, 0, 5, 100, 0, 1.2);
    TH2D *pid_neutrals_cd_debug = new TH2D("pid_neutrals_cd_debug", "PID Cut neutrals CD", 100, 0, 5, 100, 0, 1.2);
    TH2D *pid_deuteron_fd_debug = new TH2D("pid_deuteron_fd_debug", "PID Cut deuteron FD", 100, 0, 5, 100, 0, 1.2);
    TH2D *pid_deuteron_cd_debug = new TH2D("pid_deuteron_cd_debug", "PID Cut deutereon CD", 100, 0, 5, 100, 0, 1.2);

    TH1D *el_vz_debug = new TH1D("el_vz_debug", "El vertex ", 100, -20, 10);
    TH1D *el_vz_p_debug = new TH1D("el_vz_p_debug", "El-proton vertex ", 100, -10, 10);

    TH2D *dc_hit_map_a[4];  // 3 regions
    TH2D *dc_hit_map_b[4];  // 3 regions

    TH2D *dc_hit_map_a_proton[4];  // 3 regions
    TH2D *dc_hit_map_b_proton[4];  // 3 regions

    TH2D *dc_hit_map_a_pion[4];  // 3 regions
    TH2D *dc_hit_map_b_pion[4];  // 3 regions

    //<editor-fold desc="My addition (attributes)">
    std::vector<region_part_ptr> allparticles;  // My addition

    bool f_NpheCuts = false;  // My addition

    double htcc_Nphe_cut = 2;             // My addition
    double Neutron_Momentum_cut = 9999.;  // My addition
    double FD_Neutral_Beta_cut = 9999.;   // My addition
    double FD_Neutral_Beta_Mean = 1.;     // My addition
    double dPhi_p1_p2_Mean = 9999.;       // My addition

    // ME: in the old version, SF cuts where:
    // TODO: CHECK WITH JUSTIN WHAT TO DO WITH THEM!
    double SF_max_cut = .28;
    double SF_min_cut = .2;

    double dc_edge_cut = 10;  // My addition (from Larry)

    //<editor-fold desc="my debugging - multiplicity plots before cuts (= BC) - no #e cuts">
    /* my debugging - multiplicity plots before cuts (= BC) - no #e cuts */
    //    TH2D *multi_p_vs_cpi_fd_BC_debug = new TH2D("multi_p_vs_cpi_fd_BC_debug",
    //                                                "#font[12]{#p} vs. #font[12]{##pi^{#pm}} BC (no #e cuts, FD);#font[12]{#p};#font[12]{##pi^{#pm}}", 10, 0, 10, 10, 0, 10);
    //    TH2D *multi_p_vs_cpi_cd_BC_debug = new TH2D("multi_p_vs_cpi_cd_BC_debug",
    //                                                "#font[12]{#p} vs. #font[12]{##pi^{#pm}} BC (no #e cuts, CD);#font[12]{#p};#font[12]{##pi^{#pm}}", 10, 0, 10, 10, 0, 10);
    TH2D *multi_p_vs_cpi_BC_debug =
        new TH2D("multi_p_vs_cpi_BC_debug", "#font[12]{#p} vs. #font[12]{##pi^{#pm}} BC (no #e cuts, CD & FD);#font[12]{#p};#font[12]{##pi^{#pm}}", 10, 0, 10, 10, 0, 10);
    //    TH1D *multi_p_fd_BC_debug = new TH1D("multi_p_fd_BC_debug", "#font[12]{#p} BC (no #e cuts, FD);#font[12]{#p}", 10, 0, 10);
    //    TH1D *multi_p_cd_BC_debug = new TH1D("multi_p_cd_BC_debug", "#font[12]{#p} BC (no #e cuts, CD);#font[12]{#p}", 10, 0, 10);
    TH1D *multi_p_BC_debug = new TH1D("multi_p_BC_debug", "#font[12]{#p} BC (no #e cuts, CD & FD);#font[12]{#p}", 10, 0, 10);
    //    TH1D *multi_cpi_fd_BC_debug = new TH1D("multi_cpi_fd_BC_debug", "#font[12]{##pi^{#pm}} BC (no #e cuts, FD);#font[12]{##pi^{#pm}}", 10, 0, 10);
    //    TH1D *multi_cpi_cd_BC_debug = new TH1D("multi_cpi_cd_BC_debug", "#font[12]{##pi^{#pm}} BC (no #e cuts, CD);#font[12]{##pi^{#pm}}", 10, 0, 10);
    TH1D *multi_cpi_BC_debug = new TH1D("multi_cpi_BC_debug", "#font[12]{##pi^{#pm}} BC (no #e cuts, CD & FD);#font[12]{##pi^{#pm}}", 10, 0, 10);
    //</editor-fold>

    //<editor-fold desc="my debugging - multiplicity plots after cuts (= AC) - no #e cuts">
    /* my debugging - multiplicity plots after cuts (= AC) - no #e cuts */
    //    TH2D *multi_p_vs_cpi_fd_AC_debug = new TH2D("multi_p_vs_cpi_fd_AC_debug",
    //                                                "#font[12]{#p} vs. #font[12]{##pi^{#pm}} AC (no #e cuts, FD);#font[12]{#p};#font[12]{##pi^{#pm}}", 10, 0, 10, 10, 0, 10);
    //    TH2D *multi_p_vs_cpi_cd_AC_debug = new TH2D("multi_p_vs_cpi_cd_AC_debug",
    //                                                "#font[12]{#p} vs. #font[12]{##pi^{#pm}} AC (no #e cuts, CD);#font[12]{#p};#font[12]{##pi^{#pm}}", 10, 0, 10, 10, 0, 10);
    TH2D *multi_p_vs_cpi_AC_debug =
        new TH2D("multi_p_vs_cpi_AC_debug", "#font[12]{#p} vs. #font[12]{##pi^{#pm}} AC (no #e cuts, CD & FD);#font[12]{#p};#font[12]{##pi^{#pm}}", 10, 0, 10, 10, 0, 10);
    //    TH1D *multi_p_fd_AC_debug = new TH1D("multi_p_fd_AC_debug", "#font[12]{#p} AC (no #e cuts, FD);#font[12]{#p}", 10, 0, 10);
    //    TH1D *multi_p_cd_AC_debug = new TH1D("multi_p_cd_AC_debug", "#font[12]{#p} AC (no #e cuts, CD);#font[12]{#p}", 10, 0, 10);
    TH1D *multi_p_AC_debug = new TH1D("multi_p_AC_debug", "#font[12]{#p} AC (no #e cuts, CD & FD);#font[12]{#p}", 10, 0, 10);
    //    TH1D *multi_cpi_fd_AC_debug = new TH1D("multi_cpi_fd_AC_debug", "#font[12]{##pi^{#pm}} AC (no #e cuts, FD);#font[12]{##pi^{#pm}}", 10, 0, 10);
    //    TH1D *multi_cpi_cd_AC_debug = new TH1D("multi_cpi_cd_AC_debug", "#font[12]{##pi^{#pm}} AC (no #e cuts, CD);#font[12]{##pi^{#pm}}", 10, 0, 10);
    TH1D *multi_cpi_AC_debug = new TH1D("multi_cpi_AC_debug", "#font[12]{##pi^{#pm}} AC (no #e cuts, CD & FD);#font[12]{##pi^{#pm}}", 10, 0, 10);
    //</editor-fold>

    //<editor-fold desc="my debugging - multiplicity plots before cuts (= BC) - 1e cut">
    /* my debugging - multiplicity plots before cuts (= BC) - 1e cut */
    //    TH2D *multi_p_vs_cpi_1e_cut_fd_BC_debug = new TH2D("multi_p_vs_cpi_1e_cut_fd_BC_debug",
    //                                                       "#font[12]{#p} vs. #font[12]{##pi^{#pm}} BC (1e cut, FD);#font[12]{#p};#font[12]{##pi^{#pm}}",
    //                                                       10, 0, 10, 10, 0, 10);
    //    TH2D *multi_p_vs_cpi_1e_cut_cd_BC_debug = new TH2D("multi_p_vs_cpi_1e_cut_cd_BC_debug",
    //                                                       "#font[12]{#p} vs. #font[12]{##pi^{#pm}} BC (1e cut, CD);#font[12]{#p};#font[12]{##pi^{#pm}}",
    //                                                       10, 0, 10, 10, 0, 10);
    TH2D *multi_p_vs_cpi_1e_cut_BC_debug =
        new TH2D("multi_p_vs_cpi_1e_cut_BC_debug", "#font[12]{#p} vs. #font[12]{##pi^{#pm}} BC (1e cut, CD & FD);#font[12]{#p};#font[12]{##pi^{#pm}}", 10, 0, 10, 10, 0, 10);
    //    TH1D *multi_p_1e_cut_fd_BC_debug = new TH1D("multi_p_1e_cut_fd_BC_debug", "#font[12]{#p} BC (1e cut, FD);#font[12]{#p}", 10, 0, 10);
    //    TH1D *multi_p_1e_cut_cd_BC_debug = new TH1D("multi_p_1e_cut_cd_BC_debug", "#font[12]{#p} BC (1e cut, CD);#font[12]{#p}", 10, 0, 10);
    TH1D *multi_p_1e_cut_BC_debug = new TH1D("multi_p_1e_cut_BC_debug", "#font[12]{#p} BC (1e cut, CD & FD);#font[12]{#p}", 10, 0, 10);
    //    TH1D *multi_cpi_1e_cut_fd_BC_debug = new TH1D("multi_cpi_1e_cut_fd_BC_debug", "#font[12]{##pi^{#pm}} BC (1e cut, FD);#font[12]{##pi^{#pm}}", 10, 0, 10);
    //    TH1D *multi_cpi_1e_cut_cd_BC_debug = new TH1D("multi_cpi_1e_cut_cd_BC_debug", "#font[12]{##pi^{#pm}} BC (1e cut, CD);#font[12]{##pi^{#pm}}", 10, 0, 10);
    TH1D *multi_cpi_1e_cut_BC_debug = new TH1D("multi_cpi_1e_cut_BC_debug", "#font[12]{##pi^{#pm}} BC (1e cut, CD & FD);#font[12]{##pi^{#pm}}", 10, 0, 10);
    //</editor-fold>

    //<editor-fold desc="my debugging - multiplicity plots after cuts (= AC) - 1e cut">
    /* my debugging - multiplicity plots after cuts (= AC) - 1e cut */
    //    TH2D *multi_p_vs_cpi_1e_cut_fd_AC_debug = new TH2D("multi_p_vs_cpi_1e_cut_fd_AC_debug",
    //                                                       "#font[12]{#p} vs. #font[12]{##pi^{#pm}} AC (1e cut, FD);#font[12]{#p};#font[12]{##pi^{#pm}}",
    //                                                       10, 0, 10, 10, 0, 10);
    //    TH2D *multi_p_vs_cpi_1e_cut_cd_AC_debug = new TH2D("multi_p_vs_cpi_1e_cut_cd_AC_debug",
    //                                                       "#font[12]{#p} vs. #font[12]{##pi^{#pm}} AC (1e cut, CD);#font[12]{#p};#font[12]{##pi^{#pm}}",
    //                                                       10, 0, 10, 10, 0, 10);
    TH2D *multi_p_vs_cpi_1e_cut_AC_debug =
        new TH2D("multi_p_vs_cpi_1e_cut_AC_debug", "#font[12]{#p} vs. #font[12]{##pi^{#pm}} AC (1e cut, CD & FD);#font[12]{#p};#font[12]{##pi^{#pm}}", 10, 0, 10, 10, 0, 10);
    //    TH1D *multi_p_1e_cut_fd_AC_debug = new TH1D("multi_p_1e_cut_fd_AC_debug", "#font[12]{#p} AC (1e cut, FD);#font[12]{#p}", 10, 0, 10);
    //    TH1D *multi_p_1e_cut_cd_AC_debug = new TH1D("multi_p_1e_cut_cd_AC_debug", "#font[12]{#p} AC (1e cut, CD);#font[12]{#p}", 10, 0, 10);
    TH1D *multi_p_1e_cut_AC_debug = new TH1D("multi_p_1e_cut_AC_debug", "#font[12]{#p} AC (1e cut, CD & FD);#font[12]{#p}", 10, 0, 10);
    //    TH1D *multi_cpi_1e_cut_fd_AC_debug = new TH1D("multi_cpi_1e_cut_fd_AC_debug", "#font[12]{##pi^{#pm}} AC (1e cut, FD);#font[12]{##pi^{#pm}}", 10, 0, 10);
    //    TH1D *multi_cpi_1e_cut_cd_AC_debug = new TH1D("multi_cpi_1e_cut_cd_AC_debug", "#font[12]{##pi^{#pm}} AC (1e cut, CD);#font[12]{##pi^{#pm}}", 10, 0, 10);
    TH1D *multi_cpi_1e_cut_AC_debug = new TH1D("multi_cpi_1e_cut_AC_debug", "#font[12]{##pi^{#pm}} AC (1e cut, CD & FD);#font[12]{##pi^{#pm}}", 10, 0, 10);
    //</editor-fold>

    //<editor-fold desc="Hit maps for efficiency plots (My addition)">

    //<editor-fold desc="Original">
    // TODO: delete these plots
    TH2D *hTheta_e_vs_Phi_e_fiducial_cuts_map_ECAL =
        new TH2D("Electron_fiducial_cuts_map_ECAL", "#theta_{e} vs. #phi_{e};#phi_{e} [Deg];#theta_{e} [Deg]", 250, -180, 180, 250, 0, 50);  // My addition

    TH2D *hTheta_p_vs_Phi_p_AMap_DC_NO_CUTS =
        new TH2D("Proton_AMap_DC_NO_CUTS", "#theta_{p} vs. #phi_{p} - NO DC edge cuts;#phi_{p} [Deg];#theta_{p} [Deg]", 250, -180, 180, 250, 0, 50);  // My addition
    TH2D *hTheta_p_vs_Phi_p_AMap_DC_WITH_CUTS =
        new TH2D("Proton_AMap_DC_WITH_CUTS", "#theta_{p} vs. #phi_{p} - WITH DC edge cuts;#phi_{p} [Deg];#theta_{p} [Deg]", 150, -180, 180, 150, 0, 50);  // My addition

    TH2D *hTheta_vs_Phi_hit_map_ECAL_no_fiducial_cuts =
        new TH2D("hit_map_ECAL_no_fiducial_cuts", "ECAL #theta vs. #phi - no fiducial cuts;#phi [Deg];#theta [Deg]", 250, -180, 180, 250, 0, 50);  // My addition
    TH2D *hTheta_vs_Phi_hit_map_ECAL_w_fiducial_cuts =
        new TH2D("hit_map_ECAL_w_fiducial_cuts", "ECAL #theta vs. #phi - with fiducial cuts;#phi [Deg];#theta [Deg]", 250, -180, 180, 250, 0, 50);  // My addition
    //</editor-fold>

    TH2D *hAng_hit_map_electrons = new TH2D("Electron_AMap", "Electron hit map with ECAL & DC fiducial cuts;#phi [Deg];#theta [Deg]", 250, -180, 180, 250, 0, 50);  // My addition
    TH2D *hAng_hit_map_protons = new TH2D("Protons_hit_map", "Protons hit map with DC fiducial cuts;#phi [Deg];#theta [Deg]", 250, -180, 180, 250, 0, 50);          // My addition
    TH2D *hAng_hit_map_neutrons = new TH2D("Neutrons_hit_map", "Neutrons hit map (no fiducial cuts);#phi [Deg];#theta [Deg]", 250, -180, 180, 250, 0, 50);          // My addition
    //</editor-fold>

    //</editor-fold>
};

#endif
