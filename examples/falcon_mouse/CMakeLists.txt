IF(UNIX)
  FIND_PACKAGE(X11)
  IF (X11_FOUND)
	INCLUDE_DIRECTORIES(../../src/cpp ${INC_DIRS} ${X11_INCLUDE_DIR})
	LINK_DIRECTORIES(../../lib ${LINK_DIRS})
	SET(SRCS falcon_mouse.cpp falcon_mouse_linux.cpp)

	ADD_EXECUTABLE(falcon_mouse ${SRCS})
	SET_COMPILE_FLAGS(falcon_mouse ${CPP_DEFINES})
	TARGET_LINK_LIBRARIES(falcon_mouse ${CPP_LINK_LIBS} nifalcon_kinematic_stamper ${X11_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${LIBNIFALCON_CPP_UTIL} ${X11_XTest_LIB} nifalcon_cpp_device_boostthread) 
  ENDIF(X11_FOUND)
ENDIF(UNIX)

