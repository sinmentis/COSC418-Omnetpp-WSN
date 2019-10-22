#include <string.h>
#include <omnetpp.h>
#include <iostream>
#include <fstream>

#include "AppMessage_m.h"
#include "MacMessage_m.h"

using namespace omnetpp;

/* Accepts messages of type AppMessage and prints textual information about the message to a log file.
 */

class PacketSink : public cSimpleModule
{
private:
    std::fstream logFile;
    int packet_received;

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

Define_Module(PacketSink);

void PacketSink::initialize()
{
    packet_received = 0;
    // Open a log file with the parameter name, and set the header line.
    std::string fileName = par("packetSinkfileName");
    logFile.open(fileName, std::fstream::out);
    char header[200];
    sprintf(header, "%10s%10s%10s%10s%10s\n", "Time", "timeStamp", "senderId", "seqNum", "msgSize");
    logFile << header;
}


// Write information about received packet to log file.
void PacketSink::handleMessage(cMessage *msg)
{
    packet_received++;
    EV << "PacketSink: received message " << packet_received << endl;
    AppMessage* appMsg = check_and_cast<AppMessage *>(msg);
    //char revBuffer[200];
    //sprintf(revBuffer, "%10.3f%10.3f%10d%10d%10d\n", simTime().dbl()
            //, appMsg->getTimestamp().dbl(), appMsg->getSenderID()
            //, appMsg->getSequenceNumber(), appMsg->getMsgSize());
    //logFile << revBuffer;

    drop(appMsg);
    delete appMsg;
}


// Close the log file upon simulation end.
void PacketSink::finish()
{
    char revBuffer[30];
    sprintf(revBuffer, "\n\nTotal Packet Received: %10d\n",packet_received);
    logFile << revBuffer;
    logFile.close();
}
