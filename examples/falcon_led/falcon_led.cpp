/***
 * @file falcon_led.cpp
 * @brief Command line interface to change LED states on the falcon
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

#include <iostream>
#include <string>
#include <csignal>
#include "falcon/core/FalconDevice.h"
#include "falcon/util/FalconCLIBase.h"
#include "falcon/firmware/FalconFirmwareNovintSDK.h"

using namespace libnifalcon;

bool stop = true;

void sigproc(int i)
{
	if(!stop)
	{
		stop = true;
		std::cout << "Quitting" << std::endl;
	}
	else exit(0);
}


class FalconLED : public FalconCLIBase
{
public:
	void addOptions(int value)
	{
		FalconCLIBase::addOptions(value);
		m_parser.add_option("--led_red").help("Turn on Red LED")
				.action("store_true");
		m_parser.add_option("--led_green").help("Turn on Green LED")
				.action("store_true");
		m_parser.add_option("--led_blue").help("Turn on Blue LED")
				.action("store_true");
	}

	bool parseOptions(int argc, char** argv)
	{
		if(!FalconCLIBase::parseOptions(argc, argv)) return false;
		optparse::Values options = m_parser.parse_args(argc, argv);

		int led = 0;
		if(options.get("led_red"))
		{
			std::cout << "Turning on RED LED" << std::endl;
			led |= FalconFirmware::RED_LED;
		}
		if(options.get("led_green"))
		{
			std::cout << "Turning on GREEN LED" << std::endl;
			led |= FalconFirmware::GREEN_LED;
		}
		if(options.get("led_blue"))
		{
			std::cout << "Turning on BLUE LED" << std::endl;			
			led |= FalconFirmware::BLUE_LED;
		}
		m_falconDevice->getFalconFirmware()->setLEDStatus(led);
		m_falconDevice->runIOLoop();
		return true;
	}
};
	
int main(int argc, char** argv)
{

	signal(SIGINT, sigproc);
#ifndef WIN32
	signal(SIGQUIT, sigproc);
#endif
	
	FalconLED f;
	f.addOptions(FalconLED::DEVICE_OPTIONS | FalconLED::COMM_OPTIONS | FalconLED::FIRMWARE_OPTIONS);
	f.parseOptions(argc, argv);
	return 0;
}
