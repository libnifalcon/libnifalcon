/***
 * @file nifalcon.h
 * @brief Declaration file for C Version of libnifalcon driver - Bootloader and Base Functionality
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @version $Id$
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL$
 * 
 * Project info at http://libnifalcon.nonpolynomial.com/ 
 *
 */

#ifndef LIBNIFALCON_H
#define LIBNIFALCON_H

#ifdef WIN32
#include <windows.h>
#endif

#if defined(LIBFTDI)
#include <ftdi.h>
typedef struct ftdi_context falcon_handle; /*!< libftdi typedef for falcon access */
#elif defined(LIBFTD2XX)
#include <ftd2xx.h>
typedef FT_HANDLE falcon_handle; /*!< ftd2xx typedef for falcon access */
#else
#error "Either LIBFTDI or FTD2XX (but not both) must be defined to compile libnifalcon"
#endif

/// VID for the Novint Falcon
#define NIFALCON_VENDOR_ID 0x0403
/// PID for the Novint Falcon
#define NIFALCON_PRODUCT_ID 0xcb48
/// FTDI Text Description for the Novint Falcon
#define NIFALCON_DESCRIPTION "FALCON HAPTIC"

/** Falcon Bootloader Error Enumeration
	Lists all errors that can happen in the bootloader functions
 */
enum {
	NIFALCON_DEVICE_NOT_FOUND_ERROR = 1000, /*!< No devices found on the system */
	NIFALCON_DEVICE_NOT_VALID_ERROR, /*!< Device not initialized or opened */
	NIFALCON_DEVICE_INDEX_OUT_OF_RANGE_ERROR, /*!< Device index for opening out of range of available devices */
	NIFALCON_FIRMWARE_NOT_FOUND_ERROR, /*!< Firmware file not found */
	NIFALCON_WRITE_ERROR, /*!< Write timeout hit, etc... */
	NIFALCON_READ_ERROR, /*!< Read timeout hit, etc... */
	NIFALCON_FIRMWARE_CHECKSUM_ERROR, /*!< Error thrown when library does not get back exact copy of firmware as it sent. Usually happens on slow hubs. */
	NIFALCON_NUM_ERRORS
};

#ifdef __cplusplus
extern "C" {
#endif

/** Falcon Device Structure
	Holds the device accessor, as well as state information about the device.
 */
typedef struct falcon_device {
	falcon_handle falcon; /*!< FTDI object to access falcon */
	char is_initialized;  /*!< Boolean set to true when device is opened successfully, false when closed/uninitialized */
	char is_open; /*!< Boolean set to true when device is opened successfully, false when closed/uninitialized */
	int falcon_status_code; /*!< Status code returned from either libnifalcon or ftdi access library (can also be bytes read/written, etc...) */
	char* falcon_status_str;	 /*!< Status string for libnifalcon specific errors and messages */   
} falcon_device ;
	
/** 
 * Initializes the usb bus and device handle
 *
 * @param dev Device structure
 * 
 * @return 0 on success, < 0 on error
 */
int nifalcon_init(falcon_device* dev);

/** 
 * Counts the number of devices connected to the system
 *
 * @param dev Initialized device structure
 * 
 * @return Number of falcons connected, < 0 on error
 */
int nifalcon_get_count(falcon_device* dev);

/** 
 * Opens the device
 * 
 * @param dev Initialized device structure
 * @param device_index < 0 for first available device, otherwise index of the device to open (for multiple devices)
 * 
 * @return 0 on success, < 0 on error
 */
int nifalcon_open(falcon_device* dev, unsigned int device_index);

/** 
 * Closes device passed to it
 * 
 * @param dev Opened device structure to close
 *
 * @return 0 on success, < 0 on error
 */
int nifalcon_close(falcon_device* dev);

/** 
 * Runs initialization sequence for the device, including loading the firmware onto the internal microcontroller
 *
 * @param dev Opened device structure to initialize
 * @param firmware_filename Filename of the firmware to load onto the device
 *
 * @return 0 on success, < 0 on error
 */
int nifalcon_load_firmware(falcon_device* dev, const char* firmware_filename);

/** 
 * Wrapper for read functions to do non-blocking, timeout capable read
 *
 * @param dev Opened device structure 
 * @param str Buffer to store data received from device
 * @param size Size of data to expect from device
 * @param timeout_ms Read timeout, in milliseconds
 * @param bytes_read Pointer to an int, stores the number of bytes read (in case time out is reached)
 *
 * @return 0 on success, < 0 on error
 */
int nifalcon_read(falcon_device* dev, unsigned char* str, unsigned int size, unsigned int timeout_ms);

/** 
 * Wrapper for write functions
 *
 * @param dev Opened device structure
 * @param str Buffer to send to device
 * @param size Size of data to send to device
 *
 * @return 0 on success, < 0 on error
 */
int nifalcon_write(falcon_device* dev, unsigned char* str, unsigned int size);

/** 
 * Returns error string
 *
 * @param dev Device structure
 *
 * @return error string from device structure
 */
char* nifalcon_get_error_string(falcon_device* dev);
	
#ifdef __cplusplus
}
#endif
	
#endif
