/***
 * @file FalconFirmware.cpp
 * @brief FalconFirmware class, base implementation for firmware operations
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include "falcon/core/FalconFirmware.h"
#include <iostream>
#include <fstream>
#include <cstring>

namespace libnifalcon
{
	FalconFirmware::FalconFirmware() :
		m_homingMode(false),
		m_isFirmwareLoaded(false),
		m_hasWritten(false),
		m_loopCount(0),
		m_outputCount(0),
		INIT_LOGGER("FalconFirmware")
		//m_packetBufferSize(1)
	{
		//Who needs loops!
		m_forceValues[0] = 0;
		m_forceValues[1] = 0;
		m_forceValues[2] = 0;
		m_encoderValues[0] = 0;
		m_encoderValues[1] = 0;
		m_encoderValues[2] = 0;
	}

	bool FalconFirmware::setFirmwareFile(const std::string& filename)
    {
		std::fstream test_file(filename.c_str(),  std::fstream::in | std::fstream::binary);
		if(!test_file.is_open())
		{
			m_errorCode = FALCON_FIRMWARE_FILE_NOT_VALID;
			return false;
		}
		m_firmwareFilename = filename;
		return true;
	}

	bool FalconFirmware::loadFirmware(unsigned int retries, bool skip_checksum)
	{
		for(unsigned int i = 0; i < retries; ++i)
		{
			if(loadFirmware(skip_checksum))
			{
				return true;
			}
		}
		return false;
	}

	bool FalconFirmware::loadFirmware(bool skip_checksum)
	{
		if(m_firmwareFilename.length() == 0)
		{
			m_errorCode = FALCON_FIRMWARE_FILE_NOT_VALID;
			return false;
		}
		std::fstream firmware_file(m_firmwareFilename.c_str(), std::fstream::in | std::fstream::binary);
		if(!firmware_file.is_open())
		{
			m_errorCode = FALCON_FIRMWARE_FILE_NOT_VALID;
			return false;
		}
		long begin, end, file_size;
		begin = firmware_file.tellg();
		firmware_file.seekg (0, std::ios::end);
		end = firmware_file.tellg();
		file_size = end-begin;
		firmware_file.seekg (0, std::ios::beg);

		uint8_t* buf = new uint8_t[file_size];
		firmware_file.read((char*)buf, file_size);
		bool success = loadFirmware(skip_checksum, file_size, buf);
		firmware_file.close();
		delete[] buf;
		return success;
	}

	bool FalconFirmware::loadFirmware(bool skip_checksum, const unsigned int& firmware_size, uint8_t* buffer)
	{
		if(m_falconComm == NULL)
		{
			m_errorCode = FALCON_FIRMWARE_NO_COMM_SET;
			return false;
		}
		if(!m_falconComm->isCommOpen())
		{
			m_errorCode = FalconComm::FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}
		if(!m_falconComm->setFirmwareMode())
		{
			m_errorCode = m_falconComm->getErrorCode();
			return false;
		}
		uint8_t send_buf[128], receive_buf[128];
		int bytes_read, total_read = 0;

		//58 is an odd number to use for this, isn't it?
		//Well, full speed USB packets can only take 64 bytes
		//The FTDI tacks 2 bytes onto that for its status
		//If you send 60-62, libusb-1.0 freaks out
		//So, 58 it is. Happy medium

		const int READ_SIZE = 62;

		while(total_read != firmware_size)
		{

			if(total_read + READ_SIZE > firmware_size)
			{
				bytes_read = firmware_size-total_read;
				//bytes_read = READ_SIZE;
			}
			else
			{
				bytes_read = READ_SIZE;
			}
			if(!m_falconComm->writeBlocking(buffer+total_read, bytes_read))
			{
				m_errorCode = m_falconComm->getErrorCode();
				return false;
			}
			int bytes_check = 0;
			int bytes_current = 0;
			while(bytes_check < bytes_read)
			{
				if(!m_falconComm->readBlocking(receive_buf, bytes_read))
				{
					LOG_DEBUG("Firmware read failed, only returned " << m_falconComm->getLastBytesRead() << " bytes");
					m_errorCode = m_falconComm->getErrorCode();

					return false;
				}
				if(!skip_checksum)
				{
					for(unsigned int i = 0; i < m_falconComm->getLastBytesRead(); ++i)
					{
						if((buffer[total_read+i]) != receive_buf[i])
						{
							m_errorCode = FALCON_FIRMWARE_CHECKSUM_MISMATCH;
							return false;
						}
					}
				}
				else
				{
					LOG_DEBUG("Skipping checksum for firmware");
				}
				bytes_check += m_falconComm->getLastBytesRead();
				total_read += m_falconComm->getLastBytesRead();
			}
		}
		m_falconComm->setNormalMode();
		m_hasWritten = false;
		m_isFirmwareLoaded = true;
		return true;
	}

	bool FalconFirmware::isFirmwareLoaded()
	{
		resetFirmwareState();
		m_outputCount = 0;
		for(unsigned int j = 0; j < 10; ++j)
		{
			for(unsigned int i = 0; i < 100; ++i)
			{
				runIOLoop();
				if(m_outputCount > 0)
				{
					m_isFirmwareLoaded = true;
					return true;
				}
			}
			//Sometimes we need to kick out another write to get a proper return
			resetFirmwareState();
		}
		m_isFirmwareLoaded = false;
		m_errorCode = FALCON_FIRMWARE_NO_FIRMWARE_LOADED;
		return false;
	}
}
