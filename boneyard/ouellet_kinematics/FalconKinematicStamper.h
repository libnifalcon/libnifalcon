/***
 * @file FalconKinematicStamper.h
 * @brief IK and DK implementation for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net) / Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONSTAMPERKINEMATIC_H
#define FALCONSTAMPERKINEMATIC_H

#include "falcon/core/FalconKinematic.h"
#include "falcon/kinematic/stamper/DirectKinematic.h"
#include "falcon/kinematic/stamper/InverseKinematic.h"

namespace libnifalcon
{

	class FalconKinematicStamper : public FalconKinematic
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param init_now If true, runs initialize() function on construction (can block). Defaults to true.
		 *
		 * @return
		 */
		FalconKinematicStamper(bool init_now = true);

		/**
		 * Destructor
		 *
		 *
		 * @return
		 */
		~FalconKinematicStamper() {}

		/**
		 * Initializes lookup tables for kinematics (can block)
		 *
		 */
		void initialize();

		/**
		 * Returns the internal InverseKinematic object
		 *
		 *
		 * @return InverseKinematic object
		 */
		StamperKinematicImpl::InverseKinematic* getInverseKinematic() { return &m_inv; }

		/**
		 * Returns the internal DirectKinematic object
		 *
		 *
		 * @return DirectKinematic object
		 */
		StamperKinematicImpl::DirectKinematic* getDirectKinematic() { return &m_dir; }

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

	protected:
		StamperKinematicImpl::InverseKinematic m_inv; /**< Internal inverse kinematics representation */
		StamperKinematicImpl::DirectKinematic m_dir; /**< Internal direct kinematics representation *///@}
	};
}

#endif
