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

#include "CSP.h"
#include "veins/modules/Prext/base/PrivBase.h"
#include <cmath>
using namespace std;
Define_Module(CSP);

CSP::~CSP() {
}

void CSP::initialize(int stage)
{
    BasePrivLayer::initialize(stage);
    if(stage==0){
        psynmLifetime = par("psynmLifetime").longValue();
        silentPeriod = par("silentPeriod").longValue();
        double drift = par("clockDrift").longValue()/1000.0;

        double cycleTime = psynmLifetime + silentPeriod;

        // schedule changing pseudonym and exit silence events
        changePsynmEvt = new cMessage("chngPsynm", PrivLayerMessageKinds::CHANGE_PSYNM);
        exitSilenceEvt = new cMessage("exitSil", PrivLayerMessageKinds::EXIT_SILENCE);

        //check if car should be initially silent
        double remainderTime = fmod(simTime().dbl(), cycleTime);

        if (remainderTime >= psynmLifetime) {
            bSilent = true;
            silenceStartTime = simTime().trunc(SimTimeUnit::SIMTIME_MS);
            SimTime coordinatedExitSilenceTime = simTime() + cycleTime - remainderTime + drift;
            scheduleAt(coordinatedExitSilenceTime, exitSilenceEvt);
        }
        else {
            bSilent = false;
            silenceStartTime = 0;
            SimTime coordinatedChngTime = simTime()+ psynmLifetime - remainderTime  + drift;
            scheduleAt(coordinatedChngTime , changePsynmEvt);
        }

        // register silent times signal
        sSilentTimes = registerSignal("stimes");
    }
}

void CSP::handleSelfMsg(cMessage* msg) {
    switch (msg->getKind()) {
        case PrivLayerMessageKinds::CHANGE_PSYNM : {
            if (!bSilent) {
                bSilent = true;
                silenceStartTime = simTime().trunc(SimTimeUnit::SIMTIME_MS);
                scheduleAt(simTime()+ silentPeriod, exitSilenceEvt);
            }
            break;
        }
        case PrivLayerMessageKinds::EXIT_SILENCE : {
            if (bSilent) {
                bSilent = false;
                if (silenceStartTime != 0)
                    emit(sSilentTimes, (simTime().trunc(SimTimeUnit::SIMTIME_MS)-silenceStartTime).dbl());
                silenceStartTime = 0;
                changePsynm();
                scheduleAt(simTime()+ psynmLifetime, changePsynmEvt);
            }
            break;
        }
        default: {
            if (msg)
                EV << "CSP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
            break;
        }
    }

}
void CSP::handleUpperMsg(cMessage *msg) {
    if (bSilent)
        delete msg;
    else
        BasePrivLayer::handleUpperMsg(msg);

}

void CSP::handleUpperControl(cMessage *msg) {
    if (bSilent)
        delete msg;
    else
        BasePrivLayer::handleUpperControl(msg);
 }
