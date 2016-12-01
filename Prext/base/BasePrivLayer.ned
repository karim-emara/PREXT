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
import org.car2x.veins.modules.Prext.base.IBasePrivLayer;

simple BasePrivLayer like IBasePrivLayer
{
    parameters:
        
    	@signal[vlt](type=double);
    	@signal[ptimes](type=double);
    	@signal[npchng](type=unsigned long);
    	@signal[nxz](type=unsigned long);
    	@signal[mxztimes](type=double);	
    	@statistic[vehicleLifeTime](source=vlt;title="vehicleLifetime";record=last);
    	@statistic[psynmTimes](source=ptimes;title="pseudonymLifetimes";record=stats,vector);
    	@statistic[nPsynmChange](source=npchng;title="nPseudonyms";record=last);
    	
    	// no of mix zones encountered over the vehicle life time regardless the vehicle engagement  
    	@statistic[nMixZone](source=nxz;title="nMixZones";record=count,vector?);  
    	
    	// time ranges a vehicle is engaged with a mix zone (i.e., beacons are encrypted during those times)  
    	@statistic[mixZoneTimes](source=mxztimes;title="mixZoneTimes";record=stats,vector);
    	
    gates:
        input lowerLayerIn;
        output lowerLayerOut;
        input upperLayerIn;
        output upperLayerOut;

        input lowerControlIn;
        output lowerControlOut;
        input upperControlIn;
        output upperControlOut;
}
