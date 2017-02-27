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

#include "PeriodicalPC.h"
#include "veins/modules/Prext/base/PrivBase.h"

Define_Module(PeriodicalPC);

void PeriodicalPC::initialize(int stage)
{
    BasePrivLayer::initialize(stage);
    if(stage==0){
        minPsynmLifetime = par("minPsynmLifetime").longValue();
        maxPsynmLifetime = par("maxPsynmLifetime").longValue();
        ASSERT(minPsynmLifetime <= maxPsynmLifetime);

        initPsynmLifetime = par("initPsynmLifetime").longValue();
        if (initPsynmLifetime < 0 || initPsynmLifetime >= minPsynmLifetime) initPsynmLifetime = 0;

        changePsynmEvt = new cMessage("chngPsynm", PrivLayerMessageKinds::CHANGE_PSYNM);
        SimTime initTime = SimTime(uniform(minPsynmLifetime, maxPsynmLifetime) - initPsynmLifetime, SIMTIME_S) ;
        scheduleAt(simTime()+ initTime, changePsynmEvt);
    }

}

void PeriodicalPC::handleSelfMsg(cMessage* msg) {
    switch (msg->getKind()) {
        case PrivLayerMessageKinds::CHANGE_PSYNM : {
            BasePrivLayer::changePsynm();
            scheduleAt(simTime()+ SimTime(uniform(minPsynmLifetime, maxPsynmLifetime), SIMTIME_S), changePsynmEvt);
            break;
        }
        default: {
            if (msg)
                EV << "PeriodicalPC: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
            break;
        }
    }
}

void PeriodicalPC::finish() {
    cancelAndDelete(changePsynmEvt);
    BasePrivLayer::finish();
}
