//
// Obtained from Ron Wagner on 06/11/2025.
// Edited by Alon Sportes.
//
// ChargedHitStruct.cpp
// -------------------
// A lightweight POD-like struct used to store information about a *charged* FD-associated
// detector hit (FTOF / ECAL) and derived quantities used by the neutron-veto framework.
//
// Notes:
// - This struct is filled from clas12root `region_part_ptr` particles.
// - The same hit information is stored both as scalar components (xHit, yHit, zHit, ...)
//   and as cached ROOT 3-vectors (TVector3) for cleaner downstream vector algebra.
// - Error handling follows the `SectorDebugTest` style: print file/line/function context,
//   print hit contents, and abort. This is intentional for debugging/data-quality checks.
//
// NOTE:
// - Despite the .cpp extension, this file is intentionally written as a header-style definition unit
//   (include guard + inline struct methods). It is meant to be included where needed.
//

#ifndef CHARGED_HIT  // Include guard: ChargedHit definition (header-style)
#define CHARGED_HIT  // Include guard: ChargedHit definition (header-style)

#include <algorithm>  // std::max
#include <cstdlib>    // std::exit
#include <iomanip>    // std::setw
#include <iostream>   // std::cout
#include <map>        // std::map
#include <sstream>    // std::ostringstream (formatted error messages)
#include <string>     // std::string
#include <utility>    // std::pair
#include <vector>     // std::vector

#include "TVector3.h"  // ROOT 3-vector for cached hit/vertex vectors

// Prefer C++20 std::source_location when available; otherwise fall back to __FILE__/__LINE__/__FUNCTION__.
#if __has_include(<source_location>) && (__cplusplus >= 202002L)  // Check header availability + C++20
    #include <source_location>                                    // std::source_location (call-site context)
    #define HAVE_SOURCE_LOCATION 1                                // Compile-time feature flag
#else                                                             // Fallback branch
    #define HAVE_SOURCE_LOCATION 0                                // Compile-time feature flag
#endif                                                            // End feature detection

// Include libraries:
#include "../../namespaces/general_utilities/utilities.h"  // bt utilities (BoolToString, FindSubstring, etc.)

// Include CLAS12 libraries:
#include "../../includes/clas12_include.h"  // clas12root types (region_part_ptr, clas12::PCAL, ...)

namespace bt = basic_tools;    // Short alias for general utilities namespace
namespace am = analysis_math;  // Short alias for math utilities namespace

// Container for a single charged-particle hit associated with FD detectors (FTOF/ECAL).
/// \struct ChargedHit
/// \brief POD-like container for a single FD-associated *charged* detector hit (FTOF/ECAL) plus cached vectors.
///
/// Stores scalar hit information and cached TVector3 objects (HitPos, Vtx, HitPath) to simplify downstream
/// veto/geometry calculations. Filled from a clas12root `region_part_ptr` with strict validation; any
/// inconsistency triggers a context-rich abort dump.
struct ChargedHit {
    // Common to both NeutralHit and ChargedHit:
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Indices:
    int hitIndex;  // Hit counter within the event
    int pindex;    // Index of the region_part_ptr particle in the allParticles vector

    // Detector flags of the hit:
    int detector;                                                                               // Detector ID in CLAS12 data banks (12 for FTOF and 7 for ECAL)
    int sector;                                                                                 // Sector ID in CLAS12 data banks (1 to 6)
    int layer;                                                                                  // Layer ID of Ron's veto (0 for FTOF, 1 for PCAL, 2 for ECIN, and 3 for ECOUT)
    std::map<int, std::string> Layers = {{0, "FTOF"}, {1, "PCAL"}, {2, "ECIN"}, {3, "ECOUT"}};  // Ron-veto layer index -> label

    // ECAL hit flags of region_part_ptr particle:
    bool clas12particle_hit_in_PCAL, clas12particle_hit_in_ECIN, clas12particle_hit_in_ECOUT;  // Cached ECAL hit presence flags
    short First_ECAL_clas12detlayer;                                                           // First ECAL layer that fired (clas12::* or -1)
    std::string First_ECAL_clas12detlayer_str;                                                 // First ECAL layer (string for printouts)
    short particle_clas12detlayer;                                                             // clas12 detlayer id for THIS hit
    std::string particle_clas12detlayer_str;                                                   // clas12 detlayer name for THIS hit (string)

