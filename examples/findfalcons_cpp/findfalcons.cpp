/*
 * Implementation file for NovInt Falcon FindFalcon Example Program
 *
 * Copyright (c) 2007 Kyle Machulis <kyle@nonpolynomial.com> http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 */

#include "core/FalconDevice.h"
#ifdef LIBFTD2XX
#include "comm/FalconCommFTD2XX.h"
#endif
#ifdef LIBFTDI
#include "comm/FalconCommLibFTDI.h"
#endif
#include "firmware/FalconNovintFirmware.h"
#include "kinematic/FalconKinematicStamper.h"
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
	dev.close();
	exit(0);
}

void runFalconTest(FalconDevice d)
{
	FalconNovintFirmware f;
	FalconKinematicStamper k(true);
	double position[3];
	int8_t num_falcons;
	int status, i;
	u_int8_t count;
	u_int32_t error_count = 0;
	u_int32_t loop_count = 0;

	dev.setFalconFirmware(&f);
	dev.setFalconKinematic(&k);
	
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
	
	if(!dev.setFirmwareFile("test_firmware.bin"))
	{
		std::cout << "Cannot find firmware" << std::endl;
	}

	for(int i = 0; i < 10; ++i)
	{
		if(!dev.loadFirmware())
		{
			std::cout << "Could not load firmware" << std::endl;
		}
		else
		{
			std::cout <<"Firmware loaded" << std::endl;
			break;
		}
	}

	for(int j = 0; j < 3; ++j)
	{
		f.setLEDStatus(2 << j);
		
		for(int i = 0; i < 1000; ++i)
		{
			dev.runIOLoop();
			std::cout << f.getEncoderValues()[0] << " " << f.getEncoderValues()[1] << " " << f.getEncoderValues()[2] << std::endl;
		}
	}
	f.setLEDStatus(0);
	dev.runIOLoop();
	
	dev.close();
}

int main(int argc, char** argv)
{
	signal(SIGINT, sigproc);
	signal(SIGQUIT, sigproc);
#ifdef LIBFTDI
	std::cout << "Running libftdi test" << std::endl;
	dev.setFalconComm(new FalconCommLibFTDI());
	runFalconTest(dev);
#endif
	return 0;
}
