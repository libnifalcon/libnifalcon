SET(LIBRARY_SRCS core/FalconDevice.cpp firmware/FalconFirmwareNovintSDK.cpp)
INSTALL(FILES core/FalconDevice.h core/FalconComm.h core/FalconFirmware.h core/FalconGrip.h core/FalconKinematic.h DESTINATION "/usr/local/include/falcon/core")
INSTALL(FILES firmware/FalconFirmwareNovintSDK.h DESTINATION "/usr/local/include/falcon/firmware")
INSTALL(FILES grip/FalconGripFourButton.h DESTINATION "/usr/local/include/falcon/grip")

INCLUDE_DIRECTORIES(.)

ADD_LIBRARY (nifalcon_cpp_static STATIC ${LIBRARY_SRCS})
ADD_LIBRARY (nifalcon_cpp_shared SHARED ${LIBRARY_SRCS})

SET(NIF_COMPILE_FLAGS)
SET(NIF_LINK_FLAGS)

SET_TARGET_PROPERTIES (nifalcon_cpp_static PROPERTIES OUTPUT_NAME ${LIBNIFALCON_CPP})
SET_TARGET_PROPERTIES (nifalcon_cpp_shared PROPERTIES OUTPUT_NAME ${LIBNIFALCON_CPP})
SET_TARGET_PROPERTIES (nifalcon_cpp_static PROPERTIES VERSION ${LIBNIFVERSION})
SET_TARGET_PROPERTIES (nifalcon_cpp_shared PROPERTIES VERSION ${LIBNIFVERSION})

IF(NOT WIN32)
  INSTALL(TARGETS nifalcon_cpp_static nifalcon_cpp_shared LIBRARY DESTINATION "/usr/local/lib" ARCHIVE DESTINATION "/usr/local/lib")
  IF(APPLE)
	SET_TARGET_PROPERTIES(nifalcon_cpp_shared PROPERTIES INSTALL_NAME_DIR "/usr/local/lib")
  ENDIF(APPLE)
ENDIF(NOT WIN32)

ADD_SUBDIRECTORY(comm)
ADD_SUBDIRECTORY(kinematic)
