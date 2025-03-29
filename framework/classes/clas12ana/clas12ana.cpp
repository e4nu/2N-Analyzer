
#include "clas12ana.h"

// TODO: check which clas12ana version is this! It might be from the MSc analysis, and was altered for the next gen analysis

// #################################################################### //
// Analysis suite for CLAS12 analysis                                   //
// #################################################################### //

// Clear function -------------------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::Clear() {
    allparticles.clear();  // My addition
    //  particles.clear(); // Justin's original
    electrons.clear();
    protons.clear();
    deuterons.clear();
    neutrals.clear();
    neutrons.clear();
    piplus.clear();
    piminus.clear();
    kplus.clear();
    kminus.clear();
    otherpart.clear();

    lead_proton.clear();
    recoil_proton.clear();

    event_mult = 0;
}

// Run function ---------------------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::Run(const std::unique_ptr<clas12::clas12reader> &c12) {
    Clear();

    auto particles = c12->getDetParticles();  // particles is now a std::vector of particles for this event
    auto electrons_det = c12->getByID(11);

    //<editor-fold desc="My addition">
    auto protons_det = c12->getByID(2212);
    auto piplus_det = c12->getByID(211);
    auto piminus_det = c12->getByID(-211);

    //<editor-fold desc="Filling multiplicity plots before cuts (BC) - no #e cuts">
    multi_p_BC_debug->Fill(protons_det.size());
    multi_cpi_BC_debug->Fill(piplus_det.size() + piminus_det.size());
    multi_p_vs_cpi_BC_debug->Fill(protons_det.size(), piplus_det.size() + piminus_det.size());
    //</editor-fold>

    //<editor-fold desc="Filling multiplicity plots before cuts (BC) - 1e cut">
    if (electrons_det.size() == 1) {
        multi_p_1e_cut_BC_debug->Fill(protons_det.size());
        multi_cpi_1e_cut_BC_debug->Fill(piplus_det.size() + piminus_det.size());
        multi_p_vs_cpi_1e_cut_BC_debug->Fill(protons_det.size(), piplus_det.size() + piminus_det.size());
    }
    //</editor-fold>

    int nf_initial = particles.size();

    //<editor-fold desc="Debugging print">
    //    auto protons_det = c12->getByID(2212);
    //    auto deuterons_det = c12->getByID(45);
    //    auto piplus_det = c12->getByID(211);
    //    auto piminus_det = c12->getByID(-211);
    //    auto kplus_det = c12->getByID(321);
    //    auto kminus_det = c12->getByID(-321);
    //    auto z_det = c12->getByID(0);
    //    auto n_det = c12->getByID(2112);
    //</editor-fold>

    //</editor-fold>

    /* ME: for any number of electrons */
    for (auto el = electrons_det.begin(); el != electrons_det.end();) {
        int sector = (*el)->getSector();
        double el_mom = (*el)->getP();
        double el_sf = getSF(*el);
        double el_pcal_energy = (*el)->cal(PCAL)->getEnergy();

        double ecin_v = (*el)->cal(ECIN)->getLv();
        double ecin_w = (*el)->cal(ECIN)->getLw();
        double ecout_v = (*el)->cal(ECOUT)->getLv();
        double ecout_w = (*el)->cal(ECOUT)->getLw();
        double pcal_v = (*el)->cal(PCAL)->getLv();
        double pcal_w = (*el)->cal(PCAL)->getLw();

        // DEBUG plots
        if (debug_plots) {
            sf_v_ecalIN_debug->Fill(ecin_v, el_sf);
            sf_w_ecalIN_debug->Fill(ecin_w, el_sf);
            sf_v_ecalOUT_debug->Fill(ecout_v, el_sf);
            sf_w_ecalOUT_debug->Fill(ecout_w, el_sf);
            sf_v_pcal_debug->Fill(pcal_v, el_sf);
            sf_w_pcal_debug->Fill(pcal_w, el_sf);

            if (sector <= 6 && sector >= 1) {
                sf_e_debug_b[sector]->Fill(el_pcal_energy, el_sf);
                sf_p_debug_b[sector]->Fill(el_mom, el_sf);
            }

            fillDCdebug(*el, dc_hit_map_b);
        }

        /*
                //HTCC photo electron cuts (Justin's original)
                if ((*el)->che(HTCC)->getNphe() <= 2)
                    el = electrons_det.erase(el);
        */

        if (!checkEcalSFCuts(*el) && f_ecalSFCuts) {
            // ECAL SF cuts
            el = electrons_det.erase(el);
        } else if (!checkEcalPCuts(*el) && f_ecalPCuts) {
            // ECAL SF cuts
            el = electrons_det.erase(el);
        } else if (!EcalEdgeCuts(*el) && f_ecalEdgeCuts) {
            // ECAL edge cuts
            el = electrons_det.erase(el);
        } else if (!HTCCNpheCuts(*el) && f_NpheCuts) {
            // HTCC Nphe cuts (my addition)
            el = electrons_det.erase(el);
        } else if (!checkVertex(*el) && f_e_vertexCuts)
        // else if (!checkVertex(*el) && f_vertexCuts)
        {
            // Vertex cut
            el = electrons_det.erase(el);
        } else if (!DCEdgeCuts(*el) && f_e_DCEdgeCuts)
        // else if (!DCEdgeCuts(*el) && f_DCEdgeCuts)
        {
            // DC edge cut
            el = electrons_det.erase(el);
        } else {
            // DEBUG plots
            if (debug_plots &&
                sector

                    <= 6 &&
                sector >= 1) {
                sf_e_debug_a[sector]->Fill((*el)->cal(PCAL)->getEnergy(), el_sf);
                sf_p_debug_a[sector]->Fill(el_mom, el_sf);
            }

            el_vz_debug->Fill((*el)->par()->getVz());

            sf_v_ecalIN_a_debug->Fill(ecin_v, el_sf);
            sf_w_ecalIN_a_debug->Fill(ecin_w, el_sf);
            sf_v_ecalOUT_a_debug->Fill(ecout_v, el_sf);
            sf_w_ecalOUT_a_debug->Fill(ecout_w, el_sf);
            sf_v_pcal_a_debug->Fill(pcal_v, el_sf);
            sf_w_pcal_a_debug->Fill(pcal_w, el_sf);

            ++el;  // itterate
        }
    }  // end of loop over electrons_det vector

    if (electrons_det.size() == 1) {
        // good trigger electron

        //        //<editor-fold desc="Debugging print - START">
        //        int op = particles.size() - electrons_det.size() - protons_det.size() - deuterons_det.size() - piplus_det.size() - piminus_det.size() - kplus_det.size() -
        //                 kminus_det.size() - (z_det.size() + n_det.size());
        //
        //        std::cout << "#particles in event (START):\t" << particles.size() << "\n";
        //        std::cout << "electrons.size() = " << electrons_det.size() << "\n";
        //        std::cout << "protons_det.size() = " << protons_det.size() << "\n";
        //        std::cout << "deuterons_det.size() = " << deuterons_det.size() << "\n";
        //        std::cout << "piplus_det.size() = " << piplus_det.size() << "\n";
        //        std::cout << "piminus_det.size() = " << piminus_det.size() << "\n";
        //        std::cout << "kplus_det.size() = " << kplus_det.size() << "\n";
        //        std::cout << "kminus_det.size() = " << kminus_det.size() << "\n";
        //        std::cout << "neutrals_det.size() = " << z_det.size() + n_det.size() << "\n";
        //        std::cout << "otherpart.size() = " << op << "\n\n";
        //        //</editor-fold>

        //       setByPid(electrons_det[0]); //set good trigger electron

        if (debug_plots) {
            fillDCdebug(electrons_det[0], dc_hit_map_a);  // electron DC hit debug maps
            hTheta_e_vs_Phi_e_fiducial_cuts_map_ECAL->Fill(electrons_det[0]->getPhi() * 180.0 / analysis_math::pi, electrons_det[0]->getTheta() * 180.0 / analysis_math::pi);
            // My addition
            hAng_hit_map_electrons->Fill(electrons_det[0]->getPhi() * 180.0 / analysis_math::pi, electrons_det[0]->getTheta() * 180.0 / analysis_math::pi);  // My addition
        }

        // DON'T FORGET TO ADD ++p ITTERATOR in this loop, it's not added in the for statement for a reason
        for (auto p = particles.begin(); p != particles.end();) {
            if (debug_plots) {
                FillECALHitMap(*p);  // My addition

                if ((*p)->par()->getPid() == 2212) fillDCdebug(*p, dc_hit_map_b_proton);
                if ((*p)->par()->getPid() == 211) fillDCdebug(*p, dc_hit_map_b_pion);
            }

            // neutrals and electrons don't follow cuts below, skip them
            if ((*p)->par()->getCharge() == 0 || (*p)->par()->getPid() == 11) {
                setByPid(*p);
                addToAllParticles(*p);  // add neutrals and electrons to allparticles (My addition)
                ++p;                    // itterate
                continue;
                // ME: the continue (line above) will skip the rest of the cuts. Apparently, it was added here to allow the log of event_mult (recheck!)
            } else {
                if ((*p)->par()->getPid() != 11) event_mult++;
            }

            double par_mom = (*p)->par()->getP();
            double par_beta = (*p)->par()->getBeta();

            bool is_cd = ((*p)->getRegion() == CD);
            bool is_fd = ((*p)->getRegion() == FD);

            // DEBUG plots
            if (debug_plots && ((*p)->par()->getCharge() >= 1) && ((*p)->par()->getPid() != 11)) {
                if (is_cd) pid_cd_debug->Fill(par_mom, par_beta);
                if (is_fd) pid_fd_debug->Fill(par_mom, par_beta);
            }

            //	   bool pid_cut    = checkPidCut(*p);
            //	   bool vertex_cut = checkVertex(*p);
            //	   bool vertex_corr_cut = checkVertexCorrelation(electrons_det[0],*p); //correlation between good electron and particles vertex
            //	   bool dc_edge_cut     = DCEdgeCuts(*p);

            if (!checkPidCut(*p) && f_pidCuts) {
                // PID cuts
                p = particles.erase(p);
            } else if (!checkVertex(*p) && f_vertexCuts) {
                // Vertex cut
                p = particles.erase(p);
            } else if (!DCEdgeCuts(*p) && f_DCEdgeCuts) {
                // DC edge cut
                p = particles.erase(p);
            } else if (!checkVertexCorrelation(electrons_det[0], *p) && f_corr_vertexCuts) {
                // Vertex correlation cut between electron
                p = particles.erase(p);
            } else {
                // itterate
                setByPid(*p);
                addToAllParticles(*p);  // add all particles surviving the cuts in event to allparticles (My addition)

                if (debug_plots) {
                    if ((*p)->par()->getPid() == 2212 && (*p)->getRegion() == FD) {
                        // My addition!!
                        hTheta_p_vs_Phi_p_AMap_DC_WITH_CUTS->Fill((*p)->getPhi() * 180.0 / analysis_math::pi, (*p)->getTheta() * 180.0 / analysis_math::pi);
                        hAng_hit_map_protons->Fill((*p)->getPhi() * 180.0 / analysis_math::pi, (*p)->getTheta() * 180.0 / analysis_math::pi);
                    }

                    if ((*p)->par()->getCharge() != 0 && (*p)->par()->getPid() != 11) el_vz_p_debug->Fill((*p)->par()->getVz() - electrons_det[0]->par()->getVz());

                    debugByPid(*p);
                    if ((*p)->par()->getPid() == 2212) fillDCdebug(*p, dc_hit_map_a_proton);
                    if ((*p)->par()->getPid() == 211) fillDCdebug(*p, dc_hit_map_a_pion);
                }

                ++p;
            }
        }  // particle loop

        //        //<editor-fold desc="Debugging print - END">
        //        std::cout << "#particles in event (END):\t" << electrons.size() + protons.size() + deuterons.size() + piplus.size() + piminus.size() + kplus.size() + kminus.size() +
        //                                                  neutrals.size() + otherpart.size() << "\n";
        //        std::cout << "electrons.size() = " << electrons.size() << "\n";
        //        std::cout << "protons_det.size() = " << protons.size() << "\n";
        //        std::cout << "deuterons_det.size() = " << deuterons.size() << "\n";
        //        std::cout << "piplus_det.size() = " << piplus.size() << "\n";
        //        std::cout << "piminus_det.size() = " << piminus.size() << "\n";
        //        std::cout << "kplus_det.size() = " << kplus.size() << "\n";
        //        std::cout << "kminus_det.size() = " << kminus.size() << "\n";
        //        std::cout << "neutrals_det.size() = " << neutrals.size() << "\n";
        //        std::cout << "otherpart.size() = " << otherpart.size() << "\n\n\n\n";
        //        //</editor-fold>

        //<editor-fold desc="Filling multiplicity plots after cuts (AC) - 1e cut">
        multi_p_1e_cut_AC_debug->Fill(protons.size());
        multi_cpi_1e_cut_AC_debug->Fill(piplus.size() + piminus.size());
        multi_p_vs_cpi_1e_cut_AC_debug->Fill(protons.size(), piplus.size() + piminus.size());
        //</editor-fold>
    }  // good electron loop
}

