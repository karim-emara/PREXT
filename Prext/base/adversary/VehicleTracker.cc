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

#include <cmodule.h>
#include "VehicleTracker.h"
Define_Module(VehicleTracker);

void VehicleTracker::initialize()
{
    trackerInitTime = clock();
    outNVeh.setName("nVeh");

    enableTracking = par("enableTracking").boolValue();
    exportEncrypted = par("exportEncrypted").boolValue();
    exportBeacons = par("exportBeacons").boolValue();
    matlabfileformat = par("matlabfileformat").boolValue();
    trackInterval = par("trackInterval").doubleValue();
    timestepIdx = 1;
    if (par("fullCoverage").boolValue())
        createFullCoverageEavesdropper();

    string strfilename = "BeaconsExport"; //par("exportFilePath").stringValue();
    if (exportBeacons) {
        if (matlabfileformat)
            strfilename += ".m";
        else
            strfilename += ".txt";
        outfile.open(strfilename ,ios::out);
        outfile.setf(ios::fixed, ios::floatfield);
        outfile.precision(4);
    }
    if (enableTracking) {
        double gateSize = par("gateSize").doubleValue();
        unsigned int waitBfrDelete = ceil(par("waitBfrDelete").doubleValue()/trackInterval);
        unsigned int maxSilence = ceil(par("maxSilence").doubleValue()/trackInterval);
        double anonomitySetPthreshold = par("anonomitySetPthreshold").doubleValue();
        kalmanTrack::initKalmanParam(trackInterval, par("kalman_p0").doubleValue(), par("kalman_q").doubleValue(), par("kalman_rp").doubleValue(), par("kalman_rv").doubleValue());
        tracker.initNNPDAParam(gateSize, waitBfrDelete, maxSilence);
        tracker.enableEvaluation(anonomitySetPthreshold);

        sVId = registerSignal("vId");
        sMaxEnt = registerSignal("maxEnt");
        sSumEnt = registerSignal("sumEnt");
        sMaxAas = registerSignal("maxAas");
        sConf = registerSignal("conf");
        sPsynmChng = registerSignal("psynmchng");
        sContTrackTime = registerSignal("contTrackTimePer");
        sMonitorTime = registerSignal("monitorTime");

    }
    trackBSMEvt = new cMessage("TRK", PrivLayerMessageKinds::TRACK_BSM_EVT);
    scheduleAt(simTime() + trackInterval, trackBSMEvt);

}
void VehicleTracker::createFullCoverageEavesdropper() {
    double playgroundSizeX = getParentModule()->par("playgroundSizeX");
    double playgroundSizeY = getParentModule()->par("playgroundSizeY");
    double eavesdropperRange = par("eavesdropperRange");
    double w = par("eavesdropperRangeOverlap");

    int nex = playgroundSizeX/(2*(eavesdropperRange - w));
    int ney = playgroundSizeY/(2*(eavesdropperRange - w));

    int nodeVectorIndex = 0;
    cModule* parentmod = getParentModule();
    cModuleType* nodeType = cModuleType::get("org.car2x.veins.nodes.RSU");

    for (int i = 0; i <= nex; i++) {
        if ((eavesdropperRange - w)*(2*i+1) > playgroundSizeX)
            break;
        for (int j = 0; j <= ney; j++) {
            if ((eavesdropperRange - w)*(2*j+1) > playgroundSizeY)
               break;
            cModule* mod = nodeType->create("eavesdropper", parentmod, nodeVectorIndex, nodeVectorIndex);
            mod->finalizeParameters();
            mod->buildInside();
            mod->getSubmodule("mobility")->par("x") = (eavesdropperRange - w)*(2*i+1);
            mod->getSubmodule("mobility")->par("y") = (eavesdropperRange - w)*(2*j+1);

            cDisplayString& dispStr = mod->getDisplayString();
            stringstream  str;
            str << "p=" << mod->getSubmodule("mobility")->par("x").doubleValue() << "," << mod->getSubmodule("mobility")->par("y").doubleValue()
                    << ";b=10,10,oval;r= " << eavesdropperRange;
            dispStr.parse(str.str().c_str());

            mod->scheduleStart(simTime());
            dynmEavesdropper.push_back(mod);
            nodeVectorIndex++;
        }
    }
}

