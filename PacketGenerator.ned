package cosc418;
simple PacketGenerator
{
	parameters:
		int messageSize;
		double iatDistribution;
		bool channelTestFlag;
		string packetGenfileName;
	gates:
	    output localOut;
	    input localIn;
}