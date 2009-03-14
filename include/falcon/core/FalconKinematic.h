/***
 * @file FalconKinematic.h
 * @brief Base class for falcon kinematic classes 
 * @author Kyle Machulis (kyle@nonpolynomial.com) / Kevin Ouellet (kouellet@users.sourceforge.net)
 * @version $Id$
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL$
 * 
 * Project info at http://libnifalcon.sourceforge.net/ 
 *
 */
#ifndef FALCONKINEMATIC_H
#define FALCONKINEMATIC_H

#include <stdint.h>
#include <iostream>

#include "falcon/core/FalconCore.h"
#include "falcon/core/FalconGeometry.h"

namespace libnifalcon
{
	class FalconKinematic : public FalconCore
	{
	public:
		enum {
			FALCON_KINEMATIC_OUT_OF_RANGE = 5000 /**< Returned if value requested is out of workspace range */
		};

		/** 
		 * Constructor
		 * 
		 * 
		 */
		FalconKinematic() {}

		/** 
		 * Destructor
		 * 
		 * 
		 */
		virtual ~FalconKinematic() {}

		/** 
		 * Given an encoder value, return the angle (in degrees) the leg is at (in the leg's local reference frame)
		 * 
		 * @param encoder_value Encoder ticks for a leg
		 * 
		 * @return Angle (in degrees) the leg is at
		 */
		double getTheta(int16_t encoder_value)
		{
			return (((SHAFT_DIAMETER*PI) / (WHEEL_SLOTS_NUMBER*4)) * (encoder_value))/((PI*SMALL_ARM_DIAMETER)/360.0f) + THETA_OFFSET_ANGLE;
		}

		/** 
		 * Given a caretesian position (in meters), return the angle of the legs requires to achieve the positions
		 *
		 * @param position Position to get the angles for (in cartesian coordinates, meters)
		 * @param angles Array to write result into
		 *
		 * @return true if angles are found, false otherwise (i.e. position out of workspace range)
		 */
		virtual bool getAngles(const double (&position)[3], double (&angles)[3]) = 0;

		/** 
		 * Given a set of encoder values, return the cartesian position (in meters) of the end effector in relation to the origin.
		 * Note: Origin subject to change based on kinematics system. Use the workspaceOrigin() function to get what the system thinks
		 * its origin is.
		 *
		 * @param encoders Encoder values for the 3 legs
		 * @param position Array to write result into
		 *
		 * @return true if angles are found, false otherwise (i.e. position out of workspace range)
		 */
		virtual bool getPosition(const int16_t (&encoders)[3], double (&position)[3]) = 0;

		/** 
		 * Returns the center point of the workspace. May not always be [0,0,0].
		 *
		 * @param origin Array to store values in
		 */
		virtual void getWorkspaceOrigin(double (&origin)[3]) = 0;

		/** 
		 * Given a caretesian position (in meters), and force vector (in newtons),
		 * return the force values that need to be sent to the firmware. Force values are capped at 4096.
		 *
		 * @param position Current position of the end effector
		 * @param cart_force Force vector to apply to the end effector
		 * @param enc_force Force to be sent to the firmware
		 *
		 * @return true if forces are generated, false otherwise.
		 */

		virtual bool getForces(const double (&position)[3], const double (&cart_force)[3], int16_t (&enc_force)[3]) = 0;
	};
}

#endif
