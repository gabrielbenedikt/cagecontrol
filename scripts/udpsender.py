#!/usr/bin/python2
import socket

###########
# IP
# IP of PC running cagecontrol
IP = "127.0.0.1"
#IP = "131.130.45.30"
#IP = "131.130.45.13"
###########
# PORT
# port client listens to
PORT=65000

###########
# MSG
# known commands:
# move(str cage, str basis)
# move(str cage, double HWP, double QWP)
# useoffset(int uo)
#
# string cage can either be "all" or the color of the corresponding cage (red,brown,...)
# string basis can be "hv","pm","lr"
# double HWP and QWP are the respective angles in degrees (e.g. 120,60)
# int uo: 	if 0: do not use waveplate reference angles as offset 
#			else: use .....
#			default: true
#MSG = "move(red,120,60)"
#MSG = "move(all,hv)"
MSG = "move(brown,pm)"
#MSG="invertbases(red,0)"
#MSG = "useoffset(1)"
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(MSG, (IP,PORT))

# If you use python 3, you need to do something like this (out of my head)
# sock.sendto(MSG.encode('utf-8', (IP,PORT)))
