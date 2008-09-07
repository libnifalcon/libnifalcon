#ifndef PARALLEL_H_
#define PARALLEL_H_

#include "Element.h"

namespace view
{

class Parallel : public Element
{
public:
	Parallel(gmtl::Vec3f position, float orientation);
	virtual ~Parallel();
	void update(float alpha, float beta, float theta);
	
	const static float offset;
	const static float length;
private:
	gmtl::Vec3f position;
	float orientation;
	osg::MatrixTransform *jointBaseTransform;
	osg::MatrixTransform *parallelTransform;
	osg::MatrixTransform *jointEffectorTransform;
	osg::MatrixTransform* parallelATransform;
	osg::MatrixTransform* parallelBTransform;
};

}

#endif /*PARALLEL_H_*/
