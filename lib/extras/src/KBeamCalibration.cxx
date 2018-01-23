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

// ROOT
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
// HYDRA
#include "hgeomvector.h"

#include "KTifiniAnalysis.h"
#include "KBeamCalibration.h"

const TString KBeamCalibration::kRunIdFiles = "/home/gu27buz/hadesdst/pp35/Full_gen4_all_list_RunId.root";
const TString KBeamCalibration::kPrimVertexPos = "/home/gu27buz/hadesdst/pp35/hades_pp35_gen4_ge4_V17fg_PrimVertexPos.root";

const Int_t KBeamCalibration::kBeamRunFilesNumber = 12161;
const Int_t KBeamCalibration::kNTargetCorr = 23;

KBeamCalibration::KBeamCalibration(const TargetGeom & TargetGeom) : runIdFiles(kRunIdFiles), primVertexPos(kPrimVertexPos), tg(TargetGeom) {
    targetXOffset    = new Double_t[kNTargetCorr];
    targetYOffset    = new Double_t[kNTargetCorr];
    runID            = new Int_t[kBeamRunFilesNumber];
    beamTime        = new Int_t[kBeamRunFilesNumber];

    runIdUsed = new TH1I("runIdUsed", "Used RunIDs", 40000, 0, 40000);
    beamPosX = new TH1D("beamPosX", "Beam position X", 40000, 0, 40000);
    beamPosY = new TH1D("beamPosY", "Beam position Y", 40000, 0, 40000);
    beamPosXY = new TH2D("beamPosXY", "Beam position XY", 100, -5, 5, 100, -5, 5);
}

KBeamCalibration::~KBeamCalibration() {
    delete targetXOffset;
    delete targetYOffset;
    delete runID;
    delete beamTime;

    runIdUsed->Write();
    beamPosX->Write();
    beamPosY->Write();
    beamPosXY->Write();
}

void KBeamCalibration::initBeamCorrArray(KT::AnalysisType analType) {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //Correction of the Beam position -->
    //Read in RunID-beamtime-histo into array

    if (analType == KT::Exp) {
        TFile * file_RunID = TFile::Open(runIdFiles);
        if (!file_RunID) {
            std::cout << "File " << runIdFiles << " not open. Aborting!" << std::endl;
            exit(EXIT_FAILURE);
        }

        TH1I * hRunID_Beamtime = (TH1I*)file_RunID->Get("CFile_RunId");

        Int_t k = 0;

         for(Int_t i = 0; i < 40000; ++i) {
//        for(Int_t i = 0; k < kBeamRunFilesNumber; ++i) {
            Int_t bc = hRunID_Beamtime->GetBinContent(i);
            if (bc != 0) {
                runID[k] = bc;        //RunID
                beamTime[k] = i;        //Beamtime in minutes
                ++k;
            }
        }
    }

    //Read in PrimVertexPosition XY for Target Correction per day into array
    if(analType == KT::Exp) {
        TFile * file_PrimVertexPos = TFile::Open(primVertexPos);
        if (!file_PrimVertexPos) {
            std::cout << "File " << primVertexPos << " not open. Aborting!" << std::endl;
            exit(EXIT_FAILURE);
        }
        TH1F * hPrimVertexPos_X = (TH1F*)file_PrimVertexPos->Get("PrimVertexPos_X");
        TH1F * hPrimVertexPos_Y = (TH1F*)file_PrimVertexPos->Get("PrimVertexPos_Y");

        for(Int_t i = 0; i < kNTargetCorr; ++i) {
            targetXOffset[i] = hPrimVertexPos_X->GetBinContent(i+1);
            targetYOffset[i] = hPrimVertexPos_Y->GetBinContent(i+1);
        }
    }
    // <-- Correction of the Beam position
}


const HGeomVector & KBeamCalibration::calculateBeamOffset(Int_t RunID) {
    //cout<<"Estoy calculating the beam offset!!!"<<endl;

    Int_t beamtime = 0;
//    HGeomVector baseBeam;

    //searching the beamtime regarding to the RunID and correction of the targetPos, if RunID has changed
    if (RunID != cachedRunID) {

        for(Int_t i = 0; i < kBeamRunFilesNumber; i++) {
            if (RunID == runID[i]) {
                cachedRunID = RunID;
                beamtime = (Int_t)beamTime[i];
                break;
            }
        }

        Float_t start_time    = 106 * 24.0 * 60.0 + 20*60 + 5; // be07106200516.hld 16-APR-2007 20:05
        Float_t file_time    = beamtime + start_time;         // time and date, when file was taken e.g. 106200516 in minutes
        Int_t file_day        = (Int_t)file_time/(60*24);      // day, when file was taken
        Int_t CorrDay        = file_day - 106;

        if (file_day == 113) {
            Int_t file_min = (Int_t)file_time%(60*24);

            if (file_min < 803) {
                //file be07113132228_dst_apr07_gen2_TS_V18_ge4.root and smaller of day 113
                baseBeam = HGeomVector(targetXOffset[7], targetYOffset[7], (tg.b + tg.a)/2.0);
            } else {
                baseBeam = HGeomVector(targetXOffset[8], targetYOffset[8], (tg.b + tg.a)/2.0);
            }
        } else {
            if (file_day < 113) {
                baseBeam = HGeomVector(targetXOffset[CorrDay], targetYOffset[CorrDay], (tg.b + tg.a)/2.0);
            }
            else
            {
                baseBeam = HGeomVector(targetXOffset[CorrDay+1], targetYOffset[CorrDay+1], (tg.b + tg.a)/2.0);
            }
        }

        if (!runIdUsed->GetBinContent(RunID+1)) {
            runIdUsed->Fill(beamtime, RunID);
            beamPosX->Fill(beamtime, baseBeam.getX());
            beamPosY->Fill(beamtime, baseBeam.getY());
            beamPosXY->Fill(baseBeam.getX(), baseBeam.getY());
        }
    }

    return baseBeam;
}
