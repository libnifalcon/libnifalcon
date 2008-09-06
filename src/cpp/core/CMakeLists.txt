SET(LIBRARY_SRCS FalconDevice.cpp FalconFirmware.cpp)
INSTALL(FILES FalconDevice.h FalconComm.h FalconFirmware.h FalconGrip.h FalconKinematic.h DESTINATION "/usr/local/include/falcon")

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
