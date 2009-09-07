/**
@mainpage  libnifalcon
@author kyle@nonpolynomial.com

Cross-platform driver for the novint falcon

@section libnifalconIntro Introduction

libnifalcon is a development library for the NovInt Falcon, and is an open source, crossplatform alternative to NovInt's SDK. 

libnifalcon provides basic functionality to connect to the falcon and load firmware to the internal microcontroller. In addition, it comes with sample functionality made available through the firmware available in NovInt's drivers (the novint.bin file in TestUtilties and the nifalcon_test_fw files for the library source). This firmware is distributed in the firmware directory of the source distribution, and is required for the findfalcons utility to run.

@section libnifalconDesignOverview Design Overview

Here's where I'd put some design documentation if I had it.

@section libnifalconHardwareInformation Hardware Information

Pictures of falcon disassembly, thanks to Kevin Ouellet.

http://picasaweb.google.com/kouellet/FalconProject?authkey=kAvHYd9lo5A

Motors: Mabuchi RS-555PH, number 15280

Inscription on Motor:
RS-555PH-15280
(Mabuchi LOGO) MADE IN CHINA
TD427420

The falcon uses a FTDI FT232R Chip to communicate with the host computer. Datasheet at

http://www.ftdichip.com/Documents/DataSheets/DS_FT232R.pdf

The falcon uses a TI TMS320R2811 DSP Chip for device control. Datasheet at

http://focus.ti.com/docs/prod/folders/print/tms320r2811.html
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
