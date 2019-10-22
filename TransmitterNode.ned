package cosc418;
// A compound module that creates AppMessages
// and sends them out to the channel.
module TransmitterNode
{
    parameters: 
        int nodeIdentifier;
        double nodeXPosition; 
        double nodeYPosition;
    gates:
    	input in;
    	output out;
    submodules:
        packetGenerator: PacketGenerator;
        mac: MAC;
        transceiver: Transceiver;
    connections: 
        packetGenerator.localOut --> mac.pktIn;
        mac.pktOut --> packetGenerator.localIn;
        mac.tranOut --> transceiver.localIn;
        transceiver.localOut --> mac.tranIn;
        transceiver.channelOut --> out;
        in --> transceiver.channelIn;
}