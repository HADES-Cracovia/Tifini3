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


#ifndef KFITTOOLS_H
#define KFITTOOLS_H

#include "Rtypes.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TF2.h"

class KFitTools {
public:
    KFitTools();
    virtual ~KFitTools();

private:
    Int_t steps;
    Int_t n_div_x;
    Int_t n_div_y;
    Bool_t Smooth_option;
    Float_t start;
    Float_t stop;
    Float_t start_fit;
    Float_t stop_fit;
    Float_t sigma_fac;
    
    TCanvas* Canvas;
    TCanvas* Canvas2;
    TH2F* hist;
    TH1D* ProjY[70];
    TH1D* Proj_pos;
    TH1D* Proj_width;
    TF1*  Fitfunc;
    TString hName;

    void SetName(TString ihName); // Name specifier
    void SetNSteps(Int_t isteps, Float_t istart, Float_t istop); // number of slices in x, starting from istart, stopping at istop
    void SetNDiv(Int_t in_div_x, Int_t in_div_y); // number of divisions of the canvas -> must be correlated with isteps
    TCanvas* getCanvas() const;  // returns the canvas
    TCanvas* getCanvas2() const;  // returns the canvas
    void SetHist(TH2F* h); // Put in the 2-Dim histogram
    void SetFitRange(Float_t istart_fit, Float_t istop_fit); // Fit range for the slices
    void SetFitFunction(TF1* iFitfunc); // Put in the fit function for the x-slices
    void SetSigmaFac(Float_t isigma_fac); // Factor for fit range: sigma_fac * sigma_landau
    void Smooth_Projection(Bool_t  iSmooth_option); //smoothes your projection in case of sparks due to binning effects
    void ProjectHist(); // Loops over all x-slices, projects to y-direction and fits the distribution with iFitfunc
    TH1D* getProjPos(); // Returns the resulting position histogram of the fits
    TH1D* getProjWidth(); // Returns the resultin width histogram of the fits
};

#endif // KFITTOOLS_H
