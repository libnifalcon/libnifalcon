######################################################################################
# Build function for findfalcons (C++ version)
######################################################################################

SET(SRCS main.cpp)
LIST(APPEND NP_EXE_CPP_LINK_LIBS nifalcon_device_boost_thread)

FOREACH(COMM_TYPE_DEFINE ${NP_COMM_TYPES})
  SET(DEFINE "-D${COMM_TYPE_DEFINE}")
  NP_BUILD_EXE(
	NAME barrow_mechanics_${COMM_TYPE_DEFINE} 
	SOURCES "${SRCS}" 
	CXX_FLAGS "${DEFINE}" 
	LINK_LIBS "${NP_EXE_CPP_LINK_LIBS}" 
	LINK_FLAGS FALSE 
	DEPENDS ${LIBNIFALCON_CPP}_DEPEND
	SHOULD_INSTALL TRUE
	)
ENDFOREACH(COMM_TYPE_DEFINE "${NP_EXE_CPP_DEFINES}")