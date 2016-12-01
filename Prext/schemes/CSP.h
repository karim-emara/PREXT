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

#ifndef __VEINS_CSP_H_
#define __VEINS_CSP_H_

#include "veins/modules/Prext/base/BasePrivLayer.h"

class CSP : public BasePrivLayer
{

public:
    CSP() : BasePrivLayer(), changePsynmEvt(0), exitSilenceEvt(0), bSilent(false)
    {}
    CSP(unsigned stacksize) : BasePrivLayer(stacksize), changePsynmEvt(0), exitSilenceEvt(0), bSilent(false)
    {}
    ~CSP();
    virtual void initialize(int);
    virtual void finish() {
        cancelAndDelete(changePsynmEvt);
        cancelAndDelete(exitSilenceEvt);

    }

protected:
      cMessage* changePsynmEvt;
      cMessage* exitSilenceEvt;

      double psynmLifetime;
      double silentPeriod;

      bool bSilent;
      simtime_t silenceStartTime;
      simsignal_t sSilentTimes;

      virtual void handleSelfMsg(cMessage* msg) ;

      virtual void handleUpperMsg(cMessage *msg);
      virtual void handleUpperControl(cMessage *msg);
};

#endif
