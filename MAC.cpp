#include <string.h>
#include <omnetpp.h>
#include <fstream>
#include <iostream>
#include <deque>
#include "AppMessage_m.h" // will be generate by complier from AppMessage.msg
#include "MacMessage_m.h"
#include "CSRequest_m.h"
#include "CSResponse_m.h"
#include "TransmissionConfirm_m.h"
#include "TransmissionRequest_m.h"
#include "TransmissionIndication_m.h"


#define STATUS_BUSY 0 // transmissionConfirm status
#define STATUS_OK 1 // tranmissionConfirm status

using namespace omnetpp;

/* Accepts messages to transmit from higher layers, buffers these in a queue (circular buffer) of a size
 * that is set by a parameter, and executes a CSMA-type medium access control protocol to transmit these
 * messages over the channel.
 * Interacts with local transceiver from transmission and reception of packets and for carrier-sense
 * operations.
 * Any packets received from the lower layers are eventually sent to the higher layers after minor
 * processing.
 */

class MAC : public cSimpleModule
{
    private:
        int bufferSize; // parameter - size of circular buffer
        int maxBackoffs; // parameter
        double backoffDistribution; // parameter
        std::deque<AppMessage *> macBuffer; // circular buffer to store AppMessages
        int backoff; // current number of backoff attempts
        bool sentCSRequest; // the current CSRequest that is being processed
        TransmissionRequest *TransReqmsg; // the current TransmissionRequest that has been sent to the Transceiver
        std::fstream logFile;
        int bufferLoss;
        int backOffLoss;

    public:
        MAC() : cSimpleModule() {}

    protected:
        virtual void processMsgFromHigherLayer(AppMessage *msg);
        virtual void processCSResponse(CSResponse *msg);
        virtual void checkTransceiverStatus();
        virtual void processTransmissionIndication(TransmissionIndication *msg);
        virtual void processTransmissionConfirm(TransmissionConfirm *msg);
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void finish();
};

Define_Module(MAC);


void MAC::initialize()
{
    bufferSize = par("bufferSize");
    maxBackoffs = par("maxBackoffs");
    backoffDistribution = par("backoffDistribution");
    macBuffer.resize(bufferSize);//cbuf_init(&macBuffer, bufferSize);
    macBuffer.clear();
    sentCSRequest = false;
    backoff=0;
    TransReqmsg = nullptr;
    bufferLoss = 0;
    backOffLoss = 0;
    std::string fileName = par("macfileName");
    try {
        int id = getParentModule()->par("nodeIdentifier");
        fileName += std::to_string(id) + ".txt";
    } catch (...){
        fileName += ".txt";
    }
    EV << "fileName: " << fileName << endl;
    logFile.open(fileName, std::fstream::out);
}


// Write AppMessage to the circular buffer.
void MAC::processMsgFromHigherLayer(AppMessage *msg)
{
    EV << "MAC: processing AppMessage" << endl;
    if (macBuffer.size()==(size_t) bufferSize)
    {
        bufferLoss++;
        drop(msg);
        delete msg;
    }
    else
    {
        macBuffer.push_back(msg);
        EV << "MAC: have " << macBuffer.size() << " elements in buffer" << endl;
    }
    checkTransceiverStatus();
}


// Send the AppMessage in the TransmissionIndication message to the
// PacketSink (or Packet Generator)
void MAC::processTransmissionIndication(TransmissionIndication *msg)
{
    EV << "Sending packet to PacketSink (or PacketGenerator)" << endl;
    MacMessage *mmsg = check_and_cast<MacMessage *>(msg->decapsulate());
    AppMessage *amsg = check_and_cast<AppMessage *>(mmsg->decapsulate());
    drop(msg);
    delete msg;
    drop(mmsg);
    delete mmsg;
    send(amsg, "pktOut");
}


// See if the TransmissionRequest has successfully been sent.
void MAC::processTransmissionConfirm(TransmissionConfirm *msg)
{
    int msgStatus = ((TransmissionConfirm *) msg)->getStatus();
    EV << "was MacMessage successfully sent: " << msgStatus << endl;
    if (STATUS_BUSY) {
        drop(TransReqmsg);
        delete TransReqmsg;
    }
    drop(msg);
    delete msg;
    TransReqmsg = nullptr;
    checkTransceiverStatus();
}


// Sees if a new CSRequest can be sent.
void MAC::checkTransceiverStatus()
{
    // If the buffer is empty there is no need to send a message.
    if (macBuffer.empty()) {
        EV << "MAC: called checkTransceiverStatus but buffer was empty" << endl;
        return;
    }
    EV << "MAC: checkTransceiverStatus be reaching this part?" << endl;
    if(!sentCSRequest) {
        EV << "MAC: checking if transceiver is ready for message" << endl;
        CSRequest *omsg = new CSRequest();
        send(omsg, "tranOut"); //send packet to transceiver
        sentCSRequest = true;
    }
}


// Keep calling CSRequest until packet is either sent of max backoff is reached.
void MAC::processCSResponse(CSResponse *msg)
{
    EV << "MAC: processing csresponse message" << endl;
    if (!sentCSRequest) { // for the backoff part
        drop(msg);
        delete msg;
        checkTransceiverStatus();
        return;
    }
    sentCSRequest = false; // as we now have a response
    if (!msg->getBusyChannel()) {
        EV << "MAC: sending TransmissionRequest" << endl;
        backoff = 0;
        AppMessage *appmsg = macBuffer.front();
        macBuffer.pop_front();
        MacMessage *macmsg = new MacMessage();
        macmsg->encapsulate(appmsg);
        TransmissionRequest *trmsg = new TransmissionRequest();
        trmsg->encapsulate(macmsg);
        TransReqmsg = trmsg;
        send(trmsg, "tranOut");
    } else {
        backoff += 1;
        EV << "MAC: transceiver is busy, doing backoff " << backoff << endl;
        if (backoff < maxBackoffs) {
            scheduleAt(simTime()+exponential(backoffDistribution), new CSResponse());
        } else {
            EV << "MAC: max backoffs reached" << endl;
            backoff = 0;
            AppMessage *aMsg = macBuffer.front();
            macBuffer.pop_front();
            backOffLoss++;
            drop(aMsg);
            delete aMsg;
        }
    }
    drop(msg);
    delete msg;

}


// Process the incoming message based off packet type.
void MAC::handleMessage(cMessage *msg)
{
    EV << "MAC: message arrived" << endl;
    if (dynamic_cast<AppMessage *>(msg) != nullptr) {
        processMsgFromHigherLayer((AppMessage *) msg);
    } else if (dynamic_cast<CSResponse *>(msg)!= nullptr) {
        processCSResponse((CSResponse *) msg);

    } else if (dynamic_cast<TransmissionConfirm *>(msg) != nullptr) {
        processTransmissionConfirm((TransmissionConfirm *) msg);
    } else if (dynamic_cast<TransmissionIndication *>(msg) != nullptr) {
        processTransmissionIndication((TransmissionIndication *) msg);
    }
}


// Free buffer
void MAC::finish()
{
    char revBuffer[50];
    sprintf(revBuffer, "MAC Buffer Packet Loss: %d\nBack off Loss: %d\n",bufferLoss, backOffLoss);
    logFile << revBuffer;
    logFile.close();
    while(macBuffer.size() > 0)
    {
        AppMessage* Msg = macBuffer.front();
        macBuffer.pop_front();
        drop(Msg);
        delete Msg;
    }
}

