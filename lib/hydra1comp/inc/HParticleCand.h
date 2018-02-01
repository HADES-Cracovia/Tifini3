#ifndef HPARTICLECAND_H
#define HPARTICLECAND_H

#include "TMath.h"
#include "TObject.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

// Modified by A. Schmah 07.04.2009: Sim info added
// Modified by A. Schmah 17.04.2009: Forward wall info added
// Modified by A. Schmah 18.06.2009: New structure with Sim track and WallHit
// Modified by R. Lalik  04.04.2013: Ancestor chain implemented

#include "hpidtrackcand.h"
#include "hpidhitdata.h"
#include "hpidtrackdata.h"

static const Double_t LEPTON_MASS_P = 0.51099892;  // PDG: 2006

class HParticleCand : public TLorentzVector
{
private:
    Short_t fSector;            // sector information from MDC (0..5)
    Short_t fSystem;            // 0 == TOFino, 1 == TOF

    // Runge-Kutta track properties
    Short_t fCharge;            // particle's charge (+1,-1)
    Short_t fTofRec;            // 0 -> time-of-flight reconstruction was not succesfull, 1 -> tof+mdc dEdx, 2 -> only tof dEdx, 3 -> only Mdc dEdx
    Float_t fBeta;              // particle's beta
    Float_t fMomentum;          // particle's momentum [MeV]
    Float_t fMass2;             // particle's mass^2 [MeV^2]
    Float_t fPhi;               // track's phi in cave coordinates (0..360 deg)
    Float_t fTheta;             // track's theta in cave coordinates (0..90 deg)
    Float_t fR;                 // distance of closest point to beamline [mm]
    Float_t fZ;                 // z coordinate of closest point to beamline [mm]
    Float_t fChi2;              // tracking chi^2 (1e6 == tracking failed)
    Float_t fDistanceToMetaHit; // track's distance to meta hit [mm]

    Float_t fMdcdEdx;           // Mdc dE/dx for inner and outer segment
    Float_t fMdcdEdxIn;         // Mdc dE/dx for inner segment
    Float_t fMdcdEdxOu;         // Mdc dE/dx for outer segment
    Float_t fTofdEdx;           // Tof dE/dx

    // MDC related track properties
    UShort_t   fRingCorr;                   // ring - mdc correlation
    Float_t    fInnerSegmentChi2;           // MDC segment fitter chi^2 (-1 if not fitted)
    Float_t    fOuterSegmentChi2;           // MDC segment fitter chi^2 (-1 if not fitted)
    Float_t    fAngleToNearbyFittedInner;   // angle to closest fitted inner segment (negative angles indicate segments which belong to hadrons or neighbouring tracks) [deg]
    Float_t    fAngleToNearbyUnfittedInner; // angle to closest unfitted inner segment (negative angles indicate segments which belong to hadrons or neighbouring tracks) [deg]

    // properties of the RICH ring
    Int_t    fRingNumPads;         // number of fired pads
    Float_t  fRingCentroid;        // ring centroid
    Float_t  fRingPatternMatrix;   // pattern matrix of ring
    Float_t  fRichPhi;
    Float_t  fRichTheta;
    Int_t    fRingAmplitude;
    Int_t    fRingHouTra;

    // properties of meta hits
    Float_t  fMetaMatchQuality;    // distance of the outer segment to the meta hit [mm]
    Short_t  fTofinoMultiplicity;  // how often this TOFino paddle was hit in this event

    //properties of shower hits
    Float_t  fShowerSum0;          // charge sum of PreShower cluster (0)
    Float_t  fShowerSum1;          // charge sum of PreShower cluster (1)
    Float_t  fShowerSum2;          // charge sum of PreShower cluster (2)

public:
    HParticleCand();
    HParticleCand(HPidTrackCand & cand);
    ~HParticleCand() {}

    void calc4vectorProperties(Double_t mass = LEPTON_MASS_P)
    {
        // first, transform from spherical to cartesian coordinates
        SetXYZM( fMomentum * TMath::Sin( TMath::DegToRad() * fTheta )
                    * TMath::Cos( TMath::DegToRad() * fPhi ),
                    fMomentum * TMath::Sin( TMath::DegToRad() * fTheta )
                    * TMath::Sin( TMath::DegToRad() * fPhi ),
                    fMomentum * TMath::Cos( TMath::DegToRad() * fTheta ),
                    mass );
    }

    void    setSector(Int_t s)      { fSector = s;       }
    Short_t getSector() const       { return fSector;    }

    void    setSystem(Int_t s)      { fSystem = s;       }
    Short_t getSystem() const       { return fSystem;    }

    void    setRingCorr(UShort_t rc)        { fRingCorr = rc;            }
    UShort_t getRingCorr() const            { return fRingCorr;          }

    void    setTofRec(Short_t tr)           { fTofRec = tr;              }
    Short_t getTofRec() const               { return fTofRec;            }

    void    setMdcdEdx(Float_t d)           { fMdcdEdx = d;              }
    Float_t getMdcdEdx() const              { return fMdcdEdx;           }

