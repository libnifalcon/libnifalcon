#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "core/FalconDevice.h"
#ifdef LIBFTD2XX
#include "comm/FalconCommFTD2XX.h"
#endif
#ifdef LIBFTDI
#include "comm/FalconCommLibFTDI.h"
#endif
#include "firmware/FalconFirmwareNovintSDK.h"

using namespace libnifalcon;
using namespace boost::program_options;

void sigproc(int i)
{
	exit(0);
}

bool parseCommandLineOptions(int argc, char** argv)
{
	options_description program("Program Options");
	program.add_options()
		("help", "show this help message");

	options_description comm("Communication Options");
	comm.add_options()
#ifdef LIBFTDI
		("libftdi", "use libftdi driver")
#endif
#ifdef LIBFTD2XX
		("ftd2xx", "use ftd2xx driver")
#endif
		("device_index", value<int>(), "Index of device to open (default: 0)")
		;

	options_description firmware("Firmware Options");
	firmware.add_options()
		("firmware", value<std::string>(), "Firmware file")
		("force_firmware", "Force firmware download, even if already loaded")
		;
	
	options_description led("LED Status");
	led.add_options()
		("led_red", "Turn on Red LED")
		("led_green", "Turn on Green LED")
		("led_blue", "Turn on Blue LED");

	options_description cli;
	cli.add(program).add(comm).add(firmware).add(led);

	variables_map vm;
	store(parse_command_line(argc, argv, cli), vm);
	notify(vm);    
	
	if (vm.count("help")) {
		std::cout << "Usage: falcon_test_cli [args]" << std::endl;
		std::cout << cli << std::endl;
		return false;
	}

}

int main(int argc, char** argv)
{

	signal(SIGINT, sigproc);
	signal(SIGQUIT, sigproc);
	if(!parseCommandLineOptions(argc, argv))
		return 0;
	return 0;
}
