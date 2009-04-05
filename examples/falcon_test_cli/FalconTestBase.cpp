#include "FalconTestBase.h"
#include <iostream>

void FalconTestBase::run()
{
	if(m_countLimit > 0)
	{
		int count_diff = m_falconDevice->getFalconFirmware()->getLoopCount() - m_lastLoopCount;
		if(count_diff >= m_countLimit)
		{
			tend();
			std::cout << "Time for " << count_diff << " loops: " << tval() << std::endl;
			m_lastLoopCount = m_falconDevice->getFalconFirmware()->getLoopCount();
			tstart();
		}
	}
	runFunction();
}

