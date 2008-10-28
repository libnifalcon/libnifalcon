/***
 * @file findfalcons.cpp
 * @brief Minimal open-and-run test for the C++ version of libnifalcon
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

#include "core/FalconDevice.h"
#if defined(LIBUSB)
#include "comm/FalconCommLibUSB.h"
#elif defined(LIBFTDI)
#include "comm/FalconCommLibFTDI.h"
#elif defined(LIBFTD2XX)
#include "comm/FalconCommFTD2XX.h"
#endif
#include "firmware/FalconFirmwareNovintSDK.h"
//#include "kinematic/FalconKinematicStamper.h"
#include <sys/time.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <csignal>

using namespace libnifalcon;

#define PACKET_TIMEOUT 1000

FalconDevice dev;
	
void sigproc(int i)
{
	std::cout << "closing falcon and quitting" << std::endl;
	exit(0);
}

void runFalconTest(FalconDevice& d)
{
	FalconFirmware* f;
	FalconKinematic* k;
	double position[3];
	int8_t num_falcons;
	int status, i;
	u_int8_t count;
	u_int32_t error_count = 0;
	u_int32_t loop_count = 0;

	dev.setFalconFirmware<FalconFirmwareNovintSDK>();
//	dev.setFalconKinematic<FalconKinematicStamper>();

	std::cout << "running" << std::endl;
	f = dev.getFalconFirmware();
	if(!dev.getDeviceCount(num_falcons))
	{
		std::cout << "Cannot get device_libftdi count" << std::endl;
	}

	count = 0;

	std::cout << "Falcons found: " << num_falcons << std::endl;

	std::cout << "Opening falcon" << std::endl;
	
	if(!dev.open(0))
	{
		std::cout << "Cannot open falcon - Error: " << std::endl; // << dev.getErrorCode() << std::endl;
		return;
	}
	std::cout << "Opened falcon" << std::endl;

	std::cout << "Finding firmware" << std::endl;
	if(!dev.isFirmwareLoaded())
	{
		if(!dev.setFirmwareFile("test_firmware.bin"))
		{
			std::cout << "Cannot find firmware" << std::endl;
		}
		
		for(int i = 0; i < 10; ++i)
		{
			if(!dev.loadFirmware(false))
			{
				std::cout << "Could not load firmware" << std::endl;
			}
			else
			{
				std::cout <<"Firmware loaded" << std::endl;
				break;
			}
		}
	}
	std::cout << "Firmware found" << std::endl;
	std::cout << "closing" << std::endl;

	dev.close();
	std::cout << "reopening" << std::endl;
	if(!dev.open(0))
	{
		std::cout << "Cannot open falcon - Error: " << std::endl; // << dev.getErrorCode() << std::endl;
		return;
	}
	std::cout << "Opened falcon" << std::endl;

	std::cout << "Finding firmware" << std::endl;
	if(!dev.isFirmwareLoaded())
	{
		std::cout << "Loading firmware!" << std::endl;
		if(!dev.setFirmwareFile("test_firmware.bin"))
		{
			std::cout << "Cannot find firmware" << std::endl;
		}
		
		for(int i = 0; i < 10; ++i)
		{
			if(!dev.loadFirmware(false))
			{
				std::cout << "Could not load firmware" << std::endl;
			}
			else
			{
				std::cout <<"Firmware loaded" << std::endl;
				break;
			}
		}
	}
	std::cout << "Firmware found" << std::endl;
	
	for(int j = 0; j < 3; ++j)
	{
		f->setLEDStatus(2 << (j % 3));
		for(int i = 0; i < 1000; )
		{
			if(dev.runIOLoop()) ++i;
			else continue;
			printf("Loops: %8d | Enc1: %5d | Enc2: %5d | Enc3: %5d \n", (j*1000)+i,  f->getEncoderValues()[0], f->getEncoderValues()[1], f->getEncoderValues()[2]);
			++count;
		}
	}
	f->setLEDStatus(0);
	dev.runIOLoop();	
	dev.close();
}

int main(int argc, char** argv)
{
	signal(SIGINT, sigproc);
	signal(SIGQUIT, sigproc);
#if defined(LIBUSB)
	std::cout << "Running libusb test" << std::endl;
	dev.setFalconComm<FalconCommLibUSB>();
#elif defined(LIBFTDI)
	std::cout << "Running libftdi test" << std::endl;
	dev.setFalconComm<FalconCommLibFTDI>();
#elif defined(FTD2xx)
	std::cout << "Running ftd2xx test" << std::endl;
	dev.setFalconComm<FalconCommLibFTD2XX>();	
#endif	
	runFalconTest(dev);
	return 0;
}
