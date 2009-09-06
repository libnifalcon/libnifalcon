/***
 * @file InverseKinematic.cpp
 * @brief IK implementation for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net) / Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
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
			float pu = -r+gmtl::Math::cos(phy[arm])*position[X]+gmtl::Math::sin(phy[arm])*position[Y];
			float pv = -(s-gmtl::Math::cos(phy[arm])*position[Y]+gmtl::Math::sin(phy[arm])*position[X]);
			float pw = position[Z];

			float theta3 = gmtl::Math::aCos((pv+f)/b);

			float l0 = gmtl::Math::pow(pw,2) + gmtl::Math::pow(pu,2) + 2*c*pu - 2*a*pu + gmtl::Math::pow(a,2) + gmtl::Math::pow(c,2) - gmtl::Math::pow(d,2) - gmtl::Math::pow(e,2)
				- gmtl::Math::pow(b,2)*gmtl::Math::pow(gmtl::Math::sin(theta3),2) - 2*b*e*gmtl::Math::sin(theta3) - 2*b*d*gmtl::Math::sin(theta3) - 2*d*e - 2*a*c;

			float l1 = -4*a*pw;

			float l2 = gmtl::Math::pow(pw,2) + gmtl::Math::pow(pu,2) + 2*c*pu + 2*a*pu + gmtl::Math::pow(a,2) + gmtl::Math::pow(c,2) - gmtl::Math::pow(d,2) - gmtl::Math::pow(e,2)
				- gmtl::Math::pow(b,2)*gmtl::Math::pow(gmtl::Math::sin(theta3),2) - 2*b*e*gmtl::Math::sin(theta3) - 2*b*d*gmtl::Math::sin(theta3) - 2*d*e + 2*a*c;

			float t = -(gmtl::Math::sqrt(gmtl::Math::pow(l1,2)-4*l2*l0)+l1)/(2*l2);

			float theta1 = gmtl::Math::aTan(t)*2;

			float theta2 = gmtl::Math::aCos(-(a*gmtl::Math::cos(theta1)-c-pu)/(b*gmtl::Math::sin(theta3)+d+e));

			angle.theta1[arm] = theta1;
			angle.theta2[arm] = theta2;
			angle.theta3[arm] = theta3;
		}
	}
}