    // ECAL module coordinates:
    double Lv, Lw;  // ECAL local coordinates (valid only for ECAL cal-bank)

    // Charged hit coordinates of region_part_ptr particle in the relevant detector:
    double xHit, yHit, zHit;  // Hit coordinates in the relevant detector layer (cm)
    TVector3 HitPos;          // Cached 3-vector of hit position (kept consistent with xHit/yHit/zHit)

    // Timeing information:
    // TODO: confirm that this is really the hit time!
    double HitTime;  // Hit time from sci/cal bank (units per clas12root)
    // TODO: is this ToF?
    double Time;  // Particle->getTime() (units per clas12root)

    // Path variables:
    // clas12path = Path of region_part_ptr particle
    // xHitPath, yHitPath, zHitPath = Path from the vertex to the neutron hit coordinates in the relevant detector (xHit, yHit, zHit), along each axis
    double clas12path;  // Path length from sci/cal bank (cm)

    // Momentum componenets of region_part_ptr particle:
    double Px, Py, Pz;  // Momentum components from Particle->par() (GeV/c)
    TVector3 P;         // Cached 3-vector momentum (kept consistent with Px/Py/Pz)

    // ChargedHit only:
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Charge of region_part_ptr particle:
    int charge;  // Particle charge from Particle->par()->getCharge()

    // PDG of region_part_ptr particle:
    int PID;  // PDG id from Particle->par()->getPid()

    double Vx, Vy, Vz;  // Particle vertex coordinates from Particle->par() (cm)
    TVector3 Vtx;       // Cached particle vertex position (cm)

    double path, xHitPath, yHitPath, zHitPath;  // Vertex->hit vector (cm) and its components
    TVector3 HitPath;                           // Cached (HitPos - Vtx) vector (cm)

    // TVector3 accessors
    const TVector3& GetHitPos() const { return HitPos; }    // Return cached hit position vector
    const TVector3& GetHitPath() const { return HitPath; }  // Return cached vertex->hit vector

    const TVector3& GetP() const { return P; }      // Return cached momentum vector
    const TVector3& GetVtx() const { return Vtx; }  // Return cached vertex vector

    void UpdateHitPath() { HitPath.SetXYZ(xHitPath, yHitPath, zHitPath); }

    /// \brief Reset all fields to invalid sentinels and keep cached vectors consistent.
    void reset() {
        // Common to both NeutralHit and ChargedHit:
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------
        hitIndex = -1, pindex = -1;  // Default invalid indices

        detector = 0, sector = 0, layer = 0;  // 0 treated as invalid sector by SectorDebugTest

        clas12particle_hit_in_PCAL = false, clas12particle_hit_in_ECIN = false, clas12particle_hit_in_ECOUT = false;  // Reset hit flags
        First_ECAL_clas12detlayer = -1;                                                                               // -1 means \"no ECAL layer\"
        First_ECAL_clas12detlayer_str = "";                                                                           // Empty label
        particle_clas12detlayer = -1;                                                                                 // -1 means \"no layer\"
        particle_clas12detlayer_str = "";                                                                             // Empty label

        Lv = -999., Lw = -999.;  // Default invalid Lv/Lw (not applicable until ECAL hit)

        xHit = -999., yHit = -999., zHit = -999.;  // Default invalid hit coordinates
        HitPos.SetXYZ(xHit, yHit, zHit);           // Keep vector cache consistent

        HitTime = -999., Time = -999.;  // Default invalid times

        clas12path = -999.;  // Default invalid bank path

        Px = -999., Py = -999., Pz = -999.;  // Default invalid momentum
        P.SetXYZ(Px, Py, Pz);                // Keep vector cache consistent

        path = -999., xHitPath = -999., yHitPath = -999., zHitPath = -999.;  // Default invalid vertex->hit path
        HitPath.SetXYZ(xHitPath, yHitPath, zHitPath);                        // Keep vector cache consistent

        // ChargedHit only:
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------
        charge = -999;  // Default invalid charge

        PID = 0;  // 0 used as \"undefined PID\" sentinel

        Vx = Vy = Vz = -999.;    // Default invalid vertex coordinates
        Vtx.SetXYZ(Vx, Vy, Vz);  // Keep vector cache consistent
    }

