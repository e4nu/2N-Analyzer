//
// Obtained from Ron Wagner on 06/11/2025.
//

#ifndef MY_HIT
#define MY_HIT

struct NeutronHit {
    int hitIndex;
    int pindex;
    double x;
    double y;
    double z;
    double Px;
    double Py;
    double Pz;
    double path;
    double tof;
    double dE;
    int layer;
    int detector;
    int sector;
    int num_of_hits_ahead;  // the num of neutrals ahead if greter than 2 remove
    bool blocking = false;

    // Reset function
    void reset() {
        hitIndex = -1;
        pindex = -1;
        x = -999.;
        y = -999.;
        z = -999.;
        Px = -999.;
        Py = -999.;
        Pz = -999.;
        tof = -999.;
        path = -999.;
        dE = -999.;
        layer = 0;
        detector = 0;
        sector = 0;
        num_of_hits_ahead = 0;
        bool blocking = false;
    }
};

#endif
