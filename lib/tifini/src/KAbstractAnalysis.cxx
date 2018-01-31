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

#include "hphysicsconstants.h"

#include "KTifiniAnalysis.h"
#include "KAbstractAnalysis.h"

#include <iostream>
#include <cstring>

using namespace KTifini;

TrackInfo::TrackInfo()
{
    for(Int_t ipid = 0; ipid < KT::PID_Dummy; ++ipid)
        for(Int_t ipst = 0; ipst < KT::PST_Dummy; ++ipst)
            pid[ipid][ipst] = false;
}

KAbstractAnalysis::KAbstractAnalysis(const TString & analysisName, const TString & treeName) : _analysisName(analysisName), verbose_flag(false)
{
    tree = new TTree(treeName, analysisName);

    // event info
    tree->Branch("eGoodEvent",          &eGoodEvent,        "eGoodEvent/O");
    tree->Branch("eGoodTrigger",        &eGoodTrigger,      "eGoodTrigger/O");
    tree->Branch("eGoodStart",          &eGoodStart,        "eGoodStart/O");
    tree->Branch("eGoodVertCluster",    &eGoodVertCluster,  "eGoodVertCluster/O");
    tree->Branch("eGoodVertCand",       &eGoodVertCand,     "eGoodVertCand/O");
    tree->Branch("eNoPileUpStart",      &eNoPileUpStart,    "eNoPileUpStart/O");

#ifndef HYDRA1COMP
    setGoodEventSelector(Particle::kGoodTRIGGER |
        Particle::kGoodVertexClust |
        Particle::kGoodVertexCand |
        Particle::kGoodSTART |
        Particle::kNoPileUpSTART);
#endif

    setPidSelectionHades(KT::p, KT::Charge);
    setPidSelectionHades(KT::pim, KT::Charge);
    setPidSelectionHades(KT::pip, KT::Charge);
    setPidSelectionHades(KT::Km, KT::Charge);
    setPidSelectionHades(KT::Kp, KT::Charge);

    setPidSelectionFwDet(KT::p, KT::Beta);
    setPidSelectionFwDet(KT::pim, KT::Beta);
    setPidSelectionFwDet(KT::pip, KT::Beta);
    setPidSelectionFwDet(KT::Km, KT::Beta);
    setPidSelectionFwDet(KT::Kp, KT::Beta);
}

KAbstractAnalysis::~KAbstractAnalysis()
{
    delete tree;
//     delete cuts;
    std::cout << "+++ Analysis destructed! +++" << std::endl;
}

