######################################################################################
# CMake directives
######################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

IF(EXISTS "${CMAKE_SOURCE_DIR}/../cmake_modules")
  SET(NP_MODULE_DIR "${CMAKE_SOURCE_DIR}/../cmake_modules")
  MESSAGE(STATUS "Using outside cmake_modules directory ${NP_MODULE_DIR}")
ELSEIF(EXISTS "${CMAKE_SOURCE_DIR}/cmake_modules")
  SET(NP_MODULE_DIR "${CMAKE_SOURCE_DIR}/cmake_modules")
  MESSAGE(STATUS "Using cmake_modules git submodule ${NP_MODULE_DIR}")
ELSE(EXISTS "${CMAKE_SOURCE_DIR}/cmake_modules")
  MESSAGE(FATAL_ERROR 
	"Cannot find cmake_modules directory for np labs project functions.\n"
	"Make sure you've either put the cmake_modules directory in the same root as your repository directory, or that you've used the cmake_modules submodule (git submodule update --init).\n"
	"NP Labs cmake_module repository is available at git://github.com/qdot/cmake_modules.git"
	)
ENDIF(EXISTS "${CMAKE_SOURCE_DIR}/../cmake_modules")

IF(NOT CMAKE_MODULE_PATH)
  SET(CMAKE_MODULE_PATH "${NP_MODULE_DIR}")
ELSE(NOT CMAKE_MODULE_PATH)
  LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${NP_MODULE_DIR})
ENDIF(NOT CMAKE_MODULE_PATH)
INCLUDE( ${NP_MODULE_DIR}/NPCMakeLib.cmake )

#setting link directory policy

IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

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
#OPTION_CREATE_VERSION_FILE(ON "${CMAKE_CURRENT_SOURCE_DIR}/src/version.cc")

#Project specific options

OPTION(BUILD_C_LIBNIFALCON "Build the C version of libnifalcon, with findfalcons test utility" OFF)
OPTION(FORCE_FTD2XX_BUILD "Force building FTD2XX libraries on non-windows platforms" OFF)
OPTION(LIBUSB_DEBUG "For LibFTDI debugging - have LibUSB spam debug messages to stdout/stderr (VERY SPAMMY)" OFF)
OPTION(STATIC_LINK_SUFFIXES "Add a symbolic link with [library_name]_s on static libraries (for ease in building staticly linked binaries under gcc)" OFF)
OPTION(DEBUG_LOGGING "Use log4cxx to output debug log messages (requires log4cxx library)" OFF)

######################################################################################
# Project specific package finding
######################################################################################

IF(DEBUG_LOGGING)
  FIND_PACKAGE(Log4Cxx REQUIRED)
ENDIF(DEBUG_LOGGING)

IF(WIN32 OR FORCE_FTD2XX_BUILD)
  MESSAGE(STATUS "Looking for FTD2XX")
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

FIND_PACKAGE(Boost 1.33 COMPONENTS program_options thread)
IF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "libnifalcon requires a minimum of the Boost 1.33 headers to build")
ENDIF(NOT Boost_FOUND)

######################################################################################
# Project specific globals
######################################################################################

IF(NOT LIBFTDI_FOUND AND NOT LIBFTD2XX_FOUND AND NOT LIBUSB_1_FOUND)
  MESSAGE(FATAL_ERROR "Either ftd2xx, libftdi, or libusb-1.0 is required to build libnifalcon. Check the README file for info.")
ENDIF(NOT LIBFTDI_FOUND AND NOT LIBFTD2XX_FOUND AND NOT LIBUSB_1_FOUND)

#library definitions

SET(LIBNIFALCON_VERSION 1.0.beta2)

#library name definitions

SET(LIBNIFALCON_C_PREFIX "nifalcon_c")

SET(LIBNIFALCON_CPP "nifalcon")
SET(LIBNIFALCON_CPP_COMM_PREFIX "nifalcon_comm")
SET(LIBNIFALCON_CPP_CLI_BASE "nifalcon_cli_base")
SET(LIBNIFALCON_CPP_BOOST_THREAD "nifalcon_device_boost_thread")
SET(LIBNIFALCON_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include")
#the two main source directories

ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples)