    void    setMdcdEdxIn(Float_t d)         { fMdcdEdxIn = d;            }
    Float_t getMdcdEdxIn() const            { return fMdcdEdxIn;         }

    void    setMdcdEdxOu(Float_t d)         { fMdcdEdxOu = d;            }
    Float_t getMdcdEdxOu() const            { return fMdcdEdxOu;         }

    void    setTofdEdx(Float_t d)           { fTofdEdx = d;              }
    Float_t getTofdEdx() const              { return fTofdEdx;           }

    void    setCharge(Short_t c)            { fCharge = c;               }
    Short_t getCharge() const               { return fCharge;            }

    void    setBeta(Float_t b)              { fBeta = b;                 }
    Float_t getBeta() const                 { return fBeta;              }

    void    setMomentum(Float_t m)          { fMomentum = m;             }
    Float_t getMomentum() const             { return fMomentum;          }

    void    setMass2(Float_t m)             { fMass2 = m;                }
    Float_t getMass2() const                { return fMass2;             }

    void    setPhi(Float_t p)               { fPhi = p;                  }
    Float_t getPhi() const                  { return fPhi;               }

    void    setTheta(Float_t t)             { fTheta = t;                }
    Float_t getTheta() const                { return fTheta;             }

    void    setR(Float_t r)                 { fR = r;                    }
    Float_t getR() const                    { return fR;                 }

    void    setZ(Float_t z)                 { fZ = z;                    }
    Float_t getZ() const                    { return fZ;                 }

    void    setChi2(Float_t c)              { fChi2 = c;                 }
    Float_t getChi2() const                 { return fChi2;              }

    void    setDistanceToMetaHit(Float_t d) { fDistanceToMetaHit = d;    }
    Float_t getDistanceToMetaHit() const    { return fDistanceToMetaHit; }

    void     setInnerSegmentChi2(Float_t c)     { fInnerSegmentChi2 = c;    }
    Float_t  getInnerSegmentChi2() const        { return fInnerSegmentChi2; }

    void     setOuterSegmentChi2(Float_t c)     { fOuterSegmentChi2 = c;    }
    Float_t  getOuterSegmentChi2() const        { return fOuterSegmentChi2; }

    void     setAngleToNearbyFittedInner(Float_t a)   { fAngleToNearbyFittedInner = a;       }
    Float_t  getAngleToNearbyFittedInner() const      { return fAngleToNearbyFittedInner;    }

    void     setAngleToNearbyUnfittedInner(Float_t a) { fAngleToNearbyUnfittedInner = a;     }
    Float_t  getAngleToNearbyUnfittedInner() const    { return fAngleToNearbyUnfittedInner;  }

    void     setRingNumPads(Int_t n)          { fRingNumPads = n;           }
    Int_t    getRingNumPads() const           { return fRingNumPads;        }

    void     setRingCentroid(Float_t c)       { fRingCentroid = c;          }
    Float_t  getRingCentroid() const          { return fRingCentroid;       }

    void     setRingPatternMatrix(Float_t p)  { fRingPatternMatrix = p;     }
    Float_t  getRingPatternMatrix() const     { return fRingPatternMatrix;  }

    void     setRichPhi(Float_t p)            { fRichPhi = p;               }
    Float_t  getRichPhi() const               { return fRichPhi;            }

    void     setRichTheta(Float_t p)          { fRichTheta = p;             }
    Float_t  getRichTheta() const             { return fRichTheta;          }

    void     setRingAmplitude(Int_t a)        { fRingAmplitude = a;         }
    Int_t    getRingAmplitude() const         { return fRingAmplitude;      }

    void     setRingHouTra(Int_t a)           { fRingHouTra = a;            }
    Int_t    getRingHouTra() const            { return fRingHouTra;         }

    void     setMetaMatchQuality(Float_t q)   { fMetaMatchQuality = q;      }
    Float_t  getMetaMatchQuality() const      { return fMetaMatchQuality;   }

    void     setTofinoMultiplicity(Short_t m) { fTofinoMultiplicity = m;    }
    Short_t  getTofinoMultiplicity() const    { return fTofinoMultiplicity; }

    void     setShowerSum0(Float_t q)         { fShowerSum0 = q;            }
    Float_t  getShowerSum0() const            { return fShowerSum0;         }

    void     setShowerSum1(Float_t q)         { fShowerSum1 = q;            }
    Float_t  getShowerSum1() const            { return fShowerSum1;         }

    void     setShowerSum2(Float_t q)         { fShowerSum2 = q;            }
    Float_t  getShowerSum2() const            { return fShowerSum2;         }

    HPidTrackCand * getTrackCand()            { return tcand; }
    HPidTrackData * getTrackData()            { return tcand->getTrackData();	}
    HPidHitData *   getHitData()              { return tcand->getHitData();    	}
private:
    Float_t calcMassSq(Float_t Tof, Float_t Momentum, Float_t PathLength);

    HPidTrackCand * tcand;

    ClassDef(HParticleCand,1)  // A simple track of a particle
};

#endif // HPARTICLECAND_H
