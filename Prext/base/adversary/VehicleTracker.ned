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

package org.car2x.veins.modules.Prext.base.adversary;

simple VehicleTracker
{
    parameters:
        //Tracker module parameters
        double eavesdropperRange @unit(m) = default(300m);
        double eavesdropperRangeOverlap @unit(m) = default(30m);
        bool fullCoverage = default(true);
        bool exportEncrypted  = default(false);
        bool exportBeacons  = default(true);
        //string exportFilePath = default("BeaconsExport"); //path + name without extension
        bool enableTracking = default(true);        
        bool matlabfileformat = default(false);
        
        //Tracking algorithm parameters 
        double kalman_p0 = default(50);
        double kalman_q = default(1);
        double kalman_rp = default(5);
        double kalman_rv = default(1);                
        double gateSize = default(30);
        double anonomitySetPthreshold = default(0.01);
    	
    	//important parameters that should be carefully assigned
        double waitBfrDelete @unit(s) = default(2s);    	
    	double trackInterval @unit(s);
        double maxSilence @unit(s);
        
    	@signal[vId](type=unsigned long);
    	@signal[maxEnt](type=double);
    	@signal[sumEnt](type=double);
    	@signal[maxAas](type=unsigned long);
    	@signal[conf](type=unsigned long);
    	@signal[psynmchng](type=unsigned long);
    	@signal[contTrackTimePer](type=double); //continous tracking time
    	@signal[monitorTime](type=double); // time difference between the last and first times a vehicle is seen by an eavesdropper
    	  
    	@statistic[vehicleId](source=vId;title="vehicleId";record=vector);
    	@statistic[MonitorTime](source=monitorTime;title="MonitorTime";record=vector);
    	@statistic[maxEntropy](source=maxEnt;title="maximumEntropy";record=vector);
    	@statistic[sumEntropy](source=sumEnt;title="sumEntropy";record=vector);
    	@statistic[maxAnonymitySetSize](source=maxAas;title="maximumAnonymitySetSize";record=vector);
    	@statistic[ContTrackingTimePer](source=contTrackTimePer;title="ContinousTrackingTimePercentage";record=vector);
    	@statistic[nTrackerConfusion](source=conf;title="nTrackerConfusion";record=histogram);
    	@statistic[nPsynmChange](source=psynmchng;title="nPsynmChange";record=histogram);
    	
    	
    	
}
