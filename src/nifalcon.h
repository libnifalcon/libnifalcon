/*
 * Declaration file for NovInt Falcon User Space Driver
 *
 * Copyright (c) 2007 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
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
#include <ftd2xx.h>
typedef FT_HANDLE falcon_device;

#define NOVINT_FALCON_VENDOR_ID 0x0403
#define NOVINT_FALCON_PRODUCT_ID 0xCB48

// Functions for connection handling and firmware sending
// Functionality is specific to the bootloader on the falcon (outside of read_wait, which is just a wrapper for non-blocking ftdi driver reads)
int nifalcon_get_count();
int nifalcon_open(falcon_device *dev, unsigned int device_index);
int nifalcon_init(falcon_device dev, const char* firmware_filename);
int nifalcon_close(falcon_device dev);
FT_STATUS nifalcon_read_wait(falcon_device dev, char* str, unsigned int size, unsigned int timeout_ms, unsigned int* bytes_read);
#endif
