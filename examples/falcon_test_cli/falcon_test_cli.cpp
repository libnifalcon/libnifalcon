/***
 * @file falcon_test_cli.cpp
 * @brief Command line interface to run basic device tests on the falcon
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


#include <iostream>
#include <string>
#include <csignal>
#include <boost/program_options.hpp>
#include <boost/scoped_ptr.hpp>
#include "falcon/core/FalconDevice.h"
#include "falcon/kinematic/FalconKinematicStamper.h"
#include "falcon/firmware/FalconFirmwareNovintSDK.h"
#include "falcon/util/FalconCLIBase.h"
#include "FalconLoopTimeTest.h"
#include "FalconCubeTest.h"

using namespace libnifalcon;
namespace po = boost::program_options;

bool stop = false;

void sigproc(int i)
{
	if(!stop)
	{
		stop = true;
		std::cout << "Quitting" << std::endl;
	}
	else exit(0);
}


class FalconCLITest : public FalconCLIBase
{

public:
	enum
	{
		LED_OPTIONS = 0x8
	};
	void addOptions(int value)
	{
		FalconCLIBase::addOptions(value);
		po::options_description tests("Tests");
		tests.add_options()
#ifdef FTC_USE_TIME
			("loop_time_test", "Loops infinitely, printing time every 1000 I/O loops (should be as near 1.0 as possible)")
#endif
			("cube_test", "Presents a cube-shaped surface to touch")
			//("x_wall_test", "Presents a wall surface to touch (wall @ x = 0, force along positive x axis)")
			//("y_wall_test", "Presents a wall surface to touch (wall @ y = 0, force along positive y axis)")
			//("z_wall_test", "Presents a wall surface to touch (wall @ z = 150, force along positive z axis)")
			;
		m_progOptions.add(tests);
	}

	bool parseOptions(int argc, char** argv)
	{
		boost::scoped_ptr<FalconTestBase> t;
		if(!FalconCLIBase::parseOptions(argc, argv)) return false;
		
		if(m_varMap.count("loop_time_test"))
		{
			std::cout << "Running loop timing test" << std::endl;
			t.reset(new FalconLoopTimeTest(m_falconDevice));
		}
		else if(m_varMap.count("cube_test"))
		{
			while(!calibrateDevice() && !stop);
			std::cout << "Running cube test" << std::endl;
			t.reset(new FalconCubeTest(m_falconDevice));
		}

		while(!stop)
		{
			t->run();
		}
		return true;
	}
};
	
int main(int argc, char** argv)
{

	signal(SIGINT, sigproc);
#ifndef WIN32
	signal(SIGQUIT, sigproc);
#endif
	
	FalconCLITest f;
	f.addOptions(FalconCLITest::LED_OPTIONS | FalconCLITest::DEVICE_OPTIONS | FalconCLITest::COMM_OPTIONS | FalconCLITest::FIRMWARE_OPTIONS);	
	if(!f.parseOptions(argc, argv))
		return 0;
	return 0;
}
