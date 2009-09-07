/**
@mainpage  libnifalcon
@author kyle@nonpolynomial.com

Cross-platform driver for the novint falcon.

Software website: http://libnifalcon.nonpolynomial.com

Novint Falcon website: http://home.novint.com

@section libnifalconIntro Introduction

libnifalcon is a development library for the NovInt Falcon, and is an open source, crossplatform alternative to NovInt's SDK. 

libnifalcon provides basic functionality to connect to the falcon and load firmware to the internal microcontroller. In addition, it comes with sample functionality made available through the firmware available in NovInt's drivers (the novint.bin file in TestUtilties and the nifalcon_test_fw files for the library source). This firmware is distributed in the firmware directory of the source distribution, and is required for the findfalcons utility to run.

@section libnifalconDesignOverview Design Overview

The main design goal of libnifalcon is to make a driver that's as flexible as the hardware itself. The Novint Falcon provides many extendable features, such as firmware loading and grip switch. We tried to design libnifalcon with the same idea.

Access to the falcon consists of 4 'behaviors':

- Communications - How we talk to the hardware
- Firmware - How the embedded hardware works and communicates
- Kinematics - The ability to get the position of the end effector (the grip) and apply forces to it
- Grip - Access to all the features of any manufactured grip

libnifalcon ships with a stable set of classes implementing all of these behaviors. Each of these behaviors can be switched out as needed under the libnifalcon::FalconDevice class, meaning that the driver can be used both for simple device access, and as well as research on new firmware, grip hardware, and kinematics system. Here's a few ideas for usages of libnifalcon outside of just getting the falcon to work on your non-windows platform:

- Education
 - Mechanical Engineering (dynamics)
 - Computer Engineering (hardware drivers, firmware/dsp development)
 - Computer Science (hardware drivers, software organization)
 - HCI (haptics, grip development)
- Audio (physical instrument interfaces)
- Animatronics
- Punching anyone who gets too near your falcon
 
@section libnifalconShouldIUseIt Should You Use libnifalcon?

One thing to note if you're new to haptics: libnifalcon is simply a driver for a certain piece of haptics hardware. It is not a haptic scene graph. This means that, while we have examples for implementing collision with some simple geometry, it's not really meant for haptics research usage unless you're looking to do very specific kinematics or grip interface work with this piece of hardware. We expect that libnifalcon will more normally be implemented as part of a larger haptics framework, such as:

- H3D - http://www.h3d.org (libnifalcon support exists in source repository as of August 2009)
- Chai3d - http://www.chai3d.org/ (not currently directly supported by developers, but soon hopefully!)

@section libnifalconHardwareInformation Hardware Information

- Pictures of falcon disassembly, thanks to Kevin Ouellet - http://picasaweb.google.com/kouellet/FalconProject?authkey=kAvHYd9lo5A
- Motors
 - Mabuchi RS-555PH, number 15280
 - Inscription on Motor: RS-555PH-15280 (Mabuchi LOGO) MADE IN CHINA TD427420
- Communications Chip: FTDI FT232R
 - Datasheet: http://www.ftdichip.com/Documents/DataSheets/DS_FT232R.pdf
- Internal DSP chip for machine control: TI TMS320RMS2811 
 - Datasheet: http://focus.ti.com/docs/prod/folders/print/tms320r2811.html
*/

/**
@defgroup CoreClasses Core Classes

Core classes are the base classes the libnifalcon behaviors derive from (firmware, grip, etc...), or that glue the behaviors together (libnifalcon::FalconDevice).
*/

/**
@defgroup CommClasses Communications Classes

Communications classses are used to establish a connection to the falcon, set firmware or normal communication modes, and read from/write to the device. More instructions for implementing communications are available in the description of the libnifalcon::FalconComm class.
*/

/**
@defgroup KinematicsClasses Kinematics Classes

Kinematics classes provide a way to turn the joint angles we receive from the falcon into a corresponding cartesian coordinates (forward kinematics), and to go from effector position to joint angles (inverse kinematics). This information is used to calculate where the effector is in relation to a particular scene, and calculate the forces needed to render whatever is happening in that scene.
*/

/**
@defgroup GripClasses Grip Classes

The falcon provides a grip connection mechanism that allows the user to attach multiple kinds of grips. These classes implement the different capabilities of these grips.
*/

/**
@defgroup FirmwareClasses Firmware Classes

The falcon has firmware that is loaded on first connection to the host computer. For most needs, firmware is important because it defines the communications format for the falcon.
*/

/**
@defgroup UtilityClasses Utility Classes

Utility classes are not required for using libnifalcon, but come in handy when building applications based around it.
*/
