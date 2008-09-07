#ifndef FALCON_DEVICE_H
#define FALCON_DEVICE_H

#include <string>
#include <fstream>
#include <iostream>
#include "FalconComm.h"
#include "FalconFirmware.h"
#include "FalconKinematic.h"
#include "FalconGrip.h"

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

		bool runIOLoop();
		
		void setFalconComm(FalconComm* f);
		void setFalconFirmware(FalconFirmware* f);
		void setFalconGrip(FalconGrip* f);
		void setFalconKinematic(FalconKinematic* f);

		FalconComm* getFalconComm() { return m_falconComm; }
		FalconFirmware* getFalconFirmware() { return m_falconFirmware; }
		FalconGrip* getFalconGrip() { return m_falconGrip; }
		FalconKinematic* getFalconKinematic() { return m_falconKinematic; }

	protected:
		std::string m_firmwareFilename;
		int32_t m_errorCode;
		FalconComm* m_falconComm;
		FalconKinematic* m_falconKinematic;
		FalconFirmware* m_falconFirmware;
		FalconGrip* m_falconGrip;
		double m_position[3];
	};

}

#endif
