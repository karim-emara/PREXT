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

package org.car2x.veins.modules.Prext.base;

import org.car2x.veins.nodes.RSU;
import org.car2x.veins.nodes.Scenario;
import org.car2x.veins.modules.Prext.base.adversary.*;
import org.car2x.veins.modules.Prext.base.*;

network PrivScenario extends Scenario
{
    parameters:
        int nEavesdropper;
        int nMXRSU; 
        @display("bgl=2;");
    submodules:
        tracker: VehicleTracker {
            @display("p=60,60;i=device/antennatower;is=l");
        }
        eavesdropper[nEavesdropper]: RSU {
            @display("p=135,113;b=10,10,oval;r=400");
        }
        MxZ[nMXRSU]: RSU {
            @display("p=233,163;r=300,lavender,,1;i=block/routing;is=s");
        }
}
