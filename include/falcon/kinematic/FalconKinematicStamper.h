/***
 * @file FalconKinematicStamper.h
 * @brief IK and DK implementation for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net) / Kyle Machulis (kyle@nonpolynomial.com)
 * @version $Id$
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL$
 * 
 * Project info at http://libnifalcon.sourceforge.net/ 
 *
 */

#ifndef FALCONSTAMPERKINEMATIC_H
#define FALCONSTAMPERKINEMATIC_H

#include <stdint.h>
#include "falcon/core/FalconKinematic.h"
#include "falcon/kinematic/stamper/DirectKinematic.h"
#include "falcon/kinematic/stamper/InverseKinematic.h"

namespace libnifalcon
{
	class FalconKinematicStamper : public FalconKinematic
	{
	public:
		FalconKinematicStamper(bool init_now = true);
		~FalconKinematicStamper() {}
		void initialize();
		StamperKinematicImpl::InverseKinematic* getInverseKinematic() { return &m_inv; }
		StamperKinematicImpl::DirectKinematic* getDirectKinematic() { return &m_dir; }

		virtual bool getForces(const double (&position)[3], const double (&cart_force)[3], int16_t (&enc_force)[3]);
		virtual bool getAngles(const double (&position)[3], double (&angles)[3]);
		virtual bool getPosition(const int16_t (&angles)[3], double (&position)[3]);
	protected:

		StamperKinematicImpl::InverseKinematic m_inv;
		StamperKinematicImpl::DirectKinematic m_dir;
	};
}

#endif
