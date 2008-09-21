/***
 * @file FalconDeviceBoostThread.cpp
 * @brief Utility class for threading a FalconDevice instance using boost::thread (http://www.boost.org)
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
#include "FalconDeviceBoostThread.h"

#include <boost/bind.hpp>
#include <iostream>
namespace libnifalcon
{

	FalconDeviceBoostThread::FalconDeviceBoostThread() : m_runThreadLoop(false)
	{		
	}
	
	FalconDeviceBoostThread::~FalconDeviceBoostThread()
	{
	}
	
	void FalconDeviceBoostThread::getPosition(double pos[3])
	{
		pos[0] = m_position[0];
		pos[1] = m_position[1];
		pos[2] = m_position[2];
	}
	
	void FalconDeviceBoostThread::startThread()
	{
		if(!m_runThreadLoop)
		{
			m_runThreadLoop = true;
			m_ioThread = new boost::thread(boost::bind(&libnifalcon::FalconDeviceBoostThread::runThreadLoop, this));
		}
	}

	void FalconDeviceBoostThread::runThreadLoop()
	{		
		while(m_runThreadLoop)
		{
			runIOLoop();
		}
	}
	
	void FalconDeviceBoostThread::stopThread()
	{
		m_runThreadLoop = false;
	}
}
