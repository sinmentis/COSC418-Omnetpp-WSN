package cosc418;

network Network
{
    parameters:
        receNode.nodeXPosition = 0;
        receNode.nodeYPosition = 0;
        receNode.transceiver.recordStats = true;

        tranNode1.nodeIdentifier = 1;
        tranNode1.nodeXPosition = 20;
        tranNode1.nodeYPosition = 0;

        tranNode2.nodeIdentifier = 2;
        tranNode2.nodeXPosition = 18.79;
        tranNode2.nodeYPosition = 6.84;

        tranNode3.nodeIdentifier = 3;
        tranNode3.nodeXPosition = 15.32;
        tranNode3.nodeYPosition = 12.86;

        tranNode4.nodeIdentifier = 4;
        tranNode4.nodeXPosition = 10;
        tranNode4.nodeYPosition = 17.32;

        tranNode5.nodeIdentifier = 5;
        tranNode5.nodeXPosition = 3.47;
        tranNode5.nodeYPosition = 19.7;

        tranNode6.nodeIdentifier = 6;
        tranNode6.nodeXPosition = -3.47;
        tranNode6.nodeYPosition = 19.7;

        tranNode7.nodeIdentifier = 7;
        tranNode7.nodeXPosition = -10;
        tranNode7.nodeYPosition = 17.32;

        tranNode8.nodeIdentifier = 8;
        tranNode8.nodeXPosition = -15.32;
        tranNode8.nodeYPosition = 12.86;

        tranNode9.nodeIdentifier = 9;
        tranNode9.nodeXPosition = -18.79;
        tranNode9.nodeYPosition = 6.84;

        tranNode10.nodeIdentifier = 10;
        tranNode10.nodeXPosition = -20;
        tranNode10.nodeYPosition = 0;
        
        tranNode11.nodeIdentifier = 11;
        tranNode11.nodeXPosition = 18.79;
        tranNode11.nodeYPosition = -6.84;

        tranNode12.nodeIdentifier = 12;
        tranNode12.nodeXPosition = 15.32;
        tranNode12.nodeYPosition = -12.86;

        tranNode13.nodeIdentifier = 13;
        tranNode13.nodeXPosition = 10;
        tranNode13.nodeYPosition = -17.32;

        tranNode14.nodeIdentifier = 14;
        tranNode14.nodeXPosition = 3.47;
        tranNode14.nodeYPosition = -19.7;

        tranNode15.nodeIdentifier = 15;
        tranNode15.nodeXPosition = -3.47;
        tranNode15.nodeYPosition = -19.7;

        tranNode16.nodeIdentifier = 16;
        tranNode16.nodeXPosition = -10;
        tranNode16.nodeYPosition = -17.32;

        tranNode17.nodeIdentifier = 17;
        tranNode17.nodeXPosition = -15.32;
        tranNode17.nodeYPosition = -12.86;

        tranNode18.nodeIdentifier = 18;
        tranNode18.nodeXPosition = -18.79;
        tranNode18.nodeYPosition = -6.84;
//
//        tranNode19.nodeIdentifier = 19;
//        tranNode19.nodeXPosition = 16.18;
//        tranNode19.nodeYPosition = -11.776;
//
//        tranNode20.nodeIdentifier = 20;
//        tranNode20.nodeXPosition = 19.02;
//        tranNode20.nodeYPosition = -6.18;


    submodules:
        channel1: Channel;
        receNode: ReceiverNode;
        tranNode1: TransmitterNode;
        tranNode2: TransmitterNode;
        tranNode3: TransmitterNode;
        tranNode4: TransmitterNode;
        tranNode5: TransmitterNode;
        tranNode6: TransmitterNode;
        tranNode7: TransmitterNode;
        tranNode8: TransmitterNode;
        tranNode9: TransmitterNode;
        tranNode10: TransmitterNode;
        tranNode11: TransmitterNode;
        tranNode12: TransmitterNode;
        tranNode13: TransmitterNode;
        tranNode14: TransmitterNode;
        tranNode15: TransmitterNode;
        tranNode16: TransmitterNode;
        tranNode17: TransmitterNode;
        tranNode18: TransmitterNode;
//        tranNode19: TransmitterNode;
//        tranNode20: TransmitterNode;

    connections:
        receNode.out --> channel1.in++;
        channel1.out++ --> receNode.in;
        tranNode1.out --> channel1.in++;
        channel1.out++ --> tranNode1.in;
        tranNode2.out --> channel1.in++;
        channel1.out++ --> tranNode2.in;
        tranNode3.out --> channel1.in++;
        channel1.out++ --> tranNode3.in;
        tranNode4.out --> channel1.in++;
        channel1.out++ --> tranNode4.in;
        tranNode5.out --> channel1.in++;
        channel1.out++ --> tranNode5.in;
        tranNode6.out --> channel1.in++;
        channel1.out++ --> tranNode6.in;
        tranNode7.out --> channel1.in++;
        channel1.out++ --> tranNode7.in;
        tranNode8.out --> channel1.in++;
        channel1.out++ --> tranNode8.in;
        tranNode9.out --> channel1.in++;
        channel1.out++ --> tranNode9.in;
        tranNode10.out --> channel1.in++;
        channel1.out++ --> tranNode10.in;
        tranNode11.out --> channel1.in++;
        channel1.out++ --> tranNode11.in;
        tranNode12.out --> channel1.in++;
        channel1.out++ --> tranNode12.in;
        tranNode13.out --> channel1.in++;
        channel1.out++ --> tranNode13.in;
        tranNode14.out --> channel1.in++;
        channel1.out++ --> tranNode14.in;
        tranNode15.out --> channel1.in++;
        channel1.out++ --> tranNode15.in;
        tranNode16.out --> channel1.in++;
        channel1.out++ --> tranNode16.in;
        tranNode17.out --> channel1.in++;
        channel1.out++ --> tranNode17.in;
        tranNode18.out --> channel1.in++;
        channel1.out++ --> tranNode18.in;
//        tranNode19.out --> channel1.in++;
//        channel1.out++ --> tranNode19.in;
//        tranNode20.out --> channel1.in++;
//        channel1.out++ --> tranNode20.in;

}
