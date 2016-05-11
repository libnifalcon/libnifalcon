# - Try to find libusb-1.0
# Once done this will define
#
#  LIBUSB1_FOUND - system has libusb
#  LIBUSB1_INCLUDE_DIRS - the libusb include directory
#  LIBUSB1_LIBRARIES - Link these to use libusb
#  LIBUSB1_DEFINITIONS - Compiler switches required for using libusb
#
#  Adapted from cmake-modules Google Code project
#
#  Copyright (c) 2006 Andreas Schneider <mail@cynapses.org>
#
#  (Changes for libusb) Copyright (c) 2008 Kyle Machulis <kyle@nonpolynomial.com>
#
# Redistribution and use is allowed according to the terms of the New BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

find_path(LIBUSB1_INCLUDE_DIR
  NAMES
    libusb.h
  PATHS
    /usr/include
    /usr/local/include
    /opt/local/include
    /sw/include
  PATH_SUFFIXES
    libusb-1.0
    )
mark_as_advanced(LIBUSB1_INCLUDE_DIR)

find_library(LIBUSB1_LIBRARY
  NAMES
    usb-1.0
    libusb-1.0
  PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
    )
mark_as_advanced(LIBUSB1_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Libusb1
  REQUIRED_VARS
	  LIBUSB1_LIBRARY
	  LIBUSB1_INCLUDE_DIR
    )

set(LIBUSB1_INCLUDE_DIRS ${LIBUSB1_INCLUDE_DIR})
set(LIBUSB1_LIBRARIES ${LIBUSB1_LIBRARY})