// fillDCdebug function -------------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::fillDCdebug(region_part_ptr p, TH2D **h) {
    //  if(p->par()->getPid() == 11)
    //    {
    h[1]->Fill(p->traj(DC, 6)->getX(), p->traj(DC, 6)->getY());
    h[2]->Fill(p->traj(DC, 18)->getX(), p->traj(DC, 18)->getY());
    h[3]->Fill(p->traj(DC, 36)->getX(), p->traj(DC, 36)->getY());
    //    }
}

// plotDebug function ---------------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::plotDebug() {
    TCanvas *c1 = new TCanvas("c1", "c1", 1000, 2000);
    c1->Divide(2, 6);

    for (int i = 1; i <= 6; i++) {
        c1->cd(i);
        sf_p_debug_b[i]->Draw("colz");
    }

    for (int i = 7; i <= 12; i++) {
        c1->cd(i);
        sf_p_debug_a[i]->Draw("colz");
    }
}

// InitSFEcalCuts function ----------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::InitSFEcalCuts() {
    // ME: used to be InitCuts
    std::cout << "PARAMETERS for SF vs Ecal cuts" << endl;
    for (int i = 1; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            std::cout << "sector " << i << " j " << j << " par " << ecal_sf_fcn_par[i][j] << endl;
            ecal_sf_fcn[0][i]->SetParameter(j, ecal_sf_fcn_par[i][j]);
            ecal_sf_fcn[1][i]->SetParameter(j, ecal_sf_fcn_par[i][j]);
        }

        ecal_sf_fcn[0][i]->SetParameter(6, sigma_cut);
        ecal_sf_fcn[1][i]->SetParameter(6, sigma_cut);
    }
}

