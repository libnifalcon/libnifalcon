/***
 * @file FalconDeviceThread.cpp
 * @brief Utility class for threading a FalconDevice instance using std::thread
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include "falcon/util/FalconDeviceThread.h"
#include <iostream>

namespace libnifalcon
{

	FalconDeviceThread::FalconDeviceThread() :
		m_ioThread(nullptr),
		m_runThreadLoop(false)
	{
	}

	FalconDeviceThread::~FalconDeviceThread()
	{
    stopThread();
	}

	void FalconDeviceThread::getPosition(std::array<double, 3>& pos)
	{
		pos[0] = m_position[0];
		pos[1] = m_position[1];
		pos[2] = m_position[2];
	}

	void FalconDeviceThread::startThread()
	{
		if(!m_runThreadLoop)
		{
			m_runThreadLoop = true;
			m_ioThread = std::unique_ptr<std::thread>(new std::thread(&libnifalcon::FalconDeviceThread::runThreadLoop, this));
		}
	}

	void FalconDeviceThread::runThreadLoop()
	{
		while(m_runThreadLoop)
		{
			runIOLoop();
		}
	}

	void FalconDeviceThread::stopThread()
	{
		m_runThreadLoop = false;
    if(m_ioThread)
      m_ioThread->join();
	}
}
