#ifndef FALCONARM_H_
#define FALCONARM_H_

#include "Element.h"

namespace view
{

class Arm : public Element
{
public:
	Arm(gmtl::Vec3f position, float orientation);
	virtual ~Arm();
	void setAngle(float angle);
private:
	gmtl::Vec3f position;
	float orientation;
	osg::MatrixTransform *transform;
};

}

#endif /*FALCONARM_H_*/