    /// \brief Fill this ChargedHit from a reconstructed particle and a selected detector layer.
    /// \param Particle region_part_ptr handle (must be valid, and Particle->par() must exist).
    /// \param i particle index in allParticles (bookkeeping).
    /// \param Hitnum hit counter within event (bookkeeping).
    /// \param Detector CLAS12 detector id (12=FTOF, 7=ECAL).
    /// \param Layer Ron-veto layer index (0=FTOF, 1=PCAL, 2=ECIN, 3=ECOUT).
    /// \param clas12detlayer clas12 detlayer id (e.g., clas12::FTOF1A, clas12::PCAL, ...).
    /// \param PCAL_hit/ECIN_hit/ECOUT_hit ECAL-hit presence flags used for \"first ECAL\" bookkeeping.
    void FillHitData(region_part_ptr& Particle, int i, int Hitnum, int Detector, int Layer, short clas12detlayer, const bool PCAL_hit, const bool ECIN_hit, const bool ECOUT_hit) {
        bool printOut = false;

        // Common to both NeutralHit and ChargedHit:
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------

        if (printOut) {
            std::cout << Detector << "\n";
            std::cout << Layer << "\n";
            std::cout << clas12detlayer << "\n";
        }

        // Guards:
        if (!Particle) { AbortWithContext("FillHitData: Particle is null"); }                // Particle handle must exist
        if (!Particle->par()) { AbortWithContext("FillHitData: Particle->par() is null"); }  // Particle kinematics must exist

        hitIndex = Hitnum, pindex = i;  // Store bookkeeping indices

        detector = Detector;                                                                                                    // Cache detector ID early (needed to choose the correct bank)
        sector = ((detector == 12) ? Particle->sci(clas12detlayer)->getSector() : Particle->cal(clas12detlayer)->getSector());  // Sector from the relevant bank
        layer = Layer;                                                                                                          // Cache Ron-veto layer index

        clas12particle_hit_in_PCAL = PCAL_hit, clas12particle_hit_in_ECIN = ECIN_hit, clas12particle_hit_in_ECOUT = ECOUT_hit;  // Cache ECAL flags

        First_ECAL_clas12detlayer = PCAL_hit    ? static_cast<short>(clas12::PCAL)   // Prefer PCAL if hit
                                    : ECIN_hit  ? static_cast<short>(clas12::ECIN)   // Else ECIN if hit
                                    : ECOUT_hit ? static_cast<short>(clas12::ECOUT)  // Else ECOUT if hit
                                                : static_cast<short>(-1);            // Else none

        First_ECAL_clas12detlayer_str = Clas12DetLayerName(Detector, First_ECAL_clas12detlayer);  // Human-readable first ECAL layer

        particle_clas12detlayer = clas12detlayer;                                    // Cache current clas12 detlayer
        particle_clas12detlayer_str = Clas12DetLayerName(Detector, clas12detlayer);  // Human-readable current detlayer

        if (detector == 7) {
            Lv = Particle->cal(clas12detlayer)->getLv();  // ECAL local v coordinate
            Lw = Particle->cal(clas12detlayer)->getLw();  // ECAL local w coordinate
        } else {
            Lv = -999.;  // Not applicable for FTOF
            Lw = -999.;  // Not applicable for FTOF
        }

        HitPos.SetXYZ((detector == 12) ? Particle->sci(clas12detlayer)->getX() : Particle->cal(clas12detlayer)->getX(),
                      (detector == 12) ? Particle->sci(clas12detlayer)->getY() : Particle->cal(clas12detlayer)->getY(),
                      (detector == 12) ? Particle->sci(clas12detlayer)->getZ() : Particle->cal(clas12detlayer)->getZ());

        xHit = HitPos.X(), yHit = HitPos.Y(), zHit = HitPos.Z();  // Cache scalar components for convenience

        // Hit time of region_part_ptr particle in the relevant detector:
        HitTime = (detector == 12) ? Particle->sci(clas12detlayer)->getTime() : Particle->cal(clas12detlayer)->getTime();  // Cache hit time from correct bank

        Time = Particle->getTime();  // Cache particle time (clas12root)

        // Path length and hit-path vector
        clas12path = (detector == 12) ? Particle->sci(clas12detlayer)->getPath() : Particle->cal(clas12detlayer)->getPath();  // Cache bank path length

        // Momentum (cache as TVector3 and keep scalars in sync)
        P.SetXYZ(Particle->par()->getPx(), Particle->par()->getPy(), Particle->par()->getPz());  // Cache momentum vector
        Px = P.X(), Py = P.Y(), Pz = P.Z();                                                      // Cache scalar components

        // ChargedHit only:
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------

        charge = Particle->par()->getCharge();  // Cache charge

        PID = Particle->par()->getPid();  // Cache PID

        Vx = Particle->par()->getVx(), Vy = Particle->par()->getVy(), Vz = Particle->par()->getVz();  // Cache vertex components
        Vtx.SetXYZ(Vx, Vy, Vz);                                                                       // Cache vertex vector

        HitPath = HitPos - Vtx;                                                  // Build vertex->hit vector
        path = HitPath.Mag();                                                    // Cache magnitude
        xHitPath = HitPath.X(), yHitPath = HitPath.Y(), zHitPath = HitPath.Z();  // Cache components

        // Guards:
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------

        if (hitIndex < 0) {                                               // Guard: hitIndex must be non-negative (negative implies uninitialized bookkeeping)
            std::ostringstream oss;                                       // Build an informative error message with the bad value
            oss << "FillHitData: invalid hitIndex (" << hitIndex << ")";  // Include the invalid hitIndex in the message
            AbortWithContext(oss.str());                                  // Print context + full hit dump and terminate (intentional hard fail for debugging)
        }  // Validate
        if (pindex < 0) {                                             // Guard: pindex must be non-negative (negative implies missing particle index in allParticles)
            std::ostringstream oss;                                   // Build an informative error message with the bad value
            oss << "FillHitData: invalid pindex (" << pindex << ")";  // Include the invalid pindex in the message
            AbortWithContext(oss.str());                              // Print context + full hit dump and terminate
        }  // Validate

        if (detector != 12 && detector != 7) {                            // Guard: only FTOF(12) and ECAL(7) are supported by this struct
            std::ostringstream oss;                                       // Build an informative error message with the bad value
            oss << "FillHitData: invalid detector (" << detector << ")";  // Include the invalid detector id in the message
            AbortWithContext(oss.str());                                  // Abort with context: detector id is outside the supported set
        }  // Validate detector
        if (sector == 0) {                                            // Guard: sector==0 is treated as invalid and indicates missing bank content
            std::ostringstream oss;                                   // Build an informative error message with the bad value
            oss << "FillHitData: invalid sector (" << sector << ")";  // Include the invalid sector in the message
            AbortWithContext(oss.str());                              // Abort: sector is required for geometry and matching logic
        }  // Validate sector
        if (layer != 0 && layer != 1 && layer != 2 && layer != 3) {  // Guard: Ron-veto layer index must be one of {0,1,2,3}
            std::ostringstream oss;                                  // Build an informative error message with the bad value
            oss << "FillHitData: invalid layer (" << layer << ")";   // Include the invalid layer index in the message
            AbortWithContext(oss.str());                             // Validate Ron-veto layer
        }
        if ((detector == 12 && layer != 0) || (detector == 7 && layer == 0)) {                                   // Guard: enforce detector<->layer consistency (FTOF uses 0; ECAL uses 1..3)
            std::ostringstream oss;                                                                              // Build mismatch message
            oss << "FillHitData: invalid layer (" << layer << ") does not match detector (" << detector << ")";  // Explain the mismatch
            AbortWithContext(oss.str());                                                                         // Abort with context
        }

        if (particle_clas12detlayer != 1 && particle_clas12detlayer != 2 && particle_clas12detlayer != 3 && particle_clas12detlayer != 4 && particle_clas12detlayer != 7) {
            // Guard: clas12detlayer must match known constants in clas12defs.h (FTOF1A=1, FTOF1B=2, FTOF2=3, PCAL=1, ECIN=4, ECOUT=7)
            std::ostringstream oss;                                                            // Build an informative error message with the bad value
            oss << "FillHitData: invalid clas12detlayer (" << particle_clas12detlayer << ")";  // Include the invalid detlayer id
            AbortWithContext(oss.str());                                                       // Validate detlayer id
        }
        if (particle_clas12detlayer_str == "") {                                                                // Guard: string mapping must succeed so debug printouts are meaningful
            std::ostringstream oss;                                                                             // Build an informative error message
            oss << "FillHitData: invalid particle_clas12detlayer_str (" << particle_clas12detlayer_str << ")";  // Include the (empty) label
            AbortWithContext(oss.str());                                                                        // Validate name mapping
        }

        if (detector == 7) {                                      // ECAL-only validations (Lv/Lw exist only for cal-bank hits)
            if (Lv <= 0 && PID == 11) {                           // Guard: for electrons, Lv must be positive (0 often means missing cal-bank content)
                std::ostringstream oss;                           // Build an informative error message with the bad value
                oss << "FillHitData: invalid Lv (" << Lv << ")";  // Include the invalid Lv
                AbortWithContext(oss.str());                      // Abort: Lv is required for ECAL edge and quality cuts
            }  // Validate
            if (Lw <= 0 && PID == 11) {                           // Guard: for electrons, Lw must be positive (0 often means missing cal-bank content)
                std::ostringstream oss;                           // Build an informative error message with the bad value
                oss << "FillHitData: invalid Lw (" << Lw << ")";  // Include the invalid Lw
                AbortWithContext(oss.str());                      // Abort: Lw is required for ECAL edge and quality cuts
            }  // Validate
        }

        if (HitPos.Mag() == 0) {     // Reject zero vector: indicates missing/invalid bank coordinates (x=y=z=0)
            std::ostringstream oss;  // Build an informative error message with the full vector components
            oss << "FillHitData: invalid |HitPos| (HitPos(x,y,z) = " << HitPos.X() << ", " << HitPos.Y() << ", " << HitPos.Z() << ")";  // Include coordinates
            AbortWithContext(oss.str());                                                                                                // Abort with context + dump
        }

        if (HitTime == 0) {                                             // Guard: bank hit time should not be exactly zero for real reconstructed hits
            std::ostringstream oss;                                     // Build an informative error message with the bad value
            oss << "FillHitData: invalid HitTime (" << HitTime << ")";  // Include the invalid HitTime
            AbortWithContext(oss.str());                                // Abort: timing is used for TOF and matching
        }  // Validate

        if (Time == 0) {                                          // Guard: Particle->getTime() should not be exactly zero for real reconstructed particles
            std::ostringstream oss;                               // Build an informative error message with the bad value
            oss << "FillHitData: invalid Time (" << Time << ")";  // Include the invalid Time
            AbortWithContext(oss.str());                          // Abort: particle time is used for comparisons and consistency checks
        }  // Validate

        if (clas12path == 0) {                                                // Guard: bank path length should not be exactly zero for real detector hits
            std::ostringstream oss;                                           // Build an informative error message with the bad value
            oss << "FillHitData: invalid clas12path (" << clas12path << ")";  // Include the invalid path length
            AbortWithContext(oss.str());                                      // Abort: path is used for TOF-based beta and geometry
        }  // Validate

        if (abs(charge) != 1) {                    // Guard: charged tracks should have charge magnitude 1 in this FD charged-hit container
            std::ostringstream oss;                // Build an informative error message
            oss << "FillHitData: invalid charge";  // Keep message short; full details are in the hit dump
            AbortWithContext(oss.str());           // Abort: a non-unit charge breaks charged/neutral separation assumptions
        }  // Validate charged hit

        if (PID == 22 || PID == 2112 || PID == 0) {             // Guard: ChargedHit must not represent gamma, neutron, or unclassified PID
            std::ostringstream oss;                             // Build an informative error message with the bad value
            oss << "FillHitData: invalid PID (" << PID << ")";  // Include the invalid PID
            AbortWithContext(oss.str());                        // Validate not gamma/neutron/unclassified
        }  // Validate not gamma/neutron/unclassified
    }

