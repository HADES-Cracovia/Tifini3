/*
Copyright 2018 Rafał Lalik <Rafal.Lalik@uj.edu.pl>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <KTools.h>
#include <TVirtualPad.h>
#include <TMath.h>
#include <TF3.h>
#include <TH3.h>

#include <hphysicsconstants.h>

#include <cmath>
#include <iostream>
#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";

namespace KTifini
{
//HGeomVector baseBEAM = HGeomVector(TARGET_X_MEAN,TARGET_Y_MEAN,(TARGET_Z_MAX+TARGET_Z_MIN)/2.0);
//HGeomVector dirBEAM  = HGeomVector(BEAM_X,BEAM_Y,BEAM_Z);

TLatex * plotTopLegend(char * label,float x,float y,float size,int color,float angle)
{
    // coordinates in NDC!
    // plots the string label in position x and y in NDC coordinates
    // size is the text size
    // color is the text color

    if ( x < 0 || y < 0 ) {                    // defaults
        x = gPad->GetLeftMargin() * 1.15;
        y = (1-gPad->GetTopMargin()) * 1.04;
    }
    TLatex * text = new TLatex(x,y,label);
    text->SetTextSize(size);
    text->SetNDC();
    text->SetTextColor(color);
    text->SetTextAngle(angle);
    text->Draw();
    return text;
}

void plotVerLine(double x_val, double y_val_low, TH1* Histo, double y_fac, int Line_Col) {
    TLine * Zero_line = new TLine();
    Zero_line->SetX1(x_val);
    Zero_line->SetX2(x_val);
    Zero_line->SetY1(y_val_low);
    Zero_line->SetY2(y_fac*Histo->GetBinContent(Histo->FindBin(x_val)));
    //cout << "x_val = " << x_val << ", Bin = " << Histo->FindBin(x_val) << ", Y2 = " << Histo->GetBinContent(Histo->FindBin(x_val)) << endl;
    Zero_line->SetLineWidth(2);
    Zero_line->SetLineStyle(2);
    Zero_line->SetLineColor(Line_Col);
    Zero_line->Draw();
    //delete Zero_line;
}

void plotVerLine2(double x_val, double y_val_low,double y_val_high, int Line_Col) {
    TLine* Zero_line = new TLine();
    Zero_line->SetX1(x_val);
    Zero_line->SetX2(x_val);
    Zero_line->SetY1(y_val_low);
    Zero_line->SetY2(y_val_high);
    //cout << "x_val = " << x_val << ", Bin = " << Histo->FindBin(x_val) << ", Y2 = " << Histo->GetBinContent(Histo->FindBin(x_val)) << endl;
    Zero_line->SetLineWidth(2);
    Zero_line->SetLineStyle(2);
    Zero_line->SetLineColor(Line_Col);
    Zero_line->Draw("same");
    //delete Zero_line;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-..-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void plotHorLine(double x1_val, double x2_val, double y_val, int Line_Col) {
    TLine* Zero_line = new TLine();
    Zero_line->SetX1(x1_val);
    Zero_line->SetX2(x2_val);
    Zero_line->SetY1(y_val);
    Zero_line->SetY2(y_val);
    Zero_line->SetLineWidth(2);
    Zero_line->SetLineStyle(2);
    Zero_line->SetLineColor(Line_Col);
    Zero_line->Draw();
    //delete Zero_line;
}

void setTH1Histo(TH1 * Histo, const TString & Xtitel, const TString & Ytitel) {
    Histo->SetStats(0);
    Histo->SetTitle("");
    Histo->GetYaxis()->SetTitleOffset(1.4);
    Histo->GetXaxis()->SetLabelSize(0.05);
    Histo->GetYaxis()->SetLabelSize(0.05);
    Histo->GetXaxis()->SetTitleSize(0.06);
    Histo->GetYaxis()->SetTitleSize(0.06);
    Histo->GetXaxis()->CenterTitle();
    Histo->GetYaxis()->CenterTitle();
    Histo->GetXaxis()->SetNdivisions(5);
    Histo->GetYaxis()->SetNdivisions(5);
    Histo->GetXaxis()->SetTitle(Xtitel);
    Histo->GetYaxis()->SetTitle(Ytitel);
}

//-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-°-
void setTH2Histo(TH2 * Histo, const TString & Xtitel, const TString & Ytitel) {
    Histo->SetStats(0);
    Histo->SetTitle("");
    Histo->GetYaxis()->SetTitleOffset(1.2);
    Histo->GetXaxis()->SetLabelSize(0.05);
    Histo->GetYaxis()->SetLabelSize(0.05);
    Histo->GetXaxis()->SetTitleSize(0.06);
    Histo->GetYaxis()->SetTitleSize(0.06);
    Histo->GetXaxis()->CenterTitle();
    Histo->GetYaxis()->CenterTitle();
    Histo->GetXaxis()->SetNdivisions(5);
    Histo->GetYaxis()->SetNdivisions(5);
    Histo->GetXaxis()->SetTitle(Xtitel);
    Histo->GetYaxis()->SetTitle(Ytitel);
}

void setTH1HistoERR(TH1 * Histo, const TString & Xtitel, const TString & Ytitel, int Color) {
    Histo->SetStats(0);
    //Histo->SetTitle("");
    Histo->GetYaxis()->SetTitleOffset(1.3);
    Histo->GetYaxis()->SetTitleOffset(1.2);
    Histo->GetXaxis()->SetLabelSize(0.05);
    Histo->GetYaxis()->SetLabelSize(0.05);
    Histo->GetXaxis()->SetTitleSize(0.06);
    Histo->GetYaxis()->SetTitleSize(0.06);
    Histo->GetXaxis()->CenterTitle();
    Histo->GetYaxis()->CenterTitle();
    Histo->GetXaxis()->SetNdivisions(5);
    Histo->GetYaxis()->SetNdivisions(5);
    Histo->GetXaxis()->SetTitle(Xtitel);
    Histo->GetYaxis()->SetTitle(Ytitel);
    Histo->SetMarkerStyle(21);
    Histo->SetMarkerSize(0.6);
    Histo->SetLineWidth(1);
    Histo->SetLineColor(Color);
    Histo->SetMarkerColor(Color);
}
//eventuell ausgliedern!!!!
void pxpypz(float energy, float theta, float phi, float & px, float & py, float & pz){
    px = energy*sin(theta)*cos(phi);
    py = energy*sin(theta)*sin(phi);
    pz = energy*cos(theta);
}

double poly2FitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    x = x_val[0];
    y = par0 + par1*x + par2*x*x;
    return y;
}

double poly4FitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2, par3, par4;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    par3    = par[3];
    par4    = par[4];

    x = x_val[0];
    y = par0 + par1*x + par2*x*x + par3*x*x*x + par4*x*x*x*x;
    return y;
}

double poly5FitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2, par3, par4, par5;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    par3    = par[3];
    par4    = par[4];
    par5    = par[5];

    x = x_val[0];
    y = par0 + par1*x + par2*x*x + par3*x*x*x + par4*x*x*x*x + par5*x*x*x*x*x;
    return y;
}

double ratioPoly5FitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2, par3, par4, par5, par6, par7, par8, par9, par10, par11;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    par3    = par[3];
    par4    = par[4];
    par5    = par[5];
    par6    = par[6];
    par7    = par[7];
    par8    = par[8];
    par9    = par[9];
    par10 = par[10];
    par11 = par[11];

    x = x_val[0];
    y = (par0 + par1*x + par2*x*x + par3*x*x*x + par4*x*x*x*x + par5*x*x*x*x*x)/(par6 + par7*x + par8*x*x + par9*x*x*x + par10*x*x*x*x + par11*x*x*x*x*x);
    return y;
}

double meanRatioPoly5FitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2, par3, par4, par5, par6, par7, par8, par9, par10, par11, par12, par13, par14, par15, par16, par17, par18, par19, par20, par21, par22, par23;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    par3    = par[3];
    par4    = par[4];
    par5    = par[5];
    par6    = par[6];
    par7    = par[7];
    par8    = par[8];
    par9    = par[9];
    par10    = par[10];
    par11    = par[11];
    par12    = par[12];
    par13    = par[13];
    par14    = par[14];
    par15    = par[15];
    par16    = par[16];
    par17    = par[17];
    par18    = par[18];
    par19    = par[19];
    par20    = par[20];
    par21    = par[21];
    par22    = par[22];
    par23    = par[23];

    x = x_val[0];
    y = 0.5*((par0 + par1*x + par2*x*x + par3*x*x*x + par4*x*x*x*x + par5*x*x*x*x*x)/(par6 + par7*x + par8*x*x + par9*x*x*x + par10*x*x*x*x + par11*x*x*x*x*x)+(par12 + par13*x + par14*x*x + par15*x*x*x + par16*x*x*x*x + par17*x*x*x*x*x)/(par18 + par19*x + par20*x*x + par21*x*x*x + par22*x*x*x*x + par23*x*x*x*x*x));
    return y;
}

double ratioPoly7FitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2, par3, par4, par5, par6, par7, par8, par9, par10, par11, par12, par13, par14, par15;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    par3    = par[3];
    par4    = par[4];
    par5    = par[5];
    par6    = par[6];
    par7    = par[7];
    par8    = par[8];
    par9    = par[9];
    par10    = par[10];
    par11    = par[11];
    par12    = par[12];
    par13    = par[13];
    par14    = par[14];
    par15    = par[15];

    x = x_val[0];
    y = (par0+par1*x+par2*x*x+par3*x*x*x+par4*x*x*x*x+par5*x*x*x*x*x+par6*x*x*x*x*x*x+par7*x*x*x*x*x*x*x)/(par8+par9*x+par10*x*x+par11*x*x*x+par12*x*x*x*x+par13*x*x*x*x*x+par14*x*x*x*x*x*x+par15*x*x*x*x*x*x*x);
    return y;
}

double poly7FitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2, par3, par4, par5, par6, par7;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    par3    = par[3];
    par4    = par[4];
    par5    = par[5];
    par6    = par[6];
    par7    = par[7];
    x = x_val[0];
    y = par0+par1*x+par2*x*x+par3*x*x*x+par4*x*x*x*x+par5*x*x*x*x*x+par6*x*x*x*x*x*x+par7*x*x*x*x*x*x*x;
    return y;
}

double Poly9FitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2, par3, par4, par5, par6, par7, par8, par9;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    par3    = par[3];
    par4    = par[4];
    par5    = par[5];
    par6    = par[6];
    par7    = par[7];
    par8    = par[8];
    par9    = par[9];
    x = x_val[0];
    y = par0+par1*x+par2*x*x+par3*x*x*x+par4*x*x*x*x+par5*x*x*x*x*x+par6*x*x*x*x*x*x+par7*x*x*x*x*x*x*x+par8*x*x*x*x*x*x*x*x+par9*x*x*x*x*x*x*x*x*x;
    return y;
}

double poly8LandauFitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2, par3, par4, par5, par6, par7;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    par3    = par[3];
    par4    = par[4];
    par5    = par[5];
    par6    = par[6];
    par7    = par[7];
    x = x_val[0];
    y = ((par0+par1*x+par2*x*x+par3*x*x*x+par4*x*x*x*x)+par5*(TMath::Landau(x,par6,par7,0)));
    return y;
}

double linearFitFunc(double * x_val, double * par) {
    double x, y, par0, par1;
    par0    = par[0];
    par1    = par[1];
    x = x_val[0];
    y = par0 + par1*x;
    return y;
}

double linearFitFunc_Iverse(double * x_val, double * par) {
    double x, y, par0, par1;
    par0    = par[0];
    par1    = par[1];
    x = x_val[0];
    y = (x-par0)/par1;
    return y;
}

double expFitFunc(double * x_val, double * par) {
    double x, y, par0, par1;
    par0    = par[0];
    par1    = par[1];
    x = x_val[0];
    y = par0*TMath::Exp(-x/par1);
    return y;
}

double gaussFitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    x = x_val[0];
    y = par0*TMath::Gaus(x,par1,par2,0);
    return y;
}

double landauFitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    x = x_val[0];
    y = par2*(TMath::Landau(x,par0,par1,0));
    return y;
}

double polyOneGaussFitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2, par3, par4, par5, par6, par7, par8;//, par9, par10;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    par3    = par[3];
    par4    = par[4];
    par5    = par[5];
    par6    = par[6];
    par7    = par[7];
    par8    = par[8];
    x = x_val[0];
    y = (par0*(TMath::Gaus(x,par1,par2,0))+(par3+par4*x+par5*x*x+par6*x*x*x+par7*x*x*x*x+par8*x*x*x*x*x));
    return y;
}

double polyTwoGaussFitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2, par3, par4, par5, par6, par7, par8, par9, par10, par11, par12, par13;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    par3    = par[3];
    par4    = par[4];
    par5    = par[5];
    par6    = par[6];
    par7    = par[7];
    par8    = par[8];
    par9    = par[9];
    par10    = par[10];
    par11    = par[11];
    par12    = par[12];
    par13    = par[13];
    x = x_val[0];
    y = par0*(TMath::Gaus(x,par1,par2,0))+par3*(TMath::Gaus(x,par4,par5,0))+(par6+par7*x+par8*x*x+par9*x*x*x+par10*x*x*x*x+par11*x*x*x*x*x+par12*x*x*x*x*x*x+par13*x*x*x*x*x*x*x);
    return y;
}

double polyGaussLandauFitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2, par3, par4, par5, par6, par7, par8, par9, par10;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    par3    = par[3];
    par4    = par[4];
    par5    = par[5];
    par6    = par[6];
    par7    = par[7];
    par8    = par[8];
    par9    = par[9];
    par10 = par[10];
    x = x_val[0];
    y = (par0*(TMath::Gaus(x,par1,par2,0))+(par3+par4*x+par5*x*x+par6*x*x*x+par7*x*x*x*x)+par8*(TMath::Landau(x,par9,par10,0)));
    return y;
}

double legendrePolynomials5_FitFunc(double * x_val, double * par) {
    double x, y, par0, par1, par2, par3, par4, par5;
    par0    = par[0];
    par1    = par[1];
    par2    = par[2];
    par3    = par[3];
    par4    = par[4];
    par5    = par[5];

    x = x_val[0];
    y = par0*1.0 + par1*x + par2*0.5*((3*x*x)-1) + par3*0.5*((5*pow(x,3))-3*x) + par4*(1/8)*(35*pow(x,4)-30*pow(x,2)+3) + par5*(1/8)*(63*pow(x,5)-70*pow(x,3)-15*x);
    return y;
}

double BetaMomFunc(double * x_val, double * par) {
    //prints you the curves in your Beta vs momentum distribution

    double p, mass, beta;
    mass    = HPhysicsConstants::mass(par[0]);
    p = x_val[0];
    beta = sqrt(1.0/(((mass*mass)/(p*p)) + 1.0));
    return beta;
}

double MassSq(double Tof, double Momentum, double PathLength, double Rec_Flag) {
    // calculates mass*mass as a function of momentum, time-of-flight and path length
    double Velocity = 0.0;
    double MassSq     = 3240000;
    double clighty    = 299792458.0; // [m/s]
    double BetaSq     = 0.0;
    double GammaSq    = 0.0;
    if(Tof > 0.0 && PathLength >= 0) {
        Velocity = PathLength/Tof;
        BetaSq     = (Velocity/clighty)*(Velocity/clighty);
        GammaSq    = 1.0/(1.0-BetaSq);
        MassSq     = Momentum*Momentum/(BetaSq*GammaSq);
        Rec_Flag = 5;
        return MassSq;
    } else {
        //std::cout << "WARNING: time-of-flight < 0: " << Tof << std::endl;
        Rec_Flag = 10;
        return MassSq;
    }
}

double PtY(double * yP, double * par) {
    // Angle lines
    double M            = par[0];
    double ThetaLab    = par[1];
    double Part_A        = 0.0;
    double pt            = 0.0;
    ThetaLab            = TMath::Pi()*ThetaLab/180.0;
    double y            = yP[0];

    Part_A = pow(1/(tan(ThetaLab)*sinh(y)),2.0)-1.0;

    if(Part_A >= 0.0) {
        pt = M*pow(Part_A,-0.5);
    } else return 1000000.0;

    if(pt > 0.0 && pt < 10000.0) {
        return pt;
    } else {
        return 1000000.0;
    }

    return pt;
}

double PtY2(double * yP, double * par) {
    // Momentum lines
    double M                = par[0];
    double PLab = par[1];
    double y = yP[0];

    double pt = sqrt((pow(PLab,2)-pow(M*sinh(y),2))/(pow(sinh(y),2)+1.0));

    if(pt > 0.0 && pt < 10000.0) {
        return pt;
    } else {
        return 0.0;
    }

    return pt;
}

double MtY(double * yP, double * par) {
    // Angle lines
    double M            = par[0];
    double ThetaLab    = par[1];
    double Part_A        = 0.0;
    double pt            = 0.0;
    double mt            = 0.0;
    ThetaLab             = TMath::Pi()*ThetaLab/180.0;
    double y            = yP[0];

    Part_A = pow(1/(tan(ThetaLab)*sinh(y)),2.0)-1.0;

    if (Part_A >= 0.0) {
        pt = M*pow(Part_A,-0.5);
        mt = sqrt(pt*pt+M*M)-M;
    } else
        return 1000000.0;

    if(pt > 0.0 && pt < 10000.0) {
        return pt;
    } else {
        return 1000000.0;
    }

    return pt;
}

double MtY2(double * yP, double * par) {
    // Momentum lines
    double M        = par[0];
    double PLab    = par[1];
    double y        = yP[0];
    double mt        = 0.0;

    double pt = sqrt((pow(PLab,2)-pow(M*sinh(y),2))/(pow(sinh(y),2)+1.0));
    mt = sqrt(pt*pt+M*M)-M;

    if(pt > 0.0 && pt < 10000.0) {
        return mt;
    } else {
        return 0.0;
    }

    return mt;
}

double armenteros(double * alphaP, double * par) {
    // Armenteros-Podolanski plot with mother particle mass M,
    // daughter particle masses m1 (positive) and m2 (negative)
    // and mother particle momentum P.
    // alpha is the variable (p1l-p2l)/(p1l+p2l)
    double M    = par[0];
    double m1 = par[1];
    double m2 = par[2];
    double P    = par[3];
    double alpha = alphaP[0];

    if(alpha < -1.0 || alpha > 1.0 || m1 <= 0.0 || m2 <= 0.0 || M <= 0.0 || (m1+m2) > M || P <= 0.0) {
        return -1.0;
    }

    double ps         = (1.0/(2.0*M))*sqrt(pow(M,2)-pow(m1+m2,2))*sqrt(pow(M,2)-pow(m1-m2,2));
    double alphas = (pow(m1,2)-pow(m2,2))/pow(M,2);
    double pt = sqrt(pow(ps,2)-(1.0/(4.0*((1.0/pow(M,2))+(1.0/pow(P,2)))))*pow(alpha-alphas,2));

    if(pt > 0.0 && pt < 10000.0) {
        return pt;
    } else {
        return 0.0;
    }
}

double massSelect(double MassSq, int Polarity) {
    // selects the particle mass (e.g. proton (931 MeV, pion 139 MeV)) which is closest to the input mass
    double ProtonMass     = 938.271998;
    double PionMass         = 139.57018;
    double ElectronMass = 0.510998918;
    double KaonMass         = 493.677;
    double Mass                 = 0.0;
    if(MassSq < 0.0) return ElectronMass;
    else Mass = sqrt(MassSq);

    if(Polarity > 0) {
        if(Mass < (ElectronMass+PionMass)/2.0)                                                                         return ElectronMass;
        if(Mass > (ElectronMass+PionMass)/2.0 && Mass < (PionMass+KaonMass)/2.0)     return PionMass;
        if(Mass > (PionMass+KaonMass)/2.0         && Mass < (ProtonMass+KaonMass)/2.0) return KaonMass;
        if(Mass > (ProtonMass+KaonMass)/2.0)                                                                             return ProtonMass;
        else {
            std::cout << "WARNING: Mass = 0.0!" << std::endl;
            return 0.0;
        }
    }

    if(Polarity < 0) {
        if(Mass < (ElectronMass+PionMass)/2.0)                                                                         return ElectronMass;
        if(Mass > (ElectronMass+PionMass)/2.0 && Mass < (PionMass+KaonMass)/2.0)     return PionMass;
        if(Mass > (PionMass+KaonMass)/2.0)                                                                                 return KaonMass;
        else {
            std::cout << "WARNING: Mass = 0.0!" << std::endl;
            return 0.0;
        }
    }
    return 0.0;
}

int findMinValue(double ChiqArray[20], int N_timesteps) {
    int MinBin = 0;
    double TestVal = ChiqArray[0];
    for(int i = 1; i < N_timesteps; i++) {
        //std::cout << "ChiqArray[i] = " << ChiqArray[i] << std::endl;
        if(ChiqArray[i] < TestVal) {
            TestVal = ChiqArray[i];
            MinBin    = i;
        }
    }
    return MinBin;
}

double mass2Calc(double Tofold, double DeltaTof, double Path, double Momentum) {
    double NewMass2    = -1000.0;
    double NewTof        = Tofold+DeltaTof;
    //double NewBeta     = ((Path/1000.0)/NewTof)/clight;
    NewMass2    = MassSq(NewTof, Momentum, Path/1000.0, 1.0);
    return    NewMass2;
}

void CalcSegVector(double z, double rho, double phi, double theta, HGeomVector & base, HGeomVector & dir)
{
    static const double pi2 = TMath::PiOver2();
    base.setXYZ(rho*cos(phi+pi2), rho*sin(phi+pi2), z);
    dir.setXYZ(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
}

double calcDeterminant(const HGeomVector & v1, const HGeomVector & v2, const HGeomVector & v3) {
    // calculating the Determinant of a 3 x 3 Matrix 
    // with the column vectors [v1, v2, v3]
    // using the RULE of SARRUS
    //
    // | v1(0)   v2(0)   v3(0) |      | v1(0) v2(0) v3(0)|v1(0) v2(0)  .
    // |                       |      |  \\     \\     X   |  /     /    . 
    // |                       |      |   \\     \\   / \\  | /     /     . 
    // |                       |      |    \\     \\ /   \\ |/     /      . 
    // |                       |      |     \\     X     \\/     /       . 
    // |                       |      |      \\   / \\    /\\    /        .  
    // |                       |      |       \\ /   \\  / |\\  /         . 
    // | v1(1)   v2(1)   v3(1) |   =  | v1(1) v2(1) v3(1)|v1(1) v2(1)  .
    // |                       |      |       / \\    /\\  | /\\          . 
    // |                       |      |      /   \\  /  \\ |/  \\         . 
    // |                       |      |     /     \\/    \\/    \\        . 
    // |                       |      |    /      /\\    /\\     \\       . 
    // |                       |      |   /      /  \\  / |\\     \\      .  
    // |                       |      |  /      /    \\/  | \\     \\     . 
    // | v1(2)   v2(2)   v3(2) |      | v1(2) v2(2) v3(2)| v1(2) v2(2) .  
    //                                 /      /     /  \\     \\     \\   .
    //                                                                
    //                                -      -     -    +     +     +  .

    return ( v1(0) * v2(1) * v3(2) 
        + v2(0) * v3(1) * v1(2) 
        + v3(0) * v1(1) * v2(2) 
        - v3(0) * v2(1) * v1(2) 
        - v1(0) * v3(1) * v2(2) 
        - v2(0) * v1(1) * v3(2) ); 
}

double calculateMinimumDistanceStraightToPoint(const HGeomVector & base, const HGeomVector & dir, const HGeomVector & point) {
    // calculates the minimum distance of a point to a straight given as parametric straight x = base + n * dir

    if (!(dir.length() > 0)) {
        return -1000000.;
    }

    HGeomVector diff = base - point;
    HGeomVector cross = dir.vectorProduct(diff);
    
    return cross.length()/dir.length();
}

double calculateMinimumDistance(const HGeomVector & base1, const HGeomVector & dir1, const HGeomVector & base2, const HGeomVector & dir2) {
    // calculates the minimum distance of two tracks given as parametric straights x = base + n * dir
    HGeomVector cross = dir1.vectorProduct(dir2);
    HGeomVector ab = base1 - base2;

    if (!(fabs(cross.length()) >0.)) { // dir1 || dir2
        return calculateMinimumDistanceStraightToPoint(base1, dir1, base2);
    }

    return fabs(ab.scalarProduct(cross)/cross.length());
}

HGeomVector calcVertexAnalytical(const HGeomVector & base1, const HGeomVector & dir1, const HGeomVector & base2, const HGeomVector & dir2) {
    // Calculates the Vertex of two straight lines define by the vectors base and dir
    //
    //      g: x1 = base1 + l * dir1 
    //      h: x2 = base2 + m * dir2 , where l,m are real numbers 
    //
    // 1. are g and h
    //       parallel / identical, i.e. are dir1 and dir2 linear dependent?
    //       
    //                                        /-                               
    //                                        |
    //                                        |   = 0    linear dependent, no unique solution, returning dummy  
    //      => cross product : dir1 x dir2 = -|  
    //                                        |  != 0    linear independent
    //                                        |
    //                                        \\-         
    //
    // 2. are g and h 
    //       skew or do they have a crossing point, i.e are dir1, dir2 and (base1 - base2) linear dependent ?
    //
    //                                                    /-                               
    //                                                    |
    //                                                    |   = 0    linear dependent
    //                                                    |          g and h are intersecting
    //                                                    |          calculating vertex as point of intersection
    //                                                    |
    //    => determinant: det[ dir1, dir2, base1-base2]= -|
    //                                                    |  != 0    linear independent
    //                                                    |          g and h are skew
    //                                                    |          calulating vertex as point of closest approach
    //                                                    |
    //                                                    \\-         
    //  
    // 3.
    //    (a) calculating intersection point
    //    (b) calculating point of closest approach



    // 1. exists a unique solution ?

    if ((dir1.vectorProduct(dir2)).length()> 0.)    // dir1 and dir2 linear independent
    {
        // straight lines are either skew or have a cross point
        HGeomVector diff = base1;
        diff -= base2; // Difference of two base vectors base1 - base2
        
        // 2. skew or intersecting ?
        if (fabs(KTifini::calcDeterminant(dir2, dir1 ,diff))>0.) {
        // 3. (b) skew 
            return HGeomVector(calculatePointOfClosestApproach(base1, dir1, base2, dir2));
        } else {
        // 3. (a) intersection 
            return HGeomVector(calculateCrossPoint(base1 ,dir1, base2 ,dir2));
        }
    }
    else
    {
        // dir1 and dir2 linear dependent -> g1 and g2 identical or parallel
        return HGeomVector(-10000000.,-10000000.,-10000000.);
    }
    return HGeomVector(-10000000.,-10000000.,-10000000.);
}

HGeomVector calculateCrossPoint(const HGeomVector & base1, const HGeomVector & dir1, const HGeomVector & base2, const HGeomVector & dir2) {
    /* calculating cross point
    // taking all three equations into account solving the overdetermined set of lin. equations
    // of 
    // base1 + l * dir2 =  base1 + m * dir2 
    //
    // set of lin. equations:
    //  
    //   base1(0) + l * dir1(0) = base2(0) + m * dir2(0) 
    //   base1(1) + l * dir1(1) = base2(1) + m * dir2(1)
    //   base1(2) + l * dir1(2) = base2(2) + m * dir2(2) this line is ignored
    //
    //   written in matrix form
    //
    //        l \\\
    //   M * |   | = base2 - base1
    //       \\ m /
    //
    //   M is a 3x2 matrix
    //     
    // to solve multiply the equation by the transposed Matrix of M from the left: M 
    //     
    //  T      /  l \\                                                               .
    // M * M * |    | = M  * (base2 - base1)
    //         \\ -m /
    // MIND THE '-' of m
    //
    //     / dir1(0) dir2(0) \\                                                      .
    //     |                 |    T   / dir1(0) dir1(1) dir1(2) \\                   .
    // M = | dir1(1) dir2(1) |,  M  = |                         |
    //     |                 |        \\ dir2(0) dir2(1) dir2(2) /                   .
    //     \\ dir1(2) dir2(2) /                                    
    //
    //  T      / (dir1(0)*dir1(0) + dir1(1)*dir1(1) + dir1(2)*dir1(2))   (dir1(0)*dir2(0) + dir1(1)*dir2(1) + dir1(2)*dir2(2))  \\ .
    // M * M = |                                                                                                                |
    //         \\ (dir1(0)*dir2(0) + dir1(1)*dir2(1) + dir1(2)*dir2(2))   (dir2(0)*dir2(0) + dir2(1)*dir2(1) + dir2(2)*dir2(2))  /                        
    //
    //  T       / d1d1 d1d2 \\                           .
    // M  * M = |           |
    //          \\ d1d2 d2d2 /
    //
    // diff = base2 - base1
    //
    //  T           /  (dir1(0)*diff(0) + dir1(1)*diff(1) + dir1(2)*diff(2)) \\         .
    // M  * diff =  |                                                        |
    //              \\  (dir2(0)*diff(0) + dir2(1)*diff(1) + dir2(2)*diff(2)) /
    //
    //  T           /  d1diff  \\                                          .
    // M  * diff =  |          |
    //              \\  d2diff  /
    // 
    // now the new Matrix set is to be solved by CRAMER'S Rule:
    // 
    // / d1d1 d1d2 \\   /  l \\   /  d1diff \\                   .
    // |           | * |    | = |          |
    // \\ d1d2 d2d2 /   \\ -m /   \\  d2diff /
    //
    //     | d1d1 d1d2 |
    // D = |           | = d1d1*d2d2 - d1d2*d1d2;
    //     | d1d2 d2d2 |
    // 
    //     | d1diff d1d2 |
    // Dl= |              | = d1diff*d2d2 - d1d2*d2diff;
    //     | d2diff d2d2 |              
    //
    // l = Dl/D = l_cross
    // 
    // vertex = base1 + l_cross * dir1
    */

    double d1d1 = dir1(0)*dir1(0) + dir1(1)*dir1(1) + dir1(2)*dir1(2);
    double d2d2 = dir2(0)*dir2(0) + dir2(1)*dir2(1) + dir2(2)*dir2(2);
    double d1d2 = dir1(0)*dir2(0) + dir1(1)*dir2(1) + dir1(2)*dir2(2);
    
    double D = d1d1*d2d2 - (d1d2*d1d2);
    
    if (!(fabs(D) > 0.)) {
        ::Warning("calculateCrossPoint","Error while calculating cross point ... eqns are lin. dependent:returning default Vertex (-20000,-20000,-20000)");
        return HGeomVector(-20000.,-20000.,-20000.);
    }

    double d1diff = dir1(0)*(base2(0)-base1(0))+dir1(1)*(base2(1)-base1(1))+dir1(2)*(base2(2)-base1(2));
    double d2diff = dir2(0)*(base2(0)-base1(0))+dir2(1)*(base2(1)-base1(1))+dir2(2)*(base2(2)-base1(2));

    double Dlambda = d1diff*d2d2-d1d2*d2diff;
    
    double lambda = Dlambda/D;
    
    HGeomVector vertex;
    vertex += dir1;
    vertex *= lambda;
    vertex += base1;

    //std::cout << "Cross point calculated" << std::endl;
    return HGeomVector(vertex);

    // return HGeomVector(-20000.,-20000.,-20000.);
}
HGeomVector calculatePointOfClosestApproach(const HGeomVector & base1, const HGeomVector & dir1, const HGeomVector & base2, const HGeomVector & dir2) {
//  calculating point of closest approach
//        
//        from the equations of the straight lines of g and h 
//        g: x1 = base1 + l * dir1       
//        h: x2 = base2 + m * dir2 
//        
//        you can construct the following planes:
//        
//        E1: e1 = base1  +  a * dir1  +  b * (dir1 x dir2)
//        E2: e2 = base2  +  s * dir2  +  t * (dir1 x dir2)
//        
//        now the intersection point of E1 with g2 = {P1} 
//        and the intersection point of E2 with g1 = {P2}
//        
//        form the base points of the perpendicular to both straight lines.
//        
//        The point of closest approach is the middle point between P1 and P2: 
//        
//        vertex = (p2 - p1)/2
// 
//        E1 ^ g2:
//
//           e1 = x2
//    -->    base1  +  a * dir1  +  b * (dir1 x dir2) = base2 + m * dir2 
//    -->    base1 - base2 = m * dir2  -  a * dir1  -  b * (dir1 x dir2)       
//                                          (m)
//    -->    [ dir2, -dir1, -(dir1 x dir2)] (a) = base1 - base2        
//                                          (b)
//           
//           using CRAMER's RULE you can find the solution for m (a,b, not used)
//           
//           using the rules for converting determinants:
//           
//           D12 = det [dir2, -dir1, -(dir1 x dir2)] 
//               = det [dir2,  dir1,  (dir1 x dir2)]
//           
//           Dm  = det [base1 - base2, -dir1, -(dir1 x dir2)]
//               = det [base1 - base2,  dir1,  (dir1 x dir2)]
//  
//            m  = Dm/D12
//           
//           P1: p1 = x2(m)
//                  = base2 + Dm/D12 * dir2
//
//        E2 ^ g1:
//
//           e2 = x1
//    -->    base2  +  s * dir2  +  t * (dir1 x dir2) = base1 + l * dir1 
//    -->    base2 - base1 = l * dir1  -  s * dir2  -  t * (dir1 x dir2)       
//                                          (l)
//    -->    [ dir1, -dir2, -(dir1 x dir2)] (s) = base2 - base1        
//                                          (t)
//           
//           again using CRAMER's RULE you can find the solution for m (a,b, not used)
//           
//           using the rules for converting determinants:
//           
//           D21 =  det [dir1, -dir2, -(dir1 x dir2)] 
//               =  det [dir1,  dir2,  (dir1 x dir2)]
//               = -det [dir2,  dir1,  (dir1 x dir2)]
//               = -D12
//           
//           Dl  =  det [base2 - base1, -dir2, -(dir1 x dir2)]
//               =  det [base2 - base1,  dir1,  (dir1 x dir2)]
//               = -det [base1 - base2,  dir1,  (dir1 x dir2)]
//
//            l  =   Dl/D21
//               = - Dl/D12
//           
//           P2: p2 = x1(m)
//                  = base1 - Dl/D12 * dir1
//           
//           
//           vertex = p1 + 1/2 * (p2 - p1)
//                  = 1/2 * (p2 + p1)
//                  = 1/2 *( (base1 + base2) +  1/D12 * ( Dm * dir2 - Dl * dir1) )
//                      

    HGeomVector cross = dir1.vectorProduct(dir2); // cross product: dir1 x dir2 

    // straight lines are either skew or have a cross point
            
    HGeomVector diff = base1;
    diff-=base2; // Difference of two base vectors base1 - base2
            
    double D;
    D =  KTifini::calcDeterminant(dir2, dir1 ,cross);

    if (!(fabs(D) > 0.))
    {
        ::Warning(":calculatePointOfClosestApproach","Dirs and cross-product are lin. dependent: returning default Vertex (-20000,-20000,-20000)");
        return HGeomVector(-20000.,-20000.,-20000.);
    }

    double Dm =  KTifini::calcDeterminant(diff , dir1, cross);
    double Dl = -KTifini::calcDeterminant(diff , dir2, cross);

    HGeomVector vertex;
    HGeomVector dm;
    HGeomVector dl;

    dm = dir2;
    dm *= Dm;

    dl = dir1;
    dl *= Dl;

    vertex = dm - dl;

    vertex *= ((1.)/D);

    vertex+=base1;
    vertex+=base2;
    vertex*=0.5;

    return HGeomVector(vertex);
}

