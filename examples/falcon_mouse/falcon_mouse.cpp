/***
 * @file falcon_mouse.cpp
 * @brief Main execution class for falcon mouse emulation
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
#include <ctime>
#include <boost/program_options.hpp>
#include "falcon/core/FalconDevice.h"
#ifdef LIBFTD2XX
#include "falcon/comm/FalconCommFTD2XX.h"
#endif
#ifdef LIBFTDI
#include "falcon/comm/FalconCommLibFTDI.h"
#endif
#include "falcon/firmware/FalconFirmwareNovintSDK.h"
#include "falcon/kinematic/FalconKinematicStamper.h"
#include "falcon/util/FalconCLIBase.h"
#include "falcon/util/FalconDeviceBoostThread.h"
#include "falcon_mouse.h"

using namespace libnifalcon;
namespace po = boost::program_options;

bool run_app = true;

void sigproc(int i)
{
	run_app = false;
}
/*
void millisleep(unsigned long milliseconds)
{
	timespec tmReq;
    tmReq.tv_sec = (time_t)(milliseconds / 1000);
    tmReq.tv_nsec = (milliseconds % 10)* 10 * 10;
    // we're not interested in remaining time nor in return value
    (void)nanosleep(&tmReq, (timespec *)NULL);
}
*/

class FalconMouse : public FalconCLIBase
{
	FalconDeviceBoostThread m_falconDevice;
public:
	enum
	{
		LED_OPTIONS = 0x8
	};
	FalconMouse()
	{
	}
	~FalconMouse()
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

	void runMouseLoop()
	{
		m_falconDevice.setFalconKinematic<FalconKinematicStamper>();
		m_falconDevice.startThread();
		double d[3];
		int i = 0, j = 0;
		int old_x = 0, old_y = 0, x = 0, y = 0;
		while(run_app)
		{
			//This math doesn't map the workspace to the window properly yet.
			m_falconDevice.getPosition(d);
			old_x = x;
			old_y = y;
			x = ((d[0] + 60.0) / 120.0) * 1600;
			y = (((120.0 - (d[1] + 60.0)) / 120.0) * 1050);

			if(old_x != x || old_y != y)
				setMousePosition(x, y);
		}
		m_falconDevice.stopThread();
	}
};

int main(int argc, char** argv)
{

	signal(SIGINT, sigproc);
	signal(SIGQUIT, sigproc);
	FalconMouse f;
	if(!initDisplay())
	{
		std::cout << "Cannot initialize display, exiting..." << std::endl;
		return 1;
	}
	f.addOptions(FalconMouse::LED_OPTIONS | FalconMouse::DEVICE_OPTIONS | FalconMouse::COMM_OPTIONS | FalconMouse::FIRMWARE_OPTIONS);
	if(!f.parseOptions(argc, argv))
		return 0;
	f.runMouseLoop();
	return 0;
}