    /// \brief Convert the stored clas12 detlayer id to a readable string (for debugging printouts).
    /// \param Detector CLAS12 detector id (12=FTOF, 7=ECAL).
    /// \param clas12detlayer clas12 layer id (numeric).
    /// \return A string like \"clas12::PCAL\" or empty string if not matched.
    std::string Clas12DetLayerName(int Detector, short clas12detlayer) const {
        std::string particle_clas12detlayer_str;  // Output label

        if (Detector == 12) {  // FTOF detector
            if (clas12detlayer == 1) {
                particle_clas12detlayer_str = "clas12::FTOF1A";  // FTOF 1A
            } else if (clas12detlayer == 2) {
                particle_clas12detlayer_str = "clas12::FTOF1B";  // FTOF 1B
            } else if (clas12detlayer == 3) {
                particle_clas12detlayer_str = "clas12::FTOF2";  // FTOF 2
            }
        } else if (Detector == 7) {  // ECAL detector
            if (clas12detlayer == 1) {
                particle_clas12detlayer_str = "clas12::PCAL";  // PCAL
            } else if (clas12detlayer == 4) {
                particle_clas12detlayer_str = "clas12::ECIN";  // ECIN
            } else if (clas12detlayer == 7) {
                particle_clas12detlayer_str = "clas12::ECOUT";  // ECOUT
            }
        }

        return particle_clas12detlayer_str;  // Return mapped label (or empty)
    }

