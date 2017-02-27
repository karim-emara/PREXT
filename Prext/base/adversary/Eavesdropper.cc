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

#include "Eavesdropper.h"
#include "veins/modules/Prext/base/messages/WAVEBeacon_m.h"
#include <stdio.h>
#include <veins/modules/Prext/base/PrivBase.h>
#include "veins/base/utils/FindModule.h"

Define_Module(Eavesdropper);

void Eavesdropper::initialize(int stage) {
	BaseApplLayer::initialize(stage);
	if (stage == 0) {
	    myId = getParentModule() -> getIndex();
	    dumpEncrypted = par("dumpEncrypted").boolValue();
	    localDump = par("localDump").boolValue();
	    nBcn = 0;
	    encounteredPsynms.clear();

	    tracker = FindModule<VehicleTracker*>::findSubModule(getParentModule()->getParentModule());
	    ASSERT(tracker);
	    if (localDump) {
            char filename[50];
            sprintf(filename, "Eavesdropper_%d.txt", getParentModule()->getIndex());
            outfile.open(filename,ios::out);
            outfile.setf(ios::fixed, ios::floatfield);
            outfile.precision(4);
	    }

	    sBcns = registerSignal("nbcn");
	    sPsynms = registerSignal("npsynm");
	}
}
void Eavesdropper::handleLowerMsg(cMessage* msg) {
    WAVEBeacon* bsm = dynamic_cast<WAVEBeacon*>(msg);
    if (bsm != 0) {
        // report it directly to the tracker
        // direct function call is used for efficiency
        tracker->beaconReceived(bsm, myId);

        nBcn++;
        encounteredPsynms.insert(bsm->getSenderPsynm());

        // Should I dump it locally?
        if (localDump &&
            bsm->getKind() == PrivateApplMessageKinds::WAVE_BEACON &&
            (dumpEncrypted || !bsm->getIsEncrypted()) ) {       //if beacon is encrypted, then it is supposed to be unreadable by the eavesdropper
                 dumpWAVEBeacon(outfile, bsm);
            }
    }
    delete msg ;
}
void Eavesdropper::finish() {
    if (localDump) outfile.close();
    emit(sBcns, nBcn);
    emit(sPsynms, encounteredPsynms.size());

}
