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
IF(NOT MSVC)
  #We can't build shared on MSVC yet
  OPTION_LIBRARY_BUILD_SHARED(ON)
ENDIF(NOT MSVC)
OPTION_GPROF(OFF)
#OPTION_CREATE_VERSION_FILE(ON "${CMAKE_CURRENT_SOURCE_DIR}/src/version.cc")

#Project specific options
OPTION(FORCE_FTD2XX_BUILD "Force building against FTD2XX libraries instead of the default libusb1" OFF)
OPTION(STATIC_LINK_SUFFIXES "Add a symbolic link with [library_name]_s on static libraries (for ease in building staticly linked binaries under gcc)" OFF)
OPTION(BUILD_SWIG_BINDINGS "Build Java/Python bindings for libnifalcon" OFF)
OPTION(BUILD_EXAMPLES "Build libnifalcon examples" ON)

######################################################################################
# Project specific package finding
######################################################################################

#Find communications libraries
if(FORCE_FTD2XX_BUILD)
  find_package(libftd2xx REQUIRED)
  if(LIBFTD2XX_FOUND)
    message(STATUS "Found libftd2xx:")
    message(STATUS " - Includes: ${LIBFTD2XX_INCLUDE_DIRS}")
    message(STATUS " - Libraries: ${LIBFTD2XX_LIBRARIES}")
    include_directories(${LIBFTD2XX_INCLUDE_DIRS})
    set(LIBNIFALCON_REQ_LIBS ${LIBFTD2XX_LIBRARIES})
  else()
    message(FATAL_ERROR "libftd2xx is required to build libnifalcon. Check the README file for info.")
  endif()
else()
  find_package(libusb-1.0 REQUIRED)
  if(LIBUSB1_FOUND)
    message(STATUS "Found libusb-1.0:")
    message(STATUS " - Includes: ${LIBUSB1_INCLUDE_DIRS}")
    message(STATUS " - Libraries: ${LIBUSB1_LIBRARIES}")
    include_directories(${LIBUSB1_INCLUDE_DIRS})
    set(LIBNIFALCON_REQ_LIBS ${LIBUSB1_LIBRARIES})
  else()
    message(FATAL_ERROR "libusb-1.0 is required to build libnifalcon. Check the README file for info.")
  endif()
endif()

######################################################################################
# Project specific globals
######################################################################################

#library definitions
SET(LIBNIFALCON_MAJOR_VERSION 1)
SET(LIBNIFALCON_MINOR_VERSION 0)
SET(LIBNIFALCON_BUILD_VERSION 2)
SET(LIBNIFALCON_VERSION ${LIBNIFALCON_MAJOR_VERSION}.${LIBNIFALCON_MINOR_VERSION}.${LIBNIFALCON_BUILD_VERSION})

# LibNiFalcon Include directory
SET(LIBNIFALCON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
INCLUDE_DIRECTORIES(${LIBNIFALCON_INCLUDE_DIR})

# LibNiFalcon link libraries
LIST(APPEND LIBNIFALCON_EXE_LINK_LIBS ${LIBNIFALCON_REQ_LIBS} nifalcon)
IF(BUILD_STATIC AND APPLE)
 #If we build libusb staticly on apple, we need the proper frameworks
  LIST(APPEND LIBNIFALCON_EXE_LINK_LIBS "-framework IOKit" "-framework CoreFoundation")
ENDIF(BUILD_STATIC AND APPLE)

# Add sub-directories
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
IF(BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(examples)
ENDIF(BUILD_EXAMPLES)
IF(BUILD_SWIG_BINDINGS)
  ADD_SUBDIRECTORY(lang/swig)
ENDIF(BUILD_SWIG_BINDINGS)

# Packaging
# BUILDSYS_CPACK_INFO(
#   NAME "libnifalcon"
#   MAJOR_VERSION ${LIBNIFALCON_MAJOR_VERSION}
#   MINOR_VERSION ${LIBNIFALCON_MINOR_VERSION}
#   BUILD_VERSION ${LIBNIFALCON_BUILD_VERSION}
#   VENDOR "Nonpolynomial Labs"
#   DESCRIPTION "Open Source Driver for the Novint Falcon Haptic Controller"
#   )
# INCLUDE(CPack)

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
