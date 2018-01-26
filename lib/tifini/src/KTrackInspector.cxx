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

const TString KTrackInspector::CutTypeName[KTrackInspector::CTnumber] = { "dEdx", "Beta" };
const TString KTrackInspector::CutTypeUnit[KTrackInspector::CTnumber] = { "dE/dx [MeV * 1/m]", "beta [c] " };
const TString KTrackInspector::MetaSystemName[KTrackInspector::DSnumber] = { "MDC", "TOF", "TOFINO", "RPC", "STRAW", "FDRPC" };

const int KTrackInspector::mom_bins = 600;
const int KTrackInspector::mom_min = -3000;
const int KTrackInspector::mom_max = 3000;
const int KTrackInspector::dEdx_bins = 400;
const int KTrackInspector::dEdx_min = 0;
const int KTrackInspector::dEdx_max = 20;
const int KTrackInspector::beta_bins = 440;
const float KTrackInspector::beta_min = 0;
const float KTrackInspector::beta_max = 1.1;
const int KTrackInspector::can_width = 800;
const int KTrackInspector::can_height = 600;

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
    for (Int_t ct = 0; ct < CTnumber; ++ct)
    for (Int_t s = 0; s < DSnumber; ++s)
    {
        cutHistograms[ct][s] = nullptr;
        cutCanvases[ct][s] = nullptr;

        cutHistogramFilled[ct][s] = false;

        for (Int_t c = 0; c < KT::CID_Dummy; ++c)
        {
            metaSystemCfg[ct][s][c] = false;
            reqCutFilesArray[ct][s][c] = nullptr;
            reqCutsArray[ct][s][c] = nullptr;
        }
    }

    for (Int_t p = 0; p < DSnumber; ++p)
    {
        energyLoss[p][0] = nullptr;
        energyLoss[p][1] = nullptr;
    }
}

bool KTrackInspector::registerCut(KT::CutType ct, KT::DetSystem system, KT::CutID particle, const TString & file, const TString & cutname, Bool_t mirror_cut)
{
    TFile * f = TFile::Open(file);
    TCutG * cut = nullptr;

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

    reqCutFilesArray[ct][system][particle] = f;
    reqCutsArray[ct][system][particle] = cut;
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

    registerdEdxPlot(ct, system);
    return true;
}

bool KTrackInspector::optionalCut(KT::CutType ct, KT::DetSystem system, KT::CutID particle, const TString & file, const TString & cutname, Bool_t mirror_cut)
{
    TFile * f = TFile::Open(file);
    TCutG * cut = nullptr;

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

    optCutFilesArray[ct][system][particle] = f;
    optCutsArray[ct][system][particle] = cut;

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

    registerdEdxPlot(ct, system);
    return true;
}

bool KTrackInspector::registerdEdxPlot(KT::CutType ct, KT::DetSystem system)
{
    if (!cutHistograms[ct][system])
    {
        TString hist_name = "Hist_" + CutTypeName[ct] + "_" + MetaSystemName[system];
        TString hist_title = CutTypeName[ct] + " vs mom: " + MetaSystemName[system] + ";polarity * momentum [MeV/c];" + CutTypeUnit[ct];
        cutHistograms[ct][system] = new TH2I(hist_name, hist_title, mom_bins, mom_min, mom_max, dEdx_bins, dEdx_min, dEdx_max);

        TString can_name = "Can_" + CutTypeName[ct] + "_" + MetaSystemName[system];
        TString can_title = CutTypeName[ct] + " vs mom: " + MetaSystemName[system];
        cutCanvases[ct][system] = new TCanvas(can_name, can_title, can_width, can_height);
    }

    return true;
}

void KTrackInspector::configureMetaSystem(KT::CutID cid, Int_t meta)
{
//     pseudoMetaSystemCfg[cid] = 0x00;

    for (Int_t ct = 0; ct < CTnumber; ++ct)
    for (int s = 0; s < DSnumber; ++s)
    {
        if (s & meta)
            metaSystemCfg[ct][s][cid] = true;
        else
            metaSystemCfg[ct][s][cid] = false;

//         pseudoMetaSystemCfg[ct][cid] |= (metaSystemCfg[s][cid] << pseudoMetaMapArray[s]);
    }
}

bool KTrackInspector::isInside(KT::CutType ct, KT::CutID cid, const HParticleCand * track) const
{
    Double_t mom = track->getMomentum();
    Double_t charge = track->getCharge();
    Double_t momcharge = mom*charge;
    Double_t dEdx = 0.0;

    Bool_t _isInsideSystem = kFALSE;

    for (int s = 1; s < DSnumber; s = s << 1)
    {
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

        if (metaSystemCfg[s][cid])
        {
            if (reqCutsArray[s][cid])
            {
                _isInsideSystem = reqCutsArray[ct][s][cid]->IsInside(momcharge, dEdx);
            }
            else
            {
                std::cerr << "+++ WARNING: No cuts for DET: " << MetaSystemName[s] << " and PART: " << cid << std::endl;
                return false;
            }
        }

        if (cutHistograms[ct][s] and !cutHistogramFilled[ct][s])
        {
            cutHistograms[ct][s]->Fill(momcharge, dEdx);
        }

        
    }

    return _isInsideSystem;
}

void KTrackInspector::drawCuts() const
{
    for (Int_t ct = 0; ct < CTnumber; ++ct)
    for (Int_t s = 0; s < DSnumber; ++s)
    {
        if (!cutCanvases[ct][s])
            continue;

        cutCanvases[ct][s]->cd();
        cutHistograms[ct][s]->Write();
        cutHistograms[ct][s]->Draw("colz");

        for (int i = 0;  i < KT::CID_Dummy; ++i)
        {
            if (metaSystemCfg[ct][s][i])
            {

                if (reqCutsArray[ct][s][i])
                {
                    reqCutsArray[ct][s][i]->SetLineColor(gcutParams[i].lineColor);
                    reqCutsArray[ct][s][i]->SetLineWidth(gcutParams[i].lineWidth);
                    reqCutsArray[ct][s][i]->SetLineStyle(gcutParams[i].lineStyle);
                    reqCutsArray[ct][s][i]->Draw(gcutParams[i].drawOpts);
                }
                else
                {
                    std::cerr << "+++ WARNING: No cuts for DET: " << s << " and PART: " << i << std::endl;
                }
            }
        }

        TF1 * fitFunc = energyLoss[s][KT::Exp];

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

        cutCanvases[ct][s]->Write();
    }
}

void KTrackInspector::clearCache()
{
    for (Int_t ct = 0; ct < CTnumber; ++ct)
    for (Int_t s = 0; s < DSnumber; ++s)
    {
        cutHistogramFilled[ct][s] = false;
    }
}
