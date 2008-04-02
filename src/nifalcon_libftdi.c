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
#include "usb.h"
#include <time.h>
#include <stdio.h>
#define MAX_DEVICES 128

//Ripped from libftdi, so we can standardize how we return errors
#define nifalcon_error_return(code, str) do {  \
        dev->falcon_error_str = str;             \
        return -code;                       \
   } while(0);

int nifalcon_init(falcon_device* dev)
{
	(dev->falcon) = (falcon_handle)malloc(sizeof(falcon_handle));
	return ftdi_init(dev->falcon);
}

//Sets the USB timeout for reads then reads
int nifalcon_read(falcon_device* dev, unsigned char* str, unsigned int size, unsigned int timeout_ms)
{
	int bytes_read;
	(dev->falcon)->usb_read_timeout = timeout_ms;
	
	bytes_read = ftdi_read_data(dev->falcon, str, size);
	if(bytes_read >= 0) return 0;
	return (bytes_read);	
}

int nifalcon_write(falcon_device* dev, unsigned char* str, unsigned int size)
{
	int bytes_written;
	bytes_written = ftdi_write_data(dev->falcon, str, size);
	if(bytes_written >= 0) return 0;
	return bytes_written;
}

int nifalcon_get_count(falcon_device* dev)
{
	int count;
	struct ftdi_device_list* dev_list[128];
	count = ftdi_usb_find_all(dev->falcon, dev_list, NOVINT_FALCON_VENDOR_ID, NOVINT_FALCON_PRODUCT_ID);
	ftdi_list_free(dev_list);
	return count;
}

int nifalcon_open(falcon_device* dev, unsigned int device_index)
{
	unsigned int count, i, status, ret;
	struct ftdi_device_list *dev_list, *current;

	count = ftdi_usb_find_all(dev->falcon, &dev_list, NOVINT_FALCON_VENDOR_ID, NOVINT_FALCON_PRODUCT_ID);
	if(count <= 0 || device_index > count)
	{
		ftdi_list_free(&dev_list);
		if(count == 0) nifalcon_error_return(NOVINT_DEVICE_NOT_FOUND_ERROR, "No devices connected to system");
		return count;
	}
	for(i = 0, current = dev_list; current != NULL && i < device_index; current = dev_list->next, ++i);
	ret = ftdi_usb_open_dev(dev->falcon, current->dev);
	ftdi_list_free(&dev_list);
	return ret;
}

int nifalcon_close(falcon_device* dev)
{
	if(!dev->falcon) nifalcon_error_return(NOVINT_DEVICE_NOT_VALID_ERROR, "tried to close an invalid falcon device");
	ftdi_usb_close(dev->falcon);
	ftdi_deinit(dev->falcon);
	return 0;
}

int nifalcon_load_firmware(falcon_device* dev, const char* firmware_filename)
{
	unsigned int bytes_written, bytes_read;
	unsigned char check_msg_1[3] = {0x0a, 0x43, 0x0d};
	unsigned char check_msg_2[1] = "A";
	unsigned char check_buf[128];
	int status;
	FILE* firmware_file;
	if((status = ftdi_usb_reset(dev->falcon)) < 0) return status;
	//Set to:
	// 9600 baud
	// 8n1
	// No Flow Control
	// RTS Low
	// DTR High	
	if((status = ftdi_set_baudrate(dev->falcon, 9600)) < 0) return status;
	if((status = ftdi_set_line_property(dev->falcon, BITS_8, STOP_BIT_1, NONE)) < 0) return status;
	if((status = ftdi_setflowctrl(dev->falcon, SIO_DISABLE_FLOW_CTRL)) < 0) return status;
	if((status = ftdi_setrts(dev->falcon, 0)) < 0) return status;
	if((status = ftdi_setdtr(dev->falcon, 0)) < 0) return status;
	if((status = ftdi_setdtr(dev->falcon, 1)) < 0) return status;

	//Send 3 bytes: 0x0a 0x43 0x0d
	if((status = nifalcon_write(dev, check_msg_1, 3)) < 0) return status;

	//Expect 5 bytes back
	if((status = nifalcon_read(dev, check_buf, 4, 1000)) < 0) return status;	

	//Set to:
	// DTR Low
	// 140000 baud (0x15 clock ticks per signal)
	if((status = ftdi_setdtr(dev->falcon,0)) < 0) return status;
	if((status = ftdi_set_baudrate(dev->falcon, 140000)) < 0) return status;
	if((status = ftdi_usb_purge_buffers(dev->falcon)) < 0) return status;

	//Send "A" character
	if((status = nifalcon_write(dev, check_msg_2, 1)) < 0) return status;

	//Expect back 1 byte:
	// 0x41 ("A")
	if((status = nifalcon_read(dev, check_buf, 1, 1000)) < 0) return status;	

	firmware_file = fopen(firmware_filename, "rb");

	if(!firmware_file)
	{
		nifalcon_error_return(NOVINT_FIRMWARE_NOT_FOUND_ERROR, "cannot find falcon firmware file");
	}

	while(!feof(firmware_file))
	{
		int firmware_bytes_read;
		firmware_bytes_read = fread(check_buf, 1, 128, firmware_file);
		if((status = nifalcon_write(dev, check_buf, firmware_bytes_read)) < 0) return status;
		if((status = nifalcon_read(dev, check_buf, firmware_bytes_read, 1000)) < 0) return status;	
		if(firmware_bytes_read < 128) break;
	}
	fclose(firmware_file);

	if((status = ftdi_set_baudrate(dev->falcon, 1456312)) < 0) return status;

	return 0;
}

