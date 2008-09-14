#include "FalconFirmwareNovintSDK.h"
#include <cstdlib>

namespace libnifalcon
{

	FalconFirmwareNovintSDK::FalconFirmwareNovintSDK() :
		m_hasWritten(false)
	{
	}
	
	void FalconFirmwareNovintSDK::formatOutput()
	{
		//Turn motor values into system specific ints
		int i;
		for(i = 0; i < 3; ++i)
		{
			int idx = 1 + (i*4);
			m_encoderValues[i] =
				(((*(m_rawOutput+idx) - 0x41) & 0xf)) |
				(((*(m_rawOutput+idx+1) - 0x41) & 0xf) << 4) |
				(((*(m_rawOutput+idx+2) - 0x41) & 0xf) << 8) |
				(((*(m_rawOutput+idx+3) - 0x41) & 0xf) << 12);
			//Shift value down a nibble for homing status
			m_homingStatus[i] = ((m_rawOutput[13] - 0x41) >> 4) & (1 << i);
		}
	}

	void FalconFirmwareNovintSDK::formatInput()
	{
		//Turn system-specific ints into motor values
		int i;
		for(i = 1; i < 15; ++i)
		{
			m_rawInput[i] = 0x0;
		}
		m_rawInput[0] = '<';
		m_rawInput[15] = '>';
		for(i = 0; i < 3; ++i)
		{
			int idx = 1 + (i*4);
			*(m_rawInput+idx) =   ((m_forceValues[i]) & 0x000f) ;
			*(m_rawInput+idx+1) = ((m_forceValues[i]) & 0x00f0) >> 4;
			*(m_rawInput+idx+2) = ((m_forceValues[i]) & 0x0f00) >> 8;
			*(m_rawInput+idx+3) = ((m_forceValues[i]) & 0xf000) >> 12;
		}
		m_rawInput[13] = m_ledStatus;
		if(m_homingMode) m_rawInput[13] |= 0x01;
		for(i = 1; i < 15; ++i)
		{
			m_rawInput[i] += 0x41;
		}
	}

	bool FalconFirmwareNovintSDK::runIOLoop()
	{
		bool read_successful = false;
		if(m_falconComm == NULL)
		{
			return false;
		}

		//Receive information from the falcon
		if(m_hasWritten)
		{
			if(m_falconComm->read((u_int8_t*)m_rawOutput, (u_int32_t)16))
			{
				formatOutput();
				m_hasWritten = false;
				read_successful = true;
			}
		}
		//Send information to the falcon
		formatInput();
		if(!m_falconComm->write((u_int8_t*)m_rawInput, (u_int32_t)16))
		{
			return false;
		}
		m_hasWritten = true;
		return read_successful;
	}
	
}