// InitSFPCuts function -------------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::InitSFPCuts() {
    // ME: used to be InitCuts
    std::cout << "PARAMETERS for SF vs P cuts" << endl;
    for (int i = 1; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            std::cout << "sector " << i << " j " << j << " par " << ecal_p_fcn_par[i][j] << endl;
            ecal_p_fcn[0][i]->SetParameter(j, ecal_p_fcn_par[i][j]);
            ecal_p_fcn[1][i]->SetParameter(j, ecal_p_fcn_par[i][j]);
        }

        ecal_p_fcn[0][i]->SetParameter(6, sigma_cut);
        ecal_p_fcn[1][i]->SetParameter(6, sigma_cut);
    }
}

// Init function --------------------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::Init() {
    for (int i = 0; i < 7; i++) {
        ecal_sf_fcn[0][i] = new TF1(Form("ecal_sf_fcn_0_%d", i), "[0] + [1]/x + [2]/pow(x,2) - [6]*( [3] + [4]/x + [5]/pow(x,2))", 0, 1.5);
        ecal_sf_fcn[1][i] = new TF1(Form("ecal_sf_fcn_1_%d", i), "[0] + [1]/x + [2]/pow(x,2) + [6]*( [3] + [4]/x + [5]/pow(x,2))", 0, 1.5);

        ecal_p_fcn[0][i] = new TF1(Form("ecal_p_fcn_0_%d", i), "[0] + [1]/x + [2]/pow(x,2) - [6]*( [3] + [4]/x + [5]/pow(x,2))", 0, 10);
        ecal_p_fcn[1][i] = new TF1(Form("ecal_p_fcn_1_%d", i), "[0] + [1]/x + [2]/pow(x,2) + [6]*( [3] + [4]/x + [5]/pow(x,2))", 0, 10);
    }

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            if (j == 3) {
                ecal_sf_fcn_par[i][j] = 9999;
                ecal_p_fcn_par[i][j] = 9999;
            }

            ecal_sf_fcn_par[i][j] = 0;
            ecal_p_fcn_par[i][j] = 0;
        }
    }

    if (debug_plots) InitDebugPlots();
}

// DCEdgeCuts function --------------------------------------------------------------------------------------------------------------------------------------------------

bool clas12ana::DCEdgeCuts(region_part_ptr p) {
    // true if inside cut
    // cut all charged particles
    if (p->par()->getCharge() != 0) {
        auto traj_index_1 = p->traj(DC, 6)->getIndex();   // layer 1
        auto traj_index_2 = p->traj(DC, 18)->getIndex();  // layer 2
        auto traj_index_3 = p->traj(DC, 36)->getIndex();  // layer 3

        auto traj_edge_1 = p->traj(DC, 6)->getFloat("edge", traj_index_1);
        auto traj_edge_2 = p->traj(DC, 18)->getFloat("edge", traj_index_2);
        auto traj_edge_3 = p->traj(DC, 36)->getFloat("edge", traj_index_3);

        if (traj_edge_1 > dc_edge_cut && traj_edge_2 > dc_edge_cut && traj_edge_3 > dc_edge_cut)
            return true;
        else
            return false;
    } else
        return true;
}

// EcalEdgeCuts function ------------------------------------------------------------------------------------------------------------------------------------------------

bool clas12ana::EcalEdgeCuts(region_part_ptr p) {
    // true if inside cut
    double sampling_frac = getSF(p);

    if (p->par()->getPid() == 11) {
        if (p->cal(clas12::PCAL)->getLv() > ecal_edge_cut && p->cal(clas12::PCAL)->getLw() > ecal_edge_cut)
            return true;
        else
            return false;
    } else
        return true;
}

