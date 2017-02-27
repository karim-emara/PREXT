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

#include "SLOW.h"
using Veins::TraCIMobilityAccess;

Define_Module(SLOW);

void SLOW::initialize(int stage)
{
    BasePrivLayer::initialize(stage);
    if(stage==0){
        traci = TraCIMobilityAccess().get(getParentModule());

        silentTimeThreshold = SimTime(par("silentTimeThreshold").longValue(), SIMTIME_S);
        speedThresholdSqr = par("speedThreshold").doubleValue();
        speedThresholdSqr *= speedThresholdSqr;

        slowPeriodStart = 0;
        sSilentTimes = registerSignal("stimes");
        spcSilentTimes = registerSignal("spctimes");
    }
}

bool SLOW::isSLOW(){
    if (traci->getCurrentSpeed().squareLength() < speedThresholdSqr) {
        if (slowPeriodStart == 0)
            slowPeriodStart = simTime();
        return true;
    }
    else {
        if (slowPeriodStart != 0 ) {
            // silence should be ended here, then emit a "silent time" signal
            emit(sSilentTimes, (simTime()-slowPeriodStart).dbl());
            if ((simTime() - slowPeriodStart) >= silentTimeThreshold) {
                // if the pseudonym should be changed, then emit an extra "silent time wt psynm change" signal
                emit(spcSilentTimes, (simTime()-slowPeriodStart).dbl());
                changePsynm();
            }
        }
        slowPeriodStart = 0;
        return false;
    }
}
void SLOW::handleUpperMsg(cMessage *msg) {
    if (isSLOW())
        delete msg;
    else
        BasePrivLayer::handleUpperMsg(msg);

}

void SLOW::handleUpperControl(cMessage *msg) {
    if (isSLOW())
        delete msg;
    else
        BasePrivLayer::handleUpperControl(msg);
 }
