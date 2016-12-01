//
// Copyright (C) 2016 Karim Emara <emara@in.tum.de>
//

#ifndef PrivBase_H
#define PrivBase_H

#include <veins/base/modules/BaseApplLayer.h>
#include <veins/modules/Prext/base/messages/WAVEBeacon_m.h>
#include <kalmanTrack.h>
#include <ostream>
using namespace std;

//TODO: implement dumpWAVEBeacon as a member function of a derived class of WAVEBeacon
inline void dumpWAVEBeacon(ostream & out, WAVEBeacon* bsm) {
    out << bsm->getTimestamp().trunc(SimTimeUnit::SIMTIME_MS) << "\t"
            << bsm->getSenderAddress() << "\t"
            << bsm->getSenderPos().x << "\t"
            << bsm->getSenderVel().x << "\t"
            << bsm->getSenderPos().y << "\t"
            << bsm->getSenderVel().y << "\t"
            << bsm->getSenderPsynm() << "\t"
            << bsm->getSenderAngle() << "\t"
            << bsm->getIsEncrypted() << endl;
}
enum PrivateApplMessageKinds {
            SERVICE_PROVIDER = BaseApplLayer::BaseApplMessageKinds::LAST_BASE_APPL_MESSAGE_KIND,
            SEND_BEACON_EVT,
            WAVE_BEACON,
            WAVE_DATA
        };

enum PrivLayerMessageKinds {
            CHANGE_PSYNM,
            EXIT_SILENCE,
            RSU_ADV_EVT,
            MIX_ZONE_AD,
            TRACK_BSM_EVT,
            OTHER

};

#endif        
