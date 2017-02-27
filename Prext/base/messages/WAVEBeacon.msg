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

cplusplus {{
#include <veins/base/utils/Coord.h>
#include <veins/modules/messages/WaveShortMessage_m.h>
}}

class noncobject Coord;
message WaveShortMessage;

//
// TODO generated message class
//
message WAVEBeacon extends WaveShortMessage {
    uint64_t senderPsynm;
    Coord senderVel;
    double senderAngle;
    bool CPN_readyFlag;        
    bool isEncrypted;	//message should be encrypted in mix-zone, thus eavesdropper cannot read it
}
