/*
 * Implementation file for NovInt Falcon Kinematics Test Program
 *
 * Copyright (c) 2008 Kyle Machulis <kyle@nonpolynomial.com> http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 *
 */

#include "nifalcon_test_fw.h"
#include "nifalcon_kinematics.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int fps, lps, iops, errps, current_error;

falcon_device dev;
falcon_kinematics dk;
falcon_packet input_packet, output_packet;

void sigproc()
{
	printf("closing falcon and quitting\n");
	nifalcon_close(&dev);
	exit(0);
}

int InitFalcon()
{
	int num_falcons = 0, status = 0, i = 0;
	nifalcon_test_fw_init_packet(&input_packet);
	nifalcon_test_fw_init_packet(&output_packet);

	nifalcon_init(&dev);
	
	num_falcons = nifalcon_get_count(&dev);
	
	printf("Falcons found: %d\n", num_falcons);

	printf("Opening falcon\n");
	if((status = nifalcon_open(&dev, 0)) < 0)
	{
		printf ("Cannot open falcon! Error: %s\n", nifalcon_get_error_string(&dev));
		return status;
	}
	printf("Opened falcon\n");
	printf("Loading firmware\n");
	for(i = 0; i < 10; ++i)
	{
		if((status = nifalcon_load_firmware(&dev, "test_firmware.bin")) == 0)
		{
			break;
		}
		printf("Firmware not loaded! Error: %d %s\n", dev.falcon_status_code, nifalcon_get_error_string(&dev));
	}
	if(i==10)
	{
		printf("Cannot load firmware, bailing...");
		return status;
	}
	printf("Firmware loaded\n");
	return 0;
}

int RunFalconIO(void)
{
	int status;
	++iops;

	/******** Falcon I/O section ********/

	input_packet.info |= NOVINT_TEST_FW_HOMING_MODE;
	if(!(output_packet.info & (NOVINT_TEST_FW_HOMED_AXIS1 | NOVINT_TEST_FW_HOMED_AXIS2 | NOVINT_TEST_FW_HOMED_AXIS3)))
	{
		input_packet.info &= ~NOVINT_TEST_FW_LED_GREEN;
		input_packet.info |= NOVINT_TEST_FW_LED_RED;
	}
	else
	{
		input_packet.info &= ~NOVINT_TEST_FW_LED_RED;
		input_packet.info |= NOVINT_TEST_FW_LED_GREEN;
	}

	if((status = nifalcon_test_fw_send_struct(&dev, &input_packet)) < 0)
	{
		++errps;
		current_error = 1;
		return 0;
	}
	if((status = nifalcon_test_fw_receive_struct(&dev, &output_packet, 0)) < 0)
	{
		++errps;
		current_error = 1;
		return 0;
	}
	return 1;
}

int main(int argc, char** argv)
{

	int num_falcons, status, i;
	unsigned int count;
	nifalcon_test_fw_init_packet(&input_packet);
	nifalcon_test_fw_init_packet(&output_packet);

	/*
	if(InitFalcon() != 0)
	{
		printf("Cannot initialize falcon, returning...\n");
		return 1;
	}
	*/
	//nifalcon_init_kinematics(&dk);
	dk.e = 8;
	dk.f = 16;
	dk.re = 10.3094;
	dk.rf = 8;

	//Let's start with inverse kinematics
    nifalcon_inverse_kinematics_angle(&dk, 2.0, 3.0, -8.7488);
	printf("Inverse: %f %f %f resolves to %f %f %f\n", 2.0, 3.0, -8.7488, dk.thigh_angle[0], dk.thigh_angle[1], dk.thigh_angle[2]);
	
	//Then undo those back to direct
    nifalcon_direct_kinematics_angle(&dk, 55.19, 9.5, 37.8);
	printf("Direct: %f %f %f resolves to %f %f %f or %f %f %f\n", 55.19, 9.5, 37.8, dk.solution[0].x, dk.solution[0].y, dk.solution[0].z, dk.solution[1].x, dk.solution[1].y, dk.solution[1].z);
	
	return 0;	
}
