/*
 * PacketSink.h
 *
 *  Created on: 5/10/2019
 *      Author: Shun
 */

#ifndef PACKETSINK_H_
#define PACKETSINK_H_

#include <omnetpp.h>

using namespace omnetpp;


class PacketSink : public cSimpleModule
{

    private:
        std::fstream logFile;
    public:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;
};




#endif /* PACKETSINK_H_ */
