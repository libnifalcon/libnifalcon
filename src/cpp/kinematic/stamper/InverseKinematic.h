//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net
#ifndef INVERSEKINEMATIC_H_
#define INVERSEKINEMATIC_H_

#include "gmtl/gmtl.h"

#include "kinematic/FalconGeometry.h"
#include "StamperUtils.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{
		class InverseKinematic
		{
		public:
	
			InverseKinematic() {}
			virtual ~InverseKinematic() {}

			static Angle calculate(gmtl::Point3f position);	

		private:
			static void calulateArm(arm_id arm, gmtl::Point3f position, Angle *angle);
		};
	}
}

#endif /*INVERSEKINEMATIC_H_*/
