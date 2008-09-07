#ifndef FALCONSTAMPERKINEMATIC_H
#define FALCONSTAMPERKINEMATIC_H


#include <sys/types.h>
#include "core/FalconKinematic.h"
#include "DirectKinematic.h"
#include "InverseKinematic.h"

namespace libnifalcon
{
	namespace stamper
	{
		class FalconStamperKinematic : public FalconKinematic
		{
		public:
			FalconStamperKinematic(bool init_now = true);
			~FalconStamperKinematic() {}
			void initialize();
			float getTheta(int16_t encoder_value);
			virtual bool getAngles(double* position, double* angles);
			virtual bool getPosition(int16_t angles[3], double* position);
		private:
			InverseKinematic m_inv;
			DirectKinematic m_dir;
		};
	}
}

#endif
