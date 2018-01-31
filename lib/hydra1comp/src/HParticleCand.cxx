#include "HParticleCand.h"

static const Float_t clight = 299792458.0; // velocity of light [m/s]

// RK meta match sigma square values for system 0 and system 1
static const Float_t sigma_dx_sys0 = 11.0*11.0;  // 11.3
static const Float_t sigma_dy_sys0 = 13.0*13.0;  // 15.0
static const Float_t sigma_dz_sys0 = 8.0*8.0;  // 10.0
static const Float_t sigma_dx_sys1 = 22.5*22.5;  // 22.5
static const Float_t sigma_dy_sys1 = 4.1*4.1;    // 4.1
static const Float_t sigma_dz_sys1 = 7.3*7.3;    // 7.3

HParticleCand::HParticleCand() :
    fSector(-1), fSystem(-1), fCharge(0),
    fTofRec(0), fBeta(-1), fMomentum(-1), fMass2(-1), fPhi(-1),
    fTheta(-1), fR(-1), fZ(-1), fChi2(-1),
    fDistanceToMetaHit(-1),
    fMdcdEdx(-1), fMdcdEdxIn(-1), fMdcdEdxOu(-1), fTofdEdx(-1),
    fRingCorr(0),
    fInnerSegmentChi2(-1), fOuterSegmentChi2(-1),
    fAngleToNearbyFittedInner(0), fAngleToNearbyUnfittedInner(0),
    fRingNumPads(0),
    fRingCentroid(-1), fRingPatternMatrix(-1),
    fRichPhi(-1), fRichTheta(-1), fRingAmplitude(0),
    fRingHouTra(0),
    fMetaMatchQuality(-1), fTofinoMultiplicity(0),
    fShowerSum0 (-1), fShowerSum1(-1), fShowerSum2(-1)
{
}

HParticleCand::HParticleCand(HPidTrackCand& cand)
{
    tcand = cand;
    hd = *cand.getHitData();   // hit information
    td = *cand.getTrackData(); // track canidate

    Float_t dxRkMeta = td.dxRkMeta;
    Float_t dyRkMeta = td.dyRkMeta;
    Float_t dzRkMeta = td.dzRkMeta;

    Float_t MetaMatchRKChiq = sqrt(
        (dxRkMeta*dxRkMeta/sigma_dx_sys0) +
        (dyRkMeta*dyRkMeta/sigma_dy_sys0) +
        (dzRkMeta*dzRkMeta/sigma_dz_sys0) );

    Float_t angLepFit = td.getAngleWithClosestCandidate(1,1);
    Float_t angLepUnFit = td.getAngleWithClosestCandidate(1,0);
    Float_t angHadFit = td.getAngleWithClosestCandidate(0,1);
    Float_t angHadUnFit = td.getAngleWithClosestCandidate(0,0);

    Float_t nearFit = 0.0;
    if (angLepFit > angHadFit)
        nearFit = - angHadFit;
    else
        nearFit = angLepFit;

    Float_t nearUnFit = 0.0;
    if (angLepUnFit > angHadUnFit)
        nearUnFit = - angHadUnFit;
    else
        nearUnFit = angLepUnFit;

    setSector(hd.nSector);
    setTofRec(-1.0);
    setRingCorr(hd.hasRingCorrelation[4] ? 1 : 0);
    setMdcdEdx(hd.fCombinedMdcdEdx);
    setMdcdEdxIn(hd.getInnerMdcdEdx());
    setMdcdEdxOu(hd.getOuterMdcdEdx());
    setTofdEdx(td.fCorrectedEloss[4]);
    setSystem(hd.iSystem);
    setCharge(td.nPolarity[4]);
    setMomentum(td.fMomenta[4]);
    setPhi(td.fRKPhi);
    setTheta(td.fRKTheta);
    setR(td.fTrackR[4]);
    setZ(td.fTrackZ[4]);
    setChi2(td.fRKChiSquare);
    setInnerSegmentChi2(hd.fInnerMdcChiSquare);
    setOuterSegmentChi2(hd.fOuterMdcChiSquare);
    setMetaMatchQuality(MetaMatchRKChiq);
    setTofinoMultiplicity(hd.getTofinoMult());
    setRichPhi(hd.getRichPhi());
    setRichTheta(hd.getRichTheta());
    setRingAmplitude(hd.getRingAmplitude());
    setRingHouTra(hd.getRingHouTra());
    setRingNumPads(hd.getRingPadNr());
    setRingCentroid(hd.getRingCentroid());
    setRingPatternMatrix(hd.getRingPatMat());
    setDistanceToMetaHit(td.getPathLength(4));
    setShowerSum0(hd.getShowerSum(0));
    setShowerSum1(hd.getShowerSum(1));
    setShowerSum2(hd.getShowerSum(2));
    setAngleToNearbyFittedInner(nearFit);
    setAngleToNearbyUnfittedInner(nearUnFit);

    setBeta( ( (getDistanceToMetaHit()/1000.0) / hd.getTof()*1e-9)/clight );
    setMass2( calcMassSq( hd.getTof()*1e-9,
                          getMomentum(),
                          getDistanceToMetaHit()/1000.0) );
}

Float_t HParticleCand::calcMassSq(Float_t Tof, Float_t Momentum, Float_t PathLength)
{
    // calculates mass*mass as a function of momentum, time-of-flight and path length
    Float_t Velocity = 0.0;
    Float_t MassSq   = 0.0;
    Float_t clight   = 299792458.0; // [m/s]
    Float_t BetaSq   = 0.0;
    Float_t GammaSq  = 0.0;
    if (Tof > 0.0)
    {
        Velocity = PathLength/Tof;
        BetaSq   = (Velocity/clight)*(Velocity/clight);
        GammaSq  = 1.0/(1.0-BetaSq);
        MassSq   = Momentum*Momentum/(BetaSq*GammaSq);
        return MassSq;
    }
    else
    {
        //cout << "WARNING: time-of-flight < 0: " << Tof << endl;
        return MassSq;
    }
}

ClassImp(HParticleCand);
