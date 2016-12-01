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

#ifndef __VEINS_SLOW_H_
#define __VEINS_SLOW_H_

#include "veins/modules/Prext/base/BasePrivLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
using Veins::TraCIMobility;

class SLOW : public BasePrivLayer
{

public:
    SLOW() : BasePrivLayer()
    {}
    SLOW(unsigned stacksize) : BasePrivLayer(stacksize)
    {}
    ~SLOW()
    {}

    virtual void initialize(int);

protected:
      SimTime silentTimeThreshold;
      double speedThresholdSqr;

      SimTime slowPeriodStart;
      simsignal_t sSilentTimes;
      simsignal_t spcSilentTimes;

      TraCIMobility* traci;
      virtual void handleSelfMsg(cMessage* msg) {};
      virtual void handleUpperMsg(cMessage *msg);
      virtual void handleUpperControl(cMessage *msg);
      virtual bool isSLOW();
};

#endif