HGeomVector calcPrimVertex_Track_Mother(const EventTracks & event, const HGeomVector & DecayVertex, const HGeomVector & dirMother, int trackA_num, int trackB_num, Int_t & pv_type)
{
    return calcPrimVertex_Track_Mother(event, baseBEAM, DecayVertex, dirMother, trackA_num, trackB_num, pv_type);
}

HGeomVector calcPrimVertex_Track_Mother(const EventTracks & event, const HGeomVector & beamVector, const HGeomVector & DecayVertex, const HGeomVector & dirMother, int trackA_num, int trackB_num, Int_t & pv_type)
{
    size_t run_nevents = event.size();
//PR(run_nevents);
    TLorentzVector trackArray[run_nevents];
    HGeomVector baseArray[run_nevents];
    HGeomVector dirArray[run_nevents];
    HGeomVector PrimVertexMod, VertexSum;

    PrimVertexMod.setXYZ(-100000.0, -100000.0, -100000.0);

    int num_TarTracks = 0;                    // number of tracks in target region
    int TarTrack[run_nevents];

    for (int l = 0; l < run_nevents; ++l)        TarTrack[l] = -100;  // default value

    // intersection of beam vertex (average for teh day) and primary vector
    HGeomVector SingleVertex = calcVertexAnalytical(beamVector, dirBEAM, DecayVertex, dirMother);

    // if intersection is in reasonable area
    if (IntersectionInTarget(beamVector, SingleVertex))
    {
        // Track loop
        for(int i = 0; i < run_nevents; ++i)
        {
            if (i != trackA_num && i != trackB_num) // exclude two tracks belonging to analysed track
            {
                trackArray[i] = event[i];

                CalcSegVector(trackArray[i].Z(),
                              trackArray[i].Rho(),
                              trackArray[i].Phi(),
                              trackArray[i].Theta(),
                              baseArray[i], dirArray[i]);

                HGeomVector SingleVertex = calcVertexAnalytical(beamVector, dirBEAM, baseArray[i], dirArray[i]);

                if (IntersectionInTarget(beamVector, SingleVertex))
                {
                    TarTrack[num_TarTracks++] = i;
                }
            }
        }
        TarTrack[num_TarTracks] = trackA_num;                          // save mother track in in position of trackA (pi+-)
        baseArray[trackA_num] = DecayVertex;
        dirArray[trackA_num] = dirMother;
        ++num_TarTracks;

        pv_type = num_TarTracks;

        if (num_TarTracks < 2)
        {
            PrimVertexMod = calcVertexAnalytical(beamVector, dirBEAM, baseArray[TarTrack[0]], dirArray[TarTrack[0]]);
        }
// std::cout << num_TarTracks << std::endl;
        if (num_TarTracks > 1)
        {
            int i_singVertex = 0;

            VertexSum.setXYZ(0.0, 0.0, 0.0);

            //cout <<"Start*********************************************************"<<endl;
            //cout << num_TarTracks <<endl;
            for(int l = 0; l < num_TarTracks; ++l)    // Calculate intersection points
            {
                for(int k = l; k < num_TarTracks; ++k)
                {
                    // Vertex near target
                    if ( TarTrack[l] != -100 && TarTrack[k] != -100 && TarTrack[l] != TarTrack[k] )
                    {
                        HGeomVector v = calcVertexAnalytical(baseArray[TarTrack[l]], dirArray[TarTrack[l]], baseArray[TarTrack[k]], dirArray[TarTrack[k]]);
                        if (v > -1000)
                        {
                            VertexSum += v;
                            //cout << calcVertexAnalytical(baseArray[TarTrack[l]],dirArray[TarTrack[l]],baseArray[TarTrack[k]],dirArray[TarTrack[k]]) <<endl;
                            i_singVertex++;
                        }
                    }
                }
            }
            if (i_singVertex > 0)
            {
                PrimVertexMod = VertexSum;
                PrimVertexMod /= i_singVertex;
            }
        }
    }

    return PrimVertexMod;
}

