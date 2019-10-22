package cosc418;
simple MAC
{
    parameters:
        int bufferSize;
        int maxBackoffs;
        double backoffDistribution;
       	string macfileName;
       
    gates:
        input pktIn;
        output pktOut;
        input tranIn;
        output tranOut;
}