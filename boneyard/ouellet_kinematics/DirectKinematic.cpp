/***
 * @file DirectKinematic.cpp
 * @brief DK implementation for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net) / Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include "falcon/core/FalconGeometry.h"
#include "falcon/kinematic/stamper/DirectKinematic.h"
#include "falcon/kinematic/stamper/InverseKinematic.h"
#include "falcon/kinematic/stamper/JacobianMatrix.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{
		const double DirectKinematic::POSITION_CENTER[3] = {0.0, 0.0, 0.150};
		const float DirectKinematic::POSITION_RANGE = 0.200;
		const unsigned int DirectKinematic::POSITION_MATRIX_DENSITY = 64;

		DirectKinematic::DirectKinematic() :
			m_basePositionMatrix(POSITION_CENTER, POSITION_RANGE, POSITION_MATRIX_DENSITY),
			m_baseAngularMatrix(gmtl::Vec3f(gmtl::Math::deg2Rad(35.0f),gmtl::Math::deg2Rad(35.0f),gmtl::Math::deg2Rad(35.0f)), gmtl::Math::deg2Rad(130.0f), 16)
		{
		}

		void DirectKinematic::initialize()
		{
			m_basePositionMatrix.populate(m_baseAngularMatrix);
			//std::cout << m_baseAngularMatrix.isComplete() << std::endl;
			epsilonAngle = gmtl::Math::deg2Rad(0.05f);
			epsilonPosition = 1;
		}

		gmtl::Point3f DirectKinematic::calculate(gmtl::Vec3f& angle)
		{
			iteration = 0;
			gmtl::Point3f position;
			m_baseAngularMatrix.getPosition(angle, position);
			return newtonRaphsonMethod(angle, position);
		}

		gmtl::Point3f DirectKinematic::newtonRaphsonMethod(gmtl::Vec3f& angle, gmtl::Point3f& approxPosition)
		{
			Angle approxAngle = InverseKinematic::calculate(approxPosition);
			gmtl::Point3f newPosition = approxPosition + JacobianMatrix::calculate(approxAngle, (angle-gmtl::Vec3f(approxAngle.theta1[0], approxAngle.theta1[1], approxAngle.theta1[2])));

			if (iteration++ < 10)
			{
				if ( (fabsf(approxAngle.theta1[0] - angle[0]) > epsilonAngle) ||
					 (fabsf(approxAngle.theta1[1] - angle[1]) > epsilonAngle) ||
					 (fabsf(approxAngle.theta1[2] - angle[2]) > epsilonAngle) )
				{
					approxPosition = newtonRaphsonMethod(angle, newPosition);
				}
			}
			return approxPosition;
		}
	}
}
