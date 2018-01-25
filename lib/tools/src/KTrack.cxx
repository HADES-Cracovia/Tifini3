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

#include <iostream>
#include <cstdlib>

#include "KTrack.h"

#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";

KTrack::KTrack() : HParticleCand()
{
}

KTrack::~KTrack()
{
}

void KTrack::useAnglesDeg(bool use_deg)
{
    if (use_deg)
        r2d = TMath::RadToDeg();
    else
        r2d = 1.0;
}

#define CREATE_BRANCH(name, var, type) \
{ \
TString str = TString::Format("f%s_%s", unique_name.Data(), name); \
TString str_t = TString::Format("%s/%c", str.Data(), type); \
tree->Branch(str.Data(), &var, str.Data()); \
}

bool KTrack::setTree(TTree* tree, const TString& unique_name, UInt_t b)
{
    if (!tree)
        return false;

    if (b & bE)         CREATE_BRANCH("E",          fE,         'F');
    if (b & bM)         CREATE_BRANCH("M",          fM,         'F');
    if (b & bP)         CREATE_BRANCH("P",          fP,         'F');
    if (b & bTheta)     CREATE_BRANCH("Theta",      fTheta,     'F');
    if (b & bCosTheta)  CREATE_BRANCH("CosTheta",   fCosTheta,  'F');
    if (b & bPhi)       CREATE_BRANCH("Phi",        fPhi,       'F');
    if (b & bY)         CREATE_BRANCH("Y",          fY,         'F');
    if (b & bPT)        CREATE_BRANCH("Pt",         fPt,        'F');

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

    false;
}

bool KTrack::fill()
{
    fE = TLorentzVector::E();
    fM = TLorentzVector::M();
    fP = TLorentzVector::P();
    fTheta = TLorentzVector::Theta() * r2d;
    fCosTheta = TMath::Cos(TLorentzVector::Theta());
    fPhi = TLorentzVector::Phi() * r2d;
    fY = TLorentzVector::Rapidity();
    fPt = TLorentzVector::Pt();
}

KTrack & KTrack::operator=(const KTrack& cand)
{
    *((HParticleCand *)this) = cand;
    return *this;
}

KTrack & KTrack::operator=(const HParticleCand& cand)
{
    *((HParticleCand *)this) = cand;
    return *this;
}

KTrack & KTrack::operator=(const TLorentzVector& cand)
{
    *((TLorentzVector *)this) = cand;
    return *this;
}
