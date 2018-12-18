#!/usr/bin/python2
import socket

###########
# IP
# IP of client
#IP = "127.0.0.1"
#IP = "131.130.45.30"
UDP_IP = "131.130.45.13"
###########
# PORT
# port client listens to
PORT=65000

###########
# MSG
# known commands:
# move(str cage, str basis)
# move(str cage, double HWP, double QWP)
#
# string cage can either be "all" or the color of the corresponding cage (red,brown,...)
# string basis can be "hv","pm","lr"
# double HWP and QWP are the respective angles in degrees (e.g. 120,60)
#MESSAGE = "move(red,120,60)"
#MESSAGE = "move(all,hv)"
MSG = "move(brown,pm)"
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(MSG, (IP,PORT))