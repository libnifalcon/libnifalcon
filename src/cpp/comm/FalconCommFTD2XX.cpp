#include "FalconCommFTD2XX.h"
#include <cstring>

namespace libnifalcon
{
 	const char* FalconCommFTD2XX::FALCON_DESCRIPTION = "FALCON HAPTIC";
	
	bool FalconCommFTD2XX::getDeviceCount(int8_t& device_count)
	{
		device_count = openDeviceFTD2XX(0, true);
		return true;
	}

	bool FalconCommFTD2XX::open(uint8_t index)
	{
		return openDeviceFTD2XX(index, false) >= 0;
	}

	int8_t FalconCommFTD2XX::openDeviceFTD2XX(uint8_t device_index, bool stop_at_count)
	{
		unsigned int falcon_count = 0, device_count = 0, i = 0;
		char* pcBufLD[MAX_DEVICES + 1];
		char cBufLD[MAX_DEVICES][64];
		char serial[64];

		//If we're not using windows, we can set PID/VID to filter on. I have no idea why this isn't provided in the windows drivers.
#ifndef WIN32
		FT_SetVIDPID(FALCON_VENDOR_ID, FALCON_PRODUCT_ID);
#endif

		for(i = 0; i < MAX_DEVICES; i++) {
			pcBufLD[i] = cBufLD[i];
		}
		if((m_deviceErrorCode = FT_ListDevices(pcBufLD, &device_count, FT_LIST_ALL | FT_OPEN_BY_DESCRIPTION)) != FT_OK)
		{
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return -1;
		}
		for(i = 0; (i < device_count) && (falcon_count <= device_index); ++i)
		{
			if(!strcmp(cBufLD[i], FALCON_DESCRIPTION)) ++falcon_count;
		}

		if(stop_at_count)
		{
			return falcon_count;
		}
	
		if(falcon_count == 0 || device_index > falcon_count)
		{
			m_errorCode = FALCON_COMM_DEVICE_NOT_FOUND_ERROR;
			return -1;
		}

		m_errorCode = FALCON_COMM_DEVICE_ERROR;
		
		//Now that we know the index, get the serial number
		if((m_deviceErrorCode = FT_ListDevices((PVOID)(i-1), serial, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER))) return -1;
		//Open and reset device using serial number
		if((m_deviceErrorCode = FT_OpenEx(serial, FT_OPEN_BY_SERIAL_NUMBER, &m_falconDevice)) != FT_OK) return -1;

		m_errorCode = 0;
		m_isCommOpen = true;
		return 0;
	}

	bool FalconCommFTD2XX::close()
	{
		if(!m_isCommOpen)
		{
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}
		m_isCommOpen = false;
		FT_Close(m_falconDevice);
		return true;
	}

	bool FalconCommFTD2XX::read(uint8_t* str, uint32_t size)
	{
		unsigned long bytes_rx = 0, b_read = 0, bytes_read = 0;

		m_lastBytesRead = 0;
		if(!m_isCommOpen)
		{
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}
		m_errorCode = FALCON_COMM_DEVICE_ERROR;
		//Jankywait! But still better than permablocking for the moment.
		//Will turn this back into a timeout at some point.
		for(int i = 0; i < 10000000 && bytes_read < size; ++i)
		{
			if((m_deviceErrorCode = FT_GetQueueStatus(m_falconDevice, &bytes_rx)) != FT_OK) return false;
			if(bytes_rx == 0) continue;
			if(bytes_rx > size) bytes_rx = size - bytes_read;
			if(bytes_rx > 0)
			{
				if((m_deviceErrorCode = FT_Read(m_falconDevice, str, bytes_rx, &b_read)) != FT_OK) return false;
				bytes_read += bytes_rx;
			}
		}
		m_lastBytesRead = bytes_read;
		if(bytes_read != size)
		{
			m_errorCode = FALCON_COMM_READ_ERROR;
			return false;
		}
		m_errorCode = 0;
		return true;
	}

	bool FalconCommFTD2XX::write(uint8_t* str, uint32_t size)
	{
		if(!m_isCommOpen)
		{
			m_errorCode = FALCON_COMM_DEVICE_NOT_FOUND_ERROR;
			return false;
		}
		m_lastBytesWritten = 0;
		if((m_deviceErrorCode = FT_Write(m_falconDevice, str, size, (DWORD*)&m_lastBytesWritten)) != FT_OK)
		{
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return false;
		}
		if(m_lastBytesWritten != size)
		{
			m_errorCode = FALCON_COMM_WRITE_ERROR;
			return false;
		}
		return true;
	}

	bool FalconCommFTD2XX::setFirmwareMode()
	{
		uint32_t bytes_written, bytes_read;
		uint8_t check_msg_1[3] = {0x0a, 0x43, 0x0d};
		uint8_t check_msg_2[1] = {'A'};
		uint8_t send_buf[128], receive_buf[128];

		if(!m_isCommOpen)
		{
			m_errorCode = FALCON_COMM_DEVICE_NOT_FOUND_ERROR;
			return false;
		}
		m_errorCode = FALCON_COMM_DEVICE_ERROR;
		if((m_deviceErrorCode = FT_SetLatencyTimer(m_falconDevice, 16)) != FT_OK)
		{
			return false;
		}
		//Set to:
		// 9600 baud
		// 8n1
		// No Flow Control
		// RTS Low
		// DTR High	
		if((m_deviceErrorCode = FT_SetBaudRate(m_falconDevice, 9600)) != FT_OK) return false;
		if((m_deviceErrorCode = FT_SetDataCharacteristics(m_falconDevice, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE)) != FT_OK) return false;
		if((m_deviceErrorCode = FT_SetFlowControl(m_falconDevice, FT_FLOW_NONE, 0, 0)) != FT_OK) return false;
		if((m_deviceErrorCode = FT_ClrRts(m_falconDevice)) != FT_OK) return false;
		if((m_deviceErrorCode = FT_ClrDtr(m_falconDevice)) != FT_OK) return false;
		if((m_deviceErrorCode = FT_SetDtr(m_falconDevice)) != FT_OK) return false;

		//Send 3 bytes: 0x0a 0x43 0x0d
		if(!write((uint8_t*)check_msg_1, (uint32_t)3)) return false;
	
		//Expect 4 bytes back (LibFTDI expects 5. This expects 4. I dunno.)
		if(!read(receive_buf, 5)) return false;	

		//Set to:
		// DTR Low
		// 140000 baud (0x15 clock ticks per signal)
		if((m_deviceErrorCode = FT_ClrDtr(m_falconDevice)) != FT_OK) return false;
		if((m_deviceErrorCode = FT_SetBaudRate(m_falconDevice, 140000)) != FT_OK) return false;

		//Send "A" character
		if(!write(check_msg_2, 1)) return false;

		//Expect back 2 bytes:
		// 0x13 0x41
		// (Both LibFTDI and FTD2XX except 2)
		if(!read(receive_buf, 1)) return false;
		m_errorCode = 0;
		return true;
	}

	bool FalconCommFTD2XX::setNormalMode()
	{
		m_errorCode = FALCON_COMM_DEVICE_ERROR;
		if((m_deviceErrorCode = FT_SetBaudRate(m_falconDevice, 1456312)) != FT_OK)
		{
			return false;
		}
		if((m_deviceErrorCode = FT_SetLatencyTimer(m_falconDevice, 1)) != FT_OK)
		{
			return false;
		}
		m_errorCode = 0;
		return true;
	}


};
