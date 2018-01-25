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

#include "KVector.h"

#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";

KVector::KVector() : HGeomVector()
{
}

KVector::~KVector()
{
}

#define CREATE_BRANCH(name, var, type) \
{ \
TString str = TString::Format("f%s%s", unique_name.Data(), name); \
TString str_t = TString::Format("%s/%c", str.Data(), type); \
tree->Branch(str.Data(), &var, str.Data()); \
}

bool KVector::setTree(TTree* tree, const TString& unique_name)
{
    if (!tree)
        return false;

    CREATE_BRANCH("X",          fX,         'F');
    CREATE_BRANCH("Y",          fY,         'F');
    CREATE_BRANCH("Z",          fZ,         'F');

    return false;
}

void KVector::fill()
{
    fX = HGeomVector::getX();
    fY = HGeomVector::getY();
    fZ = HGeomVector::getZ();
}

KVector & KVector::operator=(const KVector& cand)
{
    *((HGeomVector *)this) = cand;
    return *this;
}

KVector & KVector::operator=(const HGeomVector& cand)
{
    *((HGeomVector *)this) = cand;
    return *this;
}
