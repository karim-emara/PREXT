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

#include "CPN.h"
#include <veins/modules/Prext/base/messages/WAVEBeacon_m.h>
using Veins::TraCIMobilityAccess;

Define_Module(CPN);

void CPN::initialize(int stage)
{
    BasePrivLayer::initialize(stage);
    if(stage==0){
        traci = TraCIMobilityAccess().get(getParentModule());
        neighborRadius = par("neighborRadius").doubleValue();
        kNeighbors = par("kNeighbors").longValue();
        readyFlag = false;
        setReadyFlagReceived = false;
        nNeighbor = 0;
    }
}
void CPN::beaconToBeSent(WAVEBeacon *bcn) {

    if (readyFlag || setReadyFlagReceived) { //local or remote flags is set, change pseudonym
        BasePrivLayer::changePsynm();
        readyFlag = false;
    }
    else if (nNeighbor >= kNeighbors) //meets the trigger, set the flag
        readyFlag = true;

    bcn->setCPN_readyFlag(readyFlag);

    //reset state for the next time slot
    nNeighbor = 0;
    setReadyFlagReceived= false;
}
void CPN::handleUpperMsg(cMessage *msg) {
    WAVEBeacon * bcn = dynamic_cast<WAVEBeacon*>(msg);
    if (bcn)
        beaconToBeSent(bcn);
    BasePrivLayer::handleUpperMsg(msg);
}

void CPN::handleUpperControl(cMessage *msg) {
    WAVEBeacon * bcn = dynamic_cast<WAVEBeacon*>(msg);
    if (bcn)
        beaconToBeSent(bcn);
    BasePrivLayer::handleUpperControl(msg);
}

void CPN::msgArrived(cMessage *msg) {
    WAVEBeacon * bcn = dynamic_cast<WAVEBeacon*>(msg);

    // if not wave beacon, then do nothing
    if (!bcn) return;

    // the vehicle is farther than I expect, ignore it too
    if (traci->getCurrentPosition().distance(bcn->getSenderPos()) > neighborRadius) return;

    // update the state for the next time slot
    if (bcn->getCPN_readyFlag()) setReadyFlagReceived = true;

    nNeighbor ++;
}
void CPN::handleLowerMsg(cMessage *msg) {
    msgArrived(msg);
    BasePrivLayer::handleLowerMsg(msg);
}

void CPN::handleLowerControl(cMessage *msg) {
    msgArrived(msg);
    BasePrivLayer::handleLowerControl(msg);
 }
