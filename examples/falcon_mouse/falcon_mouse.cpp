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
#include "kinematic/FalconKinematicStamper.h"
#include "util/FalconCLIBase.h"
#include "falcon_mouse.h"

using namespace libnifalcon;
namespace po = boost::program_options;

void sigproc(int i)
{
	exit(0);
}

class FalconMouse : public FalconCLIBase
{
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
		if(!FalconCLIBase::parseOptions(argc, argv)) return false;
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
		FalconKinematic::WorkspaceBounds x;
		m_falconDevice.setFalconKinematic<FalconKinematicStamper>(x);
		while(1)
		{
			m_falconDevice.runIOLoop();
			//This math doesn't map the workspace to the window properly yet.		
			int x = ((m_falconDevice.getPosition()[0] + 60.0) / 120.0) * 1600;
			int y = (((120.0 - (m_falconDevice.getPosition()[1] + 60.0)) / 120.0) * 1050);
			setMousePosition(x, y);
		}
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
