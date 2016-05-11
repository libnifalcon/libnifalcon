######################################################################################
# Build function for utilities libraries
######################################################################################

set(CPP_LINK_LIBS ${LIBNIFALCON_REQ_LIBS} nifalcon)

# NIFALCON CLI BASE
set(LIBNIFALCON_CLI_BASE_LIBRARY_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/FalconCLIBase.cpp
	${LIBNIFALCON_INCLUDE_DIR}/falcon/util/FalconCLIBase.h
)

BUILDSYS_BUILD_LIB(
	NAME nifalcon_cli_base
	SOURCES ${LIBNIFALCON_CLI_BASE_LIBRARY_SRCS}
	CXX_FLAGS FALSE
	LINK_LIBS ${CPP_LINK_LIBS}
	LINK_FLAGS FALSE 
	DEPENDS nifalcon
	SHOULD_INSTALL TRUE
	VERSION ${LIBNIFALCON_VERSION}
	)

# NIFALCON DEVICE THREAD
set(LIBNIFALCON_DEVICE_THREAD_LIBRARY_SRCS
   ${CMAKE_CURRENT_SOURCE_DIR}/FalconDeviceThread.cpp
   ${LIBNIFALCON_INCLUDE_DIR}/falcon/util/FalconDeviceThread.h
)

BUILDSYS_BUILD_LIB(
    NAME nifalcon_device_thread
    SOURCES ${LIBNIFALCON_DEVICE_THREAD_LIBRARY_SRCS}
    CXX_FLAGS FALSE
    LINK_LIBS ${CPP_LINK_LIBS}
    LINK_FLAGS FALSE
    DEPENDS nifalcon
    SHOULD_INSTALL TRUE
    VERSION ${LIBNIFALCON_VERSION}
)