double g3(double * x, double * par)
{
    double r1 = double( (x[0]-par[1])/par[2]);
    double r2 = double( (x[1]-par[3])/par[4]);
    double r3 = double( (x[2]-par[5])/par[6]);
    
    return par[0]*TMath::Exp(-0.5 * (r1*r1 + r2*r2 + r3*r3));
}

const int npar = 7;
double fparams[npar] = { 3, 0, 3, 0, 3, -40, 3 };

TF3 * f3 = new TF3("f3", g3, -10, 10, -10, 10, -90, 10, npar);
TH3F * h3 = new TH3F("h3", "h3", 100, -10, 10, 100, -10, 10, 100, -90, 10);

hgvpair calcPrimVertex_ALL(const EventTracks & event, const HGeomVector & beam, const HGeomVector & vertex, const HGeomVector & mother, int trA_num, int trB_num, bool scaling, bool mother_as_ref)
{
    size_t run_nevents = event.size();

    TLorentzVector trackArray[run_nevents];
    HGeomVector baseArray[run_nevents];
    HGeomVector dirArray[run_nevents];
    HGeomVector PrimVertexMod, VertexSum, vfit;

    h3->Reset();

    PrimVertexMod.setXYZ(-100000.0, -100000.0, -100000.0);
    vfit.setXYZ(-100000.0, -100000.0, -100000.0);

    int num_TarTracks = 0;                    // number of tracks in target region
    int TarTrack[run_nevents];

    for (int l = 0; l < run_nevents; ++l)        TarTrack[l] = -100;  // default value

    HGeomVector SingleVertex = calcVertexAnalytical(beam, dirBEAM, vertex, mother);

    if (IntersectionInTarget(beam, SingleVertex))
    {
        // Track loop
        for(int i = 0; i < run_nevents; ++i)
        {
            if (i != trA_num && i != trB_num) // exclude two tracks belonging to analysed track
            {
                trackArray[i] = event[i];

                CalcSegVector(trackArray[i].Z(),
                              trackArray[i].Rho(),
                              trackArray[i].Phi(),
                              trackArray[i].Theta(),
                              baseArray[i], dirArray[i]);

                HGeomVector SingleVertex = calcVertexAnalytical(beam, dirBEAM, baseArray[i], dirArray[i]);

                if (IntersectionInTarget(beam, SingleVertex))
                {
                    TarTrack[num_TarTracks++] = i;
                }
            }
        }
        TarTrack[num_TarTracks] = trA_num;                          // save mother track in in position of trackA (pi+-)
        baseArray[trA_num] = vertex;
        dirArray[trA_num] = mother;
        ++num_TarTracks;

        if (num_TarTracks < 2)
        {
            vfit = PrimVertexMod = calcVertexAnalytical(beam, dirBEAM, baseArray[TarTrack[0]], dirArray[TarTrack[0]]);
        }
        else
        {
            int i_singVertex = 0;
            float i_scaling = 1.0;

            VertexSum.setXYZ(0.0, 0.0, 0.0);

            //cout <<"Start*********************************************************"<<endl;
            //cout << num_TarTracks <<endl;
            for(int l = 0; l < num_TarTracks; ++l)    // Calculate intersection points
            {
                for(int k = l; k < num_TarTracks; ++k)
                {
                    // Vertex near target
                    if ( TarTrack[l] != -100 && TarTrack[k] != -100 && TarTrack[l] != TarTrack[k] )
                    {
                        HGeomVector v = calcVertexAnalytical(baseArray[TarTrack[l]], dirArray[TarTrack[l]], baseArray[TarTrack[k]], dirArray[TarTrack[k]]);
                        if (v > -1000)
                        {
//                             if (scaling)
//                                 i_scaling = KTifini::calculateMinimumDistance(baseArray[TarTrack[l]], dirArray[TarTrack[l]], baseArray[TarTrack[k]], dirArray[TarTrack[k]]);
                            h3->Fill(v.X(), v.Y(), v.Z());

                            VertexSum += v;//*i_scaling;
                            //cout << calcVertexAnalytical(baseArray[TarTrack[l]],dirArray[TarTrack[l]],baseArray[TarTrack[k]],dirArray[TarTrack[k]]) <<endl;
                            i_singVertex++;
                        }
                    }
                }
            }
            if (i_singVertex > 0)
            {
                PrimVertexMod = VertexSum;
                PrimVertexMod /= i_singVertex;

                fparams[0] = i_singVertex/3 + 1;
                fparams[1] = PrimVertexMod.X();
                fparams[3] = PrimVertexMod.Y();
                fparams[5] = PrimVertexMod.Z();

                f3->SetParameters(fparams);
                h3->Fit(f3, "Q0N", "");

                vfit.setXYZ(f3->GetParameter(1), f3->GetParameter(3), f3->GetParameter(5));
            }

        }
    }

    return { PrimVertexMod, vfit };
}

