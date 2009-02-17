######################################################################################
# Build function for findfalcons (C++ version)
######################################################################################

SET(SRCS findfalcons.cpp)
BUILD_EXE(findfalcons_cpp "${SRCS}" "${CPP_DEFINES}" "${CPP_LINK_LIBS}" FALSE TRUE)