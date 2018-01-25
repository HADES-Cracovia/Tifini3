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

#include "KVertex.h"

#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";

KVertex::KVertex() : HGeomVector()
{
}

KVertex::~KVertex()
{
}

#define CREATE_BRANCH(name, var, type) \
{ \
TString str = TString::Format("f%s%s", unique_name.Data(), name); \
TString str_t = TString::Format("%s/%c", str.Data(), type); \
tree->Branch(str.Data(), &var, str.Data()); \
}

bool KVertex::setTree(TTree* tree, const TString& unique_name, UInt_t b)
{
    if (!tree)
        return false;

    if (b & bXYZ)       CREATE_BRANCH("X",          fX,         'F');
    if (b & bXYZ)       CREATE_BRANCH("Y",          fY,         'F');
    if (b & bXYZ)       CREATE_BRANCH("Z",          fZ,         'F');
    if (b & bR)         CREATE_BRANCH("R",          fR,         'F');

    return false;
}

void KVertex::fill()
{
    fX = HGeomVector::getX();
    fY = HGeomVector::getY();
    fZ = HGeomVector::getZ();
    fR = TMath::Sqrt(fX*fX + fY*fY);
}

KVertex & KVertex::operator=(const KVertex& cand)
{
    *((HGeomVector *)this) = cand;
    return *this;
}

KVertex & KVertex::operator=(const HGeomVector& cand)
{
    *((HGeomVector *)this) = cand;
    return *this;
}
