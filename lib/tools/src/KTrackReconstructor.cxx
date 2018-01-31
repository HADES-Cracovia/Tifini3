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

const HGeomVector KTrackReconstructor::vec_no_data = HGeomVector(-10000.0, -10000.0, -10000.0);

KTrackReconstructor::KTrackReconstructor() : KTrack(), full_case(-1), decay_vertex(vec_no_data)
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
    tr_ah = cand.tr_ah;
    tr_bh = cand.tr_bh;

#ifndef HYDRA1COMP
    tr_af = cand.tr_af;
    tr_bf = cand.tr_bf;
#endif

    tr_al = cand.tr_al;
    tr_bl = cand.tr_bl;

#ifndef HYDRA1COMP
    pair_had = cand.pair_had;
#endif
    decay_vertex = vec_no_data;
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
    HGeomVector ev_vertex = vec_no_data;

#ifndef HYDRA1COMP
    pair_had.setPair(&tr_ah, -1, &tr_bh, -1, -1, 0, ev_vertex);
    decay_vertex = pair_had.getDecayVertex();
    mtd = pair_had.getMinDistCandidates();
    mtd_a = pair_had.getVerMinDistCand(0);
    mtd_b = pair_had.getVerMinDistCand(1);
#else
    // TODO
#endif
}

void KTrackReconstructor::reconstruct(const HParticleCand& track_a, const TLorentzVector& track_b)
{
    reconstruct((TLorentzVector&)track_a, (TLorentzVector&)track_b);
    tr_ah = track_a;
    full_case = 1;
    clear_no_reco();
}

void KTrackReconstructor::reconstruct(const TLorentzVector& track_a, const TLorentzVector& track_b)
{
    *this = track_a + track_b;
    tr_al = track_a;
    tr_bl = track_b;
    full_case = 0;
    clear_no_reco();
}

#ifndef HYDRA1COMP
void KTrackReconstructor::reconstruct(const HParticleCand& track_a, const HFwDetCand& track_b)
{
    reconstruct((TLorentzVector&)track_a, (TLorentzVector&)track_b);
    tr_ah = track_a;
    tr_bf = track_b;
    full_case = 11;
    clear_no_reco();
}

void KTrackReconstructor::reconstruct(const HFwDetCand& track_a, const HFwDetCand& track_b)
{
    reconstruct((TLorentzVector&)track_a, (TLorentzVector&)track_b);
    tr_af = track_a;
    tr_bf = track_b;
    full_case = 12;
    clear_no_reco();
}
#endif

void KTrackReconstructor::clear_no_reco()
{
    decay_vertex = vec_no_data;
    mtd = -100.0;
    mtd_a = -100.0;
    mtd_b = -100.0;
}
