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
#include <time.h>
#include <stdio.h>
#define MAX_DEVICES 128

//Ripped from libftdi, so we can standardize how we return errors
#define nifalcon_error_return(code, str) do {	   \
		dev->falcon_error_str = str;			   \
        dev->falcon_error_code = -code;			   \
        return -code;							   \
   } while(0);

int nifalcon_init(falcon_device* dev)
{
	dev->is_open = 0;
	dev->falcon_error_str = "";
	dev->falcon_error_code = 0;
	return FT_OK;
}

//Just wraps the FT_Read function in a QueueStatus call so we don't block on reads
int nifalcon_read(falcon_device* dev, unsigned char* str, unsigned int size, unsigned int timeout_ms)
{
	unsigned long bytes_rx, bytes_read = 0;
	clock_t timeout = (clock_t)(((double)timeout_ms * .001) * (double)CLOCKS_PER_SEC) + clock();	

	if(!dev->is_open) nifalcon_error_return(NOVINT_DEVICE_NOT_FOUND_ERROR, "tried to read from an unopened device");
	
	while(bytes_read < size)
	{
		if((dev->falcon_error_code = FT_GetQueueStatus(dev->falcon, &bytes_rx)) != FT_OK) return -dev->falcon_error_code;
		if(bytes_rx > size) bytes_rx = size - bytes_read;
		if(bytes_rx > 0)
		{
			FT_Read(dev->falcon, str, bytes_rx, &bytes_read);
			bytes_read += bytes_rx;
		}
		if (clock() > timeout) nifalcon_error_return(NOVINT_READ_ERROR, "read timed out");
	}
	return FT_OK;
}

int nifalcon_write(falcon_device* dev, unsigned char* str, unsigned int size)
{
	unsigned long bytes_written;

	if(!dev->is_open) nifalcon_error_return(NOVINT_DEVICE_NOT_FOUND_ERROR, "tried to write to an unopened device");

	if((dev->falcon_error_code = FT_Write(dev->falcon, str, size, &bytes_written)) != FT_OK) return -dev->falcon_error_code;
	if(bytes_written < size) nifalcon_error_return(NOVINT_WRITE_ERROR, "write error");
	return FT_OK;
}

int nifalcon_get_count(falcon_device* dev)
{
    unsigned int falcon_count = 0, device_count = 0, i = 0;
	char* pcBufLD[MAX_DEVICES + 1];
	char cBufLD[MAX_DEVICES][64];
	
	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}
	//If we're not using windows, we can set PID/VID to filter on. I have no idea why this isn't provided in the windows drivers.
//#ifndef WIN32
//	FT_SetVIDPID(0x0403, 0xCB48);
//#endif
	if((dev->falcon_error_code = FT_ListDevices(pcBufLD, &device_count, FT_LIST_ALL | FT_OPEN_BY_DESCRIPTION)) != FT_OK) return -dev->falcon_error_code;
	for(i = 0; i < device_count; ++i)
	{
		if(!strcmp(cBufLD[i], NOVINT_FALCON_DESCRIPTION)) ++falcon_count;
	}
	return falcon_count;
}

int nifalcon_open(falcon_device *dev, unsigned int device_index)
{
	unsigned int falcon_count = 0, device_count = 0, i = 0;
	char* pcBufLD[MAX_DEVICES + 1];
	char cBufLD[MAX_DEVICES][64];
	char serial[64];

	//If we're not using windows, we can set PID/VID to filter on. I have no idea why this isn't provided in the windows drivers.
#ifndef WIN32
	FT_SetVIDPID(0x0403, 0xCB48);
#endif

	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}
	if((dev->falcon_error_code = FT_ListDevices(pcBufLD, &device_count, FT_LIST_ALL | FT_OPEN_BY_DESCRIPTION)) != FT_OK) return -dev->falcon_error_code;
	for(i = 0; (i < device_count) && (falcon_count <= device_index); ++i)
	{
		if(!strcmp(cBufLD[i], NOVINT_FALCON_DESCRIPTION)) falcon_count++;
	}
	if(falcon_count == 0) nifalcon_error_return(NOVINT_DEVICE_NOT_FOUND_ERROR, "no devices connected to system");
	if(device_index > falcon_count)	nifalcon_error_return(NOVINT_DEVICE_INDEX_OUT_OF_RANGE_ERROR, "device index out of range");

	//Now that we know the index, get the serial number
	if((dev->falcon_error_code = FT_ListDevices((PVOID)(i-1), serial, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER))) return -dev->falcon_error_code;

	//Open and reset device using serial number
	if((dev->falcon_error_code = FT_OpenEx(serial, FT_OPEN_BY_SERIAL_NUMBER, &(dev->falcon))) != FT_OK) return -dev->falcon_error_code;
	dev->is_open = 1;
	return FT_OK;
}

