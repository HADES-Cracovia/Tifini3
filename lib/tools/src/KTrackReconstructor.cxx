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

#include "KTrackReconstructor.h"

#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";

KTrackReconstructor::KTrackReconstructor() : KTrack(), full_case(-1)
{
}

KTrackReconstructor::~KTrackReconstructor()
{
}

void KTrackReconstructor::useAnglesDeg(bool use_deg)
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

bool KTrackReconstructor::setTree(TTree* tree, const TString& unique_name, UInt_t b, UInt_t bt)
{
    if (!tree)
        return false;

    KTrack::setTree(tree, unique_name, bt);

    if (b & bEchild)    CREATE_BRANCH("Ea",         fEa,        'F');
    if (b & bMchild)    CREATE_BRANCH("Ma",         fMa,        'F');
    if (b & bPchild)    CREATE_BRANCH("Pa",         fPa,        'F');

    if (b & bEchild)    CREATE_BRANCH("Eb",         fEb,        'F');
    if (b & bMchild)    CREATE_BRANCH("Mb",         fMb,        'F');
    if (b & bPchild)    CREATE_BRANCH("Pb",         fPb,        'F');

    return true;
}

void KTrackReconstructor::fill()
{
    KTrack::fill();

    fEa = tr_al.E();
    fMa = tr_al.M();
    fPa = tr_al.P();

    fEb = tr_bl.E();
    fMb = tr_bl.M();
    fPb = tr_bl.P();
}

KTrackReconstructor & KTrackReconstructor::operator=(const KTrackReconstructor& cand)
{
    *((KTrack *)this) = cand;
    return *this;
}

KTrackReconstructor & KTrackReconstructor::operator=(const KTrack& cand)
{
    *((KTrack *)this) = cand;
    return *this;
}

KTrackReconstructor & KTrackReconstructor::operator=(const TLorentzVector& cand)
{
    *((TLorentzVector *)this) = cand;
    return *this;
}

void KTrackReconstructor::reconstruct(const HParticleCand& track_a, const HParticleCand& track_b)
{
    reconstruct((TLorentzVector&)track_a, (TLorentzVector&)track_b);
    tr_ah = track_a;
    tr_bh = track_b;
    full_case = 2;
}

void KTrackReconstructor::reconstruct(const HParticleCand& track_a, const TLorentzVector& track_b)
{
    reconstruct((TLorentzVector&)track_a, (TLorentzVector&)track_b);
    tr_ah = track_a;
    full_case = 1;
}

void KTrackReconstructor::reconstruct(const TLorentzVector& track_a, const TLorentzVector& track_b)
{
    *this = track_a + track_b;
    tr_al = track_a;
    tr_bl = track_b;
    full_case = 0;
}
