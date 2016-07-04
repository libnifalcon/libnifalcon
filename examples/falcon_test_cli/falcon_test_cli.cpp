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
#include "falcon/core/FalconDevice.h"
#include "falcon/kinematic/FalconKinematicStamper.h"
#include "falcon/firmware/FalconFirmwareNovintSDK.h"
#include "falcon/util/FalconCLIBase.h"
#include "FalconLoopTimeTest.h"
#include "FalconCubeTest.h"
#include "FalconSphereTest.h"
#include "FalconWallTest.h"
#include "FalconColorTest.h"

using namespace libnifalcon;

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
#ifdef FTC_USE_TIME
			m_parser.add_option("--loop_time_test").help("Loops infinitely, printing time every 1000 I/O loops (should be as near 1.0 as possible")
				.action("store_true");
#endif
			m_parser.add_option("--cube_test").help("Presents a cube-shaped surface to touch")
					.action("store_true");
			m_parser.add_option("--sphere_test").help("Presents a sphere-shaped surface to touch")
					.action("store_true");
			m_parser.add_option("--color_test").help("Fades LEDs based on the position of the end effector")
					.action("store_true");
			m_parser.add_option("--x_wall_test").help("Presents a wall surface to touch (force along x axis)")
					.action("store_true");
			m_parser.add_option("--y_wall_test").help("Presents a wall surface to touch (force along y axis)")
					.action("store_true");
			m_parser.add_option("--z_wall_test").help("Presents a wall surface to touch (force along z axis)")
					.action("store_true");
	}

	bool parseOptions(int argc, char** argv)
	{
		std::unique_ptr<FalconTestBase> t;
		if(!FalconCLIBase::parseOptions(argc, argv)) return false;
		optparse::Values options = m_parser.parse_args(argc, argv);

		if(options.get("loop_time_test"))
		{
			std::cout << "Running loop timing test" << std::endl;
			t.reset(new FalconLoopTimeTest(std::move(m_falconDevice)));
		}
		else if(options.get("cube_test"))
		{
			while(!calibrateDevice() && !stop);
			std::cout << "Running cube test" << std::endl;
			t.reset(new FalconCubeTest(std::move(m_falconDevice)));
		}
		else if(options.get("sphere_test"))
		{
			while(!calibrateDevice() && !stop);
			std::cout << "Running sphere test" << std::endl;
			t.reset(new FalconSphereTest(std::move(m_falconDevice)));
		}
		else if(options.get("color_test"))
		{
			while(!calibrateDevice() && !stop);
			std::cout << "Running color test" << std::endl;
			t.reset(new FalconColorTest(std::move(m_falconDevice)));
		}
		else if(options.get("x_wall_test"))
		{
			while(!calibrateDevice() && !stop);
			std::cout << "Running x axis wall test" << std::endl;
			t.reset(new FalconWallTest(std::move(m_falconDevice), 0));
		}
		else if(options.get("y_wall_test"))
		{
			while(!calibrateDevice() && !stop);
			std::cout << "Running y axis wall test" << std::endl;
			t.reset(new FalconWallTest(std::move(m_falconDevice), 1));
		}
		else if(options.get("z_wall_test"))
		{
			while(!calibrateDevice() && !stop);
			std::cout << "Running z axis wall test" << std::endl;
			t.reset(new FalconWallTest(std::move(m_falconDevice), 2));
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
