===========
libnifalcon
===========

by Kyle Machulis <kyle@nonpolynomial.com>
Nonpolynomial Labs - http://www.nonpolynomial.com

===========
Description
===========

libnifalcon is a C-based development library for the NovInt Falcon, and is an open source, crossplatform alternative to NovInt's SDK. 

libnifalcon provides basic functionality to connect to the falcon and load firmware to the internal microcontroller. In addition, it comes with sample functionality made available through the firmware available in NovInt's drivers (the novint.bin file in TestUtilties and the nifalcon_test_fw files for the library source). This firmware is distributed in the firmware directory of the source distribution, and is required for the findfalcons utility to run.

Compilation directions are included in the INSTALL.txt file

=========================
NovInt Falcon Information
=========================

More information about the NovInt Falcon can be found at 

http://home.novint.com

Technical information, including protocol and hardware information, is available at

http://wiki.nonpolynomial.com/NovintFalcon

====================
Library Requirements
====================

-----
cmake
-----
http://www.cmake.org

------
ftd2xx
------
http://www.ftdichip.com/Drivers/D2XX.htm

AND/OR

-------
libftdi
-------

http://www.intra2net.com/de/produkte/opensource/ftdi/

=====================
ftd2xx versus libftdi
=====================

libnifalcon can use one of two libraries to access the falcon: ftd2xx, distributed by the FTDI company, or libftdi, an open source version of the FTDI drivers. 

FTD2XX:
Windows: Fine
Linux: Mostly Works?
64-bit Linux: No 64-bit drivers available
OS X: Locks up on write function after < 1000 loops

libftdi:
Windows: Untested (Recommended to use FTD2XX)
Linux: Stable, but runs at quarter speed of windows FTD2XX
64-bit Linux: Stable, but runs at quarter speed of windows FTD2XX 
OS X: Stable, but runs at quarter speed of windows FTD2XX

So, basically, ftd2xx is stable on windows, iffy everywhere else. libftdi is slow, but works everywhere. Just expect things to be choppy as you'll be lucky to get a ~300hz update rate.

==================
Platform Specifics
==================

-------
Windows
-------

- NovInt uses the stock ftd2xx drivers, and it is recommended to use a ftd2xx version of libnifalcon on windows. If you have installed the drivers from NovInt's website, access through libnifalcon should "just work".
- libftdi drivers will require a libusb filter driver install. This is not recommended for programs that will be distributed publically, as it will require an uninstall of the usual FTDI drivers.

-----
Linux
-----

- libftdi is recommended for Linux, for stability and compatibility reasons. ftd2xx seems to work ok, but libftdi is a bit more stable.
- 64-BIT USERS: There are no versions of ftd2xx drivers for 64-bit linux. libftdi has been tested and found to work fine with libnifalcon on 64-bit platforms.
- Some installations of libftdi using debian apt-get seem to be a little unstable with libnifalcon (this has mainly been found on 64-bit machines). If you have installed libftdi from a prepackaged install and have stability issues accessing the falcon, it is recommended that you try compiling libftdi yourself.
- Programs built with libnifalcon (including example programs that come with the library) requires either root access (i.e. running under sudo) or correct udev based USB permissions to run as non-root.
- libnifalcon has been known to close the falcon incorrectly, at which point the USB cable must be unplugged and replugged for the falcon to start working again. If things tend to freeze up during initlization, this is probably the case.

---
OSX
---

- If you use ftd2xx: OS X seems to block on writes after a very short period of time. This does not happen with libftdi. Stopping and restarting the program seems to clear up the issue. I have no idea what the problem is here. 
- libnifalcon has been known to close the falcon incorrectly, at which point the USB cable must be unplugged and replugged for the falcon to start working again. If things tend to freeze up during initlization, this is probably the case.
