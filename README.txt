libnifalcon 
===========
Kyle Machulis <kyle@nonpolynomial.com>
Version 1.0.1, September 20, 2009

== Description ==

libnifalcon is a development library for the NovInt Falcon, and is an open source, crossplatform alternative to NovInt's SDK. 

libnifalcon provides basic functionality to connect to the falcon and load firmware to the internal microcontroller. In addition, it comes with sample functionality made available through the firmware available in NovInt's drivers (the novint.bin file in TestUtilties and the nifalcon_test_fw files for the library source). This firmware is distributed in the firmware directory of the source distribution, and is required for the findfalcons utility to run.

Compilation directions are included in the COMPILE.txt file

libnifalcon is now a supported device in the H3D Haptics Framework. More information on this is available at

http://www.h3d.org

== Novint Falcon Information  ==

More information about the Novint Falcon can be found at 

http://home.novint.com

Technical information, including protocol and hardware information, is available at the libnifalcon website

http://libnifalcon.nonpolynomial.com

== Library Requirements ==

=== cmake ===

http://www.cmake.org

=== Boost ===

http://www.boost.org/

=== ftd2xx (Recommended for Windows) ===

http://www.ftdichip.com/Drivers/D2XX.htm

=== libusb 1.0 (Recommended for Linux or Mac) ===

http://www.libusb.org/

== ftd2xx versus libusb 1.0 ==

libnifalcon can use two different libraries to access the falcon:

FTD2XX - Library distributed by FTDI themselves
Windows: OK - RECOMMENDED
Linux: Untested
64-bit Linux: Untested
OS X: Untested

libusb-1.0 (use whatever the latest version is. As of this writing, 1.0.2):
Windows: Not Available for Windows
Linux: OK - RECOMMENDED
64-bit Linux: OK - RECOMMENDED
OS X: OK - RECOMMENDED

Note that libnifalcon has been written in such a way that which comm library you use should have little effect on the cross-platform performance of your code. However, this is more of a hope than a promise.

== What Comes With libnifalcon ==

With libnifalcon, you get

* libnifalcon
** Provides device access to the falcon
* libnifalcon_cli_base (optional, requires boost::program_options library)
** Provides base class for small test applications for the falcon
* libnifalcon_boost_thread (optional, requires boost::thread library)
** Provides example of threaded device support for the falcon
* Example Programs (all require libnifalcon, plus other libraries as specified)
** findfalcons - Prints out the number of falcon devices connected, opens each of them (one at a time), runs simple test (changes LED color, runs a few thousand I/O loops). Good for making sure things 'just work'.
** findfalcons_multi - Same as findfalcons, but all falcons at once. For testing multiple communications/devices.
** barrow_mechanics - Alastair Barrow's original implementation of the RL Stamper kinematics system used in the libnifalcon kinematics core
** falcon_led (requires libnifalcon_cli_base) - Simple example application for extending FalconCLIBase class.
** falcon_mouse (requires libnifalcon_cli_base) - Lets the falcon position control the mouse cursor
** falcon_test_cli (requires libnifalcon_cli_base) - Suite of test scenes for the falcon (cube, walls, timing tests, etc...)
* SWIG bindings (requires SWIG and whatever language bindings you want to use)
** FalconBridge proxy class for accessing the falcon from other languages. Tested with Java and Python so far.

== Platform Specifics ==

=== Windows ===

* If you are using the falcon on Windows, even if it's a Windows VM, make sure it's plugged in directly to a port on the machine you're on. Hubs don't work with the falcon on Windows.
* NovInt uses the stock ftd2xx drivers, and it is recommended to use a ftd2xx version of libnifalcon on windows. If you have installed the drivers from NovInt's website, access through libnifalcon should "just work".
* Building under Visual Studio will only work for STATIC libraries, as I'm too lazy to add the declspec calls.

=== Linux ===

* If you are using the falcon on Linux, make sure it's plugged in through a powered USB hub. For some reason, plugging it directly into a Linux Box causes it to work erractically.
* Programs built with libnifalcon (including example programs that come with the library) requires either root access (i.e. running under sudo) or correct udev based USB permissions to run as non-root. There's a sample udev file in the "linux" directory of the distribution that can help.

=== OS X ===

* If you are using the falcon on OS X, make sure it's plugged in through a powered USB hub. For some reason, plugging it directly into a Mac causes it to work erractically.

== Credits ==

libnifalcon is developed and maintained and generally the fault of Kyle Machulis
More information on Kyle can be found at Nonpolynomial Labs - http://www.nonpolynomial.com

Kinematics development by Kevin Ouellet and Alastair Barrow

Contributions by 
Jared Allen
Alastair Barrow
Niall Begley
Edgar Berdahl
Steven Martin
Kevin Ouellet
Devanshi Shah
Stephen Sinclair

== Licenses ==

(License text for all following licenses is available in the license directory)

libnifalcon is licensed under the BSD license, with the following copyrights:

libnifalcon is Copyright 2007-2009 Kyle Machulis/Nonpolynomial Labs

libnifalcon Kinematics Core is Copyright 2007-2008 Kevin Ouellet, 2009 Alastair Barrow, 2007-2009 Kyle Machulis

libnifalcon uses GMTL, part of GGT, which is licensed under LGPL 3.0 with header exception.

---------------------

GGT: The Generic Graphics Toolkit
Copyright (C) 2001,2002 Allen Bierbaum

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

---------------------

On some platforms, libnifalcon uses libusb-1.0, which is licensed under LGPL 2.1.

---------------------

libusb 1.0
Copyright (C) 2007-2008 Daniel Drake <dsd@gentoo.org>
Copyright (c) 2001 Johannes Erdfelt <johannes@erdfelt.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

---------------------