// HTCCNpheCuts function ------------------------------------------------------------------------------------------------------------------------------------------------

bool clas12ana::HTCCNpheCuts(region_part_ptr p) {
    // My addition
    // true if inside cut
    double Nphe = p->che(HTCC)->getNphe();

    if (p->par()->getPid() == 11) {
        if (Nphe > htcc_Nphe_cut) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

// checkEcalSFCuts function ---------------------------------------------------------------------------------------------------------------------------------------------

bool clas12ana::checkEcalSFCuts(region_part_ptr p) {
    // ME: used to be checkEcalCuts
    // true if inside cut

    //    //<editor-fold desc="Justin's original">
    //    if (p->par()->getPid() == 11) {
    //        double sampling_frac = getSF(p);
    //        double energy = p->cal(PCAL)->getEnergy();
    //        //      double energy =  p->cal(ECIN)->getEnergy() +  p->cal(ECOUT)->getEnergy();
    //
    //        int sector = p->getSector();
    //
    //        //Turn on for functional form
    //        double sf_max_cut = ecal_sf_fcn[1][sector]->Eval(energy);
    //        double sf_min_cut = ecal_sf_fcn[0][sector]->Eval(energy);
    //        //      std::cout<<"sf cut "<<sf_max_cut<<" "<<sf_min_cut<< " "<< sampling_frac <<" mom "<<p->par()->getP()<<endl;
    //        //      std::cout<<ecal_fcn[0][sector]->GetParameter(0)<<" "<<ecal_fcn[0][sector]->GetParameter(1)<<" "<<ecal_fcn[0][sector]->GetParameter(2)<<" sector "<<sector<<endl;
    //
    //        //      double sf_max_cut = .28;
    //        //      double sf_min_cut = .2;
    //
    //        if (sampling_frac < sf_max_cut && sampling_frac > sf_min_cut)
    //            return true;
    //        else
    //            return false;
    //    } else
    //        return false;
    //    //</editor-fold>

    //<editor-fold desc="My addition">
    if (p->par()->getPid() == 11) {
        double sampling_frac = getSF(p);

        if (sampling_frac < SF_max_cut && sampling_frac > SF_min_cut)
            return true;
        else
            return false;
    } else
        return false;
    //</editor-fold>
}

// checkEcalPCuts function ----------------------------------------------------------------------------------------------------------------------------------------------

bool clas12ana::checkEcalPCuts(region_part_ptr p) {
    // true if inside cut

    if (p->par()->getPid() == 11) {
        double sampling_frac = getSF(p);
        int sector = p->getSector();

        // Turn on for functional form
        double sf_max_cut = ecal_p_fcn[1][sector]->Eval(p->par()->getP());
        double sf_min_cut = ecal_p_fcn[0][sector]->Eval(p->par()->getP());
        //      std::cout<<"sf cut "<<sf_max_cut<<" "<<sf_min_cut<< " "<< sampling_frac <<" mom "<<p->par()->getP()<<endl;
        //      std::cout<<ecal_fcn[0][sector]->GetParameter(0)<<" "<<ecal_fcn[0][sector]->GetParameter(1)<<" "<<ecal_fcn[0][sector]->GetParameter(2)<<" sector "<<sector<<endl;

        //      double sf_max_cut = .28;
        //      double sf_min_cut = .2;

        if (sampling_frac < sf_max_cut && sampling_frac > sf_min_cut)
            return true;
        else
            return false;
    } else
        return false;
}

// getSF function -------------------------------------------------------------------------------------------------------------------------------------------------------

double clas12ana::getSF(region_part_ptr p) {
    if (p->par()->getPid() == 11)
        return (p->cal(clas12::PCAL)->getEnergy() + p->cal(clas12::ECIN)->getEnergy() + p->cal(clas12::ECOUT)->getEnergy()) / p->par()->getP();
    else
        return -9999.;
}

// checkVertex function -------------------------------------------------------------------------------------------------------------------------------------------------

bool clas12ana::checkVertex(region_part_ptr p) {
    // true if inside cut
    if (p->getRegion() == FD) {
        // My addition!
        return ((p->par()->getVx() > vertex_x_cuts_FD.at(0) && p->par()->getVx() < vertex_x_cuts_FD.at(1)) &&
                (p->par()->getVy() > vertex_y_cuts_FD.at(0) && p->par()->getVy() < vertex_y_cuts_FD.at(1)) &&
                (p->par()->getVz() > vertex_z_cuts_FD.at(0) && p->par()->getVz() < vertex_z_cuts_FD.at(1)));
    } else if (p->getRegion() == CD) {
        return ((p->par()->getVx() > vertex_x_cuts_CD.at(0) && p->par()->getVx() < vertex_x_cuts_CD.at(1)) &&
                (p->par()->getVy() > vertex_y_cuts_CD.at(0) && p->par()->getVy() < vertex_y_cuts_CD.at(1)) &&
                (p->par()->getVz() > vertex_z_cuts_CD.at(0) && p->par()->getVz() < vertex_z_cuts_CD.at(1)));
    } else {
        return ((p->par()->getVx() > vertex_x_cuts.at(0) && p->par()->getVx() < vertex_x_cuts.at(1)) &&
                (p->par()->getVy() > vertex_y_cuts.at(0) && p->par()->getVy() < vertex_y_cuts.at(1)) && (p->par()->getVz() > vertex_z_cuts.at(0) && p->par()->getVz() < vertex_z_cuts.at(1)));
    }

    //    //<editor-fold desc="Justin's original">
    //    //true if inside cut
    //    return ((p->par()->getVx() > vertex_x_cuts.at(0) && p->par()->getVx() < vertex_x_cuts.at(1))
    //            && (p->par()->getVy() > vertex_y_cuts.at(0) && p->par()->getVy() < vertex_y_cuts.at(1))
    //            && (p->par()->getVz() > vertex_z_cuts.at(0) && p->par()->getVz() < vertex_z_cuts.at(1)));
    //    //</editor-fold>
}

// checkVertexCorrelation function --------------------------------------------------------------------------------------------------------------------------------------

bool clas12ana::checkVertexCorrelation(region_part_ptr el, region_part_ptr p) {
    // true if inside cut

    if (p->getRegion() == FD) {
        // My addition!
        return ((p->par()->getVz() - el->par()->getVz()) > vertex_corr_cuts_FD.at(0) && (p->par()->getVz() - el->par()->getVz()) < vertex_corr_cuts_FD.at(1));
    } else if (p->getRegion() == CD) {
        return ((p->par()->getVz() - el->par()->getVz()) > vertex_corr_cuts_CD.at(0) && (p->par()->getVz() - el->par()->getVz()) < vertex_corr_cuts_CD.at(1));
    } else {
        return ((p->par()->getVz() - el->par()->getVz()) > vertex_corr_cuts.at(0) && (p->par()->getVz() - el->par()->getVz()) < vertex_corr_cuts.at(1));
    }

    //    //<editor-fold desc="Justin's original">
    //    //true if inside cut
    //    return ((p->par()->getVz() - el->par()->getVz()) > vertex_corr_cuts.at(0) && (p->par()->getVz() - el->par()->getVz()) < vertex_corr_cuts.at(1));
    //    //</editor-fold>
}

// checkPidCut function -------------------------------------------------------------------------------------------------------------------------------------------------

bool clas12ana::checkPidCut(region_part_ptr p) {
    // function returns true if inside PID cuts

    // electron pid is handled by ECal sampling fractions cuts NOT here
    if (p->par()->getPid() == 11) return true;

    if (p->getRegion() == FD) {
        // forward detector cuts
        auto itter = pid_cuts_fd.find(p->par()->getPid());
        if (itter != pid_cuts_fd.end())
            return (abs(p->par()->getChi2Pid() - itter->second.at(0)) < itter->second.at(1));
        else
            return false;
    } else if (p->getRegion() == CD) {
        // central detector cuts
        auto itter = pid_cuts_cd.find(p->par()->getPid());
        if (itter != pid_cuts_cd.end())
            return (abs(p->par()->getChi2Pid() - itter->second.at(0)) < itter->second.at(1));
        else
            return false;
    }

    return true;
}

// readEcalSFPar function -----------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::readEcalSFPar(const char *filename) {
    int num_par = 6;
    std::ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        std::string tp;

        // remove 3 lines of header
        for (int i = 0; i < 2; i++) getline(infile, tp);
        std::cout << tp << endl;

        for (int i = 1; i < 7; i++) {
            getline(infile, tp);  // read data from file object and put it into string.
            std::stringstream ss(tp);
            double parameter;
            // get parameters for a given sector
            for (int j = 0; j < num_par; j++) {
                ss >> parameter;
                ecal_sf_fcn_par[i][j] = parameter;
            }
        }

        InitSFEcalCuts();
    } else
        std::cout << "ECal parameter files does not exist!!!" << endl;
}

