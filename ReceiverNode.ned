package cosc418;
// A compound module that receieves AppMessages and logs them.
module ReceiverNode
{
    parameters: 
        double nodeXPosition;
        double nodeYPosition;
    gates:
    	input in;
    	output out;
    submodules:
        packetSink: PacketSink;
        mac: MAC;
        transceiver: Transceiver;
    connections: 
        packetSink.localOut --> mac.pktIn;
        mac.pktOut --> packetSink.localIn;
        mac.tranOut --> transceiver.localIn;
        transceiver.localOut --> mac.tranIn;
        transceiver.channelOut --> out;
        in --> transceiver.channelIn;
}
