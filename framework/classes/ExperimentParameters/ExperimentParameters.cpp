#ifndef EXPERIMENTPARAMETERS_H
#define EXPERIMENTPARAMETERS_H

#include "ExperimentParameters.h"

// Constructor ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Constructor */
ExperimentParameters::ExperimentParameters(const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample) {
    SampleName = ConfigureSampleName(AnalyzeFilePath, AnalyzeFileSample);
    BeamEnergy = GetBeamEnergyFromString(SampleName);
    // BeamEnergy = ConfigureBeamEnergy(SampleName);

    if (SampleName.find("H1") <= SampleName[SampleName.size() - 1]) {
        TargetElement = "H1";
        TargetElementPDG = 1000010010;
        TotalBaryonNumber_A = 1;
        TotalChargeNumber_Z = 1;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else if (SampleName.find("D2") <= SampleName[SampleName.size() - 1]) {
        TargetElement = "D2";
        TargetElementPDG = 1000010020;
        TotalBaryonNumber_A = 2;
        TotalChargeNumber_Z = 1;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else if ((SampleName.find("c12") <= SampleName[SampleName.size() - 1]) || (SampleName.find("C12") <= SampleName[SampleName.size() - 1]) ||
               (SampleName.find("12c") <= SampleName[SampleName.size() - 1]) || (SampleName.find("12C") <= SampleName[SampleName.size() - 1]) ||
               (SampleName.find("_c_") <= SampleName[SampleName.size() - 1]) || (SampleName.find("_C_") <= SampleName[SampleName.size() - 1])) {
        TargetElement = "C12";
        TargetElementPDG = 1000060120;
        TotalBaryonNumber_A = 12;
        TotalChargeNumber_Z = 6;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else if (SampleName.find("Ca48") <= SampleName[SampleName.size() - 1]) {
        TargetElement = "Ca48";
        TargetElementPDG = 1000200480;
        TotalBaryonNumber_A = 48;
        TotalChargeNumber_Z = 20;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else if (SampleName.find("Ar40") <= SampleName[SampleName.size() - 1]) {
        TargetElement = "Ar40";
        TargetElementPDG = 1000180400;
        TotalBaryonNumber_A = 40;
        TotalChargeNumber_Z = 18;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else {
        TargetElement = "UNKOWN";
        TargetElementPDG = -9999;
        TotalBaryonNumber_A = TotalChargeNumber_Z = StrangeQuarksNumber_L = IsomerNumber_I = 0;
    }
}
#pragma endregion

// ConfigureSampleName function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigureSampleName function */
string ExperimentParameters::ConfigureSampleName(const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample) {
    std::string sName = "UNKNOWN_SAMPLE";

    if (basic_tools::FindSubstring(AnalyzeFilePath, "Uniform_e-p-n_samples")) {  // Uniform samples
        if (basic_tools::FindSubstring(AnalyzeFilePath, "OutPut_Tester_e_Tester_e"))
        // if (basic_tools::FindSubstring(AnalyzeFilePath, "OutPut_Tester_e_1e"))
        {  // Electron tester samples
            if (AnalyzeFilePath == "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_Tester_e_Tester_e")
            // if (AnalyzeFilePath == "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_Tester_e_1e")
            {  // Uniform, simulation, 2GeV, ifarm
                if (AnalyzeFileSample == "reconhipo") {
                    BeamAt2GeV = SimulationSample = true;
                    sName = "Uniform_sample_e_tester_2070MeV";
                }
            } else if (AnalyzeFilePath == "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV/OutPut_Tester_e_Tester_e")
            // else if (AnalyzeFilePath == "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV/OutPut_Tester_e_1e")
            {  // Uniform, simulation, 4GeV, ifarm
                if (AnalyzeFileSample == "reconhipo") {
                    BeamAt4GeV = SimulationSample = true;
                    sName = "Uniform_sample_e_tester_4029MeV";
                }
            } else if (AnalyzeFilePath == "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/5986MeV/OutPut_Tester_e_Tester_e")
            // else if (AnalyzeFilePath == "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/5986MeV/OutPut_Tester_e_1e")
            {  // Uniform, simulation, 6GeV, ifarm
                if (AnalyzeFileSample == "reconhipo") {
                    BeamAt6GeV = SimulationSample = true;
                    sName = "Uniform_sample_e_tester_5986MeV";
                }
            }
        } else {
            // Uniform samples for acceptance maps
            if (basic_tools::FindSubstring(AnalyzeFilePath, "2070MeV")) {
                /* 2GeV samples
                NOTE: here '2070MeV' has to be in the sample path! */
                if (AnalyzeFilePath == "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_1e") {  // Uniform 1e, simulation, 2GeV, ifarm
                    if (AnalyzeFileSample == "reconhipo") {
                        BeamAt2GeV = SimulationSample = true;
                        sName = "Uniform_1e_sample_2070MeV";
                    }
                } else if (AnalyzeFilePath ==
                           "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_ep") {  // Uniform ep, simulation, 2GeV, ifarm
                    if (AnalyzeFileSample == "reconhipo") {
                        BeamAt2GeV = SimulationSample = true;
                        sName = "Uniform_ep_sample_2070MeV";
                    }
                } else if ((AnalyzeFilePath == "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_en") ||
                           (AnalyzeFilePath ==
                            "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV_ConstPn/OutPut_en")) {  // Uniform en, simulation, 2GeV, ifarm
                    if (AnalyzeFileSample == "reconhipo") {
                        BeamAt2GeV = SimulationSample = true;
                        sName = "Uniform_en_sample_2070MeV";
                    }
                }
            } else if (basic_tools::FindSubstring(AnalyzeFilePath, "4029MeV")) {
                /* 4GeV samples
                NOTE: here '4029MeV' has to be in the sample path! */
                if (AnalyzeFilePath ==
                    "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV_2/OutPut_1e") {  // Uniform 1e, simulation, 4GeV, ifarm
                    // if (AnalyzeFilePath == "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV/OutPut_1e") {  // Uniform 1e, simulation,
                    // 4GeV, ifarm
                    if (AnalyzeFileSample == "reconhipo") {
                        BeamAt4GeV = SimulationSample = true;
                        sName = "Uniform_1e_sample_4029MeV";
                    }
                } else if (AnalyzeFilePath ==
                           "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV/OutPut_ep") {  // Uniform ep, simulation, 4GeV, ifarm
                    if (AnalyzeFileSample == "reconhipo") {
                        BeamAt4GeV = SimulationSample = true;
                        sName = "Uniform_ep_sample_4029MeV";
                    }
                } else if ((AnalyzeFilePath == "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV/OutPut_en") ||
                           (AnalyzeFilePath ==
                            "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV_ConstPn/OutPut_en")) {  // Uniform en, simulation, 4GeV, ifarm
                    if (AnalyzeFileSample == "reconhipo") {
                        BeamAt4GeV = SimulationSample = true;
                        sName = "Uniform_en_sample_4029MeV";
                    }
                }
            } else if (basic_tools::FindSubstring(AnalyzeFilePath, "5986MeV")) {
                /* 6GeV samples
                NOTE: here '5986MeV' has to be in the sample path! */
                if (AnalyzeFilePath == "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/5986MeV/OutPut_1e") {  // Uniform 1e, simulation, 6GeV, ifarm
                    if (AnalyzeFileSample == "reconhipo") {
                        BeamAt6GeV = SimulationSample = true;
                        sName = "Uniform_1e_sample_5986MeV";
                    }
                } else if (AnalyzeFilePath ==
                           "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/5986MeV/OutPut_ep") {  // Uniform ep, simulation, 6GeV, ifarm
                    if (AnalyzeFileSample == "reconhipo") {
                        BeamAt6GeV = SimulationSample = true;
                        sName = "Uniform_ep_sample_5986MeV";
                    }
                } else if ((AnalyzeFilePath == "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/5986MeV/OutPut_en") ||
                           (AnalyzeFilePath ==
                            "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/5986MeV_ConstPn/OutPut_en")) {  // Uniform en, simulation, 6GeV, ifarm
                    if (AnalyzeFileSample == "reconhipo") {
                        BeamAt6GeV = SimulationSample = true;
                        sName = "Uniform_en_sample_5986MeV";
                    }
                }
            }
        }
    } else {
        if (basic_tools::FindSubstring(AnalyzeFilePath, "rg-m") && basic_tools::FindSubstring(AnalyzeFilePath, "dst")) {  // Data samples
            if (AnalyzeFilePath == "volatile/clas12/rg-m/48Ca/dst/recon") {                                               // Ca48, data, 6GeV, ifarm
                if (AnalyzeFileSample == "015832") {
                    BeamAt6GeV = DataSample = true;
                    sName = "Ca48_data_6GeV_run_015832";
                } else if (AnalyzeFileSample == "015843") {
                    BeamAt6GeV = DataSample = true;
                    sName = "Ca48_data_6GeV_run_015843";
                } else if (AnalyzeFileSample == "015852") {
                    BeamAt6GeV = DataSample = true;
                    sName = "Ca48_data_6GeV_run_015852";
                } else if (AnalyzeFileSample == "015854") {
                    BeamAt6GeV = DataSample = true;
                    sName = "Ca48_data_6GeV_run_015854";
                }
            } else if (AnalyzeFilePath == "cache/hallb/scratch/rg-m/LH2/8.6.0") {  // LH2, data, 6GeV, ifarm
                if (AnalyzeFileSample == "015032") {
                    BeamAt6GeV = DataSample = true;
                    sName = "LH2_data_6GeV_run_015032";
                }
            } else if (AnalyzeFilePath == "cache/clas12/rg-m/production/pass1/2gev/H/dst/recon") {  // H1, data, 2GeV, ifarm (Q2 study)
                if (AnalyzeFileSample == "015634") {
                    BeamAt2GeV = DataSample = true;
                    sName = "H1_data_2GeV_run_015634";
                }
            }
            // else if (AnalyzeFilePath == "cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon")
            // { // H1, data, 4GeV, ifarm (Q2 study)
            //     if (AnalyzeFileSample == "015743")
            //     {
            //         BeamAt4GeV = DataSample = true;
            //         sName = "H1_data_4GeV_run_015743";
            //     }
            // }
            else if (AnalyzeFilePath == "cache/clas12/rg-m/production/pass1/6gev/H/dst/recon") {  // H1, data, 6GeV, ifarm (Q2 study)
                if (AnalyzeFileSample == "015028") {
                    BeamAt6GeV = DataSample = true;
                    sName = "H1_data_6GeV_run_015028";
                }
            } else if (AnalyzeFilePath == "cache/clas12/rg-m/production/pass1/2gev/D/dst/recon") {  // D2, data, 2GeV, ifarm (Q2 study)
                if (AnalyzeFileSample == "") {
                    BeamAt2GeV = DataSample = true;
                    sName = "D2_data_2GeV";
                }
            }
            // else if (AnalyzeFilePath == "cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon")
            // { // D2, data, 4GeV, ifarm (Q2 study)
            //     if (AnalyzeFileSample == "015743")
            //     {
            //         BeamAt4GeV = DataSample = true;
            //         sName = "D2_data_4GeV_run_015743";
            //     }
            // }
            else if (AnalyzeFilePath == "cache/clas12/rg-m/production/pass1/6gev/D/dst/recon") {  // D2, data, 6GeV, ifarm (Q2 study)
                if (AnalyzeFileSample == "015443") {
                    BeamAt6GeV = DataSample = true;
                    sName = "D2_data_6GeV_run_015443";
                }
            } else if (AnalyzeFilePath == "cache/clas12/rg-m/production/pass1/2gev/Ar/dst/recon") {  // Ar40, data, 2GeV, ifarm (Q2 study)
                if (AnalyzeFileSample == "015672") {
                    BeamAt2GeV = DataSample = true;
                    sName = "Ar40_data_2GeV_run_015672";
                }
            } else if (AnalyzeFilePath == "cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon") {  // Ar40, data, 4GeV, ifarm (Q2 study)
                if (AnalyzeFileSample == "015743") {
                    BeamAt4GeV = DataSample = true;
                    sName = "Ar40_data_4GeV_run_015743";
                }
            } else if (AnalyzeFilePath == "cache/clas12/rg-m/production/pass1/6gev/Ar/dst/recon") {  // Ar40, data, 6GeV, ifarm (Q2 study)
                if (AnalyzeFileSample == "015792") {
                    BeamAt6GeV = DataSample = true;
                    sName = "Ar40_data_6GeV_run_015792";
                }
            } else if (AnalyzeFilePath == "cache/clas12/rg-m/production/pass1/2gev/C/dst/recon") {  // C, data, 2GeV, ifarm (Q2 study)
                if (AnalyzeFileSample == "015664") {
                    BeamAt2GeV = DataSample = true;
                    sName = "C12_data_2GeV_run_015664";
                }
            } else if (AnalyzeFilePath == "cache/clas12/rg-m/production/pass1/4gev/C/dst/recon") {  // C, data, 4GeV, ifarm (Q2 study)
                if (AnalyzeFileSample == "015778") {
                    BeamAt4GeV = DataSample = true;
                    sName = "C12_data_4GeV_run_015778";
                }
            } else if (AnalyzeFilePath == "cache/clas12/rg-m/production/pass1/6gev/Cx4/dst/recon") {  // Cx4, data, 6GeV, ifarm
                if (AnalyzeFileSample == "015186")                                                    // (Q2 study)
                {
                    BeamAt6GeV = DataSample = true;
                    sName = "C12x4_data_6GeV_run_015186";
                } else if (AnalyzeFileSample == "015187") {
                    BeamAt6GeV = DataSample = true;
                    sName = "C12x4_data_6GeV_run_015187";
                } else if (AnalyzeFileSample == "015188") {
                    BeamAt6GeV = DataSample = true;
                    sName = "C12x4_data_6GeV_run_015188";
                } else if (AnalyzeFileSample == "015189") {
                    BeamAt6GeV = DataSample = true;
                    sName = "C12x4_data_6GeV_run_015189";
                } else if (AnalyzeFileSample == "015190") {
                    BeamAt6GeV = DataSample = true;
                    sName = "C12x4_data_6GeV_run_015190";
                } else if (AnalyzeFileSample == "015191") {
                    BeamAt6GeV = DataSample = true;
                    sName = "C12x4_data_6GeV_run_015191";
                } else if (AnalyzeFileSample == "015192") {
                    BeamAt6GeV = DataSample = true;
                    sName = "C12x4_data_6GeV_run_015192";
                } else if (AnalyzeFileSample == "015193") {
                    BeamAt6GeV = DataSample = true;
                    sName = "C12x4_data_6GeV_run_015193";
                } else if (AnalyzeFileSample == "015194") {
                    BeamAt6GeV = DataSample = true;
                    sName = "C12x4_data_6GeV_run_015194";
                } else if (AnalyzeFileSample == "015195") {
                    BeamAt6GeV = DataSample = true;
                    sName = "C12x4_data_6GeV_run_015195";
                } else if (AnalyzeFileSample == "") {
                    BeamAt6GeV = DataSample = true;
                    sName = "C12x4_data_6GeV";
                }
            }

        } else if (basic_tools::FindSubstring(AnalyzeFilePath, "2N_Analysis_Reco_Samples")) {                                                               // Simulation samples
            if (basic_tools::FindSubstring(AnalyzeFilePath, "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12")) {  // C12 simulation samples
                if (basic_tools::FindSubstring(AnalyzeFilePath,
                                               "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/G18_10a_00_000")) {  // C12, G18, simulation samples
                    if (AnalyzeFilePath ==
                        "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/G18_10a_00_000/2070MeV_Q2_0_02") {  // C12, G18, simulation, 2GeV, ifarm
                        if (AnalyzeFileSample == "reconhipo") {
                            BeamAt2GeV = SimulationSample = true;
                            sName = "C12_simulation_G18_Q2_0_02_2070MeV";
                        }
                    } else if (AnalyzeFilePath ==
                               "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/G18_10a_00_000/4029MeV_Q2_0_25") {  // C12, G18, simulation, 4GeV,
                                                                                                                                                             // ifarm
                        if (AnalyzeFileSample == "reconhipo") {
                            BeamAt4GeV = SimulationSample = true;
                            sName = "C12_simulation_G18_Q2_0_25_4029MeV";
                        }
                    } else if (AnalyzeFilePath ==
                               "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/G18_10a_00_000/2070MeV_Q2_0_02_wFC") {  // C12, G18, simulation,
                                                                                                                                                                 // 2GeV, ifarm
                        if (AnalyzeFileSample == "reconhipo") {
                            BeamAt2GeV = SimulationSample = true;
                            sName = "C12_simulation_G18_Q2_0_02_2070MeV_wFC";
                        }
                    } else if (AnalyzeFilePath ==
                               "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/G18_10a_00_000/4029MeV_Q2_0_25_wFC") {  // C12, G18, simulation,
                                                                                                                                                                 // 4GeV, ifarm
                        if (AnalyzeFileSample == "reconhipo") {
                            BeamAt4GeV = SimulationSample = true;
                            sName = "C12_simulation_G18_Q2_0_25_4029MeV_wFC";
                        }
                    }
                } else if (basic_tools::FindSubstring(
                               AnalyzeFilePath,
                               "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/GEM21_11a_00_000")) {  // C12, SuSa, simulation samples
                    if (AnalyzeFilePath ==
                        "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/GEM21_11a_00_000/2070MeV_Q2_0_02") {  // C12, SuSa, simulation, 2GeV, ifarm
                        if (AnalyzeFileSample == "reconhipo") {
                            BeamAt2GeV = SimulationSample = true;
                            sName = "C12_simulation_SuSa_Q2_0_02_2070MeV";
                        }
                    } else if (AnalyzeFilePath ==
                               "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/GEM21_11a_00_000/4029MeV_Q2_0_25") {  // C12, SuSa, simulation, 4GeV,
                                                                                                                                                               // ifarm
                        if (AnalyzeFileSample == "reconhipo") {
                            BeamAt4GeV = SimulationSample = true;
                            sName = "C12_simulation_SuSa_Q2_0_25_4029MeV";
                        }
                    } else if (AnalyzeFilePath ==
                               "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/GEM21_11a_00_000/2070MeV_Q2_0_02_wFC") {  // C12, SuSa, simulation,
                                                                                                                                                                   // 2GeV, ifarm
                        if (AnalyzeFileSample == "reconhipo") {
                            BeamAt2GeV = SimulationSample = true;
                            sName = "C12_simulation_SuSa_Q2_0_02_2070MeV_wFC";
                        }
                    } else if (AnalyzeFilePath ==
                               "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/GEM21_11a_00_000/4029MeV_Q2_0_25_wFC") {  // C12, SuSa, simulation,
                                                                                                                                                                   // 4GeV, ifarm
                        if (AnalyzeFileSample == "reconhipo") {
                            BeamAt4GeV = SimulationSample = true;
                            sName = "C12_simulation_SuSa_Q2_0_25_4029MeV_wFC";
                        }
                    }
                }
            }
        }
    }

#pragma region /* Safety checks */
    if (!BeamAt2GeV && !BeamAt4GeV && !BeamAt6GeV) {
        cout << "\n\nConfigureSampleName: beam energy unknown! Configured variables are:\n";
        cout << "AnalyzeFilePath = " << AnalyzeFilePath << "\n";
        cout << "sName = " << sName << "\n";
        cout << "BeamAt2GeV = " << BeamAt2GeV << "\n";
        cout << "BeamAt4GeV = " << BeamAt4GeV << "\n";
        cout << "BeamAt6GeV = " << BeamAt6GeV << "\n";
        cout << "Aborting...\n", exit(1);
    }

    if (BeamAt2GeV && BeamAt4GeV && BeamAt6GeV) {
        cout << "\n\nConfigureSampleName: beam energy configuration error! Configured variables are:\n";
        cout << "AnalyzeFilePath = " << AnalyzeFilePath << "\n";
        cout << "sName = " << sName << "\n";
        cout << "BeamAt2GeV = " << BeamAt2GeV << "\n";
        cout << "BeamAt4GeV = " << BeamAt4GeV << "\n";
        cout << "BeamAt6GeV = " << BeamAt6GeV << "\n";
        cout << "SimulationSample = " << SimulationSample << "\n";
        cout << "DataSample = " << DataSample << "\n";
        cout << "Aborting...\n", exit(1);
    }

    if (SimulationSample && DataSample) {
        cout << "\n\nConfigureSampleName: sample type unknown! Configured variables are:\n";
        cout << "AnalyzeFilePath = " << AnalyzeFilePath << "\n";
        cout << "sName = " << sName << "\n";
        cout << "BeamAt2GeV = " << BeamAt2GeV << "\n";
        cout << "BeamAt4GeV = " << BeamAt4GeV << "\n";
        cout << "BeamAt6GeV = " << BeamAt6GeV << "\n";
        cout << "SimulationSample = " << SimulationSample << "\n";
        cout << "DataSample = " << DataSample << "\n";
        cout << "Aborting...\n", exit(1);
    }

    if (!SimulationSample && !DataSample) {
        cout << "\n\nConfigureSampleName: sample type configuration error! Configured variables are:\n";
        cout << "AnalyzeFilePath = " << AnalyzeFilePath << "\n";
        cout << "sName = " << sName << "\n";
        cout << "BeamAt2GeV = " << BeamAt2GeV << "\n";
        cout << "BeamAt4GeV = " << BeamAt4GeV << "\n";
        cout << "BeamAt6GeV = " << BeamAt6GeV << "\n";
        cout << "SimulationSample = " << SimulationSample << "\n";
        cout << "DataSample = " << DataSample << "\n";
        cout << "Aborting...\n", exit(1);
    }
#pragma endregion

    ConfigureVaryingSampleName(sName);
    ConfigureVz_cuts(sName);
    ConfiguredVz_cuts(sName);

    return sName;
}
#pragma endregion

// ConfigureVaryingSampleName function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region                                                                  /* ConfigureVaryingSampleName function */
void ExperimentParameters::ConfigureVaryingSampleName(const std::string &sn) {  // TODO: change this to VaryingSampleName of the appropriate data. currently the same for all data.
    if (basic_tools::FindSubstring(sn, "sim") || (basic_tools::FindSubstring(sn, "Uniform") || basic_tools::FindSubstring(sn, "iniform") || basic_tools::FindSubstring(sn, "Isotrop") ||
                                                  basic_tools::FindSubstring(sn, "isotrop"))) {  // Sample is simulation
        //        VaryingSampleName = SampleName;
        VaryingSampleName = "C12x4_simulation_G18_Q204_6GeV";
    } else if (basic_tools::FindSubstring(sn, "data")) {  // Sample is data
        VaryingSampleName = sn;
        // VaryingSampleName = "C12x4_simulation_G18_Q204_6GeV";
    } else {
        cout << "\n\n\nExperimentParameters::ConfigureVaryingSampleName: sample can't be configured! Aborting...\n", exit(1);
    }
}
#pragma endregion

// ConfigureVz_cuts function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigureVz_cuts function */
void ExperimentParameters::ConfigureVz_cuts(const std::string &sn) {
    if (basic_tools::FindSubstring(sn, "Uniform")) {  // Sample is uniform simulation
        if (basic_tools::FindSubstring(sn, "2070MeV")) {
            if (sn == "Uniform_1e_sample_2070MeV") {
                Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -6., 0.);
                Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -6., 0.);
                Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -6., 0.);
            } else if (sn == "Uniform_ep_sample_2070MeV") {
                Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -6., 0.);
                Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -6., 0.);
                Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -5., -1.);
            } else if (sn == "Uniform_en_sample_2070MeV") {
                Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -6., 0.);
                Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -6., 0.);
                Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -6., 0.);
            }
        } else if (basic_tools::FindSubstring(sn, "4029MeV")) {
            if (sn == "Uniform_1e_sample_4029MeV") {
                Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -5., -1.);
                Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -5., -1.);
                Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -5., -1.);
            } else if (sn == "Uniform_ep_sample_4029MeV") {
                Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -6., 0.);
                Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -6., 0.);
                Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -5., -1.);
            } else if (sn == "Uniform_en_sample_4029MeV") {
                Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -6., 0.);
                Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -6., 0.);
                Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -6., 0.);
            }
        } else if (basic_tools::FindSubstring(sn, "5986MeV")) {
            if (sn == "Uniform_1e_sample_5986MeV") {
                Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -5., -1.);
                Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -5., -1.);
                Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -5., -1.);
            } else if (sn == "Uniform_ep_sample_5986MeV") {
                Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -6., 0.);
                Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -6., 0.);
                Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -5., -1.);
            } else if (sn == "Uniform_en_sample_5986MeV") {
                Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -6., 0.);
                Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -6., 0.);
                Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -6., 0.);
            }
        }
    } else if (basic_tools::FindSubstring(sn, "simulation")) {  // Sample is simulation
        if (sn == "C12x4_simulation_G18_Q204_6GeV") {           // 4-foil
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -8., 3.);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -7., 2.);
        } else if (sn == "C12_simulation_G18_Q204_6GeV") {  // 1-foil
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -5, 5);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -5, 5);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -2, 1);
        } else if (sn == "C12_simulation_G18_Q2_0_02_2070MeV" || sn == "C12_simulation_G18_Q2_0_02_2070MeV_wFC") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -5, 4);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -5, 4);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -2, 1);
        } else if (sn == "C12_simulation_G18_Q2_0_25_4029MeV" || sn == "C12_simulation_G18_Q2_0_25_4029MeV_wFC") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -5, 4);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -5, 4);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -2, 1);
        } else if (sn == "C12_simulation_SuSa_Q2_0_02_2070MeV" || sn == "C12_simulation_SuSa_Q2_0_02_2070MeV_wFC") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -5, 4);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -5, 4);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -2, 1);
        } else if (sn == "C12_simulation_SuSa_Q2_0_25_4029MeV" || sn == "C12_simulation_SuSa_Q2_0_25_4029MeV_wFC") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -5, 4);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -5, 4);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -2, 1);
        }
    } else if (basic_tools::FindSubstring(sn, "data")) {  // Sample is data
        if (sn == "Ar40_data_2GeV_run_015672") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -12., -3.);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -7., -4.);
        } else if (sn == "Ar40_data_4GeV_run_015743") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -9., -2.);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -7., -4.);
        } else if (sn == "Ar40_data_6GeV_run_015792") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -9., -2.);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -7., -4.);
        } else if (sn == "C12_data_2GeV_run_015664") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -9., -1.);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -4., 0.);
        } else if (sn == "C12_data_4GeV_run_015778") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -6., 1.);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -4., -1.);
        } else if (sn == "D2_data_2GeV") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -10., 1.);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -7., 0.);
        } else if (sn == "D2_data_6GeV_run_015443") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -7., 3.);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -7., 1.);
        } else if (sn == "H1_data_2GeV_run_015634") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -11., 1.);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -7., 0.);
        } else if (sn == "H1_data_6GeV_run_015028") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -7., 1.);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -7., 1.);
        } else if (basic_tools::FindSubstring(sn, "C12x4_data_6GeV_run_0151")) {
            if (basic_tools::FindSubstring(sn, "C12x4_data_6GeV_run_015186")) {  // 4-foil
                Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
                Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -7., 2.);
                Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -7., 2.);
            } else if (basic_tools::FindSubstring(sn, "C12x4_data_6GeV_run_015187")) {  // 4-foil
                Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
                Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -7., 2.);
                Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -7., 2.);
            } else if (basic_tools::FindSubstring(sn, "C12x4_data_6GeV_run_015188")) {  // 4-foil
                Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
                Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -8., 3.);
                //            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -7., 2.);
                Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -7., 2.);
            } else {
                Vz_cuts = Vz_cuts_def;
                Vz_cuts_FD = Vz_cuts_FD_def;
                Vz_cuts_CD = Vz_cuts_CD_def;
            }
        } else if (sn == "C12x4_data_6GeV") {  // Sample is data (full 4-foil run)
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -7., 2.);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -8., 3.);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -7., 2.);
        } else if (sn == "LH2_data_6GeV_run_015032") {
            Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -15, 5);
            Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -15, 5);
            Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -15, 5);
        }
    } else {
        Vz_cuts = DSCuts("Vertex z component", "", "", "1e cut", 0, -5, 5);
        Vz_cuts_FD = DSCuts("Vertex z component", "FD", "", "1e cut", 0, -5, 5);
        Vz_cuts_CD = DSCuts("Vertex z component", "CD", "", "1e cut", 0, -5, 5);
    }
}
#pragma endregion

