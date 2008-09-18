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
		~FalconDeviceBoostThread();
		void getPosition(double* pos);
		void startThread();
		void runThreadLoop();
		void stopThread();
	protected:
		boost::thread* m_ioThread;
		boost::mutex m_threadMutex;
		bool m_runThreadLoop;
	};
}
#endif
