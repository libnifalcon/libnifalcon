/***
 * @file FalconCLIBase.h
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
#ifndef FALCONCLIBASE_H
#define FALCONCLIBASE_H

#include <boost/program_options.hpp>
#include "FalconDeviceBoostThread.h"

namespace libnifalcon
{
	class FalconCLIBase
	{
	protected:
		boost::program_options::options_description m_progOptions;
		FalconDeviceBoostThread m_falconDevice;
		boost::program_options::variables_map m_varMap;
	public:
		FalconCLIBase();
		~FalconCLIBase()
		{
		}

		enum {
			DEVICE_OPTIONS = 0x1,
			COMM_OPTIONS = 0x2,
			FIRMWARE_OPTIONS = 0x4
		};
		virtual void addOptions(int value);	
		virtual bool parseOptions(int argc, char** argv);
	};
}
#endif
