#ifndef FALCON_COLOR_TEST_H
#define FALCON_COLOR_TEST_H

#include "FalconTestBase.h"
#include <array>

class FalconColorTest : public FalconTestBase
{
public:
	FalconColorTest(std::shared_ptr<libnifalcon::FalconDevice> d);
	~FalconColorTest();
protected:
	std::array<float, 3> m_posLimit;
	std::array<int, 3> m_ledState;
	std::array<unsigned int, 3> m_ledLevel;
	std::array<unsigned int, 3> m_ledCount;
	unsigned int m_tickLimit;
	void runFunction();
};

#endif
