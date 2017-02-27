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

#ifndef EAVESDROPPER_H
#define EAVESDROPPER_H

#include <veins/base/modules/BaseApplLayer.h>
#include <fstream>
#include <veins/modules/Prext/base/adversary/VehicleTracker.h>
#include <set>
using namespace std;

/**
 * Eavesdropper  Demo
 */
class Eavesdropper : public BaseApplLayer {
	public:
		virtual void initialize(int stage);
		virtual void finish();
		virtual void handleLowerMsg(cMessage * msg);

	protected:
		ofstream outfile;
		bool dumpEncrypted;
		bool localDump;
		VehicleTracker * tracker;
		int myId;

		simsignal_t sBcns;
        simsignal_t sPsynms;
        set<uint64_t> encounteredPsynms;
        unsigned int nBcn;
};

#endif
