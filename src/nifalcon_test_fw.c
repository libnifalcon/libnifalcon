/*
 * Implementation file for NovInt Falcon User Space Driver - Test Firmware Functionality
 *
 * Copyright (c) 2007-2008 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
 *
 * More info on Nonpolynomial Labs @ http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 */

#include "nifalcon_test_fw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//STRUCT FORMATTING FOR LITTLE-ENDIAN ARCHS ONLY (I'll fix this at some point)
//Damn you PPC people, go buy an intel mac already.

void nifalcon_test_fw_format_output(char* output_stream, falcon_packet* output)
{
	//Turn motor values into system specific ints
	int i;
	for(i = 0; i < 3; ++i)
	{

		int idx = 1 + (i*4);
		output->motor[i] =
			(((*(output_stream+idx) - 0x41) & 0xf)) |
			(((*(output_stream+idx+1) - 0x41) & 0xf) << 4) |
			(((*(output_stream+idx+2) - 0x41) & 0xf) << 8) |
			(((*(output_stream+idx+3) - 0x41) & 0xf) << 12);
	}
	output->info = output_stream[13] - 0x41;
	output->unknown = output_stream[14] - 0x41;
}

void nifalcon_test_fw_format_input(char* input_stream, falcon_packet* input)
{
	//Turn system-specific ints into motor values
	int i;
	input_stream[0] = '<';
	input_stream[15] = '>';
	for(i = 0; i < 3; ++i)
	{
		int idx = 1 + (i*4);
		*(input_stream+idx) =   ((input->motor[i]) & 0xf000) >> 12;
		*(input_stream+idx+1) = ((input->motor[i]) & 0x0f00) >> 8;
		*(input_stream+idx+2) = ((input->motor[i]) & 0x00f0) >> 4;
		*(input_stream+idx+3) = ((input->motor[i]) & 0x000f);
	}
	input_stream[13] = input->info;
	input_stream[14] = input->unknown;
	for(i = 1; i < 15; ++i)
	{
		input_stream[i] += 0x41;
	}
}


void nifalcon_test_fw_init_packet(falcon_packet* packet)
{
	memset(packet, 0, sizeof(falcon_packet));
}

int nifalcon_test_fw_send_struct(falcon_device dev, falcon_packet* input, unsigned int* bytes_written)
{
	char input_temp[16];
	if(!dev) return -1;
	nifalcon_test_fw_format_input(&input_temp, input);
	return nifalcon_test_fw_send_raw(dev, input_temp, bytes_written);
}

int nifalcon_test_fw_send_raw(falcon_device dev, char* input, unsigned int* bytes_written)
{
	if(!dev) return -1;
	return nifalcon_write(dev, input, 16, bytes_written);
}

int nifalcon_test_fw_receive_struct(falcon_device dev, falcon_packet* output, unsigned int timeout_ms, unsigned int* bytes_read)
{
	int status = 0;
	char output_temp[16];
	if(!dev) return -1;
	if((status = nifalcon_test_fw_receive_raw(dev, &output_temp, timeout_ms, bytes_read)) != 0) return status;	
	nifalcon_test_fw_format_output(&output_temp, output);
	return status;
}

int nifalcon_test_fw_receive_raw(falcon_device dev, char* output, unsigned int timeout_ms, unsigned int* bytes_read)
{
	if(!dev) return -1;
	return nifalcon_read_wait(dev, output, 16, timeout_ms, bytes_read); 
}
