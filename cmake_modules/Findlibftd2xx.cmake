# - Try to find libftd2xx
# Once done this will define
#
#  LIBFTD2XX_FOUND - system has libftd2xx
#  LIBFTD2XX_INCLUDE_DIRS - the libftd2xx include directory
#  LIBFTD2XX_LIBRARIES - Link these to use libftd2xx
#  LIBFTD2XX_DEFINITIONS - Compiler switches required for using libftd2xx
#
#  Adapted from cmake-modules Google Code project
#
#  Copyright (c) 2006 Andreas Schneider <mail@cynapses.org>
#
#  (Changes for libftd2xx) Copyright (c) 2008 Kyle Machulis <kyle@nonpolynomial.com>
#
# Redistribution and use is allowed according to the terms of the New BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

find_path(LIBFTD2XX_INCLUDE_DIR
  NAMES
    ftd2xx.h
  PATHS
    /usr/include
    /usr/local/include
    /opt/local/include
    /sw/include
  )
mark_as_advanced(LIBFTD2XX_INCLUDE_DIR)

find_library(LIBFTD2XX_LIBRARY
  NAMES
    ftd2xx
    libftd2xx
  PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
  )
mark_as_advanced(LIBFTD2XX_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBFTD2XX
  REQUIRED_VARS
	  LIBFTD2XX_LIBRARY
	  LIBFTD2XX_INCLUDE_DIR
    )

set(LIBFTD2XX_INCLUDE_DIRS ${LIBFTD2XX_INCLUDE_DIR})
set(LIBFTD2XX_LIBRARIES ${LIBFTD2XX_LIBRARY})