void VehicleTracker::finish() {
    cancelAndDelete(trackBSMEvt);
    recordParametersAsScalars();

    clock_t elapsedtime = double(clock() - trackerInitTime)/CLOCKS_PER_SEC;
    recordScalar("simRunningTime", elapsedtime );

    for (unsigned int i = 0; i<dynmEavesdropper.size();i++) {
        dynmEavesdropper[i]->callFinish();
        dynmEavesdropper[i]->deleteModule();
    }

    if (exportBeacons)
        outfile.close();

    if (enableTracking && tracker.getCurTimestep() > 0) {
        trackerInitTime = clock();
        tracker.finalizeEvaluation();

        double trace90 = 0;
        double ntrace90 = 0;
        double nPsynmChng = 0;
        double nConfusion = 0;
        double meanMaxASS = 0;
        double meanMaxEntropy = 0;
        double meanSumEntropy = 0;
        unsigned int nVeh = tracker.vehEvaluation.size();
        unsigned int nChngPsynmVeh = 0;

        map<unsigned int, EvaluationMetric>::iterator it = tracker.vehEvaluation.begin();
        for (; it != tracker.vehEvaluation.end(); it++) {
            if ((double)it->second.bestTrackLifetime / (it->second.vehEndTime - it->second.vehStartTime + 1) > .9)
                trace90++;
            if (it->second.nPsynmChange > 0) {
                nChngPsynmVeh++;
                if ((double)it->second.bestTrackLifetime / (it->second.vehEndTime - it->second.vehStartTime + 1) > .9)
                    ntrace90++;
            }
            nPsynmChng+= it->second.nPsynmChange;
            nConfusion += it->second.nConfusion;
            meanMaxEntropy += it->second.psynmChngMaxEntropy;
            meanSumEntropy += it->second.psynmChngSumEntropy;
            meanMaxASS += it->second.psynmChngMaxASS;

            emit(sVId, (unsigned int) it->first);
            emit(sMaxEnt, it->second.psynmChngMaxEntropy);
            emit(sSumEnt, it->second.psynmChngSumEntropy);
            emit(sMaxAas, it->second.psynmChngMaxASS);
            emit(sConf, it->second.nConfusion);
            emit(sPsynmChng, it->second.nPsynmChange);
            emit(sContTrackTime, ((double)it->second.bestTrackLifetime)/(it->second.vehEndTime - it->second.vehStartTime + 1)*100.0);
            emit(sMonitorTime, (it->second.vehEndTime - it->second.vehStartTime + 1)*trackInterval);

        }
        recordScalar("nTraces", nVeh);
        recordScalar("nTracesChngPsynms", nChngPsynmVeh);
        recordScalar("Traceability90", trace90/nVeh*100);
        recordScalar("N_Traceability90", ntrace90/nChngPsynmVeh*100);
        recordScalar("max AS size per trace", meanMaxASS/nVeh);
        recordScalar("max entropy per trace", meanMaxEntropy/nVeh);
        recordScalar("Pseudonym change per trace", nPsynmChng/nVeh);
        recordScalar("Confusions per pseudonym change",  nConfusion/nPsynmChng);
        recordScalar("Confusions per trace",  nConfusion/nVeh);
        recordScalar("trackerEvalTime", double(clock() - trackerInitTime)/CLOCKS_PER_SEC);

        EV   << "Elapsed Time: " << elapsedtime << endl
            << "Evaluation Time: " << double(clock() - trackerInitTime)/CLOCKS_PER_SEC << endl
            << "Number of traces: " << nVeh << endl
             << "Number of traces that changed pseudonyms: " << nChngPsynmVeh << endl
             << "Traceability %90: " << trace90/nVeh*100 << endl
             << "Normalized traceability %90: " << ntrace90/nChngPsynmVeh*100 << endl
             << "Average max AS size per trace: " << meanMaxASS/nVeh << endl
             << "Average max entropy per trace: " << meanMaxEntropy/nVeh << endl
             << "Average sum entropy per trace: " << meanSumEntropy/nVeh << endl
             << "Average Pseudonym change per trace: " << nPsynmChng/nVeh << endl
             << "Average Confusions per pseudonym change: " << nConfusion/nPsynmChng << endl
             << "Average Confusions per trace: " << nConfusion/nVeh << endl;
    }


}
void VehicleTracker::removeEncryptedBeacons() {
    map<uint64_t, bcnSample>::iterator it = rcvdBcns.begin();
    while (it != rcvdBcns.end())
        if (it->second.bEncrypted)
            it = rcvdBcns.erase(it);
        else
            it++;

}
void VehicleTracker::exportWAVEBeacon() {
   map<uint64_t, bcnSample>::iterator it = rcvdBcns.begin();
   for (; it != rcvdBcns.end(); it++)
       if (!it->second.bEncrypted || (exportEncrypted && it->second.bEncrypted))
       outfile << it->second.timestampe << "\t"
               << it->second.vehId << "\t"
               << it->second.px << "\t"
               << it->second.vx << "\t"
               << it->second.py << "\t"
               << it->second.vy << "\t"
               << it->second.psynm << "\t"
               << it->second.angle << "\t"
               << it->second.bEncrypted << endl;
}

