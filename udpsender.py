#!/usr/bin/python2
import socket

###########
# IP
# IP of client
IP = "127.0.0.1"
#IP = "131.130.45.30"
#IP = "131.130.45.13"
###########
# PORT
# port client listens to
PORT=65000

WPSTR="217.80,192.00,25.40,79.10,95.80,137.10,22.80,157.50,158.90,199.40,54.40,59.00"
###########
# MSG
# known commands:
# move(str cage, str basis)
# move(str cage, double HWP, double QWP)
#
# string cage can either be "all" or the color of the corresponding cage (red,brown,...)
# string basis can be "hv","pm","lr"
# double HWP and QWP are the respective angles in degrees (e.g. 120,60)
#MSG = "move(red,120,60)"
#MSG = "move(all,hv)"
#MSG = "move(brown,pm)"
#MSG = "useoffset(1)"
#MSG = "setWPangles(1,2,3,4,5,6,7,8,9,10,11,12)"
MSG = "setWPangles("+WPSTR+")"
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(MSG, (IP,PORT))
