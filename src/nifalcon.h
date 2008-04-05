/*
 * Declaration file for NovInt Falcon User Space Driver - Bootloader and Base Functionality
 *
 * Copyright (c) 2007-2008 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
 *
 * More info on Nonpolynomial Labs @ http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 */

#ifndef LIBNIFALCON_H
#define LIBNIFALCON_H

#ifdef WIN32
#include <windows.h>
#endif

#ifdef LIBFTDI
#include <ftdi.h>
#else
#include <ftd2xx.h>
#endif

/// Typedef for translation between libftdi and ftd2xx devices
#ifdef LIBFTDI
typedef struct ftdi_context* falcon_handle;
#else
typedef FT_HANDLE falcon_handle;
#endif

/// VID for the Novint Falcon
#define NIFALCON_VENDOR_ID 0x0403
/// PID for the Novint Falcon
#define NIFALCON_PRODUCT_ID 0xcb48
/// FTDI Text Description for the Novint Falcon
#define NOVINT_FALCON_DESCRIPTION "FALCON HAPTIC"

enum {
	NOVINT_DEVICE_NOT_FOUND_ERROR = 1000,
	NOVINT_DEVICE_NOT_VALID_ERROR,
	NOVINT_DEVICE_INDEX_OUT_OF_RANGE_ERROR,
	NOVINT_FIRMWARE_NOT_FOUND_ERROR,
	NOVINT_WRITE_ERROR,
	NOVINT_READ_ERROR
};

#ifdef __cplusplus
extern "C" {
#endif


typedef struct falcon_device {
	falcon_handle falcon;
	int falcon_error_code;
	char is_open;	
	char* falcon_error_str;	   
} falcon_device ;
	
/** 
 * Initializes the usb bus and device handle
 *
 * @param *dev Pointer to device (can be null for ftd2xx)
 * 
 * @return 0 on success, < 0 on error
 */
int nifalcon_init(falcon_device* dev);

/** 
 * Counts the number of devices connected to the system
 *
 * @param dev Initialized device handle
 * 
 * @return Number of falcons connected, < 0 on error
 */
int nifalcon_get_count(falcon_device* dev);

/** 
 * Opens the device
 * 
 * @param dev Pointer to initialized device handle
 * @param device_index < 0 for first available device, otherwise index of the device to open (for multiple devices)
 * 
 * @return 0 on success, < 0 on error
 */
int nifalcon_open(falcon_device* dev, unsigned int device_index);

/** 
 * Closes device passed to it
 * 
 * @param dev Device handle to close
 *
 * @return 0 on success, < 0 on error
 */
int nifalcon_close(falcon_device* dev);

/** 
 * Runs initialization sequence for the device, including loading the firmware onto the internal microcontroller
 *
 * @param dev Opened device handle to initialize
 * @param firmware_filename Filename of the firmware to load onto the device
 *
 * @return 0 on success, < 0 on error
 */
int nifalcon_load_firmware(falcon_device* dev, const char* firmware_filename);

/** 
 * Wrapper for read functions to do non-blocking, timeout capable read
 *
 * @param dev Initialized and opened device handle 
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
 * @param dev Initialized and opened device handle
 * @param str Buffer to send to device
 * @param size Size of data to send to device
 * @param bytes_written Pointer to an int, stores the number of bytes written
 *
 * @return 0 on success, < 0 on error
 */
int nifalcon_write(falcon_device* dev, unsigned char* str, unsigned int size);

char* nifalcon_get_error_string(falcon_device* dev);
	
#ifdef __cplusplus
}
#endif
	
#endif
