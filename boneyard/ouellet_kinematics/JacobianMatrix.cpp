/***
 * @file JacobianMatrix.cpp
 * @brief Jacobian implementation for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net) / Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include "falcon/kinematic/stamper/JacobianMatrix.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		gmtl::Vec3f JacobianMatrix::calculate(const Angle& angle, const gmtl::Vec3f& cartesian)
		{
			gmtl::Matrix33f matrixJ = calculateJacobian(angle);

			//gmtl::Matrix<float, 3, 1> velocityMatrix;
			//velocityMatrix[0][0] = velocity[0];
			//velocityMatrix[1][0] = velocity[1];
			//velocityMatrix[2][0] = velocity[2];

			//gmtl::Matrix<float, 3, 1> solution = matrixJ * velocityMatrix;

			return matrixJ * cartesian;//gmtl::Vec3f(solution[0][0], solution[1][0], solution[2][0]);
		}

		gmtl::Vec3f JacobianMatrix::calculateInverse(const Angle& angle, const gmtl::Vec3f& angular)
		{
			gmtl::Matrix33f matrixJ = calculateJacobian(angle);

			//gmtl::Matrix<float, 3, 1> velocityMatrix;
			//velocityMatrix[0][0] = velocity[0];
			//velocityMatrix[1][0] = velocity[1];
			//velocityMatrix[2][0] = velocity[2];

			//gmtl::Matrix<float, 3, 1> solution = matrixJ * velocityMatrix;
			//invert(matrixJ);
			return invert(matrixJ) * angular;//gmtl::Vec3f(solution[0][0], solution[1][0], solution[2][0]);
		}

		gmtl::Matrix33f JacobianMatrix::calculateJacobian(const Angle& angle)
		{
			//This is the two step jacobian laid out in chapter 4 of the Stamper paper

			//JF represents the jacobian with respect to joint velocities
			gmtl::Matrix33f matrixJF;
			calculateJFi(ARM_1, angle, matrixJF);
			calculateJFi(ARM_2, angle, matrixJF);
			calculateJFi(ARM_3, angle, matrixJF);
			//std::cout << "JF:" << std::endl;
			//std::cout << matrixJF << std::endl;

			//JI represents the jacobian with respect to angles of the legs
			gmtl::Matrix33f matrixJI;
			calculateJIi(ARM_1, angle, matrixJI);
			calculateJIi(ARM_2, angle, matrixJI);
			calculateJIi(ARM_3, angle, matrixJI);
			//std::cout << "JI:" << std::endl;
			//std::cout << matrixJI << std::endl;

			//Combine to make the final jacobian
			return invert(matrixJI) * matrixJF;
		}

		void JacobianMatrix::calculateJFi(const arm_id arm, const Angle& angle, gmtl::Matrix33f& matrix)
		{
			//Following equation (4.12) in the stamper paper
			float JFi1 = gmtl::Math::cos(angle.theta2[arm])*gmtl::Math::sin(angle.theta3[arm])*gmtl::Math::cos(phy[arm])-
				gmtl::Math::cos(angle.theta3[arm])*gmtl::Math::sin(phy[arm]);

			float JFi2 = gmtl::Math::cos(angle.theta3[arm])*gmtl::Math::cos(phy[arm])+
				gmtl::Math::cos(angle.theta2[arm])*gmtl::Math::sin(angle.theta3[arm])*gmtl::Math::sin(phy[arm]);

			float JFi3 = gmtl::Math::sin(angle.theta2[arm])*gmtl::Math::sin(angle.theta3[arm]);

			matrix[arm][0] = JFi1;
			matrix[arm][1] = JFi2;
			matrix[arm][2] = JFi3;
		}

		void JacobianMatrix::calculateJIi(const arm_id arm, const Angle& angle, gmtl::Matrix33f& matrix)
		{
			//Following equation (4.12) in the stamper paper
			float JIi = a*gmtl::Math::sin(angle.theta2[arm]-angle.theta1[arm])*gmtl::Math::sin(angle.theta3[arm]);
			matrix[arm][arm] = JIi;
		}
	}
}