hgvpair calcPrimVertex_Spec(const EventTracks event, const HGeomVector & beam, const HGeomVector & vertex, const HGeomVector & mother, int trA_num, int trB_num, bool scaling, bool mother_as_ref)
{
    size_t run_nevents = event.size();

    TLorentzVector trackArray[run_nevents];
    HGeomVector baseArray[run_nevents];
    HGeomVector dirArray[run_nevents];
    HGeomVector PrimVertexMod, VertexSum, vfit;

    double fparams[npar] = { 3, 0, 3, 0, 3, -40, 3 };

    h3->Reset();

    PrimVertexMod.setXYZ(-100000.0, -100000.0, -100000.0);
    vfit.setXYZ(-100000.0, -100000.0, -100000.0);

    int num_TarTracks = 0;                    // number of tracks in target region
    int TarTrack[run_nevents];

    for (int l = 0; l < run_nevents; ++l)        TarTrack[l] = -100;  // default value

    HGeomVector SingleVertex = calcVertexAnalytical(beam, dirBEAM, vertex, mother);

    if (IntersectionInTarget(beam, SingleVertex))
    {
        // Track loop
        for(int i = 0; i < run_nevents; ++i)
        {
            if (i != trA_num && i != trB_num) // exclude two tracks belonging to analysed track
            {
                trackArray[i] = event[i];

                CalcSegVector(trackArray[i].Z(), trackArray[i].Rho(),
                              trackArray[i].Phi(), trackArray[i].Theta(),
                              baseArray[i], dirArray[i]);

                HGeomVector SingleVertex = calcVertexAnalytical(beam, dirBEAM, baseArray[i], dirArray[i]);

                if (IntersectionInTarget(beam, SingleVertex))
                {
                    TarTrack[num_TarTracks++] = i;
                }
            }
        }
        TarTrack[num_TarTracks] = trA_num;                          // save mother track in in position of trackA (pi+-)
        baseArray[trA_num] = vertex;
        dirArray[trA_num] = mother;
        ++num_TarTracks;

        if (num_TarTracks < 2)
        {
            vfit = PrimVertexMod = calcVertexAnalytical(beam, dirBEAM, baseArray[TarTrack[0]], dirArray[TarTrack[0]]);
        }
        else
        {
            int i_singVertex = 0;
            float i_scaling = 1.0;

            VertexSum.setXYZ(0.0, 0.0, 0.0);

            //cout <<"Start*********************************************************"<<endl;
            //cout << num_TarTracks <<endl;
            for(int l = 0; l < (num_TarTracks-1); ++l)    // Calculate intersection points
            {
                if ( TarTrack[l] != -100 )
                {
                    HGeomVector v = calcVertexAnalytical(baseArray[TarTrack[l]], dirArray[TarTrack[l]], baseArray[TarTrack[num_TarTracks-1]], dirArray[TarTrack[num_TarTracks-1]]);
                    if (v > -1000)
                    {
//                         if (scaling)
//                             i_scaling = KTifini::calculateMinimumDistance(baseArray[TarTrack[l]], dirArray[TarTrack[l]], baseArray[TarTrack[k]], dirArray[TarTrack[k]]);

                        h3->Fill(v.X(), v.Y(), v.Z());
                        VertexSum += v;//*i_scaling;
                        //cout << calcVertexAnalytical(baseArray[TarTrack[l]],dirArray[TarTrack[l]],baseArray[TarTrack[k]],dirArray[TarTrack[k]]) <<endl;
                        i_singVertex++;
                    }
                }
            }
            if (i_singVertex > 0)
            {
                PrimVertexMod = VertexSum;
                PrimVertexMod /= i_singVertex;

                fparams[0] = i_singVertex/3 + 1;
                fparams[1] = PrimVertexMod.X();
                fparams[3] = PrimVertexMod.Y();
                fparams[5] = PrimVertexMod.Z();

                f3->SetParameters(fparams);
                h3->Fit(f3, "Q0N", "");

                vfit.setXYZ(f3->GetParameter(1), f3->GetParameter(3), f3->GetParameter(5));

//                 printf("orig: %f %f %f\n", PrimVertexMod.X(), PrimVertexMod.Y(), PrimVertexMod.Z());
//                 printf("fit:  %f %f %f\n", f3->GetParameter(1), f3->GetParameter(3), f3->GetParameter(5));

            }

//             printf("orig: %f %f %f\n", PrimVertexMod.X(), PrimVertexMod.Y(), PrimVertexMod.Z());
//             printf("fit:  %f %f %f\n", f3->GetParameter(1), f3->GetParameter(3), f3->GetParameter(5));
        }
    }

    return { PrimVertexMod, vfit };
}

