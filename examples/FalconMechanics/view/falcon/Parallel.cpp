#include "Parallel.h"

//#include "Toolbox.h"

namespace view
{

	const float Parallel::offset = 11.25f;
	const float Parallel::length = 102.5f;

	Parallel::Parallel(gmtl::Vec3f position, float orientation) : Element()
	{
		this->position = position;
		this->orientation = orientation;
	
		osg::Node* nodeJoint = osgDB::readNodeFile("data/joint.stl");
		nodeJoint->setStateSet(createTransparency(0.5));
	
		osg::Node* nodeParallel = osgDB::readNodeFile("data/parallel.stl");
		nodeParallel->setStateSet(createTransparency(0.5));
	
		parallelATransform = new osg::MatrixTransform();
		parallelATransform->setMatrix(osg::Matrix::translate(0, 0, 1.75) *
									  osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 0, 0, 1) *
									  osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 0, -1, 0) *
									  osg::Matrix::translate(11.25, 14, 0));
		parallelATransform->addChild(nodeParallel);
	
		parallelBTransform = new osg::MatrixTransform();
		parallelBTransform->setMatrix(osg::Matrix::translate(0, 0, 1.75) *
									  osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 0, 0, -1) *
									  osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 0, -1, 0) *
									  osg::Matrix::translate(11.25, -14, 0));
		parallelBTransform->addChild(nodeParallel);
	
		// Centering the model
		parallelTransform = new osg::MatrixTransform();
		parallelTransform->setMatrix(osg::Matrix::translate(offset, 0, 0));
	
		// Centering the model
		osg::MatrixTransform* baseJointTransform = new osg::MatrixTransform();
		baseJointTransform->setMatrix(osg::Matrix::translate(0, 3.75f, 0) *
									  osg::Matrix::rotate(osg::DegreesToRadians(-90.0f), 0, 0, 1) * 
									  osg::Matrix::translate(0, 0, 5));
		baseJointTransform->addChild(nodeJoint);
	
		// Centering the model
		osg::MatrixTransform* modelTransform = new osg::MatrixTransform();
		modelTransform->setMatrix(osg::Matrix::translate(0, 3.75f, 0) *
								  osg::Matrix::rotate(osg::DegreesToRadians(-90.0f), 0, 0, 1) * 
								  osg::Matrix::translate(0, 0, 5));
		modelTransform->addChild(nodeJoint);
	
		jointEffectorTransform = new osg::MatrixTransform();
		jointEffectorTransform->addChild(modelTransform);
		jointEffectorTransform->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(180.0f), 0, 1, 0) *
										  osg::Matrix::translate(125, 0, 0));
	
		jointBaseTransform = new osg::MatrixTransform();
		jointBaseTransform->addChild(parallelTransform);
		jointBaseTransform->addChild(modelTransform);
		jointBaseTransform->addChild(jointEffectorTransform);
		jointBaseTransform->addChild(parallelATransform);
		jointBaseTransform->addChild(parallelBTransform);
		jointBaseTransform->setMatrix(osg::Matrix::translate(60, 0, 0) *
									  osg::Matrix::rotate(osg::DegreesToRadians(orientation), 0, 0, 1) * 
									  osg::Matrix::translate(position[0], position[1], position[2]));	
		model->addChild(jointBaseTransform);
	}

	Parallel::~Parallel()
	{
	}

	void Parallel::update(float theta1, float theta2, float theta3)
	{
		jointEffectorTransform->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(180.0f), 0, 1, 0) *
										  osg::Matrix::translate(gmtl::Math::sin(theta3)*length+2*offset, gmtl::Math::cos(theta3)*length, 0));
	
		jointBaseTransform->setMatrix(osg::Matrix::rotate(theta2-theta1, 0, -1, 0) *
									  osg::Matrix::translate(60, 0, 0) *
									  osg::Matrix::rotate(theta1, 0, -1, 0) *
									  osg::Matrix::rotate(osg::DegreesToRadians(orientation), 0, 0, 1) * 
									  osg::Matrix::translate(position[0], position[1], position[2]));
	
		parallelTransform->setMatrix(osg::Matrix::rotate(theta3-osg::DegreesToRadians(90.0f), 0, 0, 1) *
									 osg::Matrix::translate(offset, 0, 0));

		parallelATransform->setMatrix(osg::Matrix::translate(0, 0, 1.75) *
									  osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 0, 0, 1) *
									  osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 0, -1, 0) *
									  osg::Matrix::rotate(osg::DegreesToRadians(90.0f)-theta3, 0, 0, 1) *
									  osg::Matrix::translate(11.25, 14, 0));
	
		parallelBTransform->setMatrix(osg::Matrix::translate(0, 0, 1.75) *
									  osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 0, 0, -1) *
									  osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 0, -1, 0) *
									  osg::Matrix::rotate(osg::DegreesToRadians(90.0f)-theta3, 0, 0, 1) *
									  osg::Matrix::translate(11.25, -14, 0));
	}

}
