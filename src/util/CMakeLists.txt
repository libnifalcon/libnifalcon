######################################################################################
# Build function for utilities libraries
######################################################################################

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

SET(DEPEND ${LIBNIFALCON_CPP}_DEPEND)

#Command Line Base Utility
IF(Boost_PROGRAM_OPTIONS_FOUND)
  INSTALL(FILES ${PROJECT_INCLUDE_DIR}/falcon/util/FalconCLIBase.h DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/util)
  SET(CPP_LINK_LIBS ${LIBNIFALCON_CPP} ${Boost_PROGRAM_OPTIONS_LIBRARY})
  LIBNIFALCON_COMM_VAR_SETUP()
  NP_BUILD_LIB(
	NAME ${LIBNIFALCON_CPP_CLI_BASE} 
	SOURCES "FalconCLIBase.cpp" 
	CXX_FLAGS "${CPP_DEFINES}" 
	LINK_LIBS "${CPP_LINK_LIBS}" 
	LINK_FLAGS FALSE 
	DEPENDS "${DEPEND}")
ENDIF(Boost_PROGRAM_OPTIONS_FOUND)

#Boost thread library
IF(Boost_THREAD_FOUND)
  INSTALL(
	FILES ${PROJECT_INCLUDE_DIR}/falcon/util/FalconDeviceBoostThread.h 
	DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/util
	)

  SET(CPP_LINK_LIBS 
	${LIBNIFALCON_CPP} 
	${Boost_THREAD_LIBRARY}
	)

  NP_BUILD_LIB(
	NAME ${LIBNIFALCON_CPP_BOOST_THREAD} 
	SOURCES "FalconDeviceBoostThread.cpp"
	CXX_FLAGS FALSE 
	LINK_LIBS "${CPP_LINK_LIBS}" 
	LINK_FLAGS FALSE 
	DEPENDS "${DEPEND}")
ENDIF(Boost_THREAD_FOUND)
