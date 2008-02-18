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

------
ftd2xx
------
http://www.ftdichip.com/Drivers/D2XX.htm

-----
cmake
-----
http://www.cmake.org

==================
Platform Specifics
==================

-------
Windows
-------

- NovInt uses the stock d2xx drivers. If you have installed the drivers from NovInt's website, access through libnifalcon should "just work"

-----
Linux
-----

- libnifalcon has been known to close the falcon incorrectly, at which point the USB cable must be unplugged and replugged for the falcon to start working again. If things tend to freeze up during initlization, this is probably the case.

---
OSX
---

- OS X seems to block on writes after a very short period of time. Stopping and restarting the program seems to clear up the issue. I have no idea what the problem is here.
- libnifalcon has been known to close the falcon incorrectly, at which point the USB cable must be unplugged and replugged for the falcon to start working again. If things tend to freeze up during initlization, this is probably the case.
