/*
 * Channel.h
 *
 *  Created on: 5/10/2019
 *      Author: kate
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <omnetpp.h>

using namespace omnetpp;

class MAC : public cSimpleModule
{
    private:
        int pathLossExponent;
    public:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
};



#endif /* MAC_H_ */
