/*
 * Implementation file for NovInt Falcon User Space Driver - Bootloader and Base Functionality
 *
 * Copyright (c) 2007-2008 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
 *
 * More info on Nonpolynomial Labs @ http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 */

#include "nifalcon.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DEVICES 128

int nifalcon_init(falcon_device* dev)
{	
	return 0;
}

//Just wraps the FT_Read function in a QueueStatus call so we don't block on reads
int nifalcon_read_wait(falcon_device dev, char* str, unsigned int size, unsigned int timeout_ms, unsigned int* bytes_read)
{
	unsigned int o, bytes_rx, bytes_written;
	FT_STATUS ftStatus;
	clock_t timeout = (((float)timeout_ms * .001) * (float)CLOCKS_PER_SEC) + clock();
	*bytes_read = 0;

	while(*bytes_read < size)
	{
		if((ftStatus = FT_GetQueueStatus(dev, &bytes_rx)) != FT_OK) return ftStatus;
		if(bytes_rx > 0)
		{
			FT_Read(dev, str, bytes_rx, &bytes_written);
			*bytes_read += bytes_rx;
		}
		if (clock() > timeout) return FT_OK;
	}
	return FT_OK;
}

int nifalcon_get_count(falcon_device dev)
{
    unsigned int falcon_count;
	char* pcBufLD[MAX_DEVICES + 1];
	char cBufLD[MAX_DEVICES][64];
	int i;
	FT_STATUS ftStatus;

	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}
	//If we're not using windows, we can set PID/VID to filter on. I have no idea why this isn't provided in the windows drivers.
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

	//If we're not using windows, we can set PID/VID to filter on. I have no idea why this isn't provided in the windows drivers.
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

int nifalcon_load_firmware(falcon_device dev, const char* firmware_filename)
{
	unsigned int bytes_written, bytes_read;
	char check_msg_1[3] = {0x0a, 0x43, 0x0d};
	char check_buf[128];
	FILE* firmware_file;
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
	if((ftStatus = nifalcon_read_wait(dev, check_buf, 4, 1000, &bytes_read)) != FT_OK) return ftStatus;	
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
	if((ftStatus = nifalcon_read_wait(dev, check_buf, 1, 1000, &bytes_read)) != FT_OK) return ftStatus;	
	printf("Got back %c\n", check_buf[0]);

	printf("Writing %s\n", firmware_filename);
	firmware_file = fopen(firmware_filename, "rb");

	if(!firmware_file)
	{
		return -1;
	}
	while(!feof(firmware_file))
	{
		int firmware_bytes_read;
		firmware_bytes_read = fread(check_buf, 1, 128, firmware_file);
		if((ftStatus = FT_Write(dev, check_buf, firmware_bytes_read, &bytes_written)) != FT_OK) return ftStatus;
		if((ftStatus = nifalcon_read_wait(dev, check_buf, firmware_bytes_read, 1000, &bytes_read)) != FT_OK) return ftStatus;	
		if(firmware_bytes_read < 128) break;
	}
	fclose(firmware_file);

	if((ftStatus = FT_SetBaudRate(dev, 1456312)) != FT_OK) return ftStatus;
	return FT_OK;
}

int nifalcon_close(falcon_device dev)
{
	if(!*dev) return -1;
	FT_Close(*dev);
	return 0;
}

int nifalcon_write(falcon_device dev, char* str, unsigned int size, unsigned int* bytes_written)
{
	FT_STATUS ftStatus;
	if((ftStatus = FT_Write(dev, str, size, &bytes_written)) != FT_OK) return ftStatus;
	return FT_OK;
}
