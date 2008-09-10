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
	virtual void addOptions(int value)
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
	virtual bool parseOptions(int argc, char** argv)
	{
		if(!FalconCLIBase::parseOptions(argc, argv)) return false;		
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