void KAbstractAnalysis::initAnalysis(KT::Experiment exp, KT::AnalysisType analysisType)
{
    std::cout << "+++ Preapring analysis... " << std::endl;

    this->analysisType = analysisType;

    configureGraphicalCuts(trackInspector);
    configureTree(tree);

    if (!tree)
    {
        std::cerr << "Tree in not initilized! Aborting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    switch (exp)
    {
        case KT::pp45:
            bg = {0.0, 0.0, 1.0};
            tg = {0.0, 0.0, -90.0, 80.0};
            break;
        default:
            bg = {0.0, 0.0, 0.0};
            tg = {0.0, 0.0, 0.0, 0.0};
            break;
    }
    std::cout << "* Create analysis " << _analysisName << std::endl;
}

void KAbstractAnalysis::finalizeAnalysis()
{
    std::cout << "+++ Finalizing Analysis +++" << std::endl;

    if (tree) {
        tree->Write();
        if (verbose_flag)
            tree->Print();
    }
}

void KAbstractAnalysis::resetEvent()
{
    hades_tracks.clear();
    fwdet_tracks.clear();
}

#ifndef HYDRA1COMP
Bool_t KAbstractAnalysis::setEventInfo(HParticleEvtInfo* evtinfo)
{
    eGoodTrigger = evtinfo->isGoodEvent(Particle::kGoodTRIGGER);
    eGoodStart = evtinfo->isGoodEvent(Particle::kGoodSTART);
    eGoodVertCluster = evtinfo->isGoodEvent(Particle::kGoodVertexClust);
    eGoodVertCand = evtinfo->isGoodEvent(Particle::kGoodVertexCand);
    eNoPileUpStart = evtinfo->isGoodEvent(Particle::kNoPileUpSTART);
    eGoodEvent = evtinfo->isGoodEvent(good_event_selector);

    return eGoodEvent;
}
#endif

Bool_t KAbstractAnalysis::isGoodEvent() const
{
#ifndef HYDRA1COMP
    return eGoodEvent;
#else
    return kTRUE;
#endif
}

void KAbstractAnalysis::setHadesTrackInfo(HParticleCand * track, Int_t track_num)
{
    TrackInfo ti;
    ti.tid = track_num;
    ti.polarity = track->getCharge();
    PID(track, ti);
    hades_tracks.push_back(ti);
}

#ifndef HYDRA1COMP
void KAbstractAnalysis::setFwDetTrackInfo(HFwDetCand * track, Int_t track_num)
{
    TrackInfo ti;
    ti.tid = track_num;
    ti.polarity = 0;
    PID(track, ti);
    fwdet_tracks.push_back(ti);
}

void KAbstractAnalysis::PID(HFwDetCand * track, TrackInfo & tinfo)
{
    if (pid_fwdet[KT::p] & KT::Charge) tinfo.pid[KT::p][KT::Charge] = true;
    if (pid_fwdet[KT::p] & KT::Beta) tinfo.pid[KT::p][KT::Beta] = true;
    if (pid_fwdet[KT::p] & KT::Graphical) tinfo.pid[KT::p][KT::Beta] = true;

    if (pid_fwdet[KT::pip] & KT::Charge) tinfo.pid[KT::pip][KT::Charge] = true;
    if (pid_fwdet[KT::pip] & KT::Beta) tinfo.pid[KT::pip][KT::Beta] = true;
    if (pid_fwdet[KT::pip] & KT::Graphical) tinfo.pid[KT::pip][KT::Beta] = true;

    if (pid_fwdet[KT::pim] & KT::Charge) tinfo.pid[KT::pim][KT::Charge] = true;
    if (pid_fwdet[KT::pim] & KT::Beta) tinfo.pid[KT::pim][KT::Beta] = true;
    if (pid_fwdet[KT::pim] & KT::Graphical) tinfo.pid[KT::pim][KT::Beta] = true;

    if (pid_fwdet[KT::Kp] & KT::Charge) tinfo.pid[KT::Kp][KT::Charge] = true;
    if (pid_fwdet[KT::Kp] & KT::Beta) tinfo.pid[KT::Kp][KT::Beta] = true;
    if (pid_fwdet[KT::Kp] & KT::Graphical) tinfo.pid[KT::Kp][KT::Beta] = true;

    if (pid_fwdet[KT::Km] & KT::Charge) tinfo.pid[KT::Km][KT::Charge] = true;
    if (pid_fwdet[KT::Km] & KT::Beta) tinfo.pid[KT::Km][KT::Beta] = true;
    if (pid_fwdet[KT::Km] & KT::Graphical) tinfo.pid[KT::Km][KT::Beta] = true;
}
#endif

void KAbstractAnalysis::PID(HParticleCand* track, TrackInfo& tinfo)
{
    // proton = 14, pi+ = 8, pi- = 9, K+ = 11, K- = 12, e+ = 2, e- = 3 (GEANT PID definitions)
    // trackPIDs[j] Array (global definition) j: Protons == 0, Kaons == 1, Pions == 2, Electrons == 3
    // If System == -1 there is no META hit! MetaQual is also negative

    Short_t charge = track->getCharge();
    KTrackInspector & trackInsp = getCuts();
    trackInsp.clearCache();

    if (pid_hades[KT::p] & KT::Charge) tinfo.pid[KT::p][KT::Charge] = (charge == 1);
    if (pid_hades[KT::p] & KT::Beta) tinfo.pid[KT::p][KT::Beta] = makePID_Beta(track, KT::p);
    if (pid_hades[KT::p] & KT::Graphical) tinfo.pid[KT::p][KT::Graphical] = trackInsp.isInside(KT::DEDX, KT::cut_p, track);

    if (pid_hades[KT::pip] & KT::Charge) tinfo.pid[KT::pip][KT::Charge] = (charge == 1);
    if (pid_hades[KT::pip] & KT::Beta) tinfo.pid[KT::pip][KT::Beta] = makePID_Beta(track, KT::pip);
    if (pid_hades[KT::pip] & KT::Graphical) tinfo.pid[KT::pip][KT::Graphical] = trackInsp.isInside(KT::DEDX, KT::cut_pip, track);

    if (pid_hades[KT::pim] & KT::Charge) tinfo.pid[KT::pim][KT::Charge] = (charge == -1);
    if (pid_hades[KT::pim] & KT::Beta) tinfo.pid[KT::pim][KT::Beta] = makePID_Beta(track, KT::pim);
    if (pid_hades[KT::pim] & KT::Graphical) tinfo.pid[KT::pim][KT::Graphical] = trackInsp.isInside(KT::DEDX, KT::cut_pim, track);

    if (pid_hades[KT::Kp] & KT::Charge) tinfo.pid[KT::Kp][KT::Charge] = (charge == 1);
    if (pid_hades[KT::Kp] & KT::Beta) tinfo.pid[KT::Kp][KT::Beta] = makePID_Beta(track, KT::Kp);
    if (pid_hades[KT::Kp] & KT::Graphical) tinfo.pid[KT::Kp][KT::Graphical] = trackInsp.isInside(KT::DEDX, KT::cut_Kp, track);

    if (pid_hades[KT::Km] & KT::Charge) tinfo.pid[KT::Km][KT::Charge] = (charge == -1);
    if (pid_hades[KT::Km] & KT::Beta) tinfo.pid[KT::Km][KT::Beta] = makePID_Beta(track, KT::Km);
    if (pid_hades[KT::Km] & KT::Graphical) tinfo.pid[KT::Km][KT::Graphical] = trackInsp.isInside(KT::DEDX, KT::cut_Km, track);
// printf("* charge = %d, %d, %d\n", charge, KT::p, tinfo.pid[KT::p][KT::Charge]);
// printf("* charge = %d, %d, %d\n", charge, KT::pim, tinfo.pid[KT::pim][KT::Charge]);
// printf("  charge = %d  pid=%d\n", track->getCharge(), ((HParticleCandSim *)track)->getGeantPID());
// static Int_t good_p = 0, bad_p = 0, good_pim = 0, bad_pim = 0;

// if ( ((HParticleCandSim *)track)->getGeantParentPID() <= 0 ) return;
// if ( ((HParticleCandSim *)track)->getGeantParentPID() > 0 ) return;
// if (charge < 0 and ((HParticleCandSim *)track)->getGeantPID() == 14) ++bad_p;
// if (charge > 0 and ((HParticleCandSim *)track)->getGeantPID() == 14) ++good_p;
// if (charge < 0 and ((HParticleCandSim *)track)->getGeantPID() == 9) ++good_pim;
// if (charge > 0 and ((HParticleCandSim *)track)->getGeantPID() == 9) ++bad_pim;
// printf("good_p=%d   bad_p=%d   good_pim=%d   bad_pim=%d\n", good_p, bad_p, good_pim, bad_pim);
//     Float_t RichPhi   = track->getRichPhi();
//     Float_t RichTheta = track->getRichTheta();
//     Float_t Phi       = track->getPhi();
//     Float_t Theta     = track->getTheta();
//     Float_t dphi      = fabs(RichPhi-Phi);
//     Float_t dtheta    = fabs(RichTheta-Theta);
//
//     if (dphi > 180.0)        dphi = 360.0-dphi;
//     if (dtheta > 180.0)        dtheta = 360.0-dtheta;

/*
    // PID for Leptons
    // sigma = 1.5 degree -> 3 sigma = 4.5 (4.5*4.5 = 20.25)
    if (RichPhi >= 0.0 && (dphi*dphi + dtheta*dtheta) < 20.25) {
        if(Polarity > 0) {
            trackPID[3] = PID_elp; // e+
        }
        if(Polarity < 0) {
            trackPID[3] = PID_elm; // e-
        }
    }
*/
}

bool KAbstractAnalysis::makePID_Beta(const HParticleCand *cand, KT::ParticleID pid)
{
    if (!cand)
        return false;

    Float_t beta = cand->getBeta();
    Float_t cut_val = cand->getMomentum()/sqrt( cand->getMomentum()*cand->getMomentum() +
    HPhysicsConstants::mass(pid)*HPhysicsConstants::mass(pid) );

    if( (cut_val - 0.2) < beta && (cut_val + 0.2) > beta )
        return true;
    else
        return false;
}

int KAbstractAnalysis::Configure(int argc, char** argv)
{
    return argc;
}

void KAbstractAnalysis::Usage() const
{
}