bool IntersectionInTarget(const HGeomVector & beamVector, const HGeomVector & vertex)
{
//     HGeomVector cv = vertex - beamVector;
    float dist2 = pow(vertex.getX() - beamVector.getX(), 2.0) + pow(vertex.getY() - beamVector.getY(), 2.0);

    return    dist2 < TARGET_R_MAX*TARGET_R_MAX &&
            vertex.getZ() < TARGET_Z_MAX &&
            vertex.getZ() > TARGET_Z_MIN;
}

void CalculateEventVertexSL(double phi, double r, double z, HGeomVector * event_vertex, Bool_t reset) {
    // Simon Lang 2007
    // It seems that the XYZ calculatin is wrong (A. Schmah 2008)
    // In addition the vertex calculation is relativ to a not shifted beam axis (A. Schmah 2008)
    static double num_used_tracks = 0;
    static HGeomVector mean_vertex;
    HGeomVector vertex;
    double dist2;

    if (event_vertex != NULL) {
        // return the event vertex
        if (num_used_tracks)
            mean_vertex *= 1.0 / num_used_tracks;
        else
            mean_vertex.setXYZ( 999999999, 999999999, 999999999 );
        *event_vertex = mean_vertex;
        std::cout << "Test A" << std::endl;
    }
    if (reset) {
        // reset for next event
        num_used_tracks = 0;
        mean_vertex.setXYZ( 0, 0, 0 );
        std::cout << "RESET" << std::endl;
        return;
    }

    /* at z:              Y
    // r         track    |
    // perp.         \    |
    // to             \   |
    // track          /\  |
    //              phi \ |
    //          _ _/_ _ _\| _ _ _
    //                    |\
    //                    | \
    //                    |  \
    //               _____|__/\
    //           phi+90   | /  \
    //             /      |/r   \
    //  X---------/-------+      \
    */
    vertex.setXYZ( r * TMath::Cos(phi + TMath::PiOver2()), r * TMath::Sin(phi + TMath::PiOver2()), z);
    dist2    = (vertex.X() - TARGET_X_MEAN) * (vertex.X() - TARGET_X_MEAN)
            + (vertex.Y() - TARGET_Y_MEAN) * (vertex.Y() - TARGET_Y_MEAN);

    std::cout << "num_used_tracks = " << num_used_tracks << std::endl;
    // calculate the mean vertex from tracks which come from the target
    if (vertex.Z() > TARGET_Z_MIN  &&  vertex.Z() < TARGET_Z_MAX  && dist2 < VERTEX_MIN_DIST*VERTEX_MIN_DIST) {
        mean_vertex += vertex;
        ++num_used_tracks;
    }
}

