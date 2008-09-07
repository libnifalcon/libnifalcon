//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net

#include "FalconStamperKinematic.h"

namespace libnifalcon
{
	namespace stamper
	{
		FalconStamperKinematic::FalconStamperKinematic(bool init_now)
		{
			if(init_now)
			{
				initialize();
			}
		}

		void FalconStamperKinematic::initialize()
		{
			m_dir.initialize();		
		}

		float FalconStamperKinematic::getTheta(int16_t encoder_value)
		{
			return (((SHAFT_DIAMETER*gmtl::Math::PI) / (WHEEL_SLOTS_NUMBER*4)) * (encoder_value))/((gmtl::Math::PI*SMALL_ARM_DIAMETER)/360.0f) + THETA_OFFSET_ANGLE;
		}

										  
		bool FalconStamperKinematic::getAngles(double position[3], double* angles)
		{
			Angle a;
			gmtl::Point3f p(position[0], position[1], position[2]);
			a = m_inv.calculate(p);
			
			angles[0] = a.theta1[0];
			angles[1] = a.theta1[1];
			angles[2] = a.theta1[2];			
			return true;
		}

		bool FalconStamperKinematic::getPosition(int16_t encoders[3], double* position)
		{
			float angle1 = getTheta(encoders[0]);
			float angle2 = getTheta(encoders[1]);
			float angle3 = getTheta(encoders[2]);
			gmtl::Point3f p = m_dir.calculate(gmtl::Vec3f(gmtl::Math::deg2Rad(angle1), gmtl::Math::deg2Rad(angle2), gmtl::Math::deg2Rad(angle3)));
			position[0] = p[0];
			position[1] = p[1];
			position[2] = p[2];
			return true;
		}
	}
}
