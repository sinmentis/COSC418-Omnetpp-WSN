packet AppMessage
{
    simtime_t timestamp; // Simulation time at which the message has been generated.
    int senderID; // Node identification of the sender TransmitterNode.
    int sequenceNumber; // How many messages were before this one.
    int msgSize; // Indicates message size in bytes.
}