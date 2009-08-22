/***
 * @file FalconDeviceDefault.cpp
 * @brief Utility class for wrapping template calls for device behaviors
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.sourceforge.net/
 *
 */

#include <iostream>

#include "falcon/util/FalconDeviceDefault.h"

#if defined(LIBUSB)
#include "falcon/comm/FalconCommLibUSB.h"
#elif defined(LIBFTDI)
#include "falcon/comm/FalconCommLibFTDI.h"
#elif defined(LIBFTD2XX)
#include "falcon/comm/FalconCommFTD2XX.h"
#endif

#include "falcon/firmware/FalconFirmwareNovintSDK.h"
#include "falcon/kinematic/FalconKinematicStamper.h"
#include "falcon/grip/FalconGripFourButton.h"
#include "falcon/util/FalconFirmwareBinaryNvent.h"

namespace libnifalcon
{
	FalconDeviceDefault::FalconDeviceDefault()
	{
		//Fall back through the default device chain
		std::cout << "Running Constructor!" << std::endl;
#if defined(LIBUSB)
		std::cout << "Creating LIBUSB comms!" << std::endl;
		setFalconComm<FalconCommLibUSB>();
#elif defined(LIBFTDI)
		setFalconComm<FalconCommLibFTDI>();
#elif defined(LIBFTD2XX)
		setFalconComm<FalconCommFTD2XX>();
#else
#endif
		setFalconFirmware<FalconFirmwareNovintSDK>();
		setFalconKinematic<FalconKinematicStamper>();
		setFalconGrip<FalconGripFourButton>();
	}

	FalconDeviceDefault::~FalconDeviceDefault()
	{
	}

	int FalconDeviceDefault::getCount()
	{
		unsigned int c;
		if(!getDeviceCount(c)) return -1;
		return c;
	}

	bool FalconDeviceDefault::open(int device_count)
	{
		return FalconDevice::open(device_count);
	}

	bool FalconDeviceDefault::loadFirmware()
	{
		if(!isFirmwareLoaded())
		{
			std::cout << "Loading firmware" << std::endl;
			for(int i = 0; i < 10; ++i)
			{
				if(!getFalconFirmware()->loadFirmware(true, NOVINT_FALCON_NVENT_FIRMWARE_SIZE, const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE)))
				{
					std::cout << "Could not load firmware" << std::endl;
					return false;
				}
				else
				{
					std::cout <<"Firmware loaded" << std::endl;
					break;
				}
			}
			if(!isFirmwareLoaded())
			{
				std::cout << "Firmware didn't load correctly. Try running findfalcons again" << std::endl;
				return false;
			}
		}
		return true;
	}
	
	void FalconDeviceDefault::close()
	{
		FalconDevice::close();
	}
	
	bool FalconDeviceDefault::runIOLoop(unsigned int exe_flags)
	{
		return FalconDevice::runIOLoop(exe_flags);
	}
	
	std::string FalconDeviceDefault::getRawOutput()
	{
		return FalconDevice::getFalconFirmware()->getRawReturn();
	}
	
	void FalconDeviceDefault::setRawInput(char* str)
	{
	}

	int FalconDeviceDefault::getErrorCode()
	{
		return FalconCore::getErrorCode();
	}

	void FalconDeviceDefault::setForces(FalconVec3d v)
	{
		boost::array<double, 3> j;
		j[0] = v.x;
		j[1] = v.y;
		j[2] = v.z;
		FalconDevice::setForce(j);
	}

	FalconDeviceDefault::FalconVec3d FalconDeviceDefault::getPosition()
	{
		boost::array<double, 3> j = FalconDevice::getPosition();
		FalconVec3d v;
		v.x = j[0];
		v.y = j[1];
		v.z = j[2];
		return v;
	}

	void FalconDeviceDefault::setLEDStatus(int v)
	{
		return FalconDevice::getFalconFirmware()->setLEDStatus(v);		
	}
}
