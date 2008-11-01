//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net
#include "falcon/kinematic/stamper/JacobianMatrix.h"

using namespace gmtl;
using namespace gmtl::Math;

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		gmtl::Vec3f JacobianMatrix::calculate(Angle angle, gmtl::Vec3f cartesian)
		{
			gmtl::Matrix33f matrixJ = calculateJacobian(angle);
	
			//gmtl::Matrix<float, 3, 1> velocityMatrix;
			//velocityMatrix[0][0] = velocity[0];
			//velocityMatrix[1][0] = velocity[1];
			//velocityMatrix[2][0] = velocity[2];

			//gmtl::Matrix<float, 3, 1> solution = matrixJ * velocityMatrix;
	
			return matrixJ * cartesian;//gmtl::Vec3f(solution[0][0], solution[1][0], solution[2][0]);
		}

		gmtl::Vec3f JacobianMatrix::calculateInverse(Angle angle, gmtl::Vec3f angular)
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

		gmtl::Matrix33f JacobianMatrix::calculateJacobian(Angle angle)
		{
			gmtl::Matrix33f matrixJF;
			calculateJFi(ARM_1, angle, &matrixJF);
			calculateJFi(ARM_2, angle, &matrixJF);
			calculateJFi(ARM_3, angle, &matrixJF);
	
			gmtl::Matrix33f matrixJI;
			calculateJIi(ARM_1, angle, &matrixJI);
			calculateJIi(ARM_2, angle, &matrixJI);
			calculateJIi(ARM_3, angle, &matrixJI);
	
			gmtl::Matrix33f matrixJ = invert(matrixJI) * matrixJF;
	
			return matrixJ;
		}

		void JacobianMatrix::calculateJFi(arm_id arm, Angle angle, gmtl::Matrix33f *matrix)
		{
			float JFi1 = cos(angle.theta2[arm])*sin(angle.theta3[arm])*cos(phy[arm])-
				cos(angle.theta3[arm])*sin(phy[arm]);
	
			float JFi2 = cos(angle.theta3[arm])*cos(phy[arm])+
				cos(angle.theta2[arm])*sin(angle.theta3[arm])*sin(phy[arm]);
	
			float JFi3 = sin(angle.theta2[arm])*sin(angle.theta3[arm]);
	
			(*matrix)[arm][0] = JFi1;
			(*matrix)[arm][1] = JFi2;
			(*matrix)[arm][2] = JFi3;
		}

		void JacobianMatrix::calculateJIi(arm_id arm, Angle angle, gmtl::Matrix33f *matrix)
		{
			float JIi = a*sin(angle.theta2[arm]-angle.theta1[arm])*sin(angle.theta3[arm]);
	
			(*matrix)[arm][arm] = JIi;
		}
	}
}
