//
// Copyright (C) 2016 Karim Emara <emara@in.tum.de>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "PrivateSampleApp.h"
#include <veins/modules/Prext/base/PrivBase.h>

using Veins::TraCIMobilityAccess;

Define_Module(PrivateSampleApp);
PrivateSampleApp::~PrivateSampleApp() {
}

void PrivateSampleApp::initialize(int stage) {
    BaseApplLayer::initialize(stage);
	if (stage == 0) {
		traci = TraCIMobilityAccess().get(getParentModule());

        myId = getParentModule()->getIndex() + 1; //avoid CarID == 0

        sendBeaconEvt = new cMessage("beacon evt", PrivateApplMessageKinds::SEND_BEACON_EVT);

        headerLength = par("headerLength");
        dataLength = par("dataLength");
        //simulate asynchronous channel access
        double offSet = dblrand() * (par("beaconInterval").doubleValue()/2);
        offSet = offSet + floor(offSet/0.050)*0.050;
        scheduleAt(simTime() + offSet, sendBeaconEvt);

        individualOffset = dblrand() * 0.005;
	}
}
void PrivateSampleApp::handleSelfMsg(cMessage* msg) { // it an event/self-msg that has be called by scheduleAt with certain time

    switch (msg->getKind()) {
        case PrivateApplMessageKinds::SEND_BEACON_EVT: {

            WAVEBeacon* bcn = prepareBeacon();
            sendDelayedDown(bcn,individualOffset);

            scheduleAt(simTime() + par("beaconInterval").doubleValue(), sendBeaconEvt);
            break;
        }
        default: {
            if (msg)
                EV << "APP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
            break;
        }
    }

}

void PrivateSampleApp::handleLowerMsg(cMessage* msg) {


    if (msg->getKind() == PrivateApplMessageKinds::WAVE_BEACON) {
        onBeacon(dynamic_cast<WAVEBeacon*>(msg));
    }
    else if (msg->getKind() == PrivateApplMessageKinds::WAVE_DATA) {
        onData(dynamic_cast<WAVEBeacon*>(msg));
    }
    else if (msg->getKind() == PrivLayerMessageKinds::MIX_ZONE_AD) {

    }
    else {
        EV << "unknown message (" << msg->getName() << ")  received\n";
    }
    delete msg;
}
void PrivateSampleApp::onBeacon(WAVEBeacon* wsm) {
}

void PrivateSampleApp::onData(WAVEBeacon* wsm) {

}
WAVEBeacon*  PrivateSampleApp::prepareBeacon() {

    WAVEBeacon* bsm = new WAVEBeacon("BSM");
    bsm->addBitLength(headerLength);
    bsm->addByteLength(dataLength);

    bsm->setKind(PrivateApplMessageKinds::WAVE_BEACON);
    bsm->setChannelNumber(Channels::CCH);
    bsm->setSenderAddress(myId);
    bsm->setRecipientAddress(0);
    bsm->setSerial(-1);
    bsm->setWsmData("");
    bsm->setWsmLength(headerLength+dataLength);

    bsm->setTimestamp(simTime());
    bsm->setSenderPsynm(0);     //TO be changed in the privacy layer
    bsm->setSenderPos(traci->getCurrentPosition());
    bsm->setSenderAngle(traci->getAngleRad());
    bsm->setSenderVel(traci->getCurrentSpeed());

    return bsm;
}
void PrivateSampleApp::finish() {
    BaseApplLayer::finish();

    cancelAndDelete(sendBeaconEvt);
    recordParametersAsScalars();
}
