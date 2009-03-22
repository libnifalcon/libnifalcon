######################################################################################
# Installation of headers
######################################################################################

INSTALL(DIRECTORY 
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/gmtl 
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/core
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/firmware
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/kinematic
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/grip
  DESTINATION ${INCLUDE_INSTALL_DIR}/falcon
  )

INSTALL(FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/util/FalconFirmwareBinaryTest.h
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/util/FalconFirmwareBinaryNvent.h
  CONFIGURATIONS Debug Release
  DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/util
)

#Headers for comm or util library specific development

IF(LIBUSB_1_FOUND)
  INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/falcon/comm/FalconCommLibUSB.h DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/comm)
ENDIF(LIBUSB_1_FOUND)

IF(LIBFTDI_FOUND)
  INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/falcon/comm/FalconCommLibFTDI.h DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/comm)
ENDIF(LIBFTDI_FOUND)

IF(LIBFTD2XX_FOUND)
  INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/falcon/comm/FalconCommFTD2XX.h DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/comm)
ENDIF(LIBFTD2XX_FOUND)

IF(Boost_PROGRAM_OPTIONS_FOUND)
  INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/falcon/util/FalconCLIBase.h DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/util)
ENDIF(Boost_PROGRAM_OPTIONS_FOUND)

IF(Boost_THREAD_FOUND)
  INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/falcon/util/FalconDeviceBoostThread.h DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/util)
ENDIF(Boost_THREAD_FOUND)
