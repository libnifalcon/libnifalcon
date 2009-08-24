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

pos_fv = FalconVec3d()
force_fv = FalconVec3d()

pos = [0,0,0]
force = [0,0,0]
start = time.time()
count = 0

while 1:
    ret = fdd.runIOLoop(1 | 2)
    if not ret:
        continue
    if (time.time() - start) > 1.0:
#        print count
        count = 0
        start = time.time()
    else:
        count = count + 1

    # Cube Test Code
    pos_fv = fdd.getPosition()
    pos = [pos_fv.x, pos_fv.y, pos_fv.z]

    cornerA = [-.030, -.030, .095]
    cornerB = [.030, .030, .155]
    stiffness = 1000
    dist = 10000
    closest = -1
    outside = 3
    axis = 0

    for axis in range(0, 3):
        force[axis] = 0
        if pos[axis] > cornerA[axis] and pos[axis] < cornerB[axis]:
            dA = pos[axis]-cornerA[axis]
            dB = pos[axis]-cornerB[axis]

            if fabs(dA) < fabs(dist): 
                dist = dA 
                closest = axis
            if fabs(dB) < fabs(dist):
                dist = dB
                closest = axis
            outside = outside - 1
    
    if closest > -1 and outside == 0:
        force[closest] = -stiffness*dist;

    force_fv.x = force[0]
    force_fv.y = force[1]
    force_fv.z = force[2]
    fdd.setForces(force_fv)
