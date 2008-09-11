INCLUDE_DIRECTORIES( ${GLUI_INCLUDE_DIR} ${GLUT_INCLUDE_DIR} ${ODE_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../../src/cpp )
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

AUX_SOURCE_DIRECTORY(. MAIN_SOURCE)
ADD_EXECUTABLE(falcon main.cpp 
  view/Viewer.cpp 
  view/Toolbox.cpp 
  view/falcon/Falcon.cpp 
  view/falcon/Element.cpp 
  view/falcon/Arm.cpp
  view/falcon/Base.cpp
  view/falcon/Effector.cpp
  view/falcon/Joint.cpp
  view/falcon/Parallel.cpp
  view/falcon/Force.cpp
  controller/Controller.cpp)

TARGET_LINK_LIBRARIES(falcon
  ${OSG_LIBRARY}
  ${OSGVIEWER_LIBRARY}
  ${OSGGA_LIBRARY}
  ${OSGDB_LIBRARY}
  ${OSGTEXT_LIBRARY}
  ${OSGFX_LIBRARY}
#  ${BOOST_THREAD_MT}
  ${CPP_LINK_LIBS}
  ${GLUI_LIBRARIES}
  ${ODE_LIBRARY}
  ${Boost_LIBRARIES}
  ${GLUT_glut_LIBRARY}
  ${LIBNIFALCON_CPP_KINEMATIC_STAMPER}
  )

SET(CMAKE_CXX_FLAGS_RELEASE -O3)

IF(UNIX AND NOT APPLE)
  TARGET_LINK_LIBRARIES(falcon m ${GLUT_glut_LIBRARY} ${GLUI_LIBRARIES} ${ODE_LIBRARY} ${Boost_LIBRARIES}) 
ENDIF(UNIX AND NOT APPLE)
IF(APPLE)
#  TARGET_LINK_LIBRARIES(falcon m ${ODE_LIBRARY} ${Boost_LIBRARIES} ${GLUI_LIBRARIES}) 
  SET_TARGET_PROPERTIES(falcon PROPERTIES LINK_FLAGS "-framework OpenGL -framework IOKit -framework Carbon -framework GLUT -framework GLUI")
ENDIF(APPLE)