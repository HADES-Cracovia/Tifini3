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


#ifndef KTOOLS_H
#define KTOOLS_H

#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLine.h>
#include <TLorentzVector.h>

#include <hparticlecand.h>
#include <hgeomvector.h>

typedef std::pair<HGeomVector, HGeomVector> hgvpair;
typedef std::vector<HParticleCand> EventTracks;

namespace KTifini
{
    static const double TARGET_X_MEAN        = 0.0;
    static const double TARGET_Y_MEAN        = 0.0;
    static const double TARGET_Z_MIN        = -80.0;
    static const double TARGET_Z_MAX        = 0.0;
    static const double TARGET_R_MAX        = 5.0;
    static const double VERTEX_MIN_DIST    = 4.0;

    static const double BEAM_X            = 0.0;
    static const double BEAM_Y            = 0.0;
    static const double BEAM_Z            = 1.0;

    static HGeomVector baseBEAM = HGeomVector(TARGET_X_MEAN,TARGET_Y_MEAN,(TARGET_Z_MAX+TARGET_Z_MIN)/2.0);
    static HGeomVector dirBEAM  = HGeomVector(BEAM_X,BEAM_Y,BEAM_Z);

    void plotVerLine(double x_val, double y_val_low, TH1* Histo, double y_fac, int Line_Col);
    void plotVerLine2(double x_val, double y_val_low,double y_val_high, int Line_Col);
    void plotHorLine(double x1_val, double x2_val, double y_val, int Line_Col);

    TLatex * plotTopLegend(char* label,float x=-1,float y=-1,float size=0.06,int color=1,float angle=0.0);

    void setTH1Histo(TH1 * Histo, const TString & Xtitel, const TString & Ytitel);
    void setTH2Histo(TH2 * Histo, const TString & Xtitel, const TString & Ytitel);
    void setTH1HistoERR(TH1 * Histo, const TString & Xtitel, const TString & Ytitel, int Color);
    void pxpypz(float energy, float theta, float phi, float &px, float &py, float &pz);

    double BetaMomFunc(double * x_val, double * par);
    double MassSq(double Tof, double Momentum, double PathLength, double Rec_Flag);

    double massSelect(double MassSq, int Polarity);

    double PtY(double * yP, double * par);
    double PtY2(double * yP, double * par);
    double MtY(double * yP, double * par);
    double MtY2(double * yP, double * par);

    int findMinValue(double ChiqArray[20], int N_timesteps);
    double mass2Calc(double Tofold, double DeltaTof, double Path, double Momentum);

    void CalcSegVector(double z, double rho, double phi, double theta, HGeomVector & base, HGeomVector & dir);
    double calcDeterminant(const HGeomVector & v1, const HGeomVector & v2, const HGeomVector & v3);

    double calculateMinimumDistanceStraightToPoint(const HGeomVector & base, const HGeomVector & dir, const HGeomVector &point);
    double calculateMinimumDistance(const HGeomVector & base1, const HGeomVector & dir1, const HGeomVector & base2, const HGeomVector & dir2);
    HGeomVector calcVertexAnalytical(const HGeomVector & base1, const HGeomVector & dir1, const HGeomVector & base2, const HGeomVector & dir2);
    HGeomVector calculateCrossPoint(const HGeomVector & base1, const HGeomVector & dir1, const HGeomVector & base2, const HGeomVector & dir2);
    HGeomVector calculatePointOfClosestApproach(const HGeomVector & base1, const HGeomVector & dir1, const HGeomVector & base2, const HGeomVector & dir2);
    HGeomVector calcPrimVertex_Track_Mother(const EventTracks & event, const HGeomVector & DecayVertex, const HGeomVector & dirMother, int trackA_num, int trackB_num, Int_t & pv_type);

    HGeomVector calcPrimVertex_Track_Mother(const EventTracks & event, const HGeomVector & beamVector, const HGeomVector & DecayVertex, const HGeomVector & dirMother, int trackA_num, int trackB_num, Int_t & pv_type);

    hgvpair calcPrimVertex_ALL(const EventTracks & event, const HGeomVector & beam, const HGeomVector & vertex, const HGeomVector & mother, int trA_num, int trB_num, bool scaling = false, bool mother_as_ref = false);
    hgvpair calcPrimVertex_Spec(const EventTracks & event, const HGeomVector & beam, const HGeomVector & vertex, const HGeomVector & mother, int trA_num, int trB_num, bool scaling = false, bool mother_as_ref = false);

    bool IntersectionInTarget(const HGeomVector & beamVector, const HGeomVector & vertex);
    void CalculateEventVertexSL(double phi, double r, double z, HGeomVector * event_vertex, Bool_t reset);

    double calcRapidity(double MomTot, double Momz, double Mass);
    double calcCOS_of_OpeningAngle(TLorentzVector& v1, TLorentzVector& v2);

    double fBeta(double Mass, double Momentum);
    double fTof(double Beta, double PathLength);

    double fDeltaTof(double Mass, double PathLength, double Momentum, double Tof);
    double fDeltaTofMean(double MaxRMS=10.0,int Counter = 0,
                        double DeltaTofA = -1000.0, double SystemA=-10.0,
                        double DeltaTofB = -1000.0, double SystemB=-10.0,
                        double DeltaTofC = -1000.0, double SystemC=-10.0,
                        double DeltaTofD = -1000.0, double SystemD=-10.0);

    double EnergyLossFitMassFunc2(double * x_val, double * par);
    double EnergyLossFitFunc2(double * x_val, double * par);

    double armenteros(double * alphaP, double * par);

    double poly2FitFunc(double * x_val, double * par);
    double poly4FitFunc(double * x_val, double * par);
    double poly5FitFunc(double * x_val, double * par);
    double ratioPoly5FitFunc(double * x_val, double * par);
    double meanRatioPoly5FitFunc(double * x_val, double * par);
    double ratioPoly7FitFunc(double * x_val, double * par);  //früher PolyFitFunc
    double poly7FitFunc(double * x_val, double * par);  //früher PolyFitFunc
    double poly9FitFunc(double * x_val, double * par);
    double poly8LandauFitFunc(double * x_val, double * par);  //früher PolyFitFunc

    double linearFitFunc(double * x_val, double * par);
    double linearFitFunc_Iverse(double * x_val, double * par);
    double expFitFunc(double * x_val, double * par);
    double gaussFitFunc(double * x_val, double * par);
    double landauFitFunc(double * x_val, double * par);
    double polyOneGaussFitFunc(double * x_val, double * par);
    double polyTwoGaussFitFunc(double * x_val, double * par);
    double polyGaussLandauFitFunc(double * x_val, double * par);  //hiess früher polygaussFitFunc
    double legendrePolynomials5_FitFunc(double * x_val, double * par);
};

#endif // KTOOLS_H
