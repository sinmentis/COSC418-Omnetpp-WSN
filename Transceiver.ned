package cosc418;
simple Transceiver
{
    parameters:
        int txPowerDBm;
        int bitRate;
        int csThreshDBm;
        double noisePowerDBm;
        double turnaroundTime;
        double csTime;
        string transceiverfileName;
        bool recordStats = default(false);
    
    gates:
    	input localIn;
    	output localOut;
    	output channelOut;
    	input channelIn;
}
        
