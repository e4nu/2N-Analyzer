//
// Created by Alon Sportes on 05/03/2026.
//

#ifndef EVENTCOUNTERS_H
#define EVENTCOUNTERS_H

struct EventCounters {
    int num_of_events = 0, num_of_events_without_any_e = 0, num_of_events_with_any_e = 0;
    int num_of_QEL_events = 0, num_of_MEC_events = 0, num_of_RES_events = 0, num_of_DIS_events = 0;

    int num_of_events_with_at_least_1e = 0, num_of_events_with_exactly_1e = 0, num_of_events_with_exactly_1e_from_file = 0, num_of_events_more_then_1e = 0;
    int num_of_QEL_events_1e_cut = 0, num_of_MEC_events_1e_cut = 0, num_of_RES_events_1e_cut = 0, num_of_DIS_events_1e_cut = 0;

    int num_of_events_with_nFD_CLA12 = 0;
    int num_of_events_with_nFD_CLA12_PCAL = 0, num_of_events_with_nFD_CLA12_ECIN = 0, num_of_events_with_nFD_CLA12_ECOUT = 0, num_of_events_with_nFD_CLA12_EC = 0;
    int num_of_events_with_phFD_CLA12 = 0;
    int num_of_events_with_phFD_CLA12_PCAL = 0, num_of_events_with_phFD_CLA12_ECIN = 0, num_of_events_with_phFD_CLA12_ECOUT = 0, num_of_events_with_phFD_CLA12_EC = 0;

    int num_of_events_1n_in_FD = 0, num_of_events_2n_in_FD = 0, num_of_events_3n_in_FD = 0, num_of_events_Xn_in_FD = 0;

    int num_of_events_1e1p_all = 0, num_of_events_with_1e1p = 0;

    int num_of_events_1p_inFD = 0;

    int num_of_events_1n_inFD = 0, num_of_events_1n_inFD_AV = 0, num_of_events_1n_inFD_AV_wmt1gTLn = 0;

    int num_of_events_1e2p_all = 0, num_of_events_1e2p_all_woFDphotons = 0, num_of_events_with_1e2p = 0;
    int num_of_events_2p_wFakeProtons = 0, num_of_events_2p = 0;

    int num_of_events_pFDpCD = 0;
    int num_of_events_with_1epFDpCD = 0, num_of_events_with_1epFDpFD = 0, num_of_events_with_1epCDpCD = 0;

    int num_of_events_nFDpCD = 0;
    int num_of_events_nFDpCD_AV = 0;
};

#endif  // EVENTCOUNTERS_H
