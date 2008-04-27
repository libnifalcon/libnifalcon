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
#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define PACKET_TIMEOUT 1000
#define ENABLE_DEBUG_LOGGING 1

falcon_device dev;
libusb_device_handle* hnd;
static struct libusb_transfer* in_transfer;
static struct libusb_transfer* out_transfer[10];
unsigned char input[16] = "<AAAAAAAAAAAAAA>";
unsigned char output[10][64]; //= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};	

int do_exit = 0;
int out_count = 0;
int in_done = 1;
int out_done = 1;
int in_count = 0;
int error_count = 0;
int p = 0;
int q = 0;


static struct timeval _tstart[10], _tend[10];
static struct timezone tz;
void tstart(int i)
{
	gettimeofday(&_tstart[i], &tz);
}

void tend(int i)
{
	gettimeofday(&_tend[i],&tz);
}

double tval(int i)
{
	double t1, t2;
	t1 =  (double)_tstart[i].tv_sec + (double)_tstart[i].tv_usec/(1000*1000);
	t2 =  (double)_tend[i].tv_sec + (double)_tend[i].tv_usec/(1000*1000);
	return t2-t1;
}


void sigproc()
{
	printf("closing falcon and quitting\n");
	do_exit = 1;
	nifalcon_close(&dev);
	exit(0);
}

static void cb_in(struct libusb_transfer *transfer)
{
//	printf("in done!\n");
//	++in_count;
	in_done = 1;

//	tend(3);
//	printf("in time %f\n", tval(3));
//	tstart(2);
			libusb_submit_transfer(out_transfer[0]);
/*			++p;   
			if(p > 9)
			{
				p = 0;
			}

*/
//	printf("submitting transfer %d\n", p);
}

static void cb_out(struct libusb_transfer *transfer)
{
//x	printf("out %d\n", transfer->user_data);	
//	printf("out done!\n");
//	tend(2);
//	printf("Out time %f\n", tval(2));
	out_done = 1;
//	if(transfer->actual_length > 0) ++out_count;
//	else
//	{
//		ftdi_usb_purge_tx_buffer(&dev.falcon);
//		++error_count;;
//	}
//	libusb_submit_transfer(out_transfer[(int)transfer->user_data]);
	/*
	for(q = 0; q < transfer->actual_length; ++q)
	{
		printf("0x%x ", transfer->buffer[q]);
	}
	printf("\n");
	*/

}


int main(int argc, char** argv)
{
	int num_falcons, status, i, r;
	unsigned int count;
	falcon_packet input_packet, output_packet;
	struct timeval tv_current;
	

//	out_transfer = (struct libusb_transfer**)malloc(sizeof(struct libusb_transfer)*10);
	nifalcon_init(&dev);
	
	num_falcons = nifalcon_get_count(&dev);
	
	count = 0;

	printf("Falcons found: %d\n", num_falcons);

	printf("Opening falcon\n");
	if((status = nifalcon_open(&dev, 0)) < 0)
	{
		printf ("Cannot open falcon! Error: %s\n", nifalcon_get_error_string(&dev));
		return 1;
	}
	printf("Opened falcon\n");
	printf("Loading firmware\n");
	for(i = 0; i < 5; ++i)
	{
		if((status = nifalcon_load_firmware(&dev, "test_firmware.bin")) == 0) break;
		printf("Firmware not loaded! Error: %d %s\nRetrying...\n", dev.falcon_status_code, nifalcon_get_error_string(&dev));
	}
	if(i == 5)
	{
		printf("Cannot load firmware! Bailing...\n");
		return 1;
	}
	printf("Firmware loaded\n");


	ftdi_usb_purge_buffers(&dev.falcon);
	
	hnd = dev.falcon.usb_dev;

	in_transfer = libusb_alloc_transfer(0);
	if (!in_transfer)
	{
		printf("Cannot allocate in transfer\n");
		return -ENOMEM;
	}

	libusb_fill_bulk_transfer(in_transfer, hnd, dev.falcon.in_ep, input,
							  sizeof(input), cb_in, NULL, 0);

	for(q = 0; q < 10; ++q)
	{
		out_transfer[q] = libusb_alloc_transfer(0);
		if (!out_transfer[q])
		{
			printf("Cannot allocate out transfer\n");
			return -ENOMEM;
			}
		libusb_fill_bulk_transfer(out_transfer[q], hnd, dev.falcon.out_ep, output[q],
							  64, cb_out, q, 1000);
		libusb_submit_transfer(out_transfer[q]);
	}

	signal(SIGINT, sigproc);
	signal(SIGQUIT, sigproc);
//	libusb_submit_transfer(in_transfer);
//	tstart(1);
	while(1)
	{
//		tstart(4);
//		tstart(5);
		if(in_done && out_done && !do_exit)
		{
//			if(in_count <= out_count+2)
			{
				libusb_submit_transfer(in_transfer);
			}
//			out_done = 0;
//			printf("submitting %d\n", p);
//			tstart(3);
			in_done = out_done = 0;
		}
//		tend(5);
/*		if(out_done && !do_exit)
		{
		}*/

//		tend(1);
//		if(tval(1) > 1.0)
		{
//			tstart(1);
//			printf("I: %d O: %d E: %d\n", in_count, out_count, error_count);
//			in_count = out_count = error_count = 0;
		}
//		tstart(6);
		r = libusb_handle_events();
		if (r < 0)
			break;
//		tend(6);
//		tend(4);
//		printf("handle time: %f %f %f\n", tval(4), tval(5), tval(6));
	}
	nifalcon_close(&dev);
	return 0;
}
