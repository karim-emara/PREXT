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
import org.car2x.veins.base.modules.IBaseApplLayer;

simple MixRSU like IBaseApplLayer
{
    parameters: 
        @display("i=block/app2");
        int headerLength @unit("bit");         
        int dataLength @unit("byte"); // length of the message data in bytes
        double advertiseInterval @unit("s");
        
        int zoneType; // 1 circular, 2 convex polygon
        int zoneCircularRange @unit("m");
        string zoneConvexPolygons; // x1, y1; x2, y2; x3, y3; | x4, y4; x5, y5; x6, y6; x7, y7;
          
    gates:
        input lowerLayerIn; // from network layer
        output lowerLayerOut; // to network layer
        input lowerControlIn; // control from network layer
        output lowerControlOut; // control to network layer     

}
