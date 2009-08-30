######################################################################################
# Build function for SWIG
######################################################################################

#Required because it has to be turned on in the first place, so if we don't have it,
#just die
FIND_PACKAGE(SWIG REQUIRED)
MESSAGE(STATUS "Building SWIG Bindings")

INCLUDE(${SWIG_USE_FILE})

SET(CMAKE_SWIG_FLAGS "")

SET(LIBNIFALCON_INTERFACE libnifalcon_swig.i)
SET_SOURCE_FILES_PROPERTIES(${LIBNIFALCON_INTERFACE} PROPERTIES CPLUSPLUS ON)
INCLUDE_DIRECTORIES(../include)

######################################################################################
# Build function for Java library bindings
######################################################################################

FIND_PACKAGE(JNI)

# Apparently there's no FOUND variable for JNI. I really hate the lack of consistency 
# in Cmake find modules. >.<
IF(JNI_LIBRARIES)
  MESSAGE(STATUS "- Build SWIG Java library")
  INCLUDE_DIRECTORIES(${JNI_INCLUDE_DIRS})
  SET(ARGS "-includeall" "-module" "JNIFalcon")

  SET_SOURCE_FILES_PROPERTIES(${LIBNIFALCON_INTERFACE} PROPERTIES SWIG_FLAGS "${ARGS}")
  SET(LIBS_JNI ${LIBNIFALCON_EXE_LINK_LIBS} ${JNI_LIBRARIES})
  SWIG_ADD_MODULE(JNIFalcon java ${LIBNIFALCON_INTERFACE} ../src/FalconDeviceBridge.cpp)
  SWIG_LINK_LIBRARIES(JNIFalcon ${LIBS_JNI})
ELSE(JNI_LIBRARIES)
  MESSAGE(STATUS "--- Cannot make Java bindings, JNI libraries not found!")
ENDIF(JNI_LIBRARIES)

######################################################################################
# Build function for Python library bindings
######################################################################################

FIND_PACKAGE(PythonLibs)
IF(PYTHONLIBS_FOUND)
  MESSAGE(STATUS "- Build SWIG Python library")
  INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
  SET(ARGS "-includeall" "-module" "pynifalcon")

  SET_SOURCE_FILES_PROPERTIES(${LIBNIFALCON_INTERFACE} PROPERTIES SWIG_FLAGS "${ARGS}")
  SET(LIBS_PYTHON ${LIBNIFALCON_EXE_LINK_LIBS} ${PYTHON_LIBRARIES})
  SWIG_ADD_MODULE(pynifalcon python ${LIBNIFALCON_INTERFACE} ../src/FalconDeviceBridge.cpp)
  SWIG_LINK_LIBRARIES(pynifalcon ${LIBS_PYTHON})
ELSE(PYTHONLIBS_FOUND)
  MESSAGE(STATUS "--- Cannot make Python bindings, Python dev libraries not found!")
ENDIF(PYTHONLIBS_FOUND)



