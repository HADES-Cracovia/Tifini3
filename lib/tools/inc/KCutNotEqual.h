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


#ifndef KCUTNOTEQUAL_H
#define KCUTNOTEQUAL_H

#include "KCut.h"

template<typename T>
class KCutNotEqual : public KCut<T>
{
public:
    KCutNotEqual(T val_r) : val_r(val_r) {}
    virtual ~KCutNotEqual() {}

    virtual bool test(T val) {
        return (val != val_r);
    }

private:
    T val_r;
};

#endif // KCUTNOTEQUAL_H
