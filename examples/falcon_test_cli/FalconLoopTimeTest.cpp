#include "FalconLoopTimeTest.h"
#include "falcon/core/FalconDevice.h"

void FalconLoopTimeTest::runFunction()
{
	m_falconDevice->runIOLoop();
}
