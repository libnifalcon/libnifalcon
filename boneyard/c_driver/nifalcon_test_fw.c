/***
 * @file nifalcon_test_fw.c
 * @brief Implementation file for C version of libnifalcon driver - test firmware functionality
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
#include <string.h>

//Ripped from libftdi, so we can standardize how we return errors
#define nifalcon_error_return(code, str) do {	   \
		dev->falcon_status_str = str;			   \
        dev->falcon_status_code = -code;			   \
        return -code;							   \
   } while(0);

//Double buffering for message receiving
unsigned char recv_buffer[2][16];
unsigned char recv_buffer_ready = 0;
unsigned char recv_buffer_current = 0;
unsigned char recv_buffer_pos = 0;

//STRUCT FORMATTING FOR LITTLE-ENDIAN ARCHS ONLY (I'll fix this at some point)
void nifalcon_test_fw_format_output(unsigned char* output_stream, falcon_packet* output)
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

void nifalcon_test_fw_format_input(unsigned char* input_stream, falcon_packet* input)
{
	//Turn system-specific ints into motor values
	int i;
	input_stream[0] = '<';
	input_stream[15] = '>';
	for(i = 0; i < 3; ++i)
	{
		int idx = 1 + (i*4);
		*(input_stream+idx) =   ((input->motor[i]) & 0x000f) ;
		*(input_stream+idx+1) = ((input->motor[i]) & 0x00f0) >> 4;
		*(input_stream+idx+2) = ((input->motor[i]) & 0x0f00) >> 8;
		*(input_stream+idx+3) = ((input->motor[i]) & 0xf000) >> 12;
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

int nifalcon_test_fw_send_struct(falcon_device* dev, falcon_packet* input)
{
	unsigned char input_temp[16];
	nifalcon_test_fw_format_input(input_temp, input);
	return nifalcon_test_fw_send_raw(dev, input_temp);
}

int nifalcon_test_fw_send_raw(falcon_device* dev, unsigned char* input)
{
	return nifalcon_write(dev, input, 16);
}

int nifalcon_test_fw_receive_struct(falcon_device* dev, falcon_packet* output, unsigned int timeout_ms)
{
	int status = 0, bytes_read = 0;
	unsigned char output_temp[64];
	if((status = nifalcon_test_fw_receive_raw(dev, output_temp, timeout_ms)) < 0) return status;

	if(recv_buffer_ready)
	{
		recv_buffer_ready = 0;
		if(recv_buffer_current) nifalcon_test_fw_format_output(recv_buffer[0], output);
		else nifalcon_test_fw_format_output(recv_buffer[1], output);
		return 0;
	}	
	nifalcon_error_return(NOVINT_TEST_FW_RECEIVE_STRUCT_ERROR, "No struct buffer available for read yet");
}

int nifalcon_test_fw_receive_raw(falcon_device* dev, unsigned char* output, unsigned int timeout_ms)
{
	int status, i;
	if((status = nifalcon_read(dev, output, 16, timeout_ms)) < 16)
	{
		if(status < 0) return status;
		nifalcon_error_return(NOVINT_TEST_FW_RECEIVE_AMOUNT_ERROR, "Less than 16 bytes received from firmware query");
	}
	
	for(i = 0; i < status; ++i)
	{		
		recv_buffer[recv_buffer_current][recv_buffer_pos] = output[i];
		++recv_buffer_pos;
		if(output[i] == '>')
		{
			if(recv_buffer_pos == 16) recv_buffer_ready = 1;
			recv_buffer_pos = 0;
			if(recv_buffer_current) recv_buffer_current = 0;
			else recv_buffer_current = 1;
		}
	}

	return 0;
}
