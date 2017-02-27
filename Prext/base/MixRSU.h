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

#ifndef MIXRSU_H
#define MIXRSU_H

#include <veins/base/modules/BaseApplLayer.h>
#include <veins/modules/Prext/base/messages/MixZoneAd_m.h>



/**
 * Mix-zone RSU
 */
class MixRSU : public BaseApplLayer {
	public:
        ~MixRSU();
		virtual void initialize(int stage);

	protected:
        virtual void handleLowerMsg(cMessage * msg);
        virtual void handleLowerControl(cMessage * msg);

		virtual void handleSelfMsg(cMessage* msg);

        MixZoneAd* prepareAdvertise();
        vector<Convex> parseConvexPolygon(string strPolygon);

        int headerLength;
        int dataLength;
        //double advertiseInterval;

        simtime_t individualOffset;
        int myId;

        cMessage* sendAdvertiseEvt = 0;
        MixZoneAd* localAd = 0;
};

#endif