double fDeltaTofMean(double MaxRMS ,int Counter, double DeltaTofA, double SystemA, double DeltaTofB, double SystemB, double DeltaTofC, double SystemC, double DeltaTofD, double SystemD) {
    //new function for second correction of the time of flight by Johannes Siebenson
    //cout << "StartStartStart" << endl;
    Counter++;

    double TofCounter= 0.0;
    int MetaHits= 0;                                            // Number of particles ith a meta hit
    double DeltaTofMean=0.0;
    double RMS=0.0;
    double DeltaTof[4];
    double System[4];

    DeltaTof[0] = DeltaTofA;
    DeltaTof[1] = DeltaTofB;
    DeltaTof[2] = DeltaTofC;
    DeltaTof[3] = DeltaTofD;

    System[0] = SystemA;
    System[1] = SystemB;
    System[2] = SystemC;
    System[3] = SystemD;

    /*//test if one can recunstruct the Kaon Mass with Kaon information
    if((System[0]!=-1 && System[2]!=-1)||
    (System[0]!=-1 && System[3]!=-1)||
    (System[2]!=-1 && System[3]!=-1)
    )
    {
        System[1]= SystemB;
    }
    else System[1]= -10;*/

    double DeltaDeltaTofMax=0.0;
    double CandidatesArray[4]= {0.0,0.0,0.0,0.0};
    //cout << "STAAAAAAAAAAAAAAAAAAAAAAAAAAAART" << endl;
    for(int i=0; i<4; i++) {                                      //calculate mean value
        if(System[i]==0 && DeltaTof[i]>-1000.0) {
            DeltaTofMean += DeltaTof[i];
            TofCounter = TofCounter+1;             //weight Tofino with 1 (450 ps resolution)
            MetaHits +=1;
        }
        if(System[i]>0 && DeltaTof[i]>-1000.0) {
            DeltaTofMean += 2.5*DeltaTof[i];
            TofCounter = TofCounter+2.5;           //weight Tof with 2.5 (180 ps resolution)
            MetaHits +=1;
        }
    }
    if(TofCounter>0) {                                              //calculate mean value
        DeltaTofMean = DeltaTofMean/((float)TofCounter);
    } else {
        DeltaTofMean = -1000.0;
        return DeltaTofMean;
    }

    for(int i=0; i<4; i++)                                      //calculate root mean square
    {
    if(System[i]==0 && DeltaTof[i]>-1000.0)
        {
            RMS += pow(DeltaTof[i] - DeltaTofMean,2);            //weight Tofino with 1 (450 ps resolution)
        }
        if(System[i]>0 && DeltaTof[i]>-1000.0)
        {
            RMS += 2.5*pow(DeltaTof[i] - DeltaTofMean,2);          //weight Tof with 2.5 (180 ps resolution)
        }

    }
    if(TofCounter>0)                                              //calculate mean value
    {
        RMS = sqrt(RMS/((float)TofCounter));
    }
    if(DeltaTofMean>100)// && MaxRMS < 0.0000000002901)
    {
    //cout << "STAAAAAAAAAAAAAAAAAAAAAAAAAAAART" << endl;
    //cout<<"Event"
    cout << "DeltaTofMean :"<< DeltaTofMean << endl;
    cout << "MxRMS :"<< MaxRMS << endl;
    cout << "RMS   :"<< RMS << endl;
    cout << "MetaHits :"<< MetaHits << endl;
    cout << "TofCounter :"<< TofCounter << endl;
    cout << "DeltaTof0 :"<<  DeltaTof[0]<< endl;
    cout << "DeltaTof1 :"<<  DeltaTof[1]<< endl;
    cout << "DeltaTof2 :"<<  DeltaTof[2]<< endl;
    cout << "DeltaTof3 :"<<  DeltaTof[3]<< endl;
    cout << "System0 :"<<  System[0] << endl;
    cout << "System1 :"<<  System[1] << endl;
    cout << "System2 :"<<  System[2] << endl;
    cout << "System3 :"<<  System[3] << endl;
    cout << "Loop counter :"<< Counter <<endl;
    }
        if(fabs(RMS)>fabs(MaxRMS) && MetaHits>2)                           // MetaHits should always be larger than 2 for truncated mean
    {
        for(int i=0; i<4; i++)
        {
            if(fabs(DeltaTof[i] - DeltaTofMean)>(fabs(RMS)) && DeltaTof[i]>-1000.0 && MetaHits>2 )
            {

                if(fabs(DeltaTof[i] - DeltaTofMean)>DeltaDeltaTofMax)                 // Throw out the largest difference
                {
                    DeltaDeltaTofMax = fabs(DeltaTof[i] - DeltaTofMean);
                    for(int j=0; j<4; j++)
                    {
                        CandidatesArray[j]=0.0;
                    }
                    CandidatesArray[i]=1.0;
                }
            }

        }
        for(int i=0; i<4; i++)
        {
            if(CandidatesArray[i]==1.0)
            {
                DeltaTof[i]=-1000.0;
                System[i]=-10.0;
            }
        }
        DeltaTofMean = fDeltaTofMean(MaxRMS,Counter,DeltaTof[0],System[0],DeltaTof[1],System[1],DeltaTof[2],System[2],
                                    DeltaTof[3],System[3]);
    }
    return DeltaTofMean;
}

