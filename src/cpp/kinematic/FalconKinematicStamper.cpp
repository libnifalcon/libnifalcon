//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net

#include "FalconKinematicStamper.h"
#include "stamper/JacobianMatrix.h"
namespace libnifalcon
{
	FalconKinematicStamper::FalconKinematicStamper(bool init_now)
	{
		if(init_now)
		{
			initialize();
		}
	}

	void FalconKinematicStamper::initialize()
	{
		m_dir.initialize();
	}

	gmtl::Point3f FalconKinematicStamper::translatePointToWorkspace(gmtl::Point3f p)
	{
		return gmtl::Point3f();
	}
	
	gmtl::Point3f FalconKinematicStamper::translatePointFromWorkspace(gmtl::Point3f p)
	{
		return gmtl::Point3f();
	}

	bool FalconKinematicStamper::getForces(double position[3], double cart_force[3], int16_t enc_force[3])
	{
		double ang[3];
		StamperKinematicImpl::Angle a;
		gmtl::Point3f p(position[0], position[1], position[2]);
		gmtl::Vec3f c(cart_force[0], cart_force[1], cart_force[2]);
		a = m_inv.calculate(p);
	
		gmtl::Vec3f angularVelocity = StamperKinematicImpl::JacobianMatrix::calculate(a, c);
		enc_force[0] = -angularVelocity[0];
		enc_force[1] = -angularVelocity[1];
		enc_force[2] = -angularVelocity[2];
		return true;
	}
	
	bool FalconKinematicStamper::getAngles(double position[3], double angles[3])
	{
		StamperKinematicImpl::Angle a;
		gmtl::Point3f p(position[0], position[1], position[2]);
		a = m_inv.calculate(p);
			
		angles[0] = a.theta1[0];
		angles[1] = a.theta1[1];
		angles[2] = a.theta1[2];			
		return true;
	}

	bool FalconKinematicStamper::getPosition(int16_t encoders[3], double position[3])
	{
		float angle1 = getTheta(encoders[0]);
		float angle2 = getTheta(encoders[1]);
		float angle3 = getTheta(encoders[2]);
		gmtl::Point3f p = m_dir.calculate(gmtl::Vec3f(gmtl::Math::deg2Rad(angle1), gmtl::Math::deg2Rad(angle2), gmtl::Math::deg2Rad(angle3)));
		position[0] = p[0];
		position[1] = p[1];
		position[2] = p[2];
		return true;
	}
}
