/***
 * @file falcon_mouse.cpp
 * @brief Main execution class for falcon mouse emulation
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
#include <ctime>
#include <boost/program_options.hpp>
#include "falcon/core/FalconDevice.h"
#include "falcon/firmware/FalconFirmwareNovintSDK.h"
#include "falcon/kinematic/FalconKinematicStamper.h"
#include "falcon/util/FalconCLIBase.h"
#include "falcon/util/FalconDeviceBoostThread.h"
#include "falcon_mouse.h"

using namespace libnifalcon;
namespace po = boost::program_options;

bool run_app = true;

void sigproc(int i)
{
	run_app = false;
}

class FalconMouse : public FalconCLIBase
{
public:
	FalconMouse()
	{
	}
	~FalconMouse()
	{
	}
	void addOptions(int value)
	{
		FalconCLIBase::addOptions(value);
	}
	
	bool parseOptions(int argc, char** argv)
	{
		if(!FalconCLIBase::parseOptions(argc, argv)) return false;
		return true;
	}

	void runMouseLoop()
	{
		m_falconDevice->setFalconKinematic<FalconKinematicStamper>();
		//m_falconDevice.startThread();
		boost::array<double, 3> d;
		int i = 0, j = 0;
		int old_x = 0, old_y = 0, x = 0, y = 0;
		int width = getDisplayWidth();
		int height = getDisplayHeight();
		while(run_app)
		{
			//This math doesn't map the workspace to the window properly yet.
			if(!m_falconDevice->runIOLoop())
				continue;
			d = m_falconDevice->getPosition();
			old_x = x;
			old_y = y;
			x = ((d[0] + .06) / .12) * width;
			y = (((.12 - (d[1] + .06)) / .12) * height);

			if(old_x != x || old_y != y)
				setMousePosition(x, y);
		}
		//m_falconDevice.stopThread();
	}
};

int main(int argc, char** argv)
{

	signal(SIGINT, sigproc);
#if !defined(WIN32)
	signal(SIGQUIT, sigproc);
#endif
	FalconMouse f;
	if(!initDisplay())
	{
		std::cout << "Cannot initialize display, exiting..." << std::endl;
		return 1;
	}
	f.addOptions(FalconMouse::DEVICE_OPTIONS | FalconMouse::COMM_OPTIONS | FalconMouse::FIRMWARE_OPTIONS);
	if(!f.parseOptions(argc, argv))
		return 0;
	f.runMouseLoop();
	return 0;
}
