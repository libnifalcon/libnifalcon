#cmake directives

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)
SET(CMAKE_MODULE_PATH "./cmake_modules/")

#setting link directory policy

IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

#taken from http://www.cmake.org/pipermail/cmake/2006-February/008334.html

MACRO(SET_COMPILE_FLAGS TARGET)
  SET(FLAGS)
  FOREACH(flag ${ARGN})
    SET(FLAGS "${FLAGS} ${flag}")
  ENDFOREACH(flag)
  SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_FLAGS "${FLAGS}")
ENDMACRO(SET_COMPILE_FLAGS)

#project definition

PROJECT(LIBNIFALCON)

#finds for libnifalcon building

FIND_PACKAGE(libusb)
FIND_PACKAGE(libftd2xx)
FIND_PACKAGE(libftdi)
FIND_PACKAGE(Boost 1.33 COMPONENTS program_options thread)

IF(NOT LIBFTDI_FOUND AND NOT LIBFTD2XX_FOUND)
  MESSAGE(FATAL_ERROR "Either ftd2xx (http://www.ftdichip.com) or libftdi (http://www.intra2net.com/de/produkte/opensource/ftdi/) is required to build libnifalcon")
ENDIF(NOT LIBFTDI_FOUND AND NOT LIBFTD2XX_FOUND)

#Local output paths

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
SET(GMTL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

#library definitions

SET(LIBNIFALCON_VERSION 1.0)
SET(LIBNIFALCON_DEBUG_POSTFIX "_d")

#library name definitions

SET(LIBNIFALCON_C_PREFIX "nifalcon_c")

SET(LIBNIFALCON_CPP "nifalcon")
SET(LIBNIFALCON_CPP_UTIL "nifalcon_util")
SET(LIBNIFALCON_CPP_COMM_PREFIX "nifalcon_comm")
SET(LIBNIFALCON_CPP_COMM_FTD2XX "nifalcon_comm_ftd2xx")
SET(LIBNIFALCON_CPP_COMM_LIBFTDI "nifalcon_comm_libftdi")
SET(LIBNIFALCON_CPP_KINEMATIC_STAMPER "nifalcon_kinematic_stamper")
SET(LIBNIFALCON_CPP_CLI_BASE "nifalcon_clibase")
SET(LIBNIFALCON_CPP_BOOST_THREAD "nifalcon_boost_thread_device")

#common library additions

IF(UNIX)
  SET(LIBMATH "m")
ENDIF(UNIX)

#default install directories

IF(UNIX)
  IF(NOT INCLUDE_INSTALL_DIR)
	SET(INCLUDE_INSTALL_DIR /usr/local/include)
  ENDIF(NOT INCLUDE_INSTALL_DIR)
  IF(NOT LIBRARY_INSTALL_DIR)
	SET(LIBRARY_INSTALL_DIR /usr/local/lib)
  ENDIF(NOT LIBRARY_INSTALL_DIR)
  IF(NOT RUNTIME_INSTALL_DIR)
	SET(RUNTIME_INSTALL_DIR /usr/local/bin)
  ENDIF(NOT RUNTIME_INSTALL_DIR)
ENDIF(UNIX)

MESSAGE(STATUS "Include Install Directory: ${INCLUDE_INSTALL_DIR}")
MESSAGE(STATUS "Library Install Directory: ${LIBRARY_INSTALL_DIR}")
MESSAGE(STATUS "Runtime Install Directory: ${RUNTIME_INSTALL_DIR}")

#the two main source directories

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples)
