#Build rules for core libnifalcon C++ library

SET(LIBRARY_SRCS 
  core/FalconDevice.cpp 
  core/FalconFirmware.cpp 
  firmware/FalconFirmwareNovintSDK.cpp 
  kinematic/stamper/AngularMatrix.cpp 
  kinematic/stamper/JacobianMatrix.cpp 
  kinematic/stamper/PositionMatrix.cpp 
  kinematic/stamper/DirectKinematic.cpp 
  kinematic/stamper/InverseKinematic.cpp 
  kinematic/FalconKinematicStamper.cpp)

INCLUDE_DIRECTORIES(${PROJECT_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

BUILD_NIFALCON_LIB(${LIBNIFALCON_CPP} "${BUILD_TYPES}" "${LIBRARY_SRCS}" FALSE FALSE)

ADD_SUBDIRECTORY(comm)
ADD_SUBDIRECTORY(util)