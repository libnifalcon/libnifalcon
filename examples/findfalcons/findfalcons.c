/*
 * Implementation file for NovInt Falcon FindFalcon Example Program
 *
 * Copyright (c) 2007 Kyle Machulis <kyle@nonpolynomial.com> http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 */

#include "nifalcon_test_fw.h"
#include <stdio.h>

#define PACKET_TIMEOUT 1000

int main(int argc, char** argv)
{
	int num_falcons, status;
	unsigned int count;
	unsigned char input[17] = "<AAAAAAAAAAAAAA>";
	unsigned char output[17];	
	falcon_packet input_packet, output_packet;
	falcon_device dev;

	nifalcon_init(&dev);
	
	num_falcons = nifalcon_get_count(&dev);
	
	count = 0;
	input[16] = output[16] = 0;

	printf("Falcons found: %d\n", num_falcons);

	printf("Opening falcon\n");
	if((status = nifalcon_open(&dev, 0)) < 0)
	{
		printf ("Cannot open falcon! %d\n", status);
		return 1;
	}
	printf("Opened falcon\n");
	printf("Loading firmware\n");
	if((status = nifalcon_load_firmware(&dev, "test_firmware.bin")) < 0)
	{
		printf("Firmware not loaded! %d\n", status);
		return 1;
	}
	printf("Firmware loaded\n");

	if(nifalcon_test_fw_send_raw(&dev, input) < 0) return 1;
	printf("Send Raw: %s\n", input);
	if(nifalcon_test_fw_receive_raw(&dev, output, PACKET_TIMEOUT) < 0) return 1;
	printf("Receive Raw: %s\n", output);
	nifalcon_test_fw_init_packet(&input_packet);
	nifalcon_test_fw_init_packet(&output_packet);
	while(1)
	{
		printf("Writing %d\n", count);
		if(nifalcon_test_fw_send_struct(&dev, &input_packet) < 0) return 1;
		printf("reading %d\n", count);
		if(nifalcon_test_fw_receive_struct(&dev, &output_packet, PACKET_TIMEOUT) < 0) return 1;
		printf("%x %x %x %x\n", output_packet.motor[0], output_packet.motor[1], output_packet.motor[2], output_packet.info);
		++count;
	}

	nifalcon_close(&dev);
	return 0;
}
