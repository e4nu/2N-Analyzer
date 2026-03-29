//
// Obtained from Ron Wagner on 06/11/2025.
// Edited by Alon Sportes.
//

#include <TVector3.h>
#include <stdio.h>

#include <iostream>
#include <vector>

#include "ChargedHit.h"
#include "NeutronHitStruct.cpp"

// should return vector of int that point to hit indexes that should be considered as neutrons
/*
struct NeutronHit{
    int hitIndex;
    float x;
    float y;
    float z;
    float time;
    int lay;
    int det;
    int sec;
    int comp;
    bool blocking=false;
};
*/

double TDIFF = 5.;
double TDIFF_tof = 5.;
// int SECDIFF = 3;
double ANGLEDIFF = 5.;
double Dif_Z = 1045.;
int sector_diff = 0;

// in this method, I will run over all neutron candidates and eliminated the ones that fall inside the cone due to charge particle
/*vector<NeutronHit> ClearNeutrons(vector<NeutronHit> neut, vector<NeutronHit> charged){
    vector<NeutronHit> tempNeut;
    tempNeut.clear();
    for(int i=0;i<charged.size();i++){

        for(int j=0;j<neut.size();j++){

        }
    }
}
*/
void AddNeutrons(std::vector<NeutronHit*> tof_n, std::vector<NeutronHit*> mid_n, std::vector<ChargedHit> vChrg, std::vector<NeutronHit*>& testLay, std::vector<NeutronHit*>& neut) {
    /*
    This function add neutral hits from the relevant test layer that pass the veto

    tof_n - neutral hits in the FTOF
    mid_n - neutral hits in the layers before the test layer and not the FTOF layer (first layer)
    vChrg - all the charged hits from FTOF to the test layer included
    testLay - the test layer of neutron hits
    neut - neutral hits that passed the veto.
    */
    bool found;
    //    cout<<" ===> size of test Layer = "<<testLay.size()<<"  size of blocking layer = "<<blockLay.size()<<endl;
    for (int i = 0; i < testLay.size(); i++) {
        TVector3 testVector(testLay.at(i)->x, testLay.at(i)->y, testLay.at(i)->z);
        double tof = testLay.at(i)->tof;
        int sector = testLay.at(i)->sector;
        found = false;
        //        cout<<" ----> i "<<i<<" (x,y,z,t, phi) "<<x<<", "<<y<<", "<<z<<", "<<t<<", "<<phi<<endl;

        // check that there was no neutral hit in the FTOF that fits test layer hit
        for (int j = 0; j < tof_n.size(); j++) {
            TVector3 tofVector(tof_n.at(j)->x, tof_n.at(j)->y, tof_n.at(j)->z);
            double new_tof = tof_n.at(j)->tof;
            double ang = tofVector.Angle(testVector) * TMath::RadToDeg();
            int new_sector = tof_n.at(j)->sector;
            if (sector - new_sector == sector_diff && ang < ANGLEDIFF && TMath::Abs(tof - new_tof) < TDIFF_tof) found = true;
        }

        // check that there was no charged hit near neutral hit
        for (int j = 0; j < vChrg.size(); j++) {
            TVector3 ChargedVector(vChrg.at(j).x, vChrg.at(j).y, vChrg.at(j).z);
            double new_tof = vChrg.at(j).tof;
            double ang = ChargedVector.Angle(testVector) * TMath::RadToDeg();
            int new_sector = vChrg.at(j).sector;
            if (sector - new_sector == sector_diff && ang < ANGLEDIFF && TMath::Abs(tof - new_tof) < TDIFF_tof) found = true;
        }

        // check that there was no neutral hit near the tested hit in layers before it
        for (int j = 0; j < mid_n.size(); j++) {
            TVector3 neutralVector(mid_n.at(j)->x, mid_n.at(j)->y, mid_n.at(j)->z);
            double new_tof = mid_n.at(j)->tof;
            double ang = neutralVector.Angle(testVector) * TMath::RadToDeg();
            int new_sector = mid_n.at(j)->sector;
            if (sector - new_sector == sector_diff && ang < ANGLEDIFF && TMath::Abs(tof - new_tof) < TDIFF_tof) {
                found = true;
                mid_n.at(j)->num_of_hits_ahead++;
            }
        }

        double xAvg = testLay.at(i)->x;
        double yAvg = testLay.at(i)->y;
        double zAvg = testLay.at(i)->z;
        double near_naibor = 1;
        // check if there are neutral hits in the same sector near each other
        if (!testLay.at(i)->blocking) {
            for (int j = i + 1; j < testLay.size(); j++) {
                TVector3 neutralVector(testLay.at(j)->x, testLay.at(j)->y, testLay.at(j)->z);
                double new_tof = testLay.at(j)->tof;
                double ang = neutralVector.Angle(testVector) * TMath::RadToDeg();
                int new_sector = testLay.at(j)->sector;
                if (sector - new_sector == sector_diff && ang < ANGLEDIFF && TMath::Abs(tof - new_tof) < TDIFF_tof && testLay.at(j)->blocking == false) {
                    near_naibor++;
                    xAvg += testLay.at(j)->x;
                    yAvg += testLay.at(j)->y;
                    zAvg += testLay.at(j)->z;
                    testLay.at(j)->blocking = true;
                }
            }
        }
        // if there was not found a particle behind it and there more than one near it
        if (found == false && near_naibor > 0 && !testLay.at(i)->blocking) {
            testLay.at(i)->x = xAvg / near_naibor;
            testLay.at(i)->y = yAvg / near_naibor;
            testLay.at(i)->z = zAvg / near_naibor;
        }

        // for(int j=0;j<blockLay.size();j++){
        //     float new_x = blockLay.at(j).x;
        //     float new_y = blockLay.at(j).y;
        //     float new_z = blockLay.at(j).z;
        //     float new_t = blockLay.at(j).time;
        //     float new_phi = TMath::ATan2(new_y,new_x)*180./TMath::Pi();
        //     if(new_phi<0) new_phi = new_phi+360;
        //     int new_sec = blockLay.at(j).sec;

        //     float tDiff = TMath::Abs(new_t - t);
        //     float xDiff = TMath::Abs(x-new_x);
        //     float yDiff = TMath::Abs(y-new_y);
        //     float zDiff = TMath::Abs(z-new_z);
        //     float rDiff = TMath::Sqrt(pow(x-new_x,2) + pow(y-new_y,2));
        //     float phiDiff = TMath::Abs(new_phi - phi);
        //     int secDiff = TMath::Abs(new_sec - sec);

        //     if(tDiff<TDIFF && (phiDiff<ANGLEDIFF || TMath::Abs(phiDiff - 360)<ANGLEDIFF) && zDiff<Dif_Z){
        //         found = true;
        //     }

        //     //            cout<<" j "<<j<<" (x,y,z,t, phi) "<<new_x<<", "<<new_y<<", "<<new_z<<", "<<new_t<<", "<<new_phi<<" found = "<<found<<endl;

        // }

        if (!found && !testLay.at(i)->blocking && testLay.at(i)->num_of_hits_ahead < 2) { neut.push_back(testLay.at(i)); }
    }

    // cout<<" ==== > "<<neut.size()<<endl;
}