    /// \brief Build the standard ChargedHit key/value dump items (used by PrintOut and any debug helpers).
    /// \return Vector of (key,value) pairs in the exact PrintOut order.
    std::vector<std::pair<std::string, std::string>> BuildPrintOutItems() const {
        std::vector<std::pair<std::string, std::string>> items;  // Accumulate aligned key/value printouts (key -> value string)

        // Build key/value strings (keep the exact content/format as the previous PrintOut)
        {
            std::ostringstream oss;
            oss << "ChargedHit";
            items.emplace_back("Hit type", oss.str());  // Indices
        }
        {
            std::ostringstream oss;
            oss << "{" << hitIndex << ", " << pindex << "}";
            items.emplace_back("{hitIndex, pindex}", oss.str());  // Indices
        }
        {
            std::ostringstream oss;
            oss << "{" << detector << ", " << sector << ", " << layer << " (" << Layers.at(layer) << ")}";
            items.emplace_back("{detector, sector, layer}", oss.str());  // IDs
        }
        {
            std::ostringstream oss;
            oss << "{" << bt::BoolToString(clas12particle_hit_in_PCAL) << ", " << bt::BoolToString(clas12particle_hit_in_ECIN) << ", " << bt::BoolToString(clas12particle_hit_in_ECOUT)
                << "}";
            items.emplace_back("{PCAL hit, ECIN hit, ECOUT hit}", oss.str());  // ECAL flags
        }

        items.emplace_back("First_ECAL_clas12detlayer", First_ECAL_clas12detlayer_str);  // First ECAL layer label
        items.emplace_back("particle_clas12detlayer", particle_clas12detlayer_str);      // This hit layer label

        {
            std::ostringstream oss;
            oss << "{" << Lv << " [cm], " << Lw << " [cm]}";
            items.emplace_back("{Lv, Lw}", oss.str());  // ECAL local coords
        }
        {
            std::ostringstream oss;
            oss << "{" << HitPos.Mag() << " [cm], " << HitPos.Theta() * TMath::RadToDeg() << " [deg], " << HitPos.Phi() * TMath::RadToDeg() << " [deg]}";
            items.emplace_back("{|HitPos|, Theta_HitPos, Phi_HitPos}", oss.str());  // Hit position magnitude + angles
        }
        {
            std::ostringstream oss;
            oss << HitTime << " [ns]";
            items.emplace_back("HitTime", oss.str());  // Bank hit time
        }
        {
            std::ostringstream oss;
            oss << Time << " [ns]";
            items.emplace_back("Time", oss.str());  // Particle time
        }
        {
            std::ostringstream oss;
            oss << clas12path << " [cm]";
            items.emplace_back("clas12path", oss.str());  // Bank path length
        }
        {
            std::ostringstream oss;
            oss << "{" << charge << ", " << PID << "}";
            items.emplace_back("{charge, PID}", oss.str());  // Particle ID info
        }
        {
            std::ostringstream oss;
            oss << "{" << Vtx.Mag() << " [cm], " << Vtx.Theta() * TMath::RadToDeg() << " [deg], " << Vtx.Phi() * TMath::RadToDeg() << " [deg]}";
            items.emplace_back("{|Vtx|, Vtx_HitPos, Vtx_HitPos}", oss.str());  // Vertex
        }
        {
            std::ostringstream oss;
            oss << "{" << HitPath.Mag() << " [cm], " << HitPath.Theta() * TMath::RadToDeg() << " [deg], " << HitPath.Phi() * TMath::RadToDeg() << " [deg]}";
            items.emplace_back("{|HitPath|, Vtx_HitPath, Vtx_HitPath}", oss.str());  // Vertex->hit vector
        }

        return items;
    }

