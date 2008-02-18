/*
 * Declaration file for NovInt Falcon User Space Driver - Test Firmware Functionality
 *
 * Copyright (c) 2007-2008 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
 *
 * More info on Nonpolynomial Labs @ http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 */

#ifndef LIBNIFALCON_TEST_FIRMWARE_H
#define LIBNIFALCON_TEST_FIRMWARE_H

#include "nifalcon.h"

//Bit values for flags in the info packet
//Input (info sent to falcon)
#define NOVINT_TEST_FW_LED_GREEN 0x2
#define NOVINT_TEST_FW_LED_BLUE 0x4
#define NOVINT_TEST_FW_LED_RED 0x8

//Output (info receieved from falcon)
#define NOVINT_TEST_FW_HOMED_AXIS1 0x10
#define NOVINT_TEST_FW_HOMED_AXIS2 0x20
#define NOVINT_TEST_FW_HOMED_AXIS3 0x40
#define NOVINT_TEST_FW_HOMED (NOVINT_TEST_FW_HOMED_AXIS1 | NOVINT_TEST_FW_HOMED_AXIS2 | NOVINT_TEST_FW_HOMED_AXIS3)

typedef struct falcon_packet
{
	short motor[3]; 
	unsigned char info;
	unsigned char unknown;
} falcon_packet;

void nifalcon_test_fw_init_packet(falcon_packet* packet);
int nifalcon_test_fw_send_struct(falcon_device dev, falcon_packet* input, unsigned int* bytes_written);
int nifalcon_test_fw_send_raw(falcon_device dev, char* input, unsigned int* bytes_written);
int nifalcon_test_fw_receive_struct(falcon_device dev, falcon_packet* output, unsigned int timeout_ms, unsigned int* bytes_read);
int nifalcon_test_fw_receive_raw(falcon_device dev, char* output, unsigned int timeout_ms, unsigned int* bytes_read);

#endif
