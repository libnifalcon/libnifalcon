//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net
#ifndef INVERSEKINEMATIC_H_
#define INVERSEKINEMATIC_H_

#include "gmtl/gmtl.h"

#include "FalconGeometry.h"

namespace libnifalcon
{
	namespace stamper
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
