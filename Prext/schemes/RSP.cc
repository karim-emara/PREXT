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

#include "RSP.h"
#include "veins/modules/Prext/base/PrivBase.h"

Define_Module(RSP);

RSP::~RSP() {
    cancelAndDelete(changePsynmEvt);
    cancelAndDelete(exitSilenceEvt);
}

void RSP::initialize(int stage)
{
    BasePrivLayer::initialize(stage);
    if(stage==0){
        psynmLifetime = par("psynmLifetime").longValue();
        psynmLifetime_t = SimTime(psynmLifetime, SIMTIME_S);
        initPsynmLifetime = par("initPsynmLifetime").longValue();
        if (initPsynmLifetime < 0 || initPsynmLifetime >= psynmLifetime) initPsynmLifetime = 0;

        minSilentTime = par("minSilentTime").longValue();
        maxSilentTime = par("maxSilentTime").longValue();
        ASSERT(minSilentTime < maxSilentTime);

        changePsynmEvt = new cMessage("chngPsynm", PrivLayerMessageKinds::CHANGE_PSYNM);
        exitSilenceEvt = new cMessage("exitSil", PrivLayerMessageKinds::EXIT_SILENCE);
        scheduleAt(simTime()+ SimTime(psynmLifetime - initPsynmLifetime, SIMTIME_S), changePsynmEvt);

        // register silent times signal
        sSilentTimes = registerSignal("stimes");
        silenceStartTime = 0;
    }
}

void RSP::handleSelfMsg(cMessage* msg) {
    switch (msg->getKind()) {
        case PrivLayerMessageKinds::CHANGE_PSYNM : {
            if (!bSilent) {
                bSilent = true;
                silenceStartTime = simTime().trunc(SimTimeUnit::SIMTIME_MS);
                SimTime silentperiod = SimTime(uniform(minSilentTime, maxSilentTime), SIMTIME_S);
                scheduleAt(simTime()+ silentperiod, exitSilenceEvt);
            }
            break;
        }
        case PrivLayerMessageKinds::EXIT_SILENCE : {
            bSilent = false;
            if (silenceStartTime != 0)
                emit(sSilentTimes, (simTime().trunc(SimTimeUnit::SIMTIME_MS)-silenceStartTime).dbl());
            silenceStartTime = 0;
            changePsynm();
            scheduleAt(simTime()+ psynmLifetime_t, changePsynmEvt);
            break;
        }
        default: {
            if (msg)
                EV << "RSP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
            break;
        }
    }
}
void RSP::handleUpperMsg(cMessage *msg) {
    if (bSilent)
        delete msg;
    else
        BasePrivLayer::handleUpperMsg(msg);

}

void RSP::handleUpperControl(cMessage *msg) {
    if (bSilent)
        delete msg;
    else
        BasePrivLayer::handleUpperControl(msg);
 }

