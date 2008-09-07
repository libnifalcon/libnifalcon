#include "Joint.h"

namespace view
{

	Joint::Joint() : Element()
	{
		float length = 11.25f;
		osg::Geometry* geometry = new osg::Geometry();
		osg::Vec3Array* vertices = new osg::Vec3Array;
		vertices->push_back(osg::Vec3( 0, 0, 0) );
		vertices->push_back(osg::Vec3( length, 0, 0) );
		geometry->setVertexArray(vertices);
		osg::DrawElementsUInt* element = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
		element->push_back(0);
		element->push_back(1);
		geometry->addPrimitiveSet(element);
	
		osg::Geode* geode(new osg::Geode);
		geode->addDrawable(geometry);
	
		osg::Node* node = osgDB::readNodeFile("data/joint.stl");
		node->setStateSet(createTransparency(0.5));
	
		// Centering the model
		osg::MatrixTransform* modelTransform = new osg::MatrixTransform();
		modelTransform->setMatrix(osg::Matrix::translate(0, 3.75f, 0) *
								  osg::Matrix::rotate(osg::DegreesToRadians(-90.0f), 0, 0, 1) * 
								  osg::Matrix::translate(0, 0, 5));
		modelTransform->addChild(node);
	
		osg::MatrixTransform *transform = new osg::MatrixTransform();
		transform->addChild(geode);
		transform->addChild(modelTransform);
	
		model->addChild(transform);
	}

	Joint::~Joint()
	{
	}

}
