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
	
	void FalconDeviceBoostThread::getPosition(double* pos)
	{
		m_threadMutex.lock();
		pos[0] = m_position[0];
		pos[1] = m_position[1];
		pos[2] = m_position[2];
		m_threadMutex.unlock();
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
			m_threadMutex.lock();
			runIOLoop();			
			m_threadMutex.unlock();
		}
	}

	void FalconDeviceBoostThread::stopThread()
	{
		m_runThreadLoop = false;
	}
}
