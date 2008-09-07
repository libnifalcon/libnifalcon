
INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} ${ODE_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} /usr/local/include/bullet ${CMAKE_CURRENT_SOURCE_DIR}/../../src/cpp  )
LINK_DIRECTORIES(/usr/local/lib /opt/local/lib ${Boost_LIBRARY_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

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
	boost_thread
	${CPP_LINK_LIBS}
	${LIBNIFALCON_CPP_KINEMATIC_STAMPER}
)

SET(CMAKE_CXX_FLAGS_RELEASE -O3)

SET(TARGET_LIBRARIES_VARS GLUT_glut_LIBRARY )


IF(WIN32)
    TARGET_LINK_LIBRARIES(falcon glut32 glui ${ODE_LIBRARY} ${Boost_LIBRARIES})
ENDIF(WIN32)
IF(UNIX AND NOT APPLE)
    TARGET_LINK_LIBRARIES(falcon m glut glui ${ODE_LIBRARY} ${Boost_LIBRARIES}) 
ENDIF(UNIX AND NOT APPLE)
IF(APPLE)
    TARGET_LINK_LIBRARIES(falcon m ${ODE_LIBRARY} ${Boost_LIBRARIES} glui boost_thread-mt) 
    SET_TARGET_PROPERTIES(falcon PROPERTIES LINK_FLAGS "-L/opt/local/lib -framework GLUT -framework OpenGL -framework IOKit -framework Carbon")
ENDIF(APPLE)