#ifndef FORCE_H_
#define FORCE_H_

#include "Element.h"


namespace view
{

class Force : public Element
{
public:
	Force();
	virtual ~Force();
	void update(gmtl::Point3f position, gmtl::Vec3f force, gmtl::Matrix44f rotation);
private:
	osg::MatrixTransform *transform;
};

}

#endif /*FORCE_H_*/
