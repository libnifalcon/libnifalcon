#include "Effector.h"

#include <iostream>

//#include "Toolbox.h"

using namespace gmtl::Math;
using namespace gmtl;

namespace view
{

Effector::Effector(Vec3f position) : Element()
{
	this->position = position;
	
	float Re = 25.0f/sin(deg2Rad(60.0f));
	Vec3f effector1(cos(deg2Rad(30.0f))*Re,sin(deg2Rad(30.0f))*Re,0);
	Vec3f effector3(-(cos(deg2Rad(30.0f))*Re),sin(deg2Rad(30.0f))*Re,0); 
	Vec3f effector2(0,-Re,0);
	
	osg::Node* node = osgDB::readNodeFile("data/effector.stl");
	node->setStateSet(createTransparency(0.5));
	
	// Centering the model
	osg::MatrixTransform* modelTransform = new osg::MatrixTransform();
	modelTransform->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(180.0f), 0, 0, 1) * 
							  osg::Matrix::translate(0, 0, 5));
	modelTransform->addChild(node);
	
	transform = new osg::MatrixTransform();
	transform->addChild(modelTransform);
	//transform->addChild(createAxis(10));
	transform->setMatrix(osg::Matrix::translate(position[0], position[1], position[2]));
	
	model->addChild(transform);
	model->setStateSet(createTransparency(0.5));
}

Effector::~Effector()
{
} 

gmtl::Vec3f Effector::getPosition()
{
	return position;
}

void Effector::setPosition(gmtl::Point3f position)
{
	this->position = position;
	transform->setMatrix(osg::Matrix::translate(position[0], position[1], position[2]));
}

}