void VehicleTracker::exportWAVEBeaconMat() {
   map<uint64_t, bcnSample>::iterator it = rcvdBcns.begin();
   outfile << "veinsdata{" << timestepIdx++ << "} = [" << endl;
   for (; it != rcvdBcns.end(); it++)
       if (!it->second.bEncrypted || (exportEncrypted && it->second.bEncrypted))
           outfile << it->second.vehId << "\t"
                   << it->second.px << "\t"
                   << it->second.vx << "\t"
                   << it->second.py << "\t"
                   << it->second.vy << "\t"
                   << it->second.psynm << "\t"
                   << it->second.angle << ";" << endl;
   outfile << "];" << endl;
}

void VehicleTracker::beaconReceived(WAVEBeacon * bsm, int EavesdropperId) {

    Enter_Method("V:%d, R:%d", bsm->getSenderAddress(), EavesdropperId);
    //only consider WAVE_Beacon messages
    if (bsm->getKind() != PrivateApplMessageKinds::WAVE_BEACON )
        return;

    map<long, simtime_t>::iterator it = psynmBSMTimeMap.find(bsm->getSenderPsynm());
    if (it == psynmBSMTimeMap.end() || it->second < bsm->getTimestamp()) {
        psynmBSMTimeMap[bsm->getSenderPsynm()] = bsm->getTimestamp();

        // collect vehicle state from new BSMs
        rcvdBcns[bsm->getSenderPsynm()] = msg2BcnSample(bsm);
    }
    // Ignore repeated beacons of the same pseudonym
    else if (it->second == bsm->getTimestamp())
        return;
    // Tracker shouldn't receive a delayed beacon older than current time stamp (i.e., it->second  > bsm->getTimestamp())
    // (assuming delay won't exceed the beacon interval anyway)
    else
        // Oops, a delayed beacon received
        EV << "Tracker: Delayed beacon received from Eavesdropper: " << EavesdropperId <<
        " for vehicle: " << bsm->getSenderAddress() << " at time: " << simTime() << endl;

}
void VehicleTracker::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
        switch (msg->getKind()) {
            case PrivLayerMessageKinds::TRACK_BSM_EVT: {
                outNVeh.record(rcvdBcns.size());
                if (exportBeacons) {
                    if (matlabfileformat)
                        exportWAVEBeaconMat();
                    else
                        exportWAVEBeacon();
                }
                if (enableTracking) {
                    removeEncryptedBeacons();
                    tracker.trackBeacons(rcvdBcns);
                }


                rcvdBcns.clear();
                scheduleAt(simTime() + trackInterval, trackBSMEvt);
                break;
            }
            default:
                break;
        }

}


bcnSample VehicleTracker::msg2BcnSample(WAVEBeacon * bsm) {
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
