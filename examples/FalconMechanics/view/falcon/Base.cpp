#include "Base.h"

using namespace gmtl::Math;
using namespace gmtl;

namespace view
{

Base::Base() :
	Element()
{
	float motorDistance = 46.188;
	// Positions des moteurs par rapport au referenciel des moteurs
	Vec3f motor1(sin(deg2Rad(36.28f))*motorDistance, cos(deg2Rad(36.28f))
			*motorDistance, 0);
	Vec3f motor3(-(cos(deg2Rad(36.28f-30.0f))*motorDistance),
			sin(deg2Rad(36.28f-30.0f))*motorDistance, 0);
	Vec3f motor2(sin(deg2Rad(60.0f-36.28f))*motorDistance, -(cos(deg2Rad(60.0f
			-36.28f))*motorDistance), 0);
	
	osg::Node* node = osgDB::readNodeFile("data/base.stl");
	node->setStateSet(createTransparency(0.5));
	
	// Centering the model
	osg::MatrixTransform* modelTransform = new osg::MatrixTransform();
	modelTransform->setMatrix(osg::Matrix::translate(0, 0, -5));
	modelTransform->addChild(node);

	osg::MatrixTransform* transform = new osg::MatrixTransform();
	transform->addChild(modelTransform);
	transform->setMatrix(osg::Matrix::translate( 0, 0, 0));

	model->addChild(transform);
}

Base::~Base()
{
}

}
