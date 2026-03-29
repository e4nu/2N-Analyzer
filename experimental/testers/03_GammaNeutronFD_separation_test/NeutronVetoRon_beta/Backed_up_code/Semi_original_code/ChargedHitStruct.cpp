//
// Obtained from Ron Wagner on 06/11/2025.
//

#ifndef CHARGE_HIT
#define CHARGE_HIT

struct ChargedHit {
    int hitIndex;
    int pindex;
    int charge;
    int PID;
    int detector;
    int layer;
    int sector;
    double x;
    double y;
    double z;
    double tof;
    double path;

    // Reset function
    void reset() {
        hitIndex = -1;
        pindex = -1;
        charge = -999;
        PID = 0;
        x = -999.;
        y = -999.;
        z = -999.;
        tof = -999.;
        path = -999.;
        layer = 0;
        detector = 0;
        sector = 0;
    }
};

#endif
