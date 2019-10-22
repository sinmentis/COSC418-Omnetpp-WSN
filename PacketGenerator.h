/*
 * PacketGenerator.h
 *
 *  Created on: 5/10/2019
 *      Author: Kate Olsen
 */

#ifndef PACKETGENERATOR_H_
#define PACKETGENERATOR_H_

#include <omnetpp.h>

using namespace omnetpp;


class PacketGenerator : public cSimpleModule
{
    private:
        int seqno;
        int messageSize;
        double iatDistribution;
        bool channelTestFlag;
        std::fstream logFile;
    public:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;
};




#endif /* PACKETGENERATOR_H_ */
