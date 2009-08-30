######################################################################################
# Build function for falcon_test_cli
######################################################################################

#Can't compile without program options
IF(NOT Boost_PROGRAM_OPTIONS_FOUND)
  MESSAGE("Cannot compile falcon_led - Missing Boost Program Options")
  RETURN()
ENDIF(NOT Boost_PROGRAM_OPTIONS_FOUND)

SET(SRCS falcon_led.cpp)

SET(DEPENDS 
  nifalcon_DEPEND 
  nifalcon_cli_base_DEPEND
  )

BUILDSYS_BUILD_EXE(
  NAME falcon_led
  SOURCES "${SRCS}" 
  CXX_FLAGS FALSE
  LINK_LIBS "${BUILDSYS_EXE_PO_LINK_LIBS}" 
  LINK_FLAGS FALSE 
  DEPENDS "${DEPENDS}"
  SHOULD_INSTALL TRUE
  )

