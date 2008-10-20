/***
 * @file FalconDevice.h
 * @brief FalconDevice class, glue class for falcon components
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

#ifndef FALCON_DEVICE_H
#define FALCON_DEVICE_H

#include <string>
#include <fstream>
#include <iostream>
#include "FalconCore.h"
#include "FalconComm.h"
#include "FalconFirmware.h"
#include "FalconKinematic.h"
#include "FalconGrip.h"

namespace libnifalcon
{
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
			FALCON_LOOP_FIRMWARE = 0x1,
			FALCON_LOOP_KINEMATIC = 0x2,
			FALCON_LOOP_GRIP = 0x4
		};
		
		/** 
		 * Constructor
		 *
		 */

		FalconDevice();
		/** 
		 * Destructor
		 *
		 * if m_cleanupObjects is set to true, this will also delete all member pointers to falcon
		 * behavior objects (comm, firmware, grips, kinematics)
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
		bool setFirmwareFile(std::string filename);
		/** 
		 * Conveinence function, calls loadFirmware with a certain number of retries
		 *
		 * @param retries Number of times to retry loading firmware before quitting
		 * @param skip_checksum Whether or not to skip checksum tests when loading firmware (useful with ftd2xx on non-windows platforms)
		 *
		 * @return true if firmware is loaded successfully, false otherwise
		 */		
		bool loadFirmware(int retries, bool skip_checksum = false);
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
		bool getDeviceCount(int8_t& count);
		/** 
		 * Opens the falcon at the specified index
		 *
		 * @param index Index of falcon to open (starts at 0)
		 *
		 * @return true if falcon opened successfully, false otherwise
		 */				
		bool open(uint8_t index);
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
		bool runIOLoop(uint8_t exe_flags = (FALCON_LOOP_FIRMWARE | FALCON_LOOP_KINEMATIC | FALCON_LOOP_GRIP));

		/**
		 * Set whether the FalconDevice object should delete its behavior objects on destruction
		 *
		 * @param cleanup_objects true to delete pointers on destruction
		 */	
		void setCleanupObjects(bool cleanup_objects) { m_cleanupObjects = cleanup_objects; }

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
		double* getPosition() { return m_position; }
		/**
		 * Set the instantanious force for the next I/O loop
		 *
		 * @param force Force vector, in cartesian coordinates (x,y,z)
		 */					
		void setForce(double force[3])
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
		FalconComm* getFalconComm() { return m_falconComm; }
		/**
		 * Get firmware behavior object pointer
		 *
		 * @return Non-smart pointer to internal falcon firmware object
		 */					
		FalconFirmware* getFalconFirmware() { return m_falconFirmware; }
		/**
		 * Get grip behavior object pointer
		 *
		 * @return Non-smart pointer to internal falcon grip object
		 */					
		FalconGrip* getFalconGrip() { return m_falconGrip; }
		/**
		 * Get kinematic behavior object pointer
		 *
		 * @return Non-smart pointer to internal falcon kinematic object
		 */					
		FalconKinematic* getFalconKinematic() { return m_falconKinematic; }
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
		 * Deletes the interal pointer to the falcon communications object
		 *
		 */								
		void removeFalconComm()
		{
			if(m_falconComm != NULL)
			{
				delete m_falconComm;
				m_falconComm = NULL;
			}			
		}
		/**
		 * Deletes the interal pointer to the falcon firmware object
		 *
		 */								
		void removeFalconFirmware()
		{
			if(m_falconFirmware != NULL)
			{
				delete m_falconFirmware;
				m_falconFirmware = NULL;
			}
		}
		/**
		 * Deletes the interal pointer to the falcon grip object
		 *
		 */								
		void removeFalconGrip()
		{
			if(m_falconGrip != NULL)
			{
				delete m_falconGrip;
				m_falconGrip = NULL;
			}
		}
		/**
		 * Deletes the interal pointer to the falcon grip object
		 *
		 */								
		void removeFalconKinematic()
		{
			if(m_falconKinematic != NULL)
			{
				delete m_falconKinematic;
				m_falconKinematic = NULL;
			}
		}
		/**
		 * Get the number of total errors generated by the I/O loop
		 *
		 * @return Number of errors generated by the I/O loop since device creation
		 */								
		uint32_t getErrorCount() { return m_errorCount; }
	protected:
		bool m_isFirmwareLoaded; /**< True if firmware has been loaded, false otherwise */
		bool m_cleanupObjects;	/**< If true, object should clean up objects on destruction */
		uint32_t m_errorCount;	/**< Number of errors in I/O loops */
		std::string m_firmwareFilename; /**< Filename of the firmware to load */
		FalconComm* m_falconComm; /**< Falcon communication object */
		FalconKinematic* m_falconKinematic; /**<  Falcon kinematics object */
		FalconFirmware* m_falconFirmware; /**<  Falcon firmware object */
		FalconGrip* m_falconGrip; /**< Falcon grip object */
		double m_position[3];	/**< Current position in 3D cartesian coordinates */
		double m_forceVec[3];	/**< Current force in 3D cartesian coordinates */
	};

	template<class T>
	void FalconDevice::setFalconComm()
	{
		removeFalconComm();
		m_falconComm = new T();
		if(m_falconFirmware != NULL)
		{
			m_falconFirmware->setFalconComm(m_falconComm);
		}		
	}

	template<class T>
	void FalconDevice::setFalconFirmware()
	{
		removeFalconFirmware();
		m_falconFirmware = new T();
		if(m_falconComm != NULL)
		{
			m_falconFirmware->setFalconComm(m_falconComm);
		}		
	}

	template<class T>
	void FalconDevice::setFalconGrip()
	{
		removeFalconGrip();
		m_falconGrip = new T();
	}

	template<class T>
	void FalconDevice::setFalconKinematic()
	{
		removeFalconKinematic();
		m_falconKinematic = new T();
	}
	
}



#endif
