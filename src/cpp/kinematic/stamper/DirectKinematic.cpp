//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net
#include "DirectKinematic.h"

#include "InverseKinematic.h"
#include "JacobianMatrix.h"

namespace libnifalcon
{
	namespace stamper
	{
		void DirectKinematic::initialize()
		{
			gmtl::Vec3f positionCenter(0,0,150); // center of workspace
			float positionRange = 200; // range of the workspace
			basePositionMatrix = new PositionMatrix(positionCenter, positionRange, 64);
	
			gmtl::Vec3f angularCenter(gmtl::Math::deg2Rad(35.0f),gmtl::Math::deg2Rad(35.0f),gmtl::Math::deg2Rad(35.0f)); // minimum -45 degrees, maximum 135 degrees
			float angularRange = gmtl::Math::deg2Rad(130.0f); // range of 130 degrees
			baseAngularMatrix = new AngularMatrix(angularCenter, angularRange, 16);
		
			basePositionMatrix->populate(baseAngularMatrix);
			std::cout << baseAngularMatrix->isComplete() << std::endl;
	
			epsilonAngle = gmtl::Math::deg2Rad(0.05f);
			epsilonPosition = 1;
		}
		
		gmtl::Point3f DirectKinematic::calculate(gmtl::Vec3f angle)
		{
			iteration = 0;
			gmtl::Point3f position;
			baseAngularMatrix->getPosition(angle, position);
			return newtonRaphsonMethod(angle, position);
		}

		gmtl::Point3f DirectKinematic::newtonRaphsonMethod(gmtl::Vec3f angle, gmtl::Point3f approxPosition)
		{
			Angle approxAngle = InverseKinematic::calculate(approxPosition);	
			gmtl::Vec3f newPosition = approxPosition + JacobianMatrix::calculate(approxAngle, (angle-gmtl::Vec3f(approxAngle.theta1[0], approxAngle.theta1[1], approxAngle.theta1[2])));

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