double calcRapidity(double MomTot, double Momz, double Mass) {
    double Eges = sqrt(pow(MomTot,2)+pow(Mass,2));
    double Rap = 0.5*log((Eges+Momz)/(Eges-Momz));
    return Rap;
}


double calcCOS_of_OpeningAngle(TLorentzVector& v1, TLorentzVector& v2) {
//calculate the cos(Angle) between two Vectors
    double cos_Angle=-10;

    if ((v1.P()* v2.P())>0){cos_Angle = (v1.Px()*v2.Px()+v1.Py()*v2.Py()+v1.Pz()*v2.Pz())/( v1.P()* v2.P());}
    //else

    return (cos_Angle);
}

double fBeta(double Mass, double Momentum) {
    // calculates velocity beta as a function of mass and momentum
    double Beta   = 0.0;
    double poverm = 0.0;
    if (Mass > 0.0) {
        poverm = Momentum/Mass;
        Beta   = poverm*1.0/(sqrt(poverm*poverm+1.0));
    }
    return Beta;
}

double fTof(double Beta, double PathLength) {
    // calculates timo-of-flight as af function of velocity beta and path length
    double Tof = 0.0;
    double clight = 299792458.0; // [m/s]
    if (Beta > 0.0) {
        Tof = PathLength/(Beta*clight);
    }
    return Tof;
}

