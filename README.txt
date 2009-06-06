===========
libnifalcon 
===========

by Kyle Machulis <kyle@nonpolynomial.com>
Nonpolynomial Labs - http://www.nonpolynomial.com

Kinematics by Kevin Ouellet
http://sourceforge.net/users/kouellet/

Contributions by 
Jared Allen
Edgar Berdahl
Devanshi Shah
Stephen Sinclair

===========
Description
===========

libnifalcon is a development library for the NovInt Falcon, and is an open source, crossplatform alternative to NovInt's SDK. 

libnifalcon provides basic functionality to connect to the falcon and load firmware to the internal microcontroller. In addition, it comes with sample functionality made available through the firmware available in NovInt's drivers (the novint.bin file in TestUtilties and the nifalcon_test_fw files for the library source). This firmware is distributed in the firmware directory of the source distribution, and is required for the findfalcons utility to run.

Compilation directions are included in the INSTALL.txt file

=========================
NovInt Falcon Information
=========================

More information about the NovInt Falcon can be found at 

http://home.novint.com

Technical information, including protocol and hardware information, is available at

http://wiki.libnifalcon.sourceforge.net

====================
Library Requirements
====================

-----
cmake
-----

http://www.cmake.org

-----
Boost
-----

http://www.boost.org/

--------------------------------
ftd2xx (Recommended for Windows)
--------------------------------
http://www.ftdichip.com/Drivers/D2XX.htm

OR

-----------------------------------------
libusb 1.0 (Recommended for Linux or Mac)
-----------------------------------------

http://libusb.wiki.sourceforge.net/Libusb1.0

OR

-----------------------------
libftdi (Deprecated)
-----------------------------

http://www.intra2net.com/de/produkte/opensource/ftdi/

=======================================
ftd2xx versus libftdi versus libusb 1.0
=======================================

libnifalcon can use three libraries to access the falcon:

FTD2XX - Library distributed by FTDI themselves
Windows: Fine - RECOMMENDED
Linux: Untested
64-bit Linux: Untested
OS X: Untested

libusb-1.0.1:
Windows: Not Available for Windows
Linux: OK - RECOMMENDED
64-bit Linux: OK - RECOMMENDED
OS X: OK - RECOMMENDED

libftdi:
Windows: Untested (Recommended to use FTD2XX)
Linux: OK - Slower than libusb-1.0
64-bit Linux: OK - Slower than libusb-1.0
OS X: OK - Slower than libusb-1.0

Note that libnifalcon has been written in such a way that which comm library you use should have little effect on the cross-platform performance of your code. However, this is more of a hope than a promise.

==================
Platform Specifics
==================

-------
Windows
-------

- If you are using the falcon on Linux, make sure it's plugged in directly to a port on the machine you're on. Hubs screw with the falcon on Windows.
- NovInt uses the stock ftd2xx drivers, and it is recommended to use a ftd2xx version of libnifalcon on windows. If you have installed the drivers from NovInt's website, access through libnifalcon should "just work".
- Building under Visual Studio will remove the loop timing tests in the examples, as I'm too lazy to switch from gtod calls to GPC.
- Building under Visual Studio will only work for STATIC libraries, as I'm too lazy to add the declspec calls.

-----
Linux
-----

- If you are using the falcon on Linux, make sure it's plugged in through a powered USB hub. For some reason, plugging it directly into a Linux Box causes it to work erractically.
- Programs built with libnifalcon (including example programs that come with the library) requires either root access (i.e. running under sudo) or correct udev based USB permissions to run as non-root. There's a sample udev file in the "linux" directory of the distribution that can help.

---
OSX
---

- If you are using the falcon on OS X, make sure it's plugged in through a powered USB hub. For some reason, plugging it directly into a Mac causes it to work erractically.
- As of ftd2xx 0.1.4 for OS X, ftd2xx seems to have gotten decent stability on OS X. libnifalcon developers prefer libusb for the time being.

