######################################################################################
# Build function for main library
######################################################################################

SET(LIBRARY_SRCS 
  ${LIBNIFALCON_INCLUDE_FILES}
  core/FalconDevice.cpp 
  core/FalconFirmware.cpp 
  firmware/FalconFirmwareNovintSDK.cpp 
  kinematic/FalconKinematicStamper.cpp)

IF(LIBUSB_1_FOUND)
  LIST(APPEND LIBRARY_SRCS
	"comm/FalconCommLibUSB.cpp" 
	"${LIBNIFALCON_INCLUDE_DIR}/falcon/comm/FalconCommLibUSB.h"
	)
  SET(LIBNIFALCON_DEVICE_DEFINES "-DLIBNIFALCON_USE_LIBUSB")
ELSEIF(LIBFTD2XX_FOUND)
  LIST(APPEND LIBRARY_SRCS
	"comm/FalconCommFTD2XX.cpp" 
	"${LIBNIFALCON_INCLUDE_DIR}/falcon/comm/FalconCommFTD2XX.h"
	)
  SET(LIBNIFALCON_DEVICE_DEFINES "-DLIBNIFALCON_USE_LIBFTD2XX")
ENDIF(LIBUSB_1_FOUND)

BUILDSYS_BUILD_LIB(
  NAME nifalcon
  SOURCES "${LIBRARY_SRCS}" 
  CXX_FLAGS "${LIBNIFALCON_DEVICE_DEFINES}"
  LINK_LIBS "${LIBNIFALCON_REQ_LIBS}"
  LINK_FLAGS FALSE 
  DEPENDS FALSE
  SHOULD_INSTALL TRUE
  VERSION "${LIBNIFALCON_VERSION}"
  )

ADD_SUBDIRECTORY(util)

SET(SRC_GROUPS
  Core
  Kinematic
  GMTL
  Comm
  Grip
  Firmware
  Util
)

FOREACH(GROUP ${SRC_GROUPS})
  STRING(TOLOWER ${GROUP} LOWER_GROUP)
  SOURCE_GROUP(${GROUP} REGULAR_EXPRESSION "${LOWER_GROUP}")
ENDFOREACH(GROUP ${SRC_GROUPS})
	