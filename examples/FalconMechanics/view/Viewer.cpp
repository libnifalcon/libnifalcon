#include "Viewer.h"

#include <osgDB/ReadFile>

namespace view
{

	Viewer::Viewer()
	{
		viewer = new osgViewer::Viewer();
		window = viewer->setUpViewerAsEmbeddedInWindow(100, 100, 800, 600);
		//osg::Node* node = osgDB::readNodeFile("data/cow.osg");
		//viewer->setSceneData(node);
		viewer->setCameraManipulator(new osgGA::TrackballManipulator);
		viewer->addEventHandler(new osgViewer::StatsHandler);
		viewer->realize();
	}

	Viewer::~Viewer()
	{
	}

	void Viewer::display()
	{
		if (viewer.valid())
		{
			viewer->frame();
		}
	}

	void Viewer::reshape(int w, int h)
	{
		if (window.valid())
		{
			window->resized(window->getTraits()->x, window->getTraits()->y, w, h);
			window->getEventQueue()->windowResize(window->getTraits()->x, window->getTraits()->y, w, h);
		}
	}

	void Viewer::mouseButton(int button, int state, int x, int y)
	{
		if (window.valid())
		{
			if (state==0)
			{
				window->getEventQueue()->mouseButtonPress(x, y, button+1);
			}
			else
			{
				window->getEventQueue()->mouseButtonRelease(x, y, button+1);
			}
		}
	}

	void Viewer::mouseMove(int x, int y)
	{
		if (window.valid())
		{
			window->getEventQueue()->mouseMotion(x, y);
		}
	}

	void Viewer::keyboard(unsigned char key, int x, int y)
	{
		if (window.valid())
		{
			window->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) key );
			window->getEventQueue()->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) key );
		}
	}

	void Viewer::idle()
	{
	}

	void Viewer::setRootModel(osg::Group *root)
	{
		//osg::Node* node = osgDB::readNodeFile("data/cow.osg");
		viewer->setSceneData(root);
	}


}
