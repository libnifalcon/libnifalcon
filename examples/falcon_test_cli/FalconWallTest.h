#ifndef FALCONWALLTEST_H
#define FALCONWALLTEST_H

#include "FalconTestBase.h"
#include "falcon/gmtl/Vec.h"

class FalconWallTest : public FalconTestBase
{
public:
	FalconWallTest(boost::shared_ptr<libnifalcon::FalconDevice> d, unsigned int axis);
protected:
	void runFunction();

	unsigned int m_axis;
	unsigned long m_runClickCount;
	bool m_positiveForce;
	double m_stiffness;
	gmtl::Vec3f m_axisBounds;
	bool m_isInitializing;
	bool m_hasPrintedInitMsg;
	bool m_buttonDown;
};

#endif
