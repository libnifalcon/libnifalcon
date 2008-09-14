#ifndef FALCONFTD2XXCOMM_H
#define FALCONFTD2XXCOMM_H

#include "core/FalconComm.h"
#include "ftd2xx.h"

namespace libnifalcon
{
	class FalconCommFTD2XX : public FalconComm
	{
	public:
		virtual bool getDeviceCount(int8_t& );
		virtual bool open(uint8_t );
		virtual bool close();
		virtual bool read(uint8_t*, uint32_t);
		virtual bool write(uint8_t*, uint32_t);
		virtual bool setFirmwareMode();
		virtual bool setNormalMode();
	protected:
		const static char* FALCON_DESCRIPTION;
		int8_t openDeviceFTD2XX(uint8_t , bool );
		FT_HANDLE m_falconDevice;
	};
};

#endif
