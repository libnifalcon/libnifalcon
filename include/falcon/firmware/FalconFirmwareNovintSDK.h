/***
 * @file FalconFirmwareNovintSDK.h
 * @brief Firmware communications for the firmware included with the Novint SDK/Official Drivers
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.sourceforge.net/
 *
 */

#ifndef FALCONNOVINTFIRMWARE_H
#define FALCONNOVINTFIRMWARE_H

#include "falcon/core/FalconFirmware.h"
#include "falcon/core/FalconLogger.h"

namespace libnifalcon
{
	class FalconFirmwareNovintSDK : public FalconFirmware
	{
	public:
		/**
		 * Constructor
		 *
		 *
		 */
		FalconFirmwareNovintSDK();

		/**
		 * Destructor
		 *
		 *
		 */
		virtual ~FalconFirmwareNovintSDK();

		/**
		 * Get the raw data string returned from the falcon
		 *
		 * @return std::string of last full packet received from the falcon
		 */
		virtual std::string getRawReturn();

		/**
		 * Runs device polling, then tries to write next command to device, or read return from device if write has already happened.
		 *
		 * @return True if we've read something, false otherwise. Sets error and returns false on communications error.
		 */		
		bool runIOLoop();

		/**
		 * Returns size of the grip data portion of the message. Currently always 1.
		 *
		 * @return Size of the grip data portion of the message. Currently always 1.
		 */		
		int32_t getGripInfoSize() { return 1; }
		
		/**
		 * Accessor for raw grip info
		 *
		 * @return Const pointer to internal representation of raw grip info
		 */		
		const uint8_t* getGripInfo() { return &(m_gripInfo); }
	protected:

		/**
		 * Formats current input (forces, LED commands, etc...) for sending to falcon.
		 *
		 */		
		void formatInput();

		/**
		 * Formats current output from falcon (joint positions, calibration, etc...)
		 *
		 * @return True if at least one complete packet has been parsed
		 */		
		bool formatOutput();
		
		uint8_t m_gripInfo; /**< Internal representation of grip data (buttons pressed, etc...) */
		uint8_t m_rawData[128]; /**< Raw buffer to read into */
		uint8_t m_rawInput[17]; /**< Raw buffer for formatting input. Plus one character to make it zero terminated */
		uint8_t m_rawOutput[17]; /**< Raw buffer for last full output packet. Plus one character to make it zero terminated */
		uint8_t m_rawOutputInternal[17]; /**< Raw buffer for formatting output incrementally. Plus one character to make it zero terminated */

		unsigned int m_currentOutputIndex; /**< How far the firmware object is into parsing the current packet */
		unsigned int m_rawDataSize; /**< Amount of data last returned from communications object read */
	private:
		DECLARE_LOGGER();

	};
}

#endif
