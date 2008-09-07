//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net
#include "InverseKinematic.h"

using namespace gmtl;
using namespace gmtl::Math;

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		Angle InverseKinematic::calculate(gmtl::Point3f position)
		{
			Angle angle;
			calulateArm(ARM_1, position, &angle);
			calulateArm(ARM_2, position, &angle);
			calulateArm(ARM_3, position, &angle);
			return angle;
		}

		void InverseKinematic::calulateArm(arm_id arm, Point3f position, Angle *angle)
		{
			float pu = -r+cos(phy[arm])*position[X]+sin(phy[arm])*position[Y];
			float pv = -(s-cos(phy[arm])*position[Y]+sin(phy[arm])*position[X]);
			float pw = position[Z];
	
			float theta3 = aCos((pv+f)/b);
	
			float l0 = pow(pw,2) + pow(pu,2) + 2*c*pu - 2*a*pu + pow(a,2) + pow(c,2) - pow(d,2) - pow(e,2) 
				- pow(b,2)*pow(sin(theta3),2) - 2*b*e*sin(theta3) - 2*b*d*sin(theta3) - 2*d*e - 2*a*c;
	
			float l1 = -4*a*pw;
	
			float l2 = pow(pw,2) + pow(pu,2) + 2*c*pu + 2*a*pu + pow(a,2) + pow(c,2) - pow(d,2) - pow(e,2) 
				- pow(b,2)*pow(sin(theta3),2) - 2*b*e*sin(theta3) - 2*b*d*sin(theta3) - 2*d*e + 2*a*c;
	
			float t = -(sqrt(pow(l1,2)-4*l2*l0)+l1)/(2*l2);
		
			float theta1 = aTan(t)*2;
	
			float theta2 = aCos(-(a*cos(theta1)-c-pu)/(b*sin(theta3)+d+e));
	
			angle->theta1[arm] = theta1;
			angle->theta2[arm] = theta2;
			angle->theta3[arm] = theta3;
		}
	}
}
