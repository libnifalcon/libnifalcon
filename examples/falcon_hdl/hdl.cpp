//Implementation file for emulation of Novints SDK using libnifalcon
//I have no idea what license this is under now. So just don't like,
//go crazy with it or something.

#include <vector>
#include "hdl.h"
#include "core/FalconDevice.h"
#include "comm/FalconCommLibFTDI.h"
#include "firmware/FalconFirmwareNovintSDK.h"
#include "kinematic/FalconKinematicStamper.h"
#include "util/FalconNovintHDALCompat.h"
#include "grip/FalconGripFourButton.h"

using namespace libnifalcon;

FalconNovintHDALCompat dev;
int op_handle_count = 0;

HDLAPI HDLError HDLAPIENTRY hdlGetError()
{
	return 0;
}

HDLAPI void HDLAPIENTRY hdlSetConfigPath(const char configPath)
{
}

HDLAPI HDLDeviceHandle HDLAPIENTRY hdlInitDevice(HDLDeviceID deviceID)
{	
	dev.setFalconComm<FalconCommLibFTDI>();
	dev.setFalconFirmware<FalconFirmwareNovintSDK>();
	dev.setFalconKinematic<FalconKinematicStamper>();
	dev.setFalconGrip<FalconGripFourButton>();
	dev.open(0);	
	return 0;
}

HDLAPI HDLDeviceHandle HDLAPIENTRY hdlInitNamedDevice(const char* deviceName, const char* configPath)
{
	return hdlInitDevice(0);	
}

HDLAPI HDLDeviceHandle HDLAPIENTRY hdlInitIndexedDevice(const int index, const char* configPath)
{
	return hdlInitDevice(0);
}

HDLAPI void HDLAPIENTRY hdlStart()
{
	dev.startThread();
}

HDLAPI void HDLAPIENTRY hdlStop()
{
	dev.stopThread();
}

HDLAPI unsigned int HDLAPIENTRY hdlGetState()
{
	int ret = 0;
	if(!dev.isOpen())
		ret |= HDAL_UNINITIALIZED;
	if(!dev.isThreadRunning())
		ret |= HDAL_SERVO_NOT_STARTED;
	if(!dev.getFalconFirmware()->isHomed())
		ret |= HDAL_NOT_CALIBRATED;
	return ret;
}

HDLAPI void HDLAPIENTRY hdlUninitDevice(HDLDeviceHandle hHandle)
{
	dev.close();
}

HDLAPI void HDLAPIENTRY hdlMakeCurrent(HDLDeviceHandle hHandle)
{
	
}

HDLAPI void HDLAPIENTRY hdlDeviceWorkspace(double workspaceDimensions[6])
{
	workspaceDimensions[0] = -60.0;
	workspaceDimensions[1] = -60.0;
	workspaceDimensions[2] = 75.0;
	workspaceDimensions[3] = 60.0;
	workspaceDimensions[4] = 60.0;
	workspaceDimensions[5] = 175.0;	
}

HDLAPI const char* HDLAPIENTRY hdlDeviceModel()
{
	return "FALCON DEVICE";
}

HDLAPI void HDLAPIENTRY hdlSetToolForce(double force[3])
{
	dev.setForce(force);
}

HDLAPI void HDLAPIENTRY hdlToolPosition(double position[3])
{
	dev.getPosition(position);
}

HDLAPI void HDLAPIENTRY hdlToolButton(bool* pButton)
{
	*pButton = (dev.getFalconGrip()->getDigitalInputs() > 0);
}

HDLAPI void HDLAPIENTRY hdlToolButtons(int* pButton)
{
	*pButton = dev.getFalconGrip()->getDigitalInputs();
}

HDLAPI HDLOpHandle HDLAPIENTRY hdlCreateServoOp(HDLServoOp pServoOp, void* pParam, bool bBlocking)
{
	if(!bBlocking)
	{
		dev.addServoOp(pServoOp, pParam);
	}
	else
	{
		dev.addServoOp(pServoOp, pParam);
	}
	return 0;
}

HDLAPI void HDLAPIENTRY hdlDestroyServoOp(HDLOpHandle hServoOp)
{
}

HDLAPI bool HDLAPIENTRY hdlGetVersion(HDL_VERSION_REQUEST requestType, HDL_VERSION_INFO_TYPE* versionInfo)
{
	return true;
}

HDLAPI int HDLAPIENTRY HDL_MAJOR_VERSION(HDL_VERSION_INFO_TYPE versionInfo)
{
	return 1;
}

HDLAPI int HDLAPIENTRY HDL_MINOR_VERSION(HDL_VERSION_INFO_TYPE versionInfo)
{
	return 0;
}

HDLAPI long HDLAPIENTRY HDL_BUILD_VERSION(HDL_VERSION_INFO_TYPE versionInfo)
{
	return 0;
}

HDLAPI void HDLAPIENTRY hdluGenerateHapticToAppWorkspaceTransform(double hapticWorkspace[6], double gameWorkspace[6], bool useUniformScale, double transformMat[16])
{
	transformMat[0] = (((gameWorkspace[3]) - (gameWorkspace[0])) / ((hapticWorkspace[3]) - (hapticWorkspace[0])));
	transformMat[1] = 0;
	transformMat[2] = 0;
	transformMat[3] = 0;
	
	transformMat[4] = 0;
	transformMat[5] = (((gameWorkspace[4]) - (gameWorkspace[1])) / ((hapticWorkspace[4]) - (hapticWorkspace[1])));
	transformMat[6] = 0;
	transformMat[7] = 0;

	transformMat[8] = 0;
	transformMat[9] = 0;
	transformMat[10] = (((gameWorkspace[5]) - (gameWorkspace[2])) / ((hapticWorkspace[5]) - (hapticWorkspace[2])));
	transformMat[11] = 0;
	
	transformMat[12] = ((gameWorkspace[3] + gameWorkspace[0]))/2.0 - (((transformMat[0]*hapticWorkspace[3])+(transformMat[0]*hapticWorkspace[0])))/2.0;	
	transformMat[13] = ((gameWorkspace[4] + gameWorkspace[1]))/2.0 - (((transformMat[5]*hapticWorkspace[4])+(transformMat[5]*hapticWorkspace[1])))/2.0;	
	transformMat[14] = ((gameWorkspace[5] + gameWorkspace[2]))/2.0 - (((transformMat[10]*hapticWorkspace[5])+(transformMat[10]*hapticWorkspace[2])))/2.0;	
	transformMat[15] = 1;
	for(int i = 0; i < 16; ++i)
	{
		std::cout << transformMat[i] << std::endl;
	}
}

HDLAPI int HDLAPIENTRY hdlCountDevices()
{
	int8_t i;
	dev.getDeviceCount(i);
	return i;
}
