######################################################################################
# Build function for findfalcons (C++ version)
######################################################################################

SET(SRCS findfalcons.cpp)

BUILDSYS_BUILD_EXE(
  NAME findfalcons_cpp
  SOURCES "${SRCS}" 
  CXX_FLAGS "${DEFINE}" 
  LINK_LIBS "${BUILDSYS_EXE_LINK_LIBS}" 
  LINK_FLAGS FALSE 
  DEPENDS ${libnifalcon_LIBRARY}_DEPEND
  SHOULD_INSTALL TRUE
  )
