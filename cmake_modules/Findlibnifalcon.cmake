# Find libnifalcon 
# If found, will create the following values
#
#  LIBNIFALCON_C_FOUND - system has libnifalcon c libraries
#  LIBNIFALCON_C_INCLUDE_DIRS - the libnifalcon c include directory
#  LIBNIFALCON_C_FTD2XX_LIBRARIES - libnifalcon c libraries for ftd2xx
#  LIBNIFALCON_C_LIBFTDI_LIBRARIES - libnifalcon c libraries for libftdi
#
#  LIBNIFALCON_FOUND - system has base libnifalcon c++ libraries
#  LIBNIFALCON_INCLUDE_DIRS - the libnifalcon c++ include directory
#  LIBNIFALCON_LIBRARIES - libnifalcon c++ libraries
#
#  LIBNIFALCON_COMM_FTD2XX_FOUND - system has ftd2xx based falcon communications libs
#  LIBNIFALCON_COMM_FTD2XX_LIBRARIES - libraries required to use ftd2xx communications
#  LIBNIFALCON_COMM_LIBFTDI_FOUND - sstem has libftdi based falcon communications libs
#  LIBNIFALCON_COMM_LIBFTDI_LIBRARIES - libraries required to use libftdi communications
#
#  LIBNIFALCON_CLI_BASE_FOUND - system has cli base utilities for falcon programs
#  LIBNIFALCON_CLI_BASE_LIBRARIES - libraries required to use cli base utilities
#  LIBNIFALCON_BOOST_THREAD_FOUND - system has boost thread utilities for falcon programs
#  LIBNIFALCON_BOOST_THREAD_LIBRARIES - libraries required to use boost thread device
#
# Copyright (c) 2008 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
#

MESSAGE(STATUS "Finding libnifalcon")

if (LIBNIFALCON_C_INCLUDE_DIRS)
  set(LIBNIFALCON_C_FOUND TRUE)
