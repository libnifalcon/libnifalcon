######################################################################################
# Build function for falcon_test_qt
######################################################################################

#If we're using a Qt rc, this apparently requires us to explicitly identify it
LIST(APPEND CMAKE_PREFIX_PATH /usr/local/Trolltech/Qt-4.5.0-rc1)
#LIST(APPEND CMAKE_PREFIX_PATH /usr/local/Trolltech/)

#This is currently the only package using Qt, so do the find here
FIND_PACKAGE(Qt)
IF(QT4_INSTALLED)
  MESSAGE(STATUS "Found QT4!")
ELSE(QT4_INSTALLED)
  MESSAGE(STATUS "Cannot find QT4")
  RETURN()
ENDIF(QT4_INSTALLED)

#Probably going to use OpenGL to do the cube rendering demo
SET(QT_USE_OPENGL 1)
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

SET(LINK_LIBS ${QT_LIBRARIES} ${LIBNIFALCON_LIBS})

BUILD_EXE(falcon_test_qt "${SRCS}" FALSE "${LINK_LIBS}" FALSE TRUE)
