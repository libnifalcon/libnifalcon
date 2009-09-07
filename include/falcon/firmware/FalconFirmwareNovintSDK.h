/***
 * @file FalconFirmwareNovintSDK.h
 * @brief Firmware communications for the firmware included with the Novint SDK/Official Drivers
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONNOVINTFIRMWARE_H
#define FALCONNOVINTFIRMWARE_H

#include "falcon/core/FalconFirmware.h"
#include "falcon/core/FalconLogger.h"

namespace libnifalcon
{
/**
 * @class FalconFirmwareNovintSDK
 * @ingroup FirmwareClasses
 *
 * FalconFirmwareNovintSDK implements the firmware communications strategy for the firmware
 * that comes with the Novint drivers, and with the nVent software.
 *
 * @section FirmwareIntro Intro to Novint Firmware Format
 *
 * 16 bytes sent to the Falcon causes 16 bytes to be returned. It's expected that the host will
 * be constantly polling the falcon to set motor positions, as it is assumed that the PID loops
 * for the control exist in the software, not the hardware.
 *
 * Input and output strings are printable. Each string starts with 0x3c ('<') and ends with
 * 0x3e ('>'), and the byte range is 0x41 ('A') to 0x50 ('P').
 * 
 * A null input/output string looks like:
 *
 * <AAAAAAAAAAAAAA>
 *
 * However, after any amount of usage, the input/output string will probably look something like:
 *
 * <HNGPOKOPFPOPAA>
 *
 * @section FirmwareInputLayout Input Layout (To Controller)
 * 
 * 0x3C 0x(g0) 0x(g1) 0x(g2) 0x(g3) 0x(h0) 0x(h1) 0x(h2) 0x(h3) 0x(i0) 0x(i1) 0x(i2) 0x(i3) 0x(j0) 0x(k0) 0x3E
 *
 * Input Packet Byte Definitions:
 * - 0x3C - Start Byte
 * - g thru i - Instantanious Motor Torque for the 3 feedback motors
 * - j - Homing and LED Control
 * - k - Unknown
 * - 0x3E - End Byte
 * 
 * Homing and LED Controls (Value of j0 byte):
 * - 0x02 - Red LED On
 * - 0x04 - Green LED On
 * - 0x08 - Blue LED On
 * - 0x10 - Homing Mode On
 * 
 * @section FirmwareOutputLayout Output Layout (To Controller)
 * 
 * 0x3C 0x(m0) 0x(m1) 0x(m2) 0x(m3) 0x(n0) 0x(n1) 0x(n2) 0x(n3) 0x(o0) 0x(o1) 0x(o2) 0x(o3) 0x4(p0) 0x(q0) 0x3E
 * 
 * Output packet byte definitions:
 * - 0x3C - Start Byte
 * - m thru o - Axis Position Report
 * - p - Buttons and Homing Control Report
 * - q - Unknown
 * - 0x3E - End Byte
 * 
 * Buttons and Homing Control Report (Value of j0 byte) - Directions taken from looking
 * at the Falcon from the front:
 * - 0x01 - Far Right Button Down
 * - 0x02 - Center (Forward) Button Down
 * - 0x04 - Center (Circle) Button Down
 * - 0x08 - Far Left Button Down
 * - 0x10 - (If homing mode on) Top Encoder Homed
 * - 0x20 - (If homing mode on) Right Encoder Homed
 * - 0x40 - (If homing mode on) Left Encoder Homed
 *
 * @section FirmwareOutputLayout Motor Value Extraction Example
 * 
 * Retrieving/Setting Axis Position:
 * - Retrieving: Subtract 0x41 for all bytes of the 32-bit integer representation.
 * - Setting: Add 0x41.
 * 
 * Example:
 * 
 * -# Test GUI gives us 1447 for first encoder, so that's the number we're trying to get to in the end.
 * -# Driver gives us (positions represented by .s can be ignored): <JFBA..........>
 * -# Translate to ascii values: 0x3c 0x4A 0x46 0x4B 0x50
 * -# Drop first byte since it's just a marker: 0x4A 0x46 0x4B 0x50
 * -# Subtract 0x41 (ASCII value for A) from all bytes: 0x9 0x5 0xA 0xF
 * -# //(For readability only, not in the drivers)// Reverse into Big Endian: 0xF 0xA 0x5 0x9
 * -# Shift into 16 bit number: 0xFA59
 * -# Twos compliment conversion (~x+1): 0x05A7
 * -# Decimal conversion (16 bit signed): 1447
 * 
 */
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
