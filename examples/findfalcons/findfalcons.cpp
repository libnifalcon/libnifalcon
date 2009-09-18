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
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include "falcon/core/FalconLogger.h"
#include "falcon/core/FalconDevice.h"
#include "falcon/firmware/FalconFirmwareNovintSDK.h"
#include "falcon/util/FalconFirmwareBinaryNvent.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include "stdint.h"
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

void sigproc(int i)
{
	std::cout << "closing falcon and quitting" << std::endl;
	exit(0);
}

void runFalconTest()
{
	boost::shared_ptr<FalconFirmware> f;
	FalconKinematic* k;
	double position[3];
	unsigned int num_falcons = 0;
	int status, i;
	unsigned int count;
	unsigned int error_count = 0;
	unsigned int loop_count = 0;
	FalconDevice dev;

	dev.setFalconFirmware<FalconFirmwareNovintSDK>();

	f = dev.getFalconFirmware();
	if(!dev.getDeviceCount(num_falcons))
	{
		std::cout << "Cannot get device count" << std::endl;
		return;
	}

	count = 0;

	std::cout << "Falcons found: " << (int)num_falcons << std::endl;

	if(num_falcons == 0)
	{
		std::cout << "No falcons found, exiting..." << std::endl;
		return;
	}

	for(int z = 0; z < num_falcons; ++z)
	{
		std::cout << "Opening falcon " << z + 1  << std::endl;

		if(!dev.open(z))
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
					return;
				}
				else
				{
					std::cout <<"Firmware loaded" << std::endl;
					break;
				}
			}
			if(!dev.isFirmwareLoaded())
			{
				std::cout << "Firmware didn't load correctly. Try running findfalcons again" << std::endl;
				return;
			}
		}

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

	runFalconTest();
	return 0;
}
