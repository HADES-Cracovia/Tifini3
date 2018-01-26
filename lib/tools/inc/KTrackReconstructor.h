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

#include "hparticlecand.h"
#include "hfwdetcand.h"
#include "hparticlepair.h"

#include "KTifiniAnalysis.h"
#include "KTrack.h"

class KTrackReconstructor : public KTrack
{
public:
    KTrackReconstructor();
    virtual ~KTrackReconstructor();
    void useAnglesDeg(bool use_deg);

    KTrackReconstructor & operator=(const KTrackReconstructor & cand);
    KTrackReconstructor & operator=(const KTrack & cand);
    KTrackReconstructor & operator=(const TLorentzVector & cand);

    void reconstruct(const HParticleCand & track_a, const HParticleCand & track_b);
    void reconstruct(const HParticleCand & track_a, const TLorentzVector & track_b);
    void reconstruct(const TLorentzVector & track_a, const TLorentzVector & track_b);
    void reconstruct(const HParticleCand & track_a, const HFwDetCand & track_b);
    void reconstruct(const HFwDetCand & track_a, const HFwDetCand & track_b);

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

    Int_t full_case;            // 0=L&L, 1=l&H, 2=H&H, 11=F&L, 12=F&H L: TLorentzVector, H: HParticleCand, F: HFeDwtCand
    HParticleCand tr_ah, tr_bh;
    HFwDetCand tr_af, tr_bf;
    TLorentzVector tr_al, tr_bl;

    HParticlePair pair_had;
    HGeomVector decay_vertex;
    Float_t mtd;
    Float_t mtd_a;
    Float_t mtd_b;

    Float_t r2d;

    Float_t fEa, fMa, fPa;
    Float_t fEb, fMb, fPb;

//     tree->Branch("fXf",             &g_ads.fXf,             "fXf/F");
//     tree->Branch("fXfi",            &g_ads.fXfi,            "fXfi/F");

//     tree->Branch("fMinTrackDist",   &g_ads.fMTD,            "fMinTrackDist/F" );
//     tree->Branch("fVertDistX",      &g_ads.fVertDistX,      "fVertDistX/F");
//     tree->Branch("fPVA",            &g_ads.fPVA,            "fPVA/F");
//     tree->Branch("fPVA_miss",       &g_ads.fPVA_miss,       "fPVA_miss/F");
//     tree->Branch("fDVres",          &g_ads.fDVres,          "fDVres/F");
// 
//     tree->Branch("fMomA",           &g_ads.fMomA,           "fMomA/F");
//     tree->Branch("fMomB",           &g_ads.fMomB,           "fMomB/F" );
// //    tree->Branch("fMomAx",        &g_ads.fMomAx,          "fMomAx/F");
// //    tree->Branch("fMomAy",        &g_ads.fMomAy,          "fMomAy/F");
// //    tree->Branch("fMomAz",        &g_ads.fMomAz,          "fMomAz/F");
// //    tree->Branch("fMomBx",        &g_ads.fMomBx,          "fMomBx/F" );
// //    tree->Branch("fMomBy",        &g_ads.fMomBy,          "fMomBy/F" );
// //    tree->Branch("fMomBz",        &g_ads.fMomBz,          "fMomBz/F" );
//     tree->Branch("fMomA_cms",       &g_ads.fMomA_cms,       "fMomA_cms/F");
//     tree->Branch("fMomB_cms",       &g_ads.fMomB_cms,       "fMomB_cms/F");
//     tree->Branch("fAngleAB",        &g_ads.fAngleAB,        "fAngleAB/F");
//     tree->Branch("fRelAngleA",      &g_ads.fRelAngleA,      "fRelAngleA/F");
//     tree->Branch("fRelAngleB",      &g_ads.fRelAngleB,      "fRelAngleB/F");
// 
//     tree->Branch("fMDCdEdxA",       &g_ads.fMDCdEdxA,       "fMDCdEdxA/F");
//     tree->Branch("fMDCdEdxB",       &g_ads.fMDCdEdxB,       "fMDCdEdxB/F");
// 
//     tree->Branch("fChiA",           &g_ads.fChiA,           "fChiA/F");
//     tree->Branch("fChiB",           &g_ads.fChiB,           "fChiB/F");
//     tree->Branch("fMetaMatchQA",    &g_ads.fMetaMatchQA,    "fMetaMatchQA/F");
//     tree->Branch("fMetaMatchQB",    &g_ads.fMetaMatchQB,    "fMetaMatchQB/F");
//     tree->Branch("fVertDistA",      &g_ads.fVertDistA,      "fVertDistA/F");
//     tree->Branch("fVertDistB",      &g_ads.fVertDistB,      "fVertDistB/F");
//     tree->Branch("fHadesTracksNum", &g_ads.fHadesTracksNum, "fHadesTracksNum/I");
//     tree->Branch("fFwDetTracksNum", &g_ads.fFwDetTracksNum, "fFwDetTracksNum/I");
//     tree->Branch("fIsFwDetData",    &g_ads.fIsFwDetData,    "fIsFwDetData/I");
// 
//     tree->Branch("fMultA",          &g_ads.fMultA,          "fMultA/I");
//     tree->Branch("fMultB",          &g_ads.fMultB,          "fMultB/I");
// 
//     tree->Branch("fAccA",           &g_ads.fAccA,           "fAccA/I");
//     tree->Branch("fAccB",           &g_ads.fAccB,           "fAccB/I");
// 
//     tree->Branch("fBestTry",        &g_ads.fBestTry,        "fBestTry/I");
//     tree->Branch("fEventLCounter",  &g_ads.fEventLCounter,  "fEventLCounter/I");
};

#endif // KTRACKRECONSTRUCTOR_H
