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

INCLUDE_DIRECTORIES(${PROJECT_INCLUDE_DIR})

INSTALL(DIRECTORY 
  ${PROJECT_INCLUDE_DIR}/falcon/gmtl 
  ${PROJECT_INCLUDE_DIR}/falcon/core
  ${PROJECT_INCLUDE_DIR}/falcon/firmware
  ${PROJECT_INCLUDE_DIR}/falcon/kinematic
  ${PROJECT_INCLUDE_DIR}/falcon/grip
  DESTINATION ${INCLUDE_INSTALL_DIR}/falcon
  REGEX ".svn" EXCLUDE
  REGEX ".git" EXCLUDE
  )


BUILD_NIFALCON_LIB(${LIBNIFALCON_CPP} "${BUILD_TYPES}" "${LIBRARY_SRCS}" FALSE FALSE)

ADD_SUBDIRECTORY(comm)
ADD_SUBDIRECTORY(util)