// ConfiguredVz_cuts function -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfiguredVz_cuts function */
void ExperimentParameters::ConfiguredVz_cuts(const std::string &sn) {
    if (basic_tools::FindSubstring(sn, "Uniform")) {  // Sample is uniform simulation
        if (basic_tools::FindSubstring(sn, "2070MeV")) {
            if (sn == "Uniform_1e_sample_2070MeV") {
                dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -9999, 9999);
                dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -9999, 9999);
                dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -9999, 9999);
                // dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -9999, 9999);
                // dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -9999, 9999);
                // dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -9999, 9999);
            } else if (sn == "Uniform_ep_sample_2070MeV") {
                dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -2.5, 2.5);
                dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -2.5, 2.5);
                dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -2.5, 2.5);
                // dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -4, 2);
                // dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -4, 2);
                // dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -1, 3);
            } else if (sn == "Uniform_en_sample_2070MeV") {
                dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -9999, 9999);
                dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -9999, 9999);
                dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -9999, 9999);
                // dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -9999, 9999);
                // dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -9999, 9999);
                // dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -9999, 9999);
            }
        } else if (basic_tools::FindSubstring(sn, "4029MeV")) {
            if (sn == "Uniform_1e_sample_4029MeV") {
                dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -9999, 9999);
                dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -9999, 9999);
                dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -9999, 9999);
                // dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
                // dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 4);
                // dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 4);
            } else if (sn == "Uniform_ep_sample_4029MeV") {
                dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -2.5, 2.5);
                dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -2.5, 2.5);
                dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -2.5, 2.5);
                // dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
                // dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 4);
                // dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 4);
            } else if (sn == "Uniform_en_sample_4029MeV") {
                dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -9999, 9999);
                dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -9999, 9999);
                dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -9999, 9999);
                // dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
                // dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 4);
                // dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 4);
            }
        } else if (basic_tools::FindSubstring(sn, "5986MeV")) {
            if (sn == "Uniform_1e_sample_5986MeV") {
                dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -9999, 9999);
                dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -9999, 9999);
                dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -9999, 9999);
                // dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
                // dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 4);
                // dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 4);
            } else if (sn == "Uniform_ep_sample_5986MeV") {
                dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -2.5, 2.5);
                dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -2.5, 2.5);
                dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -2.5, 2.5);
                // dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
                // dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 4);
                // dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 4);
            } else if (sn == "Uniform_en_sample_5986MeV") {
                dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -9999, 9999);
                dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -9999, 9999);
                dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -9999, 9999);
                // dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
                // dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 4);
                // dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 4);
            }
        }
    } else if (basic_tools::FindSubstring(sn, "simulation")) {  // Sample is simulation
        if (sn == "C12x4_simulation_G18_Q204_6GeV") {           // 4-foil
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 4);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 4);
        } else if (sn == "C12_simulation_G18_Q204_6GeV") {  // 1-foil
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 4);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 4);
        } else if (sn == "C12_simulation_G18_Q2_0_02_2070MeV" || sn == "C12_simulation_G18_Q2_0_02_2070MeV_wFC") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -6, 6);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -6, 6);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -4, 4);
        } else if (sn == "C12_simulation_G18_Q2_0_25_4029MeV" || sn == "C12_simulation_G18_Q2_0_25_4029MeV_wFC") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -6, 6);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -6, 6);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -4, 4);
        } else if (sn == "C12_simulation_SuSa_Q2_0_02_2070MeV" || sn == "C12_simulation_SuSa_Q2_0_02_2070MeV_wFC") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -6, 6);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -6, 6);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -4, 4);
        } else if (sn == "C12_simulation_SuSa_Q2_0_25_4029MeV" || sn == "C12_simulation_SuSa_Q2_0_25_4029MeV_wFC") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -6, 6);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -6, 6);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -4, 4);
        }
    } else if (basic_tools::FindSubstring(sn, "data")) {  // Sample is data
        if (sn == "Ar40_data_2GeV_run_015672") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -4, 8);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -2, 6);
        } else if (sn == "Ar40_data_4GeV_run_015743") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -8, 4);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -4, 2);
        } else if (sn == "Ar40_data_6GeV_run_015792") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -8, 4);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -4, 2);
        } else if (sn == "C12_data_2GeV_run_015664") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -3, 5);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -2, 5);
        } else if (sn == "C12_data_4GeV_run_015778") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -6, 3);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -4, 2);
        } else if (sn == "D2_data_2GeV") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 7);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -2, 5);
        } else if (sn == "D2_data_6GeV_run_015443") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -7, 3);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 3);
        } else if (sn == "H1_data_2GeV_run_015634") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -3, 6);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -2, 5);
        } else if (sn == "H1_data_6GeV_run_015028") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -6, 2);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -4, 2);
        } else if (basic_tools::FindSubstring(sn, "C12x4_data_6GeV_run_0151")) {
            if (basic_tools::FindSubstring(sn, "C12x4_data_6GeV_run_015186")) {  // 4-foil
                dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
                dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 4);
                dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 4);
            } else if (basic_tools::FindSubstring(sn, "C12x4_data_6GeV_run_015187")) {  // 4-foil
                dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
                dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 4);
                dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 4);
            } else if (basic_tools::FindSubstring(sn, "C12x4_data_6GeV_run_015188")) {  // 4-foil
                dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
                dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 4);
                dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 4);
            } else {
                dVz_cuts = dVz_cuts_def;
                dVz_cuts_FD = dVz_cuts_FD_def;
                dVz_cuts_CD = dVz_cuts_CD_def;
            }
        } else if (sn == "C12x4_data_6GeV") {  // Sample is data (full 4-foil run)
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -5, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -5, 4);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -5, 4);
        } else if (sn == "LH2_data_6GeV_run_015032") {
            dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -8, 4);
            dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -8, 4);
            dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -8, 4);
        }
    } else {
        dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -8, 4);
        dVz_cuts_FD = DSCuts("dVz", "FD", "", "1e cut", 0, -8, 4);
        dVz_cuts_CD = DSCuts("dVz", "CD", "", "1e cut", 0, -8, 4);
    }
}
#pragma endregion

