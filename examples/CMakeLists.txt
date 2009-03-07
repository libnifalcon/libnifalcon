######################################################################################
# Build function for examples
######################################################################################

INCLUDE_DIRECTORIES(${LIBNIFALCON_INCLUDE_DIRS})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH} ${LINK_DIRS})

SET(NP_EXE_CPP_DEFINES )
LIBNIFALCON_COMM_VAR_SETUP()

LIST(APPEND NP_EXE_CPP_LINK_LIBS ${LIBNIFALCON_REQ_LIBS})


ADD_SUBDIRECTORY(findfalcons)
ADD_SUBDIRECTORY(falcon_test_cli)
ADD_SUBDIRECTORY(falcon_test_qt)
