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
    lastSentTime[0]=simTime();
    lastSentTime[1]=simTime();
    lastSentTime[2]=simTime();

}


void Scheduler::handleMessage(cMessage *msg)
{
    cModule *hpqModule = getModuleByPath("Network.hpq");
    cModule *mpqModule = getModuleByPath("Network.mpq");
    cModule *lpqModule = getModuleByPath("Network.lpq");
    int qLength[3];

    if (hpqModule && mpqModule && lpqModule) {
        qLength[0] = check_and_cast<MyQ *>(hpqModule)->getQLength();
        qLength[1] = check_and_cast<MyQ *>(mpqModule)->getQLength();
        qLength[2] = check_and_cast<MyQ *>(lpqModule)->getQLength();

        // Use retrieved parameters as needed
        EV << "hpq Length: " << qLength[0] << endl;
        EV << "mpq Length: " << qLength[1] << endl;
        EV << "lpq Length: " << qLength[2] << endl;

        // Your logic using these parameters
    } else {
        EV << "Error: One or more queues not found!" << endl;
        // Handle error condition (if needed)
    }

    int toServe;
    int userWeights[3]={4,2,1};

    if (msg == selfMsg){
        cMessage *cmd = new cMessage("cmd");
        toServe=auctionByTime(lastSentTime,3,qLength,userWeights);
        if(toServe!=-1)
            send(cmd,"txScheduling",toServe);

        scheduleAt(simTime()+par("schedulingPeriod").doubleValue(), selfMsg);



        //unde initializam dimensiunea pachetului?
        //trebuie schimbat timpul de schedule ca sa se activeze din nou schedulerul dupa ce se trimite pachetul curent?
        //la comparatia din scheduler trebuie comparate intervale de timp sau timp gen simtime?



        }
}

int Scheduler::auctionByTime(simtime_t* lastSentTime,int size,int *qLength,int *userWeights){
    EV<<"Simulation time:"<<simTime()<<endl;
    EV<<"Scheduler chooses between: "<<lastSentTime[0]<<"      "<<lastSentTime[1]<<"      "<<lastSentTime[2]<<"      ";
    simtime_t max=0;
    int maxUser=-1;
    for(int i=0;i<size;i++){
        if((simTime()-lastSentTime[i])*userWeights[i]>max && qLength[i]!=0)
            {
            max=(simTime()-lastSentTime[i])*userWeights[i];
            maxUser=i;
            }
    }
    if(maxUser!=-1)
        lastSentTime[maxUser]=simTime();
    EV<<"Winner:"<<maxUser;
    return maxUser;
}




