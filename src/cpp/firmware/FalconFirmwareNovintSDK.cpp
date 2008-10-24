/***
 * @file FalconFirmwareNovintSDK.cpp
 * @brief Firmware communications for the firmware included with the Novint SDK/Official Drivers
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @version $Id$
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL$
 * 
 * Project info at http://libnifalcon.sourceforge.net/ 
 *
 */
#include "FalconFirmwareNovintSDK.h"
#include <iostream>
#include <cstdlib>

namespace libnifalcon
{

	FalconFirmwareNovintSDK::FalconFirmwareNovintSDK() :
		m_hasWritten(false),
		m_currentOutputIndex(0)
	{
	}
	
	void FalconFirmwareNovintSDK::formatOutput()
	{
		for(int i = 0; i < m_rawDataSize; ++i)
		{
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
					m_encoderValues[i] =
						(((*(m_rawOutput+idx) - 0x41) & 0xf)) |
						(((*(m_rawOutput+idx+1) - 0x41) & 0xf) << 4) |
						(((*(m_rawOutput+idx+2) - 0x41) & 0xf) << 8) |
						(((*(m_rawOutput+idx+3) - 0x41) & 0xf) << 12);
					//Shift value down a nibble for homing status
					m_homingStatus |= ((m_rawOutput[13] - 0x41) >> 4) & (1 << i);
					m_gripInfo = (m_rawOutput[13] - 0x41) & 0x0f;
				}
				m_currentOutputIndex = 0;
			}			
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

		if(m_falconComm->requiresPoll())
		{
			m_falconComm->poll();
		}
		
		//Receive information from the falcon
		if(m_hasWritten && m_falconComm->hasBytesAvailable())
		{
			m_rawDataSize = m_falconComm->getBytesAvailable();
			//std::cout << m_rawDataSize << std::endl;
			//hack to make libftdi work for the time being
			if(m_rawDataSize <= 0 && !m_falconComm->requiresPoll()) m_rawDataSize = 16;
			if(m_falconComm->read((uint8_t*)m_rawData, (uint32_t)m_rawDataSize))
			{
				formatOutput();
				m_hasWritten = false;
				if(m_rawDataSize <= 0) read_successful = false;
				else read_successful = true;
			}
			else
			{
				return false;
			}
		}
		else if(m_hasWritten && !m_falconComm->hasBytesAvailable())
		{
			//std::cout << "Waiting!" << std::endl;
			return false;
		}
		//Send information to the falcon
		formatInput();
		if(!m_falconComm->write((uint8_t*)m_rawInput, (uint32_t)16))
		{
			return false;
		}
		m_hasWritten = true;
		return read_successful;
	}
	
}
