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


#ifndef KVERTEX_H
#define KVERTEX_H

#include "TH2.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TFile.h"
#include "TString.h"

#include "KTifiniAnalysis.h"

class KVertex : public HGeomVector
{
public:
    KVertex();
    virtual ~KVertex();

    KVertex & operator=(const KVertex & cand);
    KVertex & operator=(const HGeomVector & cand);

    Float_t getR() const { return TMath::Sqrt(getX()*getX() + getY()*getY()); }

    enum Branches {
        bXYZ        = 1 << 0,
        bR          = 1 << 1
    };
    bool setTree(TTree * tree, const TString& unique_name, UInt_t b = 0x01);
    void fill();

private:
    TTree * tree;
    TString unique_name;

    Float_t fX, fY, fZ;
    Float_t fR;
};

#endif // KVERTEX_H
