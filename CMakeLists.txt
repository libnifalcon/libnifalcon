######################################################################################
# CMake directives
######################################################################################

#Require 2.6 or higher. 
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

#See if we've got our cmake modules checked out. If not, advise to do so.
IF(EXISTS "${CMAKE_SOURCE_DIR}/../cmake_modules")
  SET(NP_CMAKE_DIR "${CMAKE_SOURCE_DIR}/../cmake_modules")
  MESSAGE(STATUS "Using outside cmake_modules directory ${NP_CMAKE_DIR}")
ELSEIF(EXISTS "${CMAKE_SOURCE_DIR}/cmake_modules")
  SET(NP_CMAKE_DIR "${CMAKE_SOURCE_DIR}/cmake_modules")
  MESSAGE(STATUS "Using cmake_modules git submodule ${NP_CMAKE_DIR}")
ELSE(EXISTS "${CMAKE_SOURCE_DIR}/cmake_modules")
  MESSAGE(FATAL_ERROR 
	"Cannot find cmake_modules directory for np labs project functions.\n"
	"Make sure you've either put the cmake_modules directory in the same root as your repository directory, or that you've used the cmake_modules submodule (git submodule update --init).\n"
	"NP Labs cmake_module repository is available at git://github.com/qdot/cmake_modules.git"
	)
ENDIF(EXISTS "${CMAKE_SOURCE_DIR}/../cmake_modules")

LIST(APPEND CMAKE_MODULE_PATH ${NP_CMAKE_DIR})

INCLUDE( ${NP_CMAKE_DIR}/NPCMakeLib.cmake )

#setting link directory policy

IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######################################################################################
# Macro for setting up defines for communications libraries
######################################################################################

MACRO(LIBNIFALCON_COMM_VAR_SETUP)
  LIST(APPEND NP_EXE_CPP_LINK_LIBS ${LIBNIFALCON_CPP})
  IF(LIBUSB_1_FOUND)
	LIST(APPEND NP_EXE_CPP_DEFINES -DLIBUSB)
	LIST(APPEND NP_EXE_CPP_LINK_LIBS ${LIBNIFALCON_CPP_COMM_PREFIX}_libusb ${LIBUSB_1_LIBRARIES}) 
  ENDIF(LIBUSB_1_FOUND)

  IF(LIBFTDI_FOUND)
 	LIST(APPEND NP_EXE_CPP_DEFINES -DLIBFTDI)
 	LIST(APPEND NP_EXE_CPP_LINK_LIBS ${LIBNIFALCON_CPP_COMM_PREFIX}_libftdi ${LIBFTDI_LIBRARIES}) 
  ELSEIF(LIBFTD2XX_FOUND)
 	LIST(APPEND NP_EXE_CPP_DEFINES -DLIBFTD2XX)
 	LIST(APPEND NP_EXE_CPP_LINK_LIBS ${LIBNIFALCON_CPP_COMM_PREFIX}_ftd2xx ${LIBFTD2XX_LIBRARIES})
  ENDIF(LIBFTDI_FOUND)
ENDMACRO(LIBNIFALCON_COMM_VAR_SETUP)

######################################################################################
# Project declaration and options
######################################################################################

#Project declaration

PROJECT(LIBNIFALCON)
INITIALIZE_BUILD()

#Common Options

OPTION_LIBRARY_BUILD_STATIC(OFF)
OPTION_LIBRARY_BUILD_SHARED(ON)
OPTION_FAST_MATH(ON)
OPTION_ARCH_OPTS(ON)
OPTION_GPROF(OFF)
OPTION_LOG4CXX(OFF LIBNIFALCON_REQ_LIBS)
#OPTION_CREATE_VERSION_FILE(ON "${CMAKE_CURRENT_SOURCE_DIR}/src/version.cc")

#Project specific options

OPTION(FORCE_FTD2XX_BUILD "Force building FTD2XX libraries on non-windows platforms" OFF)
OPTION(LIBUSB_DEBUG "For LibFTDI debugging - have LibUSB spam debug messages to stdout/stderr (VERY SPAMMY)" OFF)
OPTION(STATIC_LINK_SUFFIXES "Add a symbolic link with [library_name]_s on static libraries (for ease in building staticly linked binaries under gcc)" OFF)

######################################################################################
# Project specific package finding
######################################################################################

#Find communications libraries
IF(WIN32 OR FORCE_FTD2XX_BUILD)
  FIND_PACKAGE(libftd2xx)
ENDIF(WIN32 OR FORCE_FTD2XX_BUILD)