    /// \brief Print a colorized diagnostic dump of the full NeutralHit content to stdout.
    /// \details Intended to be called automatically by AbortWithContext() on validation failures.
    void PrintOut(std::string PrintColor = "\033[33m", bool DebugPrint = false) {
        const auto items = BuildPrintOutItems();  // Build items once and print with aligned formatting
        PrintAlignedKV(items, PrintColor, !DebugPrint, true);
    }

    /// \brief Convert a value to a string using stream insertion (`operator<<`).
    /// \details This is the base overload: it does not append units and does not force numeric formatting.
    template <typename T>
    static std::string ToStr(const T& v) {
        std::ostringstream oss;
        oss << v;
        return oss.str();
    }

    /// \brief Convert a value to a string and append an arbitrary unit suffix.
    /// \param unit Unit string to append (e.g., "[ns]", "[cm]", "[deg]"). Empty string disables the suffix.
    template <typename T>
    static std::string ToStr(const T& v, const std::string& unit) {
        std::ostringstream oss;
        oss << v;
        if (!unit.empty()) { oss << " " << unit; }
        return oss.str();
    }

    /// \brief Convert a numeric value to a string with controlled precision and an optional unit suffix.
    /// \param precision Number of digits after the decimal point (fixed) or significant digits (scientific).
    /// \param fixed If true, use fixed formatting; otherwise use scientific formatting.
    template <typename T>
    static std::string ToStr(const T& v, const std::string& unit, int precision, bool fixed = true) {
        std::ostringstream oss;
        if (fixed) {
            oss << std::fixed << std::setprecision(precision);
        } else {
            oss << std::scientific << std::setprecision(precision);
        }
        oss << v;
        if (!unit.empty()) { oss << " " << unit; }
        return oss.str();
    }

