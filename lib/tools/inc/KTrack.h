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


#ifndef KTRACK_H
#define KTRACK_H

#include "TH2.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TFile.h"
#include "TString.h"

#include "hparticlecand.h"

#include "KTifiniAnalysis.h"

class KTrack : public HParticleCand
{
public:
    KTrack();
    virtual ~KTrack();
    void useAnglesDeg(bool use_deg);

    KTrack & operator=(const KTrack & cand);
    KTrack & operator=(const HParticleCand & cand);
    KTrack & operator=(const TLorentzVector & cand);

    enum Branches {
        bE          = 1 << 0,
        bM          = 1 << 1,
        bP          = 1 << 2,
        bTheta      = 1 << 3,
        bCosTheta   = 1 << 4,
        bPhi        = 1 << 5,
        bY          = 1 << 6,
        bPT         = 1 << 7,
    };
    bool setTree(TTree * tree, const TString& unique_name, UInt_t b = 0xffff);
    void fill();

private:
    TTree * tree;
    TString unique_name;

    Float_t r2d;

    Float_t fE, fM, fP;
    Float_t fTheta, fCosTheta, fPhi;
    Float_t fY, fPt;

//     tree->Branch("fPolX",           &g_ads.fPolX,           "fPolX/F");
//     tree->Branch("fPolY",           &g_ads.fPolY,           "fPolY/F");
//     tree->Branch("fPolZ",           &g_ads.fPolZ,           "fPolZ/F");
//     tree->Branch("fZetaX",          &g_ads.fZetaX,          "fZetaX/F");
//     tree->Branch("fZetaY",          &g_ads.fZetaY,          "fZetaY/F");
//     tree->Branch("fZetaZ",          &g_ads.fZetaZ,          "fZetaZ/F");
//     tree->Branch("fXf",             &g_ads.fXf,             "fXf/F");
//     tree->Branch("fPolXi",          &g_ads.fPolXi,          "fPolXi/F");
//     tree->Branch("fPolYi",          &g_ads.fPolYi,          "fPolYi/F");
//     tree->Branch("fPolZi",          &g_ads.fPolZi,          "fPolZi/F");
//     tree->Branch("fZetaXi",         &g_ads.fZetaXi,         "fZetaXi/F");
//     tree->Branch("fZetaYi",         &g_ads.fZetaYi,         "fZetaYi/F");
//     tree->Branch("fZetaZi",         &g_ads.fZetaZi,         "fZetaZi/F");
//     tree->Branch("fXfi",            &g_ads.fXfi,            "fXfi/F");
// 
//     tree->Branch("fMinTrackDist",   &g_ads.fMTD,            "fMinTrackDist/F" );
//     tree->Branch("fVertDistX",      &g_ads.fVertDistX,      "fVertDistX/F");
//     tree->Branch("fPVA",            &g_ads.fPVA,            "fPVA/F");
//     tree->Branch("fPVA_miss",       &g_ads.fPVA_miss,       "fPVA_miss/F");
// 
//     tree->Branch("fPrimVertexX",    &g_ads.fPrimVertexX,    "fPrimVertexX/F");
//     tree->Branch("fPrimVertexY",    &g_ads.fPrimVertexY,    "fPrimVertexY/F");
//     tree->Branch("fPrimVertexZ",    &g_ads.fPrimVertexZ,    "fPrimVertexZ/F");
//     tree->Branch("fPrimVertexR",    &g_ads.fPrimVertexR,    "fPrimVertexR/F");
// 
//     tree->Branch("fFitVertexX",     &g_ads.fFitVertexX,     "fFitVertexX/F");
//     tree->Branch("fFitVertexY",     &g_ads.fFitVertexY,     "fFitVertexY/F");
//     tree->Branch("fFitVertexZ",     &g_ads.fFitVertexZ,     "fFitVertexZ/F");
// 
//     tree->Branch("fDecayVertexX",   &g_ads.fDecayVertexX,   "fDecayVertexX/F");
//     tree->Branch("fDecayVertexY",   &g_ads.fDecayVertexY,   "fDecayVertexY/F");
//     tree->Branch("fDecayVertexZ",   &g_ads.fDecayVertexZ,   "fDecayVertexZ/F");
//     tree->Branch("fDecayVertexR",   &g_ads.fDecayVertexR,   "fDecayVertexR/F");
// 
//     tree->Branch("fEventVertexX",   &g_ads.fEventVertexX,   "fEventVertexX/F");
//     tree->Branch("fEventVertexY",   &g_ads.fEventVertexY,   "fEventVertexY/F");
//     tree->Branch("fEventVertexZ",   &g_ads.fEventVertexZ,   "fEventVertexZ/F");
// 
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

#endif // KTRACK_H
