#include "FalconCLIBase.h"
#ifdef LIBFTD2XX
#include "comm/FalconCommFTD2XX.h"
#endif
#ifdef LIBFTDI
#include "comm/FalconCommLibFTDI.h"
#endif
#include <iostream>

namespace libnifalcon
{
	namespace po = boost::program_options;
	FalconCLIBase::FalconCLIBase()
	{
		po::options_description program("Program Options");
		program.add_options()
			("help", "show this help message");
		m_progOptions.add(program);
	}


	void FalconCLIBase::addOptions(int value)
	{
		if(value & COMM_OPTIONS)
		{
			po::options_description comm("Communication Options");
			comm.add_options()
#ifdef LIBFTDI
				("libftdi", "use libftdi driver")
#endif
#ifdef LIBFTD2XX
				("ftd2xx", "use ftd2xx driver")
#endif
				;
			m_progOptions.add(comm);
		}

		if(value & DEVICE_OPTIONS)
		{
			po::options_description device("Device options");
			device.add_options()
				("device_count", "Print the number of devices currently connected and return")
				("device_index", po::value<int>(), "Index of device to open (default: 0)")
				;

			m_progOptions.add(device);
		}

		if(value & FIRMWARE_OPTIONS)
		{
			po::options_description firmware("Firmware Options");
			firmware.add_options()
				("firmware", po::value<std::string>(), "Firmware file")
				("force_firmware", "Force firmware download, even if already loaded")
				;
			m_progOptions.add(firmware);
		}
	}

	bool FalconCLIBase::parseOptions(int argc, char** argv)
	{
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, m_progOptions), vm);
		po::notify(vm);    

		if (vm.count("help")) {
			std::cout << "Usage: falcon_test_cli [args]" << std::endl;
			std::cout << m_progOptions << std::endl;
			return false;
		}

		//First off, see if we have a communication method
		if(vm.count("libftdi") && vm.count("ftd2xx"))
		{
			std::cout << "Error: can only use one comm method. Choose either libftdi or ftd2xx." << std::endl;		
		}
#ifdef LIBFTDI
		else if (vm.count("libftdi"))
		{
			m_falconDevice.setFalconComm(new FalconCommLibFTDI());
		}
#endif
#ifdef LIBFTD2XX
		else if (vm.count("ftd2xx"))
		{
			m_falconDevice.setFalconComm(new FalconCommFTD2XX());
		}
#endif
		//Device count check
		if(vm.count("device_count"))
		{
			int8_t count;
			m_falconDevice.getDeviceCount(count);
			std::cout << "Connected Device Count: " << (int)count << std::endl;
			return false;
		}
	
		//See if we have firmware
		
		
		std::cout << "Usage: falcon_test_cli [args]" << std::endl;
		std::cout << m_progOptions << std::endl;
		return false;		
	}
}