    /// \brief Print a list of key/value pairs with the '=' signs aligned.
    /// \param items Vector of (key,value) pairs to print in the provided order.
    /// \param PrintColor ANSI color escape string (e.g., "\033[33m").
    /// \param resetColor If true, prints the reset escape ("\033[0m") at the end.
    /// \param leadingNewlines If true, prints two leading newlines before the first line.
    /// \param forcedKeyWidth If nonzero, forces the key width to this value for alignment across blocks.
    static void PrintAlignedKV(const std::vector<std::pair<std::string, std::string>>& items, const std::string& PrintColor = "\033[33m", bool resetColor = true, bool leadingNewlines = true,
                               size_t forcedKeyWidth = 0) {
        // Find max key length so all '=' align under the same column
        size_t maxKey = 0;
        for (const auto& kv : items) { maxKey = std::max(maxKey, kv.first.size()); }  // Compute maximum key width
        if (forcedKeyWidth > maxKey) { maxKey = forcedKeyWidth; }                     // Allow alignment across multiple blocks

        // Print color + optional leading spacing
        std::cout << PrintColor << (leadingNewlines ? "\n\n" : "");  // Apply color and optional spacing

        // Print all key/value pairs with aligned '='
        for (const auto& kv : items) {
            std::cout << std::left << std::setw(static_cast<int>(maxKey)) << kv.first  // Left-align key to max width
                      << " = " << kv.second << "\n";                                   // Print separator + value
        }

        // Reset terminal formatting (optional)
        if (resetColor) { std::cout << "\033[0m\n"; }  // Reset terminal formatting
    }

