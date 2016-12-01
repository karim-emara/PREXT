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

#ifndef PrivateSampleApp_H
#define PrivateSampleApp_H

#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include <veins/modules/utility/Consts80211p.h>
#include <veins/modules/Prext/base/messages/WAVEBeacon_m.h>
#include "veins/base/connectionManager/ChannelAccess.h"
#include <veins/modules/mac/ieee80211p/WaveAppToMac1609_4Interface.h>

using Veins::TraCIMobility;
using Veins::AnnotationManager;

/**
 * Small IVC Demo using 11p
 */
class PrivateSampleApp : public BaseApplLayer {
	public:
        ~PrivateSampleApp();
		virtual void initialize(int stage);
		virtual void finish();


	protected:
		TraCIMobility* traci;
		AnnotationManager* annotations;

        virtual void handleLowerMsg(cMessage* msg);

        virtual void handleSelfMsg(cMessage* msg);

        WAVEBeacon* prepareBeacon();
        void onBeacon(WAVEBeacon* bcn);
        void onData(WAVEBeacon* bcn);

        int headerLength;
        int dataLength;
        simtime_t individualOffset;
        int myId;

        cMessage* sendBeaconEvt = 0;

        WaveAppToMac1609_4Interface* myMac;
};

#endif
