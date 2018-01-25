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

KTrack::KTrack() : TLorentzVector(), track_case(-1)
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
TString str = TString::Format("f%s%s", unique_name.Data(), name); \
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

    return true;
}

void KTrack::fill()
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

KTrack & KTrack::operator=(const KTrack& track)
{
    *((TLorentzVector *)this) = (TLorentzVector)track;
    tr_al = track.tr_al;
    tr_ah = track.tr_ah;
    track_case = track.track_case;
    return *this;
}

KTrack & KTrack::operator=(const HParticleCand& track)
{
    *((TLorentzVector *)this) = (TLorentzVector)track;
    tr_al = track;
    tr_ah = track;
    track_case = 1;
    return *this;
}

KTrack & KTrack::operator=(const TLorentzVector& track)
{
    *((TLorentzVector *)this) = track;
    tr_al = track;
    track_case = 0;
    return *this;
}
