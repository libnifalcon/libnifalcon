/*
 * Implementation file for NovInt Falcon User Space Driver
 *
 * Copyright (c) 2007 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
 *
 * More info on Nonpolynomial Labs @ http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 */

#include "nifalcon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DEVICES 128

//STRUCT FORMATTING FOR LITTLE-ENDIAN ARCHS ONLY (I'll fix this at some point)
//Damn you PPC people, go buy an intel mac already.

void nifalcon_format_output(char* output_stream, falcon_packet* output)
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

void nifalcon_format_input(char* input_stream, falcon_packet* input)
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

FT_STATUS nifalcon_read_wait(falcon_device dev, char* str, unsigned int size)
{
	unsigned int o, bytes_rx, bytes_written;
	FT_STATUS ftStatus;

	o = 0;

	while(o < size)
	{
		if((ftStatus = FT_GetQueueStatus(dev, &bytes_rx)) != FT_OK) return ftStatus;
		if(bytes_rx > 0)
		{
			FT_Read(dev, str, bytes_rx, &bytes_written);
			o += bytes_rx;
		}
	}
	return FT_OK;
}

int nifalcon_get_count()
{
    unsigned int falcon_count;
	char* pcBufLD[MAX_DEVICES + 1];
	char cBufLD[MAX_DEVICES][64];
	int i;
	FT_STATUS ftStatus;

	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}
#ifndef WIN32
	FT_SetVIDPID(0x0403, 0xCB48);
#endif
	if((ftStatus = FT_ListDevices(pcBufLD, &falcon_count, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER)) != FT_OK) return -1;
	for(i = 0; ( (i <MAX_DEVICES) && (i < falcon_count) ); i++) {
		printf("Device %d Serial Number - %s\n", i, cBufLD[i]);
	}
	return falcon_count;
}

int nifalcon_open(falcon_device *dev, unsigned int device_index)
{
	unsigned int falcon_count, i;
	char* pcBufLD[MAX_DEVICES + 1];
	char cBufLD[MAX_DEVICES][64];
	FT_STATUS ftStatus;
	
#ifndef WIN32
	FT_SetVIDPID(0x0403, 0xCB48);
#endif

	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}
	if((ftStatus = FT_ListDevices(pcBufLD, &falcon_count, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER)) != FT_OK) return ftStatus;
	if(device_index > falcon_count)	return ftStatus;
	for(i = 0; ( (i <MAX_DEVICES) && (i < falcon_count) ); i++) {
		printf("Device %d Serial Number - %s\n", i, cBufLD[i]);
	}

	//Open and reset device
	if((ftStatus = FT_OpenEx(cBufLD[0], FT_OPEN_BY_SERIAL_NUMBER, dev)) != FT_OK) return ftStatus;
	
	return FT_OK;
}

int nifalcon_init(falcon_device dev)
{
	unsigned int bytes_written, bytes_read;
	char check_msg_1[3] = {0x0a, 0x43, 0x0d};
	char check_buf[128];
	FILE* key_file;
	FT_STATUS ftStatus;

	if((ftStatus = FT_ResetDevice(dev)) != FT_OK) return ftStatus;
	
	//Set to:
	// 9600 baud
	// 8n1
	// No Flow Control
	// RTS Low
	// DTR High	
	if((ftStatus = FT_SetBaudRate(dev, 9600)) != FT_OK) return ftStatus;
	if((ftStatus = FT_SetDataCharacteristics(dev, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE)) != FT_OK) return ftStatus;
	if((ftStatus = FT_SetFlowControl(dev, FT_FLOW_NONE, 0, 0)) != FT_OK) return ftStatus;
	if((ftStatus = FT_ClrRts(dev)) != FT_OK) return ftStatus;
	if((ftStatus = FT_ClrDtr(dev)) != FT_OK) return ftStatus;
	if((ftStatus = FT_SetDtr(dev)) != FT_OK) return ftStatus;

	//Send 3 bytes: 0x0a 0x43 0x0d
	printf("Wrote 3 bytes init\n");
	if((ftStatus = FT_Write(dev, check_msg_1, 3, &bytes_written)) != FT_OK) return ftStatus;
	
	//Expect 5 bytes back
	if((ftStatus = nifalcon_read_wait(dev, check_buf, 4)) != FT_OK) return ftStatus;	
	printf("Got back confirmation\n");

	//Set to:
	// DTR Low
	// 140000 baud (0x15 clock ticks per signal)
	if((ftStatus = FT_ClrDtr(dev)) != FT_OK) return ftStatus;
	if((ftStatus = FT_SetBaudRate(dev, 140000)) != FT_OK) return ftStatus;
	if((ftStatus = FT_Purge(dev, FT_PURGE_RX)) != FT_OK) return ftStatus;

	//Send "A" character
	if((ftStatus = FT_Write(dev, "A", 1, &bytes_written)) != FT_OK) return ftStatus;

	//Expect back 1 byte:
	// 0x41 ("A")
	if((ftStatus = nifalcon_read_wait(dev, check_buf, 1)) != FT_OK) return ftStatus;	
	printf("Got back %c\n", check_buf[0]);

	printf("Writing novint.bin\n");
	key_file = fopen("/Users/qdot/novint.bin", "rb");

	if(!key_file)
	{
		return -1;
	}
	while(!feof(key_file))
	{
		bytes_read = fread(check_buf, 1, 128, key_file);
		if((ftStatus = FT_Write(dev, check_buf, bytes_read, &bytes_written)) != FT_OK) return ftStatus;
		if((ftStatus = nifalcon_read_wait(dev, check_buf, bytes_read)) != FT_OK) return ftStatus;	
		if(bytes_read < 128) break;
	}
	fclose(key_file);

	if((ftStatus = FT_SetBaudRate(dev, 1456312)) != FT_OK) return ftStatus;
	return FT_OK;
}

int nifalcon_close(falcon_device dev)
{
	if(!dev) return -1;
	FT_Close(dev);
	return 0;
}

void nifalcon_init_packet(falcon_packet* packet)
{
	memset(packet, 0, sizeof(falcon_packet));
}

int nifalcon_send_struct(falcon_device dev, falcon_packet* input)
{
	FT_STATUS ftStatus;
	char input_temp[16];
	if(!dev) return -1;
	nifalcon_format_input(&input_temp, input);
	return nifalcon_send_raw(dev, input_temp);
}

int nifalcon_send_raw(falcon_device dev, char* input)
{
	FT_STATUS ftStatus;
	unsigned int bytes_written;
	if(!dev) return -1;
	return FT_Write(dev, input, 16, &bytes_written);
}

int nifalcon_receive_struct(falcon_device dev, falcon_packet* output)
{
	FT_STATUS ftStatus;
	char output_temp[16];
	if(!dev) return -1;
	if((ftStatus = nifalcon_receive_raw(dev, &output_temp)) != FT_OK) return ftStatus;	
	nifalcon_format_output(&output_temp, output);
	return ftStatus;
}

int nifalcon_receive_raw(falcon_device dev, char* output)
{
	if(!dev) return -1;
	return nifalcon_read_wait(dev, output, 16); 
}
