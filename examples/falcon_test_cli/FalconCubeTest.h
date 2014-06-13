#ifndef FALCON_CUBE_TEST_H
#define FALCON_CUBE_TEST_H

#include "FalconTestBase.h"
#include "falcon/gmtl/Vec.h"

class FalconCubeTest : public FalconTestBase
{
public:
	FalconCubeTest(std::shared_ptr<libnifalcon::FalconDevice> d);
protected:
	gmtl::Vec3f m_cornerA;
	gmtl::Vec3f m_cornerB;
	double m_stiffness;
	void runFunction();
	bool m_isInitializing;
	bool m_hasPrintedInitMsg;

};

#endif
