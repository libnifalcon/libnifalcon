######################################################################################
# Build function for findfalcons (C++ version)
######################################################################################

SET(SRCS findfalcons.cpp)
LIST(APPEND NP_EXE_CPP_LINK_LIBS nifalcon_device_boost_thread)
NP_BUILD_EXE(
  NAME findfalcons_cpp 
  SOURCES "${SRCS}" 
  CXX_FLAGS "${NP_EXE_CPP_DEFINES}" 
  LINK_LIBS "${NP_EXE_CPP_LINK_LIBS}" 
  LINK_FLAGS FALSE 
  DEPENDS ${LIBNIFALCON_CPP}_DEPEND
  SHOULD_INSTALL TRUE
  )