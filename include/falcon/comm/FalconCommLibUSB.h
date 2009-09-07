/***
 * @file FalconCommLibUSB.h
 * @brief LibUSB 1.0 (http://libusb.sourceforge.net/) based implementation of FTDI communication for the falcon
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONCOMMLIBUSB_H
#define FALCONCOMMLIBUSB_H

#include "falcon/core/FalconComm.h"

struct timeval;
struct libusb_device_handle;
struct libusb_transfer;
struct libusb_context;

namespace libnifalcon
{
/**
 * @class FalconCommLibUSB
 * @ingroup CommClasses
 *
 * FalconCommLibUSB is the libusb-1.0 implementation of the falcon communications core. This core is meant to
 * be used on non-windows platforms, since it's open source (unlike ftd2xx) and seems to work well. Much of
 * the code is a reimplementation of functions found in libftdi:
 *
 * http://www.intra2net.com/en/developer/libftdi/
 *
 * However, due to our need to access the falcon at as close to a sustained 1khz rate as possible, we needed
 * to use a non-blocking communications layer.
 *
 * FalconCommLibUSB is built directly into the libnifalcon core library, as is chosen for the user
 * by default by the FalconDevice constructor, so it is usually not needed.
 * However, it is left here for code compatibility for code that already used comm behavior setting, which
 * was required before libnifalcon v1.0
 */

	class FalconCommLibUSB : public FalconComm
	{
	public:
		/**
		 * Constructor
		 *
		 *
		 */
		FalconCommLibUSB();

		/**
		 * Destructor
		 *
		 *
		 */
		~FalconCommLibUSB();

		/**
		 * Returns the number of devices connected to the system
		 *
		 * @param[out] count The number of devices available
		 *
		 * @return True if count was retreived correctly, false otherwise. Error code set if false.
		 */
		virtual bool getDeviceCount(unsigned int& count);

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
		virtual bool readBlocking(uint8_t* str, unsigned int size);
		
		/**
		 * Write a specified number of bytes to the device
		 *
		 * @param[in] str Buffer to write data from
		 * @param[in] size Amount of bytes to write
		 *
		 * @return True if (size) amount of bytes is written successfully, false otherwise. Error code set if false.
		 */
		virtual bool writeBlocking(uint8_t* str, unsigned int size);

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
		
		/**
		 * Sets the number of bytes that are ready for reading
		 *
		 * @return Number of bytes available to read
		 */
		virtual void setBytesAvailable(uint32_t b);

		/**
		 * Callback for result of reading data
		 */
		static void cb_in(struct libusb_transfer *transfer);

		/**
		 * Callback for result of writing data
		 */
		static void cb_out(struct libusb_transfer *transfer);

		/**
		 * Mutator function needed by static callbacks for class updates
		 */
		void setHasBytesAvailable(bool v);

		/**
		 * Mutator function needed by static callbacks for class updates
		 */
		void setSent() { m_isWriteAllocated = false; }

		/**
		 * Mutator function needed by static callbacks for class updates
		 */
		void setReceived()  { m_isReadAllocated = false; }

		/**
		 * Initializes libusb core
		 *
		 * @return True on successful initialization, false otherwise
		 */
		bool initLibUSB();
		
		/**
		 * Polls the object for confirmation of write/read return
		 */
		void poll();

		/**
		 * Reset the internal state of the communications object (bytes read/written, etc...)
		 */		
		void reset();

		/**
		 * Causes a read to be queued
		 */
		void issueRead();
	protected:
		/**
		 * True if we currently have a write queued
		 */ 
		bool m_isWriteAllocated;

		/**
		 * True if we currently have a read queued
		 */ 
		bool m_isReadAllocated;

		/**
		 * Used for setting timeouts
		 */ 
		struct timeval* m_tv;

		/**
		 * libusb device handle struct
		 */ 
		libusb_device_handle* m_falconDevice;

		/**
		 * Buffers for I/O
		 */ 
		unsigned char input[128], output[128];

		/**
		 * Transfer for reading
		 */ 
		struct libusb_transfer* in_transfer;

		/**
		 * Transfer for writing
		 */ 
		struct libusb_transfer* out_transfer;

		/**
		 * Transfer for control messages
		 */ 
		struct libusb_context* m_usbContext;
	private:
		DECLARE_LOGGER();
	};
};

#endif
