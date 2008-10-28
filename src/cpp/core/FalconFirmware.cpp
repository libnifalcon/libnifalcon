#include "FalconFirmware.h"
#include <iostream>
#include <fstream>

namespace libnifalcon
{
	FalconFirmware::FalconFirmware() :
		m_falconComm(NULL),
		m_homingMode(false),
		m_isFirmwareLoaded(false),
		m_hasWritten(false)
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
	
	bool FalconFirmware::setFirmwareFile(std::string filename)
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

	bool FalconFirmware::loadFirmware(int retries, bool skip_checksum)
	{
		for(int i = 0; i < retries; ++i)
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
		if(!m_falconComm->setFirmwareMode())
		{
			std::cout << "Can't set firmware mode! " << m_errorCode << " " << m_falconComm->getDeviceErrorCode() << std::endl;
			m_errorCode = m_falconComm->getErrorCode();
			return false;
		}
		uint8_t send_buf[128], receive_buf[128];
		int bytes_read;
		while(!firmware_file.eof())
		{
			//58 is an odd number to use for this, isn't it?
			//Well, full speed can only take 64 bytes
			//The FTDI tacks 2 bytes onto that for its status
			//If you send 60-62, libusb-1.0 freaks out
			//So, 58 it is. Happy medium
			firmware_file.read((char*)send_buf, 58);
			bytes_read = firmware_file.gcount();
			if(!m_falconComm->write(send_buf, bytes_read))
			{
				m_errorCode = m_falconComm->getErrorCode();
				return false;
			}
			if(m_falconComm->requiresPoll())
			{
				while(!m_falconComm->hasBytesAvailable())
				{
					m_falconComm->poll();
				}
			}
			if(!m_falconComm->read(receive_buf, bytes_read))
			{
				m_errorCode = m_falconComm->getErrorCode();
				return false;
			}
			if(m_falconComm->getLastBytesWritten() != m_falconComm->getLastBytesRead())
			{
				m_errorCode = FALCON_FIRMWARE_CHECKSUM_MISMATCH;
				return false;
			}
			if(!skip_checksum)
			{
				for(int i = 0; i < bytes_read; ++i)
				{
					if(send_buf[i] != receive_buf[i])
					{
						m_errorCode = FALCON_FIRMWARE_CHECKSUM_MISMATCH;
						return false;
					}
				}
			}
		}
		m_falconComm->setNormalMode();
		m_hasWritten = false;
		m_isFirmwareLoaded = true;
		return true;
	}

	bool FalconFirmware::isFirmwareLoaded()
	{
		if(m_falconComm->requiresPoll())
		{
			for(int i = 0; i < 250; ++i)
			{
				if(runIOLoop())
				{
					m_isFirmwareLoaded = true;
					return true;
				}
			}
		}
		else
		{
			for(int i = 0; i < 10; ++i)
			{
				if(runIOLoop())
				{
					m_isFirmwareLoaded = true;
					return true;
				}
			}
		}
		m_isFirmwareLoaded = false;
		m_errorCode = FALCON_FIRMWARE_NO_FIRMWARE_LOADED;
		return false;
	}
}
