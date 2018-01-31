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


#ifndef KTRACKINSPECTOR_H
#define KTRACKINSPECTOR_H

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
#endif

class TParticleTrackSim;

#include "KTifiniAnalysis.h"

struct dEdxCurveParameters
{
//    dEdxCurveParameters(Int_t lc, Int_t lw, Int_t ls, Float_t _x1, Float_t _x2) : lineColor(lc), lineWidth(lw), lineStyle(ls), x1(_x1), x2(_x2) {};
    Int_t lineColor;
    Int_t lineWidth;
    Int_t lineStyle;
    Float_t x1;
    Float_t x2;
};

struct GCutCurveParameters
{
//    GCutCurveParameters(Int_t lc, Int_t lw, Int_t ls, const TString & dop) : lineColor(lc), lineWidth(lw), lineStyle(ls), drawOpts(dop) {};
    Int_t lineColor;
    Int_t lineWidth;
    Int_t lineStyle;
    TString drawOpts;
};

class KTrackInspector
{
public:
    KTrackInspector();
    virtual ~KTrackInspector();

    static const int DSnumber = 6;                      // DetSystem count
    static const int CTnumber = 2;                      // CutType count
    static const TString CutTypeName[CTnumber];
    static const TString CutTypeUnit[CTnumber];
    static const TString MetaSystemName[DSnumber];

    bool registerCut(KT::CutType ct, KT::DetSystem system, KT::CutID particle, const TString & file, const TString & cutname, Bool_t mirror_cut = kFALSE);
    bool optionalCut(KT::CutType ct, KT::DetSystem system, KT::CutID particle, const TString & file, const TString & cutname, Bool_t mirror_cut = kFALSE);
    bool registerdEdxPlot(KT::CutType ct, KT::DetSystem system);

    void configureMetaSystem(KT::CutID cid, Int_t meta);

    bool isInside(KT::CutType ct, KT::CutID cid, const HParticleCand * track) const;

    void clearCache();
    void drawCuts() const;

    inline virtual dEdxCurveParameters getCurveParameter(KT::CutID cid) { return cidParams[cid]; }
    inline virtual void setCurveParameter(KT::CutID cid, const dEdxCurveParameters & par) { cidParams[cid] = par; }

private:
    void init();
    UInt_t findDSpos(KT::DetSystem ds) { for (UInt_t i = 0; i < DSnumber; ++i) if (ds & (1 << i)) return i; }

public:
    static const int mom_bins;
    static const int mom_min, mom_max;
    static const int dEdx_bins;
    static const int dEdx_min, dEdx_max;
    static const int beta_bins;
    static const float beta_min, beta_max;
    static const int can_width, can_height;

private:
    TFile * reqCutFilesArray[CTnumber][DSnumber][KT::CID_Dummy];
    TCutG * reqCutsArray[CTnumber][DSnumber][KT::CID_Dummy];
//     Int_t cutsFlagsArray[CTnumber][DSnumber][KT::CID_Dummy];
    TFile * optCutFilesArray[CTnumber][DSnumber][KT::CID_Dummy];
    TCutG * optCutsArray[CTnumber][DSnumber][KT::CID_Dummy];
//     Int_t optFlagsArray[CTnumber][DSnumber][KT::CID_Dummy];

    TH2I * cutHistograms[CTnumber][DSnumber];
    TCanvas * cutCanvases[CTnumber][DSnumber];
    mutable bool cutHistogramFilled[CTnumber][DSnumber];

    bool metaSystemCfg[CTnumber][DSnumber][KT::CID_Dummy];
//     UShort_t pseudoMetaSystemCfg[KT::CID_Dummy];

    // Energy loss
    TF1 * energyLoss[DSnumber][KT::Sim+1];
    dEdxCurveParameters cidParams[KT::CID_Dummy];
    GCutCurveParameters gcutParams[KT::CID_Dummy];
    static KT::ParticleID cidToPIDMapArray[KT::CID_Dummy];
};

#endif // KTRACKINSPECTOR_H
