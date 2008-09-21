#ifndef FALCONSTAMPERKINEMATIC_H
#define FALCONSTAMPERKINEMATIC_H

#include <stdint.h>
#include "core/FalconKinematic.h"
#include "stamper/DirectKinematic.h"
#include "stamper/InverseKinematic.h"

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

		virtual bool getForces(double position[3], double cart_force[3], int16_t enc_force[3]);
		virtual bool getAngles(double position[3], double angles[3]);
		virtual bool getPosition(int16_t angles[3], double position[3]);
	protected:
		gmtl::Point3f translatePointFromWorkspace(gmtl::Point3f p);
		gmtl::Point3f translatePointToWorkspace(gmtl::Point3f p);
		StamperKinematicImpl::InverseKinematic m_inv;
		StamperKinematicImpl::DirectKinematic m_dir;
	};
}

#endif