// readEcalPPar function ------------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::readEcalPPar(const char *filename) {
    int num_par = 6;
    std::ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        std::string tp;

        // remove 3 lines of header
        for (int i = 0; i < 2; i++) getline(infile, tp);
        std::cout << tp << endl;

        for (int i = 1; i < 7; i++) {
            getline(infile, tp);  // read data from file object and put it into string.
            std::stringstream ss(tp);
            double parameter;
            // get parameters for a given sector
            for (int j = 0; j < num_par; j++) {
                ss >> parameter;
                ecal_p_fcn_par[i][j] = parameter;
            }
        }

        InitSFPCuts();
    } else
        std::cout << "ECal parameter files does not exist!!!" << endl;
}

// readInputParam function ----------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::readInputParam(const char *filename) {
    std::ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        std::string tp;

        // remove 3 lines of header
        for (int i = 0; i < 3; i++) getline(infile, tp);

        while (getline(infile,
                       tp))  // read data from file object and put it into string.
        {
            std::stringstream ss(tp);
            std::string parameter, parameter2;
            double value;
            // get cut identifier
            ss >> parameter;
            if (parameter == "pid_cuts") {
                // get cut values
                ss >> parameter2;
                std::stringstream ss2(parameter2);
                std::string pid_v;
                std::string detector;
                int count = 0;  // parameter number
                int pid = -99;
                std::vector<double> par;

                while (getline(ss2, pid_v, ':')) {
                    if (count == 0)
                        pid = stoi(pid_v);
                    else if (count < 3)
                        par.push_back(atof(pid_v.c_str()));
                    else if (count == 3)
                        detector = pid_v;

                    count++;
                }
                if (pid != -99)  // if pid cut exists in file
                {
                    if (detector == "FD")
                        pid_cuts_fd.insert(pair<int, std::vector<double> >(pid, par));
                    else if (detector == "CD")
                        pid_cuts_cd.insert(pair<int, std::vector<double> >(pid, par));
                }
            }  // end PID cuts section

            else if (parameter == "vertex_cut") {
                ss >> parameter2;
                std::stringstream ss2(parameter2);
                std::string pid_v;
                int count = 0;
                std::string pid = "";
                std::vector<double> par;

                while (getline(ss2, pid_v, ':')) {
                    if (count == 0)
                        pid = pid_v;
                    else
                        par.push_back(atof(pid_v.c_str()));

                    count++;
                }

                if (pid != "") vertex_cuts.insert(pair<string, std::vector<double> >(pid, par));
            } else if (parameter == "Momentum_cuts_ECAL") {
                // My addition
                //            else if (parameter == "Momentum_cuts") { // My addition
                // TODO: organize this properly with a map for each pdg.
                ss >> parameter2;
                std::stringstream ss2(parameter2);
                std::string pid_v;
                int count = 0;
                std::string pid = "";
                std::vector<double> par;

                while (getline(ss2, pid_v, ':')) {
                    if (count == 0)
                        pid = pid_v;
                    else
                        par.push_back(atof(pid_v.c_str()));

                    count++;
                }

                if (pid != "") { Neutron_Momentum_cut = par.at(1); }
            } else if (parameter == "Beta_cut_ECAL") {
                // My addition
                // TODO: organize this properly with a map for each pdg.
                ss >> parameter2;
                std::stringstream ss2(parameter2);
                std::string pid_v;
                int count = 0;
                std::string pid = "";
                std::vector<double> par;

                while (getline(ss2, pid_v, ':')) {
                    if (count == 0)
                        pid = pid_v;
                    else
                        par.push_back(atof(pid_v.c_str()));

                    count++;
                }

                if (pid != "") {
                    FD_Neutral_Beta_Mean = par.at(0);
                    FD_Neutral_Beta_cut = par.at(1);
                }
            } else if (parameter == "dPhi_pFD_pCD") {
                // My addition
                //            } else if (parameter == "dPhi_p1_p2") { // My addition
                // TODO: organize this properly with a map for each pdg.
                ss >> parameter2;
                std::stringstream ss2(parameter2);
                std::string pid_v;
                int count = 0;
                std::string pid = "";
                std::vector<double> par;

                while (getline(ss2, pid_v, ':')) {
                    if (count == 0)
                        pid = pid_v;
                    else
                        par.push_back(atof(pid_v.c_str()));

                    count++;
                }

                if (pid != "") { dPhi_p1_p2_Mean = par.at(0); }
            }

            /*


                else if(parameter == "cell_pos")
                  {
                ss >> parameter2;
                    std::stringstream ss2(parameter2);
                    std::string cell_v;
                    while(getline(ss2, cell_v, ':'))
                      cell.push_back(atof(cell_v.c_str()));
                  }

                ss >> value;

            if(parameter == "raster_x")
                  raster_x = value;
                else if(parameter == "raster_y")
                  raster_y = value;
                else if(parameter == "cell_limit")
                  {
                    cell.push_back(value); //first limit
                    ss >> value; //get second limit
                    cell.push_back(value); //second limit
                  }
                else if(parameter == "target_A")
                  target_A = value;
                else if(parameter == "target_Z")
                  target_Z = value;
                else if(parameter == "beamspot_x")
                  beamspot_x = value;
                else if(parameter == "beamspot_y")
                  beamspot_y = value;

            */
        }
    } else {
        std::cout << "clas12ana::readInputParam: Parameter file didn't read in:\n" << filename << endl;
    }

    return;
}

