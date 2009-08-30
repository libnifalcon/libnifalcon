######################################################################################
# Build function for examples
######################################################################################

INCLUDE_DIRECTORIES(${LIBNIFALCON_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH} ${LINK_DIRS})

IF(BUILD_STATIC AND APPLE)
  LIST(APPEND BUILDSYS_EXE_LINK_LIBS "-framework IOKit" "-framework CoreFoundation")
ENDIF(BUILD_STATIC AND APPLE)

LIST(APPEND BUILDSYS_EXE_LINK_LIBS 
  ${libnifalcon_LIBRARY}
  ${LIBNIFALCON_REQ_LIBS}
  )

SET(BUILDSYS_EXE_PO_LINK_LIBS
  ${libnifalcon_cli_base_LIBRARY}
  ${BUILDSYS_EXE_LINK_LIBS} 
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  )

ADD_SUBDIRECTORY(findfalcons)
ADD_SUBDIRECTORY(barrow_mechanics)
ADD_SUBDIRECTORY(falcon_led)
ADD_SUBDIRECTORY(falcon_test_cli)
