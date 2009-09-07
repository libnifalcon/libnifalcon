/***
 * @file FalconDevice.cpp
 * @brief FalconDevice class, glue class for falcon components
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include "falcon/core/FalconDevice.h"
#if defined(LIBNIFALCON_USE_LIBUSB)
#include "falcon/comm/FalconCommLibUSB.h"
#elif defined(LIBNIFALCON_USE_LIBFTD2XX)
#include "falcon/comm/FalconCommFTD2XX.h"
#else
#error "Cannot build FalconDevice class without default comm core"
#endif
#include <iostream>

namespace libnifalcon
{

    FalconDevice::FalconDevice() :
		m_errorCount(0),
		INIT_LOGGER("FalconDevice")
	{
#if defined(LIBNIFALCON_USE_LIBUSB)
		setFalconComm<FalconCommLibUSB>();
#elif defined(LIBNIFALCON_USE_LIBFTD2XX)
		setFalconComm<FalconCommFTD2XX>();
#endif
	}

    FalconDevice::~FalconDevice()
	{
	}

    bool FalconDevice::getDeviceCount(unsigned int& count)
	{
		if(m_falconComm == NULL)
		{
			LOG_ERROR("Cannot get device count - Communications not established");
			m_errorCode = FALCON_DEVICE_NO_COMM_SET;
			return false;
		}
		return m_falconComm->getDeviceCount(count);
	}

    bool FalconDevice::open(unsigned int index)
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
		if(m_falconFirmware != NULL)
		{
			m_falconFirmware->resetFirmwareState();
		}
		return true;
	}

	void FalconDevice::close()
    {
		if(m_falconComm == NULL)
		{
			m_errorCode = FALCON_DEVICE_NO_COMM_SET;
			return;
		}
		m_falconComm->close();
		if(m_falconFirmware != NULL)
		{
			m_falconFirmware->resetFirmwareState();
		}
	}

	bool FalconDevice::setFirmwareFile(const std::string& filename)
    {
		if(m_falconFirmware == NULL)
		{
			m_errorCode = FALCON_DEVICE_NO_FIRMWARE_SET;
			return false;
		}
		return m_falconFirmware->setFirmwareFile(filename);
	}

	bool FalconDevice::loadFirmware(unsigned int retries, bool skip_checksum)
	{
		if(m_falconFirmware == NULL)
		{
			m_errorCode = FALCON_DEVICE_NO_FIRMWARE_SET;
			return false;
		}
		return m_falconFirmware->loadFirmware(retries, skip_checksum);
	}

	bool FalconDevice::loadFirmware(bool skip_checksum)
	{
		if(m_falconFirmware == NULL)
		{
			m_errorCode = FALCON_DEVICE_NO_FIRMWARE_SET;
			return false;
		}
		return m_falconFirmware->loadFirmware(skip_checksum);
	}

	bool FalconDevice::isFirmwareLoaded()
	{
		if(m_falconFirmware == NULL)
		{
			m_errorCode = FALCON_DEVICE_NO_FIRMWARE_SET;
			return false;
		}
		return m_falconFirmware->isFirmwareLoaded();
	}

	bool FalconDevice::runIOLoop(unsigned int exe_flags)
	{
		if(m_falconFirmware == NULL)
		{
			m_errorCode = FALCON_DEVICE_NO_FIRMWARE_SET;
			return false;
		}
		if(m_falconKinematic != NULL && (exe_flags & FALCON_LOOP_KINEMATIC))
		{
			boost::array<int, 3> enc_vec;
			m_falconKinematic->getForces(m_position, m_forceVec, enc_vec);
			m_falconFirmware->setForces(enc_vec);
		}
		if(!m_falconFirmware->runIOLoop() && (exe_flags & FALCON_LOOP_FIRMWARE))
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
			boost::array<int, 3> p = m_falconFirmware->getEncoderValues();
			if(!m_falconKinematic->getPosition(p, m_position))
			{
				++m_errorCount;
				m_errorCode = m_falconKinematic->getErrorCode();
				return false;
			}
		}
		return true;
	}
};
