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


#ifndef KCUTOUTSIDE_H
#define KCUTOUTSIDE_H

#include "KCut.h"

template<typename T>
class KCutOutside : public KCut<T>
{
public:
    KCutOutside(T val_l, T val_h, KT::CompType ct_l, KT::CompType ct_h) :
        val_l(val_l), val_h(val_h), ct_l(ct_l), ct_h(ct_h) {}
    virtual ~KCutOutside() {}

    virtual bool test(T val) {
        return (ct_l==KT::WEAK ? val_l>=val : val_l>val) or (ct_h==KT::WEAK ? val >= val_h : val>val_h);
    }

private:
    T val_l, val_h;
    KT::CompType ct_l, ct_h;
};

#endif // KCUTOUTSIDE_H
