######################################################################################
# Build function for main library
######################################################################################

SET(PYTHON_SRCS 
  FalconDevice_py.cpp
  )


FIND_PACKAGE(PythonLibs REQUIRED)

INCLUDE_DIRECTORIES(${LIBNIFALCON_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

SET(LIBS ${LIBNIFALCON_CPP} nifalcon_device_default ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARIES})

NP_BUILD_LIB(
  NAME nifalcon_python
  SOURCES "${PYTHON_SRCS}" 
  CXX_FLAGS "-DBOOST_PYTHON_DYNAMIC_LIB" 
  LINK_LIBS "${LIBS}"
  LINK_FLAGS FALSE 
  DEPENDS nifalcon_device_default_DEPEND
  SHOULD_INSTALL TRUE
  VERSION ${LIBNIFALCON_VERSION}
  )