// printParams function -------------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::printParams() {
    std::cout << endl;
    std::cout << "Target Parameters:" << endl;

    std::cout << "Central Detector PID cuts:" << endl;
    for (auto itr = pid_cuts_cd.begin(); itr != pid_cuts_cd.end(); ++itr) {
        std::cout << '\t' << "Particle type: " << itr->first << '\t' << "{mean,sigma}: ";
        for (auto a : itr->second) std::cout << '\t' << a;
        std::cout << '\n';
    }

    std::cout << "Forward Detector PID cuts:" << endl;
    for (auto itr = pid_cuts_fd.begin(); itr != pid_cuts_fd.end(); ++itr) {
        std::cout << '\t' << "Particle type: " << itr->first << '\t' << "{mean,sigma}: ";
        for (auto a : itr->second) std::cout << '\t' << a;
        std::cout << '\n';
    }

    std::cout << "Vertex cuts:" << endl;
    for (auto itr = vertex_cuts.begin(); itr != vertex_cuts.end(); ++itr) {
        std::cout << '\t' << "Particle type: " << itr->first << '\t' << "{min,max}: ";
        for (auto a : itr->second) std::cout << '\t' << a;
        std::cout << '\n';
    }
}

// getCOM function ------------------------------------------------------------------------------------------------------------------------------------------------------

TVector3 clas12ana::getCOM(TLorentzVector lead, TLorentzVector recoil, TLorentzVector q) {
    TVector3 com = (lead + recoil - q).Vect();  // center of momentum vector
    TVector3 pmiss = (lead - q).Vect();

    // vz along pmiss, vx,vy transverse
    TVector3 vz = pmiss.Unit();  // also called vt
    TVector3 vy = pmiss.Cross(q.Vect()).Unit();
    TVector3 vx = vz.Cross(vy).Unit();

    com *= 1000;  // GeV to MeV

    if (abs(com.Dot(vy)) < 20) std::cout << com.Mag() << " " << com.Dot(vx) << " " << com.Dot(vy) << " " << com.Dot(vz) << " " << lead.P() << " " << recoil.P() << endl;

    return TVector3(com.Dot(vx), com.Dot(vy), com.Dot(vz));
}

// getLeadRecoilSRC function --------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::getLeadRecoilSRC(TLorentzVector beam, TLorentzVector target, TLorentzVector el) {
    lead_proton.clear();
    recoil_proton.clear();

    TLorentzVector ptr;
    TLorentzVector q = beam - el;  // photon  4-vector
    double q2 = -q.M2();
    double xb = q2 / (2 * mass_proton * (beam.E() - el.E()));  // x-borken

    if (!(q2 > q2_cut && xb > xb_cut)) return;

    int lead_idx = -1;
    int lead_mult = 0;

    for (int idx_ptr = 0; idx_ptr < protons.size(); idx_ptr++) {
        ptr.SetXYZM(protons.at(idx_ptr)->par()->getPx(), protons.at(idx_ptr)->par()->getPy(), protons.at(idx_ptr)->par()->getPz(), mass_proton);

        TLorentzVector miss = beam + target - el - ptr;  // missing 4-vector
        double pmiss = miss.P();
        double mmiss = miss.M2();
        double theta_pq = ptr.Vect().Angle(q.Vect()) * TMath::RadToDeg();  // angle between vectors p_miss and q
        double p_q = ptr.Vect().Mag() / q.Vect().Mag();                    // |p|/|q|
        if (pmiss > pmiss_cut && mmiss > mmiss_cut[0] && mmiss < mmiss_cut[1] && theta_pq < theta_pq_cut && (p_q < pq_cut[1] && p_q > pq_cut[0])) {
            lead_idx = idx_ptr;
            lead_mult++;  // check for double lead
        }
    }

    ptr.SetXYZM(0, 0, 0, 0);

    if (lead_idx == -1 || lead_mult != 1) return;

    lead_proton.push_back(protons.at(lead_idx));

    int recoil_idx = -1;

    for (int idx_ptr = 0; idx_ptr < protons.size(); idx_ptr++) {
        if (idx_ptr == lead_idx) continue;

        if (protons[idx_ptr]->par()->getP() > recoil_mom_cut) recoil_proton.push_back(protons.at(idx_ptr));
    }

    return;
}