// ConfigureBeamEnergy function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigureBeamEnergy function */
double ExperimentParameters::ConfigureBeamEnergy(const std::string &sn) {
    // return GetBeamEnergyFromString(sn);

    double be;

    if (basic_tools::FindSubstring(sn, "598636MeV") || basic_tools::FindSubstring(sn, "598636mev") || basic_tools::FindSubstring(sn, "598636") || basic_tools::FindSubstring(sn, "5986MeV") ||
        basic_tools::FindSubstring(sn, "5986mev") || basic_tools::FindSubstring(sn, "5986") || basic_tools::FindSubstring(sn, "6GeV") || basic_tools::FindSubstring(sn, "6Gev") ||
        basic_tools::FindSubstring(sn, "6gev")) {
        be = 5.98636;
    } else if (basic_tools::FindSubstring(sn, "402962MeV") || basic_tools::FindSubstring(sn, "402962mev") || basic_tools::FindSubstring(sn, "402962") ||
               basic_tools::FindSubstring(sn, "4029MeV") || basic_tools::FindSubstring(sn, "4029mev") || basic_tools::FindSubstring(sn, "4029") || basic_tools::FindSubstring(sn, "4GeV") ||
               basic_tools::FindSubstring(sn, "4Gev") || basic_tools::FindSubstring(sn, "4gev")) {
        be = 4.02962;
    } else if (basic_tools::FindSubstring(sn, "207052MeV") || basic_tools::FindSubstring(sn, "207052mev") || basic_tools::FindSubstring(sn, "207052") ||
               basic_tools::FindSubstring(sn, "2070MeV") || basic_tools::FindSubstring(sn, "2070mev") || basic_tools::FindSubstring(sn, "2070") || basic_tools::FindSubstring(sn, "2GeV") ||
               basic_tools::FindSubstring(sn, "2Gev") || basic_tools::FindSubstring(sn, "2gev")) {
        be = 2.07052;
    }

    return be;
}
#pragma endregion

