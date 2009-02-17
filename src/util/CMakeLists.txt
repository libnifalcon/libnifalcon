######################################################################################
# Build function for utilities libraries
######################################################################################

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

#Command Line Base Utility
IF(Boost_PROGRAM_OPTIONS_FOUND)
  INSTALL(FILES ${PROJECT_INCLUDE_DIR}/falcon/util/FalconCLIBase.h DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/util)
  SET(CPP_LINK_LIBS ${LIBNIFALCON_CPP} ${Boost_PROGRAM_OPTIONS_LIBRARY})
  LIBNIFALCON_COMM_VAR_SETUP()
  BUILD_LIB(${LIBNIFALCON_CPP_CLI_BASE} "${BUILD_TYPES}" FalconCLIBase.cpp "${CPP_DEFINES}" "${CPP_LINK_LIBS}" FALSE)
ENDIF(Boost_PROGRAM_OPTIONS_FOUND)

#Boost thread library
IF(Boost_THREAD_FOUND)
  INSTALL(FILES ${PROJECT_INCLUDE_DIR}/falcon/util/FalconDeviceBoostThread.h DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/util)
  SET(CPP_LINK_LIBS ${LIBNIFALCON_CPP} ${Boost_THREAD_LIBRARY})
  BUILD_LIB(${LIBNIFALCON_CPP_BOOST_THREAD} "${BUILD_TYPES}" FalconDeviceBoostThread.cpp FALSE "${CPP_LINK_LIBS}" FALSE)
ENDIF(Boost_THREAD_FOUND)
