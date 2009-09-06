/***
 * @file nifalcon_libftdi.c
 * @brief Implementation file for C version of libnifalcon driver - libftdi based
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

#include "nifalcon.h"
#include "usb.h"
#include <time.h>
#include <stdio.h>
#define MAX_DEVICES 128

//Ripped from libftdi, so we can standardize how we return errors
#define nifalcon_error_return(code, str) do {	   \
		dev->falcon_status_str = str;			   \
        dev->falcon_status_code = -code;			   \
        return -code;							   \
   } while(0);

int nifalcon_init(falcon_device* dev)
{
	if((dev->falcon_status_code = ftdi_init(&(dev->falcon))) < 0) return dev->falcon_status_code;
	dev->is_open = 0;
	dev->falcon_status_str = "";
	dev->is_initialized = 1;
	return dev->falcon_status_code;
}

//Sets the USB timeout for reads then reads
int nifalcon_read(falcon_device* dev, unsigned char* str, unsigned int size, unsigned int timeout_ms)
{
	unsigned long bytes_rx, bytes_read = 0;
	clock_t timeout;

	if(!dev->is_initialized) nifalcon_error_return(NIFALCON_DEVICE_NOT_VALID_ERROR, "tried to read from an uninitialized device");
	if(!dev->is_open) nifalcon_error_return(NIFALCON_DEVICE_NOT_FOUND_ERROR, "tried to read from an unopened device");
	(dev->falcon).usb_read_timeout = timeout_ms;
	dev->falcon_status_code = ftdi_read_data(&(dev->falcon), str, size);
	return dev->falcon_status_code;
}

int nifalcon_write(falcon_device* dev, unsigned char* str, unsigned int size)
{
	if(!dev->is_initialized) nifalcon_error_return(NIFALCON_DEVICE_NOT_VALID_ERROR, "tried to write to an uninitialized device");
	if(!dev->is_open) nifalcon_error_return(NIFALCON_DEVICE_NOT_FOUND_ERROR, "tried to write to an unopened device");
	dev->falcon_status_code = ftdi_write_data(&(dev->falcon), str, size);
	return dev->falcon_status_code;
}

int nifalcon_get_count(falcon_device* dev)
{
	int count;
	struct ftdi_device_list* dev_list[128];
	if(!dev->is_initialized) nifalcon_error_return(NIFALCON_DEVICE_NOT_VALID_ERROR, "tried to get count on an uninitialized device");
	count = ftdi_usb_find_all(&(dev->falcon), dev_list, NIFALCON_VENDOR_ID, NIFALCON_PRODUCT_ID);
	ftdi_list_free(dev_list);
	return count;
}

int nifalcon_open(falcon_device* dev, unsigned int device_index)
{
	unsigned int count, i, status;
	struct ftdi_device_list *dev_list, *current;
	if(!dev->is_initialized) nifalcon_error_return(NIFALCON_DEVICE_NOT_VALID_ERROR, "tried to open an uninitialized device");
	if(dev->is_open) nifalcon_close(dev);

	count = ftdi_usb_find_all(&(dev->falcon), &dev_list, NIFALCON_VENDOR_ID, NIFALCON_PRODUCT_ID);
	if(count <= 0 || device_index > count)
	{
		ftdi_list_free(&dev_list);
		if(count == 0) nifalcon_error_return(NIFALCON_DEVICE_NOT_FOUND_ERROR, "no devices connected to system");
		nifalcon_error_return(NIFALCON_DEVICE_INDEX_OUT_OF_RANGE_ERROR, "device index out of range");
	}
	for(i = 0, current = dev_list; current != NULL && i < device_index; current = dev_list->next, ++i);
	if((dev->falcon_status_code = ftdi_usb_open_dev(&(dev->falcon), current->dev)) < 0) return dev->falcon_status_code;
	ftdi_list_free(&dev_list);
	//VERY IMPORTANT
	//If we do not reset latency to 1ms, then we either have to fill the FTDI butter (64bytes) or wait 16ms
	//to get any data back. This is what was causing massive slowness in pre-1.0 releases
	if((dev->falcon_status_code = ftdi_set_latency_timer(&(dev->falcon), 1)) < 0) return dev->falcon_status_code;

	//Shift to full speed
	if((dev->falcon_status_code = ftdi_set_baudrate(&(dev->falcon), 1456312)) < 0) return dev->falcon_status_code;

	dev->is_open = 1;
	return 0;
}

int nifalcon_close(falcon_device* dev)
{
	if(!dev->is_initialized) nifalcon_error_return(NIFALCON_DEVICE_NOT_VALID_ERROR, "tried to close an uninitialized device");
	if(!dev->is_open) nifalcon_error_return(NIFALCON_DEVICE_NOT_FOUND_ERROR, "tried to close an unopened device");
	if((dev->falcon_status_code = ftdi_usb_close(&(dev->falcon))) < 0) return dev->falcon_status_code;
	dev->is_open = 0;
	return 0;
}

int nifalcon_load_firmware(falcon_device* dev, const char* firmware_filename)
{
	unsigned int bytes_written, bytes_read;
	unsigned char check_msg_1_send[3] = {0x0a, 0x43, 0x0d};
	unsigned char check_msg_1_recv[4] = {0x0a, 0x44, 0x2c, 0x0d};
	unsigned char check_msg_2[1] = {0x41};
	unsigned char send_buf[128], receive_buf[128];
	FILE* firmware_file;
	int k;
	if(!dev->is_initialized) nifalcon_error_return(NIFALCON_DEVICE_NOT_VALID_ERROR, "tried to load firmware on an uninitialized device");
	if(!dev->is_open) nifalcon_error_return(NIFALCON_DEVICE_NOT_FOUND_ERROR, "tried to load firmware on an unopened device");
  
	//Clear out current buffers to make sure we have a fresh start
	if((dev->falcon_status_code = ftdi_usb_purge_buffers(&(dev->falcon))) < 0) return dev->falcon_status_code;

	//Reset the device
	if((dev->falcon_status_code = ftdi_usb_reset(&(dev->falcon))) < 0) return dev->falcon_status_code;

	//Make sure our latency timer is at 16ms, otherwise firmware checks tend to always fail
	if((dev->falcon_status_code = ftdi_set_latency_timer(&(dev->falcon), 16)) < 0) return dev->falcon_status_code;
	
	//Set to:
	// 9600 baud
	// 8n1
	// No Flow Control
	// RTS Low
	// DTR High	
	if((dev->falcon_status_code = ftdi_set_baudrate(&(dev->falcon), 9600)) < 0) return dev->falcon_status_code;
	if((dev->falcon_status_code = ftdi_set_line_property(&(dev->falcon), BITS_8, STOP_BIT_1, NONE)) < 0) return dev->falcon_status_code;
	if((dev->falcon_status_code = ftdi_setflowctrl(&(dev->falcon), SIO_DISABLE_FLOW_CTRL)) < 0) return dev->falcon_status_code;
	if((dev->falcon_status_code = ftdi_setrts(&(dev->falcon), 0)) < 0) return dev->falcon_status_code;
	if((dev->falcon_status_code = ftdi_setdtr(&(dev->falcon), 0)) < 0) return dev->falcon_status_code;
	if((dev->falcon_status_code = ftdi_setdtr(&(dev->falcon), 1)) < 0) return dev->falcon_status_code;
	//Send 3 bytes: 0x0a 0x43 0x0d

	if((dev->falcon_status_code = nifalcon_write(dev, check_msg_1_send, 3)) < 0) return dev->falcon_status_code;
	if((dev->falcon_status_code = nifalcon_read(dev, receive_buf, 4, 1000)) < 0) return dev->falcon_status_code;
	
	//Set to:
	// DTR Low
	// 140000 baud (0x15 clock ticks per signal)
	if((dev->falcon_status_code = ftdi_setdtr(&(dev->falcon),0)) < 0) return dev->falcon_status_code;
	if((dev->falcon_status_code = ftdi_set_baudrate(&(dev->falcon), 140000)) < 0) return dev->falcon_status_code;

	//Send "A" character
	if((dev->falcon_status_code = nifalcon_write(dev, check_msg_2, 1)) < 0) return dev->falcon_status_code;

	//Expect back 2 bytes:
	// 0x13 0x41
	if((dev->falcon_status_code = nifalcon_read(dev, receive_buf, 2, 1000)) < 0) return dev->falcon_status_code;
	
	firmware_file = fopen(firmware_filename, "rb");

	if(!firmware_file)
	{
		nifalcon_error_return(NIFALCON_FIRMWARE_NOT_FOUND_ERROR, "cannot find falcon firmware file");
	}

	while(!feof(firmware_file))
	{
		int firmware_bytes_read;
		int i;
		firmware_bytes_read = fread(send_buf, 1, 128, firmware_file);
		if((dev->falcon_status_code = nifalcon_write(dev, send_buf, firmware_bytes_read)) < 0) return dev->falcon_status_code;
		if((dev->falcon_status_code = nifalcon_read(dev, receive_buf, firmware_bytes_read, 1000)) < firmware_bytes_read)
		{			
			nifalcon_error_return(NIFALCON_FIRMWARE_CHECKSUM_ERROR, "error sending firmware (firmware send step, 128 byte reply not received)");
		}
		for(i = 0; i < firmware_bytes_read; ++i)
		{
			if(send_buf[i] != receive_buf[i])
			{
				nifalcon_error_return(NIFALCON_FIRMWARE_CHECKSUM_ERROR, "error sending firmware (firmware send step, checksum does not match)");
			}
		}
		if(firmware_bytes_read < 128) break;
	}
	fclose(firmware_file);

	//VERY IMPORTANT
	//If we do not reset latency to 1ms, then we either have to fill the FTDI butter (64bytes) or wait 16ms
	//to get any data back. This is what was causing massive slowness in pre-1.0 releases
	if((dev->falcon_status_code = ftdi_set_latency_timer(&(dev->falcon), 1)) < 0) return dev->falcon_status_code;

	//Shift to full speed
	if((dev->falcon_status_code = ftdi_set_baudrate(&(dev->falcon), 1456312)) < 0) return dev->falcon_status_code;

	return 0;
}

char* nifalcon_get_error_string(falcon_device* dev)
{
	if(dev->falcon_status_code > -NIFALCON_DEVICE_NOT_FOUND_ERROR)
	{
		return dev->falcon.error_str;
	}
	return dev->falcon_status_str;	
}
