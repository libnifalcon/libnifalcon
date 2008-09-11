#include "FalconDevice.h"

#include <iostream>

namespace libnifalcon
{

    FalconDevice::FalconDevice() :
		m_isFirmwareLoaded(false),
		m_falconComm(NULL),
		m_falconKinematic(NULL),
		m_falconGrip(NULL),
		m_falconFirmware(NULL)
	{
	}

    FalconDevice::~FalconDevice()
	{
		if(m_falconComm != NULL)
		{
			close();
		}
	}

    bool FalconDevice::getDeviceCount(int8_t& count)
	{
		if(m_falconComm == NULL)
		{
			return false;
		}
		return m_falconComm->getDeviceCount(count);		
	}

    bool FalconDevice::open(u_int8_t index)
	{
		if(m_falconComm == NULL)
		{
			return false;
		}
		if(!m_falconComm->open(index)) 
		{
			return false;
		}
		return m_falconComm->setNormalMode();
	}

	void FalconDevice::close()
    {
		if(m_falconComm == NULL)
		{
			return;
		}
		m_falconComm->close();		
	}
	
	bool FalconDevice::setFirmwareFile(std::string filename)
    {
		std::fstream test_file(filename.c_str(),  std::fstream::in | std::fstream::binary);
		if(!test_file.is_open())
		{
			return false;
		}
		m_firmwareFilename = filename;
		return true;
	}

	bool FalconDevice::loadFirmware()
	{
		if(m_falconComm == NULL)
		{
			std::cout << "No comm!" << std::endl;
			return false;
		}
		if(!m_falconComm->isCommOpen())
		{
			std::cout << "Not open!" << std::endl;
			return false;			
		}
		if(m_firmwareFilename.length() == 0)
		{
			std::cout << "No firmware!" << std::endl;
			return false;
		}
		std::cout << "Loading firmware" << std::endl;
		std::fstream firmware_file(m_firmwareFilename.c_str(), std::fstream::in | std::fstream::binary);
		if(!firmware_file.is_open())
		{
			std::cout << "Cannot open file!" << std::endl;
			return false;
		}
		
		if(!m_falconComm->setFirmwareMode())
		{
			std::cout << "Cannot set firmware mode!" << std::endl;
			return false;			
		}

		u_int8_t send_buf[128], receive_buf[128];
		u_int32_t bytes_read, bytes_written;		
		while(!firmware_file.eof())
		{
			firmware_file.read((char*)send_buf, 128);
			if((m_errorCode = m_falconComm->write(send_buf, firmware_file.gcount(), bytes_written)) < 0) return false;
			m_errorCode = m_falconComm->read(receive_buf, firmware_file.gcount(), bytes_read);
			if(bytes_read != bytes_written)
			{
				return false;
			}
			for(int i = 0; i < bytes_read; ++i)
			{
				if(send_buf[i] != receive_buf[i])
				{
					return false;
				}
			}
		}
		m_falconComm->setNormalMode();
		m_isFirmwareLoaded = true;
		return true;
	}

	bool FalconDevice::isFirmwareLoaded()
	{
		if(m_falconFirmware == NULL) return false;
		if(m_isFirmwareLoaded) return true;
		for(int i = 0; i < 10; ++i)
		{
			if(runIOLoop())
			{
				m_isFirmwareLoaded = true;
				return true;
			}
		}
		return false;
	}
	
	bool FalconDevice::runIOLoop()
	{
		if(m_falconFirmware == NULL) return false;
		
		if(!m_falconFirmware->runIOLoop())
		{
			return false;
		}
		if(m_falconGrip != NULL)
		{
			m_falconGrip->runGripLoop(m_falconFirmware->getGripInfoSize(), m_falconFirmware->getGripInfo());
		}
		if(m_falconKinematic != NULL)
		{
			m_falconKinematic->getPosition(m_falconFirmware->getEncoderValues(), m_position);
		}
		return true;
	}

};
