/***
 * @file FalconCLIBase.cpp
 * @brief Utility class for common operations (device opening, firmware loading, etc...) needed in command line interface examples
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include <iostream>

#include "falcon/util/FalconCLIBase.h"
#include "falcon/firmware/FalconFirmwareNovintSDK.h"
#include "falcon/util/FalconFirmwareBinaryTest.h"
#include "falcon/util/FalconFirmwareBinaryNvent.h"


#ifdef ENABLE_LOGGING
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
static constexpr log4cxx::LogString TTCC_CONVERSION_PATTERN(LOG4CXX_STR("%-5p [%c] - %m%n"));

/**
 * Statically initialize the log4cxx library.
 */
void configureLogging(const std::string & logString, const log4cxx::LevelPtr level) {
	log4cxx::LayoutPtr layout(new log4cxx::PatternLayout(logString));
	log4cxx::AppenderPtr appender(new log4cxx::ConsoleAppender(layout));
	log4cxx::BasicConfigurator::configure(appender);
	log4cxx::LoggerPtr rootlogger = log4cxx::Logger::getRootLogger();
	rootlogger->setLevel(level);
}
#endif

namespace libnifalcon
{
	FalconCLIBase::FalconCLIBase() :
		m_falconDevice(std::unique_ptr<FalconDevice>(new FalconDevice)),
		m_displayCalibrationMessage(true),
		INIT_LOGGER("FalconCLIBase")
	{
		m_parser = optparse::OptionParser().description("Program Options");
	}