double fDeltaTof(double Mass, double PathLength, double Momentum, double Tof) {
    if (Mass > 0.0 && PathLength > 0.0) {
        double Beta     = fBeta(Mass,Momentum);
        double Tof_new  = fTof(Beta,PathLength);
            double DeltaTof = Tof_new - Tof;
            //std::cout << "FDT: Beta = " << Beta << ", Tof_new = " << Tof_new << ", Tof_old = " << Tof << std::endl;
        return DeltaTof;
    }
    else return -1000.0;
}

double EnergyLossFitMassFunc2(double * x_val, double * par) {
    // Calculates the dEdx (MeV cm2/g ) of an particle with GEANT ID id
    // and momentum p (MeV) for He/i-butan gas mixture with He fraction hefr
    // (He (hefr) / i-butan (1-hefr)). The fomular is taken from PDG and doesn't
    // include the density correction term. The values for the mean excitation
    // energy are taken from Sauli.

    // TOFINO
    double p, hefr, par1, par2, par3, par4, par5, par6, par7, par8, par9, par10, par11;
    p = x_val[0];
    hefr = par[0];
    par1 = par[1];
    par2 = par[2];
    par3 = par[3];
    par4 = par[4];
    par5 = par[5];
    par6 = par[6];
    par7 = par[7];
    par8 = par[8];
    par9 = par[9];
    par10 =par[10];
    par11 =par[11];
    //int id = (int)par8;

    if (p==0)             return -1;
    if (hefr<0.||hefr>1.) return -1;
    double mass   = par7*par8;
    if (mass==0) return -1;

    // Z and A
    //double Z_gas  =2.*hefr+(1.-hefr)*34.;
    //double A_gas  =4.*hefr+(1.-hefr)*58.;
    double Z_gas  =par3*hefr+(1.-hefr)*par1; // 14
    double A_gas  =par4*hefr+(1.-hefr)*par2; // 58
    // I_0 and I
    //double I_0_gas=24.6*hefr+(1.-hefr)*10.8;
    double I_0_gas=par5*hefr+(1.-hefr)*par6;
    double I2     =pow(I_0_gas*Z_gas*(1.e-6),2); // sauli
    //double I2     =pow(16.*pow(Z_gas,0.9),2); //C.Lippmann thesis

    double K      =0.307075; // MeV cm2 PDG, 4*pi*N_{A}*r_{e}2*m_{e}2*c2
    double mass2  =pow(mass,2);
    double m_ec2  =HPhysicsConstants::mass(3);
    double z2     =pow((double)HPhysicsConstants::charge(14),2);
    double p2     =pow(p,2);
    double beta2  =1./((mass2/p2)+1.);
    double gamma2 =1./(1-beta2);
    double gamma  =sqrt(gamma2);

    double Tmax   =(2.*m_ec2*beta2*gamma2)/(1.+ 2.*gamma*(m_ec2/mass)+pow((m_ec2/mass),2));
    double term1  =K*z2*(Z_gas/A_gas)*(1./beta2) + par11;
    double term2  =((2.*m_ec2*beta2*gamma2*Tmax)/I2);
    double dedx   = par9 * term1 * (0.5*log(term2)-beta2) + par10;

    return dedx;
}

double EnergyLossFitFunc2(double * x_val, double * par) {
    // Calculates the dEdx (MeV cm2/g ) of an particle with GEANT ID id
    // and momentum p (MeV) for He/i-butan gas mixture with He fraction hefr
    // (He (hefr) / i-butan (1-hefr)). The fomular is taken from PDG and doesn't
    // include the density correction term. The values for the mean excitation
    // energy are taken from Sauli.

    double p,  hefr, par1, par2, par3, par4, par5, par6, par7, par8, par9, par10, par11;
    p = x_val[0];
    hefr = par[0];
    par1 = par[1];
    par2 = par[2];
    par3 = par[3];
    par4 = par[4];
    par5 = par[5];
    par6 = par[6];
    par7 = par[7];
    par8 = par[8];
    par9 = par[9];
    par10 = par[10];
    par11 = par[11];
    int id = (int)par8;

    if (p==0)             return -1;
    if (hefr<0.||hefr>1.) return -1;
    double mass   = par7 * HPhysicsConstants::mass(id);
    if (mass==0) return -1;

    // Z and A
    double Z_gas  =par3*hefr+(1.-hefr)*par1; // 14
    double A_gas  =par4*hefr+(1.-hefr)*par2; // 58

    // I_0 and I
    double I_0_gas=par5*hefr+(1.-hefr)*par6;
    double I2     =pow(I_0_gas*Z_gas*(1.e-6),2); // sauli

    double K      =0.307075; // MeV cm2 PDG, 4*pi*N_{A}*r_{e}2*m_{e}2*c2
    double mass2  =pow(mass,2);
    double m_ec2  =HPhysicsConstants::mass(3);
    double z2     =pow((double)HPhysicsConstants::charge(id),2);
    double p2     =pow(p,2);
    double beta2  =1./((mass2/p2)+1.);
    double gamma2 =1./(1-beta2);
    double gamma  =sqrt(gamma2);

    double Tmax   =(2.*m_ec2*beta2*gamma2)/(1.+ 2.*gamma*(m_ec2/mass)+pow((m_ec2/mass),2));
    double term1  =K*z2*(Z_gas/A_gas)*(1./beta2) + par11;
    double term2  =((2.*m_ec2*beta2*gamma2*Tmax)/I2);
    double dedx   = par9 * term1 * (0.5*log(term2)-beta2) + par10;

    return dedx;
}

//     double Ekin_beam, Ekin_target;
//     double CM_rap;
// 
//     TLorentzVector vec_beam;
//     TLorentzVector vec_target;
//     TLorentzVector vec_sum;

//     double E_beam = T_beam + HPhysicsConstants::mass(PID_beam);
//     double E_target = T_target + HPhysicsConstants::mass(PID_target);
//     double pz_beam = sqrt(E_beam*E_beam - HPhysicsConstants::mass(PID_beam)*HPhysicsConstants::mass(PID_beam));
//     vec_beam = TLorentzVector(0, 0, pz_beam, E_beam);
//     vec_target = TLorentzVector(0, 0, 0, E_target);
//     vec_sum = vec_beam + vec_target;
// 
//     CM_rap = vec_sum.Rapidity();

//     std::cout << "Center of momentum rapidity = " << CM_rap << std::endl;

}
