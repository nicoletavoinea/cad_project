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

#include "Sink.h"

Define_Module(Sink);

void Sink::initialize()
{
   // lifetimeSignal = registerSignal("lifetime");
}

void Sink::handleMessage(cMessage *msg)
{
    simtime_t lifetime = simTime() - msg->getCreationTime();
    EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;
    //  emit(lifetimeSignal, lifetime);

    if(msg->getKind()==0){//HQ
        histogram_lifetimeHQ.collect(lifetime*1000);
        vector_lifetimeHQ.record(lifetime *1000);
    }
    else if(msg->getKind()==1){//MQ
            histogram_lifetimeMQ.collect(lifetime*1000);
            vector_lifetimeMQ.record(lifetime *1000);
    }
    else if(msg->getKind()==2){//LQ
            histogram_lifetimeLQ.collect(lifetime*1000);
            vector_lifetimeLQ.record(lifetime *1000);
    }

    delete msg;
}
