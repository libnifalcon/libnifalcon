######################################################################################
# Build function for falcon_test_cli
######################################################################################

#Can't compile without program options
IF(NOT Boost_PROGRAM_OPTIONS_FOUND)
  MESSAGE("Cannot compile falcon_test_cli - Missing Boost Program Options")
  RETURN()
ENDIF(NOT Boost_PROGRAM_OPTIONS_FOUND)

SET(SRCS 
  falcon_test_cli.cpp 
  FalconTestBase.cpp 
  FalconLoopTimeTest.cpp 
  FalconCubeTest.cpp 
  FalconWallTest.cpp 
  FalconColorTest.cpp
  )

SET(DEPENDS 
  nifalcon_DEPEND 
  nifalcon_cli_base_DEPEND
  )

BUILDSYS_BUILD_EXE(
  NAME falcon_test_cli
  SOURCES "${SRCS}" 
  CXX_FLAGS FALSE
  LINK_LIBS "${BUILDSYS_EXE_PO_LINK_LIBS}" 
  LINK_FLAGS FALSE 
  DEPENDS "${DEPENDS}"
  SHOULD_INSTALL TRUE
  )

