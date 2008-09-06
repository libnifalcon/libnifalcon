SET(NIF_COMPILE_FLAGS)
SET(NIF_LINK_FLAGS)

INCLUDE_DIRECTORIES("..")

INSTALL(FILES FalconNovintFirmware.h DESTINATION "/usr/local/include/falcon/comm")
ADD_LIBRARY (nifalcon_novint_firmware FalconNovintFirmware.cpp)    
SET_TARGET_PROPERTIES(nifalcon_novint_firmware PROPERTIES OUTPUT_NAME nifalcon_novint_firmware)
SET_TARGET_PROPERTIES (nifalcon_novint_firmware PROPERTIES VERSION ${LIBNIFVERSION})
INSTALL(TARGETS nifalcon_novint_firmware LIBRARY DESTINATION "/usr/local/lib" ARCHIVE DESTINATION "/usr/local/lib")
IF(APPLE)
  SET_TARGET_PROPERTIES(nifalcon_novint_firmware PROPERTIES INSTALL_NAME_DIR "/usr/local/lib")
ENDIF(APPLE)
