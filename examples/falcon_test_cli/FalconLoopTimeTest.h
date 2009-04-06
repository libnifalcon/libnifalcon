#ifndef FALCONLOOPTIMETEST_H
#define FALCONLOOPTIMETEST_H

#include "FalconTestBase.h"

class FalconLoopTimeTest : public FalconTestBase
{
public:
	FalconLoopTimeTest(boost::shared_ptr<libnifalcon::FalconDevice> d) :
		FalconTestBase(d)
	{
		setPrintOnCount(1000);
	}
protected:
	void runFunction();
};

#endif
