#!/usr/bin/env python

import serial
import io
import re
import time

# 1500 milis() = 1 flap

class Motor(object):
	COMMAND_ROTATE = 002
	COMMAND_ROTATE_STOP = 003
	COMMAND_ROTATE_CW = 004
	COMMAND_ROTATE_CCW = 005
	COMMAND_ROTATE_TIME = 006

	STATUS_UNKNOWN_COMMAND = 003

	def __init__(self):
		# dsrdtr = disable auto reset arduino
		self.ser = serial.Serial('/dev/ttyACM0', 115200)
		# self.ser.setRTS(True)
		# self.ser.setDTR(True)
		# self.ser.setDSR(True)
		# self.ser.xonxoff = False     #disable software flow control
		# self.ser.rtscts = False     #disable hardware (RTS/CTS) flow control
		# self.ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
		# print self.ser.readline()
		self.ser.flushInput()

	def rotate(self):
		self._send_command(self.COMMAND_ROTATE)

	def rotate_stop(self):
		self._send_command(self.COMMAND_ROTATE_STOP)

	def rotate_cw(self):
		self._send_command(self.COMMAND_ROTATE_CW)

	def rotate_ccw(self):
		self._send_command(self.COMMAND_ROTATE_CCW)

	def rotate_time(self, time):
		self._send_command(self.COMMAND_ROTATE_TIME, time)

	def _send_command(self, command, *args):
		buffer = "%s%s;" % (command, ','.join(map(str, ('',) + args)))

		print "Tx: %s" % buffer

		self.ser.write(buffer)
		retval = self._read_return()

		print "Rx: %s" % retval

		matches = re.search('^(.*?)(,(.*?))*$', retval)

		return_code = matches.group(1)
		return_message = matches.group(3)

		if return_code != '1':
			raise Exception(return_message)

		# print "Rx: %s" % return_message

	def _read_return(self):
		buffer = ''
		while True:
			if ';' in buffer:
				lines = buffer.split(';')
				return lines.pop(0)
			else:
				buffer += self.ser.read(1)  # this will block until one more char or timeout

			buffer += self.ser.read(self.ser.inWaiting())
