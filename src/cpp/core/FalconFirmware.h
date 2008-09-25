/***
 * @file FalconFirmware.h
 * @brief Base class for firmware policy classes
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

#ifndef FALCONFIRMWARE_H
#define FALCONFIRMWARE_H

#include <stdint.h>
#include <cstdlib>
#include "FalconComm.h"

namespace libnifalcon
{
	class FalconFirmware : public FalconCore
	{	
	public:
		enum
		{
			GREEN_LED=0x2,		/**< Flag to control Green LED */
			BLUE_LED=0x4,		/**< Flag to control Blue LED */
			RED_LED=0x8			/**< Flag to control Red LED  */
		};

		enum
		{
			ENCODER_1_HOMED = 0x1, /**< Flag for encoder homing for motor 1 */
			ENCODER_2_HOMED = 0x2, /**< Flag for encoder homing for motor 2 */
			ENCODER_3_HOMED = 0x4, /**< Flag for encoder homing for motor 3 */
		};

		/** 
		 * Constructor
		 * 
		 * 
		 */
		FalconFirmware() :
			m_falconComm(NULL),
			m_homingMode(false)
		{
			//Who needs loops!
			m_forceValues[0] = 0;
			m_forceValues[1] = 0;
			m_forceValues[2] = 0;
			m_encoderValues[0] = 0;
			m_encoderValues[1] = 0;
			m_encoderValues[2] = 0;			
		}

		/** 
		 * Destructor
		 * 
		 * 
		 */
		virtual ~FalconFirmware()
		{
			//Don't do anything with the falcon communications
			//Assume that's managed elsewhere
		}
		/** 
		 * Run one read/write loop 
		 * 
		 * 
		 * @return True if read and write are successful, false otherwise
		 */
		virtual bool runIOLoop() = 0;
		/** 
		 * Returns the size of grip information for this firmware.		 
		 *
		 * This may or may not work. However, there's currently only one grip type, so I'm guessing.
		 * 
		 * @return Size of the buffer that getGripInfo will return
		 */
		virtual int32_t getGripInfoSize() = 0;
		/** 
		 * Retrieves the buffer containing grip data
		 * 
		 * 
		 * @return Byte buffer with grip data
		 */
		virtual u_int8_t* getGripInfo() = 0;

		/** 
		 * Sets the instantious force (in whatever units the falcon takes) for the next I/O loop
		 * 
		 * @param force Array of signed 16-bit integers to set force
		 */
		void setForces(int16_t force[3])
		{
			m_forceValues[0] = force[0];
			m_forceValues[1] = force[1];
			m_forceValues[2] = force[2];
		}
		/** 
		 * Returns an array of current motor encoder values from the last I/O loop
		 * 
		 * 
		 * @return Signed 16-bit integers representing the current encoder values for each motor
		 */
		int16_t* getEncoderValues() { return m_encoderValues; }

		/** 
		 * Sets the LEDs to turn on or off at the next I/O loop
		 * 
		 * @param leds Bitfield of the LED flags
		 */
		void setLEDStatus(u_int8_t leds) { m_ledStatus = leds; }
		/** 
		 * Returns the current LED status bitfield
		 * 
		 * 
		 * @return The current LED status bitfield
		 */
		u_int8_t getLEDStatus() { return m_ledStatus; }
		/** 
		 * Sets the homing mode for the next I/O loop
		 * 
		 * @param value True to turn homing mode on, false for off
		 */
		void setHomingMode(bool value) { m_homingMode = value; }
		/** 
		 * Return the homing status of each encoder
		 * 
		 * 
		 * @return Bitfield of encoder homing statuses
		 */
		u_int8_t getHomingModeStatus() { return m_homingStatus; }
		/** 
		 * Returns the overall homing status
		 *
		 * Basically, if only one encoder is homed, it doesn't do you a lot of good. This is a conveinence function to signify
		 * whether all motors are homed
		 * 
		 * @return True if all encoders are homed, false otherwise
		 */
		bool isHomed() { return ( m_homingStatus & (ENCODER_1_HOMED | ENCODER_2_HOMED | ENCODER_3_HOMED)); }
		/** 
		 * Sets the communication object for the firmware to run I/O through
		 * 
		 * @param f Pointer to the communications object
		 */
		void setFalconComm(FalconComm* f) { m_falconComm = f; }
	protected:
		FalconComm* m_falconComm; /**< Communications object for I/O */

		//Values sent to falcon
		bool m_homingMode;		/**< True if homing mode is on, false for homing mode off */
		u_int8_t m_ledStatus;	/**< Bitfield for LED Status */
		int16_t m_forceValues[3]; /**< Force values for the next I/O loop */

		//Values received from falcon
		int16_t m_encoderValues[3];	/**< Encoder values from the last I/O loop */
		u_int8_t m_homingStatus; /**< Current homing status from the last I/O loop */
	};
}
#endif
