#include "FalconCommFTD2XX.h"
#include <iostream>
#include <cstring>

namespace libnifalcon
{
 	const char* FalconCommFTD2XX::FALCON_DESCRIPTION = "FALCON HAPTIC";
	
	bool FalconCommFTD2XX::getDeviceCount(int8_t& device_count)
	{
		device_count = openDeviceFTD2XX(0, true);
		return device_count >= 0;
	}

	bool FalconCommFTD2XX::open(u_int8_t index)
	{
		return openDeviceFTD2XX(index, false) >= 0;
	}

	int8_t FalconCommFTD2XX::openDeviceFTD2XX(u_int8_t device_index, bool stop_at_count)
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
		if((m_errorCode = FT_ListDevices(pcBufLD, &device_count, FT_LIST_ALL | FT_OPEN_BY_DESCRIPTION)) != FT_OK) return -1;
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
			m_errorCode = NIFALCON_DEVICE_NOT_FOUND_ERROR;
			return -1;
		}
	
		//Now that we know the index, get the serial number
		if((m_errorCode = FT_ListDevices((PVOID)(i-1), serial, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER))) return -1;
		//Open and reset device using serial number
		if((m_errorCode = FT_OpenEx(serial, FT_OPEN_BY_SERIAL_NUMBER, &m_falconDevice)) != FT_OK) return -1;
	
		m_isCommOpen = true;
		return 0;
	}

	bool FalconCommFTD2XX::close()
	{
		if(!m_isCommOpen) return false;
		m_isCommOpen = false;
		FT_Close(m_falconDevice);
		return true;
	}

	bool FalconCommFTD2XX::read(u_int8_t* str, u_int32_t size, u_int32_t& bytes_read)
	{
		unsigned long bytes_rx, b_read;
		bytes_read = 0;
	
		if(!m_isCommOpen)
		{
			m_errorCode = NIFALCON_DEVICE_NOT_FOUND_ERROR;
			return false;
		}
		while(bytes_read < size)
		{
			if((m_errorCode = FT_GetQueueStatus(m_falconDevice, &bytes_rx)) != FT_OK) return false;
			if(bytes_rx > size) bytes_rx = size - bytes_read;
			if(bytes_rx > 0)
			{
				if((m_errorCode = FT_Read(m_falconDevice, str, bytes_rx, &b_read)) != FT_OK) return false;
				bytes_read += bytes_rx;
				std::cout << bytes_read << std::endl;
			}
			//if (clock() > timeout) return bytes_read;
		}
		bytes_read = b_read;
		if(bytes_read != size) return false;	
		return true;
	}

	bool FalconCommFTD2XX::write(u_int8_t* str, u_int32_t size, u_int32_t& bytes_written)
	{
		if(!m_isCommOpen)
		{
			m_errorCode = NIFALCON_DEVICE_NOT_FOUND_ERROR;
			return false;
		}
		unsigned long b_written;
		if((m_errorCode = FT_Write(m_falconDevice, str, size, &b_written)) != FT_OK) return false;
		bytes_written = b_written;
		if(bytes_written != size) return false;
		return true;
	}

	bool FalconCommFTD2XX::setFirmwareMode()
	{
		u_int32_t bytes_written, bytes_read;
		u_int8_t check_msg_1[3] = {0x0a, 0x43, 0x0d};
		u_int8_t check_msg_2[1] = {'A'};
		u_int8_t send_buf[128], receive_buf[128];

		//Set to:
		// 9600 baud
		// 8n1
		// No Flow Control
		// RTS Low
		// DTR High	
		if((m_errorCode = FT_SetBaudRate(m_falconDevice, 9600)) != FT_OK) return false;
		if((m_errorCode = FT_SetDataCharacteristics(m_falconDevice, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE)) != FT_OK) return false;
		if((m_errorCode = FT_SetFlowControl(m_falconDevice, FT_FLOW_NONE, 0, 0)) != FT_OK) return false;
		if((m_errorCode = FT_ClrRts(m_falconDevice)) != FT_OK) return false;
		if((m_errorCode = FT_ClrDtr(m_falconDevice)) != FT_OK) return false;
		if((m_errorCode = FT_SetDtr(m_falconDevice)) != FT_OK) return false;

		//Send 3 bytes: 0x0a 0x43 0x0d
		if((m_errorCode = write((u_int8_t*)check_msg_1, (u_int32_t)3, bytes_written)) < 3) return false;
	
		//Expect 5 bytes back
		if((m_errorCode = read(receive_buf, 5, bytes_read)) < 5) return false;	

		//Set to:
		// DTR Low
		// 140000 baud (0x15 clock ticks per signal)
		if((m_errorCode = FT_ClrDtr(m_falconDevice)) != FT_OK) return false;
		if((m_errorCode = FT_SetBaudRate(m_falconDevice, 140000)) != FT_OK) return false;

		//Send "A" character
		if((m_errorCode = write(check_msg_2, 1, bytes_written)) < 0) return false;

		//Expect back 2 bytes:
		// 0x13 0x41 
		if((m_errorCode = read(receive_buf, 2, bytes_read)) < 0) return false;
		return true;
	}

	bool FalconCommFTD2XX::setNormalMode()
	{
		if((m_errorCode = FT_SetBaudRate(m_falconDevice, 1456312)) != FT_OK) return false;
		return true;
	}


};
