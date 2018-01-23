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


#include "KFitTools.h"
#include "TLine.h"

#include <cmath>

KFitTools::KFitTools() :
    steps(0), n_div_x(0), n_div_y(0),Smooth_option(0),  start(0), stop(0), start_fit(0), stop_fit(20), sigma_fac(1), hName("")
{

}

KFitTools::~KFitTools() {
    std::cout << "deleting objects" << std::endl;
    delete Canvas;
    delete Canvas2;
    delete hist;
}

void KFitTools::SetName(TString ihName)
{
    hName = ihName;
}
void KFitTools::SetNSteps(Int_t isteps, Float_t istart, Float_t istop)
{
    steps   = isteps;
    start   = istart;
    stop    = istop;
}
void KFitTools::SetNDiv(Int_t in_div_x, Int_t in_div_y)
{
    n_div_x = in_div_x;
    n_div_y = in_div_y;
}
TCanvas* KFitTools::getCanvas() const
{
    return Canvas;
}
TCanvas* KFitTools::getCanvas2() const
{
    return Canvas2;
}
void KFitTools::SetHist(TH2F* h)
{
    hist = (TH2F*)h->Clone("hist");
}
void KFitTools::SetFitRange(Float_t istart_fit, Float_t istop_fit)
{
    start_fit = istart_fit;
    stop_fit  = istop_fit;
}
void KFitTools::SetFitFunction(TF1* iFitfunc)
{
    Fitfunc = iFitfunc;
    Fitfunc->SetName("Fitfunc");
}

void KFitTools::SetSigmaFac(Float_t isigma_fac)
{
    sigma_fac = isigma_fac;
}

void KFitTools::Smooth_Projection(Bool_t iSmooth_option)
{
    Smooth_option = iSmooth_option ;
}

