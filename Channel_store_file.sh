echo "R number?"
read number
mv Channel_Packet_Loss.txt Packet_Loss_Channel_$number.txt
mv MAC_Packet_Loss.txt Packet_Loss_MAC_$number.txt
mv PacketSink_Output.txt PacketSink_Output_Channel_$number.txt
mv PacketGen_Output.txt PacketGen_Output_Channel_$number.txt

echo "DONE"
