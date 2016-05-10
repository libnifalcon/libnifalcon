######################################################################################
# Build function for main library
######################################################################################

set(LIBNIFALCON_LIBRARY_SRCS
  ${LIBNIFALCON_INCLUDE_FILES}
  core/FalconDevice.cpp 
  core/FalconFirmware.cpp 
  firmware/FalconFirmwareNovintSDK.cpp 
  kinematic/FalconKinematicStamper.cpp
  cpp-optparse/OptionParser.cpp)

if(LIBUSB1_FOUND)
  list(APPEND LIBNIFALCON_LIBRARY_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/comm/FalconCommLibUSB.cpp
    ${LIBNIFALCON_INCLUDE_DIR}/falcon/comm/FalconCommLibUSB.h
    )
  set(LIBNIFALCON_DEVICE_DEFINES -DLIBNIFALCON_USE_LIBUSB1)
elseif(LIBFTD2XX_FOUND)
  list(APPEND LIBNIFALCON_LIBRARY_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/comm/FalconCommFTD2XX.cpp
    ${LIBNIFALCON_INCLUDE_DIR}/falcon/comm/FalconCommFTD2XX.h
    )
  set(LIBNIFALCON_DEVICE_DEFINES -DLIBNIFALCON_USE_LIBFTD2XX)
endif()

BUILDSYS_BUILD_LIB(
  NAME nifalcon
  SOURCES ${LIBNIFALCON_LIBRARY_SRCS}
  CXX_FLAGS ${LIBNIFALCON_DEVICE_DEFINES}
  LINK_LIBS ${LIBNIFALCON_REQ_LIBS}
  LINK_FLAGS FALSE 
  DEPENDS FALSE
  SHOULD_INSTALL TRUE
  VERSION ${LIBNIFALCON_VERSION}
  )

add_subdirectory(util)

set(SRC_GROUPS
  Core
  Kinematic
  GMTL
  Comm
  Grip
  Firmware
  Util
)
foreach(GROUP ${SRC_GROUPS})
  string(TOLOWER ${GROUP} LOWER_GROUP)
  source_group(${GROUP} REGULAR_EXPRESSION "${LOWER_GROUP}")
endforeach(GROUP ${SRC_GROUPS})
	
