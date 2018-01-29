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


#ifndef KPLOTS_H
#define KPLOTS_H

#include "TH2.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TFile.h"
#include "TString.h"

#include "hparticlecand.h"

#include "KTifiniAnalysis.h"

class KPlots
{
public:
    KPlots(const char * setname);
    virtual ~KPlots();

    void init(UInt_t systems, const char * label_x, const char * label_y, int bins_x, float x_min, float X_max, int bins_y, float y_min, float y_max);

    bool isSystemActive(KT::DetSystem ds) const { return systems & ds; }
    bool isSystemActive(UInt_t id) const { return systems & (1 << id); }
    virtual void fill(const HParticleCand * track) = 0;

    void clearPlots();
    void drawPlots() const;

protected:
    virtual UInt_t lookup(KT::DetSystem ds);
    virtual KT::DetSystem rlookup(UInt_t id);
    virtual TString names(KT::DetSystem ds);

public:
    static const int can_width, can_height;

protected:
    static const UInt_t ds_number = 6;
    static const TString ds_name[ds_number];
    TH2I * h_ds[ds_number];
    TCanvas * c_ds[ds_number];

private:
    TString setname;
    UInt_t systems;
    TString label_x, label_y;
};

#endif // KPLOTS_H
