/*
 * MAC.h
 *
 *  Created on: 5/10/2019
 *      Author: kate
 */

#ifndef MAC_H_
#define MAC_H_

#include <omnetpp.h>
#include <deque>
#include "AppMessage_m.h"
#include "CSResponse_m.h"
#include "TransmissionIndication_m.h"
#include "TransmissionConfirm_m.h"

using namespace omnetpp;

class MAC : public cSimpleModule
{
    private:
        int bufferSize;
        int maxBackoffs;
        double backoffDistribution;
        std::deque<AppMessage *> macBuffer;
        int backoff;
        bool sentCSRequest;
        std::fstream logFile;
        int bufferLoss;
        int backOffLoss;

    public:
        virtual void processMsgFromHigherLayer(AppMessage *msg);
        virtual void processCSResponse(CSResponse *msg);
        virtual void checkTransceiverStatus();
        virtual void processTransmissionIndication(TransmissionIndication *msg);
        virtual void processTransmissionConfirm(TransmissionConfirm *msg);
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;
};



#endif /* MAC_H_ */
