#include <omnetpp.h>
#include "AppMessage_m.h"
#include "MacMessage_m.h"
#include <iostream>
#include <fstream>

#define BYTE_TO_BITS 8

using namespace omnetpp;


/* Generates an infinite amount of application layer messages with an interarrival time that is set
 * as a parameter. These messages are sent to the MAC module of the same node.
 */

class PacketGenerator : public cSimpleModule
{
    private:
        int seqno;
        int messageSize; // size in bytes
        double iatDistribution;
        bool channelTestFlag;
        std::fstream logFile;

    public:
        PacketGenerator() : cSimpleModule() {}
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void finish();
};

Define_Module(PacketGenerator);

void PacketGenerator::initialize()
{
    seqno = 0;
    messageSize = par("messageSize");
    iatDistribution = par("iatDistribution");
    channelTestFlag = par("channelTestFlag");
    scheduleAt(simTime()+exponential(iatDistribution), new cMessage);
}

void PacketGenerator::handleMessage(cMessage *msg)
{
    // Any messages from the MAC module shall be dropped.
    if (dynamic_cast<AppMessage *>(msg) != nullptr) {
        AppMessage* amsg = check_and_cast<AppMessage *>(msg);
        drop(amsg);
        delete amsg;
        return;
    }

    // Generate a new AppMessage.
    EV << "PacketGenerator: making message " << seqno+1 <<endl;
    AppMessage *omsg = new AppMessage();
    omsg->setSenderID(getParentModule()->par("nodeIdentifier"));
    omsg->setTimestamp(simTime());
    omsg->setSequenceNumber(seqno);
    omsg->setMsgSize(messageSize);
    omsg->setBitLength(messageSize*BYTE_TO_BITS); // Set for convenience
    send(omsg, "localOut"); // Send packet to MAC
    seqno += 1; // Increase sequence number

    // Next packet creation times to be drawn from a random distribution.
    scheduleAt(simTime()+exponential(iatDistribution), msg);
}

void PacketGenerator::finish()
{
    if(channelTestFlag)
    {
        int id = getParentModule()->par("nodeIdentifier");
        std::string fileName = par("packetGenfileName");
        fileName += std::to_string(id) + ".txt";
        EV << "fileName: " << fileName << endl;
        logFile.open(fileName, std::fstream::out);
        char header[50];
        sprintf(header, "Packet Send Total: %10d\n", seqno);
        logFile << header;
        logFile.close();
    }
}
