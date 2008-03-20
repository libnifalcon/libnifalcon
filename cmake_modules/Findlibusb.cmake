# - Try to find libusb
# Once done this will define
#
#  LIBUSB_FOUND - system has libusb
#  LIBUSB_INCLUDE_DIRS - the libusb include directory
#  LIBUSB_LIBRARIES - Link these to use libusb
#  LIBUSB_DEFINITIONS - Compiler switches required for using libusb
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


if (LIBUSB_LIBRARIES AND LIBUSB_INCLUDE_DIRS)
  # in cache already
  set(LIBUSB_FOUND TRUE)
else (LIBUSB_LIBRARIES AND LIBUSB_INCLUDE_DIRS)
  find_path(LIBUSB_INCLUDE_DIR
    NAMES
      usb.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
  )

  find_library(LIBUSB_LIBRARY
    NAMES
      usb
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )

  set(LIBUSB_INCLUDE_DIRS
    ${LIBUSB_INCLUDE_DIR}
  )
  set(LIBUSB_LIBRARIES
    ${LIBUSB_LIBRARY}
)

  if (LIBUSB_INCLUDE_DIRS AND LIBUSB_LIBRARIES)
     set(LIBUSB_FOUND TRUE)
  endif (LIBUSB_INCLUDE_DIRS AND LIBUSB_LIBRARIES)

  if (LIBUSB_FOUND)
    if (NOT libusb_FIND_QUIETLY)
      message(STATUS "Found libusb: ${LIBUSB_LIBRARIES}")
    endif (NOT libusb_FIND_QUIETLY)
  else (LIBUSB_FOUND)
    if (libusb_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find libusb")
    endif (libusb_FIND_REQUIRED)
  endif (LIBUSB_FOUND)

  # show the LIBUSB_INCLUDE_DIRS and LIBUSB_LIBRARIES variables only in the advanced view
  mark_as_advanced(LIBUSB_INCLUDE_DIRS LIBUSB_LIBRARIES)

endif (LIBUSB_LIBRARIES AND LIBUSB_INCLUDE_DIRS)