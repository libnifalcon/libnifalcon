/***
 * @file InverseKinematic.h
 * @brief IK implementation for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net)
 * @version $Id$
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL$
 * 
 * Project info at http://libnifalcon.sourceforge.net/ 
 *
 */
#ifndef INVERSEKINEMATIC_H_
#define INVERSEKINEMATIC_H_

#include "falcon/gmtl/gmtl.h"

#include "falcon/core/FalconGeometry.h"
#include "falcon/kinematic/stamper/StamperUtils.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{
		class InverseKinematic
		{
		public:
	
			InverseKinematic() {}
			virtual ~InverseKinematic() {}

			static Angle calculate(const gmtl::Point3f& position);

		private:
			static void calulateArm(const arm_id arm, const gmtl::Point3f& position, Angle& angle);
		};
	}
}

#endif /*INVERSEKINEMATIC_H_*/
