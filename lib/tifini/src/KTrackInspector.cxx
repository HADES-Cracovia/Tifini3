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

#include "KTrackInspector.h"

#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";
/*
 TString KTrackInspector::MetaSystemName[KT::MS_MDC] = "MDC";
 TString KTrackInspector::MetaSystemName[KT::MS_TOF] = "TOF";
 TString KTrackInspector::MetaSystemName[KT::MS_TOFINO] = "TOFINO";
 TString KTrackInspector::MetaSystemName[KT::MS_RPC] = "RPC";
*/
//const TString KTrackInspector::MetaSystemName[KT::MS_MDC] = { "", "MDC", "TOF", "", "TOFINO", "", "", "", "RPC" };

const int KTrackInspector::mom_bins = 600;
const int KTrackInspector::mom_min = -3000;
const int KTrackInspector::mom_max = 3000;
const int KTrackInspector::dEdx_bins = 400;
const int KTrackInspector::dEdx_min = 0;
const int KTrackInspector::dEdx_max = 20;
const int KTrackInspector::can_width = 800;
const int KTrackInspector::can_height = 600;

KTrackInspector::PseudoMetaSystem KTrackInspector::pseudoMetaMapArray[KT::MS_Dummy] = {
    KTrackInspector::PMS_NOMETA,
    KTrackInspector::pMDC,
    KTrackInspector::pTOF,
    KTrackInspector::PMS_NOMETA, 
    KTrackInspector::pTOF,
    KTrackInspector::PMS_NOMETA, KTrackInspector::PMS_NOMETA, KTrackInspector::PMS_NOMETA,
    KTrackInspector::pTOF
};

KT::ParticleID KTrackInspector::cidToPIDMapArray[KT::CID_Dummy] = { KT::p, KT::pip, KT::pim, KT::Kp, KT::Km };

KTrackInspector::KTrackInspector()
{
    init();

    cidParams[KT::cut_p] = { kBlack, 2, 1, 246, 2500 };
    cidParams[KT::cut_pip] = { kBlack, 2, 1, 50,2500 };
    cidParams[KT::cut_pim] = { kBlack, 2, 1, -2500,-50 };
    cidParams[KT::cut_Kp] = { kBlack, 2, 1, 125,2500 };
    cidParams[KT::cut_Km] = { kBlack, 2, 1, -2500,-125 };

    gcutParams[KT::cut_p] = { kBlack, 1, 1, "L" };
    gcutParams[KT::cut_pip] = { kBlack, 1, 1, "L" };
    gcutParams[KT::cut_pim] = { kBlack, 1, 1, "L" };
    gcutParams[KT::cut_Kp] = { kRed, 1, 1, "L" };
    gcutParams[KT::cut_Km] = { kRed, 1, 1, "L" };
/*
    for (Int_t c = 0; c < KT::CID_Dummy; ++c) {
        cidParams[c].lineColor = kBlack;
        cidParams[c].lineWidth = 2;
        cidParams[c].lineStyle = 1;
    }

    cidParams[KT::cut_p].x1 = 246;        cidParams[KT::cut_p].x2 = 2500;
    cidParams[KT::cut_pip].x1 = 50;        cidParams[KT::cut_pip].x2 = 2500;
    cidParams[KT::cut_pim].x1 = -2500;    cidParams[KT::cut_pim].x2 = -50;
    cidParams[KT::cut_Kp].x1 = 125;        cidParams[KT::cut_Kp].x2 = 2500;
    cidParams[KT::cut_Km].x1 = -2500;    cidParams[KT::cut_Km].x2 = -125;

//    Int_t colorMap[KT::CID_Dummy] = { 2, 3, 4, 6, 7 };
    for (Int_t c = 0; c < KT::CID_Dummy; ++c) {
        gcutParams[c].lineColor = 2+c;//colorMap[c];
        gcutParams[c].lineWidth = 2;
        gcutParams[c].lineStyle = 1;
        gcutParams[c].drawOpts = "L";
    }*/
}

KTrackInspector::~KTrackInspector()
{
//     TODO: fix memory leaks!
}

void KTrackInspector::init()
{
    MetaSystemName[KT::MDC] = "MDC";
    MetaSystemName[KT::TOF] = "TOF";
    MetaSystemName[KT::TOFINO] = "TOFINO";
    MetaSystemName[KT::RPC] = "RPC";

    for (Int_t s = 0; s < KT::MS_Dummy; ++s)
    {
        cutHistograms[s] = NULL;
        cutCanvases[s] = NULL;

        cutHistogramsAcc[s] = NULL;
        cutCanvasesAcc[s] = NULL;

        cutHistogramFilled[s] = false;

        for (Int_t c = 0; c < KT::CID_Dummy; ++c)
        {
            metaSystemCfg[s][c] = false;
            reqCutFilesArray[s][c] = NULL;
            reqCutsArray[s][c] = NULL;
        }
    }

    for (Int_t p = 0; p < PMS_Dummy; ++p)
    {
        energyLoss[p][0] = NULL;
        energyLoss[p][1] = NULL;
    }
}

bool KTrackInspector::registerCut(KT::MetaSystem system, KT::CutID particle, const TString & file, const TString & cutname, Bool_t mirror_cut)
{
    TFile * f = TFile::Open(file);
    TCutG * cut = NULL;

    if (f)
    {
        cut = (TCutG*)f->Get(cutname);
        if (!cut)
        {
            std::cerr << "+++ WARNING: No cut : " << cutname << " inside file : " << file << std::endl;
            exit(EXIT_FAILURE);
            return false;
        }
    }
    else
    {
        std::cerr << "+++ WARNING: No file : " << file << std::endl;
        exit(EXIT_FAILURE);
        return false;
    }

    reqCutFilesArray[system][particle] = f;
    reqCutsArray[system][particle] = cut;
//     cutsFlagsArray[system][particle] = ignore_polarity;
    if (mirror_cut)
    {
        Double_t x, y;
        Int_t num = cut->GetN();
        for (Int_t i = 0; i < num; ++i)
        {
            cut->GetPoint(i, x, y);
            cut->SetPoint(i, -x, y);
        }
    }

    registerdEdxPlot(system);
    return true;
}

bool KTrackInspector::optionalCut(KT::MetaSystem system, KT::CutID particle, const TString & file, const TString & cutname, Bool_t mirror_cut)
{
    TFile * f = TFile::Open(file);
    TCutG * cut = NULL;

    if (f)
    {
        cut = (TCutG*)f->Get(cutname);
        if (!cut)
        {
            std::cerr << "+++ WARNING: No cut : " << cutname << " inside file : " << file << std::endl;
            exit(EXIT_FAILURE);
            return false;
        }
    }
    else
    {
        std::cerr << "+++ WARNING: No file : " << file << std::endl;
        exit(EXIT_FAILURE);
        return false;
    }

    optCutFilesArray[system][particle] = f;
    optCutsArray[system][particle] = cut;
//     cutsFlagsArray[system][particle] = ignore_polarity;
    if (mirror_cut)
    {
        Double_t x, y;
        Int_t num = cut->GetN();
        for (Int_t i = 0; i < num; ++i)
        {
            cut->GetPoint(i, x, y);
            cut->SetPoint(i, -x, y);
        }
    }

    registerdEdxPlot(system);
    return true;
}

bool KTrackInspector::registerdEdxPlot(KT::MetaSystem system)
{
    if (!cutHistograms[system])
    {
        TString hist_name = "Hist_dEdx_" + MetaSystemName[system];
        TString hist_title = "dEdx vs mom: " + MetaSystemName[system] + ";polarity * momentum [MeV/c];dE/dx [MeV * 1/m]";
        cutHistograms[system] = new TH2I(hist_name, hist_title, mom_bins, mom_min, mom_max, dEdx_bins, dEdx_min, dEdx_max);

        hist_name = "Hist_dEdx_" + MetaSystemName[system] + "_Accepted";
        hist_title = "dEdx vs mom: " + MetaSystemName[system] + ";polarity * momentum [MeV/c];dE/dx [MeV * 1/m]";
        cutHistogramsAcc[system] = new TH2I(hist_name, hist_title, mom_bins, mom_min, mom_max, dEdx_bins, dEdx_min, dEdx_max);

        TString can_name = "Can_dEdx_" + MetaSystemName[system];
        TString can_title = "dEdx vs mom: " + MetaSystemName[system];
        cutCanvases[system] = new TCanvas(can_name, can_title, can_width, can_height);

        can_name = "Can_dEdx_" + MetaSystemName[system] + "_Accepted";
        can_title = "dEdx vs mom: " + MetaSystemName[system];
        cutCanvasesAcc[system] = new TCanvas(can_name, can_title, can_width, can_height);
    }

    return true;
}

void KTrackInspector::configureMetaSystem(KT::CutID cid, Int_t meta)
{
    pseudoMetaSystemCfg[cid] = 0x00;

    for (int s = 1; s < KT::MS_Dummy; s = s << 1) {
        if (s & meta)
            metaSystemCfg[s][cid] = true;
        else
            metaSystemCfg[s][cid] = false;

        pseudoMetaSystemCfg[cid] |= (metaSystemCfg[s][cid] << pseudoMetaMapArray[s]);
    }
}

