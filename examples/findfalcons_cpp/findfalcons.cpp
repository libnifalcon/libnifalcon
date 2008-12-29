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

#include "falcon/core/FalconLogger.h"
#include "falcon/core/FalconDevice.h"
#if defined(LIBUSB)
#include "falcon/comm/FalconCommLibUSB.h"
#elif defined(LIBFTDI)
#include "falcon/comm/FalconCommLibFTDI.h"
#elif defined(LIBFTD2XX)
#include "falcon/comm/FalconCommFTD2XX.h"
#endif
#include "falcon/firmware/FalconFirmwareNovintSDK.h"
#include "falcon/util/FalconFirmwareBinaryNvent.h"
#include <sys/time.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <csignal>

#ifdef ENABLE_LOGGING
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
static const log4cxx::LogString TTCC_CONVERSION_PATTERN(LOG4CXX_STR("%-5p [%c] - %m%n"));

/**
 * Statically initialize the log4cxx library.
 */
void configureLogging(const std::string logString, const log4cxx::LevelPtr level) {
  log4cxx::LayoutPtr layout(new log4cxx::PatternLayout(logString));
  log4cxx::AppenderPtr appender(new log4cxx::ConsoleAppender(layout));
  log4cxx::BasicConfigurator::configure(appender);
  log4cxx::LoggerPtr rootlogger = log4cxx::Logger::getRootLogger();
  rootlogger->setLevel(level);
}
#endif


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
	uint8_t count;
	uint32_t error_count = 0;
	uint32_t loop_count = 0;

	dev.setFalconFirmware<FalconFirmwareNovintSDK>();


	f = dev.getFalconFirmware();
	if(!dev.getDeviceCount(num_falcons))
	{
		std::cout << "Cannot get device_libftdi count" << std::endl;
	}

	count = 0;

	std::cout << "Falcons found: " << (int)num_falcons << std::endl;

	std::cout << "Opening falcon" << std::endl;
	
	if(!dev.open(0))
	{
		std::cout << "Cannot open falcon - Error: " << std::endl; // << dev.getErrorCode() << std::endl;
		return;
	}
	std::cout << "Opened falcon" << std::endl;

	if(!dev.isFirmwareLoaded())
	{
		std::cout << "Loading firmware" << std::endl;
		for(int i = 0; i < 10; ++i)
		{
			if(!dev.getFalconFirmware()->loadFirmware(true, NOVINT_FALCON_NVENT_FIRMWARE_SIZE, const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE)))
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
#ifndef WIN32
	signal(SIGQUIT, sigproc);
#endif

#ifdef ENABLE_LOGGING
	std::string logPattern(TTCC_CONVERSION_PATTERN);
	log4cxx::LevelPtr logLevel = log4cxx::Level::toLevel("DEBUG");
	configureLogging(logPattern, logLevel);
#endif

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
