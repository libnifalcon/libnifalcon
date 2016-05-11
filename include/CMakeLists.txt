######################################################################################
# Installation of headers
######################################################################################

######################################################################################
# MAIN INCLUDES
######################################################################################

# Set list
set(LIBNIFALCON_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/gmtl 
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/cpp-optparse
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/core
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/firmware
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/kinematic
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/grip
)

# Append LIBNIFALCON_INCLUDE_FILES
foreach(DIR ${LIBNIFALCON_INCLUDE_DIRS})
  file(GLOB_RECURSE HEADER_FILES ${DIR}/*.h)
  list(APPEND LIBNIFALCON_INCLUDE_FILES ${HEADER_FILES})
endforeach()
set(LIBNIFALCON_INCLUDE_FILES ${LIBNIFALCON_INCLUDE_FILES} PARENT_SCOPE)

# Install
install(DIRECTORY ${LIBNIFALCON_INCLUDE_DIRS} DESTINATION ${INCLUDE_INSTALL_DIR}/falcon)

######################################################################################
# UTILITY INCLUDES
######################################################################################

# Set list
SET(LIBNIFALCON_UTIL_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/util/FalconFirmwareBinaryTest.h
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/util/FalconFirmwareBinaryNvent.h
  ${CMAKE_CURRENT_SOURCE_DIR}/falcon/util/FalconDeviceThread.h
  )

# Install
install(FILES ${LIBNIFALCON_UTIL_INCLUDES} DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/util)

######################################################################################
# COMM INCLUDES
######################################################################################

if(LIBUSB1_FOUND)
  set(FALCON_COMM_INCLUDE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/falcon/comm/FalconCommLibUSB.h)
elseif(LIBFTD2XX_FOUND)
  set(FALCON_COMM_INCLUDE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/falcon/comm/FalconCommFTD2XX.h)
endif()

install(FILES ${FALCON_COMM_INCLUDE_FILE} DESTINATION ${INCLUDE_INSTALL_DIR}/falcon/comm)
