######################################################################################
# Build function for findfalcons (C++ version)
######################################################################################

SET(SRCS findfalcons.cpp)
LIST(APPEND BUILDSYS_EXE_CPP_LINK_LIBS nifalcon_device_boost_thread)

FOREACH(COMM_TYPE_DEFINE ${BUILDSYS_COMM_TYPES})
  SET(DEFINE "-D${COMM_TYPE_DEFINE}")
  BUILDSYS_BUILD_EXE(
	NAME findfalcons_cpp_${COMM_TYPE_DEFINE} 
	SOURCES "${SRCS}" 
	CXX_FLAGS "${DEFINE}" 
	LINK_LIBS "${BUILDSYS_EXE_CPP_LINK_LIBS}" 
	LINK_FLAGS FALSE 
	DEPENDS ${LIBNIFALCON_CPP}_DEPEND
	SHOULD_INSTALL TRUE
	)
ENDFOREACH(COMM_TYPE_DEFINE "${BUILDSYS_EXE_CPP_DEFINES}")