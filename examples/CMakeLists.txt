######################################################################################
# Build function for examples
######################################################################################

INCLUDE_DIRECTORIES(${LIBNIFALCON_INCLUDE_DIRS})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH} ${LINK_DIRS})

LIBNIFALCON_COMM_VAR_SETUP()

ADD_SUBDIRECTORY(findfalcons)
ADD_SUBDIRECTORY(falcon_test_cli)
ADD_SUBDIRECTORY(falcon_test_qt)
