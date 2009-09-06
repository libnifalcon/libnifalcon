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
 * Project info at http://libnifalcon.nonpolynomial.com/
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
#include "FalconWallTest.h"
#include "FalconColorTest.h"

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
			("color_test", "Fades LEDs based on the position of the end effector")
			("x_wall_test", "Presents a wall surface to touch (force along x axis)")
			("y_wall_test", "Presents a wall surface to touch (force along y axis)")
			("z_wall_test", "Presents a wall surface to touch (force along z axis)")
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
		else if(m_varMap.count("color_test"))
		{
			while(!calibrateDevice() && !stop);
			std::cout << "Running color test" << std::endl;
			t.reset(new FalconColorTest(m_falconDevice));
		}
		else if(m_varMap.count("x_wall_test"))
		{
			while(!calibrateDevice() && !stop);
			std::cout << "Running x axis wall test" << std::endl;
			t.reset(new FalconWallTest(m_falconDevice, 0));
		}
		else if(m_varMap.count("y_wall_test"))
		{
			while(!calibrateDevice() && !stop);
			std::cout << "Running y axis wall test" << std::endl;
			t.reset(new FalconWallTest(m_falconDevice, 1));
		}
		else if(m_varMap.count("z_wall_test"))
		{
			while(!calibrateDevice() && !stop);
			std::cout << "Running z axis wall test" << std::endl;
			t.reset(new FalconWallTest(m_falconDevice, 2));
		}
		else
		{
			std::cout << "No test chosen, exiting..." << std::endl;
			return true;
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
