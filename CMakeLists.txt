######################################################################################
# CMake directives
######################################################################################

#Require 2.6 or higher. 
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

INCLUDE( ${CMAKE_SOURCE_DIR}/BuildSysCMakeLib.cmake )

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

OPTION_LIBRARY_BUILD_STATIC(ON)
#We can't build shared on MSVC yet
IF(NOT MSVC)
  OPTION_LIBRARY_BUILD_SHARED(ON)
ENDIF(NOT MSVC)
OPTION_GPROF(OFF)
#OPTION_CREATE_VERSION_FILE(ON "${CMAKE_CURRENT_SOURCE_DIR}/src/version.cc")

#Project specific options

OPTION(FORCE_FTD2XX_BUILD "Force building FTD2XX libraries on non-windows platforms" OFF)
OPTION(STATIC_LINK_SUFFIXES "Add a symbolic link with [library_name]_s on static libraries (for ease in building staticly linked binaries under gcc)" OFF)
OPTION(BUILD_SWIG_BINDINGS "Build Java/Python bindings for libnifalcon" OFF)
OPTION(BUILD_EXAMPLES "Build libnifalcon examples" ON)

######################################################################################
# Project specific package finding
######################################################################################

#Find communications libraries
IF(WIN32 OR FORCE_FTD2XX_BUILD)
  FIND_PACKAGE(libftd2xx REQUIRED)
  IF(LIBFTD2XX_FOUND)
    INCLUDE_DIRECTORIES(${LIBFTD2XX_INCLUDE_DIRS})
    SET(LIBNIFALCON_REQ_LIBS ${LIBFTD2XX_LIBRARIES})
  ENDIF(LIBFTD2XX_FOUND)
ELSEIF(UNIX)
  FIND_PACKAGE(libusb-1.0 REQUIRED)
  IF(LIBUSB_1_FOUND)
    INCLUDE_DIRECTORIES(${LIBUSB_1_INCLUDE_DIRS})
    SET(LIBNIFALCON_REQ_LIBS ${LIBUSB_1_LIBRARIES})
  ENDIF(LIBUSB_1_FOUND)
ENDIF()

IF(NOT LIBFTD2XX_FOUND AND NOT LIBUSB_1_FOUND)
  MESSAGE(FATAL_ERROR "Either ftd2xx or libusb-1.0 is required to build libnifalcon. Check the README file for info.")
ENDIF(NOT LIBFTD2XX_FOUND AND NOT LIBUSB_1_FOUND)

#Find boost, used all over the place
SET(Boost_USE_MULTITHREADED ON)

FIND_PACKAGE(Boost REQUIRED)
IF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "libnifalcon requires a minimum of the Boost 1.33 headers to build")
ENDIF(NOT Boost_FOUND)

FIND_PACKAGE(Boost COMPONENTS program_options thread system)

######################################################################################
# Project specific globals
######################################################################################

#library definitions

SET(LIBNIFALCON_MAJOR_VERSION 1)
SET(LIBNIFALCON_MINOR_VERSION 0)
SET(LIBNIFALCON_BUILD_VERSION 2)

# BUILDSYS_CPACK_INFO(
#   NAME "libnifalcon"
#   MAJOR_VERSION ${LIBNIFALCON_MAJOR_VERSION}
#   MINOR_VERSION ${LIBNIFALCON_MINOR_VERSION}
#   BUILD_VERSION ${LIBNIFALCON_BUILD_VERSION}
#   VENDOR "Nonpolynomial Labs"
#   DESCRIPTION "Open Source Driver for the Novint Falcon Haptic Controller"
#   )
# INCLUDE(CPack)

SET(LIBNIFALCON_VERSION ${LIBNIFALCON_MAJOR_VERSION}.${LIBNIFALCON_MINOR_VERSION}.${LIBNIFALCON_BUILD_VERSION})

#library name definitions

CREATE_LIBRARY_LINK_NAME(nifalcon)
CREATE_LIBRARY_LINK_NAME(nifalcon_cli_base)
CREATE_LIBRARY_LINK_NAME(nifalcon_device_boost_thread)

SET(LIBNIFALCON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

INCLUDE_DIRECTORIES(${LIBNIFALCON_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

#If we build libusb staticly on apple, we need the proper frameworks
IF(BUILD_STATIC AND APPLE)
  LIST(APPEND LIBNIFALCON_EXE_LINK_LIBS "-framework IOKit" "-framework CoreFoundation")
ENDIF(BUILD_STATIC AND APPLE)

LIST(APPEND LIBNIFALCON_EXE_LINK_LIBS 
  ${libnifalcon_LIBRARY}
  ${LIBNIFALCON_REQ_LIBS}
  )

ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
IF(BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(examples)
ENDIF(BUILD_EXAMPLES)
IF(BUILD_SWIG_BINDINGS)
  ADD_SUBDIRECTORY(lang/swig)
ENDIF(BUILD_SWIG_BINDINGS)

#pkg-config (use the CPack information)
IF(UNIX)
  FIND_PACKAGE(PkgConfig)
  if(PKG_CONFIG_EXECUTABLE)
    set(PC_SHORT_NAME "libnifalcon")
    set(PC_LONG_NAME "libnifalcon")
    set(PC_LIBRARY_NAME "libnifalcon")
    set(PC_DESCRIPTION "Open Source Driver for the Novint Falcon Haptic Controller")
    set(PC_VERSION "${LIBNIFALCON_MAJOR_VERSION}.${LIBNIFALCON_MINOR_VERSION}.${LIBNIFALCON_BUILD_VERSION}")
    
    if(LIBUSB_1_FOUND)
      set(PC_REQUIRES "${PC_REQUIRES}libusb-1.0 ")
      set(PC_LINK_FLAGS "${PC_LINK_FLAGS}-lnifalcon_comm_libusb ")
    endif(LIBUSB_1_FOUND)
    
    set(PC_LINK_FLAGS "${lib${PC_LIBRARY_NAME}_LINK_FLAGS} ${PC_LINK_FLAGS}")
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
    set(PKG_CONFIG_DIR "${LIBRARY_INSTALL_DIR}/pkgconfig")
    install(FILES ${PC_CONFIGURED_FILE} DESTINATION ${PKG_CONFIG_DIR})
  endif(PKG_CONFIG_EXECUTABLE)
ENDIF()
