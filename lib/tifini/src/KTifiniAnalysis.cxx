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

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>

#include "KTifiniAnalysis.h"

#include "KAbstractAnalysis.h"
#include "KTools.h"
#include "KPlotsdEdx.h"
#include "KPlotsBeta.h"

#include "hcategory.h"
#include "hgeantkine.h"

#ifdef HYDRA1COMP
#include "TChain.h"
#include "heventheader.h"
#include "hpidevtinfo.h"
#include "HParticleCand.h"
#include "hpidtracksorter.h"
#else
#include "hcategorymanager.h"
#include "hparticleevtinfo.h"
#include "hparticletracksorter.h"
#endif

#ifdef HYDRA1COMP
static Bool_t selectHadronsQa(HPidTrackCand * pcand)
{
    Bool_t test = kFALSE;
    if (pcand->isFlagAND(4,
            HPidTrackCand::kIsAcceptedHitInnerMDC,
            HPidTrackCand::kIsAcceptedHitOuterMDC,
            HPidTrackCand::kIsAcceptedHitMETA,
            HPidTrackCand::kIsAcceptedRK
    )
       &&
       pcand->getTrackData()->getRKChiSquare()     < 1000000
    )
        test = kTRUE;

    return test;
}
#else
static Bool_t selectHadronsQa(HParticleCand * pcand)
{
    Bool_t test = kFALSE;
    if (pcand->isFlagAND(3,                                  //Particle::kIsAcceptedHitRICH,
        Particle::kIsAcceptedHitInnerMDC,
        Particle::kIsAcceptedHitOuterMDC,
        Particle::kIsAcceptedHitMETA,
        Particle::kIsAcceptedRK
        ) &&
        pcand->getInnerSegmentChi2() > 0
        &&                                                   //pcand->getBeta>0.9 && pcand->getBeta<1.1
        pcand->getChi2()             < 1000      // RK       //MetaQA<4
    )
        test = kTRUE;

    return test;
}
#endif

KTifiniAnalysis::KTifiniAnalysis(int argc, char** argv, KT::AnalysisType at, KAbstractAnalysis * ana) :
    analysisType(at), expName(KT::pp45),
    ana(ana),
    flag_verbose(0), flag_list(0),
    eventsnr(100000)
{
#ifdef HYDRA1COMP
    myHades = new Hades;
    gHades->setQuietMode(2);
    source = new HRootSource();
#else
    loop = new HLoop(kTRUE);
#endif

    int optsleft = Configure(argc, argv);
    argc_ = argc - optsleft + 1;
    argv_ = argv + optsleft - 1;

    if (ana)
        optsleft = ana->Configure(argc_, argv_);

    ana->setVerboseFlag(flag_verbose);
}

KTifiniAnalysis::~KTifiniAnalysis()
{
    std::cout << "++= Tifnini finished it's job! +++" << std::endl;
}

void KTifiniAnalysis::exportCanvas()
{
    ana->getCuts().drawCuts();
}