int nifalcon_load_firmware(falcon_device* dev, const char* firmware_filename)
{
	unsigned long bytes_written;
	unsigned char check_msg_1[3] = {0x0a, 0x43, 0x0d};
	unsigned char check_buf[128];
	FILE* firmware_file;
	
	if(!dev->is_open) nifalcon_error_return(NOVINT_DEVICE_NOT_FOUND_ERROR, "tried load firmware to an unopened device");

	if((dev->falcon_error_code = FT_ResetDevice(dev->falcon)) != FT_OK) return -dev->falcon_error_code;

	//Set to:
	// 9600 baud
	// 8n1
	// No Flow Control
	// RTS Low
	// DTR High	
	if((dev->falcon_error_code = FT_SetBaudRate(dev->falcon, 9600)) != FT_OK) return -dev->falcon_error_code;
	if((dev->falcon_error_code = FT_SetDataCharacteristics(dev->falcon, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE)) != FT_OK) return -dev->falcon_error_code;
	if((dev->falcon_error_code = FT_SetFlowControl(dev->falcon, FT_FLOW_NONE, 0, 0)) != FT_OK) return -dev->falcon_error_code;
	if((dev->falcon_error_code = FT_ClrRts(dev->falcon)) != FT_OK) return -dev->falcon_error_code;
	if((dev->falcon_error_code = FT_ClrDtr(dev->falcon)) != FT_OK) return -dev->falcon_error_code;
	if((dev->falcon_error_code = FT_SetDtr(dev->falcon)) != FT_OK) return -dev->falcon_error_code;

	//Send 3 bytes: 0x0a 0x43 0x0d
	if((dev->falcon_error_code = nifalcon_write(dev, check_msg_1, 3)) != FT_OK) return -dev->falcon_error_code;
	
	//Expect 5 bytes back
	if((dev->falcon_error_code = nifalcon_read(dev, check_buf, 5, 1000)) != FT_OK) return -dev->falcon_error_code;	

	//Set to:
	// DTR Low
	// 140000 baud (0x15 clock ticks per signal)
	if((dev->falcon_error_code = FT_ClrDtr(dev->falcon)) != FT_OK) return -dev->falcon_error_code;
	if((dev->falcon_error_code = FT_SetBaudRate(dev->falcon, 140000)) != FT_OK) return -dev->falcon_error_code;
	if((dev->falcon_error_code = FT_Purge(dev->falcon, FT_PURGE_RX)) != FT_OK) return -dev->falcon_error_code;

	//Send "A" character
	if((dev->falcon_error_code = FT_Write(dev->falcon, "A", 1, &bytes_written)) != FT_OK) return -dev->falcon_error_code;

	//Expect back 1 byte:
	// 0x41 ("A")
	if((dev->falcon_error_code = nifalcon_read(dev, check_buf, 1, 1000)) != FT_OK) return -dev->falcon_error_code;	

	firmware_file = fopen(firmware_filename, "rb");

	if(!firmware_file)
	{
		nifalcon_error_return(NOVINT_FIRMWARE_NOT_FOUND_ERROR, "cannot find falcon firmware file");
	}
	while(!feof(firmware_file))
	{
		int firmware_bytes_read;
		firmware_bytes_read = fread(check_buf, 1, 128, firmware_file);
		if((dev->falcon_error_code = nifalcon_write(dev, check_buf, firmware_bytes_read)) != FT_OK) return -dev->falcon_error_code;
		if((dev->falcon_error_code = nifalcon_read(dev, check_buf, firmware_bytes_read, 1000)) != FT_OK) return -dev->falcon_error_code;	
		if(firmware_bytes_read < 128) break;
	}
	fclose(firmware_file);

	if((dev->falcon_error_code = FT_SetBaudRate(dev->falcon, 1456312)) != FT_OK) return -dev->falcon_error_code;
	return FT_OK;
}

int nifalcon_close(falcon_device* dev)
{
	if(!dev->is_open) nifalcon_error_return(NOVINT_DEVICE_NOT_FOUND_ERROR, "tried to close an unopened device");
	dev->is_open = 0;
	FT_Close(dev->falcon);
	return 0;
}

char* nifalcon_get_error_string(falcon_device* dev)
{
	if(dev->falcon_error_code > -NOVINT_DEVICE_NOT_FOUND_ERROR)
	{
		switch (dev->falcon_error_code)
		{
		case FT_OK:
			return "no error";
		case FT_INVALID_HANDLE:
			return "invalid handle";
		case FT_DEVICE_NOT_FOUND:
			return "device not found";
		case FT_DEVICE_NOT_OPENED:
			return "device not opened";
		case FT_IO_ERROR:
			return "io error";
		case FT_INSUFFICIENT_RESOURCES:
			return "insufficient resources";
		case FT_INVALID_PARAMETER:
			return "invalid parameter";
		case FT_INVALID_BAUD_RATE:
			return "invalid baud rate";
		case FT_INVALID_ARGS:
			return "invalid arguments";
		case FT_NOT_SUPPORTED:
			return "not supported";
		case FT_OTHER_ERROR:
		default:
			return "other ftd2xx error";
		}
	}
	return dev->falcon_error_str;	
}
