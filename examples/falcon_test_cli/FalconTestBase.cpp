#include "FalconTestBase.h"
#include <iostream>

void FalconTestBase::run()
{
	if(m_countLimit > 0)
	{
		++m_runCount;
		if(m_countLimit == m_runCount)
		{
			tend();
			std::cout << "Time for " << m_runCount << " loops: " << tval() << std::endl;
			m_runCount = 0;
			tstart();
		}
	}
	runFunction();
}

