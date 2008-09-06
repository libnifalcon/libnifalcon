#include "FalconFirmware.h"

namespace libnifalcon
{
	FalconFirmware::FalconFirmware() :
		m_falconComm(NULL),
		m_homingMode(false)
	{
		//Who needs loops!
		m_forceValues[0] = 0;
		m_forceValues[1] = 0;
		m_forceValues[2] = 0;
		m_encoderValues[0] = 0;
		m_encoderValues[1] = 0;
		m_encoderValues[2] = 0;			
	}
}
