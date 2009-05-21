/***
 * @file FalconKinematicStamper.h
 * @brief IK and DK implementation for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net) / Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.sourceforge.net/
 *
 */

#ifndef FALCONKINEMATICZSOMBORMURRAY_H
#define FALCONKINEMATICZSOMBORMURRAY_H

#include "falcon/core/FalconKinematic.h"

namespace libnifalcon
{

	class FalconKinematicZsomborMurray : public FalconKinematic
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param init_now If true, runs initialize() function on construction (can block). Defaults to true.
		 *
		 * @return
		 */
		FalconKinematicZsomborMurray(bool init_now = true);

		/**
		 * Destructor
		 *
		 *
		 * @return
		 */
		~FalconKinematicZsomborMurray() {}

		/**
		 * Initializes lookup tables for kinematics (can block)
		 *
		 */
		void initialize();

		/**
		 * Given a caretesian position (in meters), and force vector (in newtons),
		 * return the force values that need to be sent to the firmware. Values are capped at 4096.
		 *
		 * @param position Current position of the end effector
		 * @param cart_force Force vector to apply to the end effector
		 * @param enc_force Force to be sent to the firmware
		 *
		 * @return true if forces are generated, false otherwise.
		 */
		virtual bool getForces(const boost::array<double, 3> (&position), const boost::array<double, 3> (&cart_force), boost::array<int, 3> (&enc_force));

		/**
		 * Given a caretesian position (in meters), return the angle of the legs requires to achieve the positions
		 *
		 * @param position Position to get the angles for (in cartesian coordinates, meters)
		 * @param angles Array to write result into
		 *
		 * @return true if angles are found, false otherwise (i.e. position out of workspace range)
		 */
		virtual bool getAngles(boost::array<double, 3> (&position), boost::array<double,3> (&angles));

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
		virtual bool getPosition(boost::array<int, 3> (&angles), boost::array<double, 3> (&position));

		/**
		 * Returns the center point of the workspace. May not always be [0,0,0].
		 *
		 * @param origin Array to store values in
		 */
		virtual void getWorkspaceOrigin(boost::array<double, 3> (&origin))
		{
			origin[0] = 0.0;
			origin[1] = 0.0;
			origin[2] = 0.150;
		}
	private:
		/** 3D Point structure for kinematics calculations
		 */
		struct point3f {
			float x;
			float y;
			float z;
		};

		/** 4D Point structure for homogenous kinematics calculations
		  */
		struct point4f {
			float w;
			float x;
			float y;
			float z;
		};

	};
}

#endif
