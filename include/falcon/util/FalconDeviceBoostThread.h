/***
 * @file FalconDeviceBoostThread.h
 * @brief Utility class for threading a FalconDevice instance using boost::thread (http://www.boost.org)
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONDEVICEBOOSTTHREADS_H
#define FALCONDEVICEBOOSTTHREADS_H
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "falcon/core/FalconDevice.h"

namespace libnifalcon
{
/**
 * @class FalconDeviceBoostThread
 * @ingroup UtilityClasses
 *
 * The FalconDeviceBoostThread class is a sample device that uses the boost::thread class to run the
 * communications loop to the falcon. 
 *
 * The FalconDeviceBoostThread class is only available if the boost::thread library is available on the system.
 */

	class FalconDeviceBoostThread : public FalconDevice
	{
	public:
		/**
		 * Constructor
		 */
		FalconDeviceBoostThread();

		/**
		 * Destructor
		 */
		virtual ~FalconDeviceBoostThread();

		/**
		 * Starts a thread that runs FalconDevice::runIOLoop constantly
		 */		
		void startThread();

		/**
		 * Runs IO loop. Overridden to implement application specific functionality
		 */		
		virtual void runThreadLoop();

		/**
		 * Stops thread if running
		 */
		void stopThread();

		/**
		 * Thread run status
		 *
		 * @return True if running, false otherwise
		 */
		bool isThreadRunning() { return m_runThreadLoop; }
		
		/**
		 * Thread safe position return
		 */
		void getPosition(boost::array<double, 3>& pos);
	protected:
		/**
		 * Wrapper function for dealing with device communication
		 */
		void runDeviceComm();

		/**
		 * Internal thread object
		 */
		boost::shared_ptr<boost::thread> m_ioThread;

		/**
		 * Internal position storage
		 */
		boost::array<double, 3> m_localPosition;

		bool m_runThreadLoop; /**< Internal thread execution state. Thread loop exits if this is false. */
	};
}
#endif
