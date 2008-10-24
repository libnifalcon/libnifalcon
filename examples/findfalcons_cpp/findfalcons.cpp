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

static struct timeval _tstart[10], _tend[10];
static struct timezone tz;
void tstart(int i)
{
	gettimeofday(&_tstart[i], &tz);
}

void tend(int i)
{
	gettimeofday(&_tend[i],&tz);
}

double tval(int i)
{
	double t1, t2;
	t1 =  (double)_tstart[i].tv_sec + (double)_tstart[i].tv_usec/(1000*1000);
	t2 =  (double)_tend[i].tv_sec + (double)_tend[i].tv_usec/(1000*1000);
	return t2-t1;
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


	for(int j = 0; j < 1000; ++j)
	{
		f->setLEDStatus(2 << (j % 3));
		tstart(0);
		for(int i = 0; i < 1000; )
		{
			if(dev.runIOLoop()) ++i;
			else continue;
			//std::cout << f->getEncoderValues()[0] << " " << f->getEncoderValues()[1] << " " << f->getEncoderValues()[2] << std::endl;			
		}
		tend(0);
		std::cout << "Time: " << tval(0) << std::endl;
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
