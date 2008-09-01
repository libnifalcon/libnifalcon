#ifndef FALCONFTD2XXCOMM_H
#define FALCONFTD2XXCOMM_H

#include "core/FalconComm.h"
#include "ftd2xx.h"

namespace libnifalcon
{
	class FalconFTD2XXComm : public FalconComm
	{
	public:
		virtual bool getDeviceCount(int8_t& );
		virtual bool open(u_int8_t );
		virtual bool close();
		virtual bool read(u_int8_t*, u_int32_t , u_int32_t& );
		virtual bool write(u_int8_t*, u_int32_t , u_int32_t& );
		virtual bool setFirmwareMode();
		virtual bool setNormalMode();
	protected:
		const static char* FALCON_DESCRIPTION;
		int8_t openDeviceFTD2XX(u_int8_t , bool );
		FT_HANDLE m_falconDevice;
	};
};

#endif
