INCLUDE_DIRECTORIES(../../src/cpp ${INC_DIRS})
LINK_DIRECTORIES(../../lib ${LINK_DIRS})
SET(SRCS findfalcons.cpp)

BUILD_NIFALCON_EXE(findfalcons_cpp "${SRCS}" "${CPP_LINK_LIBS}" FALSE "${CPP_DEFINES}")