// debugByPid function --------------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::debugByPid(region_part_ptr p) {
    int pid = p->par()->getPid();
    double par_mom = p->par()->getP();
    double par_beta = p->par()->getBeta();

    bool is_cd = (p->getRegion() == CD);
    bool is_fd = (p->getRegion() == FD);

    if (is_fd) {
        if (pid == 2212)
            pid_proton_fd_debug->Fill(par_mom, par_beta);
        else if (pid == 45)
            pid_deuteron_fd_debug->Fill(par_mom, par_beta);
        else if (pid == 211)
            pid_piplus_fd_debug->Fill(par_mom, par_beta);
        else if (pid == -211)
            pid_piminus_fd_debug->Fill(par_mom, par_beta);
        else if (pid == 321)
            pid_kplus_fd_debug->Fill(par_mom, par_beta);
        else if (pid == -321)
            pid_kminus_fd_debug->Fill(par_mom, par_beta);
        else if (pid == 0 || pid == 2112)
            pid_neutrals_fd_debug->Fill(par_mom, par_beta);
    } else if (is_cd) {
        if (pid == 2212)
            pid_proton_cd_debug->Fill(par_mom, par_beta);
        else if (pid == 45)
            pid_deuteron_cd_debug->Fill(par_mom, par_beta);
        else if (pid == 211)
            pid_piplus_cd_debug->Fill(par_mom, par_beta);
        else if (pid == -211)
            pid_piminus_cd_debug->Fill(par_mom, par_beta);
        else if (pid == 321)
            pid_kplus_cd_debug->Fill(par_mom, par_beta);
        else if (pid == -321)
            pid_kminus_cd_debug->Fill(par_mom, par_beta);
        else if (pid == 0 || pid == 2112)
            pid_neutrals_cd_debug->Fill(par_mom, par_beta);
    }
}

// InitDebugPlots function ----------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::InitDebugPlots() {
    for (int i = 1; i <= 6; i++) {
        sf_p_debug_b[i] = new TH2D(Form("sf_p_debug_b_sector_%d", i), Form("Sampling Fraction Before Cuts Sector_%d", i), 100, 0, 6, 100, 0, .4);
        sf_p_debug_a[i] = new TH2D(Form("sf_p_debug_a_sector_%d", i), Form("Sampling Fraction  After Cuts Sector_%d", i), 100, 0, 6, 100, 0, .4);

        sf_e_debug_b[i] = new TH2D(Form("sf_e_debug_b_sector_%d", i), Form("Sampling Fraction Before Cuts Sector_%d", i), 100, 0, 1.5, 100, 0, .4);
        sf_e_debug_a[i] = new TH2D(Form("sf_e_debug_a_sector_%d", i), Form("Sampling Fraction  After Cuts Sector_%d", i), 100, 0, 1.5, 100, 0, .4);
    }

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        dc_hit_map_b[i] = new TH2D(Form("dc_hitmap_before_%d", i), Form("Region %d Before Cuts", i), 600, -300, 300, 600, -300, 300);
        dc_hit_map_a[i] = new TH2D(Form("dc_hitmap_after_%d", i), Form("Region %d After Cuts", i), 600, -300, 300, 600, -300, 300);
        dc_hit_map_a_proton[i] = new TH2D(Form("dc_hitmap_after_proton_%d", i), Form("Region %d After Cuts", i), 600, -300, 300, 600, -300, 300);
        dc_hit_map_b_proton[i] = new TH2D(Form("dc_hitmap_before_proton_%d", i), Form("Region %d Before Cuts", i), 600, -300, 300, 600, -300, 300);
        dc_hit_map_a_pion[i] = new TH2D(Form("dc_hitmap_after_pion_%d", i), Form("Region %d After Cuts", i), 600, -300, 300, 600, -300, 300);
        dc_hit_map_b_pion[i] = new TH2D(Form("dc_hitmap_before_pion_%d", i), Form("Region %d Before Cuts", i), 600, -300, 300, 600, -300, 300);
    }
}

