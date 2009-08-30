libnifalcon 
===========
Kyle Machulis <kyle@nonpolynomial.com>
Version 1.0.0, August 30, 2009

== Description ==

libnifalcon is a development library for the NovInt Falcon, and is an open source, crossplatform alternative to NovInt's SDK. 

libnifalcon provides basic functionality to connect to the falcon and load firmware to the internal microcontroller. In addition, it comes with sample functionality made available through the firmware available in NovInt's drivers (the novint.bin file in TestUtilties and the nifalcon_test_fw files for the library source). This firmware is distributed in the firmware directory of the source distribution, and is required for the findfalcons utility to run.

Compilation directions are included in the INSTALL.txt file

libnifalcon is now a supported device in the H3D Haptics Framework. More information on this is available at

http://www.h3d.org

== NovInt Falcon Information  ==

More information about the NovInt Falcon can be found at 

http://home.novint.com

Technical information, including protocol and hardware information, is available at

http://wiki.libnifalcon.sourceforge.net

== Library Requirements ==

=== cmake ===

http://www.cmake.org

=== Boost ===

http://www.boost.org/

=== ftd2xx (Recommended for Windows) ===

http://www.ftdichip.com/Drivers/D2XX.htm

=== libusb 1.0 (Recommended for Linux or Mac) ===

http://libusb.wiki.sourceforge.net/Libusb1.0

== ftd2xx versus libusb 1.0 ==

libnifalcon can use two different libraries to access the falcon:

FTD2XX - Library distributed by FTDI themselves
Windows: OK - RECOMMENDED
Linux: Untested
64-bit Linux: Untested
OS X: Untested

libusb-1.0.1:
Windows: Not Available for Windows
Linux: OK - RECOMMENDED
64-bit Linux: OK - RECOMMENDED
OS X: OK - RECOMMENDED

Note that libnifalcon has been written in such a way that which comm library you use should have little effect on the cross-platform performance of your code. However, this is more of a hope than a promise.

== Platform Specifics ==

=== Windows ===

- If you are using the falcon on Windows, even if it's a Windows VM, make sure it's plugged in directly to a port on the machine you're on. Hubs don't work with the falcon on Windows.
- NovInt uses the stock ftd2xx drivers, and it is recommended to use a ftd2xx version of libnifalcon on windows. If you have installed the drivers from NovInt's website, access through libnifalcon should "just work".
- Building under Visual Studio will only work for STATIC libraries, as I'm too lazy to add the declspec calls.

=== Linux ===

- If you are using the falcon on Linux, make sure it's plugged in through a powered USB hub. For some reason, plugging it directly into a Linux Box causes it to work erractically.
- Programs built with libnifalcon (including example programs that come with the library) requires either root access (i.e. running under sudo) or correct udev based USB permissions to run as non-root. There's a sample udev file in the "linux" directory of the distribution that can help.

=== OS X ===

- If you are using the falcon on OS X, make sure it's plugged in through a powered USB hub. For some reason, plugging it directly into a Mac causes it to work erractically.

== Credits ==

libnifalcon is maintained by Kyle Machulis
More information at Nonpolynomial Labs - http://www.nonpolynomial.com

Kinematics development by Kevin Ouellet and Alastair Barrow

Contributions by 
Jared Allen
Edgar Berdahl
Devanshi Shah
Stephen Sinclair
