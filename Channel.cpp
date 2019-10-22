#include <string.h>
#include <omnetpp.h>
#include "SignalStart_m.h"
#include "SignalStop_m.h"

using namespace omnetpp;

/* Distributes packets between all attached stations.
 * Sets the pathLossExponent.
 */

class Channel : public cSimpleModule
{
    private:
        int pathLossExponent;
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

Define_Module(Channel);

void Channel::initialize()
{
    pathLossExponent = par("pathLossExponent");
}

void Channel::handleMessage(cMessage *msg)
{

    EV << "Channel: message arrived" << endl;
    if (dynamic_cast<SignalStart *>(msg) != nullptr) {
        ((SignalStart *) msg)->setPathLossExponent(pathLossExponent);
    }
    //deep copy and send to all transceiver modules
    for (int i=0; i < gateSize("out")-1;i++) {
        send(msg->dup(), "out", i);
    }
    send(msg, "out", gateSize("out")-1);
}


