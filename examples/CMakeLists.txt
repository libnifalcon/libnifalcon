######################################################################################
# Build function for examples
######################################################################################

INCLUDE_DIRECTORIES(${LIBNIFALCON_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH} ${LINK_DIRS})

SET(NP_EXE_CPP_DEFINES )
LIBNIFALCON_COMM_VAR_SETUP()

LIST(APPEND NP_EXE_CPP_LINK_LIBS ${LIBNIFALCON_REQ_LIBS})
IF(BUILD_STATIC AND APPLE)
  LIST(APPEND NP_EXE_CPP_LINK_LIBS "-framework IOKit" "-framework CoreFoundation")
ENDIF(BUILD_STATIC AND APPLE)

ADD_SUBDIRECTORY(findfalcons)
ADD_SUBDIRECTORY(ally_mechanics)
ADD_SUBDIRECTORY(falcon_led)
ADD_SUBDIRECTORY(falcon_test_cli)
