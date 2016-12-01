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

package org.car2x.veins.modules.Prext.base;

import org.car2x.veins.base.modules.*;
import org.car2x.veins.modules.nic.Nic80211p; 

module PrivCar
{
    parameters:
        string applType; //type of the application layer
        string nicType = default("Nic80211p"); // type of network interface card
        string veinsmobilityType; //type of the mobility module
        string privType = default("BasePrivLayer");
    gates:
        input veinsradioIn; // gate for sendDirect
    submodules:
        appl: <applType> like org.car2x.veins.base.modules.IBaseApplLayer {
            parameters:
                @display("p=60,50");
        }
		priv: <privType> like org.car2x.veins.modules.Prext.base.IBasePrivLayer {
            parameters:
                @display("p=60,100");
        }
        
        nic: <nicType> like org.car2x.veins.modules.nic.INic80211p {
            parameters:
                @display("p=60,166");
        }

        veinsmobility: <veinsmobilityType> like org.car2x.veins.base.modules.IMobility {
            parameters:
                @display("p=130,172;i=block/cogwheel");
        }
        
    connections:
        nic.upperLayerOut --> priv.lowerLayerIn;
        nic.upperLayerIn <-- priv.lowerLayerOut;
        nic.upperControlOut --> priv.lowerControlIn;
        nic.upperControlIn <-- priv.lowerControlOut;
        
        priv.upperLayerOut --> appl.lowerLayerIn;
        priv.upperLayerIn <-- appl.lowerLayerOut;
        priv.upperControlOut --> appl.lowerControlIn;
        priv.upperControlIn <-- appl.lowerControlOut;
        
        veinsradioIn --> nic.radioIn;

}
