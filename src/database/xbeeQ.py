#! /usr/bin/python

# Import and init an XBee device
from firebase import firebase
from xbee import XBee
import serial
import time
import datetime
#####FIRBASE CREDS######
firebase = firebase.FirebaseApplication('https://burning-fire-9965.firebaseio.com/', None)




PORT = '/dev/tty.usbserial-000012FD'
BAUD = 19200
ser = serial.Serial(PORT, BAUD)
xbee = XBee(ser)
ser.flushInput()
ser.flushOutput()

GET_STATUS = "AA"
ENABLE = "EE"
NO_ARG = "00"


DEVICE_ID = '4200000069'


class FrameGen:
    DELIMITER = "2D"

    def __init__(self):
        self.hex_base = '7E 00 {:02X} 01 {:02X} {:02X} {:02X} {:02X}'
        self.length = 3 + 5
        self.frameid = 1
        self.addr = 1
        self.options = 0
        self.hex_base = self.hex_base.format(self.length, self.frameid, (self.addr&0xFF00)>>8, self.addr&0x00FF, self.options)

    def GenerateByteArray(self, command, argument):
        hex_generated = bytearray.fromhex(command + ' ' + argument + ' ' + self.DELIMITER)
        self.frame = bytearray.fromhex(self.hex_base)
        self.frame.extend(hex_generated)
        self.frame.append(0xFF - (sum(self.frame[3:])&0xFF))

    def SendFrame (self, ser):
        ser.write(self.frame)

Generator = FrameGen()
Generator.GenerateByteArray(ENABLE, NO_ARG)
Generator.SendFrame(ser)
ack=xbee.wait_read_frame()
print ack




result = firebase.get('/device', None)

if not hasattr(result, DEVICE_ID):
   result = firebase.put('/device', DEVICE_ID, {'loc': '0, 0'})


while True:
    try:
        Generator.GenerateByteArray(GET_STATUS, NO_ARG)
        Generator.SendFrame(ser)
        status = xbee.wait_read_frame()
        time.sleep(5)
        status_str = str(status)
        if status.has_key("rf_data"):
            moisture_reading = status['rf_data'].split('/')[1]
            temperature_reading = status['rf_data'].split('/')[4]        
       #     state = status['rf_data'].split('/')[5]        
            ts = time.time()
            st = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S')
            firebase.put('/device/'+DEVICE_ID+'/packet/', st, {'moisture': moisture_reading})
            firebase.put('/device/'+DEVICE_ID+'/packet/'+st, {'temperature': temperature_reading})
            print "MOISTURE: " + moisture_reading
            print "TEMP: " + temperature_reading
            #print "CURR_STATE: " + state
    except KeyboardInterrupt:
        break