/***
 * @file InverseKinematic.cpp
 * @brief IK implementation for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net) / Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.sourceforge.net/
 *
 */

#include "falcon/kinematic/stamper/InverseKinematic.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		Angle InverseKinematic::calculate(const gmtl::Point3f& position)
		{
			Angle angle;
			calulateArm(ARM_1, position, angle);
			calulateArm(ARM_2, position, angle);
			calulateArm(ARM_3, position, angle);
			return angle;
		}

		void InverseKinematic::calulateArm(const arm_id arm, const gmtl::Point3f& position, Angle& angle)
		{
			float pu = -FalconGeometry::r+gmtl::Math::cos(FalconGeometry::phy[arm])*position[X]+gmtl::Math::sin(FalconGeometry::phy[arm])*position[Y];
			float pv = -(FalconGeometry::s-gmtl::Math::cos(FalconGeometry::phy[arm])*position[Y]+gmtl::Math::sin(FalconGeometry::phy[arm])*position[X]);
			float pw = position[Z];

			float theta3 = gmtl::Math::aCos((pv+FalconGeometry::f)/FalconGeometry::b);

			float l0 = gmtl::Math::pow(pw,2) + gmtl::Math::pow(pu,2) + 2*FalconGeometry::c*pu - 2*FalconGeometry::a*pu + gmtl::Math::pow(FalconGeometry::a,2)
				+ gmtl::Math::pow(FalconGeometry::c,2) - gmtl::Math::pow(FalconGeometry::d,2) - gmtl::Math::pow(FalconGeometry::e,2)
				- gmtl::Math::pow(FalconGeometry::b,2) * gmtl::Math::pow(gmtl::Math::sin(theta3),2) - 2*FalconGeometry::b * FalconGeometry::e
				* gmtl::Math::sin(theta3) - 2 * FalconGeometry::b * FalconGeometry::d * gmtl::Math::sin(theta3)
				- 2 * FalconGeometry::d * FalconGeometry::e - 2 * FalconGeometry::a * FalconGeometry::c;

			float l1 = -4 * FalconGeometry::a * pw;

			float l2 = gmtl::Math::pow(pw,2) + gmtl::Math::pow(pu,2) + 2 * FalconGeometry::c * pu + 2 * FalconGeometry::a * pu
				+ gmtl::Math::pow(FalconGeometry::a,2) + gmtl::Math::pow(FalconGeometry::c,2) - gmtl::Math::pow(FalconGeometry::d,2)
				- gmtl::Math::pow(FalconGeometry::e,2) - gmtl::Math::pow(FalconGeometry::b,2) * gmtl::Math::pow(gmtl::Math::sin(theta3),2)
				- 2 * FalconGeometry::b * FalconGeometry::e * gmtl::Math::sin(theta3) - 2 * FalconGeometry::b * FalconGeometry::d
				* gmtl::Math::sin(theta3) - 2 * FalconGeometry::d * FalconGeometry::e + 2 * FalconGeometry::a * FalconGeometry::c;

			float t = -(gmtl::Math::sqrt(gmtl::Math::pow(l1,2)-4*l2*l0)+l1)/(2*l2);

			float theta1 = gmtl::Math::aTan(t)*2;

			float theta2 = gmtl::Math::aCos(-(FalconGeometry::a*gmtl::Math::cos(theta1)-FalconGeometry::c-pu)/(FalconGeometry::b * gmtl::Math::sin(theta3) + FalconGeometry::d + FalconGeometry::e));

			angle.theta1[arm] = theta1;
			angle.theta2[arm] = theta2;
			angle.theta3[arm] = theta3;
		}
	}
}
