#ifndef FALCON_DEVICE_H
#define FALCON_DEVICE_H

#include <string>
#include <fstream>
#include <iostream>
#include "FalconCore.h"
#include "FalconComm.h"
#include "FalconFirmware.h"
#include "FalconKinematic.h"
#include "FalconGrip.h"

namespace libnifalcon
{
	class FalconDevice : public FalconCore
	{		
	public:
		enum {
			FALCON_DEVICE_NO_COMM_SET = 1000,
			FALCON_DEVICE_NO_FIRMWARE_SET,
			FALCON_DEVICE_NO_KINEMATIC_SET,
			FALCON_DEVICE_NO_GRIP_SET,
			FALCON_DEVICE_NO_FIRMWARE_LOADED,
			FALCON_DEVICE_FIRMWARE_NOT_VALID,
			FALCON_DEVICE_FIRMWARE_CHECKSUM_MISMATCH
		};
		FalconDevice();
		~FalconDevice();
		bool isFirmwareLoaded();
		bool setFirmwareFile(std::string filename);
		bool loadFirmware(int retries, bool skip_checksum = false);
		bool loadFirmware(bool skip_checksum);
		bool getDeviceCount(int8_t& count);
		bool open(u_int8_t index);
		void close();

		bool runIOLoop();
		void runThreadLoop() { while(1) runIOLoop(); }

		void setCleanupObjects(bool cleanup_objects) { m_cleanupObjects = cleanup_objects; }
		
		template<class T>
		void setFalconComm();
		template<class T>
		void setFalconFirmware();
		template<class T>
		void setFalconGrip();
		template<class T>
		void setFalconKinematic(FalconKinematic::WorkspaceBounds b);
		double* getPosition() { return m_position; }
		FalconComm* getFalconComm() { return m_falconComm; }
		FalconFirmware* getFalconFirmware() { return m_falconFirmware; }
		FalconGrip* getFalconGrip() { return m_falconGrip; }
		FalconKinematic* getFalconKinematic() { return m_falconKinematic; }
		void removeFalconComm()
		{
			if(m_falconComm != NULL)
			{
				delete m_falconComm;
				m_falconComm = NULL;
			}			
		}
		void removeFalconFirmware()
		{
			if(m_falconFirmware != NULL)
			{
				delete m_falconFirmware;
				m_falconFirmware = NULL;
			}
		}
		void removeFalconGrip()
		{
			if(m_falconGrip != NULL)
			{
				delete m_falconGrip;
				m_falconGrip = NULL;
			}
		}
		void removeFalconKinematic()
		{
			if(m_falconKinematic != NULL)
			{
				delete m_falconKinematic;
				m_falconKinematic = NULL;
			}
		}
		uint32_t getErrorCount() { return m_errorCount; }
	protected:
		bool m_isFirmwareLoaded;
		bool m_cleanupObjects;
		uint32_t m_errorCount;
		std::string m_firmwareFilename;
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
	void FalconDevice::setFalconKinematic(FalconKinematic::WorkspaceBounds b)
	{
		m_falconKinematic = new T(b);
	}
	
}



#endif