    /// \brief Print the standard NeutralHit dump and then print extra external debug variables with matching alignment.
    /// \param extra Vector of (name,value) pairs for arbitrary external variables (order preserved).
    /// \param PrintColor ANSI color escape string used for both blocks.
    /// \param DebugPrint If true, do not reset color between blocks (useful for grouped printing).
    void DebugPrintOut(const std::vector<std::pair<std::string, std::string>>& extra, std::string PrintColor = "\033[33m", bool DebugPrint = false) {
        const auto items = BuildPrintOutItems();  // Standard hit dump

        // Compute a shared key width across BOTH blocks so '=' aligns globally
        size_t sharedWidth = 0;
        for (const auto& kv : items) { sharedWidth = std::max(sharedWidth, kv.first.size()); }
        for (const auto& kv : extra) { sharedWidth = std::max(sharedWidth, kv.first.size()); }

        // Print the main block (do not reset color here; allow caller to control)
        PrintAlignedKV(items, PrintColor, false, true, sharedWidth);

        // Print the extra block (no extra leading newlines, reset color only if requested)
        PrintAlignedKV(extra, PrintColor, !DebugPrint, false, sharedWidth);
    }

    // AbortWithContext function (unified error printout with file/line/function)
#if HAVE_SOURCE_LOCATION
    /// \brief Print a context-rich error message + hit dump and abort the program.
    /// \param message Short description of the detected inconsistency.
    /// \param loc Call-site source location (file/line/function) captured by default.
    [[noreturn]] void AbortWithContext(const std::string& message, const std::source_location& loc = std::source_location::current()) const {
        std::cout << loc.file_name() << ":" << loc.line() << " in " << loc.function_name() << " -> \033[31mERROR!\033[0m " << message << "\n";  // Context + message
        std::cout << "Hit details:\n";                                                                                                          // Header for dump
        const_cast<ChargedHit*>(this)->PrintOut();                                                                                              // Dump full content (PrintOut is non-const)
        std::cout << "Aborting...\n\n";                                                                                                         // Termination message
        std::exit(1);                                                                                                                           // Hard abort (intentional)
    }
#else
    /// \brief Print a context-rich error message + hit dump and abort the program (no std::source_location available).
    /// \param message Short description of the detected inconsistency.
    /// \param file/line/func Call-site context (default args evaluated at the call site).
    [[noreturn]] void AbortWithContext(const std::string& message, const char* file = __FILE__, int line = __LINE__,
    #ifdef __GNUC__
                                       const char* func = "(unknown)"
    #else
                                       const char* func = __FUNCTION__
    #endif
    ) const {
        std::cout << "\n";
        std::cout << file << ":" << line << " in " << func << " -> \033[31mERROR!\033[0m " << message << "\n";  // Context + message
        std::cout << "Hit details:\n";                                                                          // Header for dump
        const_cast<ChargedHit*>(this)->PrintOut();                                                              // Dump full content
        std::cout << "Aborting...\n\n";                                                                         // Termination message
        std::exit(1);                                                                                           // Hard abort (intentional)
    }
#endif

    // SectorDebugTest function
#if HAVE_SOURCE_LOCATION
    /// \brief Sanity-check sector content and abort if invalid (sector==0).
    /// \param loc Call-site source location captured by default.
    void SectorDebugTest(const std::source_location& loc = std::source_location::current()) {
        if (sector == 0) { AbortWithContext("Sector can't be 0!", loc); }  // sector==0 indicates invalid/uninitialized hit
    }
#else
    /// \brief Sanity-check sector content and abort if invalid (sector==0) (no std::source_location available).
    /// \param file/line/func Call-site context (default args evaluated at the call site).
    void SectorDebugTest(const char* file = __FILE__, int line = __LINE__,
    #ifdef __GNUC__
                         const char* func = "(unknown)"
    #else
                         const char* func = __FUNCTION__
    #endif
    ) {
        if (sector == 0) { AbortWithContext("Sector can't be 0!", file, line, func); }  // Abort with call-site context
    }
#endif
};

#endif  // CHARGED_HIT