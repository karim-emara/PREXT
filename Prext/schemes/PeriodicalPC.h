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

#ifndef __VEINS_PeriodicalPC_H_
#define __VEINS_PeriodicalPC_H_

#include "veins/modules/Prext/base/BasePrivLayer.h"

class PeriodicalPC : public BasePrivLayer
{

public:
    PeriodicalPC() : BasePrivLayer()
    {}
    PeriodicalPC(unsigned stacksize) : BasePrivLayer(stacksize)
    {}
    virtual void initialize(int);

protected:
      cMessage* changePsynmEvt = 0;
      int minPsynmLifetime;
      int maxPsynmLifetime;
      int initPsynmLifetime;

      virtual void handleSelfMsg(cMessage* msg) ;
      virtual void finish();
};

#endif
