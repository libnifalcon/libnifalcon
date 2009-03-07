######################################################################################
# Build function for falcon_test_qt
######################################################################################

#This is currently the only package using Qt, so do the find here
SET(DESIRED_QT_VERSION 4)
FIND_PACKAGE(Qt)
IF(QT4_INSTALLED)
  MESSAGE(STATUS "Found QT4!")
ELSE(QT4_INSTALLED)
  MESSAGE(STATUS "Cannot find QT4")
  RETURN()
ENDIF(QT4_INSTALLED)

#Probably going to use OpenGL to do the cube rendering demo
SET(QT_USE_QTOPENGL 1)
INCLUDE(${QT_USE_FILE})

#CMake will generate the moc and ui headers in the binary directory, so we'll
#have to include that here.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Generating UI headers out of .ui files
SET(Forms_UIS
  falcontestqt.ui
)
QT4_WRAP_UI(Forms_UIS_H ${Forms_UIS})

# Generate MOC files out of headers
SET(Sources_MOC
  falcontestqt.h
)
QT4_WRAP_CPP(Sources_MOC_cpp ${Sources_MOC})

# Regular old sources
SET(Sources_CPP
  main.cpp
  falcontestqt.cpp
)

SET(SRCS ${Sources_CPP} ${Sources_MOC_cpp} ${Forms_UIS_H})

SET(LINK_LIBS ${NP_EXE_CPP_LINK_LIBS} ${QT_LIBRARIES} ${LIBNIFALCON_LIBS})

SET(DEPENDS ${LIBNIFALCON_CPP}_DEPEND)

NP_BUILD_EXE(
  NAME falcon_test_qt
  SOURCES "${SRCS}" 
  CXX_FLAGS "${NP_EXE_CPP_DEFINES}" 
  LINK_LIBS "${LINK_LIBS}" 
  LINK_FLAGS FALSE 
  DEPENDS "${DEPENDS}"
  SHOULD_INSTALL TRUE
  )
