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

#define NOVINT_LED_GREEN 0x2
#define NOVINT_LED_BLUE 0x4
#define NOVINT_LED_RED 0x8

typedef struct falcon_packet
{
	//Is short guarenteed to be 2 bytes?
	short motor[3]; 
	unsigned char info;
	unsigned char unknown;
} falcon_packet;

int nifalcon_get_count();
int nifalcon_open(falcon_device *dev, unsigned int device_index);
int nifalcon_init(falcon_device dev);
int nifalcon_close(falcon_device dev);

void nifalcon_init_packet(falcon_packet* packet);
int nifalcon_send_struct(falcon_device dev, falcon_packet* input);
int nifalcon_send_raw(falcon_device dev, char* input);
int nifalcon_receive_struct(falcon_device dev, falcon_packet* output);
int nifalcon_receive_raw(falcon_device dev, char* output);

#endif
