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
		m_rawDataSize(0),
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
        uint8_t* data(m_rawData + m_currentOutputIndex);
        if (data[0] == '<' && data[15] == '>')
        {
            memcpy(m_rawOutput, data, 16);
            //Turn motor values into system specific ints
            for(int i(0); i < 3; ++i)
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
            }
            //Shift value down a nibble for homing status
            m_homingStatus = ((m_rawOutput[13] - 0x41) >> 4) & 7;
            m_gripInfo = (m_rawOutput[13] - 0x41) & 0x0f;
            
            ++m_outputCount;
            return true;
        }
        else
        {
            LOG_WARN("Clearing malformed packet!");
            return false;
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

		if(m_falconComm == NULL || !m_falconComm->isCommOpen())
		{
			LOG_ERROR("Cannot run IO on uninitialized/unopened device!");
			return false;
		}

		m_falconComm->poll();

		//Receive information from the falcon
		if(m_hasWritten && m_falconComm->hasBytesAvailable())
		{
			uint32_t bytes( m_falconComm->getBytesAvailable() );
            if(bytes == 0)
            {
                //We somehow just got modem bytes back. Kick out another read.
                m_falconComm->read((uint8_t*)m_rawData + m_rawDataSize, bytes);
                return false;
            }
            else if(m_falconComm->read((uint8_t*)m_rawData + m_rawDataSize, bytes))
            {
                m_rawDataSize += m_falconComm->getLastBytesRead();
                m_currentOutputIndex = 0;
                while (m_currentOutputIndex+16 <= m_rawDataSize)
                {
                    read_successful = formatOutput();
                    m_hasWritten = false;
                    ++m_loopCount;
                    m_currentOutputIndex += 16;
                }
                
                // Preserve the unprocessed tail of the data for the next cycle
                m_rawDataSize -= m_currentOutputIndex;
                if (m_rawDataSize > 0) memcpy(m_rawData, m_rawData + m_currentOutputIndex, m_rawDataSize);
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