std::vector<NeutronHit*> neutSelection(std::vector<NeutronHit*> nHit, std::vector<ChargedHit> cHit) {
    std::vector<int> output;
    output.clear();
    // cout<<" ====>>>> Size = "<<vCharge->size()<<endl;
    std::vector<NeutronHit*> vLay0n;  // FTOF all parts (we want to throw any neutral related to those hits)
    std::vector<NeutronHit*> vLay1n;  // PCAL
    std::vector<NeutronHit*> vLay2n;  // ECIN
    std::vector<NeutronHit*> vLay3n;  // ECOUT

    std::vector<ChargedHit> vLay0c;  // FTOF all
    std::vector<ChargedHit> vLay1c;  // PCAL
    std::vector<ChargedHit> vLay2c;  // ECIN
    std::vector<ChargedHit> vLay3c;  // ECOUT

    // introduce temporary vector that combine ALL preciding planes to be used as blocking layer. This is done to avoid cases when we have hit in layer 1 and 3, without hit in layer 2.
    std::vector<NeutronHit*> vTempLayersNeutral;
    std::vector<ChargedHit> vTempLayersCharged;

    std::vector<NeutronHit*> vNeut;

    vNeut.clear();

    bool found = false;

    struct NeutronHit allHits;

    // neutral hits
    for (int i = 0; i < nHit.size(); i++) {
        int d = nHit.at(i)->detector;
        int l = nHit.at(i)->layer;

        // FTOF Detector num in clas12 is 12 and ECAL && PCAL num is 7
        if (d == 12) vLay0n.push_back(nHit.at(i));
        if (d == 7 && l == 1) vLay1n.push_back(nHit.at(i));
        if (d == 7 && l == 2) vLay2n.push_back(nHit.at(i));
        if (d == 7 && l == 3) vLay3n.push_back(nHit.at(i));
    }

    // charged hits
    for (int i = 0; i < cHit.size(); i++) {
        int d = cHit.at(i).detector;
        int l = cHit.at(i).layer;

        // FTOF Detector num in clas12 is 12 and ECAL && PCAL num is 7
        if (d == 12) vLay0c.push_back(cHit.at(i));
        if (d == 7 && l == 1) vLay1c.push_back(cHit.at(i));
        if (d == 7 && l == 2) vLay2c.push_back(cHit.at(i));
        if (d == 7 && l == 3) vLay3c.push_back(cHit.at(i));
    }

    vNeut.clear();
    //    cout<<" Initial size of Neutrons (should be zero) = "<<vNeut.size()<<endl;
    //    cout<<" Starting layer 3: ========== "<<endl;

    // Whe start from neutral hits in ECOUT and go until PCAL
    vTempLayersNeutral.clear();
    vTempLayersCharged.clear();

    // ECOUT hits
    // charged
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay0c.begin(), vLay0c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay1c.begin(), vLay1c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay2c.begin(), vLay2c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay3c.begin(), vLay3c.end());

    // neutral
    vTempLayersNeutral.insert(vTempLayersNeutral.end(), vLay1n.begin(), vLay1n.end());
    vTempLayersNeutral.insert(vTempLayersNeutral.end(), vLay2n.begin(), vLay2n.end());

    // OLD - AddNeutrons(vLay0,vTempLayers, vLay3,vCharge,vNeut);
    // NEW
    AddNeutrons(vLay0n, vTempLayersNeutral, vTempLayersCharged, vLay3n, vNeut);

    vTempLayersCharged.clear();
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay0c.begin(), vLay0c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay1c.begin(), vLay1c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay2c.begin(), vLay2c.end());

    AddNeutrons(vLay0n, vLay1n, vTempLayersCharged, vLay2n, vNeut);

    vTempLayersCharged.clear();
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay0c.begin(), vLay0c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay1c.begin(), vLay1c.end());

    vTempLayersNeutral.clear();
    AddNeutrons(vLay0n, vTempLayersNeutral, vTempLayersCharged, vLay1n, vNeut);
    //    AddNeutrons(vLay0,vLay2, vLay3,vNeut);

    //    cout<<" after layer 3 = "<<vNeut.size()<<endl<<endl;

    //    cout<<" Starting layer 2: ========== "<<endl;
    // AddNeutrons(vLay0,vLay1, vLay2,vCharge,vNeut);
    //    cout<<" after layer 2 = "<<vNeut.size()<<endl<<endl;

    //    cout<<" Starting layer 1: ========== "<<endl;
    // AddNeutrons(vLay0,vLay0, vLay1,vCharge,vNeut);
    //    cout<<" after layer 1 = "<<vNeut.size()<<endl<<endl;

    // cout<<" Neutrons FINAL size = "<<vNeut.size()<<endl;

    //============================

    for (int i = 0; i < vNeut.size(); i++) { output.push_back(vNeut.at(i)->hitIndex); }

    return vNeut;
}
