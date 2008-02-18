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
	int num_falcons = nifalcon_get_count();
	unsigned int bytes_read, bytes_written, count;
	char input[17] = "<AAAAAAAAAAAAAA>";
	char output[17];	
	falcon_packet input_packet, output_packet;
	falcon_device dev;

	count = 0;
	input[16] = output[16] = 0;

	printf("Falcons found: %d\n", num_falcons);
	
	if(nifalcon_open(&dev, 0) != FT_OK) return 1;
	if(nifalcon_init(dev, "novint.bin") != FT_OK) return 1;
	
	if(nifalcon_test_fw_send_raw(dev, &input, &bytes_written) != FT_OK) return 1;
	printf("Send Raw: %s\n", input);
	if(nifalcon_test_fw_receive_raw(dev, &output, PACKET_TIMEOUT, &bytes_read) != FT_OK) return 1;
	printf("Receive Raw: %s\n", output);
	nifalcon_test_fw_init_packet(&input_packet);
	nifalcon_test_fw_init_packet(&output_packet);
	while(1)
	{
		printf("Writing %d\n", count);
		if(nifalcon_test_fw_send_struct(dev, &input_packet, &bytes_written) != FT_OK) return 1;
		printf("reading %d\n", count);
		if(nifalcon_test_fw_receive_struct(dev, &output_packet, PACKET_TIMEOUT, &bytes_read) != FT_OK) return 1;
		printf("%x %x %x %x\n", output_packet.motor[0], output_packet.motor[1], output_packet.motor[2], output_packet.info);
		++count;
	}
	
	nifalcon_close(dev);
	return 0;
}
