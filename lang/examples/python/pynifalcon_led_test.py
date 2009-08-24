from math import *
import imp, sys
import time

#if using pyplusplus
# f  = open('libnifalcon_python.dylib', 'rb')
# fd = imp.load_module('falcondevice', f, 'libnifalcon_python.dylib', ('libnifalcon_python.dylib', 'rb', imp.C_EXTENSION))
# fdd = fd.FalconDeviceBridge()
#if using swig
from pynifalcon import *
fdd = FalconDeviceBridge()
print "Devices attached: %d" % (fdd.getCount())
if fdd.getCount() is 0:
    print "No devices attached, exiting..."
    sys.exit()

if not fdd.open(0):
    print "Cannot open device, exiting..."
    sys.exit()

if not fdd.loadFirmware():
    print "Cannot load firmware, exiting..."
    sys.exit()

start = time.time()
total_time = 25
led_limit = total_time
led_count = 0
move_time = time.time()
led_move = -1

while 1:
    ret = fdd.runIOLoop(1)
    if not ret:
        continue

    # RGB fade test
    led_count = led_count + 1
    if led_count == total_time:
        led_count = 0
    
    if led_count > led_limit:
        fdd.setLEDStatus(4)
    else:
        fdd.setLEDStatus(0)
        
    if (time.time() - move_time) > 0.10:
        led_limit = led_limit + led_move
        if led_limit == 0 or led_limit == total_time:
            led_move = -led_move        
        move_time = time.time()
