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

#include "KPlots.h"

#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";

const TString KPlots::ds_name[KPlots::ds_number] = { "MDC", "TOF", "TOFINO", "RPC", "STRAW", "FDRPC" };

const int KPlots::can_width = 800;
const int KPlots::can_height = 600;

KPlots::KPlots(const char * setname) : setname(setname)
{
//     cidParams[KT::cut_p] = { kBlack, 2, 1, 246, 2500 };
//     cidParams[KT::cut_pip] = { kBlack, 2, 1, 50,2500 };
//     cidParams[KT::cut_pim] = { kBlack, 2, 1, -2500,-50 };
//     cidParams[KT::cut_Kp] = { kBlack, 2, 1, 125,2500 };
//     cidParams[KT::cut_Km] = { kBlack, 2, 1, -2500,-125 };

//     gcutParams[KT::cut_p] = { kBlack, 1, 1, "L" };
//     gcutParams[KT::cut_pip] = { kBlack, 1, 1, "L" };
//     gcutParams[KT::cut_pim] = { kBlack, 1, 1, "L" };
//     gcutParams[KT::cut_Kp] = { kRed, 1, 1, "L" };
//     gcutParams[KT::cut_Km] = { kRed, 1, 1, "L" };

    for (UInt_t s = 0; s < ds_number; ++s)
    {
        h_ds[s] = nullptr;
        c_ds[s] = nullptr;
    }
    init(KT::NOSYSTEM, "", "", 0, 0, 0, 0, 0, 0);
}

KPlots::~KPlots()
{
    for (UInt_t s = 0; s < ds_number; ++s)
    {
        delete h_ds[s];
        delete c_ds[s];
    }
}

void KPlots::init(UInt_t systems, const char * label_x, const char * label_y, int bins_x, float x_min, float X_max, int bins_y, float y_min, float y_max)
{
    this->systems = systems;
    for (UInt_t s = 0; s < ds_number; ++s)
    {
        if (h_ds[s]) delete h_ds[s];
        if (c_ds[s]) delete c_ds[s];

        h_ds[s] = nullptr;
        c_ds[s] = nullptr;

        if ( !isSystemActive(s) ) continue;

        TString hname = TString::Format("h_ds_%s_%s", setname.Data(), ds_name[s].Data());
        TString cname = TString::Format("c_ds_%s_%s", setname.Data(), ds_name[s].Data());
        TString hlabel = TString::Format("h_ds_%s_%s;%s;%s", setname.Data(), ds_name[s].Data(), label_x, label_y);
        TString clabel = TString::Format("c_ds_%s_%s", setname.Data(), ds_name[s].Data());

        h_ds[s] = new TH2I(hname.Data(), hlabel.Data(), bins_x, x_min, X_max, bins_y, y_min, y_max);
        c_ds[s] = new TCanvas(cname.Data(), clabel.Data(), can_width, can_height);
    }
}

void KPlots::drawPlots() const
{
    for (UInt_t s = 0; s < ds_number; ++s)
    {
        if ( !isSystemActive(s) ) continue;

        c_ds[s]->cd();
        h_ds[s]->Draw("colz");

        h_ds[s]->Write();
        c_ds[s]->Write();
    }
/*
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
    }*/
}

void KPlots::clearPlots()
{
    for (UInt_t s = 0; s < ds_number; ++s)
    {
        if ( !isSystemActive(s) ) continue;

        h_ds[s]->Clear();
    }
}

UInt_t KPlots::lookup(KT::DetSystem ds)
{
    switch (ds)
    {
        case KT::MDC:       return 0;
        case KT::TOF:       return 1;
        case KT::TOFINO:    return 2;
        case KT::RPC:       return 3;
        case KT::STRAW:     return 4;
        case KT::FDRPC:     return 5;
        default:            return -1;
    }
    return -1;
}

KT::DetSystem KPlots::rlookup(UInt_t id)
{
    switch (id)
    {
        case 0:     return KT::MDC;
        case 1:     return KT::TOF;
        case 2:     return KT::TOFINO;
        case 3:     return KT::RPC;
        case 4:     return KT::STRAW;
        case 5:     return KT::FDRPC;
        default:    return KT::NOSYSTEM;
    }
    return KT::NOSYSTEM;
}

TString KPlots::names(KT::DetSystem ds)
{
    switch (ds)
    {
        case KT::MDC:       return "MDC";
        case KT::TOF:       return "TOF";
        case KT::TOFINO:    return "TOFINO";
        case KT::RPC:       return "RPC";
        case KT::STRAW:     return "STRAW";
        case KT::FDRPC:     return "FDRPC";
        default:            return "";
    }

    return "";
}