	void FalconCLIBase::addOptions(int value)
	{
		if(value & DEVICE_OPTIONS)
		{
			m_parser.add_option("--device_count").help("Print the number of devices currently connected and return")
					.action("store_true");
			m_parser.add_option("--device_index").help("Opens device of given index (starts at 0)")
					.action("store").type("int");
		}

		if(value & FIRMWARE_OPTIONS)
		{
			m_parser.add_option("--nvent_firmware").help("Use 'nVent' firmware (Recommended)")
					.action("store_true");
			m_parser.add_option("--test_firmware").help("Use test firmware")
					.action("store_true");
			m_parser.add_option("--firmware_file").help("Specify external firmware file (instead of nvent or test)")
					.metavar("FILE");
			m_parser.add_option("--force_firmware").help("Force firmware download, even if already loaded")
					.action("store_true");
			m_parser.add_option("--skip_checksum").help("Ignore checksum errors when loading firmware (useful for FTD2XX on non-windows platforms)")
					.action("store_true");
		}

#ifdef ENABLE_LOGGING
		m_parser.add_option("--debug_level").help("Level of debug messages to print (FATAL, ERROR, WARN, INFO, DEBUG) (Default: FATAL)"));
#endif
	}

	void FalconCLIBase::outputProgramOptions()
	{
		m_parser.print_help();
	}

	bool FalconCLIBase::parseOptions(int argc, char** argv)
	{
		optparse::Values options = m_parser.parse_args(argc, argv);

#ifdef ENABLE_LOGGING
		std::string logPattern(TTCC_CONVERSION_PATTERN);
		log4cxx::LevelPtr logLevel = log4cxx::Level::toLevel("FATAL");

		if(options.is_set("debug_level"))
			logLevel = log4cxx::Level::toLevel((string)options.get("debug_level"));

		configureLogging(logPattern, logLevel);
#endif

		m_falconDevice->setFalconFirmware<FalconFirmwareNovintSDK>();

		//First off, see if we have a communication method
		if(options.get("libusb") && options.get("ftd2xx"))
		{
			std::cout << "Error: can only use one comm method. Choose either libusb or ftd2xx, depending on which is available." << std::endl;
			return false;
		}

		//Device count check
		if(options.get("device_count"))
		{
			unsigned int count;
			m_falconDevice->getDeviceCount(count);
			std::cout << "Connected Device Count: " << count << std::endl;
			return false;
		}
		else if(options.is_set("device_index"))
		{
			if(!m_falconDevice->open((int)options.get("device_index")))
			{
				std::cout << "Cannot open falcon device index " << (int)options.get("device_index") << " - Lib Error Code: " << m_falconDevice->getErrorCode() << " Device Error Code: " << m_falconDevice->getFalconComm()->getDeviceErrorCode() << std::endl;
				return false;
			}
		}
		else
		{
                        // assume device index 0, if none given
			if(!m_falconDevice->open(0))
			{
				std::cout << "Cannot open falcon device index 0 - Lib Error Code: " << m_falconDevice->getErrorCode() << " Device Error Code: " << m_falconDevice->getFalconComm()->getDeviceErrorCode() << std::endl;
				return false;
			}
		}

		//There's only one kind of firmware right now, so automatically set that.
		m_falconDevice->setFalconFirmware<FalconFirmwareNovintSDK>();
		//See if we have firmware
		bool firmware_loaded = false;
		if(!options.is_set("force_firmware"))
			firmware_loaded = m_falconDevice->isFirmwareLoaded();
		if(!firmware_loaded)
		{
			std::cout << "Loading firmware" << std::endl;
			uint8_t* firmware_block;
			long firmware_size;
			//First, see if we're trying to load a custom firmware file
			if(options.is_set("firmware_file"))
			{
				//Check for existence of firmware file
				std::string firmware_file = (std::string)options.get("firmware_file");
				if(!m_falconDevice->setFirmwareFile(firmware_file))
				{
					std::cout << "Cannot find firmware file - " << firmware_file << std::endl;
					return false;
				}
				for(int i = 0; i < 10; ++i)
				{
					if(!m_falconDevice->loadFirmware((bool)options.get("skip_checksum")))
					{
						std::cout << "Cannot load firmware to device" << std::endl;
						std::cout << "Error Code: " << m_falconDevice->getErrorCode() << std::endl;
						if(m_falconDevice->getErrorCode() == 2000)
						{
							std::cout << "Device Error Code: " << m_falconDevice->getFalconComm()->getDeviceErrorCode() << std::endl;
						}
						m_falconDevice->close();
						if(!m_falconDevice->open((int)options.get("device_index")))
						{
							std::cout << "Cannot open falcon device index " << (int)options.get("device_index") << " - Lib Error Code: " << m_falconDevice->getErrorCode() << " Device Error Code: " << m_falconDevice->getFalconComm()->getDeviceErrorCode() << std::endl;
							return false;
						}
					}
					else
					{
						firmware_loaded = true;
						break;
					}
				}
			}
			else
			{
				if(options.get("nvent_firmware"))
				{
					firmware_block = const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE);
					firmware_size = NOVINT_FALCON_NVENT_FIRMWARE_SIZE;
				}
				else if(options.get("test_firmware"))
				{
					firmware_block = const_cast<uint8_t*>(NOVINT_FALCON_TEST_FIRMWARE);
					firmware_size = NOVINT_FALCON_TEST_FIRMWARE_SIZE;
				}
				for(int i = 0; i < 10; ++i)
				{
					if(!m_falconDevice->getFalconFirmware()->loadFirmware((bool)options.get("skip_checksum"), NOVINT_FALCON_NVENT_FIRMWARE_SIZE, const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE)))
					{
						LOG_ERROR("Firmware loading try failed");
						//Completely close and reopen
						m_falconDevice->close();
						if(!m_falconDevice->open((int)options.get("device_index")))
						{
							std::cout << "Cannot open falcon device index " << (int)options.get("device_index") << " - Lib Error Code: " << m_falconDevice->getErrorCode() << " Device Error Code: " << m_falconDevice->getFalconComm()->getDeviceErrorCode() << std::endl;
							return false;
						}
					}
					else
					{
						firmware_loaded = true;
						break;
					}
				}
			}
		}
		else if(!firmware_loaded)
		{
			std::cout << "No firmware loaded to device, and no firmware specified to load (--nvent_firmware, --test_firmware, etc...). Cannot continue" << std::endl;
			return false;
		}
		else
		{
			return true;
		}
		if(!firmware_loaded || !m_falconDevice->isFirmwareLoaded())
		{
			std::cout << "No firmware loaded to device, cannot continue" << std::endl;
			return false;
		}
		std::cout << "Firmware loaded" << std::endl;
		return true;
	}

	bool FalconCLIBase::calibrateDevice()
	{
		m_falconDevice->getFalconFirmware()->setHomingMode(true);
		m_falconDevice->runIOLoop();
		if(!m_falconDevice->getFalconFirmware()->isHomed())
		{
			m_falconDevice->getFalconFirmware()->setLEDStatus(libnifalcon::FalconFirmware::RED_LED);
			if(m_displayCalibrationMessage)
			{
				std::cout << "Falcon not currently calibrated. Move control all the way out then push straight all the way in." << std::endl;
				m_displayCalibrationMessage = false;
			}
			return false;
		}
		std::cout << "Falcon calibrated successfully." << std::endl;
		m_falconDevice->getFalconFirmware()->setLEDStatus(libnifalcon::FalconFirmware::GREEN_LED);
		return true;
	}
}
