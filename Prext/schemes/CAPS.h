//
// Copyright (C) 2016 Karim Emara <emara@in.tum.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __VEINS_CAPS_H_
#define __VEINS_CAPS_H_

#include "veins/modules/Prext/base/BasePrivLayer.h"
#include <NNPDA.h>
#include <map>
using namespace std;

class CAPS : public BasePrivLayer
{

public:
    CAPS() : BasePrivLayer(), bSilent(false), curT(0)
    {}
    CAPS(unsigned stacksize) : BasePrivLayer(stacksize), bSilent(false), curT(0)
    {}
    ~CAPS();
    virtual void initialize(int);

protected:
      cMessage* changePsynmEvt = 0;
      cMessage* exitSilenceEvt = 0;

      simtime_t initPsynmLifetime;

      simtime_t minPsynmLifetime;
      simtime_t maxPsynmLifetime;
      simtime_t minSilentTime;
      simtime_t maxSilentTime;
      int missedBeaconsthreshold;
      int nSilentNeighbors;
      int neighborhoodThresholdSq;
      double maxGate;
      bool bSilent;
      double dist2DelTrackSq ;
      simtime_t silenceStartTime;

      simsignal_t sSilentTimes;

      map<uint64_t, bcnSample> tmpBeacons;
      NNPDA tracker;
      kalmanTrack selfTrack;
      int curT;

      //virtual void handleSelfMsg(cMessage* msg) ;

      virtual void handleUpperMsg(cMessage *msg);
      virtual void handleUpperControl(cMessage *msg);

      virtual void handleLowerMsg(cMessage *msg);
      virtual void handleLowerControl(cMessage *msg);

      virtual void handleUpperMsgSilent(WAVEBeacon *, bool ctrlMsg);
      virtual void handleUpperMsgActive(WAVEBeacon *, bool ctrlMsg);
      vector<kalmanTrack> getMissedTracks(const bcnSample &bcn);
      bcnSample msg2BcnSample(WAVEBeacon * bsm);
};

#endif
