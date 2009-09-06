/***
 * @file FalconDeviceDefault.h
 * @brief Utility class for wrapping template calls for device behaviors
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONDEVICEBRIDGE_H
#define FALCONDEVICEBRIDGE_H

#include "falcon/core/FalconDevice.h"

struct FalconVec3d
{
	double x;
	double y;
	double z;
};   

class FalconDeviceBridge : public libnifalcon::FalconDevice
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

#endif
