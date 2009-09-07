######################################################################################
# Installation of headers
######################################################################################

SET(LIBNIFALCON_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/gmtl 
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/core
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/firmware
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/kinematic
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/grip
)

INSTALL(DIRECTORY 
  ${LIBNIFALCON_INCLUDE_DIRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/falcon
  )

INSTALL(FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/util/FalconFirmwareBinaryTest.h
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/util/FalconFirmwareBinaryNvent.h
  CONFIGURATIONS Debug Release
  DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/util
)

FOREACH(DIR ${LIBNIFALCON_INCLUDE_DIRS})
  FILE(GLOB_RECURSE HEADER_FILES ${DIR}/*.h)
  LIST(APPEND LIBNIFALCON_INCLUDE_FILES ${HEADER_FILES})
ENDFOREACH(DIR ${LIBNIFALCON_INCLUDE_DIRS})
#Headers for comm or util library specific development

SET(LIBNIFALCON_INCLUDE_FILES ${LIBNIFALCON_INCLUDE_FILES} PARENT_SCOPE)

#Only install one of the comm headers
IF(LIBUSB_1_FOUND)
  INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/falcon/comm/FalconCommLibUSB.h DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/comm)
ELSEIF(LIBFTD2XX_FOUND)
  INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/falcon/comm/FalconCommFTD2XX.h DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/comm)
ENDIF(LIBUSB_1_FOUND)

IF(Boost_PROGRAM_OPTIONS_FOUND)
  INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/falcon/util/FalconCLIBase.h DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/util)
ENDIF(Boost_PROGRAM_OPTIONS_FOUND)

IF(Boost_THREAD_FOUND)
  INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/falcon/util/FalconDeviceBoostThread.h DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/util)
ENDIF(Boost_THREAD_FOUND)
