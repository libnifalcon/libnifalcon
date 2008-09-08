#ifndef FALCONKINEMATIC_H
#define FALCONKINEMATIC_H

#include <sys/types.h>
#include "kinematic/FalconGeometry.h"

namespace libnifalcon
{
	class FalconKinematic
	{
	public:
		FalconKinematic() {}
		virtual ~FalconKinematic() {}
		float getTheta(int16_t encoder_value)
		{
			return (((SHAFT_DIAMETER*PI) / (WHEEL_SLOTS_NUMBER*4)) * (encoder_value))/((PI*SMALL_ARM_DIAMETER)/360.0f) + THETA_OFFSET_ANGLE;
		}
		virtual bool getAngles(double position[3], double angles[3]) = 0;
		virtual bool getPosition(int16_t encoders[3], double position[3]) = 0;
		
	};
}

#endif
