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

#include "hparticlecand.h"

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

    TString MetaSystemName[KT::MS_Dummy];// = { "", "MDC", "TOF", "", "TOFINO", "", "", "" ,"RPC" };;

    bool registerCut(KT::MetaSystem system, KT::CutID particle, const TString & file, const TString & cutname, Bool_t mirror_cut = kFALSE);
    bool optionalCut(KT::MetaSystem system, KT::CutID particle, const TString & file, const TString & cutname, Bool_t mirror_cut = kFALSE);
    bool registerdEdxPlot(KT::MetaSystem system);

    void configureMetaSystem(KT::CutID cid, Int_t meta);

    bool isInside(KT::CutID cid, const HParticleCand * track) const;

    void clearCache();
    void drawCuts() const;

    inline virtual dEdxCurveParameters getCurveParameter(KT::CutID cid) { return cidParams[cid]; }
    inline virtual void setCurveParameter(KT::CutID cid, const dEdxCurveParameters & par) { cidParams[cid] = par; }

private:
    void init();

public:
    static const int mom_bins;
    static const int mom_min, mom_max;
    static const int dEdx_bins;
    static const int dEdx_min, dEdx_max;
    static const int can_width, can_height;

private:
    TFile * reqCutFilesArray[KT::MS_Dummy][KT::CID_Dummy];
    TCutG * reqCutsArray[KT::MS_Dummy][KT::CID_Dummy];
//     Int_t cutsFlagsArray[KT::MS_Dummy][KT::CID_Dummy];
    TFile * optCutFilesArray[KT::MS_Dummy][KT::CID_Dummy];
    TCutG * optCutsArray[KT::MS_Dummy][KT::CID_Dummy];
//     Int_t optFlagsArray[KT::MS_Dummy][KT::CID_Dummy];

    TH2I * cutHistograms[KT::MS_Dummy];
    TCanvas * cutCanvases[KT::MS_Dummy];

    TH2I * cutHistogramsAcc[KT::MS_Dummy];
    TCanvas * cutCanvasesAcc[KT::MS_Dummy];

    mutable bool cutHistogramFilled[KT::MS_Dummy];

    enum PseudoMetaSystem { PMS_NOMETA = 0x00, pMDC = 0x01, pTOF = 0x02, PMS_Dummy };
    static PseudoMetaSystem pseudoMetaMapArray[KT::MS_Dummy];

    bool metaSystemCfg[KT::MS_Dummy][KT::CID_Dummy];
    UShort_t pseudoMetaSystemCfg[KT::CID_Dummy];

    // Energy loss
    TF1 * energyLoss[PMS_Dummy][KT::Sim+1];
    dEdxCurveParameters cidParams[KT::CID_Dummy];
    GCutCurveParameters gcutParams[KT::CID_Dummy];
    static KT::ParticleID cidToPIDMapArray[KT::CID_Dummy];
};

#endif // KTRACKINSPECTOR_H
