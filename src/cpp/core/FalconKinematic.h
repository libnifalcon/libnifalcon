#ifndef FALCONKINEMATIC_H
#define FALCONKINEMATIC_H

namespace libnifalcon
{
	class FalconKinematic
	{
	public:
		FalconKinematic();
		virtual ~FalconKinematic();
		virtual bool getAngles(double position[3], double angles[3]) = 0;
		virtual bool getPosition(double angles[3], double position[3]) = 0;
		virtual bool initialize() {} ;	
	};
}

#endif
