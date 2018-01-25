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

    enum Branches {
        bEchild     = 1 << 0,
        bMchild     = 1 << 1,
        bPchild     = 1 << 2,
    };
    bool setTree(TTree * tree, const TString& unique_name, UInt_t b = 0xffff, UInt_t bt = 0xffff);
    void fill();

private:
    TTree * tree;
    TString unique_name;

    Int_t full_case;            // 0=L&L, 1=l&H, 2=H&H, L: TLorentzVector, H: HParticleCand
    HParticleCand tr_ah, tr_bh;
    TLorentzVector tr_al, tr_bl;

    Float_t r2d;

    Float_t fEa, fMa, fPa;
    Float_t fEb, fMb, fPb;
};

#endif // KTRACKRECONSTRUCTOR_H
