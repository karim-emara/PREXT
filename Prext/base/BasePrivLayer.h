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

#ifndef __VEINS_BASEPRIVLAYER_H_
#define __VEINS_BASEPRIVLAYER_H_

#include "veins/base/modules/BaseLayer.h"
#include <assert.h>
#include <veins/modules/Prext/base/messages/WAVEBeacon_m.h>
#include <veins/modules/Prext/base/messages/MixZoneAd_m.h>
#include <map>
#include <veins/modules/mobility/traci/TraCIMobility.h>
using namespace std;
using Veins::TraCIMobility;

class BasePrivLayer : public BaseLayer
{
public:
    BasePrivLayer() : BaseLayer(), psynmCnt(1)
    {}
    BasePrivLayer(unsigned stacksize) : BaseLayer(stacksize), psynmCnt(1)
    {}
    virtual void initialize(int);
    virtual void finish();
    inline uint64_t getCurrentPsynm() {
          return (((uint64_t)myId) << 32) | ((uint64_t)psynmCnt);
      }

protected:
     unsigned int psynmCnt;
     int myId;

     virtual void changePsynm();

     virtual void handleSelfMsg(cMessage* msg) {};

    virtual void handleLowerMsg(cMessage* msg) ;

    virtual void handleLowerControl(cMessage* msg);

    virtual void handleUpperMsg(cMessage *msg);

    virtual void handleUpperControl(cMessage *msg);

    virtual bool isInMixZone();
    void handleMixZoneAd(MixZoneAd* ad);

    struct mixZoneInfo {
      int zoneType;
      Coord circularPos;
      int circularRange;
      vector<Convex> polys;
    };

    map<int, mixZoneInfo> mixZones;
    TraCIMobility* traci;
    simtime_t startTime;
    simtime_t psynmStartTime;
    simtime_t mxzStartTime;

    simsignal_t sLifetime;
    simsignal_t sPtimes;
    simsignal_t sPsynmchng;
    simsignal_t sMixzone;
    simsignal_t sMxZtimes;
};

#endif
