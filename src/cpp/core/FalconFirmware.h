#ifndef FALCONFIRMWARE_H
#define FALCONFIRMWARE_H

#include <stdint.h>
#include <cstdlib>
#include "FalconComm.h"

namespace libnifalcon
{
	class FalconFirmware : public FalconCore
	{	
	public:
		enum
		{
			GREEN_LED=0x2,
			BLUE_LED=0x4,
			RED_LED=0x8
		};
	
		FalconFirmware() :
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

		virtual ~FalconFirmware() {}
		virtual bool runIOLoop() = 0;
		virtual int32_t getGripInfoSize() = 0;
		virtual u_int8_t* getGripInfo() = 0;
		
		int16_t* getEncoderValues() { return m_encoderValues; }
	
		void setLEDStatus(u_int8_t leds) { m_ledStatus = leds; }
		u_int8_t getLEDStatus() { return m_ledStatus; }
		void setHomingMode(bool value) { m_homingMode = value; }
		bool* getHomingModeStatus() { return m_homingStatus; }	
		bool isHomed() { return ( m_homingStatus[0] && m_homingStatus[1] && m_homingStatus[2]); }
	
		void setFalconComm(FalconComm* f) { m_falconComm = f; }
	protected:
		FalconComm* m_falconComm;

		//Values sent to falcon
		bool m_homingMode;
		u_int8_t m_ledStatus;
		int16_t m_forceValues[3];	

		//Values received from falcon
		int16_t m_encoderValues[3];
		bool m_homingStatus[3];
	};
}
#endif
