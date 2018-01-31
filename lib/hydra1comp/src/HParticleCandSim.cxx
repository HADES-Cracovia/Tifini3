#include "HParticleCandSim.h"

#include "hcategory.h"
#include "hgeantkine.h"
#include "hpidgeanttrackset.h"

HParticleCandSim::HParticleCandSim() :
    fGeantCorrTrackIds(0),
    fGeantxMom(-1), fGeantyMom(-1), fGeantzMom(-1), fGeantxVertex(-1), fGeantyVertex(-1), fGeantzVertex(-1),
    fGeantParentTrackNum(0), fGeantCreationMechanism(0), fGeantMediumNumber(0),
    fisEmbSimTrack(0), fisHiddenPion(0)

{
    memset(fGeantAncestorPID, 0, sizeof(Int_t)* (AncestorLevel+1));
}

HParticleCandSim::HParticleCandSim(HPidTrackCandSim& cand) : HParticleCand(cand)
{
    HPidGeantTrackSet * ptrackset = cand.getGeantTrackSet();

    Int_t GeantParenttrackNb = ptrackset ->getGeantParentTrackID(0);
    Int_t gptnb = GeantParenttrackNb;

    HCategory* pGeantKineCat = (HCategory*)gHades->getCurrentEvent()->getCategory(catGeantKine);

    for (Int_t anc = 1; anc < AncestorLevel; ++anc)
    {
        if (gptnb != 0)
        {
            HGeantKine * parent = (HGeantKine*)pGeantKineCat->getObject(gptnb-1);
            setGeantAncestorPID(anc, parent->getID());
            gptnb = parent->getParentTrack();
        }
        else
        {
            setGeantAncestorPID(anc, 0);
        }
    }
    setGeantCorrTrackIds(ptrackset->getNCorrelatedTrackIds());
    setGeantxMom(ptrackset->getGeantMomX(0));
    setGeantyMom(ptrackset->getGeantMomY(0));
    setGeantzMom(ptrackset->getGeantMomZ(0));
    setGeantxVertex(ptrackset->getGeantVertexX(0));
    setGeantyVertex(ptrackset->getGeantVertexY(0));
    setGeantzVertex(ptrackset->getGeantVertexZ(0));
    setGeantParentTrackNum(ptrackset->getGeantParentTrackID(0));
    setGeantMediumNumber(ptrackset->getGeantMediumID(0));
    setGeantCreationMechanism(ptrackset->getGeantProcessID(0));
}

HParticleCandSim::~HParticleCandSim()
{
}

ClassImp(HParticleCandSim);
