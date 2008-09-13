#include "FalconCLIBase.h"
#ifdef LIBFTD2XX
#include "comm/FalconCommFTD2XX.h"
#endif
#ifdef LIBFTDI
#include "comm/FalconCommLibFTDI.h"
#endif
#include "firmware/FalconFirmwareNovintSDK.h"
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
		m_falconDevice.setFalconFirmware<FalconFirmwareNovintSDK>();
	}


	void FalconCLIBase::addOptions(int value)
	{
		if(value & COMM_OPTIONS)
		{
			po::options_description comm("Communication Options");
			comm.add_options()
#ifdef LIBFTDI
				("libftdi", "use libftdi based driver")
#endif
#ifdef LIBFTD2XX
				("ftd2xx", "use ftd2xx based driver")
#endif
				;
			m_progOptions.add(comm);
		}

		if(value & DEVICE_OPTIONS)
		{
			po::options_description device("Device options");
			device.add_options()
				("device_count", "Print the number of devices currently connected and return")
				("device_index", po::value<int>(), "Opens device of given index (starts at 0)")
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
		po::store(po::parse_command_line(argc, argv, m_progOptions), m_varMap);
		po::notify(m_varMap);    

		if (m_varMap.count("help")) {
			std::cout << "Usage: falcon_test_cli [args]" << std::endl;
			std::cout << m_progOptions << std::endl;
			return false;
		}

		//First off, see if we have a communication method
		if(m_varMap.count("libftdi") && m_varMap.count("ftd2xx"))
		{
			std::cout << "Error: can only use one comm method. Choose either libftdi or ftd2xx." << std::endl;		
		}
#ifdef LIBFTDI
		else if (m_varMap.count("libftdi"))
		{
			std::cout << "Setting up libftdi device" << std::endl;
			m_falconDevice.setFalconComm<FalconCommLibFTDI>();
		}
#endif
#ifdef LIBFTD2XX
		else if (m_varMap.count("ftd2xx"))
		{
			std::cout << "Setting up ftd2xx device" << std::endl;
			m_falconDevice.setFalconComm<FalconCommFTD2XX>();
		}
#endif
		//Device count check
		if(m_varMap.count("device_count"))
		{
			int8_t count;
			m_falconDevice.getDeviceCount(count);
			std::cout << "Connected Device Count: " << (int)count << std::endl;
			return false;
		}		
		else if(m_varMap.count("device_index"))
		{
			if(!m_falconDevice.open(m_varMap["device_index"].as<int>()))
			{
				std::cout << "Cannot open falcon device index " << m_varMap["device_index"].as<int>() << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "No device index specified to open, cannot continue (--help for options)" << std::endl;
			return false;			
		}

		//There's only one kind of firmware right now, so automatically set that.
		m_falconDevice.setFalconFirmware<FalconFirmwareNovintSDK>();
		//See if we have firmware
		bool firmware_loaded;	 
		if(m_varMap.count("firmware"))
		{
			//Check for existence of firmware file
			std::string firmware_file = m_varMap["firmware"].as<std::string>();
			if(!m_falconDevice.setFirmwareFile(firmware_file))
			{
				std::cout << "Cannot find firmware file - " << firmware_file << std::endl;
				return false;
			}
			//See if we need to load the firmware, or force it
			if(m_varMap.count("force_firmware") || !m_falconDevice.isFirmwareLoaded())
			{
				if(!m_falconDevice.loadFirmware(10))
				{
					std::cout << "Cannot load firmware to device" << std::endl;
					std::cout << "Error Code: " << m_falconDevice.getErrorCode() << std::endl;
					return false;
				}
			}
		}
		if(!m_falconDevice.isFirmwareLoaded())
		{
			std::cout << "No firmware loaded to device, cannot continue" << std::endl;
			return false;
		}
		
		return true;
	}
}
