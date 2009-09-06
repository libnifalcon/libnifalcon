/***
 * @file FalconFirmwareNovintSDK.cpp
 * @brief Firmware communications for the firmware included with the Novint SDK/Official Drivers
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include "falcon/firmware/FalconFirmwareNovintSDK.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

namespace libnifalcon
{

	FalconFirmwareNovintSDK::FalconFirmwareNovintSDK() :
		m_currentOutputIndex(0),
		INIT_LOGGER("FalconFirmwareNovintSDK")
	{
		//Make sure we're pretty much always safe to print these
		memset(m_rawInput, 0, 17);
		memset(m_rawOutput, 0, 17);
		memset(m_rawOutputInternal, 0, 17);
	}

	FalconFirmwareNovintSDK::~FalconFirmwareNovintSDK()
	{
	}

	std::string FalconFirmwareNovintSDK::getRawReturn()
	{
		return (char*)m_rawOutput;
	}
	
	bool FalconFirmwareNovintSDK::formatOutput()
	{
		bool ret_val = false;
		//m_rawData[m_rawDataSize] = 0;
		for(int i = 0; i < m_rawDataSize; ++i)
		{
			ret_val = false;
			//Skip up to the next valid packet if need be
			if(m_currentOutputIndex == 0)
			{
				if(m_rawData[i] != '<') continue;
			}
			m_rawOutputInternal[m_currentOutputIndex] = m_rawData[i];
			++m_currentOutputIndex;
			if(m_currentOutputIndex == 16 && m_rawOutputInternal[m_currentOutputIndex - 1] == '>')
			{
				memcpy(m_rawOutput, m_rawOutputInternal, 16);
				//Turn motor values into system specific ints
				int i;
				m_homingStatus = 0;
				for(i = 0; i < 3; ++i)
				{
					int idx = 1 + (i*4);
					//We're getting a signed short int off the wire
					int16_t val =
						(((*(m_rawOutput+idx) - 0x41) & 0xf)) |
						(((*(m_rawOutput+idx+1) - 0x41) & 0xf) << 4) |
						(((*(m_rawOutput+idx+2) - 0x41) & 0xf) << 8) |
						(((*(m_rawOutput+idx+3) - 0x41) & 0xf) << 12);
					//Now convert into full system int since the compiler will
					//do the sign move for us
					m_encoderValues[i] = val;
					//Shift value down a nibble for homing status
					m_homingStatus |= ((m_rawOutput[13] - 0x41) >> 4) & (1 << i);
					m_gripInfo = (m_rawOutput[13] - 0x41) & 0x0f;
				}
				m_currentOutputIndex = 0;
				ret_val = true;
				++m_outputCount;
			}
			else if(m_currentOutputIndex == 16)
			{
				LOG_WARN("Clearing malformed packet!");
				m_currentOutputIndex = 0;
			}
		}
		return ret_val;
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

		if(m_falconComm == NULL || !m_falconComm->isCommOpen())
		{
			LOG_ERROR("Cannot run IO on uninitialized/unopened device!");
			return false;
		}

		m_falconComm->poll();

		//Receive information from the falcon
		if(m_hasWritten && m_falconComm->hasBytesAvailable())
		{
			m_rawDataSize = m_falconComm->getBytesAvailable();

			//We somehow just got modem bytes back. Kick out another read.
			if(m_rawDataSize == 0)
			{
				m_falconComm->read((uint8_t*)m_rawData, (uint32_t)m_rawDataSize);
				return false;
			}
			if(m_falconComm->read((uint8_t*)m_rawData, m_rawDataSize))
			{
				formatOutput();
				m_hasWritten = false;
				if(m_rawDataSize <= 0) read_successful = false;
				else read_successful = true;
				++m_loopCount;
			}
			else
			{
				LOG_DEBUG("Couldn't read! " << m_falconComm->getErrorCode());
				return false;
			}
		}
		else if(m_hasWritten && !m_falconComm->hasBytesAvailable())
		{
			return false;
		}
		//Send information to the falcon
		formatInput();
		if(!m_falconComm->write((uint8_t*)m_rawInput, 16))
		{
			return false;
		}
		m_hasWritten = true;
		return read_successful;
	}

}
