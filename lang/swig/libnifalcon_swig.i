%include "std_string.i"
%{
#include "falcon/util/FalconDeviceBridge.h"
%}

struct FalconVec3d
{
	double x;
	double y;
	double z;
};
	
class FalconDeviceBridge
{
public:
	
	FalconDeviceBridge();
	virtual ~FalconDeviceBridge();
	int getCount();
	bool open(int device_count);
	void close();
	bool runIOLoop(unsigned int exe_flags);
	std::string getRawOutput();
	void setRawInput(char* str);
	bool loadFirmware();
	int getErrorCode();
	void setForces(FalconVec3d v);
	FalconVec3d getPosition();
	void setLEDStatus(int v);
};
