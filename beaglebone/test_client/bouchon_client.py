# -*- coding: utf-8 -*-
import socket, time, struct

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('localhost', 1337))

pdata = struct.pack ("BBBBBBBiiii", 1, 24, 2, 0,0,0,0, 11,22,33,44)
print client.send(pdata), 'bytes sent.'
time.sleep(0.2)
print 'Received message:', client.recv(1024)

pdata = struct.pack ("BBBBBfff", 1, 20, 1, 0,0, 1.1,2.2,3.3)
print client.send(pdata), 'bytes sent.'
time.sleep(0.2)
print 'Received message:', client.recv(1024)
