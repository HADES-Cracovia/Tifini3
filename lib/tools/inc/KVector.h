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


#ifndef KVECTOR_H
#define KVECTOR_H

#include "TH2.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TFile.h"
#include "TString.h"

#include "KTifiniAnalysis.h"

class KVector : public HGeomVector
{
public:
    KVector();
    virtual ~KVector();

    KVector & operator=(const KVector & cand);
    KVector & operator=(const HGeomVector & cand);

    bool setTree(TTree * tree, const TString& unique_name);
    void fill();

private:
    TTree * tree;
    TString unique_name;

    Float_t fX, fY, fZ;
};

#endif // KVECTOR_H
