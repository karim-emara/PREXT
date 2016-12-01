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

#ifndef __VEINS_CPN_H_
#define __VEINS_CPN_H_

#include "veins/modules/Prext/base/BasePrivLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"

using Veins::TraCIMobility;

class CPN : public BasePrivLayer
{

public:
    CPN() : BasePrivLayer()
    {}
    CPN(unsigned stacksize) : BasePrivLayer(stacksize)
    {}
    ~CPN()
    {}

    virtual void initialize(int);

protected:

      double neighborRadius;
      int kNeighbors;

      bool readyFlag;
      bool setReadyFlagReceived;
      int nNeighbor;

      TraCIMobility* traci;
      //virtual void handleSelfMsg(cMessage* msg) {};
      virtual void handleUpperMsg(cMessage *msg);
      virtual void handleUpperControl(cMessage *msg);
      virtual void handleLowerMsg(cMessage *msg);
      virtual void handleLowerControl(cMessage *msg);
      virtual void msgArrived(cMessage *msg);
      virtual void beaconToBeSent(WAVEBeacon *bcn);
};

#endif
