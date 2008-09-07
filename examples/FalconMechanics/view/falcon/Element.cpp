#include "Element.h"

#include <osg/Geometry>
#include <osg/Material>

namespace view
{

	Element::Element()
	{
		model = new osg::Group;
	}

	Element::~Element()
	{
	}

	osg::Group* Element::getModel()
	{
		return model;
	}

	osg::Geode* Element::createAxis(float length)
	{
		// The axis (lines)
		int axisLength = 100; // in mm
		osg::Geometry* axisGeometry = new osg::Geometry();
		osg::Vec3Array* axisVertices = new osg::Vec3Array;
		axisVertices->push_back(osg::Vec3( 0, 0, 0) ); // center
		axisVertices->push_back(osg::Vec3(length, 0, 0) ); // X
		axisVertices->push_back(osg::Vec3( 0, 0, 0) ); // center
		axisVertices->push_back(osg::Vec3( 0, length, 0) ); // Y
		axisVertices->push_back(osg::Vec3( 0, 0, 0) ); // center
		axisVertices->push_back(osg::Vec3( 0, 0, length) ); // Z
		axisGeometry->setVertexArray(axisVertices);
		osg::DrawElementsUInt* axisElement = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
		axisElement->push_back(0);
		axisElement->push_back(1);
		axisElement->push_back(2);
		axisElement->push_back(3);
		axisElement->push_back(4);
		axisElement->push_back(5);
		axisGeometry->addPrimitiveSet(axisElement);
		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); // red
		colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f) ); // green
		colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) ); // blue
		colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); // white 
		osg::TemplateIndexArray
			<unsigned int, osg::Array::UIntArrayType,4,4> *colorIndexArray;
		colorIndexArray
			= new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>;
		colorIndexArray->push_back(0);
		colorIndexArray->push_back(0);
		colorIndexArray->push_back(1);
		colorIndexArray->push_back(1);
		colorIndexArray->push_back(2);
		colorIndexArray->push_back(2);
		axisGeometry->setColorArray(colors);
		axisGeometry->setColorIndices(colorIndexArray);
		axisGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

		osg::Geode* geode(new osg::Geode);
		geode->addDrawable(axisGeometry);
		return geode;
	}

	osg::StateSet* Element::createTransparency(float level)
	{
		osg::StateSet* stateSet = new osg::StateSet();
		osg::Material* material = new osg::Material();
		stateSet->setMode(GL_BLEND, osg::StateAttribute::ON );
		stateSet->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
		stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		material->setTransparency(osg::Material::FRONT_AND_BACK, level);
		stateSet->setAttribute(material);
		return stateSet;
	}

}
