/***
 * @file FalconCLIBase.h
 * @brief Utility class for common operations (device opening, firmware loading, etc...) needed in command line interface examples
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONCLIBASE_H
#define FALCONCLIBASE_H

#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include "falcon/core/FalconDevice.h"
#include "falcon/core/FalconLogger.h"

namespace libnifalcon
{
/**
 * @class FalconCLIBase
 * @ingroup UtilityClasses
 *
 * FalconCLIBase provides an simple framework for building test applications with libnifalcon. It wraps the information needed
 * from the user into a set of boost::program_options, and does the work of parsing command line options for:
 * 
 * - Device counts and connections
 * - Firmware loading
 * - Communications (if more that one comm core is available)
 *
 * The FalconCLIBase class is only available if the boost::program_options library is available on the system.
 *
 * See the falcon_test_cli program for an example of a test suite program built using FalconCLIBase
 */

	class FalconCLIBase
	{
	public:
		/**
		 * Constructor
		 */
		FalconCLIBase();

		/**
		 * Destructor
		 */
		virtual ~FalconCLIBase()
		{
		}

		/**
		 * Enumeration of options that this class provides
		 */
		enum {
			DEVICE_OPTIONS = 0x1, /**< Allows user to choose device index, see device count, etc... */
			COMM_OPTIONS = 0x2,  /**< Allows user to choose communications type. Not commonly used since comm is usually set by default in FalconDevice now (see FalconDevice constructor). */
			FIRMWARE_OPTIONS = 0x4 /**< Allows user to choose firmware type, load firmware, etc... */
		};
	protected:
		boost::shared_ptr<FalconDevice> m_falconDevice; /**< Internal device structure */
		boost::program_options::options_description m_progOptions; /**< Structure of options that we'll allow */
		boost::program_options::variables_map m_varMap; /**< Options passed to us through the command line */
		bool m_displayCalibrationMessage; /**< String to display with calibration instructions */
		
		/**
		 * Builds options map. Overridden to add application specific options
		 */
		virtual void addOptions(int value); 

		/**
		 * Parses options off command line. Overridden to handle application specific options
		 *
		 * @return True if command line options are parsed correctly, false otherwise
		 */
		virtual bool parseOptions(int argc, char** argv);

		/**
		 * Prints help message
		 */
		void outputProgramOptions();

		/**
		 * Conveinence function for handling device calibration 
		 */
		bool calibrateDevice();


	private:
		DECLARE_LOGGER();


	};
}
#endif
