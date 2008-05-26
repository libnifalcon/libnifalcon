/*
 * Declaration file for NovInt Falcon User Space Driver - Kinematics Calculations
 *
 * Copyright (c) 2007-2008 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
 *
 * More info on Nonpolynomial Labs @ http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * Kinematics algorithms from "Descriptive Geometric Kinematic Analysis of Clavel's Delta Robot", P.J. Zsombor-Murray, McGill University
 * Paper available at http://www.nonpolynomial.com/pdf/claveldelta.pdf
 *
 * This library is covered by the MIT License, read LICENSE for details.
 */

#ifndef LIBNIFALCON_KINEMATICS_H
#define LIBNIFALCON_KINEMATICS_H

/** 3D Point structure for kinematics calculations
 */
typedef struct point3f {
	float x;
	float y;
	float z;
} point3f;

/** 4D Point structure for homogenous kinematics calculations
 */
typedef struct point4f {
	float w;
	float x;
	float y;
	float z;
} point4f;

/** Falcon Kinematics Structure
	Holds the physical characteristics of the delta robot (in this case, the Falcon)
 */
typedef struct falcon_kinematics {
	
	float f;  /*!< Fixed Frame edge length */
	float e; /*!< End Effector edge length */
	float re; /*!< End Effector radius (shin length) */
	float rf; /*!< Fixed Frame radius (thigh length) */

	int thigh_encoder[3];  /*!< Encoder position needed to achieve the angles in thigh_angle */
	float thigh_angle[3]; /*!< Angle of the knee in relation to the fixed frame */
	point3f knee[3]; /*!< Point in 3-space of the knee */
	point4f col[3]; /*!< Collision midpoints of the knee spheres */
	point3f solution[2]; /*!< Two possibly solutions for the 3 sphere collision problem */
} falcon_kinematics;

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * Initializes a kinematics packet with the default values for the falcon
 *
 * @param dk A kinematics structure
 */
	void nifalcon_init_kinematics(falcon_kinematics* dk);

/** 
 * Given the offset angles from the 3 thighs, returns the position in 3-space of the end effector
 *
 * @param dk A kinematics structure
 * @param encoder1 Encoder for the top motor
 * @param encoder2 Encoder for the right motor
 * @param encoder3 Encoder for the left motor
 *
 * @return 0 on success, < 0 on error. Solution is stored in the solution portion of the passed struct
 */

	int nifalcon_direct_kinematics_angle(falcon_kinematics* dk, float angle1, float angle2, float angle3);

/** 
 * Convenience function. Converts encoder ticks to angle, then calls nifalcon_direct_kinematics_angle
 *
 * @param dk A kinematics structure
 * @param encoder1 Encoder for the top motor
 * @param encoder2 Encoder for the right motor
 * @param encoder3 Encoder for the right motor
 *
 * @return 0 on success, < 0 on error. Solution is stored in the solution portion of the passed struct
 */
	int nifalcon_direct_kinematics_encoder(falcon_kinematics* dk, int encoder1, int encoder2, int encoder3);

	
/** 
 * Given the position in 3-space of the end effector, gives the thigh angles needed to achieve the position
 *
 * @param dk A kinematics structure
 * @param effector_x X coordinate of the end effector
 * @param effector_y Y coordinate of the end effector
 * @param effector_z Z coordinate of the end effector
 *
 * @return 0 on success, < 0 on error. Solution is stored in the thigh_angle portion of the passed struct
 */

	int nifalcon_inverse_kinematics_angle(falcon_kinematics* dk, float effector_x, float effector_y, float effector_z);

/** 
 * Convenience function. Calls nifalcon_inverse_kinematics_angle, then does the encoder conversion.
 *
 * @param dk A kinematics structure
 * @param effector_x X coordinate of the end effector
 * @param effector_y Y coordinate of the end effector
 * @param effector_z Z coordinate of the end effector
 *
 * @return 0 on success, < 0 on error. Solution is stored in the thigh_angle portion of the passed struct
 */

	int nifalcon_inverse_kinematics_encoder(falcon_kinematics* dk, float effector_x, float effector_y, float effector_z);
	
#ifdef __cplusplus
}
#endif
	
#endif
