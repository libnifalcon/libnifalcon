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
SET(LINK_LIBS ${NP_EXE_CPP_LINK_LIBS} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${LIBNIFALCON_CPP_CLI_BASE} ${LIBNIFALCON_CPP_BOOST_THREAD})

SET(DEPENDS ${LIBNIFALCON_CPP}_DEPEND ${LIBNIFALCON_CPP_CLI_BASE}_DEPEND)

NP_BUILD_EXE(
  NAME falcon_test_cli
  SOURCES "${SRCS}" 
  CXX_FLAGS "${NP_EXE_CPP_DEFINES}" 
  LINK_LIBS "${LINK_LIBS}" 
  LINK_FLAGS FALSE 
  DEPENDS "${DEPENDS}"
  SHOULD_INSTALL TRUE
  )

