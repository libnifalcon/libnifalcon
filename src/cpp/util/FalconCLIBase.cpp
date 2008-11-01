/***
 * @file FalconCLIBase.cpp
 * @brief Utility class for common operations (device opening, firmware loading, etc...) needed in command line interface examples
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

#include "falcon/util/FalconCLIBase.h"
#ifdef LIBFTD2XX
#include "falcon/comm/FalconCommFTD2XX.h"
#endif
#ifdef LIBFTDI
#include "falcon/comm/FalconCommLibFTDI.h"
#endif
#ifdef LIBUSB
#include "falcon/comm/FalconCommLibUSB.h"
#endif
#include "falcon/firmware/FalconFirmwareNovintSDK.h"
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
#if defined(LIBUSB)
				("libusb", "use libusb-1.0 based driver")
#endif
#if defined(LIBFTDI)
				("libftdi", "use libftdi based driver")
#elif defined(LIBFTD2XX)
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
				("skip_checksum", "Ignore checksum errors when loading firmware (useful for FTD2XX on non-windows platforms)")
				;
			m_progOptions.add(firmware);
		}
	}

	bool FalconCLIBase::parseOptions(FalconDevice& device, int argc, char** argv)
	{
		po::store(po::parse_command_line(argc, argv, m_progOptions), m_varMap);
		po::notify(m_varMap);    
		
		if (m_varMap.count("help")) {
			std::cout << "Usage: falcon_test_cli [args]" << std::endl;
			std::cout << m_progOptions << std::endl;
			return false;
		}
		
		device.setFalconFirmware<FalconFirmwareNovintSDK>();
		
		//First off, see if we have a communication method
		if(m_varMap.count("libftdi") && m_varMap.count("ftd2xx"))
		{
			std::cout << "Error: can only use one comm method. Choose either libftdi or ftd2xx, depending on which is available." << std::endl;		
		}
		//This is an either/or choice, since we can't link against both. Prefer libftdi. Thanks for the static linking against old libusb binaries, FTDI!

#if defined(LIBUSB)
		else if (m_varMap.count("libusb"))
		{
			std::cout << "Setting up libusb device" << std::endl;
			device.setFalconComm<FalconCommLibUSB>();
		}
#endif
#if defined(LIBFTDI)
		else if (m_varMap.count("libftdi"))
		{
			std::cout << "Setting up libftdi device" << std::endl;
			device.setFalconComm<FalconCommLibFTDI>();
		}
#elif defined(LIBFTD2XX)
		else if (m_varMap.count("ftd2xx"))
		{
			std::cout << "Setting up ftd2xx device" << std::endl;
			device.setFalconComm<FalconCommFTD2XX>();
		}
#endif
		//Device count check
		if(m_varMap.count("device_count"))
		{
			int8_t count;
			device.getDeviceCount(count);
			std::cout << "Connected Device Count: " << (int)count << std::endl;
			return false;
		}		
		else if(m_varMap.count("device_index"))
		{
			if(!device.open(m_varMap["device_index"].as<int>()))
			{
				std::cout << "Cannot open falcon device index " << m_varMap["device_index"].as<int>() << " - Lib Error Code: " << device.getErrorCode() << " Device Error Code: " << device.getFalconComm()->getDeviceErrorCode() << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "No device index specified to open, cannot continue (--help for options)" << std::endl;
			return false;			
		}

		//There's only one kind of firmware right now, so automatically set that.
		device.setFalconFirmware<FalconFirmwareNovintSDK>();
		//See if we have firmware
		bool firmware_loaded;	 
		if(m_varMap.count("firmware"))
		{
			//Check for existence of firmware file
			std::string firmware_file = m_varMap["firmware"].as<std::string>();
			if(!device.setFirmwareFile(firmware_file))
			{
				std::cout << "Cannot find firmware file - " << firmware_file << std::endl;
				return false;
			}
			//See if we need to load the firmware, or force it
			if(m_varMap.count("force_firmware") || !device.isFirmwareLoaded())
			{
				if(!device.loadFirmware(10, m_varMap.count("skip_checksum") > 0))
				{
					std::cout << "Cannot load firmware to device" << std::endl;
					std::cout << "Error Code: " << device.getErrorCode() << std::endl;
					if(device.getErrorCode() == 2000)
					{
						std::cout << "Device Error Code: " << device.getFalconComm()->getDeviceErrorCode() << std::endl;
					}
					return false;
				}
			}
		}
		if(!device.isFirmwareLoaded())
		{
			std::cout << "No firmware loaded to device, cannot continue" << std::endl;
			return false;
		}
		
		return true;
	}
}
