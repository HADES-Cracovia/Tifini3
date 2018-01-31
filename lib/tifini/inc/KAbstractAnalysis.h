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


#ifndef KABSTRACTANALYSIS_H
#define KABSTRACTANALYSIS_H

#include <vector>

#include <TString.h>

#include "Tifini3Config.h"

#ifdef HYDRA1COMP
#include "HParticleCand.h"
#include "heventheader.h"
#else
#include "hparticlecand.h"
#include "hparticleevtinfo.h"
#include "hfwdetcand.h"
#endif

#include "hcategory.h"
#include "hevent.h"

#include "KTifiniAnalysis.h"
#include "KTrackInspector.h"

struct BeamGeom
{
    Double_t x;
    Double_t y;
    Double_t z;
};

struct TargetGeom
{
    Double_t x;
    Double_t y;
    Double_t a;
    Double_t b;
};

struct TrackInfo
{
    TrackInfo();

    Int_t tid = -1;

    bool pid[KT::PID_Dummy+1][KT::PST_Dummy];
    int polarity = 0;
    bool is_used = false;
};

class KAbstractAnalysis
{

public:
    KAbstractAnalysis(const TString & analysisName = "analysis", const TString & treeName = "tree");
    virtual ~KAbstractAnalysis();

    void resetEvent();
#ifndef HYDRA1COMP
    Bool_t setEventInfo(HParticleEvtInfo* evtinfo);
#endif
    Bool_t isGoodEvent() const;

    void setHadesTrackInfo(HParticleCand * track, Int_t track_num);
#ifndef HYDRA1COMP
    void setFwDetTrackInfo(HFwDetCand * track, Int_t track_num);
#endif

    const TrackInfo & getHadesTrackInfo(Int_t track_num) const { return hades_tracks[track_num]; }

    virtual void initAnalysis(KT::Experiment exp, KT::AnalysisType analysisType);
    virtual void finalizeAnalysis();

    inline /*const*/ KTrackInspector & getCuts() { return trackInspector; };
    inline TTree * getTree() { return tree; };

    inline void setBeamGeometry(const BeamGeom & beamGeom) { bg = beamGeom; }
    inline const BeamGeom & getBeamGeometry() { return bg; }
    inline const HGeomVector getBeamGeomVector() { return HGeomVector(bg.x, bg.y, bg.z); }

    inline void setTargetGeometry(const TargetGeom & targetGeom) { tg = targetGeom; }
    inline const TargetGeom & getTargetGeometry() { return tg; }
    inline const HGeomVector getTargetGeomVector() { return HGeomVector(tg.x, tg.y, (tg.a+tg.b)/2.0); }

    inline void setVerboseFlag(bool verbose = true) { verbose_flag = verbose; }
    inline bool getVerboseFlag() { return verbose_flag; }

    virtual void preAnalysis() {};
#ifdef HYDRA1COMP
    virtual Bool_t analysis(HEvent * fEvent, Int_t event_num, HCategory * pcand, Int_t cand_size) = 0;
#else
    virtual Bool_t analysis(HEvent * fEvent, Int_t event_num, HCategory * pcand, Int_t cand_size, HCategory * vcand, Int_t vect_size) = 0;
#endif
    virtual void postAnalysis() {};

    virtual int Configure(int argc, char ** argv);
    virtual void Usage() const;

    virtual UInt_t getGoodEventSelector() const { return good_event_selector; }
    virtual void setGoodEventSelector(UInt_t ges) { good_event_selector = ges; }

    virtual void setPidSelectionHades(KT::ParticleID pid, UInt_t pst) { pid_hades[pid] = pst; }
    virtual void setPidSelectionFwDet(KT::ParticleID pid, UInt_t pst) { pid_fwdet[pid] = pst; }

protected:
    virtual void configureGraphicalCuts(KTrackInspector & trackInsp) = 0;
    virtual void configureTree(TTree * tree) = 0;

    virtual void PID(HParticleCand * track, TrackInfo & tinfo);
#ifndef HYDRA1COMP
    virtual void PID(HFwDetCand * track, TrackInfo & tinfo);
#endif
    bool makePID_Beta(const HParticleCand *cand, KT::ParticleID pid);

protected:
    KT::AnalysisType analysisType;

    std::vector<TrackInfo> hades_tracks;
    std::vector<TrackInfo> fwdet_tracks;

private:
    TString _analysisName;
    bool verbose_flag;

    KTrackInspector trackInspector;
    TTree * tree;

    BeamGeom bg;
    TargetGeom tg;

    // Event info
    Bool_t eGoodTrigger;
    Bool_t eGoodStart;
    Bool_t eGoodVertCluster;
    Bool_t eGoodVertCand;
    Bool_t eNoPileUpStart;
    Bool_t eGoodEvent;

    UInt_t good_event_selector;

    UInt_t pid_hades[KT::PID_Dummy], pid_fwdet[KT::PID_Dummy];
};

#endif // KABSTRACTANALYSIS_H