// GetBeamEnergy function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetBeamEnergy function */
double ExperimentParameters::GetBeamEnergy() { return BeamEnergy; }
#pragma endregion

// AddToHipoChain function ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Description */
// This is the old function used to add runs to the HipoChain
void ExperimentParameters::AddToHipoChain(HipoChain &chain, const std::string &sn, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile) {
    bool PrintOut = true;

    if (DataSample) {
        if (sn == "C12x4_data_6GeV") {
            if (AnalyzeFileSample == "") {
                /* Data in cache/clas12/rg-m/production/pass1/6gev/Cx4/dst/recon */
                vector<string> Runs = {"015186", "015187", "015188", "015189", "015190", "015191", "015192", "015193", "015194", "015196", "015199", "015200", "015202", "015203", "015204",
                                       "015205", "015206", "015207", "015210", "015212", "015213", "015214", "015215", "015217", "015219", "015220", "015221", "015223", "015224", "015225",
                                       "015226", "015228", "015234", "015235", "015236", "015238", "015239", "015240", "015241", "015242", "015243", "015245", "015246", "015247", "015248",
                                       "015249", "015250", "015252", "015253", "015254", "015255", "015257", "015258", "015259", "015260", "015261", "015262", "015263", "015264", "015265",
                                       "015266", "015269", "015270", "015271", "015272", "015273", "015274", "015275", "015278", "015279", "015280", "015282", "015283", "015284", "015286",
                                       "015287", "015288", "015289", "015290", "015291", "015292", "015293", "015294", "015295", "015296", "015298", "015300", "015301", "015302", "015303",
                                       "015304", "015305", "015306", "015307", "015308", "015309", "015310", "015311", "015312", "015313", "015314", "015316", "015317"};

                for (int i = 0; i < Runs.size(); i++) {
                    std::string TempAnalyzeFile = "/" + AnalyzeFilePath + "/" + Runs.at(i) + "/*.hipo";
                    chain.Add(TempAnalyzeFile.c_str());

                    if (PrintOut) { cout << TempAnalyzeFile << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { cout << "\n"; }
            }
        } else if (sn == "D2_data_2GeV") {
            if (AnalyzeFileSample == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                vector<string> Runs = {"015567", "015573", "015578", "015583", "015590", "015595", "015602", "015608", "015613", "015618", "015624", "015568", "015574", "015579",
                                       "015586", "015591", "015598", "015603", "015609", "015614", "015619", "015625", "015569", "015575", "015580", "015587", "015592", "015599",
                                       "015604", "015610", "015615", "015620", "015626", "015570", "015576", "015581", "015588", "015593", "015600", "015606", "015611", "015616",
                                       "015622", "015627", "015572", "015577", "015582", "015589", "015594", "015601", "015607", "015612", "015617", "015623"};

                for (int i = 0; i < Runs.size(); i++) {
                    std::string TempAnalyzeFile = "/" + AnalyzeFilePath + "/" + Runs.at(i) + "/*.hipo";
                    chain.Add(TempAnalyzeFile.c_str());

                    if (PrintOut) { cout << TempAnalyzeFile << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { cout << "\n"; }
            }
        } else {
            chain.Add(AnalyzeFile.c_str());
        }
    } else if (SimulationSample) {
        chain.Add(AnalyzeFile.c_str());

        if (PrintOut) { cout << AnalyzeFile << " directory added to HipoChain!\n\n"; }
    }
}
#pragma endregion

// AddToHipoChainFromList function ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Description */
void ExperimentParameters::AddToHipoChainFromList(HipoChain &chain, const std::string &sn, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample,
                                                  const std::string &AnalyzeFile) {
    bool PrintOut = true;

    if (DataSample) {
        if (sn == "H1_data_2GeV") {
            if (AnalyzeFileSample == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                for (int i = 0; i < lists::H1_data_2GeV_runs.size(); i++) {
                    std::string TempAnalyzeFile = "/" + AnalyzeFilePath + "/" + lists::H1_data_2GeV_runs.at(i) + "/*.hipo";
                    chain.Add(TempAnalyzeFile.c_str());

                    if (PrintOut) { cout << TempAnalyzeFile << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { cout << "\n"; }
            }
        } else if (sn == "D2_data_2GeV") {
            if (AnalyzeFileSample == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                for (int i = 0; i < lists::D2_data_2GeV_runs.size(); i++) {
                    std::string TempAnalyzeFile = "/" + AnalyzeFilePath + "/" + lists::D2_data_2GeV_runs.at(i) + "/*.hipo";
                    chain.Add(TempAnalyzeFile.c_str());

                    if (PrintOut) { cout << TempAnalyzeFile << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { cout << "\n"; }
            }
        } else if (sn == "C12_data_2GeV") {
            if (AnalyzeFileSample == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                for (int i = 0; i < lists::C12_data_2GeV_runs.size(); i++) {
                    std::string TempAnalyzeFile = "/" + AnalyzeFilePath + "/" + lists::C12_data_2GeV_runs.at(i) + "/*.hipo";
                    chain.Add(TempAnalyzeFile.c_str());

                    if (PrintOut) { cout << TempAnalyzeFile << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { cout << "\n"; }
            }
        } else if (sn == "Ar40_data_2GeV") {
            if (AnalyzeFileSample == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                for (int i = 0; i < lists::Ar40_data_2GeV_runs.size(); i++) {
                    std::string TempAnalyzeFile = "/" + AnalyzeFilePath + "/" + lists::Ar40_data_2GeV_runs.at(i) + "/*.hipo";
                    chain.Add(TempAnalyzeFile.c_str());

                    if (PrintOut) { cout << TempAnalyzeFile << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { cout << "\n"; }
            }
        } else if (sn == "C12_data_4GeV") {
            if (AnalyzeFileSample == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                for (int i = 0; i < lists::C12_data_4GeV_runs.size(); i++) {
                    std::string TempAnalyzeFile = "/" + AnalyzeFilePath + "/" + lists::C12_data_4GeV_runs.at(i) + "/*.hipo";
                    chain.Add(TempAnalyzeFile.c_str());

                    if (PrintOut) { cout << TempAnalyzeFile << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { cout << "\n"; }
            }
        } else if (sn == "Ar40_data_4GeV") {
            if (AnalyzeFileSample == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                for (int i = 0; i < lists::Ar40_data_4GeV_runs.size(); i++) {
                    std::string TempAnalyzeFile = "/" + AnalyzeFilePath + "/" + lists::Ar40_data_4GeV_runs.at(i) + "/*.hipo";
                    chain.Add(TempAnalyzeFile.c_str());

                    if (PrintOut) { cout << TempAnalyzeFile << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { cout << "\n"; }
            }
        } else if (sn == "H1_data_6GeV") {
            if (AnalyzeFileSample == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                for (int i = 0; i < lists::H1_data_6GeV_runs.size(); i++) {
                    std::string TempAnalyzeFile = "/" + AnalyzeFilePath + "/" + lists::H1_data_6GeV_runs.at(i) + "/*.hipo";
                    chain.Add(TempAnalyzeFile.c_str());

                    if (PrintOut) { cout << TempAnalyzeFile << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { cout << "\n"; }
            }
        } else if (sn == "D2_data_6GeV") {
            if (AnalyzeFileSample == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                for (int i = 0; i < lists::D2_data_6GeV_runs.size(); i++) {
                    std::string TempAnalyzeFile = "/" + AnalyzeFilePath + "/" + lists::D2_data_6GeV_runs.at(i) + "/*.hipo";
                    chain.Add(TempAnalyzeFile.c_str());

                    if (PrintOut) { cout << TempAnalyzeFile << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { cout << "\n"; }
            }
        } else if (sn == "C12x4_data_6GeV") {
            if (AnalyzeFileSample == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                for (int i = 0; i < lists::C12x4_data_6GeV_runs.size(); i++) {
                    std::string TempAnalyzeFile = "/" + AnalyzeFilePath + "/" + lists::C12x4_data_6GeV_runs.at(i) + "/*.hipo";
                    chain.Add(TempAnalyzeFile.c_str());

                    if (PrintOut) { cout << TempAnalyzeFile << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { cout << "\n"; }
            }
        } else if (sn == "Ar40_data_6GeV") {
            if (AnalyzeFileSample == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                for (int i = 0; i < lists::Ar40_data_6GeV_runs.size(); i++) {
                    std::string TempAnalyzeFile = "/" + AnalyzeFilePath + "/" + lists::Ar40_data_6GeV_runs.at(i) + "/*.hipo";
                    chain.Add(TempAnalyzeFile.c_str());

                    if (PrintOut) { cout << TempAnalyzeFile << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { cout << "\n"; }
            }
        } else {
            chain.Add(AnalyzeFile.c_str());
        }
    } else if (SimulationSample) {
        chain.Add(AnalyzeFile.c_str());

        if (PrintOut) { cout << AnalyzeFile << " directory added to HipoChain!\n\n"; }
    }
}
#pragma endregion

#endif  // EXPERIMENTPARAMETERS_H