void KTifiniAnalysis::exec()
{
    if (!ana)
    {
        std::cerr << "No analysis registered! Exiting..." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "******************************" << std::endl;
    if (analysisType == KT::Exp)        std::cout << "Parameters for experiment data are loaded" << std::endl;
    if (analysisType == KT::Sim)        std::cout << "Parameters for simulated data are loaded" << std::endl;
    std::cout << "******************************" << std::endl;

    ana->initAnalysis(expName, analysisType);

    gStyle->SetPalette(1);
    gStyle->SetCanvasColor(10);

    TString outputfile_suffix;  // suffix for the output file

    if (analysisType == KT::Exp)            outputfile_suffix = "_exp_";
    if (analysisType == KT::Sim)            outputfile_suffix = "_sim_";

    outputfile_suffix += "_" + suffix;
    if (!suffix.IsNull())
        outputfile_suffix += "_" + suffix;
    else
        outputfile_suffix += "_" + TString(getenv("USER"));

    outputfile_suffix += ".root";

    if (!filedir.IsNull() and !filedir.EndsWith("/"))
        filedir = filedir + "/";

    TString filename_in = file_out;

    if (filename_in.EndsWith(".txt", TString::kIgnoreCase))
    {
        filename_in.Remove(file_out.Length() - 4, 4);
    }
    else
    if (filename_in.EndsWith(".lst", TString::kIgnoreCase))
    {
        filename_in.Remove(file_out.Length() - 4, 4);
    }
    else
    if (filename_in.EndsWith(".root", TString::kIgnoreCase))
    {
        filename_in.Remove(file_out.Length() - 5, 5);
    }

    if (!filename_in.BeginsWith("/"))
        filename_in = filedir + filename_in;

    file_out = filename_in + outputfile_suffix;

    if (!outdir.IsNull())
    {
        if (!outdir.EndsWith("/"))
            outdir = outdir + "/";

        Ssiz_t s = file_out.Last('/');
        if (s == -1)
            file_out = outdir + file_out;
        else
        {
            file_out = outdir + file_out(s, -1);
        }
    }

    // OUTPUT file is created
    std::cout << "Output file: " << file_out <<std::endl;
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HYDRA1COMP
    myHades->setDataSource(source);

    if(!myHades->init())
    {
        cout<<"Hades Init() failed!"<<endl;
        exit(1);
    }

    loop = source->getTree();
    Long64_t num_events       = loop->GetEntries();
#else
    if (!loop->setInput(""))
    {
        std::cerr << "READBACK: ERROR : cannot read input !" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    Long64_t num_events       = loop->getEntries();
#endif
    Long64_t first_event      = 0;

    Int_t nEvents = eventsnr;
    if (nEvents > num_events) nEvents = num_events;
    std::cout << "Number of Analyzed events   = " << nEvents << std::endl;

    // get hydra categories
    HEvent * fEvent = gHades->getCurrentEvent();
    if  (!fEvent) exit(1);

#ifdef HYDRA1COMP
    HCategory* fCatCand = (HCategory*)gHades->getCurrentEvent()->getCategory(catPidCandidate);
#else
    HCategory* fCatCand = (HCategory*)HCategoryManager::getCategory(catParticleCand, 1, "HParticleCandSim");
    HCategory* fFwDetCand = (HCategory*)HCategoryManager::getCategory(catFwDetCand, 1, "catFwDetCand");
    HCategory* catHParticleEvtInfo  = (HCategory*)HCategoryManager::getCategory(catParticleEvtInfo ,kTRUE,"catParticleEvtInfo");
    HIterator* iterParticleEvtInfo = (HIterator *)catHParticleEvtInfo->MakeIterator("native");

    if  (!fFwDetCand) exit(1);
#endif

    if  (!fCatCand) exit(1);

    // prepare output tree
    TFile * Outputfile = new TFile(file_out, "RECREATE");
    Outputfile->cd();
    
    std::cout << "******* Start of event loop ********" << std::endl;
    std::cout << "" << std::endl;

    ana->preAnalysis();

    KPlotsdEdx dEdx_plots("global_dEdx");
    KPlotsBeta beta_plots("global_beta");

    dEdx_plots.init(KT::MDC | KT::TOF | KT::TOFINO, "charge * momentum / q * MeV/c", "dEdx / [a.u.]",
                    KPlotsdEdx::mom_bins, KPlotsdEdx::mom_min, KPlotsdEdx::mom_max,
                    KPlotsdEdx::dEdx_bins, KPlotsdEdx::dEdx_min, KPlotsdEdx::dEdx_max);

    beta_plots.init(KT::RPC | KT::TOF | KT::TOFINO, "charge * momentum / q * MeV/c", "beta / c",
                    KPlotsBeta::mom_bins, KPlotsBeta::mom_min, KPlotsBeta::mom_max,
                    KPlotsBeta::beta_bins, KPlotsBeta::beta_min, KPlotsBeta::beta_max);

#ifdef HYDRA1COMP
    HPidTrackSorter sorter;
#else
    HParticleTrackSorter sorter;
#endif
    sorter.init();

    // loop over all events
    Int_t event_num = first_event;
    for (; event_num < nEvents; event_num++)
    {
#ifdef HYDRA1COMP
        if (!gHades->eventLoop(1,0))
            break;
#else
        if (loop->nextEvent(event_num) <= 0) 
            break;
#endif
//         if (loop->nextEvent(event_num) <= 0) { cout << " end reached " << endl; break; } // last event reached
// 
//         if (event_num != 0  &&  event_num % 50 == 0)
//             std::cout << "." << std::flush;
// 
//         if (event_num == 0)
//             std::cout << std::flush << "==> Processing data " << std::flush;
// 
//         if (event_num != 0  &&  event_num % 1000 == 0)
//         {
//             Float_t event_percent = 100.0*event_num/nEvents;
//             std::cout << " " << event_num << " (" << event_percent << "%) " << "\n" << "==> Processing data " << std::flush;
//         }

        ana->resetEvent();

#ifdef HYDRA1COMP
        // FIXME
#else
        HParticleEvtInfo* evtinfo = HCategoryManager::getObject(evtinfo, catParticleEvtInfo, 0);
        if (!ana->setEventInfo(evtinfo))
        {
            continue;
        }

        Int_t RpcMult       = evtinfo->getSumRpcMultHitCut();
        Int_t TofMult       = evtinfo->getSumTofMultCut();
        Int_t SelectedPart  = evtinfo->getSumSelectedParticleCandMult();
#endif

        sorter.cleanUp();
        sorter.resetFlags(kTRUE, kTRUE, kTRUE, kTRUE);  // reset all flags for flags (0-28), reject, used, lepton
        Int_t nCandHad = sorter.fill(selectHadronsQa);  // fill only good hadrons (already marked good leptons will be skipp)
        //Int_t nCandHad     = sorter.fill(selectHadronsQa_noMeta2);   // fill only good hadrons (already marked good leptons will be skipp$
//         Int_t nCandHad     = sorter.fill(HParticleTrackSorter::selectHadrons);   // fill only good hadrons (already marked good leptons will be skipp$
#ifdef HYDRA1COMP
        Int_t nCandHadBest = sorter.selectBest(HPidTrackSorter::kIsBestRK, HPidTrackSorter::kIsHadron);
#else
        Int_t nCandHadBest = sorter.selectBest(HParticleTrackSorter::kIsBestRK, HParticleTrackSorter::kIsHadron);
#endif
        Int_t cand_size = fCatCand->getEntries();
#ifndef HYDRA1COMP
        Int_t vect_size = fFwDetCand->getEntries(); // number of Emc hits in this event
#endif
        //#### HADES Tracks ####
        // loop over all tracks of the actual event
        for(Int_t i = 0; i < cand_size; ++i)
        {
#ifdef HYDRA1COMP
            HPidTrackCand * tcand = (HPidTrackCand *)fCatCand->getObject(i);
            HParticleCand _track = *tcand;
            HParticleCand * track = &_track;
#else
            HParticleCand * track = HCategoryManager::getObject(track, fCatCand, i);
#endif
            ana->setHadesTrackInfo(track, i);
            dEdx_plots.fill(track);
            beta_plots.fill(track);
        }

#ifndef HYDRA1COMP
        for(Int_t i = 0; i < vect_size; ++i)
        {
            HFwDetCand * track = HCategoryManager::getObject(track, fFwDetCand, i);
            ana->setFwDetTrackInfo(track, i);
//             dEdx_plots.fill(track);  FIXME
//             beta_plots.fill(track);
        }
#endif

        //#### ANALYSIS ####
#ifdef HYDRA1COMP
        ana->analysis(fEvent, event_num, fCatCand, cand_size);
#else
        ana->analysis(fEvent, event_num, fCatCand, cand_size, fFwDetCand, vect_size);
#endif
        for(Int_t i = 0; i < cand_size; ++i)
        {
            if (ana->getHadesTrackInfo(i).is_used)
            {
#ifdef HYDRA1COMP
                HPidTrackCand * tcand = (HPidTrackCand *)fCatCand->getObject(i);
                HParticleCand _track = *tcand;
                HParticleCand * track = &_track;
#else
                HParticleCand * track = HCategoryManager::getObject(track, fCatCand, i);
#endif
                dEdx_plots.fill_acc(track);
                beta_plots.fill_acc(track);
            }
        }

    } // End event loop
    std::cout << nEvents-first_event << " events analyzed" << std::endl;

    ana->postAnalysis();

    Outputfile->cd();

    dEdx_plots.drawPlots();
    beta_plots.drawPlots();

    ana->finalizeAnalysis();

    exportCanvas();

    std::cout << "******* End of program ********" << std::endl;
}

int KTifiniAnalysis::Configure(int argc, char ** argv)
{
    int c;

    while (1)
    {
        static struct option long_options[] =
            {
            /* These options set a flag. */
            {"verbose", no_argument,    &flag_verbose, 1},
            {"brief",   no_argument,    &flag_verbose, 0},
            /* These options don't set a flag.
                We distinguish them by their indices. */
            {"help",    no_argument,       0, 'h'},
            {"events",  required_argument, 0, 'e'},
            {"dir",     required_argument, 0, 'd'},
            {"output",  required_argument, 0, 'o'},
            {"suffix",  required_argument, 0, 's'},
            {0, 0, 0, 0}
            };
        /* getopt_long stores the option index here. */
        int option_index = 0;
    
        c = getopt_long (argc, argv, "d:e:f:o:s:h", long_options, &option_index);
    
        /* Detect the end of the options. */
        if (c == -1)
            break;
    
        switch (c) {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                break;
    
            case 'e':
                eventsnr = atoi(optarg);
                break;

            case 'o':
                file_out = optarg;
                break;

            case 'd':
                outdir = optarg;
                break;

            case 's':
                suffix = optarg;
                break;

            case 'h':
            case '?':
                /* getopt_long already printed an error message. */
                Usage();
                break;
    
            default:
                abort();
        }
    }

    if (optind < argc)
    {
        Bool_t ret = kFALSE;
        while (optind < argc)
        {
            TString infile = argv[optind++];
#ifdef HYDRA1COMP
            if (infile.Contains(".root"))  { ret = source->addFile(infile); if (file_out.Length() == 0) file_out = infile; }
#else
            if        (infile.Contains(","))    ret = loop->addMultFiles(infile);
            else if (infile.Contains(".root"))  { ret = loop->addFiles(infile); if (file_out.Length() == 0) file_out = infile; }
            else                                { ret = loop->addFilesList(infile); if (file_out.Length() == 0) file_out = infile; }
#endif
            if (file_out.Length() == 0)
                file_out = "output.root";

            if (!ret)
            {
                std::cerr << "READBACK: ERROR : cannot find inputfiles : " << infile.Data() << endl;
                std::exit(EXIT_FAILURE);
            }
        }
    }

    return optind;
}

void KTifiniAnalysis::Usage() const
{
    std::cout <<
    "options: \n" <<
    " -e | --events [value]\t\t - number of events to proceed, default is 100000\n" <<
    " -d | --filedir [path]\t\t - path to directory with input file, default is current directory\n" <<
    " -f | --file [filename]\t\t - filename, if relative filename then path from '-d' is used\n" <<
    " -k | --listdir [path]\t\t - path to directory with input list, default is current directory\n" <<
    " -l | --file [filename]\t\t - listname, if relative listname then path from '-k' is used\n" <<
    " -o | --outdir [string]\t\t - directory for output files, default is empty\n" <<
    " -s | --suffix [string]\t\t - optional suffix for output files, default is username from $USER\n" <<
    "      --with-wall\t\t - add ForwardWall for PID to analysis\n" <<
    "      --verbose\t\t\t - speak more\n" <<
    "      --brief\t\t\t - speak less [default]\n" <<
    "\n\n";

    if (ana)
    {
        ana->Usage();
    }

    exit(EXIT_SUCCESS);
}
