#ifndef VIEWER_H_
#define VIEWER_H_

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osg/Group>

#include <GL/glui.h>

namespace view
{

class Viewer
{
public:
	Viewer();
	virtual ~Viewer();
	
	// begin: glut dispached functions
	void display();

	void reshape(int w, int h);

	void mouseButton(int button, int state, int x, int y);

	void mouseMove(int x, int y);

	void keyboard(unsigned char key, int x, int y);

	void idle();
	// end: glut dispached functions
	
	void setRootModel(osg::Group *root);
	
private:
	osg::ref_ptr<osgViewer::Viewer> viewer;
	osg::ref_ptr<osg::Group> root;
	osg::observer_ptr<osgViewer::GraphicsWindow> window;
};

}

#endif /*VIEWER_H_*/
