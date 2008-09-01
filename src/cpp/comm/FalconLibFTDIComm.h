#ifndef FALCONLIBFTDICOMM_H
#define FALCONLIBFTDICOMM_H

#include "core/FalconComm.h"
#include "ftdi.h"

namespace libnifalcon
{
	class FalconLibFTDIComm : public FalconCommBase
	{
	public:
		FalconLibFTDIComm();
		~FalconLibFTDIComm();
		virtual bool getDeviceCount(int8_t& );
		virtual bool open(u_int8_t );
		virtual bool close();
		virtual bool read(u_int8_t*, u_int32_t , u_int32_t& );
		virtual bool write(u_int8_t*, u_int32_t , u_int32_t& );
		virtual bool setFirmwareMode();
		virtual bool setNormalMode();
	protected:
//		int8_t openDeviceFTD2XX(u_int8_t , bool );
		bool initLibFTDI();
		ftdi_context m_falconDevice;
		bool m_isInitialized;
	};
};

#endif
