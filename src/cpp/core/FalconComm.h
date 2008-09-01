#ifndef FALCONCOMMBASE_H
#define FALCONCOMMBASE_H

#include <sys/types.h>

namespace libnifalcon
{
	class FalconComm
	{		
	public:
		enum {
			NIFALCON_DEVICE_NOT_FOUND_ERROR = 1000, /*!< No devices found on the system */
			NIFALCON_DEVICE_NOT_VALID_ERROR, /*!< Device not initialized or opened */
			NIFALCON_DEVICE_INDEX_OUT_OF_RANGE_ERROR, /*!< Device index for opening out of range of available devices */
			NIFALCON_FIRMWARE_NOT_FOUND_ERROR, /*!< Firmware file not found */
			NIFALCON_WRITE_ERROR, /*!< Write timeout hit, etc... */
			NIFALCON_READ_ERROR, /*!< Read timeout hit, etc... */
			NIFALCON_FIRMWARE_CHECKSUM_ERROR, /*!< Error thrown when library does not get back exact copy of firmware as it sent. Usually happens on slow hubs. */
			NIFALCON_NUM_ERRORS
		};

		FalconComm() : m_isCommOpen(false), m_errorCode(0) {}
		virtual ~FalconComm() {}
		virtual bool getDeviceCount(int8_t& count) = 0;
		virtual bool open(u_int8_t index) = 0;
		virtual bool close() = 0;
		virtual bool read(u_int8_t* str, u_int32_t size, u_int32_t& bytes_read) = 0;
		virtual bool write(u_int8_t* str, u_int32_t size, u_int32_t& bytes_written) = 0;
		virtual bool setFirmwareMode() = 0;
		virtual bool setNormalMode() = 0;
		
		bool isCommOpen() { return m_isCommOpen; }
		int32_t getErrorCode() { return m_errorCode; }
	protected:
		const static u_int8_t MAX_DEVICES = 128;
		const static u_int16_t FALCON_VENDOR_ID = 0x0403;
		const static u_int16_t FALCON_PRODUCT_ID = 0xCB48;
	   
		int32_t m_errorCode;
		bool m_isCommOpen;
	};

};

#endif
