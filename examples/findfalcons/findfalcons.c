/*
 * Implementation file for NovInt Falcon FindFalcon Example Program
 *
 * Copyright (c) 2007 Kyle Machulis <kyle@nonpolynomial.com> http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the LGPL, read LICENSE for details.
 */

#include "nifalcon.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	int num_falcons = nifalcon_get_count();
	char input[17] = "<AAAAAAAAAAAAAA>";
	char output[17];
	falcon_packet input_packet, output_packet;
	falcon_device dev;
	
	input[16] = output[16] = 0;

	printf("Falcons found: %d\n", num_falcons);
	
	if(nifalcon_open(&dev, 0) != FT_OK) return 1;
	if(nifalcon_init(dev) != FT_OK) return 1;
	
	if(nifalcon_send_raw(dev, &input) != FT_OK) return 1;
	printf("Send Raw: %s\n", input);
	if(nifalcon_receive_raw(dev, &output) != FT_OK) return 1;
	printf("Receive Raw: %s\n", output);
	nifalcon_init_packet(&input_packet);
	nifalcon_init_packet(&output_packet);
	if(nifalcon_send_struct(dev, &input_packet) != FT_OK) return 1;
	if(nifalcon_receive_struct(dev, &output_packet) != FT_OK) return 1;

	nifalcon_close(dev);
	return 0;
}