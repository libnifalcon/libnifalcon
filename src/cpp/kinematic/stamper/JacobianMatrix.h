/***
 * @file JacobianMatrix.h
 * @brief Jacobian Matrix for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net)
 * @version $Id$
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL$
 * 
 * Project info at http://libnifalcon.sourceforge.net/ 
 *
 */
#ifndef JACOBIANMATRIX_H_
#define JACOBIANMATRIX_H_

#include "gmtl/gmtl.h"

#include "InverseKinematic.h"
#include "core/FalconGeometry.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
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
