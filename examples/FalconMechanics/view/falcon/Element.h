#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <osg/Group>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/Drawable>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Matrix>
#include <osg/MatrixTransform>

#include "gmtl/gmtl.h"

namespace view
{

class Element
{
public:
	Element();
	virtual ~Element();
	osg::Group* getModel();
	
	static osg::Geode* createAxis(float length);
	
	static osg::StateSet* createTransparency(float level);
	
protected:
	osg::Group* model;
};

}

#endif /*ELEMENT_H_*/
