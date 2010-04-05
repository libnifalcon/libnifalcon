#ifndef FALCON_SPHERE_TEST_H
#define FALCON_SPHERE_TEST_H

#include "FalconTestBase.h"
#include "falcon/gmtl/Vec.h"

class FalconSphereTest : public FalconTestBase
{
public:
	FalconSphereTest(boost::shared_ptr<libnifalcon::FalconDevice> d);
protected:
	double m_stiffness;
    double m_radius;
    boost::array<double, 3> m_oldpos;
    bool m_plusButtonDown;
    bool m_minusButtonDown;
    bool m_centerButtonDown;
    bool m_forwardButtonDown;
    

	void runFunction();
	bool m_isInitializing;
	bool m_hasPrintedInitMsg;

};

#endif
