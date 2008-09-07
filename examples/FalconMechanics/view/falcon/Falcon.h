#ifndef VIEW_FALCON_H_
#define VIEW_FALCON_H_

#include <vector>
#include <osg/Geode>
#include "gmtl/gmtl.h"


#include "Effector.h"
#include "Base.h"
#include "Arm.h"
#include "Parallel.h"
#include "Force.h"

namespace view
{

class Falcon : public Element
{
public:
	enum arm_id
	{
		ARM_1 = 0, ARM_2, ARM_3
	};

	Falcon();
	virtual ~Falcon();
	int runIOloop();
	Arm* getArm(arm_id arm);
	Parallel* getParallel(arm_id arm);
	Effector* getEffector();
	
	Force *force;

private:		
	Effector *effector;
	Base *base;
	std::vector<Arm*> arms;
	std::vector<Parallel*> parallels;
};

}

#endif /*VIEW_FALCON_H_*/
