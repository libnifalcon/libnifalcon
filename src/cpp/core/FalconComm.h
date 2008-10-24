/***
 * @file FalconComm.h
 * @brief Base class for FTDI communications policy classes
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
#ifndef FALCONCOMMBASE_H
#define FALCONCOMMBASE_H

#include <stdint.h>
#include "FalconCore.h"

namespace libnifalcon 
{
	class FalconComm : public FalconCore
	{		
	public:
		enum {
			FALCON_COMM_DEVICE_ERROR = 2000,  /*!< Device driver specific error, check getDeviceErrorCode */
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
			m_requiresPoll(false),
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
		virtual bool getDeviceCount(int8_t& count) = 0;
		/** 
		 * Opens the device at the specified index
		 * 
		 * @param[in] index Index of the device to open
		 * 
		 * @return True if device is opened successfully, false otherwise. Error code set if false.
		 */
		virtual bool open(uint8_t index) = 0;
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
		virtual bool read(uint8_t* str, uint32_t size) = 0;
		/** 
		 * Write a specified number of bytes to the device
		 * 
		 * @param[in] str Buffer to write data from
		 * @param[in] size Amount of bytes to write
		 * 
		 * @return True if (size) amount of bytes is written successfully, false otherwise. Error code set if false.
		 */
		virtual bool write(uint8_t* str, uint32_t size) = 0;
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


		bool requiresPoll() { return m_requiresPoll; }
		bool hasBytesAvailable() { return m_hasBytesAvailable; }
		uint32_t getBytesAvailable() { return m_bytesAvailable; }
		void setBytesAvailable(uint32_t b) { m_bytesAvailable = b; }
		virtual void poll() {}
	protected:
		const static uint8_t MAX_DEVICES = 128; /**< Maximum number of devices to store in count buffers */
		const static uint16_t FALCON_VENDOR_ID = 0x0403; /**< USB Vendor ID for the Falcon */
		const static uint16_t FALCON_PRODUCT_ID = 0xCB48; /**< USB Product ID from the Falcon */
		int m_deviceErrorCode;	/**< Communications policy specific error code */
		int m_lastBytesRead;	/**< Number of bytes read in last read operation */
		int m_lastBytesWritten; /**< Number of bytes written in the last write operation */
		bool m_isCommOpen; 	/**< Whether or not the communications are open */
		bool m_hasBytesAvailable;
		int m_bytesAvailable;
		bool m_requiresPoll;
	};

};

#endif
