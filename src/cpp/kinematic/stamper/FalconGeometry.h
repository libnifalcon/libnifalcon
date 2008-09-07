//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net
#ifndef STAMPERUTILS_H_
#define STAMPERUTILS_H_

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{	
		enum arm_id
		{
			ARM_1 = 0, ARM_2, ARM_3
		};	

		enum theta_id
		{
			THETA_1 = 0, THETA_2, THETA_3
		};

		enum coord
		{
			X = 0, Y, Z
		};

		struct Angle
		{
			float theta1[3];
			float theta2[3];
			float theta3[3];
		};
	}
}
#endif /*FALCONGEOMETRY_H_*/
