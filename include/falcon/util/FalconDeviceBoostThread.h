/***
 * @file FalconDeviceBoostThread.h
 * @brief Utility class for threading a FalconDevice instance using boost::thread (http://www.boost.org)
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.sourceforge.net/
 *
 */

#ifndef FALCONDEVICEBOOSTTHREADS_H
#define FALCONDEVICEBOOSTTHREADS_H
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "falcon/core/FalconDevice.h"

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
