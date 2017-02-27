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

#include "CAPS.h"
#include "veins/modules/Prext/base/PrivBase.h"

Define_Module(CAPS);

CAPS::~CAPS() {
    cancelAndDelete(changePsynmEvt);
    cancelAndDelete(exitSilenceEvt);
}

void CAPS::initialize(int stage)
{
    BasePrivLayer::initialize(stage);
    if(stage==0){
        maxGate = par("maxGate").doubleValue();
        minPsynmLifetime = SimTime(par("minPsynmLifetime").longValue(), SimTimeUnit::SIMTIME_S);
        maxPsynmLifetime = SimTime(par("maxPsynmLifetime").longValue(), SimTimeUnit::SIMTIME_S);

        initPsynmLifetime = SimTime(par("initPsynmLifetime").longValue(), SimTimeUnit::SIMTIME_S);
        if (initPsynmLifetime < 0 || initPsynmLifetime >= maxPsynmLifetime) initPsynmLifetime = 0;
        BasePrivLayer::psynmStartTime = simTime() - initPsynmLifetime;

        minSilentTime = SimTime(par("minSilentTime").longValue(), SimTimeUnit::SIMTIME_S);
        maxSilentTime = SimTime(par("maxSilentTime").longValue(), SimTimeUnit::SIMTIME_S) ;
        ASSERT(minSilentTime < maxSilentTime);

        missedBeaconsthreshold = par("missedBeaconsthreshold").longValue();
        nSilentNeighbors = par("nSilentNeighbors").longValue();

        bSilent = false;

        neighborhoodThresholdSq = par("neighborhoodThreshold").longValue();
        neighborhoodThresholdSq *= neighborhoodThresholdSq;
        dist2DelTrackSq = par("dist2DelTrack").longValue();
        dist2DelTrackSq *= dist2DelTrackSq ;

        tracker.initNNPDAParam(30, 2, maxSilentTime.dbl());
        sSilentTimes = registerSignal("stimes");
    }
}

void CAPS::handleLowerMsg(cMessage *msg) {
    WAVEBeacon * bcn = dynamic_cast<WAVEBeacon*>(msg);
    if (bcn)
        tmpBeacons[bcn->getSenderPsynm()] = msg2BcnSample(bcn);

    BasePrivLayer::handleLowerMsg(msg);
}
void CAPS::handleLowerControl(cMessage *msg) {
    WAVEBeacon * bcn = dynamic_cast<WAVEBeacon*>(msg);
    if (bcn)
        tmpBeacons[bcn->getSenderPsynm()] = msg2BcnSample(bcn);

    BasePrivLayer::handleLowerControl(msg);
}
vector<kalmanTrack> CAPS::getMissedTracks(const bcnSample &bcn) {

    vector<kalmanTrack> tracks = tracker.getTrackList();
    vector<kalmanTrack>::iterator it = tracks.begin();
    while (it != tracks.end()) {
        if (tracker.getCurTimestep() - it->lastUpdateTime < missedBeaconsthreshold)
            it = tracks.erase(it);
        else if (((bcn.px - it->x(0))*(bcn.px - it->x(0)) +
                  (bcn.py - it->x(3))*(bcn.py - it->x(3))) > neighborhoodThresholdSq)
            it = tracks.erase(it);
        else
            it++;
    }
    return tracks;

}
void CAPS::handleUpperMsgSilent(WAVEBeacon *msg, bool ctrlMsg) {
    tracker.trackBeacons(tmpBeacons);
    tmpBeacons.clear();
    bcnSample bcn = msg2BcnSample(msg);
    tracker.deleteFarTracks(dist2DelTrackSq, bcn.px, bcn.py);

    if (simTime() - silenceStartTime >= maxSilentTime)
        bSilent = false;
    else if (simTime() - silenceStartTime >= minSilentTime) {
        vector<kalmanTrack> misstracks = getMissedTracks(bcn);
        if (!misstracks.empty()) {
            double selfD = selfTrack.calcD(bcn);
            double minOtherD = INFINITY, d;
            for (int i = 0; i < misstracks.size(); i++) {
                d = misstracks[i].calcD(bcn);
                if (d <= minOtherD)
                    minOtherD = d;
            }
            if ((minOtherD < selfD || selfD > maxGate) &&  rand()%2 == 0)
                bSilent = false;
        }
    }


    if (bSilent)
        delete msg;
    else {
        emit(sSilentTimes, (simTime()-silenceStartTime).dbl());
        psynmStartTime = simTime();
        selfTrack.update(bcn, curT++);
        if (ctrlMsg)
            BasePrivLayer::handleUpperControl(msg);
        else
            BasePrivLayer::handleUpperMsg(msg);

    }

}
void CAPS::handleUpperMsgActive(WAVEBeacon *msg, bool ctrlMsg) {
    tracker.trackBeacons(tmpBeacons);
    tmpBeacons.clear();
    bcnSample bcn = msg2BcnSample(msg);
    tracker.deleteFarTracks(dist2DelTrackSq, bcn.px, bcn.py);

    if (simTime() - psynmStartTime >= maxPsynmLifetime)
        bSilent = true;
    else if (simTime() - psynmStartTime >= minPsynmLifetime)
        if (getMissedTracks(bcn).size() >= nSilentNeighbors && rand()%2 == 0)
            bSilent = true;

    if (bSilent) {
        silenceStartTime = simTime();
        selfTrack.x(2) = selfTrack.x(5) = 0;
        changePsynm();
        delete msg;
    }
    else {
        selfTrack.update(bcn, curT++);
        if (ctrlMsg)
            BasePrivLayer::handleUpperControl(msg);
        else
            BasePrivLayer::handleUpperMsg(msg);

    }

}

void CAPS::handleUpperMsg(cMessage *msg) {
    WAVEBeacon * bcn = dynamic_cast<WAVEBeacon*>(msg);
    if (!bcn)
        BasePrivLayer::handleUpperMsg(msg);
    else {
        if (bSilent)
            handleUpperMsgSilent(bcn, false);
        else
            handleUpperMsgActive(bcn, false);
    }
}

void CAPS::handleUpperControl(cMessage *msg) {
    WAVEBeacon * bcn = dynamic_cast<WAVEBeacon*>(msg);
    if (!bcn)
        BasePrivLayer::handleUpperControl(msg);
    else {
        if (bSilent)
            handleUpperMsgSilent(bcn, true);
        else
            handleUpperMsgActive(bcn, true);
    }
 }

bcnSample CAPS::msg2BcnSample(WAVEBeacon * bsm) {
    bcnSample bcn;
    bcn.timestampe = bsm->getTimestamp().dbl();
    bcn.vehId = bsm->getSenderAddress();
    bcn.psynm = bsm->getSenderPsynm();

    bcn.px = bsm->getSenderPos().x;
    bcn.vx = bsm->getSenderVel().x;
    bcn.py = bsm->getSenderPos().y;
    bcn.vy = bsm->getSenderVel().y;
    bcn.angle = bsm->getSenderAngle();
    bcn.bEncrypted = bsm->getIsEncrypted();

    return bcn;
}
