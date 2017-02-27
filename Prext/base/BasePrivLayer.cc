//
// Copyright (C) 2016 Karim Emara <emara@in.tum.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without privEVen the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "BasePrivLayer.h"
#include "veins/modules/Prext/base/PrivBase.h"
using Veins::TraCIMobilityAccess;

Define_Module(BasePrivLayer);

#define privEV if(debug) EV

void BasePrivLayer::initialize(int stage)
{
    BaseLayer::initialize(stage);
    if(stage==0){
        myId = getParentModule()->getIndex() + 1; //avoid CarId == 0
        startTime = simTime();
        psynmStartTime = simTime();
        mxzStartTime = 0;
        traci = TraCIMobilityAccess().get(getParentModule());
        sLifetime = registerSignal("vlt");
        sPtimes = registerSignal("ptimes");
        sPsynmchng = registerSignal("npchng");
        sMixzone = registerSignal("nxz");
        sMxZtimes = registerSignal("mxztimes");
    }

}
void BasePrivLayer::finish() {
    emit(sPsynmchng, (unsigned int)psynmCnt-1); //psynmCnt is initialized by 1
    emit(sLifetime, (simTime() - startTime).dbl());
    emit(sPtimes, (simTime() - psynmStartTime).dbl());
    recordParametersAsScalars();
}

void BasePrivLayer::changePsynm() {
     ++psynmCnt;
     emit(sPtimes, (simTime() - psynmStartTime).dbl());
     psynmStartTime = simTime();
}

void BasePrivLayer::handleUpperMsg(cMessage *msg) {
    WAVEBeacon *bsm = dynamic_cast<WAVEBeacon*>(msg);
    if (bsm) {
        if (isInMixZone())  // encrypt messages if car is currently in a mix-zone
            bsm->setIsEncrypted(true);
        bsm->setSenderPsynm(getCurrentPsynm());
        sendDown(bsm);
    }
    else
        sendDown(msg);
}

void BasePrivLayer::handleUpperControl(cMessage *msg) {
    WAVEBeacon *bsm = dynamic_cast<WAVEBeacon*>(msg);
    if (bsm) {
        if (isInMixZone()) // encrypt messages if car is currently in a mix-zone
            bsm->setIsEncrypted(true);
        bsm->setSenderPsynm(getCurrentPsynm());
        sendControlDown(bsm);
    }
    else
        sendControlDown(msg);

}
bool BasePrivLayer::isInMixZone(){
    map<int,mixZoneInfo>::iterator it = mixZones.begin();
    bool bChngPsynm = false;

    while (it != mixZones.end()) {
        if (it->second.zoneType == 1) { // circular
            if (it->second.circularPos.distance(traci->getCurrentPosition()) <= it->second.circularRange) {
                if (mxzStartTime == 0) mxzStartTime = simTime();
                privEV << "Currently in a mix-zone at " << traci->getCurrentPosition() << ", which located at " << it->second.circularPos << endl;
                return true;
            }
            else    { // Car left the mix zone,
                privEV << "Exit mix-zone at " << traci->getCurrentPosition() << ", distance: " << it->second.circularPos.distance(traci->getCurrentPosition()) << endl;
                // I am in a mix-zone already (mxzStartTime != 0) and I left this one.
                // mark pseudonym to be changed, if I am not in another mix-zone
                if (mxzStartTime != 0)
                    bChngPsynm = true;

                // in any case, if I am far from the effective range, then delete this record
                it = mixZones.erase(it);
            }
        }
        else { // convex polygon
            //TODO: implement convex polygon zones
            return false;
        }
    }

    // pseudonym is marked to be changed and no other mix-zone encountered
    if (bChngPsynm) {
        emit(sMxZtimes, (simTime()-mxzStartTime).dbl());
        mxzStartTime = 0;
        changePsynm();
    }

    return false;
}
void BasePrivLayer::handleMixZoneAd(MixZoneAd* ad) {
    mixZoneInfo mzi;
    mzi.zoneType = ad->getZoneType();
    if (ad->getZoneType() == 1) {   //circular zone
        privEV << "Circular mix zone ad of range (" << ad->getCircularRange() << ")received. Distance: " << ad->getSenderPos().distance(traci->getCurrentPosition()) << endl;
        if (ad->getSenderPos().distance(traci->getCurrentPosition()) <= ad->getCircularRange()) {  // and I'm in this zone range already, then add it to the encountered zones
            if (mixZones.find(ad->getSenderAddress()) == mixZones.end()) {
                // this ad does not belong to a mix-zone currently encountered, then declare a new mix zone found
                // even if the car is currently in a mix zone.
                emit(sMixzone, (unsigned long)1);

                if (mxzStartTime == 0) mxzStartTime = simTime();    //but don't override start time
            }
            mzi.circularPos = ad->getSenderPos();
            mzi.circularRange = ad->getCircularRange();
            mixZones[ad->getSenderAddress()] = mzi;     // add or update the mix-zone information
            privEV << "Entered/updated mix-zone at " << traci->getCurrentPosition() << ", which located at " << mzi.circularPos << endl;
        }
    }
    else {
        //TODO: implement convex polygon zones
    }

}

void BasePrivLayer::handleLowerMsg(cMessage* msg) {
    if (msg->getKind() == PrivLayerMessageKinds::MIX_ZONE_AD) {
        handleMixZoneAd(dynamic_cast<MixZoneAd*>(msg));
    }
    sendUp(msg);
 }

void BasePrivLayer::handleLowerControl(cMessage* msg) {
    if (msg->getKind() == PrivLayerMessageKinds::MIX_ZONE_AD) {
        handleMixZoneAd(dynamic_cast<MixZoneAd*>(msg));
    }
     sendControlUp(msg);
 }
