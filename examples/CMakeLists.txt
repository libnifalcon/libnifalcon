######################################################################################
# Build function for examples
######################################################################################

SET(LIBNIFALCON_EXE_PO_LINK_LIBS
  ${libnifalcon_cli_base_LIBRARY}
  ${LIBNIFALCON_EXE_LINK_LIBS} 
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  )

######################################################################################
# Build function for barrow_mechanics
######################################################################################

SET(SRCS barrow_mechanics/main.cpp)

BUILDSYS_BUILD_EXE(
  NAME barrow_mechanics
  SOURCES "${SRCS}" 
  CXX_FLAGS "${DEFINE}" 
  LINK_LIBS "${LIBNIFALCON_EXE_LINK_LIBS}" 
  LINK_FLAGS FALSE 
  DEPENDS ${libnifalcon_LIBRARY}_DEPEND
  SHOULD_INSTALL TRUE
  )

######################################################################################
# Build function for findfalcons
######################################################################################

SET(SRCS 
  findfalcons/findfalcons.cpp
  )

BUILDSYS_BUILD_EXE(
  NAME findfalcons
  SOURCES "${SRCS}" 
  CXX_FLAGS "${DEFINE}" 
  LINK_LIBS "${LIBNIFALCON_EXE_LINK_LIBS}" 
  LINK_FLAGS FALSE 
  DEPENDS ${libnifalcon_LIBRARY}_DEPEND
  SHOULD_INSTALL TRUE
  )

######################################################################################
# Build function for findfalcons_multi
######################################################################################

SET(SRCS 
  findfalcons_multi/findfalcons_multi.cpp
  )

BUILDSYS_BUILD_EXE(
  NAME findfalcons_multi
  SOURCES "${SRCS}" 
  CXX_FLAGS "${DEFINE}" 
  LINK_LIBS "${LIBNIFALCON_EXE_LINK_LIBS}" 
  LINK_FLAGS FALSE 
  DEPENDS ${libnifalcon_LIBRARY}_DEPEND
  SHOULD_INSTALL TRUE
  )

######################################################################################
# Build function for falcon_led
######################################################################################

#Can't compile without program options
IF(NOT Boost_PROGRAM_OPTIONS_FOUND)
  MESSAGE("Cannot compile falcon_led - Missing Boost Program Options")
ELSE(NOT Boost_PROGRAM_OPTIONS_FOUND)
  SET(SRCS 
    falcon_led/falcon_led.cpp
    )
  
  BUILDSYS_BUILD_EXE(
    NAME falcon_led
    SOURCES "${SRCS}" 
    CXX_FLAGS FALSE
    LINK_LIBS "${LIBNIFALCON_EXE_PO_LINK_LIBS}" 
    LINK_FLAGS FALSE 
    DEPENDS nifalcon_cli_base_DEPEND
    SHOULD_INSTALL TRUE
    )
ENDIF(NOT Boost_PROGRAM_OPTIONS_FOUND)

######################################################################################
# Build function for falcon_test_cli
######################################################################################

#Can't compile without program options
IF(NOT Boost_PROGRAM_OPTIONS_FOUND)
  MESSAGE("Cannot compile falcon_test_cli - Missing Boost Program Options")
ELSE(NOT Boost_PROGRAM_OPTIONS_FOUND)
  SET(SRCS 
    falcon_test_cli/falcon_test_cli.cpp 
    falcon_test_cli/FalconTestBase.cpp 
    falcon_test_cli/FalconLoopTimeTest.cpp 
    falcon_test_cli/FalconCubeTest.cpp 
    falcon_test_cli/FalconWallTest.cpp 
    falcon_test_cli/FalconColorTest.cpp
    )

  BUILDSYS_BUILD_EXE(
    NAME falcon_test_cli
    SOURCES "${SRCS}" 
    CXX_FLAGS FALSE
    LINK_LIBS "${LIBNIFALCON_EXE_PO_LINK_LIBS}" 
    LINK_FLAGS FALSE 
    DEPENDS nifalcon_cli_base_DEPEND
    SHOULD_INSTALL TRUE
    )
ENDIF(NOT Boost_PROGRAM_OPTIONS_FOUND)

######################################################################################
# Build function for falcon_mouse
######################################################################################

IF(WIN32)
  SET(MOUSE_SRCS falcon_mouse/falcon_mouse.cpp falcon_mouse/falcon_mouse_win.cpp)
  SET(MOUSE_LINK_LIBS ${LIBNIFALCON_EXE_PO_LINK_LIBS})
ELSEIF(APPLE)
  SET(MOUSE_SRCS falcon_mouse/falcon_mouse.cpp falcon_mouse/falcon_mouse_osx.cpp)
  SET(MOUSE_LINK_LIBS ${LIBNIFALCON_EXE_PO_LINK_LIBS})
  SET(MOUSE_LINK_FLAGS "-framework Carbon -framework ApplicationServices")
ELSEIF(UNIX)
  FIND_PACKAGE(X11)
  IF(X11_FOUND AND X11_XTest_LIB)
	INCLUDE_DIRECTORIES(${X11_INCLUDE_DIR})
	SET(MOUSE_SRCS falcon_mouse/falcon_mouse.cpp falcon_mouse/falcon_mouse_x11.cpp)      
	SET(MOUSE_LINK_LIBS ${LIBNIFALCON_EXE_PO_LINK_LIBS} ${X11_LIBRARIES} ${X11_XTest_LIB}) 
  ELSE(X11_FOUND AND X11_XTest_LIB)
	MESSAGE(STATUS "Not compiling falcon_mouse example - X11 compiling requires the XTest library")
  ENDIF(X11_FOUND AND X11_XTest_LIB)
ENDIF(WIN32)

IF(MOUSE_SRCS)
  BUILDSYS_BUILD_EXE(
    NAME falcon_mouse
    SOURCES "${MOUSE_SRCS}" 
    LINK_LIBS "${MOUSE_LINK_LIBS}" 
    LINK_FLAGS "${MOUSE_LINK_FLAGS}" 
    CXX_FLAGS FALSE
    DEPENDS ${libnifalcon_LIBRARY}_DEPEND
    SHOULD_INSTALL TRUE
  )
ENDIF(MOUSE_SRCS)