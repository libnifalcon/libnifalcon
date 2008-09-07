//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net
#ifndef JACOBIANMATRIX_H_
#define JACOBIANMATRIX_H_

#include "gmtl/gmtl.h"

#include "InverseKinematic.h"
#include "FalconGeometry.h"

namespace libnifalcon
{
	namespace stamper
	{

		class JacobianMatrix
		{
		public:
			JacobianMatrix() {}
			virtual ~JacobianMatrix() {}
	
			static gmtl::Vec3f calculate(Angle angle, gmtl::Vec3f cartesian);
	
			static gmtl::Vec3f calculateInverse(Angle angle, gmtl::Vec3f angular);
	
		private:
			static gmtl::Matrix33f calculateJacobian(Angle angle);
	
			static void calculateJFi(arm_id arm, Angle angle, gmtl::Matrix33f *matrix);
	
			static void calculateJIi(arm_id arm, Angle angle, gmtl::Matrix33f *matrix);
		};

	}
}
#endif /*JACOBIANMATRIX_H_*/
