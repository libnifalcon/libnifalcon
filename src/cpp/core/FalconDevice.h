#ifndef FALCON_DEVICE_H
#define FALCON_DEVICE_H

#include <string>
#include <fstream>
#include <iostream>
#include "FalconComm.h"

namespace libnifalcon
{
	class FalconDevice
	{		
	public:
		FalconDevice();
		~FalconDevice();
		bool setFirmwareFile(std::string filename);
		bool loadFirmware();
		bool getDeviceCount(int8_t& count);
		bool open(u_int8_t index);
		void close();

		void runIOLoop();
		
		void setForce();
		void getPosition();
		
		FalconComm* getFalconComm() { return m_falconComm; }
		void setFalconComm(FalconComm* b) { m_falconComm = b; }

		//void setFalconFirmware(FalconFirmwareBase* b);
		//FalconFirmwareBase getFalconFirmware() { return m_falconFirmware; }
		//void setFalconGrip(FalconGripBase* b);
		//FalconFirmwareBase getFalconGrip() { return m_falconGrip; }
		//void setFalconKinematic(FalconKinematicBase* b);
		//FalconKinematicBase getFalconKinematic() { return m_falconKinematic; }
	protected:
		std::string m_firmwareFilename;
		int32_t m_errorCode;
		FalconComm* m_falconComm;
		//FalconKinematicBase* m_falconKinematic;
		//FalconFirmwareBase* m_falconFirmware;
		//FalconGripBase* m_falconGrip;
	};

}

#endif
