/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Rafal Lalik <rafal.lalik@ph.tum.de>

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


#ifndef KTIFINIANALYSIS_H
#define KTIFINIANALYSIS_H

#include <fstream>
#include <vector>
#include <iostream>

#include "TCanvas.h"
#include "TCutG.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TNtuple.h"
#include "Rtypes.h"
#include "TApplication.h"

#include "hloop.h"

const Int_t N_MDC_momdedx_spec = 7;
const Int_t N_TOF_momdedx_spec = 7;

#include "KTools.h"

#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";

namespace KT
{
    // Particle ID (compatible with Geant ID)
    enum ParticleID
    {
        PID_None = 0,   ///< dont use, no geant PID == 0
        gamma = 1,      ///< gamma
        elp = 2,        ///< positron
        elm = 3,        ///< electron
        mup = 5,        ///< positive muon
        mum = 6,        ///< negative muon
        pip = 8,        ///< positive pion
        pim = 9,        ///< negative pion
        Kp = 11,        ///< positive Kaon
        Km = 12,        ///< negative Kaon
        p = 14,         ///< proton
        PID_Dummy       ///< don't use it, it's for limits calculation
    };

    // 
    enum MetaSystem {
        MS_NOMETA = 0x00,   ///<
        MDC = 0x01,
        TOF = 0x02,
        TOFINO = 0x04,
        RPC = 0x08,
        MS_Dummy
    };

    // particle cut ID
    enum CutID
    {
        cut_p = 0, cut_pip, cut_pim, cut_Kp, cut_Km, cut_el, cut_po, CID_Dummy
    };

    //
    enum KPidSelectionType {
        Charge = 0x01,      ///< select PID by particle charge
        Beta = 0x02,        ///< select PID by particle Beta
        Graphical = 0x04,   ///< select PID by graphical cuts
        PST_Dummy           ///< don't use it, it's for limits calculation
    };

    //
    enum AnalysisType { Exp = 0, Sim = 1 };

    //
    enum Experiment { pp45 };
}

class KAbstractAnalysis;

class KTifiniAnalysis
{
public:
    KTifiniAnalysis(int argc, char ** argv, KT::AnalysisType at, KAbstractAnalysis * ana);
    virtual ~KTifiniAnalysis();

    void exec();

    inline Int_t getEventsNumber() { return eventsnr; }

protected:
    int Configure(int argc, char ** argv);
    void Usage() const;

private:
    void exportCanvas();

    inline void initExperimentPhysics(KT::Experiment exp) { expName = exp; };

private:
    KT::AnalysisType analysisType;
    KT::Experiment expName;

    // general:
    HLoop * loop;                  // standard input
    EventTracks * event;

    KAbstractAnalysis * ana;

    // Configuration flags
    int argc_; char ** argv_;
    int flag_verbose;
    int flag_list;
    int eventsnr;
    TString filedir;
    TString file_out;
    TString outdir;
    TString suffix;
};

#endif // KTIFINIANALYSIS_H
