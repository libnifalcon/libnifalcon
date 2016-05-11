######################################################################################
# Build function for examples
######################################################################################

set(LIBNIFALCON_EXE_PO_LINK_LIBS ${LIBNIFALCON_EXE_LINK_LIBS} nifalcon_cli_base)

######################################################################################
# Build function for barrow_mechanics
######################################################################################

set(SRCS barrow_mechanics/main.cpp)

BUILDSYS_BUILD_EXE(
  NAME barrow_mechanics
  SOURCES ${SRCS}
  CXX_FLAGS ${DEFINE}
  LINK_LIBS ${LIBNIFALCON_EXE_LINK_LIBS}
  LINK_FLAGS FALSE 
  DEPENDS nifalcon
  SHOULD_INSTALL TRUE
  )

######################################################################################
# Build function for findfalcons
######################################################################################

set(SRCS 
  findfalcons/findfalcons.cpp
  )

BUILDSYS_BUILD_EXE(
  NAME findfalcons
  SOURCES ${SRCS} 
  CXX_FLAGS ${DEFINE}
  LINK_LIBS ${LIBNIFALCON_EXE_LINK_LIBS}
  LINK_FLAGS FALSE 
  DEPENDS nifalcon
  SHOULD_INSTALL TRUE
  )

######################################################################################
# Build function for findfalcons_multi
######################################################################################

set(SRCS 
  findfalcons_multi/findfalcons_multi.cpp
  )

BUILDSYS_BUILD_EXE(
  NAME findfalcons_multi
  SOURCES ${SRCS}
  CXX_FLAGS ${DEFINE}
  LINK_LIBS ${LIBNIFALCON_EXE_LINK_LIBS}
  LINK_FLAGS FALSE 
  DEPENDS nifalcon
  SHOULD_INSTALL TRUE
  )

######################################################################################
# Build function for falcon_led
######################################################################################

set(SRCS
  falcon_led/falcon_led.cpp
)
  
BUILDSYS_BUILD_EXE(
    NAME falcon_led
    SOURCES ${SRCS}
    CXX_FLAGS FALSE
    LINK_LIBS ${LIBNIFALCON_EXE_PO_LINK_LIBS}
    LINK_FLAGS FALSE 
    DEPENDS nifalcon_cli_base
    SHOULD_INSTALL TRUE
)


######################################################################################
# Build function for falcon_test_cli
######################################################################################

set(SRCS
    falcon_test_cli/falcon_test_cli.cpp 
    falcon_test_cli/FalconTestBase.cpp 
    falcon_test_cli/FalconLoopTimeTest.cpp 
    falcon_test_cli/FalconCubeTest.cpp 
    falcon_test_cli/FalconSphereTest.cpp 
    falcon_test_cli/FalconWallTest.cpp 
    falcon_test_cli/FalconColorTest.cpp
)

BUILDSYS_BUILD_EXE(
    NAME falcon_test_cli
    SOURCES ${SRCS}
    CXX_FLAGS FALSE
    LINK_LIBS ${LIBNIFALCON_EXE_PO_LINK_LIBS}
    LINK_FLAGS FALSE 
    DEPENDS nifalcon_cli_base
    SHOULD_INSTALL TRUE
)

######################################################################################
# Build function for falcon_mouse
######################################################################################

  if(WIN32)
	  set(MOUSE_SRCS falcon_mouse/falcon_mouse.cpp falcon_mouse/falcon_mouse_win.cpp)
	  set(MOUSE_LINK_LIBS ${LIBNIFALCON_EXE_PO_LINK_LIBS})
  elseif(APPLE)
	  set(MOUSE_SRCS falcon_mouse/falcon_mouse.cpp falcon_mouse/falcon_mouse_osx.cpp)
	  set(MOUSE_LINK_LIBS ${LIBNIFALCON_EXE_PO_LINK_LIBS})
	  set(MOUSE_LINK_FLAGS "-framework Carbon -framework ApplicationServices")
  elseif(UNIX)
	  find_package(X11)
	  if(X11_FOUND AND X11_XTest_LIB)
	    include_directories(${X11_INCLUDE_DIR})
	    set(MOUSE_SRCS falcon_mouse/falcon_mouse.cpp falcon_mouse/falcon_mouse_x11.cpp)      
	    set(MOUSE_LINK_LIBS ${LIBNIFALCON_EXE_PO_LINK_LIBS} ${X11_LIBRARIES} ${X11_XTest_LIB}) 
	  else(X11_FOUND AND X11_XTest_LIB)
	    message(STATUS "Not compiling falcon_mouse example - X11 compiling requires the XTest library")
	  endif()
  endif()
  
  if(MOUSE_SRCS)
    BUILDSYS_BUILD_EXE(
        NAME falcon_mouse
        SOURCES ${MOUSE_SRCS}
        LINK_LIBS ${MOUSE_LINK_LIBS}
        LINK_FLAGS ${MOUSE_LINK_FLAGS}
        CXX_FLAGS FALSE
        DEPENDS nifalcon
        SHOULD_INSTALL TRUE
      )
  endif(MOUSE_SRCS)
