/***
 * @file FalconDevice.cpp
 * @brief FalconDevice class, glue class for falcon components
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
#include "FalconDevice.h"
#include <iostream>
namespace libnifalcon
{

    FalconDevice::FalconDevice() :
		m_isFirmwareLoaded(false),
		m_cleanupObjects(true),
		m_errorCount(0),
		m_falconComm(NULL),
		m_falconKinematic(NULL),
		m_falconGrip(NULL),
		m_falconFirmware(NULL)
	{
	}

    FalconDevice::~FalconDevice()
	{
		if(m_cleanupObjects)
		{
			removeFalconGrip();
			removeFalconKinematic();
			removeFalconFirmware();
			removeFalconComm();
		}
	}

    bool FalconDevice::getDeviceCount(int8_t& count)
	{
		if(m_falconComm == NULL)
		{
			m_errorCode = FALCON_DEVICE_NO_COMM_SET;
			return false;
		}
		return m_falconComm->getDeviceCount(count);		
	}

    bool FalconDevice::open(uint8_t index)
	{
		if(m_falconComm == NULL)
		{
			m_errorCode = FALCON_DEVICE_NO_COMM_SET;
			return false;
		}
		if(!m_falconComm->open(index)) 
		{
			m_errorCode = m_falconComm->getErrorCode();
			return false;
		}
		return m_falconComm->setNormalMode();
	}

	void FalconDevice::close()
    {
		if(m_falconComm == NULL)
		{
			m_errorCode = FALCON_DEVICE_NO_COMM_SET;
			return;
		}
		m_falconComm->close();		
	}
	
	bool FalconDevice::setFirmwareFile(std::string filename)
    {
		std::fstream test_file(filename.c_str(),  std::fstream::in | std::fstream::binary);
		if(!test_file.is_open())
		{
			m_errorCode = FALCON_DEVICE_FIRMWARE_NOT_VALID;
			return false;
		}
		m_firmwareFilename = filename;
		return true;
	}

	bool FalconDevice::loadFirmware(int retries, bool skip_checksum)
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
	
	bool FalconDevice::loadFirmware(bool skip_checksum)
	{
		if(m_falconComm == NULL)
		{
			m_errorCode = FALCON_DEVICE_NO_COMM_SET;
			return false;
		}
		if(!m_falconComm->isCommOpen())
		{
			m_errorCode = FalconComm::FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;			
		}
		if(m_firmwareFilename.length() == 0)
		{
			m_errorCode = FALCON_DEVICE_FIRMWARE_NOT_VALID;
			return false;
		}
		std::fstream firmware_file(m_firmwareFilename.c_str(), std::fstream::in | std::fstream::binary);
		if(!firmware_file.is_open())
		{
			m_errorCode = FALCON_DEVICE_FIRMWARE_NOT_VALID;
			return false;
		}
		if(!m_falconComm->setFirmwareMode())
		{
			m_errorCode = m_falconComm->getErrorCode();
			return false;
		}
		uint8_t send_buf[128], receive_buf[128];
		int bytes_read;
		while(!firmware_file.eof())
		{
			firmware_file.read((char*)send_buf, 128);
			bytes_read = firmware_file.gcount();
			if(!m_falconComm->write(send_buf, bytes_read))
			{
				m_errorCode = m_falconComm->getErrorCode();
				return false;
			}
			if(!m_falconComm->read(receive_buf, bytes_read))
			{
				m_errorCode = m_falconComm->getErrorCode();
				return false;
			}
			if(m_falconComm->getLastBytesWritten() != m_falconComm->getLastBytesRead())
			{
				std::cout << m_falconComm->getLastBytesWritten() << " " << m_falconComm->getLastBytesRead() << std::endl;
				m_errorCode = FALCON_DEVICE_FIRMWARE_CHECKSUM_MISMATCH;
				return false;
			}
			if(!skip_checksum)
			{
				for(int i = 0; i < bytes_read; ++i)
				{
					if(send_buf[i] != receive_buf[i])
					{
						m_errorCode = FALCON_DEVICE_FIRMWARE_CHECKSUM_MISMATCH;
						return false;
					}
				}
			}
		}
		m_falconComm->setNormalMode();
		m_isFirmwareLoaded = true;
		return true;
	}

	bool FalconDevice::isFirmwareLoaded()
	{
		if(m_falconFirmware == NULL)
		{
			m_errorCode = FALCON_DEVICE_NO_FIRMWARE_SET;
			return false;
		}
		if(m_isFirmwareLoaded)
		{
			return true;
		}
		for(int i = 0; i < 10; ++i)
		{
			if(runIOLoop())
			{
				m_isFirmwareLoaded = true;
				return true;
			}
		}
		m_errorCode = FALCON_DEVICE_NO_FIRMWARE_LOADED;
		return false;
	}
	
	bool FalconDevice::runIOLoop(uint8_t exe_flags)
	{
		if(m_falconFirmware == NULL)
		{
			m_errorCode = FALCON_DEVICE_NO_FIRMWARE_SET;
			return false;
		}		
		if(m_falconKinematic != NULL && (exe_flags & FALCON_LOOP_KINEMATIC))
		{
			int16_t enc_vec[3];
			m_falconKinematic->getForces(m_position, m_forceVec, enc_vec);
			m_falconFirmware->setForces(enc_vec);
		}
		if(!m_falconFirmware->runIOLoop() && (exe_flags & FALCON_LOOP_KINEMATIC))
		{
			++m_errorCount;
			m_errorCode = m_falconFirmware->getErrorCode();
			return false;
		}
		if(m_falconGrip != NULL && (exe_flags & FALCON_LOOP_GRIP))
		{
			if(!m_falconGrip->runGripLoop(m_falconFirmware->getGripInfoSize(), m_falconFirmware->getGripInfo()))
			{
				m_errorCode = m_falconGrip->getErrorCode();
				return false;				
			}
		}
		if(m_falconKinematic != NULL && (exe_flags & FALCON_LOOP_KINEMATIC))
		{
			if(!m_falconKinematic->getPosition(m_falconFirmware->getEncoderValues(), m_position))
			{
				++m_errorCount;
				m_errorCode = m_falconKinematic->getErrorCode();
				return false;				
			}
		}
		return true;
	}
};
