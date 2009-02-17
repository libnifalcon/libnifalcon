######################################################################################
# Build function for falcon_test_cli
######################################################################################

#Can't compile without program options
IF(NOT Boost_PROGRAM_OPTIONS_FOUND)
  MESSAGE("Cannot compile falcon_test_cli - Missing Boost Program Options")
  RETURN()
ENDIF(NOT Boost_PROGRAM_OPTIONS_FOUND)

INCLUDE_DIRECTORIES(${PROJECT_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

SET(SRCS falcon_test_cli.cpp)
SET(LINK_LIBS ${CPP_LINK_LIBS} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${LIBNIFALCON_CPP_CLI_BASE} ${LIBNIFALCON_CPP_BOOST_THREAD})

BUILD_EXE(falcon_test_cli "${SRCS}" "${CPP_DEFINES}" "${LINK_LIBS}" FALSE TRUE)
ADD_DEPENDENCIES(falcon_test_cli ${LIBNIFALCON_CPP_CLI_BASE})