void KFitTools::ProjectHist()
{
    std::cout << "Project and fit: " << hName.Data() << std::endl;
    TString hist_pos_name   = hName + "_pos";
    TString hist_width_name = hName + "_width";
    TString canvas_name     = hName + "_can";
    Canvas      = new TCanvas(canvas_name.Data(),canvas_name.Data(),5,5,500,500);
    Canvas      ->Divide(n_div_x, n_div_y);

    canvas_name     = hName + "_can2";
    Canvas2      = new TCanvas(canvas_name.Data(),canvas_name.Data(),5,5,500,500);
    Canvas2      ->Divide(2,2);

    TString HistName;
    TLine* Peak_line;
    Peak_line =  new TLine(); // Shows the fit-maximum
    Peak_line -> SetLineWidth(1);
    Peak_line -> SetLineStyle(5);
    Proj_pos   = hist->ProjectionX("Proj_pos"); // Position histogram
    Proj_width = hist->ProjectionX("Proj_width"); // Width histogram
    Proj_pos   ->Reset();
    Proj_width ->Reset();
    Proj_pos   ->SetName(hist_pos_name.Data());
    Proj_width ->SetName(hist_width_name.Data());
    Float_t delta_step = (stop-start)/(Float_t)steps; // Step width
    for(Int_t i = 0; i < steps; i++)  // Loop over all x-slices
    {
        HistName = hName;
        HistName += "ProjY_";
        HistName += i;

        if(i>9 && i<14)
        {

        }

        Canvas ->cd(i+1);
        Canvas ->cd(i+1)->SetTopMargin(0.05);
        Canvas ->cd(i+1)->SetBottomMargin(0.05);
        Canvas ->cd(i+1)->SetLeftMargin(0.05);
        Canvas ->cd(i+1)->SetRightMargin(0.05);



        Int_t bin_num = hist->GetXaxis()->FindBin(start+i*delta_step);
        ProjY[i]      = hist->ProjectionY(HistName.Data(),bin_num,bin_num+1);

        //smooth option
        if(Smooth_option == 1)
        {
            Double_t bin = 0;
            Double_t neighbour_bin = 0;

            for(Int_t j=0;j<ProjY[i]->GetNbinsX();j++)
            {
             bin           = ProjY[i]->GetBinContent(j);
             neighbour_bin = ProjY[i]->GetBinContent(j+1);
             //std::cout<<"Binnumber ="<<j<<",bin ="<<bin<<", neighbour_bin ="<<neighbour_bin<<std::endl;
             //set the bin conten which is to high to the neighbour bincontent if the neighbour is not 0
             if(neighbour_bin!=0 && bin>1.5*neighbour_bin)
             {
                 ProjY[i]->SetBinContent(j,neighbour_bin);
                 //std::cout<<"Bin number = "<<j<<" was smoothed jeah"<<std::endl;
             }
             }
        }

        //this is necessary to smoothen the Histogram
        //if you comment this part your moved histogram will
        //have ugly effects due to binning (at least I guess this is the reason)
        //
        /*if(Smooth_option==1)
        {
            Int_t Bins_of_Projections = ProjY[i]->GetNbinsX();
            for(Int_t Proj_Bin=0;Proj_Bin<Bins_of_Projections;Proj_Bin++)
            {
                Double_t BinVal1=0;
                Double_t BinVal2=0;
                Double_t BinVal3=0;

                BinVal1=ProjY[i]->FindBin(Proj_Bin);
                if(Proj_Bin<Bins_of_Projections-1)BinVal2=ProjY[i]->FindBin(Proj_Bin+1);
                if(Proj_Bin>0)                    BinVal3=ProjY[i]->FindBin(Proj_Bin-1);

                if(BinVal2>1.5*BinVal3 && BinVal3>0)
                {
                    BinVal2 = (BinVal1+BinVal3)/2;
                    ProjY[i]->SetBinContent(ProjY[i]->FindBin(Proj_Bin),BinVal2);
                }
            }
        }*/

        ProjY[i]         ->GetXaxis()->SetRangeUser(start_fit,stop_fit);
        ProjY[i]         ->SetStats(0);

        Int_t   max_bin   = ProjY[i] ->GetMaximumBin();
    Float_t max_pos   = ProjY[i] ->GetBinCenter(max_bin);   // Fit input -> start fit position
        Float_t max_entry = ProjY[i] ->GetBinContent(max_bin);  // Fit input -> start max value
    Float_t RMSval    = ProjY[i] ->GetRMS();                 // Fit input -> start width
        //std::cout << "step = " << i << ", max_bin = " << max_bin << ", max_pos = " << max_pos << ", RMS = " << RMSval << std::endl;
        Fitfunc ->SetParameter(0,max_pos);
    Fitfunc ->SetParameter(1,RMSval/2.0);
        Fitfunc ->SetParameter(2,max_entry);
        Fitfunc ->SetRange(max_pos-RMSval*0.75*sigma_fac,max_pos+1.5*RMSval*sigma_fac);
        ProjY[i]   ->Fit("Fitfunc","QN","",max_pos-RMSval*0.75*sigma_fac,max_pos+1.5*RMSval*sigma_fac);
        Float_t pos   = Fitfunc->GetParameter(0);
        Float_t width = Fitfunc->GetParameter(1);
        ////std::cout << "Fit1: pos = " << pos << ", width = " << width << std::endl;
        Fitfunc ->SetParameter(0,pos);
    Fitfunc ->SetParameter(1,width);
        Fitfunc ->SetParameter(2,max_entry);
        Fitfunc ->SetRange(max_pos-width*1.75*sigma_fac,max_pos+3.5*width*sigma_fac);
        //std::cout<<" - - - - - - - - - -"<<std::endl;

        ProjY[i]   ->Fit("Fitfunc","QN","",max_pos-(width*1.75*(sigma_fac+0.2*sigma_fac)),max_pos+(width*3.5*(sigma_fac-0.2*sigma_fac)));
        Float_t pos2   = Fitfunc->GetParameter(0);
        Float_t width2 = Fitfunc->GetParameter(1);

        ProjY[i]   ->Fit("Fitfunc","QN","",max_pos-(width*1.75*(sigma_fac-0.2*sigma_fac)),max_pos+(width*3.5*(sigma_fac+0.2*sigma_fac)));
        Float_t pos3   = Fitfunc->GetParameter(0);
        Float_t width3 = Fitfunc->GetParameter(1);

        ProjY[i]   ->Fit("Fitfunc","QN","",max_pos-(width*1.75*sigma_fac),max_pos+(width*3.5*sigma_fac));
        Float_t pos1   = Fitfunc->GetParameter(0);
        Float_t width1 = Fitfunc->GetParameter(1);

        pos= (pos1+pos2+pos3)/3;
        width=(width1+width2+width3)/3;


        Double_t error_width = fabs(width-width1);
        if(fabs(width-width2)>error_width)error_width=fabs(width-width2);
        if(fabs(width-width3)>error_width)error_width=fabs(width-width3);

        //std::cout << "Fit2: pos = " << pos << ", width = " << width << std::endl;
        //std::cout << "Fit2: pos ERR = " << Fitfunc->GetParError(0) << ", width ERR = " << error_width << std::endl;

        Fitfunc     ->SetLineColor(2);
        ProjY[i]    ->GetXaxis()->SetRangeUser(max_pos-(width*6),max_pos+(width*9));  //set here the range of your histogram in the canvas
        ProjY[i]    ->DrawCopy("");
        Fitfunc     ->DrawCopy("same");
        //Peak_line   ->DrawLine(pos,0.0,pos,max_entry);  // max_pos
        Peak_line   ->DrawLine(max_pos,0.0,max_pos,max_entry);
        Proj_pos    ->SetBinContent(bin_num,max_pos);
        //Proj_pos    ->SetBinContent(bin_num,pos);
        Proj_pos    ->SetBinError(bin_num,width);
        Proj_width  ->SetBinContent(bin_num,width);
        Proj_width  ->SetBinError(bin_num,error_width);


        if(i>6 && i<11)
        {                         
            Canvas2 ->cd(i-6);

            Canvas2 ->cd(i-6)->SetTopMargin(0.05);
            Canvas2 ->cd(i-6)->SetBottomMargin(0.1);
            Canvas2 ->cd(i-6)->SetLeftMargin(0.12);
            Canvas2 ->cd(i-6)->SetRightMargin(0.05);
            ProjY[i]->SetStats(0);
            ProjY[i]->SetTitle("");
            ProjY[i]->GetXaxis()->SetTitleSize(0.05);
            ProjY[i]->GetXaxis()->SetTitleOffset(1.1);
            ProjY[i]->GetYaxis()->SetTitleSize(0.05);
            ProjY[i]->GetYaxis()->SetTitleOffset(1.3);
            ProjY[i]->GetXaxis()->SetTitle("MDC dE/dx [arb. units] ");//~rec mass for rec Events~");
            ProjY[i]->GetYaxis()->SetTitle("counts");
            ProjY[i]->GetYaxis()->CenterTitle();
            ProjY[i]->GetXaxis()->CenterTitle();

            ProjY[i]    ->DrawCopy("");
            Fitfunc     ->DrawCopy("same");
        }

    }
}
TH1D* KFitTools::getProjPos()
{
    return Proj_pos;
}
TH1D* KFitTools::getProjWidth()
{
    return Proj_width;
}

