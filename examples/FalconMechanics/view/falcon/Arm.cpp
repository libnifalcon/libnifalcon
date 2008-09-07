#include "Arm.h"

//#include "Toolbox.h"

using namespace gmtl::Math;
using namespace gmtl;

namespace view
{

	Arm::Arm(Vec3f position, float orientation) : Element()
	{
		this->position = position;
		this->orientation = orientation;
	
		osg::Node* node = osgDB::readNodeFile("data/arm.stl");
		node->setStateSet(createTransparency(0.5));
	
		// Centering the model
		osg::MatrixTransform* modelTransform = new osg::MatrixTransform();
		modelTransform->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 1, 0, 0) * 
								  osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 0, 1, 0) * 
								  osg::Matrix::translate(0, -10, 0));
		modelTransform->addChild(node);
	
		transform = new osg::MatrixTransform();
		transform->addChild(modelTransform);
		transform->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(orientation), 0, 0, 1) * 
							 osg::Matrix::translate(position[0], position[1], position[2]));
	
		model->addChild(transform);
	}

	Arm::~Arm()
	{
	}

	void Arm::setAngle(float angle)
	{
		transform->setMatrix(osg::Matrix::rotate(angle, 0, -1, 0) *
							 osg::Matrix::rotate(osg::DegreesToRadians(orientation), 0, 0, 1) * 
							 osg::Matrix::translate(position[0], position[1], position[2]));
	}

}

