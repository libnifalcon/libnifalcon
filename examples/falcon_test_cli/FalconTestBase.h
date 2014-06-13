#ifndef FALCONFUNCTION_H
#define FALCONFUNCTION_H

#ifdef WIN32
#ifdef __MINGW32__
#define FTC_USE_TIME 1
#endif
#else
#define FTC_USE_TIME 1
#endif

#ifdef FTC_USE_TIME
#include "sys/time.h"
#else
#include <windows.h>
#endif

#include "falcon/core/FalconDevice.h"

class FalconTestBase
{
public:

	FalconTestBase(std::shared_ptr<libnifalcon::FalconDevice> d) :
		m_lastLoopCount(0),
		m_countLimit(0),
		m_falconDevice(d)
	{
		tinit();
		tstart();
	}
	void run();
	void setPrintOnCount(uint64_t count)
	{
		m_countLimit = count;
	}
protected:
	uint64_t m_lastLoopCount;
	uint64_t m_countLimit;
	std::shared_ptr<libnifalcon::FalconDevice> m_falconDevice;
	virtual void runFunction() = 0;

#ifdef FTC_USE_TIME
	struct timeval m_tstart, m_tend;
	struct timezone m_tz;
#else
	LARGE_INTEGER m_tstart, m_tend, m_tfreq;
#endif

	void tinit()
	{
#ifndef FTC_USE_TIME
		QueryPerformanceFrequency(&m_tfreq);
#endif
	}

	void tstart()
	{
#ifdef FTC_USE_TIME
		gettimeofday(&m_tstart, &m_tz);
#else
		QueryPerformanceCounter(&m_tstart);
#endif
	}

	void tend()
	{
#ifdef FTC_USE_TIME
		gettimeofday(&m_tend,&m_tz);
#else
		QueryPerformanceCounter(&m_tend);
#endif
	}

	double tval()
	{
#ifdef FTC_USE_TIME
		double t1, t2;
		t1 =  (double)m_tstart.tv_sec + (double)m_tstart.tv_usec/(1000*1000);
		t2 =  (double)m_tend.tv_sec + (double)m_tend.tv_usec/(1000*1000);
		return t2-t1;
#else
		return ((m_tend.QuadPart - m_tstart.QuadPart) / (m_tfreq.QuadPart));
#endif
	}
};

#endif
