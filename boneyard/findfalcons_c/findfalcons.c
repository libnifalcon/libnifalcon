/***
 * @file findfalcons.c
 * @brief Minimal open-and-run test for the c version of libnifalcon
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

#include "nifalcon_test_fw.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define PACKET_TIMEOUT 1000

falcon_device dev;

void sigproc()
{
	printf("\nclosing falcon and quitting\n");
	nifalcon_close(&dev);
	exit(0);
}

int main(int argc, char** argv)
{
	int num_falcons, status, i;
	unsigned int count;
	unsigned int error_count = 0;
	unsigned int loop_count = 0;
	unsigned char input[17] = "<AAAAAAAAAAAAAA>";	
	unsigned char output[17];
	falcon_packet input_packet, output_packet;

	
	signal(SIGINT, sigproc);
	signal(SIGQUIT, sigproc);
	
	nifalcon_init(&dev);
	
	num_falcons = nifalcon_get_count(&dev);
	
	count = 0;
	input[16] = output[16] = 0;

	nifalcon_test_fw_init_packet(&input_packet);
	nifalcon_test_fw_init_packet(&output_packet);

	printf("Falcons found: %d\n", num_falcons);

	printf("Opening falcon\n");
	if((status = nifalcon_open(&dev, 0)) < 0)
	{
		printf ("Cannot open falcon! Error: %s\n", nifalcon_get_error_string(&dev));
		return 1;
	}
	printf("Opened falcon\n");
			if(nifalcon_test_fw_send_struct(&dev, &input_packet) < 0)
		{
			++error_count;
		}
		if(nifalcon_test_fw_receive_struct(&dev, &output_packet, PACKET_TIMEOUT) < 0)
		{
			++error_count;
		}

	printf("Loading firmware\n");
	for(i = 0; i < 10 && error_count > 0; ++i)
	{
		if((status = nifalcon_load_firmware(&dev, "test_firmware.bin")) == 0) break;
		printf("Firmware not loaded! Error: %d %s\nRetrying...\n", dev.falcon_status_code, nifalcon_get_error_string(&dev));
	}
	if(i == 10)
	{
		printf("Cannot load firmware! Bailing...\n");
		return 1;
	}
	printf("Firmware loaded\n");

	while(1)		
	{
		++loop_count;
		input_packet.info = NOVINT_TEST_FW_HOMING_MODE;
		if(!(output_packet.info & (NOVINT_TEST_FW_HOMED_AXIS1 | NOVINT_TEST_FW_HOMED_AXIS2 | NOVINT_TEST_FW_HOMED_AXIS3)))
		{
			input_packet.info |= NOVINT_TEST_FW_LED_RED;
		}
		else
		{
			input_packet.info |= NOVINT_TEST_FW_LED_GREEN;
		}
		if(nifalcon_test_fw_send_struct(&dev, &input_packet) < 0)
		{
			++error_count;
			continue;
		}
		if(nifalcon_test_fw_receive_struct(&dev, &output_packet, PACKET_TIMEOUT) < 0)
		{
			++error_count;
			continue;
		}

		printf("Loops: %8d | Enc1: %5d | Enc2: %5d | Enc3: %5d | Info: 0x%2x | Errors: %d\n", loop_count,  output_packet.motor[0], output_packet.motor[1], output_packet.motor[2], output_packet.info, error_count);
		++count;
	}
	printf("\n");
	nifalcon_close(&dev);
	return 0;
}
