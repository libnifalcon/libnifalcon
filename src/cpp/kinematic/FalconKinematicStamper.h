#ifndef FALCONSTAMPERKINEMATIC_H
#define FALCONSTAMPERKINEMATIC_H

#include <sys/types.h>
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
		virtual bool getAngles(double* position, double* angles);
		virtual bool getPosition(int16_t angles[3], double* position);
	private:
		StamperKinematicImpl::InverseKinematic m_inv;
		StamperKinematicImpl::DirectKinematic m_dir;
	};
}

#endif
