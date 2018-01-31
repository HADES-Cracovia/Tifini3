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


#ifndef KPLOTSDEDX_H
#define KPLOTSDEDX_H

#include "TH2.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TFile.h"
#include "TString.h"

#include "KTifiniAnalysis.h"
#include "KPlots.h"

class KPlotsdEdx : public KPlots
{
public:
    KPlotsdEdx(const char * setname);

protected:
    void fill_h(TH2I * h[], const HParticleCand * track);

public:
    static const int mom_bins;
    static const int mom_min, mom_max;
    static const int dEdx_bins;
    static const int dEdx_min, dEdx_max;
    static const int can_width, can_height;

private:
    TString setname;
    UInt_t systems;
//     TH2I * cutHistogramsAcc[KT::MS_Dummy];
//     TCanvas * cutCanvasesAcc[KT::MS_Dummy];
// 
//     mutable bool cutHistogramFilled[KT::MS_Dummy];
//     // Energy loss
//     TF1 * energyLoss[PMS_Dummy][KT::Sim+1];

    struct dEdxCurveParameters {
        Int_t lineColor, lineWidth, lineStyle;
        Float_t x1, x2;
    };
    dEdxCurveParameters cidParams[KT::CID_Dummy];
//     GCutCurveParameters gcutParams[KT::CID_Dummy];
//     static KT::ParticleID cidToPIDMapArray[KT::CID_Dummy];
};

#endif // KPLOTSDEDX_H
