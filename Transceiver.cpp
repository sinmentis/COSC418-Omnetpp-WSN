#include <string.h>
#include <omnetpp.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include "CSRequest_m.h"
#include "CSResponse_m.h"
#include "SignalStart_m.h"
#include "SignalStop_m.h"
#include "MacMessage_m.h"
#include "TransmissionConfirm_m.h"
#include "TransmissionIndication_m.h"
#include "TransmissionRequest_m.h"

#define STATUS_BUSY 0 // transmissionConfirm status
#define STATUS_OK 1 // transmissionConfirm status

using namespace omnetpp;

/* Transmits packets on the channel and helps the MAC with carrier-sending.
 * Receives packets from the channel and determines whether these packets are correctly received.
 */

typedef struct {
    SignalStart *ssmsg;
    double receivedPower; // in dBm
} transmission;

class Transceiver : public cSimpleModule
{
    private:
        int txPowerDBm; // parameter - the radiated power in dBm^2
        int bitRate; // parameter - transmission bitrate in bits/s
        int csThreshDBm; // parameter - observed signal power above which the medium is considered busy
        double noisePowerDBm; // parameter - noise power
        double turnaroundTime; // parameter - time required for transceiver to transition from transmit state to receive state
        double csTime; // parameter - time required to carry out carrier sense operation
        bool csWait; // if the carrier sense operation is happening
        double nodeXPosition; // parameter from TransmitterNode
        double nodeYPosition; // parameter from TransmitterNode
        int state; // the state of the transceiver, one of the enum s options
        enum s{RECEIVE = 0, RECEIVETOTRANSMIT, TRANSMIT, TRANSMITTORECEIVE}; // transceiver states
        TransmissionRequest *currentTransmission; // message that is going to be transmitted once the transceiver is ready
        CSRequest *currentRequest; // ensures that the the wait is for the right CSRequest
        std::map<int, transmission> currentTransmissions; // dictionary of ongoing transmissions
        std::fstream logFile; // log file to record dropped packets
        int channelPacketLoss; // to count the packet loss from errors in packets
        int macrPacketLoss; // to count the packets loss from collided packets
        bool recordStats;

    public:
        Transceiver() : cSimpleModule() {}
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual double calculateCurrentSignalPower();
        virtual double getReceivedPower(SignalStart *msg);
        virtual void respondToCSRequest(CSRequest *msg);
        virtual void sendTransmissionConfirmMessage(int status);
        virtual void respondToTransmissionRequest(TransmissionRequest *tmsg);
        virtual int64_t createSignalStart();
        virtual void respondToSignalStart(SignalStart *msg);
        virtual void respondToSignalStop(SignalStop *msg);
        virtual void finish();
};

Define_Module(Transceiver);

void Transceiver::initialize()
{
    txPowerDBm = par("txPowerDBm");
    bitRate = par("bitRate");
    csThreshDBm = par("csThreshDBm");
    noisePowerDBm = par("noisePowerDBm");
    turnaroundTime = par("turnaroundTime");
    nodeXPosition = getParentModule()->par("nodeXPosition");
    nodeYPosition = getParentModule()->par("nodeYPosition");
    csTime = par("csTime");
    csWait = false;
    currentTransmission = nullptr;
    currentRequest = nullptr;
    state=RECEIVE;
    channelPacketLoss = 0;
    macrPacketLoss = 0;
    std::string fileName = par("transceiverfileName");
    try {
        int id = getParentModule()->par("nodeIdentifier");
        fileName += std::to_string(id) + ".txt";
    } catch (...){
        fileName += ".txt";
    }
    EV << "fileName: " << fileName << endl;
    logFile.open(fileName, std::fstream::out);
    recordStats = par("recordStats");
}


// Calculate the received power in dBm
double Transceiver::getReceivedPower(SignalStart *msg) {
    int transmitPower = msg -> getTransmitPowerDBm(); // Transmit power in dbm
    // Calculate transmission distance.
    double dx2 = pow(nodeXPosition - (msg->getPositionX()), 2);
    double dy2 = pow(nodeYPosition - (msg->getPositionY()), 2);
    double distance = sqrt(dx2+dy2);
    // Calculate path loss.
    double pathLoss_NL;                                 // If the distance is less than d0, no packet loss
    if (distance < 1.0) {pathLoss_NL = 1;}
    else{pathLoss_NL = pow(distance, msg->getPathLossExponent());}

    double pathLoss_DB = 10 * log10(pathLoss_NL);      // Convert to dbm
    return transmitPower - pathLoss_DB;                // Received power dbm
}


