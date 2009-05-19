######################################################################################
# Build function for main library
######################################################################################

SET(LIBRARY_SRCS 
  ${LIBNIFALCON_INCLUDE_FILES}
  core/FalconDevice.cpp 
  core/FalconFirmware.cpp 
  firmware/FalconFirmwareNovintSDK.cpp 
  kinematic/stamper/AngularMatrix.cpp 
  kinematic/stamper/JacobianMatrix.cpp 
  kinematic/stamper/PositionMatrix.cpp 
  kinematic/stamper/DirectKinematic.cpp 
  kinematic/stamper/InverseKinematic.cpp 
  kinematic/FalconKinematicStamper.cpp
  kinematic/FalconKinematicZsomborMurray.cpp
)

INCLUDE_DIRECTORIES(${LIBNIFALCON_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

NP_BUILD_LIB(
  NAME ${LIBNIFALCON_CPP} 
  SOURCES "${LIBRARY_SRCS}" 
  CXX_FLAGS FALSE 
  LINK_LIBS "${LIBNIFALCON_REQ_LIBS}"
  LINK_FLAGS FALSE 
  DEPENDS FALSE
  SHOULD_INSTALL TRUE
  VERSION ${LIBNIFALCON_VERSION}
  )

ADD_SUBDIRECTORY(comm)
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
	