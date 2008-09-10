#ifndef FALCONCLIBASE_H
#define FALCONCLIBASE_H

#include <boost/program_options.hpp>
#include "core/FalconDevice.h"

namespace libnifalcon
{
	class FalconCLIBase
	{
	protected:
		boost::program_options::options_description m_progOptions;
		FalconDevice m_falconDevice;
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
