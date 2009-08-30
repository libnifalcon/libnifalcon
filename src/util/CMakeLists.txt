######################################################################################
# Build function for utilities libraries
######################################################################################

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

#Command Line Base Utility
IF(Boost_PROGRAM_OPTIONS_FOUND)
  SET(CPP_LINK_LIBS 
    ${libnifalcon_LIBRARY}
    ${LIBNIFALCON_REQ_LIBS}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    )

  SET(SRCS
	"FalconCLIBase.cpp" 
	"${LIBNIFALCON_INCLUDE_DIR}/falcon/util/FalconCLIBase.h"
	)
  BUILDSYS_BUILD_LIB(
	NAME nifalcon_cli_base
	SOURCES "${SRCS}"
	CXX_FLAGS "${BUILDSYS_EXE_CPP_DEFINES}" 
	LINK_LIBS "${CPP_LINK_LIBS}" 
	LINK_FLAGS FALSE 
	DEPENDS nifalcon_DEPEND
	SHOULD_INSTALL TRUE
	VERSION ${LIBNIFALCON_VERSION}
	)
ENDIF(Boost_PROGRAM_OPTIONS_FOUND)

#Boost thread library
IF(Boost_THREAD_FOUND)
  SET(CPP_LINK_LIBS 
    ${libnifalcon_LIBRARY}
    ${LIBNIFALCON_REQ_LIBS}
    ${Boost_THREAD_LIBRARY}
    )

  SET(SRCS
	"FalconDeviceBoostThread.cpp" 
	"${LIBNIFALCON_INCLUDE_DIR}/falcon/util/FalconDeviceBoostThread.h"
	)
  BUILDSYS_BUILD_LIB(
	NAME nifalcon_device_boost_thread
	SOURCES "${SRCS}"
	CXX_FLAGS FALSE 
	LINK_LIBS "${CPP_LINK_LIBS}" 
	LINK_FLAGS FALSE 
	DEPENDS nifalcon_DEPEND
	SHOULD_INSTALL TRUE
	VERSION ${LIBNIFALCON_VERSION}
	)
ENDIF(Boost_THREAD_FOUND)
