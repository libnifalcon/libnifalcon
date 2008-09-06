#include "FalconDevice.h"

namespace libnifalcon
{

    FalconDevice::FalconDevice() :
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
		return m_falconComm->open(index);
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
			return false;
		}
		if(!m_falconComm->isCommOpen())
		{
			return false;			
		}
		if(m_firmwareFilename.length() == 0)
		{
			return false;
		}
		std::fstream firmware_file(m_firmwareFilename.c_str(), std::fstream::in | std::fstream::binary);

		m_falconComm->setFirmwareMode();

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
		return true;
	}

	bool FalconDevice::runIOLoop()
	{
		if(m_falconFirmware == NULL) return false;
		return m_falconFirmware->runIOLoop();
	}

	void FalconDevice::setFalconComm(FalconComm* f)
	{
		m_falconComm = f;
		if(m_falconFirmware != NULL)
		{
			m_falconFirmware->setFalconComm(m_falconComm);
		}		
	}
	void FalconDevice::setFalconFirmware(FalconFirmware* f)
	{
		m_falconFirmware = f;
		if(m_falconComm != NULL)
		{
			m_falconFirmware->setFalconComm(m_falconComm);
		}
	}
	void FalconDevice::setFalconGrip(FalconGrip* f)
	{
		m_falconGrip = f;
	}		
	void FalconDevice::setFalconKinematic(FalconKinematic* f)
	{
		m_falconKinematic = f;
	}

};
