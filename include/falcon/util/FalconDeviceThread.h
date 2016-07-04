/***
 * @file FalconDeviceThread.h
 * @brief Utility class for threading a FalconDevice instance using std::thread
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONDEVICETHREADS_H
#define FALCONDEVICETHREADS_H
#include <thread>
#include "falcon/core/FalconDevice.h"

namespace libnifalcon
{
/**
 * @class FalconDeviceThread
 * @ingroup UtilityClasses
 *
 * The FalconDeviceThread class is a sample device that uses the std::thread class to run the
 * communications loop to the falcon. 
 *
 * The FalconDeviceThread class is only available if the std::thread library is available on the system.
 */

	class FalconDeviceThread : public FalconDevice
	{
	public:
		/**
		 * Constructor
		 */
		FalconDeviceThread();

		/**
		 * Destructor
		 */
		virtual ~FalconDeviceThread();

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
		void getPosition(std::array<double, 3>& pos);
	protected:
		/**
		 * Wrapper function for dealing with device communication
		 */
		void runDeviceComm();

		/**
		 * Internal thread object
		 */
		std::unique_ptr<std::thread> m_ioThread;

		/**
		 * Internal position storage
		 */
		std::array<double, 3> m_localPosition;

		bool m_runThreadLoop; /**< Internal thread execution state. Thread loop exits if this is false. */
	};
}
#endif
