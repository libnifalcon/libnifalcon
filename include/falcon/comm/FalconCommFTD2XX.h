/***
 * @file FalconCommFTD2XX.h
 * @brief FTD2XX (http://www.ftdichip.com) based implementation of FTDI communication for the falcon
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONFTD2XXCOMM_H
#define FALCONFTD2XXCOMM_H

#include "falcon/core/FalconComm.h"

namespace libnifalcon
{
/**
 * @class FalconCommFTD2XX
 * @ingroup CommClasses
 *
 * FalconCommFTD2XX is the FTD2XX implementation of the falcon communications core. This core is meant to
 * be used on Windows, where the FTD2XX driver is the factory standard driver for the falcon. This means
 * that libnifalcon code can work alongside the regular HDAL drivers.
 *
 * FalconCommFTD2XX is built directly into the libnifalcon core library, as is chosen for the user
 * by default by the FalconDevice constructor, so it is usually not needed.
 * However, it is left here for code compatibility for code that already used comm behavior setting, which
 * was required before libnifalcon v1.0
 */
	class FalconCommFTD2XX : public FalconComm
	{
	public:
		/**
		 * Constructor
		 *
		 *
		 */
		FalconCommFTD2XX() { }

		/**
		 * Destructor
		 *
		 *
		 */
		virtual ~FalconCommFTD2XX();

		/**
		 * Returns the number of devices connected to the system
		 *
		 * @param[out] count The number of devices available
		 *
		 * @return True if count was retreived correctly, false otherwise. Error code set if false.
		 */
		virtual bool getDeviceCount(unsigned int& count);

		/**
		 * Polls the object for confirmation of write/read return
		 *
		 *
		 */		
		virtual void poll();

		/**
		 * Opens the device at the specified index
		 *
		 * @param[in] index Index of the device to open
		 *
		 * @return True if device is opened successfully, false otherwise. Error code set if false.
		 */
		virtual bool open(unsigned int index);

		/**
		 * Closes the device, if open
		 *
		 *
		 * @return True if device is closed successfully, false otherwise. Error code set if false.
		 */		
		virtual bool close();

		/**
		 * Read a specified number of bytes from the device
		 *
		 * @param[out] str Buffer to read data into
		 * @param[in] size Amount of bytes to read
		 *
		 * @return True if (size) amount of bytes is read successfully, false otherwise. Error code set if false.
		 */		
		virtual bool read(uint8_t* str, unsigned int size);

		/**
		 * Write a specified number of bytes to the device
		 *
		 * @param[in] str Buffer to write data from
		 * @param[in] size Amount of bytes to write
		 *
		 * @return True if (size) amount of bytes is written successfully, false otherwise. Error code set if false.
		 */
		virtual bool write(uint8_t* str, unsigned int size);

		/**
		 * Read a specified number of bytes from the device
		 *
		 * @param[out] str Buffer to read data into
		 * @param[in] size Amount of bytes to read
		 *
		 * @return True if (size) amount of bytes is read successfully, false otherwise. Error code set if false.
		 */
		virtual bool readBlocking(uint8_t* str, unsigned int size); //{ return read(size, buffer); }

		/**
		 * Write a specified number of bytes to the device
		 *
		 * @param[in] str Buffer to write data from
		 * @param[in] size Amount of bytes to write
		 *
		 * @return True if (size) amount of bytes is written successfully, false otherwise. Error code set if false.
		 */
		virtual bool writeBlocking(uint8_t* str, unsigned int size) { return write(str, size); }

		/**
		 * Sets the communications mode and initializes the device to load firmware
		 *
		 *
		 * @return True if device is successfully set to load firwmare, false otherwise. Error code set if false.
		 */
		virtual bool setFirmwareMode();
		
		/**
		 * Sets the communications mode and initializes the device to run in normal operation
		 *
		 *
		 * @return True if device is successfully set to normal operation, false otherwise. Error code set if false.
		 */
		virtual bool setNormalMode();
	protected:
		/**
		 * String describing the falcon (used for identification)
		 */
		const static char* FALCON_DESCRIPTION; 
		/**
		 * Internal device opening function
		 *
		 * @param[in] index Index of device to open
		 * @param[in] should_open If true, open. If false, return count
		 *
		 * @return If should_open is true, non-zero if device opened. If should_open is false, device count or -1 if can't get count.
		 */
		int8_t openDeviceFTD2XX(unsigned int index, bool should_open);
		/**
		 * Internal pointer to ftdi device struct
		 *
		 * This would usually be a FT_HANDLE, but FT_HANDLE
		 * is just a void*, so this saves us having to deal
		 * with the include at this level.
		 */
		void* m_falconDevice;
	};
};

#endif
