#include "Falcon.h"

#include <iostream>

#include "boost/thread.hpp"

using namespace gmtl::Math;
using namespace gmtl;

namespace view
{

	Falcon::Falcon() : Element()
	{
		base = new Base();
		effector = new Effector((gmtl::Vec3f(0,50,100)));
		model->addChild(effector->getModel());
		//model->addChild(createAxis(10));
		model->addChild(base->getModel());
	
		float motorDistance = 46.188;
		// Positions des moteurs par rapport au referenciel des moteurs
		Vec3f motor1(sin(deg2Rad(36.28f))*motorDistance, cos(deg2Rad(36.28f))
					 *motorDistance, 0);
		Vec3f motor3(-(cos(deg2Rad(36.28f-30.0f))*motorDistance),
				     sin(deg2Rad(36.28f-30.0f))*motorDistance, 0);
		Vec3f motor2(sin(deg2Rad(60.0f-36.28f))*motorDistance, -(cos(deg2Rad(60.0f
																			 -36.28f))*motorDistance), 0);
	
		force = new Force();
	
		arms.push_back(new Arm(motor1, 90));
		arms.push_back(new Arm(motor2, 90-120));
		arms.push_back(new Arm(motor3, 90+120));
	
		parallels.push_back(new Parallel(motor1, 90));
		parallels.push_back(new Parallel(motor2, 90-120));
		parallels.push_back(new Parallel(motor3, 90+120));
		
		model->addChild(arms[ARM_1]->getModel());
		model->addChild(arms[ARM_2]->getModel());
		model->addChild(arms[ARM_3]->getModel());

	
		model->addChild(parallels[ARM_1]->getModel());
		model->addChild(parallels[ARM_2]->getModel());
		model->addChild(parallels[ARM_3]->getModel());
	
		model->addChild(force->getModel());
	}

	Falcon::~Falcon()
	{
	}

	Arm* Falcon::getArm(arm_id arm)
	{
		return arms[arm];
	}

	Parallel* Falcon::getParallel(arm_id arm)
	{
		return parallels[arm];
	}

	Effector* Falcon::getEffector()
	{
		return effector;
	}

}
