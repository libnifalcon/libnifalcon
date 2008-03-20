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

int gInit = 0;

int nifalcon_init(falcon_device* dev)
{
	*dev = (falcon_device)malloc(sizeof(struct ftdi_context));
	return ftdi_init(*dev);
}

//Just wraps the FT_Read function in a QueueStatus call so we don't block on reads
int nifalcon_read_wait(falcon_device dev, char* str, unsigned int size, unsigned int timeout_ms, unsigned int* bytes_read)
{
	unsigned int o, bytes_rx, bytes_written;
	(*dev).usb_read_timeout = timeout_ms;
	
	*bytes_read = ftdi_read_data(dev, str, size);
	if(*bytes_read == size) return 0;
	return -(*bytes_read);	
}

int nifalcon_write(falcon_device dev, char* str, unsigned int size, unsigned int* bytes_written)
{
	*bytes_written = ftdi_write_data(dev, str, size);
	if(*bytes_written == size) return 0;
	return -(*bytes_written);	
}

int nifalcon_get_count(falcon_device dev)
{
	int count;
	struct ftdi_device_list* dev_list[128];
	count = ftdi_usb_find_all(dev, dev_list, NOVINT_FALCON_VENDOR_ID, NOVINT_FALCON_PRODUCT_ID);
	ftdi_list_free(dev_list);
	return count;
}

int nifalcon_open(falcon_device* dev, unsigned int device_index)
{
	unsigned int count, i, status, ret;
	struct ftdi_device_list *dev_list, *current;

	count = ftdi_usb_find_all(*dev, &dev_list, NOVINT_FALCON_VENDOR_ID, NOVINT_FALCON_PRODUCT_ID);
	for(i = 0, current = dev_list; current != NULL && i < device_index; current = dev_list->next, ++i);	
	if((ret = ftdi_usb_open_dev(*dev, current->dev)) < 0) {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(*dev));
		ftdi_list_free(&dev_list);
        return EXIT_FAILURE;
    }
	ftdi_list_free(&dev_list);
	return 0;
}

int nifalcon_close(falcon_device dev)
{
	if(!dev) return -1;
	ftdi_usb_close(dev);
	ftdi_deinit(dev);
	return 0;
}

int nifalcon_load_firmware(falcon_device dev, const char* firmware_filename)
{
	unsigned int bytes_written, bytes_read;
	char check_msg_1[3] = {0x0a, 0x43, 0x0d};
	char check_buf[128];
	int status;
	FILE* firmware_file;

	printf("Resetting\n");
	status = ftdi_usb_reset(dev);
	if(status < 0) return status;

	printf("Setting up\n");
	//Set to:
	// 9600 baud
	// 8n1
	// No Flow Control
	// RTS Low
	// DTR High	
	if((status = ftdi_set_baudrate(dev, 9600)) < 0) return status;
	if((status = ftdi_set_line_property(dev, BITS_8, STOP_BIT_1, NONE)) < 0) return status;
	if((status = ftdi_setflowctrl(dev, SIO_DISABLE_FLOW_CTRL)) < 0) return status;
	if((status = ftdi_setrts(dev, 0)) < 0) return status;
	if((status = ftdi_setdtr(dev, 0)) < 0) return status;
	if((status = ftdi_setdtr(dev, 1)) < 0) return status;

	//Send 3 bytes: 0x0a 0x43 0x0d
	printf("Wrote 3 bytes init\n");
	if((bytes_written = ftdi_write_data(dev, check_msg_1, 3)) != 3) return bytes_written;
	
	//Expect 5 bytes back
	if((status = nifalcon_read_wait(dev, check_buf, 4, 1000, &bytes_read)) < 0) return status;	
	printf("Got back confirmation\n");

	//Set to:
	// DTR Low
	// 140000 baud (0x15 clock ticks per signal)
	if((status = ftdi_setdtr(dev,0)) < 0) return status;
	if((status = ftdi_set_baudrate(dev, 140000)) < 0) return status;
	if((status = ftdi_usb_purge_buffers(dev)) < 0) return status;

	//Send "A" character
	if((status = ftdi_write_data(dev, "A", 1)) < 1) return status;

	//Expect back 1 byte:
	// 0x41 ("A")
	if((status = nifalcon_read_wait(dev, check_buf, 1, 1000, &bytes_read)) < 0) return status;	
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
		if((status = ftdi_write_data(dev, check_buf, firmware_bytes_read)) != firmware_bytes_read) return status;
		if((status = nifalcon_read_wait(dev, check_buf, firmware_bytes_read, 1000, &bytes_read)) < 0) return status;	
		if(firmware_bytes_read < 128) break;
	}
	fclose(firmware_file);

	if((status = ftdi_set_baudrate(dev, 1456312)) < 0) return status;
	return 0;

}
