"""
This program is use to plot the pachet loss rate versus distance.

@Author: Shun Lyu
@Date: 15/Oct/2019
"""
import matplotlib.pyplot as plt

def get_gen_list():
    gen_list = []
    for filenumber in range(2,22,2):
        filename = "PacketGen_Output_Channel_{}.txt".format(filenumber)
        lines = open(filename).readlines()
        gen = int(lines[0].strip().split(":")[1]) * filenumber
        gen_list.append(gen)
    return gen_list


def get_sink_list(): 
    sink_list = []
    for filenumber in range(2,22,2):
        filename = "PacketSink_Output_Channel_{}.txt".format(filenumber)
        lines = open(filename).readlines()
        sink = int(lines[-1].strip().split(":")[1])
        sink_list.append(sink)
    return sink_list


def get_backoff_loss():
    backoff_list = []
    for filenumber in range(2,22,2):
        filename = "Packet_Loss_MAC_{}.txt".format(filenumber)
        lines = open(filename).readlines()
        backoff = int(lines[1].strip().split(":")[1])
        backoff_list.append(backoff)
    return backoff_list


def get_buffer_loss(sink_list, gen_list, backoff_list):
    buffer_list = []
    for filenumber in range(10):
        buffer_list.append(gen_list[filenumber]-sink_list[filenumber]-backoff_list[filenumber])
    return buffer_list


def get_packet_loss_rate(gen_list, sink_list):
    loss_rate_list = []
    for number in range(10):
        loss_rate_list.append((1-sink_list[number]/gen_list[number])*100)

    return loss_rate_list


def rate_plot(loss_rate_list):
    plt.figure()
    plt.plot(range(10), loss_rate_list)

    plt.ylabel("Packet Loss Rate")
    plt.xlabel("Number of transceiver")
    plt.title("Effect of transceiver number on packet loss rate")

    plt.show()


if __name__ == "__main__":
    
    # Against array size
    gen_list = get_gen_list()
    
    sink_list = get_sink_list() 
    
    backoff_loss = get_backoff_loss() 
    
    buffer_loss = get_buffer_loss(sink_list, gen_list, backoff_loss) 
    loss_packet_list = []
    for i in range(10):
        loss_packet_list.append((gen_list[i] - sink_list[i])/gen_list[i]*100)

    rate_plot(loss_packet_list)