else (LIBNIFALCON_C_INCLUDE_DIRS)
  find_path(LIBNIFALCON_C_INCLUDE_DIR
    NAMES
    falcon/c/nifalcon.h
    PATHS	  
    /usr/include/
    /usr/local/include/
    /opt/local/include/
    /sw/include/
	)
  
  find_library(LIBNIFALCON_C_FTD2XX_LIBRARY
    NAMES
    nifalcon_c_ftd2xx
    PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
	)

  find_library(LIBNIFALCON_C_LIBFTDI_LIBRARY
    NAMES
    nifalcon_c_libftdi
    PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
	)
  
  set(LIBNIFALCON_C_INCLUDE_DIRS
    ${LIBNIFALCON_C_INCLUDE_DIR}
	)
  
  if (LIBNIFALCON_C_INCLUDE_DIRS)
	IF(LIBNIFALCON_C_FTD2XX_LIBRARY OR LIBNIFALCON_C_LIBFTDI_LIBRARY)
      set(LIBNIFALCON_C_FOUND TRUE)
	ENDIF(LIBNIFALCON_C_FTD2XX_LIBRARY OR LIBNIFALCON_C_LIBFTDI_LIBRARY)
  endif (LIBNIFALCON_C_INCLUDE_DIRS)
  
  if (LIBNIFALCON_C_FOUND)
    if(NOT libnifalcon_c_FIND_QUIETLY)
      message(STATUS "Found libnifalcon (c version)")
	  message(STATUS " - Includes: ${LIBNIFALCON_C_INCLUDE_DIRS}")
	  if(LIBNIFALCON_C_FTD2XX_LIBRARY)
		message(STATUS " - FTD2XX Library: ${LIBNIFALCON_C_FTD2XX_LIBRARY}")
	  endif(LIBNIFALCON_C_FTD2XX_LIBRARY)
	  if(LIBNIFALCON_C_LIBFTDI_LIBRARY)
		message(STATUS " - LIBFTDI Library: ${LIBNIFALCON_C_LIBFTDI_LIBRARY}")
	  endif(LIBNIFALCON_C_LIBFTDI_LIBRARY)
    endif (NOT libnifalcon_c_FIND_QUIETLY)
  else (LIBNIFALCON_C_FOUND)
    if (libnifalcon_c_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find libnifalcon (c version)")
    endif (libnifalcon_c_FIND_REQUIRED)
  endif (LIBNIFALCON_C_FOUND)
  
  mark_as_advanced(LIBNIFALCON_C_INCLUDE_DIRS LIBNIFALCON_C_LIBRARIES)
  
endif (LIBNIFALCON_C_INCLUDE_DIRS)

if (LIBNIFALCON_LIBRARIES AND LIBNIFALCON_INCLUDE_DIRS)
  set(LIBNIFALCON_FOUND TRUE)
else (LIBNIFALCON_LIBRARIES AND LIBNIFALCON_INCLUDE_DIRS)
  find_path(LIBNIFALCON_INCLUDE_FILE
    NAMES
    falcon/core/FalconDevice.h
    PATHS	  
    /usr/include/
    /usr/local/include/
    /opt/local/include/
    /sw/include/
	)
  
  find_path(LIBNIFALCON_INCLUDE_DIR
	NAMES
	falcon
	PATHS
    /usr/include/
    /usr/local/include/
    /opt/local/include/
    /sw/include/
	)
  
  find_library(LIBNIFALCON_LIBRARY
    NAMES
    nifalcon
    PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
	)

  find_library(LIBNIFALCON_COMM_FTD2XX_LIBRARY
    NAMES
    nifalcon_comm_ftd2xx
    PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
	)

  find_library(LIBNIFALCON_COMM_LIBFTDI_LIBRARY
    NAMES
    nifalcon_comm_libftdi
    PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
	)

  find_library(LIBNIFALCON_COMM_LIBUSB_LIBRARY
    NAMES
    nifalcon_comm_libusb
    PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
	)

  find_library(LIBNIFALCON_CLI_BASE_LIBRARY
    NAMES
    nifalcon_clibase
    PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
	)

  find_library(LIBNIFALCON_DEVICE_BOOST_THREAD_LIBRARY
    NAMES
    nifalcon_device_boost_thread
    PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
	)

  set(LIBNIFALCON_INCLUDE_DIRS
	${LIBNIFALCON_INCLUDE_DIR}
	)
  
  set(LIBNIFALCON_LIBRARIES
    ${LIBNIFALCON_LIBRARY} 
	)

  set(LIBNIFALCON_COMM_LIBRARIES ${LIBNIFALCON_COMM_LIBFTDI_LIBRARY} ${LIBNIFALCON_COMM_LIBUSB_LIBRARY} ${LIBNIFALCON_COMM_FTD2XX_LIBRARY})

  if (LIBNIFALCON_INCLUDE_FILE AND LIBNIFALCON_INCLUDE_DIRS AND LIBNIFALCON_LIBRARIES AND (LIBNIFALCON_COMM_LIBUSB_LIBRARY OR LIBNIFALCON_COMM_LIBFTDI_LIBRARY OR LIBNIFLACON_COMM_LIBUSB_LIBRARY))
    set(LIBNIFALCON_FOUND TRUE)
  if (LIBNIFALCON_INCLUDE_FILE AND LIBNIFALCON_INCLUDE_DIRS AND LIBNIFALCON_LIBRARIES AND (LIBNIFALCON_COMM_LIBUSB_LIBRARY OR LIBNIFALCON_COMM_LIBFTDI_LIBRARY OR LIBNIFLACON_COMM_LIBUSB_LIBRARY))

  if (LIBNIFALCON_FOUND)
    if (NOT libnifalcon_FIND_QUIETLY)
      message(STATUS "Found libnifalcon (c++ version)")
	  message(STATUS " - Includes: ${LIBNIFALCON_INCLUDE_DIRS}")
	  message(STATUS " - Libraries: ${LIBNIFALCON_LIBRARIES}")
	  message(STATUS " - Comm Libraries:")
	  FOREACH(LIB ${LIBNIFALCON_COMM_LIBRARIES})
		message(STATUS "   - ${LIB}")
	  ENDFOREACH(LIB)
    endif (NOT libnifalcon_FIND_QUIETLY)
  else (LIBNIFALCON_FOUND)
    if (libnifalcon_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find libnifalcon (c++ version)")
    endif (libnifalcon_FIND_REQUIRED)
  endif (LIBNIFALCON_FOUND)
  mark_as_advanced(LIBNIFALCON_INCLUDE_DIRS LIBNIFALCON_LIBRARIES)
endif (LIBNIFALCON_LIBRARIES AND LIBNIFALCON_INCLUDE_DIRS)

