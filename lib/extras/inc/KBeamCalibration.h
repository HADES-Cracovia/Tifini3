/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Rafał Lalik <rafal.lalik@ph.tum.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef KBEAMCALIBARTION_H
#define KBEAMCALIBARTION_H

// ROOT
#include "TString.h"

#include "KAbstractAnalysis.h"

class KBeamCalibration {
public:
    KBeamCalibration(const TargetGeom & TargetGeom);
    ~KBeamCalibration();

    void initBeamCorrArray(KT::AnalysisType analType);
    const HGeomVector & calculateBeamOffset(Int_t RunID);

private:
    static const TString kRunIdFiles;
    static const TString kPrimVertexPos;

    static const Int_t kBeamRunFilesNumber;
    static const Int_t kNTargetCorr;

    Double_t * targetXOffset;
    Double_t * targetYOffset;

    Int_t * runID;
    Int_t * beamTime;

    TString runIdFiles;
    TString primVertexPos;

    TargetGeom tg;

    Bool_t isExp;
    Int_t cachedRunID;

    HGeomVector baseBeam;

    TH1I * runIdUsed;
    TH1D * beamPosX;
    TH1D * beamPosY;
    TH2D * beamPosXY;
};

#endif /* KBEAMCALIBARTION_H */