// WriteDebugPlots function ---------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::WriteDebugPlots() {
    TFile *f_debugOut = new TFile(debug_fileName, "RECREATE");

    //<editor-fold desc="My addition">
    hTheta_e_vs_Phi_e_fiducial_cuts_map_ECAL->Write();     // My addition
    hTheta_p_vs_Phi_p_AMap_DC_NO_CUTS->Write();            // My addition
    hTheta_p_vs_Phi_p_AMap_DC_WITH_CUTS->Write();          // My addition
    hTheta_vs_Phi_hit_map_ECAL_no_fiducial_cuts->Write();  // My addition
    hTheta_vs_Phi_hit_map_ECAL_w_fiducial_cuts->Write();   // My addition
    //</editor-fold>

    for (int i = 1; i <= 6; i++) {
        sf_p_debug_b[i]->Write();
        sf_p_debug_a[i]->Write();
        sf_e_debug_b[i]->Write();
        sf_e_debug_a[i]->Write();
    }

    for (int i = 1; i <= 6; i++) {
        ecal_sf_fcn[0][i]->Write();
        ecal_sf_fcn[1][i]->Write();
        ecal_p_fcn[0][i]->Write();
        ecal_p_fcn[1][i]->Write();
    }

    for (int i = 1; i <= 3; i++) dc_hit_map_b[i]->Write();

    for (int i = 1; i <= 3; i++) dc_hit_map_a[i]->Write();

    for (int i = 1; i <= 3; i++) dc_hit_map_b_proton[i]->Write();

    for (int i = 1; i <= 3; i++) dc_hit_map_a_proton[i]->Write();

    for (int i = 1; i <= 3; i++) dc_hit_map_b_pion[i]->Write();

    for (int i = 1; i <= 3; i++) dc_hit_map_a_pion[i]->Write();

    sf_v_ecalIN_debug->Write();
    sf_w_ecalIN_debug->Write();
    sf_v_ecalOUT_debug->Write();
    sf_w_ecalOUT_debug->Write();
    sf_v_pcal_debug->Write();
    sf_w_pcal_debug->Write();

    sf_v_ecalIN_a_debug->Write();
    sf_w_ecalIN_a_debug->Write();
    sf_v_ecalOUT_a_debug->Write();
    sf_w_ecalOUT_a_debug->Write();
    sf_v_pcal_a_debug->Write();
    sf_w_pcal_a_debug->Write();

    pid_proton_fd_debug->Write();
    pid_deuteron_fd_debug->Write();
    pid_piplus_fd_debug->Write();
    pid_piminus_fd_debug->Write();
    pid_kplus_fd_debug->Write();
    pid_kminus_fd_debug->Write();
    pid_neutrals_fd_debug->Write();

    pid_proton_cd_debug->Write();
    pid_deuteron_cd_debug->Write();
    pid_piplus_cd_debug->Write();
    pid_piminus_cd_debug->Write();
    pid_kplus_cd_debug->Write();
    pid_kminus_cd_debug->Write();
    pid_neutrals_cd_debug->Write();

    pid_cd_debug->Write();
    pid_fd_debug->Write();

    el_vz_debug->Write();
    el_vz_p_debug->Write();

    //<editor-fold desc="my debugging - multiplicity plots before and after cuts (no #e cuts)">
    //    multi_p_vs_cpi_fd_BC_debug->Write();
    //    multi_p_vs_cpi_fd_AC_debug->Write();
    //    multi_p_vs_cpi_cd_BC_debug->Write();
    //    multi_p_vs_cpi_cd_AC_debug->Write();
    multi_p_vs_cpi_BC_debug->Write();
    multi_p_vs_cpi_AC_debug->Write();
    //    multi_p_fd_BC_debug->Write();
    //    multi_p_fd_AC_debug->Write();
    //    multi_p_cd_BC_debug->Write();
    //    multi_p_cd_AC_debug->Write();
    multi_p_BC_debug->Write();
    multi_p_AC_debug->Write();
    //    multi_cpi_fd_BC_debug->Write();
    //    multi_cpi_fd_AC_debug->Write();
    //    multi_cpi_cd_BC_debug->Write();
    //    multi_cpi_cd_AC_debug->Write();
    multi_cpi_BC_debug->Write();
    multi_cpi_AC_debug->Write();
    //</editor-fold>

    //<editor-fold desc="my debugging - multiplicity plots before and after cuts (1e cut)">
    //    multi_p_vs_cpi_1e_cut_fd_BC_debug->Write();
    //    multi_p_vs_cpi_1e_cut_fd_AC_debug->Write();
    //    multi_p_vs_cpi_1e_cut_cd_BC_debug->Write();
    //    multi_p_vs_cpi_1e_cut_cd_AC_debug->Write();
    multi_p_vs_cpi_1e_cut_BC_debug->Write();
    multi_p_vs_cpi_1e_cut_AC_debug->Write();
    //    multi_p_1e_cut_fd_BC_debug->Write();
    //    multi_p_1e_cut_fd_AC_debug->Write();
    //    multi_p_1e_cut_cd_BC_debug->Write();
    //    multi_p_1e_cut_cd_AC_debug->Write();
    multi_p_1e_cut_BC_debug->Write();
    multi_p_1e_cut_AC_debug->Write();
    //    multi_cpi_1e_cut_fd_BC_debug->Write();
    //    multi_cpi_1e_cut_fd_AC_debug->Write();
    //    multi_cpi_1e_cut_cd_BC_debug->Write();
    //    multi_cpi_1e_cut_cd_AC_debug->Write();
    multi_cpi_1e_cut_BC_debug->Write();
    multi_cpi_1e_cut_AC_debug->Write();
    //</editor-fold>

    f_debugOut->Close();

    //<editor-fold desc="write hit_map_ref file (my addition)">
    TFile *hit_map_ref_Out = new TFile(hit_map_ref_Out_filename, "RECREATE");
    hAng_hit_map_electrons->Write();  // My addition
    hAng_hit_map_protons->Write();    // My addition
    hAng_hit_map_neutrons->Write();   // My addition

    hit_map_ref_Out->Close();
    //</editor-fold>
}

// FillECALHitMap function ----------------------------------------------------------------------------------------------------------------------------------------------

void clas12ana::FillECALHitMap(region_part_ptr p) {
    // My addition
    if (p->getRegion() == FD) {
        if (p->par()->getCharge() == 0) {
            double fiducial_cut = dc_edge_cut;
            //        double fiducial_cut = ecal_edge_cut;

            bool PCALhit = (p->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
            bool ECINhit = (p->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
            bool ECOUThit = (p->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

            if (!PCALhit && (ECINhit || ECOUThit)) {
                hTheta_vs_Phi_hit_map_ECAL_no_fiducial_cuts->Fill(p->getPhi() * 180.0 / analysis_math::pi, p->getTheta() * 180.0 / analysis_math::pi);
                hAng_hit_map_neutrons->Fill(p->getPhi() * 180.0 / analysis_math::pi, p->getTheta() * 180.0 / analysis_math::pi);

                auto detlayer = ECINhit ? clas12::ECIN : clas12::ECOUT; /* determine the earliest layer that the neutral hit in */

                if (p->cal(detlayer)->getLv() > fiducial_cut && p->cal(detlayer)->getLw() > fiducial_cut) {
                    hTheta_vs_Phi_hit_map_ECAL_w_fiducial_cuts->Fill(p->getPhi() * 180.0 / analysis_math::pi, p->getTheta() * 180.0 / analysis_math::pi);
                }

                /*
                if (ECINhit) {
                    if (p->cal(clas12::ECIN)->getLv() > fiducial_cut && p->cal(clas12::ECIN)->getLw() > fiducial_cut) {
                        hTheta_vs_Phi_hit_map_ECAL_w_fiducial_cuts->Fill(p->getPhi() * 180.0 / analysis_math::pi, p->getTheta() * 180.0 / analysis_math::pi);
                    }
                } else if (ECOUThit) {
                    if (p->cal(clas12::ECOUT)->getLv() > fiducial_cut && p->cal(clas12::ECOUT)->getLw() > fiducial_cut) {
                        hTheta_vs_Phi_hit_map_ECAL_w_fiducial_cuts->Fill(p->getPhi() * 180.0 / analysis_math::pi, p->getTheta() * 180.0 / analysis_math::pi);
                    }
                }
    */
            }
        } else if (p->par()->getPid() == 2212) {
            hTheta_p_vs_Phi_p_AMap_DC_NO_CUTS->Fill(p->getPhi() * 180.0 / analysis_math::pi, p->getTheta() * 180.0 / analysis_math::pi);
        }
    }
}
