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

#include "Scheduler.h"
#include "myQ.h"
Define_Module(Scheduler);



Scheduler::Scheduler()
{
    selfMsg = nullptr;
}

Scheduler::~Scheduler()
{
    cancelAndDelete(selfMsg);
}


void Scheduler::initialize()
{
    NrUsers = par("gateSize").intValue();
    selfMsg = new cMessage("selfMsg");
       scheduleAt(simTime(), selfMsg);

       // Access parameters from MyQ modules

}

void Scheduler::handleMessage(cMessage *msg)
{
    cModule *hpqModule = getModuleByPath("Network.hpq");
    cModule *mpqModule = getModuleByPath("Network.mpq");
    cModule *lpqModule = getModuleByPath("Network.lpq");
    int hpqLength;
    int mpqLength;
    int lpqLength;
  //  int userWeights[NrUsers];
    if (hpqModule && mpqModule && lpqModule) {
        hpqLength = check_and_cast<MyQ *>(hpqModule)->getQLength();
        mpqLength = check_and_cast<MyQ *>(hpqModule)->getQLength();
        lpqLength = check_and_cast<MyQ *>(hpqModule)->getQLength();

        // Use retrieved parameters as needed
        EV << "hpq Length: " << hpqLength << endl;
        EV << "mpq Length: " << mpqLength << endl;
        EV << "lpq Length: " << lpqLength << endl;

        // Your logic using these parameters
    } else {
        EV << "Error: One or more queues not found!" << endl;
        // Handle error condition (if needed)
    }
    int j=0;

    if (msg == selfMsg){

        while(j<(hpqLength+mpqLength+lpqLength)){
               for(int i =0;i<NrUsers;i++){
                   cMessage *cmd = new cMessage("cmd");
                   //set parameter value, e.g., nr of blocks to be sent from the queue by user i
                   send(cmd,"txScheduling",i);
               }
               j++;
               }
               scheduleAt(simTime()+par("schedulingPeriod").doubleValue(), selfMsg);

           }

}
