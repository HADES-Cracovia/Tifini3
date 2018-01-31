#ifndef HPARTICLECANDSIM_H
#define HPARTICLECANDSIM_H

#include "TMath.h"
#include "TObject.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

#include "HParticleCand.h"

// Modified by A. Schmah 07.04.2009: Sim info added
// Modified by A. Schmah 17.04.2009: Forward wall info added
// Modified by A. Schmah 18.06.2009: New structure with Sim track and WallHit
// Modified by R. Lalik  04.04.2013: Ancestor chain implemented

#include "hpidtrackcandsim.h"

class HParticleCandSim : public HParticleCand
{
private:

    static const Int_t AncestorLevel = 4;

    // sim info
//     Int_t   fGeantPID;               // Geant PID value
//     Int_t   fPGeantPID;               // Geant PID value of the Parent
    Int_t   fGeantAncestorPID[AncestorLevel+1];               // Geant PID value of the GrandGrantparent
    UInt_t  fGeantCorrTrackIds;     // Geant number of correlated track ids
    Float_t fGeantxMom;
    Float_t fGeantyMom;
    Float_t fGeantzMom;
    Float_t fGeantxVertex;
    Float_t fGeantyVertex;
    Float_t fGeantzVertex;
    Int_t   fGeantParentTrackNum; // parent track number
    Int_t   fGeantCreationMechanism;
    Int_t   fGeantMediumNumber;
    Bool_t  fisEmbSimTrack;
    Bool_t  fisHiddenPion;

public:
    HParticleCandSim();
    HParticleCandSim(HPidTrackCandSim & cand);

    virtual ~HParticleCandSim();

    void    setGeantPID(Int_t a)                { fGeantAncestorPID[0] = a;     }
    Int_t   getGeantPID() const                 { return fGeantAncestorPID[0];  }

    void    setPGeantPID(Int_t a)               { fGeantAncestorPID[1] = a;     }
    Int_t   getPGeantPID() const                { return fGeantAncestorPID[1];  }

    void    setGeantAncestorPID(Int_t a, Int_t b)    { fGeantAncestorPID[a] = b;    }
    Int_t   getGeantAncestorPID(Int_t a) const       { return fGeantAncestorPID[a]; }

    void    setGeantCorrTrackIds(UInt_t a)      { fGeantCorrTrackIds = a;       }
    UInt_t  getGeantCorrTrackIds() const        { return fGeantCorrTrackIds;    }

    void    setGeantxMom(Float_t a)             { fGeantxMom = a;               }
    Float_t getGeantxMom() const                { return fGeantxMom;            }

    void    setGeantyMom(Float_t a)             { fGeantyMom = a;               }
    Float_t getGeantyMom() const                { return fGeantyMom;            }

    void    setGeantzMom(Float_t a)             { fGeantzMom = a;               }
    Float_t getGeantzMom() const                { return fGeantzMom;            }

    void    setGeantxVertex(Float_t a)          { fGeantxVertex = a;            }
    Float_t getGeantxVertex() const             { return fGeantxVertex;         }

    void    setGeantyVertex(Float_t a)          { fGeantyVertex = a;            }
    Float_t getGeantyVertex() const             { return fGeantyVertex;         }

    void    setGeantzVertex(Float_t a)          { fGeantzVertex = a;            }
    Float_t getGeantzVertex() const             { return fGeantzVertex;         }

    void    setGeantParentTrackNum(Int_t a)     { fGeantParentTrackNum = a;     }
    Int_t   getGeantParentTrackNum() const      { return fGeantParentTrackNum;  }

    void    setGeantCreationMechanism(Int_t a)  { fGeantCreationMechanism = a;      }
    Int_t   getGeantCreationMechanism() const   { return fGeantCreationMechanism;   }

    void    setGeantMediumNumber(Int_t a)       { fGeantMediumNumber = a;       }
    Int_t   getGeantMediumNumber() const        { return fGeantMediumNumber;    }

    void    setisEmbSimTrack(Bool_t a)          { fisEmbSimTrack = a;           }
    Bool_t  getisEmbSimTrack() const            { return fisEmbSimTrack;        }

    void    setisHiddenPion(Bool_t a)           { fisHiddenPion = a;            }
    Bool_t  getisHiddenPion() const             { return fisHiddenPion;         }

    Int_t   getGeantParentPID() const           { return fGeantAncestorPID[1];  }
    Int_t   getGeantGrandParentPID() const      { return fGeantAncestorPID[2];  }

    ClassDef(HParticleCandSim, 2)  // A simple Sim track of a particle
};

#endif // HPARTICLECANDSIM_H
