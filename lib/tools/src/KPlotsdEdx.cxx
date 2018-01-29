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

#include "KPlotsdEdx.h"

#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";

const int KPlotsdEdx::mom_bins = 600;
const int KPlotsdEdx::mom_min = -3000;
const int KPlotsdEdx::mom_max = 3000;
const int KPlotsdEdx::dEdx_bins = 400;
const int KPlotsdEdx::dEdx_min = 0;
const int KPlotsdEdx::dEdx_max = 20;
const int KPlotsdEdx::can_width = 800;
const int KPlotsdEdx::can_height = 600;

KPlotsdEdx::KPlotsdEdx(const char * setname) : KPlots(setname)
{
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

void KPlotsdEdx::fill_h(TH2I * h[], const HParticleCand * track)
{
    Double_t mom = track->getMomentum();
    Double_t charge = track->getCharge();
    Double_t momcharge = mom*charge;
    Double_t dEdx = 0.0;
    Short_t tof_system = track->getSystem();

    for (UInt_t s = 0; s < ds_number; ++s)
    {
        if ( !isSystemActive(s) ) continue;

        KT::DetSystem sys = rlookup(s);

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

        h[s]->Fill(momcharge, dEdx);
    }
}