IF(NOT LIBFTD2XX_FOUND)
  FIND_PACKAGE(libusb)
  FIND_PACKAGE(libftdi)
  FIND_PACKAGE(libusb-1.0)
  IF(LIBUSB_1_FOUND)
	INCLUDE_DIRECTORIES(${LIBUSB_1_INCLUDE_DIR})
  ENDIF(LIBUSB_1_FOUND)
ENDIF(NOT LIBFTD2XX_FOUND)

IF(NOT LIBFTDI_FOUND AND NOT LIBFTD2XX_FOUND AND NOT LIBUSB_1_FOUND)
  MESSAGE(FATAL_ERROR "Either ftd2xx, libftdi, or libusb-1.0 is required to build libnifalcon. Check the README file for info.")
ENDIF(NOT LIBFTDI_FOUND AND NOT LIBFTD2XX_FOUND AND NOT LIBUSB_1_FOUND)

#Find boost, used all over the place
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_LIBS ON)

FIND_PACKAGE(Boost REQUIRED COMPONENTS program_options thread)
IF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "libnifalcon requires a minimum of the Boost 1.33 headers to build")
ENDIF(NOT Boost_FOUND)

######################################################################################
# Project specific globals
######################################################################################

#library definitions

SET(LIBNIFALCON_MAJOR_VERSION 1)
SET(LIBNIFALCON_MINOR_VERSION 0)
SET(LIBNIFALCON_BUILD_VERSION beta3)

NP_CPACK_INFO(
  NAME "libnifalcon"
  MAJOR_VERSION ${LIBNIFALCON_MAJOR_VERSION}
  MINOR_VERSION ${LIBNIFALCON_MINOR_VERSION}
  BUILD_VERSION ${LIBNIFALCON_BUILD_VERSION}
  VENDOR "Nonpolynomial Labs"
  DESCRIPTION "Open Source Driver for the Novint Falcon Haptic Controller"
  )
INCLUDE(CPack)

SET(LIBNIFALCON_VERSION ${LIBNIFALCON_MAJOR_VERSION}.${LIBNIFALCON_MINOR_VERSION}.${LIBNIFALCON_BUILD_VERSION})

#library name definitions

SET(LIBNIFALCON_CPP "nifalcon")
SET(LIBNIFALCON_CPP_COMM_PREFIX "nifalcon_comm")
SET(LIBNIFALCON_CPP_CLI_BASE "nifalcon_cli_base")
SET(LIBNIFALCON_CPP_BOOST_THREAD "nifalcon_device_boost_thread")
SET(LIBNIFALCON_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include")

ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples)

# pkg-config (use the CPack information)
FIND_PACKAGE(PkgConfig)
if(PKG_CONFIG_EXECUTABLE)
  set(PC_SHORT_NAME "${NP_CPACK_NAME}")
  set(PC_LONG_NAME "${NP_CPACK_NAME}")
  set(PC_LIBRARY_NAME "${LIBNIFALCON_CPP}")
  set(PC_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
  set(PC_VERSION "${LIBNIFALCON_MAJOR_VERSION}.${LIBNIFALCON_MINOR_VERSION}.${LIBNIFALCON_BUILD_VERSION}")

  if(LIBUSB_1_FOUND)
    set(PC_REQUIRES "${PC_REQUIRES}libusb-1.0 ")
  endif(LIBUSB_1_FOUND)
  if(LIBFTDI_FOUND)
    set(PC_REQUIRES "${PC_REQUIRES}libftdi ")
  endif(LIBFTDI_FOUND)

  set(PC_LINK_FLAGS "${lib${PC_LIBRARY_NAME}_LINK_FLAGS}")
  set(PC_LINK_FLAGS "-l${PC_LIBRARY_NAME} ${PC_LINK_FLAGS}")
  set(PC_COMPILE_FLAGS "${lib${PC_LIBRARY_NAME}_COMPILE_FLAGS}")
  set(PC_CONFIGURED_FILE
    ${CMAKE_BINARY_DIR}/libnifalcon.pc
  )
  MESSAGE("-- Generating ${PC_CONFIGURED_FILE}")
  configure_file(
    ${CMAKE_SOURCE_DIR}/libnifalcon.pc.cmake
    ${PC_CONFIGURED_FILE}
    @ONLY
  )
  set(PKG_CONFIG_DIR "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
  install(FILES ${PC_CONFIGURED_FILE} DESTINATION ${PKG_CONFIG_DIR})
endif(PKG_CONFIG_EXECUTABLE)
