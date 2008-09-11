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
		bool isFirmwareLoaded();
		bool setFirmwareFile(std::string filename);
		bool loadFirmware(int retries);
		bool getDeviceCount(int8_t& count);
		bool open(u_int8_t index);
		void close();

		bool runIOLoop();
		void runThreadLoop() { while(1) runIOLoop(); }
		
		template<class T>
		void setFalconComm();
		template<class T>
		void setFalconFirmware();
		template<class T>
		void setFalconGrip();
		template<class T>
		void setFalconKinematic();

		FalconComm* getFalconComm() { return m_falconComm; }
		FalconFirmware* getFalconFirmware() { return m_falconFirmware; }
		FalconGrip* getFalconGrip() { return m_falconGrip; }
		FalconKinematic* getFalconKinematic() { return m_falconKinematic; }

	protected:
		bool loadFirmware();
		bool m_isFirmwareLoaded;
		std::string m_firmwareFilename;
		int32_t m_errorCode;
		FalconComm* m_falconComm;
		FalconKinematic* m_falconKinematic;
		FalconFirmware* m_falconFirmware;
		FalconGrip* m_falconGrip;
		double m_position[3];
	};

	template<class T>
	void FalconDevice::setFalconComm()
	{
		m_falconComm = new T();
		if(m_falconFirmware != NULL)
		{
			m_falconFirmware->setFalconComm(m_falconComm);
		}		
	}

	template<class T>
	void FalconDevice::setFalconFirmware()
	{
		m_falconFirmware = new T();
		if(m_falconComm != NULL)
		{
			m_falconFirmware->setFalconComm(m_falconComm);
		}		
	}

	template<class T>
	void FalconDevice::setFalconGrip()
	{
		m_falconGrip = new T();
	}

	template<class T>
	void FalconDevice::setFalconKinematic()
	{
		m_falconKinematic = new T();
	}
	
}



#endif
