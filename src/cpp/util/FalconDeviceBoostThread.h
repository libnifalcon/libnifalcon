#ifndef FALCONDEVICEBOOSTTHREADS_H
#define FALCONDEVICEBOOSTTHREADS_H
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "core/FalconDevice.h"

namespace libnifalcon
{
	class FalconDeviceBoostThread : public FalconDevice
	{
	public:
		FalconDeviceBoostThread();
		virtual ~FalconDeviceBoostThread();
		void getPosition(double pos[3]);		
		void startThread();
		virtual void runThreadLoop();
		void stopThread();
		bool isThreadRunning() { return m_runThreadLoop; }
	protected:
		void runDeviceComm();
		boost::thread* m_ioThread;
		double m_localPosition[3];
		bool m_runThreadLoop;
	};
}
#endif
