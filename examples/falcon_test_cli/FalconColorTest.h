#ifndef FALCON_COLOR_TEST_H
#define FALCON_COLOR_TEST_H

#include "FalconTestBase.h"
#include "boost/array.hpp"

class FalconColorTest : public FalconTestBase
{
public:
	FalconColorTest(boost::shared_ptr<libnifalcon::FalconDevice> d);
	~FalconColorTest();
protected:
	boost::array<float, 3> m_posLimit;
	boost::array<int, 3> m_ledState;
	boost::array<unsigned int, 3> m_ledLevel;
	boost::array<unsigned int, 3> m_ledCount;
	unsigned int m_tickLimit;
	void runFunction();
};

#endif
