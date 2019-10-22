packet SignalStart
{
    int transmitPowerDBm;
    double positionX;
    double positionY;
    int identifier;
    bool collidedFlag = false;
    int pathLossExponent;
}
