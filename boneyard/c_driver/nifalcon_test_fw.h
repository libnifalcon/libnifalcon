/***
 * @file nifalcon_test_fw.h
 * @brief Declaration file for C version of libnifalcon driver - test firmware functionality
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


#ifndef LIBNIFALCON_TEST_FIRMWARE_H
#define LIBNIFALCON_TEST_FIRMWARE_H

#include "nifalcon.h"

/// Bit values for flags in the info portion of the packet
/// All directions are as viewed from the front of the falcon

/// Input - OR in info to turn on Homing Mode
#define NOVINT_TEST_FW_HOMING_MODE 0x01
/// Input - OR in info to turn Green LED On
#define NOVINT_TEST_FW_LED_GREEN 0x02
/// Input - OR in info to turn Blue LED On
#define NOVINT_TEST_FW_LED_BLUE 0x04
/// Input - OR in info to turn Red LED On
#define NOVINT_TEST_FW_LED_RED 0x08
/// Output - AND against info to get status of Far Right Button on 4 Button Widget
#define NOVINT_TEST_FW_BUTTON1 0x01
/// Output - AND against info to get status of Center (Forward) Button on 4 Button Widget
#define NOVINT_TEST_FW_BUTTON2 0x02
/// Output - AND against info to get status of Center (Circle) Button on 4 Button Widget
#define NOVINT_TEST_FW_BUTTON3 0x04
/// Output - AND against info to get status of Far Left Button on 4 Button Widget
#define NOVINT_TEST_FW_BUTTON4 0x08
/// Output - AND against info to get homing status of Axis1(Top axis)
#define NOVINT_TEST_FW_HOMED_AXIS1 0x10
/// Output - AND against info to get homing status of Axis2(Right axis)
#define NOVINT_TEST_FW_HOMED_AXIS2 0x20
/// Output - AND against info to get homing status of Axis3(Left axis)
#define NOVINT_TEST_FW_HOMED_AXIS3 0x40

/// Packet structure for the test firmware
typedef struct falcon_packet
{
	/// Array of motors. Input - Motor torque values. Output - Motor encoder values
	short motor[3];
	/// Misc Info. Input - LED Commands. Output - Button status and homing/calibration status.
	unsigned char info;
	/// Not currently known or used?
	unsigned char unknown; 
} falcon_packet;

enum {
	NOVINT_TEST_FW_RECEIVE_ERROR = 2000,
	NOVINT_TEST_FW_RECEIVE_STRUCT_ERROR,
	NOVINT_TEST_FW_RECEIVE_AMOUNT_ERROR
};

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * Zeros out the memory of a packet
 * 
 */
void nifalcon_test_fw_init_packet(falcon_packet* packet);

/** 
 * Formats a falcon_packet and sends it to the falcon
 *
 * @param dev Device handle to initialize
 * @param input Packet to send to the device
 * @param bytes_written Pointer to an int, stores the number of bytes written
 *
 * @return FT_OK on success, FTDI driver error on failure
 */
int nifalcon_test_fw_send_struct(falcon_device* dev, falcon_packet* input);

/** 
 * Sends a raw string (presumed to be 16 bytes and formatted appropriately) to the falcon
 *
 * @param dev Device handle to initialize
 * @param input Data to send to device
 * @param bytes_written Pointer to an int, stores the number of bytes written
 *
 * @return FT_OK on success, FTDI driver error on failure
 */
int nifalcon_test_fw_send_raw(falcon_device* dev, unsigned char* input);

/** 
 * Checks for data from the falcon, and returns it as a falcon_packet
 *
 * @param dev Device handle to initialize
 * @param output falcon_packet to receive data in
 * @param timeout_ms Send timeout, in milliseconds
 * @param bytes_read Pointer to an int, stores the number of bytes read (in case time out is reached)
 *
 * @return FT_OK on success, FTDI driver error on failure
 */
int nifalcon_test_fw_receive_struct(falcon_device* dev, falcon_packet* output, unsigned int timeout_ms);

/** 
 * Checks for data from the falcon, and returns it as a raw string
 *
 * @param dev Device handle to initialize
 * @param output Buffer to receive data in
 * @param timeout_ms Send timeout, in milliseconds
 * @param bytes_read Pointer to an int, stores the number of bytes read (in case time out is reached)
 *
 * @return FT_OK on success, FTDI driver error on failure
 */
int nifalcon_test_fw_receive_raw(falcon_device* dev, unsigned char* output, unsigned int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif
