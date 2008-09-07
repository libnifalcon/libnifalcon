#ifndef FALCONKINEMATIC_H
#define FALCONKINEMATIC_H

#include <sys/types.h>

namespace libnifalcon
{
	class FalconKinematic
	{
	public:
		FalconKinematic() {}
		virtual ~FalconKinematic() {}
		virtual bool getAngles(double position[3], double angles[3]) = 0;
		virtual bool getPosition(int16_t encoders[3], double position[3]) = 0;
	};
}

#endif
