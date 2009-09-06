/***
 * @file JacobianMatrix.h
 * @brief Jacobian Matrix for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net)/Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
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
			/**
			 * Constructor
			 *
			 */
			JacobianMatrix() {}
			/**
			 * Destructor
			 *
			 */
			virtual ~JacobianMatrix() {}

			/**
			 * Calculates the joint weights of each arm (jacobian), and uses it to scale the cartesian vector appropriately
			 *
			 * @param angle Current angles of the arms
			 * @param cartesian Cartesian force vector to apply to end effector
			 *
			 * @return Force values to apply to each leg
			 */
			static gmtl::Vec3f calculate(const Angle& angle, const gmtl::Vec3f& cartesian);

			/**
			 * Calculates the inverse of the joint weights of each arm (jacobian), and uses it to scale the cartesian vector appropriately
			 *
			 * @param angle Current angles of the arms
			 * @param cartesian Cartesian force vector to apply to end effector
			 *
			 * @return Inverse values
			 */
			static gmtl::Vec3f calculateInverse(const Angle& angle, const gmtl::Vec3f& angular);
		private:
			/**
			 * Performs the two step jacobian calculation outlined in chapter 4 of the RL Stamper paper. (See page 38)
			 * We know that JIi*(Vector of arm angles) = JFi*(Vector of arm velocities), so we can find JIi and JFi
			 * in relation to the arm angles, and can use those to find the vector of arm velocities.
			 *
			 * @param angle Current arm angles
			 *
			 * @return Jacobian matrix correspoding to the arm angles
			 */
			static gmtl::Matrix33f calculateJacobian(const Angle& angle);

			/**
			 * Performs the JFi (Jacobian in relation to velocity) calculation
			 *
			 * @param arm Arm to perform calculation on
			 * @param angle Current angle of the arm
			 * @param matrix Matrix to add row to
			 */
			static void calculateJFi(const arm_id arm, const Angle& angle, gmtl::Matrix33f& matrix);

			/**
			 * Performs the JIi (Jacobian in relation to arm angles) calculation
			 *
			 * @param arm Arm to perform calculation on
			 * @param angle Current angle of the arm
			 * @param matrix Matrix to add row to
			 */
			static void calculateJIi(const arm_id arm, const Angle& angle, gmtl::Matrix33f& matrix);
		};

	}
}
#endif /*JACOBIANMATRIX_H_*/
