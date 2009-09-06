/***
 * @file StamperUtils.h
 * @brief Enums and structs used in the Stamper kinematics implementation
 * @author Kevin Ouellet (kouellet@users.sourceforge.net)/Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef STAMPERUTILS_H_
#define STAMPERUTILS_H_

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{
		/**
		 * Enumeration for joints. Looking at the falcon from the front, goes from top, clockwise
		 */
		enum arm_id
		{
			ARM_1 = 0, ARM_2, ARM_3
		};

		/**
		 * Enumeration for joint angles.
		 * THETA_1 is thigh angle (connecting the leg to the device),
		 * THETA_2 is knee angle (connecting thigh to shin)
		 * THETA_3 is shin angle (connecting shin to end effector)
		 */
		enum theta_id
		{
			THETA_1 = 0, THETA_2, THETA_3
		};

		/**
		 * Enumeration for cartesian coordinates
		 */
		enum coord
		{
			X = 0, Y, Z
		};

		/**
		 * Structure for storing Euler angles of a single leg
		 */		
		struct Angle
		{
			float theta1[3]; /**< Euler for thigh angle */
			float theta2[3]; /**< Euler for knee angle */
			float theta3[3]; /**< Euler for shin angle */
		};
	}
}
#endif
