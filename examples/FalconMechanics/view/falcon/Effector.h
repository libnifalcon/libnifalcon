#ifndef FALCONEFFECTOR_H_
#define FALCONEFFECTOR_H_

#include "Element.h"

namespace view
{

class Effector : public Element
{
public:
	Effector(gmtl::Vec3f position);
	virtual ~Effector();
	gmtl::Vec3f getPosition();
	void setPosition(gmtl::Point3f position);
private:
	gmtl::Vec3f position;
	osg::MatrixTransform* transform;
};

}

#endif /*FALCONEFFECTOR_H_*/
