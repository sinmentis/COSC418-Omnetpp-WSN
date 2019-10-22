/*
 * Transceiver.h
 *
 *  Created on: 5/10/2019
 *      Author: kate
 */

#ifndef TRANSCEIVER_H_
#define TRANSCEIVER_H_

#include <omnetpp.h>
#include "TransmissionRequest_m.h"
#include "CSRequest_m.h"
#include "SignalStart_m.h"
#include "SignalStop_m.h"

using namespace omnetpp;

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
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual double calculateCurrentSignalPower();
        virtual double getReceivedPower(SignalStart *msg);
        virtual void respondToCSRequest(CSRequest *msg);
        virtual void sendTransmissionConfirmMessage(int status);
        virtual void respondToTransmissionRequest(TransmissionRequest *tmsg);
        virtual int64_t createSignalStart();
        virtual void respondToSignalStart(SignalStart *msg);
        virtual void respondToSignalStop(SignalStop *msg);
        virtual void finish() override;
};



#endif /* TRANSCEIVER_H_ */
