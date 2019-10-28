"""
This program is use to plot the pachet loss rate versus distance.

@Author: Shun Lyu
@Date: 15/Oct/2019
"""
import matplotlib.pyplot as plt


def get_total_packet_sent():
    total_packet_list = []
    for number in range(40):
        total_packet_list.append(99939)
    return total_packet_list


def get_packet_loss_rate():
    packet_recv_list = []
    for number in range(26):
        packet_recv_list.append(98078)
    for file_number in range(26, 40):
        filename = "PacketSink_Output_Channel_{}.txt".format(file_number+1)
        lines = open(filename).readlines()
        packet_recv = int(lines[-1].strip().split(":")[1])
        packet_recv_list.append(packet_recv)

    packet_total_list = get_total_packet_sent()
    print(packet_total_list)
    print(packet_recv_list)

    loss_rate_list = []
    for number in range(40):
        loss_rate_list.append((1-packet_recv_list[number]/packet_total_list[number])*100)

    return loss_rate_list


def rate_plot(loss_rate_list):
    plt.figure()
    plt.plot(range(1,41), loss_rate_list)

    plt.ylabel("Packet Loss Rate")
    plt.xlabel("Radius")
    plt.title("Effect of distance on packet loss rate")

    plt.show()


if __name__ == "__main__":
    # Against array size
    loss_rate_list = get_packet_loss_rate()
    rate_plot(loss_rate_list)