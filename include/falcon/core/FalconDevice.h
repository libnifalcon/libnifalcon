/***
 * @file FalconDevice.h
 * @brief FalconDevice class, glue class for falcon components
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCON_DEVICE_H
#define FALCON_DEVICE_H

#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include "falcon/core/FalconLogger.h"
#include "falcon/core/FalconCore.h"
#include "falcon/core/FalconComm.h"
#include "falcon/core/FalconFirmware.h"
#include "falcon/core/FalconKinematic.h"
#include "falcon/core/FalconGrip.h"

namespace libnifalcon
{
/**
 * @class FalconDevice
 * @ingroup CoreClasses
 *
 * FalconDevice is a conveinence class that ties together all of the components in libnifalcon to create
 * a simple, usable single object for accessing and controlling a novint falcon. It does this by using
 * a templated version of the pimpl idiom to fulfill the following behaviors:
 *
 * - Communication (set automatically in the constructor)
 * - Firmware
 * - Grip
 * - Kinematics
 *
 * Once these behaviors are established, FalconDevice can be used to get/set common parameters (end
 * effector position, force generation, LED status, button/grip status, etc...) without have to refer
 * to the specific behavior class.
 *
 * The most common use of libnifalcon will look something like
 *
 * - Create FalconDevice object
 * - Set behaviors
 * - Open device
 * - Start running the FalconDevice::runIOLoop function until falcon control is no longer needed
 * - Close device
 *
 * All of the above functions can be achieved through using the FalconDevice object.
 */

	class FalconDevice : public FalconCore
	{
	public:
		enum {
			FALCON_DEVICE_NO_COMM_SET = 1000, /**< Error for no communications policy set */
			FALCON_DEVICE_NO_FIRMWARE_SET, /**< Error for no firmware policy set */
			FALCON_DEVICE_NO_KINEMATIC_SET, /**< Error for no kinematics policy set */
			FALCON_DEVICE_NO_GRIP_SET, /**< Error for no grip policy set */
			FALCON_DEVICE_NO_FIRMWARE_LOADED, /**< Error for no firmware loaded */
			FALCON_DEVICE_FIRMWARE_NOT_VALID, /**< Error for firmware file missing */
			FALCON_DEVICE_FIRMWARE_CHECKSUM_MISMATCH /**< Error for checksum mismatch during firmware loading */
		};

		enum {
			FALCON_LOOP_FIRMWARE = 0x1, /**< runIOLoop should run firmware update (device read/write) */
			FALCON_LOOP_KINEMATIC = 0x2, /**< runIOLoop should run kinematic update (end effector position and force calculation) */
			FALCON_LOOP_GRIP = 0x4  /**< runIOLoop should run grip information update */
		};

		/**
		 * Constructor
		 *
		 * Sets the falcon communications device to the default for the system (FTD2XX for windows, libusb-1.0 for anything else)
		 */
		FalconDevice();

		/**
		 * Destructor
		 *
		 */
		~FalconDevice();

		/**
		 * Checks to see if firmware is loaded by running IO loop 10 times, returning true on first success
		 * Will automatically return false is setFalconFirmware() has not been called.
		 *
		 * @return true if firmware is loaded, false otherwise
		 */
		bool isFirmwareLoaded();

		/**
		 * Sets the firmware file to load to the falcon
		 *
		 * @param filename Name of the file to use for firmware
		 *
		 * @return true if file exists and is openable, false otherwise
		 */
		bool setFirmwareFile(const std::string& filename);

		/**
		 * Conveinence function, calls loadFirmware with a certain number of retries
		 *
		 * @param retries Number of times to retry loading firmware before quitting
		 * @param skip_checksum Whether or not to skip checksum tests when loading firmware (useful with ftd2xx on non-windows platforms)
		 *
		 * @return true if firmware is loaded successfully, false otherwise
		 */
		bool loadFirmware(unsigned int retries, bool skip_checksum = false);

		/**
		 * Tries to load the firmware that was specified by the setFirmwareFile function. For the moment, skip_checksum should be used
		 * on non-windows platforms when using the ftd2xx communications core, as there is a bug that causes bad checksum returns on
		 * otherwise proper firmware loading events
		 *
		 * @param skip_checksum Whether or not to skip checksum tests when loading firmware (useful with ftd2xx on non-windows platforms)
		 *
		 * @return true if firmware is loaded successfully, false otherwise
		 */
		bool loadFirmware(bool skip_checksum);

		/**
		 * Returns the number of falcons currectly connected to the system
		 *
		 * @param count Reference to return to
		 *
		 * @return true if count is successfully returned, false otherwise
		 */
		bool getDeviceCount(unsigned int& count);

		/**
		 * Opens the falcon at the specified index
		 *
		 * @param index Index of falcon to open (starts at 0)
		 *
		 * @return true if falcon opened successfully, false otherwise
		 */
		bool open(unsigned int index);

		/**
		 * Closes the falcon, if open
		 *
		 */
		void close();

		/**
		 * Runs one iteration of the IO Loop, with the following logic
		 * - If firmware not set, return false
		 * - Run firmware IO Loop, return false if fails
		 * - If falcon is homed and kinematic behavior is set, Run kinematic update, return false if fails
		 * - If grip behavior is set, run grip update, return false if fails
		 *
		 * @return true on success, false otherwise
		 */
		bool runIOLoop(unsigned int exe_flags = (FALCON_LOOP_FIRMWARE | FALCON_LOOP_KINEMATIC | FALCON_LOOP_GRIP));

		/**
		 * Set communications behavior type, and create a new internal object from it.
		 * Also passes new comm object to firmware behavior, if it exists.
		 *
		 * Template should be a subclass of FalconComm
		 */
		template<class T>
		void setFalconComm();

		/**
		 * Set firmware behavior type, and create a new internal object from it.
		 * Also passes communications behavior to firmware, if it exists.
		 *
		 * Template should be a subclass of FalconFirmware
		 */
		template<class T>
		void setFalconFirmware();

		/**
		 * Set grip behavior, and create a new internal object from it.
		 *
		 * Template should be a subclass of FalconGrip
		 */
		template<class T>
		void setFalconGrip();

		/**
		 * Set communications behavior, and create a new internal object from it.
		 *
		 * Template should be a subclass of FalconKinematic
		 */
		template<class T>
		void setFalconKinematic();

		/**
		 * Return the position given by the kinematic behavior.
		 *
		 * @return Array of 3 doubles, representing 3D cartesian coordinate
		 */
		boost::array<double, 3> getPosition() { return m_position; }

		/**
		 * Set the instantanious force for the next I/O loop
		 *
		 * @param force Force vector, in cartesian coordinates (x,y,z)
		 */
		void setForce(boost::array<double, 3> force)
		{
			m_forceVec[0] = force[0];
			m_forceVec[1] = force[1];
			m_forceVec[2] = force[2];
		}

		/**
		 * Get communication behavior object pointer
		 *
		 * @return Non-smart pointer to internal falcon communications object
		 */
		boost::shared_ptr<FalconComm> getFalconComm() { return m_falconComm; }

		/**
		 * Get firmware behavior object pointer
		 *
		 * @return Non-smart pointer to internal falcon firmware object
		 */
		boost::shared_ptr<FalconFirmware> getFalconFirmware() { return m_falconFirmware; }

		/**
		 * Get grip behavior object pointer
		 *
		 * @return Non-smart pointer to internal falcon grip object
		 */
		boost::shared_ptr<FalconGrip> getFalconGrip() { return m_falconGrip; }

		/**
		 * Get kinematic behavior object pointer
		 *
		 * @return Non-smart pointer to internal falcon kinematic object
		 */
		boost::shared_ptr<FalconKinematic> getFalconKinematic() { return m_falconKinematic; }

		/**
		 * Checks whether the falcon communications are open
		 *
		 * @return True if comm component is set and open, false otherwise
		 */
		bool isOpen()
		{
			if(m_falconComm != NULL)
			{
				return m_falconComm->isCommOpen();
			}
			return false;
		}

		/**
		 * Get the number of total errors generated by the I/O loop
		 *
		 * @return Number of errors generated by the I/O loop since device creation
		 */
		unsigned int getErrorCount() { return m_errorCount; }
	protected:
		unsigned int m_errorCount;	/**< Number of errors in I/O loops */
		boost::shared_ptr<FalconComm> m_falconComm; /**< Falcon communication object */
		boost::shared_ptr<FalconKinematic> m_falconKinematic; /**<  Falcon kinematics object */
		boost::shared_ptr<FalconFirmware> m_falconFirmware; /**<  Falcon firmware object */
		boost::shared_ptr<FalconGrip> m_falconGrip; /**< Falcon grip object */
		boost::array<double, 3> m_position;	/**< Current position in 3D cartesian coordinates */
		boost::array<double, 3> m_forceVec;	/**< Current force in 3D cartesian coordinates */
	private:
		DECLARE_LOGGER();
	};

	template<class T>
	void FalconDevice::setFalconComm()
	{
		m_falconComm.reset(new T());
		if(m_falconFirmware != NULL)
		{
			m_falconFirmware->setFalconComm(m_falconComm);
		}
	}

	template<class T>
	void FalconDevice::setFalconFirmware()
	{
		m_falconFirmware.reset(new T());
		if(m_falconComm != NULL)
		{
			m_falconFirmware->setFalconComm(m_falconComm);
		}
	}

	template<class T>
	void FalconDevice::setFalconGrip()
	{
		m_falconGrip.reset(new T());
	}

	template<class T>
	void FalconDevice::setFalconKinematic()
	{
		m_falconKinematic.reset(new T());
	}

}

#endif