// Calculate the current signal power observed on the channel.
double Transceiver::calculateCurrentSignalPower()
{
    // Go through current transmission and add up receiver power in normal domain
    double total_power = 0.0;
    for (auto it = currentTransmissions.begin(); it != currentTransmissions.end(); it++) {
        double power_dm = it->second.receivedPower;
        total_power += pow(10, power_dm/10);          // Convert to normal domain and add up
    }
    return 10 * log10(total_power);                   // Convert to dbm
}


// Handle the CSRequest message either by starting a call
// for the current signal power or by responding with a
// CSResponse packet for the MAC.
void Transceiver::respondToCSRequest(CSRequest *msg)
{
    // If the wait for a specific CSRequest is over.
    if (csWait and msg == currentRequest) {
        EV << "Transceiver: creating response message." << endl;
        CSResponse *outMsg = new CSResponse();
        double signalPower = calculateCurrentSignalPower();
        if (state != RECEIVE or signalPower > csThreshDBm) { // can only detect signal power if transceiver state is in receive
            outMsg->setBusyChannel(true);
        } else {
            outMsg->setBusyChannel(false);
        }
        send(outMsg, "localOut");
        // Reset the CSRequest variables.
        csWait = false;
        currentRequest = nullptr;
    } else {
        if (state != RECEIVE) { // no need to wait for a carrier sense if it isn't in the right state
            CSResponse *outMsg = new CSResponse();
            outMsg->setBusyChannel(true);
            send(outMsg, "localOut");
            drop(msg);
            delete msg;
            return;
        }
        EV << "Transceiver: waiting for carrier sense to complete" << endl;
        // If there isn't currently a carrier sense request.
        if (currentRequest == nullptr) {
            currentRequest = msg;
            csWait = true;
            scheduleAt(simTime() + csTime, msg);
            return;
        }
    }
    drop(msg);
    delete msg;
}


// Send TransmissionConfirm message to MAC.
void Transceiver::sendTransmissionConfirmMessage(int status)
{
    EV << "Transceiver: sending TransmissionConfirm with status: " << status << endl;
    TransmissionConfirm *outMsg2 = new TransmissionConfirm();
    outMsg2->setStatus(status);
    send(outMsg2, "localOut");
}


// Create a SignalStart packet and send it to the channel.
int64_t Transceiver::createSignalStart()
{
    EV << "Transceiver: sending SignalStart" << endl;
    SignalStart *outMsg = new SignalStart();
    outMsg->setTransmitPowerDBm(txPowerDBm);
    outMsg->setPositionX(getParentModule()->par("nodeXPosition"));
    outMsg->setPositionY(getParentModule()->par("nodeYPosition"));
    outMsg->setIdentifier(getParentModule()->par("nodeIdentifier"));
    MacMessage * mmsg = check_and_cast<MacMessage *>(currentTransmission->decapsulate());
    outMsg->encapsulate(mmsg);
    int64_t pktLen = outMsg->getBitLength();
    send(outMsg, "channelOut");
    return pktLen;
}


// Respond to a TransmissionRequest packet, with different outcomes depending
// on what state the transceiver is in. Will create callbacks to run through
// this program to cycle through the transceiver states.
void Transceiver::respondToTransmissionRequest(TransmissionRequest *msg)
{
    EV << "Transceiver: responding to TransmissionRequest" << endl;
    if (state == RECEIVE) {
        EV << "Transceiver: changing state to receive" << endl;
        state = RECEIVETOTRANSMIT;
        currentTransmission = msg;
        scheduleAt(simTime()+turnaroundTime, msg); // callback for module to send SignalStart
    } else if (state == RECEIVETOTRANSMIT) {
        if (msg != currentTransmission) {
            sendTransmissionConfirmMessage(STATUS_BUSY);
            return;
        }
        EV << "Transceiver: send packet to channel" << endl;
        state = TRANSMIT;
        // Create SignalStart packet
        scheduleAt(simTime() + createSignalStart()/(double)bitRate, currentTransmission); // callback for module to send SignalStop
    } else if (state == TRANSMIT) {
        if (msg != currentTransmission) {
            sendTransmissionConfirmMessage(STATUS_BUSY);
            return;
        }
        EV << "Transceiver: send SignalStop to channel" << endl;
        state = TRANSMITTORECEIVE;
        // Create SignalStop message
        SignalStop *outMsg = new SignalStop();
        outMsg->setIdentifier(getParentModule()->par("nodeIdentifier"));
        send(outMsg, "channelOut");
        // Send a successful TransmissionConfirm packet
        sendTransmissionConfirmMessage(STATUS_OK);
        scheduleAt(simTime() + turnaroundTime, msg);
    } else if (state == TRANSMITTORECEIVE) {
        if (msg != currentTransmission) {
            sendTransmissionConfirmMessage(STATUS_BUSY);
            return;
        }
        drop(currentTransmission);
        delete currentTransmission;
        currentTransmission = nullptr;
        state = RECEIVE;
        EV << "Transceiver: finally ready to receive again" << endl;
    }
}


