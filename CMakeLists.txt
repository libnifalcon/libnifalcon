######################################################################################
# CMake directives
######################################################################################

#Require 2.6 or higher. 
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

#See if we've got our cmake modules checked out. If not, advise to do so.
IF(EXISTS "${CMAKE_SOURCE_DIR}/../compily_buildd/cmake")
  SET(BUILDSYS_CMAKE_DIR "${CMAKE_SOURCE_DIR}/../compily_buildd/cmake")
  MESSAGE(STATUS "Using outside compily_buildd directory ${BUILDSYS_CMAKE_DIR}")
ELSEIF(EXISTS "${CMAKE_SOURCE_DIR}/compily_buildd/cmake")
  SET(BUILDSYS_CMAKE_DIR "${CMAKE_SOURCE_DIR}/compily_buildd/cmake")
  MESSAGE(STATUS "Using compily_buildd git submodule ${BUILDSYS_CMAKE_DIR}")
ELSE(EXISTS "${CMAKE_SOURCE_DIR}/compily_buildd/cmake")
  MESSAGE(FATAL_ERROR 
    "Cannot find compily_buildd directory for np labs project compilation functions.\n"
    "Make sure you've either put the compily_buildd directory in the same root as your repository directory, or that you've used the compily_buildd submodule (git submodule update --init).\n"
    "NP Labs build repository is available at git://github.com/qdot/compily_buildd.git"
    )
ENDIF(EXISTS "${CMAKE_SOURCE_DIR}/../compily_buildd/cmake")

LIST(APPEND CMAKE_MODULE_PATH ${BUILDSYS_CMAKE_DIR})

INCLUDE( ${BUILDSYS_CMAKE_DIR}/BuildSysCMakeLib.cmake )

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
OPTION_FAST_MATH(ON)
OPTION_ARCH_OPTS(ON)
OPTION_GPROF(OFF)
#OPTION_LOG4CXX(OFF LIBNIFALCON_REQ_LIBS)
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
  FIND_PACKAGE(libftd2xx)
  IF(LIBFTD2XX_FOUND)
    INCLUDE_DIRECTORIES(${LIBFTD2XX_INCLUDE_DIRS})
    SET(LIBNIFALCON_REQ_LIBS ${LIBFTD2XX_LIBRARIES})
  ENDIF(LIBFTD2XX_FOUND)
ENDIF(WIN32 OR FORCE_FTD2XX_BUILD)

IF(NOT LIBFTD2XX_FOUND)
  FIND_PACKAGE(libusb-1.0)
  IF(LIBUSB_1_FOUND)
    INCLUDE_DIRECTORIES(${LIBUSB_1_INCLUDE_DIRS})
    SET(LIBNIFALCON_REQ_LIBS ${LIBUSB_1_LIBRARIES})
  ENDIF(LIBUSB_1_FOUND)
ENDIF(NOT LIBFTD2XX_FOUND)

IF(NOT LIBFTD2XX_FOUND AND NOT LIBUSB_1_FOUND)
  MESSAGE(FATAL_ERROR "Either ftd2xx or libusb-1.0 is required to build libnifalcon. Check the README file for info.")
ENDIF(NOT LIBFTD2XX_FOUND AND NOT LIBUSB_1_FOUND)

#Find boost, used all over the place
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_LIBS ON)

FIND_PACKAGE(Boost REQUIRED)
IF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "libnifalcon requires a minimum of the Boost 1.33 headers to build")
ENDIF(NOT Boost_FOUND)

FIND_PACKAGE(Boost COMPONENTS program_options thread)

######################################################################################
# Project specific globals
######################################################################################

#library definitions

SET(LIBNIFALCON_MAJOR_VERSION 1)
SET(LIBNIFALCON_MINOR_VERSION 0)
SET(LIBNIFALCON_BUILD_VERSION 1)

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

# pkg-config (use the CPack information)
# FIND_PACKAGE(PkgConfig)
# if(PKG_CONFIG_EXECUTABLE)
#   set(PC_SHORT_NAME "${BUILDSYS_CPACK_NAME}")
#   set(PC_LONG_NAME "${BUILDSYS_CPACK_NAME}")
#   set(PC_LIBRARY_NAME "${LIBNIFALCON_CPP}")
#   set(PC_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
#   set(PC_VERSION "${LIBNIFALCON_MAJOR_VERSION}.${LIBNIFALCON_MINOR_VERSION}.${LIBNIFALCON_BUILD_VERSION}")

#   if(LIBUSB_1_FOUND)
#     set(PC_REQUIRES "${PC_REQUIRES}libusb-1.0 ")
#     set(PC_LINK_FLAGS "${PC_LINK_FLAGS}-lnifalcon_comm_libusb ")
#   endif(LIBUSB_1_FOUND)

#   set(PC_LINK_FLAGS "${lib${PC_LIBRARY_NAME}_LINK_FLAGS} ${PC_LINK_FLAGS}")
#   set(PC_LINK_FLAGS "-l${PC_LIBRARY_NAME} ${PC_LINK_FLAGS}")
#   set(PC_COMPILE_FLAGS "${lib${PC_LIBRARY_NAME}_COMPILE_FLAGS}")
#   set(PC_CONFIGURED_FILE
#     ${CMAKE_BINARY_DIR}/libnifalcon.pc
#     )
#   MESSAGE("-- Generating ${PC_CONFIGURED_FILE}")
#   configure_file(
#     ${CMAKE_SOURCE_DIR}/libnifalcon.pc.cmake
#     ${PC_CONFIGURED_FILE}
#     @ONLY
#     )
#   set(PKG_CONFIG_DIR "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
#   install(FILES ${PC_CONFIGURED_FILE} DESTINATION ${PKG_CONFIG_DIR})
# endif(PKG_CONFIG_EXECUTABLE)
