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
#include "falcon/core/FalconDevice.h"
#include "falcon/kinematic/FalconKinematicStamper.h"
#include "falcon/firmware/FalconFirmwareNovintSDK.h"
#include "falcon/util/FalconCLIBase.h"
#ifndef _MINGW_
#ifndef WIN32
#include "sys/time.h"
#endif
#endif

using namespace libnifalcon;
namespace po = boost::program_options;

bool stop = true;

void sigproc(int i)
{
	if(!stop)
	{
		stop = true;
		std::cout << "Quitting" << std::endl;
	}
	else exit(0);
}
#ifndef _MINGW_
#ifndef WIN32
static struct timeval _tstart, _tend;
static struct timezone tz;
void tstart()
{
	gettimeofday(&_tstart, &tz);
}

void tend()
{
	gettimeofday(&_tend,&tz);
}

double tval()
{
	double t1, t2;
	t1 =  (double)_tstart.tv_sec + (double)_tstart.tv_usec/(1000*1000);
	t2 =  (double)_tend.tv_sec + (double)_tend.tv_usec/(1000*1000);
	return t2-t1;
}
#endif
#endif

class FalconCLITest : public FalconCLIBase
{
	FalconDevice m_falconDevice;
public:
	enum
	{
		LED_OPTIONS = 0x8
	};
	FalconCLITest()
	{
	}
	~FalconCLITest()
	{
	}
	void addOptions(int value)
	{
		FalconCLIBase::addOptions(value);
		if(value & LED_OPTIONS)
		{
			po::options_description led("LED Status");
			led.add_options()
				("led_red", "Turn on Red LED")
				("led_green", "Turn on Green LED")
				("led_blue", "Turn on Blue LED");
			m_progOptions.add(led);

			po::options_description tests("Tests");
			tests.add_options()
#ifndef _MINGW_
#ifndef WIN32
				("loop_time_test", "Loops infinitely, printing time every 1000 I/O loops (should be as near 1.0 as possible)")
#endif
#endif
				("cube_test", "Presents a cube-shaped surface to touch")
				//("x_wall_test", "Presents a wall surface to touch (wall @ x = 0, force along positive x axis)")
				//("y_wall_test", "Presents a wall surface to touch (wall @ y = 0, force along positive y axis)")
				//("z_wall_test", "Presents a wall surface to touch (wall @ z = 150, force along positive z axis)")
				;
			m_progOptions.add(tests);
		}
	}
	bool parseOptions(int argc, char** argv)
	{

		if(!FalconCLIBase::parseOptions(m_falconDevice, argc, argv)) return false;
		int led = 0;
		if(m_varMap.count("led_red"))
		{
			std::cout << "Turning on RED LED" << std::endl;
			led |= FalconFirmware::RED_LED;
		}
		if(m_varMap.count("led_green"))
		{
			std::cout << "Turning on GREEN LED" << std::endl;
			led |= FalconFirmware::GREEN_LED;
		}
		if(m_varMap.count("led_blue"))
		{
			std::cout << "Turning on BLUE LED" << std::endl;			
			led |= FalconFirmware::BLUE_LED;
		}
		m_falconDevice.getFalconFirmware()->setLEDStatus(led);
		m_falconDevice.runIOLoop();
#ifndef _MINGW_
#ifndef WIN32
		if(m_varMap.count("loop_time_test"))
		{
			stop = false;
			while(!stop)
			{
				tstart();
				for(int i = 0; i < 1000 && !stop;)
				{
					if(m_falconDevice.runIOLoop()) ++i;					
				}
				if(stop) break;
				tend();
				std::cout << "Loop time (in seconds): " << tval() << std::endl;
			}
		}
		else
#endif
#endif

			if(m_varMap.count("cube_test"))
		{
			m_falconDevice.getFalconFirmware()->setHomingMode(true);
			m_falconDevice.setFalconKinematic<FalconKinematicStamper>();
            double cornerA[3] = { -.030, -.030, .095 };
            double cornerB[3] = { .030, .030, .155 };
            double force[3];
            
            // TODO: for stability, stiffness should be a function of
            // the sample rate.  Also, we can add damping.
            double stiffness = 1000;
			
			stop = false;
			bool homing = false;
			bool homing_reset = false;
			int count = 0;
			while(!stop)
			{
#ifndef _MINGW_
#ifndef WIN32
				if(!count) tstart();
#endif
#endif
                if(!m_falconDevice.runIOLoop()) continue;
				if(!m_falconDevice.getFalconFirmware()->isHomed())
				{
					if(!homing)
					{
						m_falconDevice.getFalconFirmware()->setLEDStatus(libnifalcon::FalconFirmware::RED_LED);
						std::cout << "Falcon not currently homed. Move control all the way out then push straight all the way in." << std::endl;
					}
					homing = true;
					continue;
				}
				if(homing)
				{
					m_falconDevice.getFalconFirmware()->setLEDStatus(libnifalcon::FalconFirmware::BLUE_LED);
					std::cout << "Falcon homed. Move control all the way in or out to start simulation." << std::endl;
					homing_reset = true;
					
				}
				homing = false;
                double *pos = m_falconDevice.getPosition();

				if(homing_reset)
				{
					if(pos[2] < cornerA[2] || pos[2] > cornerB[2])
					{
						m_falconDevice.getFalconFirmware()->setLEDStatus(libnifalcon::FalconFirmware::GREEN_LED);
						std::cout << "Starting cube simulation." << std::endl;
						homing_reset = false;
						//Reset our loop count to get proper timing
						count = 0;
					}
					continue;
				}

                double dist = 10000;
                int closest = -1, outside=3, axis;

                // For each axis, check if the end effector is inside
                // the cube.  Record the distance to the closest wall.

                for (axis=0; axis<3; axis++)
                {
                    force[axis] = 0;
                    if (pos[axis] > cornerA[axis] && pos[axis] < cornerB[axis])
                    {
                        double dA = pos[axis]-cornerA[axis];
                        double dB = pos[axis]-cornerB[axis];
                        if (fabs(dA) < fabs(dist)) { dist = dA; closest = axis; }
                        if (fabs(dB) < fabs(dist)) { dist = dB; closest = axis; }
                        outside--;
                    }
                }

                // If so, add a proportional force to kick it back
                // outside from the nearest wall.

                if (closest > -1 && !outside)
                    force[closest] = -stiffness*dist;
                m_falconDevice.setForce(force);
				++count;
#ifndef _MINGW_
#ifndef WIN32
				if(count == 1000)
				{
					tend();
					std::cout << "Loop time (in seconds): " << tval() << std::endl;
					count = 0;
				}
#endif
#endif
			}
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
