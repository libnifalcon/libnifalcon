/***
 * @file falcon_test_cli.cpp
 * @brief Command line interface to run basic device tests on the falcon
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

#include <iostream>
#include <string>
#include <csignal>
#include <boost/program_options.hpp>
#include "core/FalconDevice.h"
#ifdef LIBFTD2XX
#include "comm/FalconCommFTD2XX.h"
#endif
#ifdef LIBFTDI
#include "comm/FalconCommLibFTDI.h"
#endif
#include "firmware/FalconFirmwareNovintSDK.h"
#include "util/FalconCLIBase.h"

using namespace libnifalcon;
namespace po = boost::program_options;

void sigproc(int i)
{
	exit(0);
}

class FalconCLITest : public FalconCLIBase
{
	FalconDevice m_falconDevice;
public:
	enum
	{
		LED_OPTIONS = 0x8
	};
	FalconCLITest()
	{
	}
	~FalconCLITest()
	{
	}
	void addOptions(int value)
	{
		FalconCLIBase::addOptions(value);
		if(value & LED_OPTIONS)
		{
			po::options_description led("LED Status");
			led.add_options()
				("led_red", "Turn on Red LED")
				("led_green", "Turn on Green LED")
				("led_blue", "Turn on Blue LED");
			m_progOptions.add(led);
		}
	}
	bool parseOptions(int argc, char** argv)
	{
		if(!FalconCLIBase::parseOptions(m_falconDevice, argc, argv)) return false;
		int led = 0;
		if(m_varMap.count("led_red"))
		{
			std::cout << "Turning on RED LED" << std::endl;
			led |= FalconFirmware::RED_LED;
		}
		if(m_varMap.count("led_green"))
		{
			std::cout << "Turning on GREEN LED" << std::endl;
			led |= FalconFirmware::GREEN_LED;
		}
		if(m_varMap.count("led_blue"))
		{
			std::cout << "Turning on BLUE LED" << std::endl;			
			led |= FalconFirmware::BLUE_LED;
		}
		m_falconDevice.getFalconFirmware()->setLEDStatus(led);
		m_falconDevice.runIOLoop();
		return true;
	}
};
	
int main(int argc, char** argv)
{

	signal(SIGINT, sigproc);
	signal(SIGQUIT, sigproc);
	FalconCLITest f;
	f.addOptions(FalconCLITest::LED_OPTIONS | FalconCLITest::DEVICE_OPTIONS | FalconCLITest::COMM_OPTIONS | FalconCLITest::FIRMWARE_OPTIONS);	
	if(!f.parseOptions(argc, argv))
		return 0;
	return 0;
}
