#Build rules for core libnifalcon C++ library

SET(LIBRARY_SRCS 
  core/FalconDevice.cpp 
  firmware/FalconFirmwareNovintSDK.cpp 
  kinematic/stamper/AngularMatrix.cpp 
  kinematic/stamper/JacobianMatrix.cpp 
  kinematic/stamper/PositionMatrix.cpp 
  kinematic/stamper/DirectKinematic.cpp 
  kinematic/stamper/InverseKinematic.cpp 
  kinematic/FalconKinematicStamper.cpp)

INCLUDE_DIRECTORIES(. ${GMTL_INCLUDE_DIR})

INSTALL(DIRECTORY ${GMTL_INCLUDE_DIR}/gmtl DESTINATION ${INCLUDE_INSTALL_DIR}/falcon)

INSTALL(FILES 
  core/FalconComm.h 
  core/FalconCore.h  
  core/FalconDevice.h
  core/FalconFirmware.h
  core/FalconGeometry.h
  core/FalconGrip.h
  core/FalconKinematic.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/core)

INSTALL(FILES 
  firmware/FalconFirmwareNovintSDK.h 
  DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/firmware)

INSTALL(FILES 
  kinematic/FalconKinematicStamper.h 
  DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/kinematic)

INSTALL(FILES 
  kinematic/stamper/AngularMatrix.h 
  kinematic/stamper/JacobianMatrix.h 
  kinematic/stamper/PositionMatrix.h 
  kinematic/stamper/DirectKinematic.h
  kinematic/stamper/StamperUtils.h
  kinematic/stamper/InverseKinematic.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/kinematic/stamper)

INSTALL(FILES 
  grip/FalconGripFourButton.h 
  DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/grip)

BUILD_NIFALCON_LIB(${LIBNIFALCON_CPP} "${BUILD_TYPES}" "${LIBRARY_SRCS}" FALSE FALSE)

ADD_SUBDIRECTORY(comm)
ADD_SUBDIRECTORY(util)