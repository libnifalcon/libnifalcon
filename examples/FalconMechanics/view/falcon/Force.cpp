#include "Force.h"

namespace view
{

Force::Force() :
	Element()
{
	// The axis (lines)
	int length = 100; // in mm
	osg::Geometry* axisGeometry = new osg::Geometry();
	osg::Vec3Array* axisVertices = new osg::Vec3Array;
	axisVertices->push_back(osg::Vec3( 0, 0, 0) ); // center
	axisVertices->push_back(osg::Vec3( 0, 0, length) ); // Z
	axisGeometry->setVertexArray(axisVertices);
	osg::DrawElementsUInt* axisElement = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	axisElement->push_back(0);
	axisElement->push_back(1);
	axisGeometry->addPrimitiveSet(axisElement);
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); // white 
	osg::TemplateIndexArray
	<unsigned int, osg::Array::UIntArrayType,4,4> *colorIndexArray;
	colorIndexArray
			= new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>;
	colorIndexArray->push_back(0);
	colorIndexArray->push_back(0);
	axisGeometry->setColorArray(colors);
	axisGeometry->setColorIndices(colorIndexArray);
	axisGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	
	osg::Geode* geode(new osg::Geode);
	geode->addDrawable(axisGeometry);
	
	transform = new osg::MatrixTransform();
	transform->addChild(geode);
	transform->setMatrix(osg::Matrix::translate( 0, 0, 0));

	model->addChild(transform);
}

Force::~Force()
{
}

void Force::update(gmtl::Point3f position, gmtl::Vec3f force, gmtl::Matrix44f rotation)
{
	transform->setMatrix(osg::Matrix(rotation.getData()));
}


}