// Respond to a SignalStart packet by calculating the receive power, and then
// adding the SignalStart with the receive power to currentTransmissions.
void Transceiver::respondToSignalStart(SignalStart *msg)
{
    int id = msg->getIdentifier();
    EV << "Transceiver: processing signal start from " << id << endl;
    // Check if there is a problem with the simulation.
    if (currentTransmissions.find(id)!=currentTransmissions.end()) {
        throw cRuntimeError("A node is sending more than one message.");
    }
    // Add SignalStart to currentTransmissions
    transmission msgInfo;
    msgInfo.ssmsg = msg;
    msgInfo.receivedPower = getReceivedPower(msg);
    currentTransmissions[id] = msgInfo;
    EV << "Transceiver: packets in CurrentTransmission: " << currentTransmissions.size() << endl;
    // Check for collided packets
    if (currentTransmissions.size() > 1 or state != RECEIVE) {
        EV << "setting collided flags to true" << endl;
        for ( const auto &myPair : currentTransmissions) {
            myPair.second.ssmsg->setCollidedFlag(true);
        }
        // dropAndDelete(msg);
    }
}


// Respond to a SignalStop packet by dropping it if it has collided with
// another packet or if the packet has errors from transmission.
void Transceiver::respondToSignalStop(SignalStop *msg)
{
    int id = msg->getIdentifier();
    EV << "Transceiver: processing signal stop from " << id << endl;
    // Check if there is a problem with the simulation.
    if (currentTransmissions.find(id)==currentTransmissions.end()) {
        throw cRuntimeError("A node is sending a SignalStop without a SignalStart.");
    }
    // Processing SignalStop.
    transmission msginfo = currentTransmissions[id];
    SignalStart *smsg = msginfo.ssmsg;
    double receivedPower = msginfo.receivedPower;
    currentTransmissions.erase(id);
    if (smsg->getCollidedFlag() or state != RECEIVE) {
        channelPacketLoss+=1;
        EV << "Transceiver: packet has collided, dropping message" << endl;
        drop(msg);
        delete msg;
        drop(smsg);
        delete smsg;
        return;
    }
    // Calculate distance and packet success probability.
    MacMessage *mmsg = check_and_cast<MacMessage *>(smsg->decapsulate());    // Extract mac packet
    double bitRateDB = 10*log10(bitRate);
    double snr = pow(10, (receivedPower - (noisePowerDBm + bitRateDB))/10);  // Signal Noise Ratio in normal domain
    double bitErrorRate = erfc(sqrt(2*snr));                                 // Bit Error Rate
    int packetLen = mmsg->getBitLength();                                    // Get Packet Length
    double packetErrorRate = 1 - pow((1 - bitErrorRate), packetLen);         // Packet Error Rate
    double u = uniform(0,1);                                                 // Random number between 0-1
    if (packetErrorRate > u) {
        EV << "Transceiver: dropping message from transmission error" << endl;
        channelPacketLoss+=1;
        drop(mmsg);
        delete mmsg;
    } else {
        EV << "Transceiver: sending packet to MAC layer" << endl;
        TransmissionIndication *tmsg = new TransmissionIndication();
        tmsg->encapsulate(mmsg);
        send(tmsg, "localOut");
    }

    drop(msg);
    delete msg;
    drop(smsg);
    delete smsg;
}


// Figure out the type of incoming message to this module and then call
// the appropriate function.
void Transceiver::handleMessage(cMessage *msg)
{
    if (dynamic_cast<CSRequest *>(msg) != nullptr) {
        respondToCSRequest((CSRequest *) msg);
    } else if (dynamic_cast<TransmissionRequest *>(msg) != nullptr) {
        respondToTransmissionRequest((TransmissionRequest *) msg);
    } else if (dynamic_cast<SignalStart *>(msg) != nullptr) {
        respondToSignalStart((SignalStart *) msg);
    } else if (dynamic_cast<SignalStop *> (msg) != nullptr) {
        respondToSignalStop((SignalStop *) msg);
    } else {
        EV << "Transceiver: you have made a mistake, a message is not being processed" << endl;
    }
}


// Upon simulation end close the log file.
void Transceiver::finish()
{
    if (recordStats) {
        char revBuffer[100];
        sprintf(revBuffer, "Channel Packet Loss: %d\n",channelPacketLoss);
        logFile << revBuffer;
        logFile.close();
    }
}