bool KTrackInspector::isInside(KT::CutID cid, const HParticleCand * track) const
{
    UShort_t _isInside = 0x00;

    Double_t mom = track->getMomentum();
    Double_t charge = track->getCharge();
    Double_t momcharge = mom*charge;
    Double_t dEdx = 0.0;
// PR(0);
    for (int s = 1; s < KT::MS_Dummy; s = s << 1)
    {//PR(s);
        dEdx = 0.0;
        switch (s)
        {
            case KT::MDC:
                dEdx = track->getMdcdEdx();
                break;
            case KT::TOF:
            case KT::TOFINO:
                dEdx = track->getTofdEdx();
                break;
//            case KT::MS_RPC:
//                dEdx = track->getRpcdEdx();
//                break;
            default:
//                 PR(s);
                continue;
        }

        Bool_t _isInsideSystem = kFALSE;
// PR(2);
        if (metaSystemCfg[s][cid])
        {
            if (reqCutsArray[s][cid])
            {
                _isInsideSystem = reqCutsArray[s][cid]->IsInside(momcharge, dEdx);
                _isInside |= ( _isInsideSystem << pseudoMetaMapArray[s]);
            }
            else
            {
                std::cerr << "+++ WARNING: No cuts for DET: " << MetaSystemName[s] << " and PART: " << cid << std::endl;
                return false;
            }
        }
// PR(1);
        if (cutHistograms[s] and !cutHistogramFilled[s])
        {
            cutHistograms[s]->Fill(momcharge, dEdx);

            if (_isInsideSystem)
                cutHistogramsAcc[s]->Fill(momcharge, dEdx);

//             cutHistogramFilled[s] = true;
        }

        
    }

    bool res = (_isInside == pseudoMetaSystemCfg[cid]);
    
    return res;
}

void KTrackInspector::drawCuts() const {
    for (Int_t s = 0; s < KT::MS_Dummy; ++s)
    {
        if (!cutCanvases[s])
            continue;

        cutCanvases[s]->cd();
        cutHistograms[s]->Write();
        cutHistograms[s]->Draw("colz");

        for (int i = 0;  i < KT::CID_Dummy; ++i)
        {
            if (metaSystemCfg[s][i])
            {

                if (reqCutsArray[s][i])
                {
                    reqCutsArray[s][i]->SetLineColor(gcutParams[i].lineColor);
                    reqCutsArray[s][i]->SetLineWidth(gcutParams[i].lineWidth);
                    reqCutsArray[s][i]->SetLineStyle(gcutParams[i].lineStyle);
                    reqCutsArray[s][i]->Draw(gcutParams[i].drawOpts);
                }
                else
                {
                    std::cerr << "+++ WARNING: No cuts for DET: " << s << " and PART: " << i << std::endl;
                }
            }
        }

        TF1 * fitFunc = energyLoss[pseudoMetaMapArray[s]][KT::Exp];

//         cutCanvases[s]->Draw();

        if (fitFunc)
        {
            for (Int_t c = 0; c < KT::CID_Dummy; ++c) {
                fitFunc->SetLineColor(cidParams[c].lineColor);
                fitFunc->SetLineWidth(cidParams[c].lineWidth);
                fitFunc->SetLineStyle(cidParams[c].lineStyle);
                fitFunc->SetParameter(8, cidToPIDMapArray[c]);
                fitFunc->SetRange(cidParams[c].x1, cidParams[c].x2);
                fitFunc->DrawCopy("same");
            }
        }

        cutCanvases[s]->Write();
    }

    for (Int_t s = 0; s < KT::MS_Dummy; ++s)
    {
        if (!cutCanvasesAcc[s])
            continue;

        cutCanvasesAcc[s]->cd();
        cutHistogramsAcc[s]->Write();
        cutHistogramsAcc[s]->Draw("colz");

        for (int i = 0;  i < KT::CID_Dummy; ++i)
        {
            if (metaSystemCfg[s][i])
            {

                if (reqCutsArray[s][i])
                {
                    reqCutsArray[s][i]->SetLineColor(gcutParams[i].lineColor);
                    reqCutsArray[s][i]->SetLineWidth(gcutParams[i].lineWidth);
                    reqCutsArray[s][i]->SetLineStyle(gcutParams[i].lineStyle);
                    reqCutsArray[s][i]->Draw(gcutParams[i].drawOpts);
                }
                else
                {
                    std::cerr << "+++ WARNING: No cuts for DET: " << s << " and PART: " << i << std::endl;
                }
            }
        }

        TF1 * fitFunc = energyLoss[pseudoMetaMapArray[s]][KT::Exp];

//         cutCanvases[s]->Draw();

        if (fitFunc)
        {
            for (Int_t c = 0; c < KT::CID_Dummy; ++c) {
                fitFunc->SetLineColor(cidParams[c].lineColor);
                fitFunc->SetLineWidth(cidParams[c].lineWidth);
                fitFunc->SetLineStyle(cidParams[c].lineStyle);
                fitFunc->SetParameter(8, cidToPIDMapArray[c]);
                fitFunc->SetRange(cidParams[c].x1, cidParams[c].x2);
                fitFunc->DrawCopy("same");
            }
        }

        cutCanvasesAcc[s]->Write();
    }
}

void KTrackInspector::clearCache()
{
    for (Int_t s = 0; s < KT::MS_Dummy; ++s) {
        cutHistogramFilled[s] = false;
    }
}
