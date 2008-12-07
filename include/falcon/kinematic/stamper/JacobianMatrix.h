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

#include "falcon/gmtl/gmtl.h"

#include "falcon/kinematic/stamper/InverseKinematic.h"
#include "falcon/core/FalconGeometry.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		class JacobianMatrix
		{
		public:
			JacobianMatrix() {}
			virtual ~JacobianMatrix() {}   
			static gmtl::Vec3f calculate(const Angle& angle, const gmtl::Vec3f& cartesian);	
			static gmtl::Vec3f calculateInverse(const Angle& angle, const gmtl::Vec3f& angular);	
		private:
			static gmtl::Matrix33f calculateJacobian(const Angle& angle);	
			static void calculateJFi(const arm_id arm, const Angle& angle, gmtl::Matrix33f& matrix);	
			static void calculateJIi(const arm_id arm, const Angle& angle, gmtl::Matrix33f& matrix);
		};

	}
}
#endif /*JACOBIANMATRIX_H_*/
