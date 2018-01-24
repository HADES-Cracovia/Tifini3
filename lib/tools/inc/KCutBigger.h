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


#ifndef KCUTBIGGER_H
#define KCUTBIGGER_H

#include "KCut.h"

template<typename T>
class KCutBigger : public KCut<T>
{
public:
    KCutBigger(T val_l, KT::CompType ct_l) :
        val_l(val_l), ct_l(ct_l) {}
    virtual ~KCutBigger() {}

    virtual bool test(T val) {
        return (ct_l==KT::WEAK ? val >= val_l : val>val_l);
    }

private:
    T val_l;
    KT::CompType ct_l;
};

#endif // KCUTBIGGER_H
