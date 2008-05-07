/*
 * Declaration file for NovInt Falcon User Space Driver - Kinematics Calculations
 *
 * Copyright (c) 2007-2008 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
 *
 * More info on Nonpolynomial Labs @ http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 */

#ifndef LIBNIFALCON_KINEMATICS_H
#define LIBNIFALCON_KINEMATICS_H

typedef struct point3f {
	float x;
	float y;
	float z;
} point3f;

typedef struct point4f {
	float w;
	float x;
	float y;
	float z;
} point4f;

typedef struct falcon_kinematics {
	
	float f, e, re, rf;

	//thigh angles 
	float thigh_angle[3];
	//knees e1, e2, e3
	point3f knee[3];
	//collisions midpoints e12, e23, e31
	point4f col[3];
	//intersection solutions
	point3f solution[2];
} falcon_kinematics;

#ifdef __cplusplus
extern "C" {
#endif

void nifalcon_init_kinematics(falcon_kinematics* dk);
int nifalcon_direct_kinematics(falcon_kinematics* dk, int encoder1, int encoder2, int encoder3);
int nifalcon_inverse_kinematics(falcon_kinematics* dk, int effector_x, int effector_y, int effector_z);
	
#ifdef __cplusplus
}
#endif
	
#endif
