INCLUDE_DIRECTORIES(../../src/cpp ${INC_DIRS})
LINK_DIRECTORIES(../../lib ${LINK_DIRS})
SET(SRCS findfalcons.cpp)

ADD_EXECUTABLE(findfalcons_cpp ${SRCS})
SET_COMPILE_FLAGS(findfalcons_cpp ${CPP_DEFINES})
TARGET_LINK_LIBRARIES(findfalcons_cpp ${CPP_LINK_LIBS})
IF(NOT WIN32)
  INSTALL(TARGETS findfalcons_cpp RUNTIME DESTINATION "/usr/local/bin")
ENDIF(NOT WIN32)

