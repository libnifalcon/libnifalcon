#Generalized library building function for all C++ libraries
FUNCTION(BUILD_NIFALCON_LIB LIB_NAME LIB_TYPES SRCS LINK_LIBS CXX_DEFINES)
  FOREACH(LIB_TYPE ${LIB_TYPES})
	SET(CURRENT_LIB ${LIB_NAME}_${LIB_TYPE})
	ADD_LIBRARY (${CURRENT_LIB} ${LIB_TYPE} ${SRCS})

	SET_TARGET_PROPERTIES (${CURRENT_LIB} PROPERTIES OUTPUT_NAME ${LIB_NAME})
	SET_TARGET_PROPERTIES (${CURRENT_LIB} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	SET_TARGET_PROPERTIES (${CURRENT_LIB} PROPERTIES VERSION ${LIBNIFALCON_VERSION})

	#optional arguments
	IF(LINK_LIBS)
	  FOREACH(LINK_LIB ${LINK_LIBS})
		TARGET_LINK_LIBRARIES(${CURRENT_LIB} ${LINK_LIB})
	  ENDFOREACH(LINK_LIB)
	ENDIF(LINK_LIBS)

	#cpp defines
	IF(CXX_DEFINES)
	  SET_COMPILE_FLAGS(${CURRENT_LIB} ${CXX_DEFINES})
	ENDIF(CXX_DEFINES)

	#installation for non-windows platforms
	IF(NOT WIN32)
	  INSTALL(TARGETS ${CURRENT_LIB} LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR} ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR})
	ENDIF(NOT WIN32)

	#rewrite of install_name_dir in apple binaries
	IF(APPLE)
	  SET_TARGET_PROPERTIES(${CURRENT_LIB} PROPERTIES INSTALL_NAME_DIR ${LIBRARY_INSTALL_DIR})
	ENDIF(APPLE)
  ENDFOREACH(LIB_TYPE)
ENDFUNCTION(BUILD_NIFALCON_LIB)

SET(BUILD_TYPES STATIC SHARED)

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