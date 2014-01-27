#!/usr/bin/python
# -*- coding: utf-8 -*-

# fonctionnement sans install avec pip :
# downloder/builder tornado http://www.tornadoweb.org/en/stable/
# downloader/builder backports.ssl_match_hostname https://pypi.python.org/pypi/backports.ssl_match_hostname
# export PYTHONPATH=tornado-3.2/build/lib.linux-x86_64-2.6/:backports.ssl_match_hostname-3.4.0.2/build/lib/

import tornado.httpserver
import tornado.websocket
import tornado.ioloop
import tornado.web
import re
import math
import socket

import sys


return_text = 1
#botname = 'testFile'
botname = 'test'
#~ sourceMaxX = 465
#~ sourceMaxY = 780
sourceMaxX = 400
sourceMaxY = 400
diffX=400
diffY=730
config = {'test': {
				'type': 'socket',
				'socketIP': '127.0.0.1',
				'socketPort': 1337,
				'sizeX': 20,
				'sizeY': 20,
				'inverseAxes': 0,
				'trapezeFactor': 0,
				'floor': 0,
				'moveToLenght': 0,
				},
			'testFile': {
				'type': 'file',
				'file': '/tmp/gcode',
				'sizeX': 400,
				'sizeY': 400,
				'inverseAxes': 0,
				'trapezeFactor': 0,
				'floor': 0,
				'moveToLenght': 0,
				},
		}

class GCodeHGandler():
	# Parse the raw GCode and do some stuff before sending to robot
	def parseRaw(self, rawLine):
		self.raw = rawLine
		matchObj = re.match( r'.*g0x(.*)y(.*)z(.*)', rawLine, re.M|re.I)
		# exemple gcode: g0x42y24z00
		if matchObj != None:
			self.x = float(matchObj.group(1))
			self.y = float(matchObj.group(2))
			self.z = float(matchObj.group(3))
			#self.scale()
			if config[botname]['moveToLenght']:
				self.toDrawBotLength()
			if config[botname]['inverseAxes']:
				self.inverseAxes()
			if config[botname]['floor']:
				self.floor()
			self.prepareGCode()
			self.debugGCode()
			return True # gcode is valid
		else:
			return False # gcde is invalid
			
	# Prepare the GCode depending on the target (DrawBot or Reprap)
	def prepareGCode(self):
		self.gcode = 'G0X' + "%04d"%(self.x) + 'Y'+ "%04d"%(self.y) + 'Z' + "%04d"%(self.z)
	
	# inverse axes
	def inverseAxes(self):
		tmp = self.z
		self.z = self.y
		self.y = tmp

	# Floor values
	def floor(self):
		self.x = int(math.floor(self.x))
		self.y = int(math.floor(self.y))
		self.z = int(math.floor(self.z))
	
	# Prepare DrawBot length
	def toDrawBotLength(self):
		orig_x = self.x
		orig_y = self.y
		print (self.x,self.y)
		self.x = math.sqrt(math.pow(self.x,2)+math.pow(self.y,2))+diffX
		self.y = math.sqrt(math.pow(config[botname]['sizeX']-orig_x,2)+math.pow(orig_y,2))+diffY
		print (self.x,self.y)
		#if self.y > diffY:
		#	self.y -= diffY
		#else:
		#	self.y = diffY - self.y
		#print (self.x,self.y)

	# Scale the gcode to the target
	def scale(self):
		# Scale
		self.x = self.x*config[botname]['sizeX']/sourceMaxX
		self.y = self.y*config[botname]['sizeY']/sourceMaxY
		# Keep in bounds
		if self.x > config[botname]['sizeX']:
			self.x = config[botname]['sizeX']
		if self.y > config[botname]['sizeY']:
			self.y = config[botname]['sizeY']
		#~ # Projection to trapeze
		if config[botname]['trapezeFactor']:
			self.x = (self.x-config[botname]['sizeX']/2)\
						*(1+(config[botname]['trapezeFactor']-1)*self.y/config[botname]['sizeY'])\
						+config[botname]['trapezeFactor'] * config[botname]['sizeX']/2
		self.x = round(self.x,2)
		self.y = round(self.y,2)

	# Print out
	def debugGCode(self):
		print(self.gcode)

class Robot():
	# init communication with the robot
	def init(self):
		if config[botname]['type'] == 'file':
			self.f = open(config[botname]['file'], 'w')
		elif config[botname]['type'] == 'socket':
			self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.socket.connect((config[botname]['socketIP'],config[botname]['socketPort']))
	
	# send GCode and wait for ack
	def write(self, gcode_h):
		if config[botname]['type'] == 'file':
			self.f.write(gcode_h.gcode+"\n")
			self.f.flush()
			self.readLine = 'this is a file, can not read robot return'
		elif config[botname]['type'] == 'socket':
			self.socket.sendall(bytes(gcode_h.gcode+"\n", 'UTF-8'))
			self.readLine = self.socket.recv(1024).decode('UTF-8')
	
	# Close communication with the robot
	def close(self):
		if config[botname]['type'] == 'file':
			self.f.close()
		elif config[botname]['type'] == 'socket':
			self.socket.close()
	
class WSHandler(tornado.websocket.WebSocketHandler):
	# When the sebsocket is opened
	def open(self):
		print('New connection was opened')
		self.write_message("Welcome to my websocket!")
		robot.init()

	# When the websocket receive a message
	def on_message(self, message):
		print('Incoming message:', message)
		ret = gcode_h.parseRaw(message)
		if ret == True:
			robot.write(gcode_h)
			print("I send: " + gcode_h.gcode)
			print("I get: " + robot.readLine)
			# Write GCode to TTYUSB
			if return_text:
				self.write_message("You said: " + message)
				self.write_message("I send: " + gcode_h.gcode)
				self.write_message("I get: " + robot.readLine)
		else:
			if return_text:
				self.write_message("Invalid gcode")
	
	# When the websocket is closed
	def on_close(self):
		print('Connection was closed...')
		robot.close()

# MAIN
application = tornado.web.Application([
	(r'/ws', WSHandler),
])

if __name__ == "__main__":
	robot = Robot()
	gcode_h = GCodeHGandler()
	http_server = tornado.httpserver.HTTPServer(application)
	http_server.listen(22300)
	tornado.ioloop.IOLoop.instance().start()
