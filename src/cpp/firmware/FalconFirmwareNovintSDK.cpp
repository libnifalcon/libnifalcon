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
		m_hasWritten(false)
	{
	}
	
	void FalconFirmwareNovintSDK::formatOutput()
	{
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
			//The packet buffer no one uses. D:
			//Right now, to get a packet buffer working, I need to know how many bytes
			//are waiting for me from the falcon. FTD2XX implements this by running
			//a thread under the covers to buffer to the host side, then letting anything
			//accessing the driver read from that. libftdi has no such facility, meaning that
			//to figure out whether or not there's anything left to get, you have to do a bulk
			//read, at the cost of 1ms. This upsets the falcon badly. In a
			//perfect world, I'd just use libusb 1.0 and its non-blocking functionality,
			//which means I could both maintain a buffer and not have to worry about
			//thread off libftdi. But, for right now, this all will just stay commented out.
			//I/O works "well enough" as is, you only lose 5% or so of packets.
			/*
			uint8_t o[16];
			while(1)
			{
				m_falconComm->read(o, 16);
				if(m_falconComm->getLastBytesRead() == 0) break;
				if(m_packetBuffer.size() < m_packetBufferSize)
				{
					m_packetBuffer.push_back(o);
				}
				else
				{
					break;
				}
				if(m_packetBuffer.size() == m_packetBufferSize) break;
			}
			if(m_packetBuffer.size() > 0)
			{				
				memcpy((void*)m_rawOutput, (void*)m_packetBuffer.front(), 16);
				m_packetBuffer.pop_front();
				formatOutput();
				m_hasWritten = false;
				read_successful = true;
			}
			*/
			memset(m_rawOutput, 0, 16);
			if(m_falconComm->read((uint8_t*)m_rawOutput, (uint32_t)16))
			{
				formatOutput();
				m_hasWritten = false;
				read_successful = true;
			}
			else
			{
				return false;
			}
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
