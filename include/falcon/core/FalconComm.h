/***
 * @file FalconComm.h
 * @brief Base class for falcon communications policy classes
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONCOMMBASE_H
#define FALCONCOMMBASE_H

#include <stdint.h>
#include "falcon/core/FalconCore.h"

namespace libnifalcon
{

/**
 * @class FalconComm
 * @ingroup CoreClasses
 * @ingroup CommClasses
 *
 * FalconComm is the base class for all communications behavior implementations. The Falcon itself uses an
 * FTDI chip to talk to the computer, so these will most likely be proxies to either an FTDI library
 * (like FTD2XX), or reimplementation of FTDI calls (like our libusb implementation).
 *
 * FalconComm is responsible for handling a few different tasks
 * - Getting the count of devices connected to the system
 * - Opening and closing the connection to the device
 * - Establishing modes (firmware and normal)
 * - Reading data from and writing data to the device
 *
 * All communications objects are considered to be non-blocking, and should reimplement a poll function to
 * maintain this. Both FTD2XX and libusb, the two implementations of the FalconComm class as of this
 * writing, operate in a non-blocking way. If blocking calls are required (currently only used for loading
 * firmware), blocking functions are provided but are specified as such to warn the user.
 *
 * As mentioned above, the falcon can operate in two modes
 * - Firmware - Used to load firmware to the device
 * - Normal - Used to communicate with the device after firmware is loaded
 *
 * To get the device into firmware mode, the follow set of steps needs to happen:
 *
 * -# Clear send and receive buffers
 * -# Set FTDI latency timer to 16ms
 * -# Set line properties to 8N1
 * -# Set baud to 9600
 * -# Disable flow control
 * -# Set RTS low
 * -# Set DTR low
 * -# Set DTR high
 * -# Send 3 bytes: 0x0a 0x43 0x0d
 * -# Receive 5 bytes: 0x00 0xa 0x44 0x2c 0xd
 * -# Set DTR low
 * -# Set baud to new value (0x15 in control message)
 * -# Send 1 byte: 0x40
 * -# Receive 1 byte: 0x41
 *
 * Once this set of commands has been run and the send/receives match what is listed above, we can continue
 * with loading the firmware. The actual firmware loading simply consists of sending 128 byte blocks of the
 * firmware to the device, and receiving back those blocks for error checking. This happens in the
 * FalconFirmware::loadFirmware function.
 *
 * Child classes of FalconComm implement the above code in their setFirmwareMode() function.
 *
 * Once firmware is loaded, the falcon needs to be returned to normal mode in order to exchange useful data
 * with the driver. To do this, we execute the following steps:
 *
 * -# Set the FTDI latency timer to 1ms
 * -# Set the baud rate to the appropriate value (0x2 in control message)
 * -# Clear send and receive buffers
 *
 * After this, the falcon will be in normal communications mode, and regular I/O can begin. I/O specifics
 * are defined in the firmware classes of libnifalcon.
 *
 * While FalconComm is mainly geared toward making sure we can talk to the device, it can also be used for
 * test purposes, like building network interfaces to emulate the falcon hardware.
 */
 
	class FalconComm : public FalconCore
	{
	public:
		enum {
			FALCON_COMM_DEVICE_ERROR = 2000, /*!< Device driver specific error, check getDeviceErrorCode */
			FALCON_COMM_NOT_INITIALIZED, /*!< Device drivers not initialized */
			FALCON_COMM_DEVICE_NOT_FOUND_ERROR, /*!< No devices found on the system */
			FALCON_COMM_DEVICE_NOT_VALID_ERROR, /*!< Device not opened */
			FALCON_COMM_DEVICE_INDEX_OUT_OF_RANGE_ERROR, /*!< Device index for opening out of range of available devices */
			FALCON_COMM_FIRMWARE_NOT_FOUND_ERROR, /*!< Firmware file not found */
			FALCON_COMM_WRITE_ERROR, /*!< Write timeout hit, underflow, etc... */
			FALCON_COMM_READ_ERROR /*!< Read timeout hit, underflow, etc... */
		};

		/**
		 * Constructor
		 *
		 *
		 */
		FalconComm() :
			m_isCommOpen(false),
			m_hasBytesAvailable(false),
			m_bytesAvailable(0)
		{}
		
		/**
		 * Destructor
		 *
		 *
		 */
		virtual ~FalconComm() {}
		
		/**
		 * Returns the number of devices connected to the system
		 *
		 * @param[out] count The number of devices available
		 *
		 * @return True if count was retreived correctly, false otherwise. Error code set if false.
		 */
		virtual bool getDeviceCount(unsigned int& count) = 0;
		
		/**
		 * Opens the device at the specified index
		 *
		 * @param[in] index Index of the device to open
		 *
		 * @return True if device is opened successfully, false otherwise. Error code set if false.
		 */
		virtual bool open(unsigned int index) = 0;
		
		/**
		 * Closes the device, if open
		 *
		 *
		 * @return True if device is closed successfully, false otherwise. Error code set if false.
		 */
		virtual bool close() = 0;
		
		/**
		 * Read a specified number of bytes from the device
		 *
		 * @param[out] str Buffer to read data into
		 * @param[in] size Amount of bytes to read
		 *
		 * @return True if (size) amount of bytes is read successfully, false otherwise. Error code set if false.
		 */
		virtual bool read(uint8_t* str, unsigned int size) = 0;
		
		/**
		 * Write a specified number of bytes to the device
		 *
		 * @param[in] str Buffer to write data from
		 * @param[in] size Amount of bytes to write
		 *
		 * @return True if (size) amount of bytes is written successfully, false otherwise. Error code set if false.
		 */
		virtual bool write(uint8_t* str, unsigned int size) = 0;
		
 		/**
		 * Read a specified number of bytes from the device
		 *
		 * @param[out] str Buffer to read data into
		 * @param[in] size Amount of bytes to read
		 *
		 * @return True if (size) amount of bytes is read successfully, false otherwise. Error code set if false.
		 */
		virtual bool readBlocking(uint8_t* str, unsigned int size) = 0;
		
		/**
		 * Write a specified number of bytes to the device
		 *
		 * @param[in] str Buffer to write data from
		 * @param[in] size Amount of bytes to write
		 *
		 * @return True if (size) amount of bytes is written successfully, false otherwise. Error code set if false.
		 */
		virtual bool writeBlocking(uint8_t* str, unsigned int size) = 0;

		/**
		 * Sets the communications mode and initializes the device to load firmware
		 *
		 *
		 * @return True if device is successfully set to load firwmare, false otherwise. Error code set if false.
		 */
		virtual bool setFirmwareMode() = 0;
		
		/**
		 * Sets the communications mode and initializes the device to run in normal operation
		 *
		 *
		 * @return True if device is successfully set to normal operation, false otherwise. Error code set if false.
		 */
		virtual bool setNormalMode() = 0;

		/**
		 * Returns the number of bytes returned in the last read function
		 *
		 *
		 * @return Number of bytes read
		 */
		int getLastBytesRead()  {return m_lastBytesRead; }
		
		/**
		 * Returns the number of bytes returned in the last write function
		 *
		 *
		 * @return Number of bytes written
		 */
		int getLastBytesWritten()  {return m_lastBytesWritten; }
		
		/**
		 * Returns the device specific error code for in-depth debugging
		 *
		 *
		 * @return Index for the communication policy specific error code
		 */
		int getDeviceErrorCode() { return m_deviceErrorCode; }
		
		/**
		 * Returns whether the device is open or not
		 *
		 *
		 * @return True if open, false otherwise
		 */
		bool isCommOpen() { return m_isCommOpen; }

		/**
		 * Reset the internal state of the communications object (bytes read/written, etc...)
		 */
		virtual void reset() {}

		/**
		 * Checks to see whether there are bytes available to read from the object
		 *
		 * @return True if comm object has bytes available to read
		 */
		bool hasBytesAvailable() { return m_hasBytesAvailable; }

		/**
		 * Returns the number of bytes available to read (buffered from object)
		 *
		 * @return Number of bytes available to read
		 */
		unsigned int getBytesAvailable() { return m_bytesAvailable; }

		/**
		 * Sets the number of bytes that are ready for reading
		 *
		 * @return Number of bytes available to read
		 */
		virtual void setBytesAvailable(unsigned int b) { m_bytesAvailable = b; }

		/**
		 * Polls the object for confirmation of write/read return
		 */
		virtual void poll() {}
		
	protected:
		const static unsigned int MAX_DEVICES = 128; /**< Maximum number of devices to store in count buffers */
		const static unsigned int FALCON_VENDOR_ID = 0x0403; /**< USB Vendor ID for the Falcon */
		const static unsigned int FALCON_PRODUCT_ID = 0xCB48; /**< USB Product ID from the Falcon */
		int m_deviceErrorCode;	/**< Communications policy specific error code */
		int m_lastBytesRead;	/**< Number of bytes read in last read operation */
		int m_lastBytesWritten; /**< Number of bytes written in the last write operation */
		bool m_isCommOpen; 	/**< Whether or not the communications are open */
		bool m_hasBytesAvailable; /**< Whether or not the object has bytes available to read */
		int m_bytesAvailable; /**< Number of bytes object has available to read */
	};

};

#endif
