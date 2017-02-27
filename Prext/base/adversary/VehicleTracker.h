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

#ifndef __VEINS_VEHICLETRACKER_H_
#define __VEINS_VEHICLETRACKER_H_

#include <omnetpp.h>
#include <veins/modules/Prext/base/messages/WAVEBeacon_m.h>
#include <veins/modules/Prext/base/PrivBase.h>
#include <map>
#include <fstream>
#include <vector>
#include <NNPDA.h>
#include <ctime>
using namespace std;

class VehicleTracker : public cSimpleModule
{

  protected:
    void removeEncryptedBeacons();
    bcnSample msg2BcnSample(WAVEBeacon * bsm);

    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
    virtual void exportWAVEBeacon() ;
    virtual void exportWAVEBeaconMat() ;
    void createFullCoverageEavesdropper();
    int timestepIdx;
    map<long, simtime_t> psynmBSMTimeMap;

    ofstream outfile;
    bool exportEncrypted;
    bool exportBeacons;
    bool enableTracking;
    bool matlabfileformat;
    double trackInterval;
    cMessage * trackBSMEvt = 0;
    vector<cModule *> dynmEavesdropper;

    NNPDA tracker;
    map<uint64_t, bcnSample> rcvdBcns;

    simsignal_t sVId;
    simsignal_t sMaxEnt;
    simsignal_t sSumEnt;
    simsignal_t sMaxAas;
    simsignal_t sConf;
    simsignal_t sPsynmChng;
    simsignal_t sContTrackTime;
    simsignal_t sMonitorTime;

    cOutVector outNVeh;
    clock_t trackerInitTime;
  public:
    virtual void beaconReceived(WAVEBeacon * bsm, int EavesdropperId);

};

#endif
