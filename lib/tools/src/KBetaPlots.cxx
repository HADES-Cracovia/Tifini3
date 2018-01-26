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

#include "KBetaPlots.h"

#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";

const int KBetaPlots::mom_bins = 600;
const int KBetaPlots::mom_min = -3000;
const int KBetaPlots::mom_max = 3000;
const int KBetaPlots::dEdx_bins = 400;
const int KBetaPlots::dEdx_min = 0;
const int KBetaPlots::dEdx_max = 20;
const int KBetaPlots::can_width = 800;
const int KBetaPlots::can_height = 600;

KBetaPlots::KBetaPlots(char * setname, UInt_t systems) : setname(setname), systems(systems)
{
    init();

    cidParams[KT::cut_p] = { kBlack, 2, 1, 246, 2500 };
    cidParams[KT::cut_pip] = { kBlack, 2, 1, 50,2500 };
    cidParams[KT::cut_pim] = { kBlack, 2, 1, -2500,-50 };
    cidParams[KT::cut_Kp] = { kBlack, 2, 1, 125,2500 };
    cidParams[KT::cut_Km] = { kBlack, 2, 1, -2500,-125 };

//     gcutParams[KT::cut_p] = { kBlack, 1, 1, "L" };
//     gcutParams[KT::cut_pip] = { kBlack, 1, 1, "L" };
//     gcutParams[KT::cut_pim] = { kBlack, 1, 1, "L" };
//     gcutParams[KT::cut_Kp] = { kRed, 1, 1, "L" };
//     gcutParams[KT::cut_Km] = { kRed, 1, 1, "L" };
}

KBetaPlots::~KBetaPlots()
{
//     TODO: fix memory leaks!
}

void KBetaPlots::init()
{
    char * ms_names[MSnumber] = { "MDC", "TOF", "TOFINO", "RPC" };

    for (Int_t s = 0; s < MSnumber; ++s)
    {
        UInt_t sys = 1 << s;

        if (sys == KT::RPC) continue; 

        TString hname = TString::Format("h_dEdx_%s_%s", setname.Data(), ms_names[s]);
        TString cname = TString::Format("c_dEdx_%s_%s", setname.Data(), ms_names[s]);
        TString hlabel = TString::Format("h_dEdx_%s_%s", setname.Data(), ms_names[s]);
        TString clabel = TString::Format("c_dEdx_%s_%s", setname.Data(), ms_names[s]);

        h_dEdx[s] = new TH2I(hname.Data(), hlabel.Data(), mom_bins, mom_min, mom_max, dEdx_bins, dEdx_min, dEdx_max);
        c_dEdx[s] = new TCanvas(cname.Data(), clabel.Data(), can_width, can_height);
    }
}

void KBetaPlots::fill(const HParticleCand * track)
{
    Double_t mom = track->getMomentum();
    Double_t charge = track->getCharge();
    Double_t momcharge = mom*charge;
    Double_t dEdx = 0.0;
    Short_t tof_system = track->getSystem();

    for (int s = 0; s < MSnumber; ++s)
    {
        UInt_t sys = 1 << s;
        if (sys == KT::RPC) continue;

        if (sys == KT::TOFINO and tof_system != 0) continue;
        if (sys == KT::TOF and tof_system != 1) continue;

        dEdx = 0.0;

        switch (sys)
        {
            case KT::MDC:
                dEdx = track->getMdcdEdx();
                break;
            case KT::TOF:
            case KT::TOFINO:
                dEdx = track->getTofdEdx();
                break;
            default:
                continue;
        }

        if (sys & systems)
        {
            h_dEdx[s]->Fill(momcharge, dEdx);
        }
    }
}

void KBetaPlots::drawPlots() const
{
    for (Int_t s = 0; s < MSnumber; ++s)
    {
        UInt_t sys = 1 << s;
        if (sys == KT::RPC) continue;

        if (! (sys & systems) )
            continue;

        c_dEdx[s]->cd();
        h_dEdx[s]->Draw("colz");

        h_dEdx[s]->Write();
        c_dEdx[s]->Write();
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

void KBetaPlots::clearPlots()
{
    for (Int_t s = 0; s < MSnumber; ++s)
    {
        UInt_t sys = 1 << s;
        if (sys == KT::RPC) continue;

        if (! (sys & systems) )
            continue;

        h_dEdx[s]->Clear();
    }
}
