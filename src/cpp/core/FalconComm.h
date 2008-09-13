#ifndef FALCONCOMMBASE_H
#define FALCONCOMMBASE_H

#include <sys/types.h>
#include "FalconCore.h"

namespace libnifalcon 
{
	class FalconComm : public FalconCore
	{		
	public:
		enum {
			FALCON_COMM_DEVICE_ERROR = 2000,  /*!< Device driver specific error, check getDeviceErrorCode */
			FALCON_COMM_NOT_INITIALIZED, /*!< Device drivers not initialized */
			FALCON_COMM_DEVICE_NOT_FOUND_ERROR, /*!< No devices found on the system */
			FALCON_COMM_DEVICE_NOT_VALID_ERROR, /*!< Device not opened */
			FALCON_COMM_DEVICE_INDEX_OUT_OF_RANGE_ERROR, /*!< Device index for opening out of range of available devices */
			FALCON_COMM_FIRMWARE_NOT_FOUND_ERROR, /*!< Firmware file not found */
			FALCON_COMM_WRITE_ERROR, /*!< Write timeout hit, underflow, etc... */
			FALCON_COMM_READ_ERROR /*!< Read timeout hit, underflow, etc... */
		};

		FalconComm() : m_isCommOpen(false) {}
		virtual ~FalconComm() {}
		virtual bool getDeviceCount(int8_t& count) = 0;
		virtual bool open(u_int8_t index) = 0;
		virtual bool close() = 0;
		virtual bool read(u_int8_t* str, u_int32_t size) = 0;
		virtual bool write(u_int8_t* str, u_int32_t size) = 0;
		virtual bool setFirmwareMode() = 0;
		virtual bool setNormalMode() = 0;

		
		int getLastBytesRead()  {return m_lastBytesRead; }
		int getLastBytesWritten()  {return m_lastBytesWritten; }
		int getDeviceErrorCode() { return m_deviceErrorCode; }
		bool isCommOpen() { return m_isCommOpen; }
	protected:
		const static u_int8_t MAX_DEVICES = 128;
		const static u_int16_t FALCON_VENDOR_ID = 0x0403;
		const static u_int16_t FALCON_PRODUCT_ID = 0xCB48;
		int m_deviceErrorCode;
		int m_lastBytesRead;
		int m_lastBytesWritten;
		bool m_isCommOpen;
	};

};

#endif
