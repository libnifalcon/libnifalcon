/***
 * @file FalconCommFTD2XX.cpp
 * @brief FTD2XX (http://www.ftdichip.com) based implementation of FTDI communication for the falcon
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include "falcon/comm/FalconCommFTD2XX.h"

#ifdef WIN32
#include <windows.h>
#else
#include <WinTypes.h>
#endif
#include "ftd2xx.h"

#include <cstring>
#include <iostream>

namespace libnifalcon
{
 	const char* FalconCommFTD2XX::FALCON_DESCRIPTION = "FALCON HAPTIC";

	FalconCommFTD2XX::~FalconCommFTD2XX()
	{
		close();
	}

	bool FalconCommFTD2XX::getDeviceCount(unsigned int& device_count)
	{
		device_count = openDeviceFTD2XX(0, true);
		return true;
	}

	bool FalconCommFTD2XX::open(unsigned int index)
	{
		return openDeviceFTD2XX(index, false) >= 0;
	}

	int8_t FalconCommFTD2XX::openDeviceFTD2XX(unsigned int device_index, bool stop_at_count)
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

	bool FalconCommFTD2XX::read(uint8_t* str, unsigned int size)
	{
		unsigned long bytes_rx = 0, b_read = 0, bytes_read = 0;

		m_lastBytesRead = 0;
		if(!m_isCommOpen)
		{
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}
		m_errorCode = FALCON_COMM_DEVICE_ERROR;
		if(size <= m_bytesAvailable) bytes_read = size;
		else bytes_read = m_bytesAvailable;

		if((m_deviceErrorCode = FT_Read(m_falconDevice, str, bytes_read, &b_read)) != FT_OK) return false;

		m_lastBytesRead = b_read;
		m_bytesAvailable -= b_read;
		if(m_bytesAvailable == 0) m_hasBytesAvailable = false;
		m_errorCode = 0;
		return true;
	}

	bool FalconCommFTD2XX::write(uint8_t* str, unsigned int size)
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

	bool FalconCommFTD2XX::readBlocking(uint8_t* buffer, unsigned int size)
	{
		while(m_bytesAvailable != size)
		{
			poll();
		}
		return read(buffer, size);
	}

	void FalconCommFTD2XX::poll()
	{
		if(!m_isCommOpen)
		{
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return;
		}
		if((m_deviceErrorCode = FT_GetQueueStatus(m_falconDevice, (DWORD*)&m_bytesAvailable)) != FT_OK) return;
		m_hasBytesAvailable = (m_bytesAvailable > 0);
	}

	bool FalconCommFTD2XX::setFirmwareMode()
	{
		uint8_t check_msg_1_send[3] = {0x0a, 0x43, 0x0d};
		uint8_t check_msg_1_recv[5] = {0x00, 0x0a, 0x44, 0x2c, 0x0d};
		uint8_t check_msg_2[1] = {0x41};
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


		int i;
		if(!write((uint8_t*)check_msg_1_send, (uint32_t)3)) return false;
		for(i = 0; i < 100; ++i)
		{
			//Send 3 bytes: 0x0a 0x43 0x0d

			//Expect 4 bytes back (LibFTDI expects 5. This expects 4. I dunno.)
			if(!readBlocking(receive_buf, 5)) return false;
			//printf("CHECK 1 OUT %d 0x%x 0x%x 0x%x 0x%x 0x%x\n", m_lastBytesRead, receive_buf[0], receive_buf[1], receive_buf[2], receive_buf[3], receive_buf[4]);
			if(m_lastBytesRead != 5 || memcmp(receive_buf,check_msg_1_recv, 5))
			{
				continue;
				//return false;
			}
			break;
		}
		if(i == 100)
		{
			return false;
		}

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
		if(!readBlocking(receive_buf, 1)) return false;
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
