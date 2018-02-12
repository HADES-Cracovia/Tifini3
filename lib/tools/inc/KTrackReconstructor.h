/* 
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011 Rafał Lalik <rafal.lalik@ph.tum.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef KTRACKRECONSTRUCTOR_H
#define KTRACKRECONSTRUCTOR_H

#include "TH2.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TFile.h"
#include "TString.h"

#include "Tifini3Config.h"

#ifdef HYDRA1COMP
#include "HParticleCand.h"
#else
#include "hparticlecand.h"
#include "hparticlepair.h"
#include "hparticleevtinfo.h"
#include "hfwdetcand.h"
#include "hvirtualcand.h"
#endif

#include "KTifiniAnalysis.h"
#include "KTrack.h"

class KTrackReconstructor : public KTrack
{
public:
    KTrackReconstructor();
    KTrackReconstructor(const KTrackReconstructor & track);
    virtual ~KTrackReconstructor();
    void useAnglesDeg(bool use_deg);

    KTrackReconstructor & operator=(const KTrackReconstructor & cand);
    KTrackReconstructor & operator=(const KTrack & cand);
    KTrackReconstructor & operator=(const TLorentzVector & cand);

    void reconstruct(KTrackReconstructor & track_a, KTrackReconstructor & track_b);
    void reconstruct(const KTrackReconstructor & track_a, const TLorentzVector & track_b);
    void reconstruct(const TLorentzVector & track_a, const TLorentzVector & track_b);
#ifdef HYDRA1COMP
    void reconstruct(const HParticleCand & track_a, const TLorentzVector & track_b);
    void reconstruct(const HParticleCand & track_a, const HParticleCand & track_b);
    void reconstruct(KTrackReconstructor & track_a, const HParticleCand & track_b);
#else
    void reconstruct(const HVirtualCand & track_a, const TLorentzVector & track_b);
    void reconstruct(const HVirtualCand & track_a, const HVirtualCand & track_b);
    void reconstruct(KTrackReconstructor & track_a, const HVirtualCand & track_b);
#endif

#ifndef HYDRA1COMP
    HParticlePair * getPair() { return &pair_had; }
#endif

    HGeomVector getDecayVertex() const { return decay_vertex; }
    Float_t getMTD() const { return mtd; }
    Float_t getMTDa() const { return mtd_a; }
    Float_t getMTDb() const { return mtd_b; }

    enum Branches {
        bEchild     = 1 << 0,
        bMchild     = 1 << 1,
        bPchild     = 1 << 2,
    };
    bool setTree(TTree * tree, const TString& unique_name, UInt_t b = 0xffff, UInt_t bt = 0xffff);
    void fill();

private:
    void clear_no_reco();

public:
    static const HGeomVector vec_no_data;

private:
    TTree * tree;
    TString unique_name;

    Int_t full_case[2];         // track 0 or 1, sel. 0=LorentzVec, 1=HPartCand, 2=HFwDetCand, 3=KTrackReco

#ifdef HYDRA1COMP
    HParticleCand tr_a, tr_b;
#else
    HVirtualCand tr_a, tr_b;
#endif
    TLorentzVector tr_al, tr_bl;

#ifndef HYDRA1COMP
    HParticlePair pair_had;
#endif
    HGeomVector decay_vertex;
    Float_t mtd;
    Float_t mtd_a;
    Float_t mtd_b;

    Float_t r2d;

    Float_t fEa, fMa, fPa;
    Float_t fEb, fMb, fPb;
};

#endif // KTRACKRECONSTRUCTOR_H
