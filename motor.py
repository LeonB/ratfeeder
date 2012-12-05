#!/usr/bin/env python

import serial
import io
import re
import time

class Motor(object):
	COMMAND_ROTATE = 002
	COMMAND_ROTATE_STOP = 003
	COMMAND_ROTATE_CW = 004
	COMMAND_ROTATE_CCW = 005

	STATUS_UNKNOWN_COMMAND = 003

	def __init__(self):
		# dsrdtr = disable auto reset arduino
		self.ser = serial.Serial('/dev/ttyACM0', 115200, rtscts=True, dsrdtr=True)
		self.ser.setRTS(True)
		self.ser.setDTR(True)
		# self.ser.setDSR(True)
		# self.ser.xonxoff = False     #disable software flow control
		# self.ser.rtscts = False     #disable hardware (RTS/CTS) flow control
		# self.ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
		print 'RIGHT!'
		print self.ser.readline()
		# self.ser.flushInput()

	def rotate(self):
		self._send_command(self.COMMAND_ROTATE)

	def rotate_stop(self):
		self._send_command(self.COMMAND_ROTATE_STOP)

	def rotate_cw(self):
		self._send_command(self.COMMAND_ROTATE_CW)

	def rotate_ccw(self):
		self._send_command(self.COMMAND_ROTATE_CCW)

	def _send_command(self, command):
		buffer = "%s;" % command
		print buffer
		self.ser.write(buffer)
		retval = self.ser.readline()
		print "++++" + retval + "++++"

		matches = re.search('^(.*);', retval)

		if not matches or len(matches.groups()) < 2:
			raise Exception

		pieces = retval.split(',')
		print pieces
