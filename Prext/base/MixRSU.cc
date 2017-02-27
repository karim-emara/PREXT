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

#include "MixRSU.h"
#include <vector>
#include <string>
#include <veins/modules/Prext/base/PrivBase.h>
#include <veins/modules/utility/Consts80211p.h>
#include <veins/base/modules/BaseMobility.h>
using namespace std;

Define_Module(MixRSU);
MixRSU::~MixRSU() {
    cancelAndDelete(sendAdvertiseEvt);
    delete localAd;
}

void MixRSU::initialize(int stage) {
	BaseApplLayer::initialize(stage);
	if (stage == 0) {
	    myId = getParentModule()->getIndex();

        sendAdvertiseEvt = new cMessage("RSU Ad evt", PrivLayerMessageKinds::RSU_ADV_EVT);

        headerLength = par("headerLength");
        dataLength = par("dataLength");
        //simulate asynchronous channel access

        double offSet = dblrand() * (par("advertiseInterval").doubleValue()/2);
        offSet = offSet + floor(offSet/0.050)*0.050;
        scheduleAt(simTime() + offSet, sendAdvertiseEvt);

        individualOffset = dblrand() * 0.005;
        localAd = prepareAdvertise();
	}
}
vector<Convex> MixRSU::parseConvexPolygon(string strPolygon)  {

    // TODO: parse convex polygons passed in the RSU parameter

    vector<Convex> poly;
    return poly;
}

MixZoneAd* MixRSU::prepareAdvertise() {

    MixZoneAd* zad = new MixZoneAd("MIX_AD");
    zad->addBitLength(headerLength);
    zad->addByteLength(dataLength);

    zad->setKind(PrivLayerMessageKinds::MIX_ZONE_AD);
    zad->setChannelNumber(Channels::CCH);
    zad->setSenderAddress(myId);
    zad->setRecipientAddress(0);
    zad->setSerial(-1);
    zad->setWsmData("");
    zad->setWsmLength(headerLength+dataLength);

    zad->setTimestamp(simTime());
    BaseMobility* mob = (BaseMobility*)this->getParentModule()->getSubmodule("mobility");
    Coord pos(mob->par("x").doubleValue(),mob->par("y").doubleValue(),mob->par("z").doubleValue());
    zad->setSenderPos(pos);
    zad->setZoneType(par("zoneType").longValue());

    if (par("zoneType").longValue() == 1) { //circular
        zad->setCircularRange(par("zoneCircularRange").longValue());
        cDisplayString& dispStr = getParentModule()->getDisplayString();
        stringstream  str;
        str << "p=" << mob->par("x").doubleValue() << "," << mob->par("y").doubleValue() << ";r= " << par("zoneCircularRange").doubleValue() << ",lavender,,1;i=block/routing;is=s";
        dispStr.parse(str.str().c_str());
    }
    else {
        vector<Convex> poly = parseConvexPolygon(par("zoneConvexPolygons").stdstringValue());
        zad->setZonePolygonArraySize(poly.size());
        for (unsigned int i = 0; i< poly.size(); i++)
            zad->setZonePolygon(i, poly[i]);
    }
    return zad;
}
void MixRSU::handleSelfMsg(cMessage* msg) { // it an event/self-msg that has be called by scheduleAt with certain time

    switch (msg->getKind()) {
        case PrivLayerMessageKinds::RSU_ADV_EVT: {
            MixZoneAd * msg = localAd->dup();
            msg->setTimestamp(simTime());
            sendDelayedDown(msg ,individualOffset);

            scheduleAt(simTime() + par("advertiseInterval").doubleValue(), sendAdvertiseEvt);
            break;
        }
        default: {
            if (msg)
                EV << "APP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
            break;
        }
    }

}
void MixRSU::handleLowerMsg(cMessage* msg) {

    // Skip messages received from other nodes.
    // No actual key establishment.
    delete msg ;
}

void MixRSU::handleLowerControl(cMessage* msg) {

    // Skip messages received from other nodes.
    // No actual key establishment.
    delete msg ;
}

