#include "FalconTestBase.h"
#include <iostream>

void FalconTestBase::run()
{
	++m_runCount;
	if(m_countLimit > 0)
	{
		if(m_runCount % m_countLimit == 0)
		{
			tend();
			std::cout << "Time for " << m_runCount << " loops: " << tval() << std::endl;
			m_runCount = 0;
			tstart();
		}
	}
	runFunction();
}

