/***
 * @file FalconCommLibFTDI.cpp
 * @brief LibFTDI (http://www.intra2net.com/de/produkte/opensource/ftdi/) based implementation of FTDI communication for the falcon
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @version $Id$
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL$
 * 
 * Project info at http://libnifalcon.sourceforge.net/ 
 *
 */
#include "falcon/comm/FalconCommLibFTDI.h"

namespace libnifalcon {

	FalconCommLibFTDI::FalconCommLibFTDI() :
		m_isInitialized(false)
	{
		//No way to poll, so we act like we always have something available
		m_hasBytesAvailable = true;
		initLibFTDI();
	}

	FalconCommLibFTDI::~FalconCommLibFTDI() 
	{
		close();
		ftdi_deinit(&(m_falconDevice));
	}
			
	bool FalconCommLibFTDI::initLibFTDI()
	{
#if defined(LIBUSB_DEBUG)
		//Spam libusb messages
		usb_set_debug(10);
#endif
		if((m_deviceErrorCode = ftdi_init(&(m_falconDevice))) < 0)
		{
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return false;
		}
		m_isInitialized = true;
		return true;
	}

	bool FalconCommLibFTDI::getDeviceCount(int8_t& device_count)
	{
		if(!m_isInitialized)
		{
			m_errorCode = FALCON_COMM_NOT_INITIALIZED;
			return false;
		}
		struct ftdi_device_list* dev_list[128];
		device_count = ftdi_usb_find_all(&(m_falconDevice), dev_list, FALCON_VENDOR_ID, FALCON_PRODUCT_ID);
		ftdi_list_free(dev_list);
		return true;
	}

	bool FalconCommLibFTDI::open(uint8_t device_index)
	{
		if(!m_isInitialized)
		{
			m_errorCode = FALCON_COMM_NOT_INITIALIZED;
			return false;
		}
		unsigned int count, i, status;
		struct ftdi_device_list *dev_list, *current;
		if(m_isCommOpen) close();

		count = ftdi_usb_find_all(&(m_falconDevice), &dev_list, FALCON_VENDOR_ID, FALCON_PRODUCT_ID);
		if(count <= 0 || device_index > count)
		{
			ftdi_list_free(&dev_list);
			if(count == 0)
			{
				m_errorCode = FALCON_COMM_DEVICE_NOT_FOUND_ERROR;
				return false;
			}
			else if(device_index > count)
			{
				m_errorCode = FALCON_COMM_DEVICE_INDEX_OUT_OF_RANGE_ERROR;
				return false;
			}
		}
		for(i = 0, current = dev_list; current != NULL && i < device_index; current = dev_list->next, ++i);
		if((m_deviceErrorCode = ftdi_usb_open_dev(&(m_falconDevice), current->dev)) < 0)
		{
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return false;
		}
		setNormalMode();
		ftdi_list_free(&dev_list);
		m_isCommOpen = true;
		if((m_deviceErrorCode = ftdi_usb_purge_buffers(&(m_falconDevice))) < 0) return false;
		setNormalMode();
		return true;
	}

	bool FalconCommLibFTDI::close()
	{
		if(!m_isCommOpen)
		{
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}
		if((m_deviceErrorCode = ftdi_usb_close(&m_falconDevice)) < 0)
		{
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return false;
		}
		m_isCommOpen = false;
		return true;
	}

	bool FalconCommLibFTDI::read(uint8_t* str, uint32_t size)
	{
		unsigned long bytes_rx;

		if(!m_isCommOpen)
		{
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}

		m_lastBytesRead = ftdi_read_data(&(m_falconDevice), str, size);
		if(m_lastBytesRead < 0)
		{
			m_deviceErrorCode = m_lastBytesRead;
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return false;
		}
		if(m_lastBytesRead < size)
		{
			m_errorCode = FALCON_COMM_READ_ERROR;
			return false;
		}
		return true;
	}

	bool FalconCommLibFTDI::write(uint8_t* str, uint32_t size)
	{
		if(!m_isCommOpen)
		{
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}

		m_lastBytesWritten = ftdi_write_data(&(m_falconDevice), str, size);
		if(m_lastBytesWritten < 0)
		{
			m_deviceErrorCode = m_lastBytesRead;
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return false;
		}
		if(m_lastBytesWritten < size)
		{
			m_errorCode = FALCON_COMM_WRITE_ERROR;
			return false;
		}
		
		return true;
	}

	bool FalconCommLibFTDI::setFirmwareMode()
	{
		unsigned int bytes_written, bytes_read;
		unsigned char check_msg_1_send[3] = {0x0a, 0x43, 0x0d};
		unsigned char check_msg_1_recv[4] = {0x0a, 0x44, 0x2c, 0x0d};
		unsigned char check_msg_2[1] = {0x41};
		unsigned char send_buf[128], receive_buf[128];
		int k;
	
		if(!m_isCommOpen)
		{
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}

		//Save ourselves having to reset this on every error
		m_errorCode = FALCON_COMM_DEVICE_ERROR;
		
		//Clear out current buffers to make sure we have a fresh start
		if((m_deviceErrorCode = ftdi_usb_purge_buffers(&(m_falconDevice))) < 0) return false;

		//Reset the device
		if((m_deviceErrorCode = ftdi_usb_reset(&(m_falconDevice))) < 0) return false;

		//Make sure our latency timer is at 16ms, otherwise firmware checks tend to always fail
		if((m_deviceErrorCode = ftdi_set_latency_timer(&(m_falconDevice), 16)) < 0) return false;
	
		//Set to:
		// 9600 baud
		// 8n1
		// No Flow Control
		// RTS Low
		// DTR High	
		if((m_deviceErrorCode = ftdi_set_baudrate(&(m_falconDevice), 9600)) < 0) return false;
		if((m_deviceErrorCode = ftdi_set_line_property(&(m_falconDevice), BITS_8, STOP_BIT_1, NONE)) < 0) return false;
		if((m_deviceErrorCode = ftdi_setflowctrl(&(m_falconDevice), SIO_DISABLE_FLOW_CTRL)) < 0) return false;
		if((m_deviceErrorCode = ftdi_setrts(&(m_falconDevice), 0)) < 0) return false;
		if((m_deviceErrorCode = ftdi_setdtr(&(m_falconDevice), 0)) < 0) return false;
		if((m_deviceErrorCode = ftdi_setdtr(&(m_falconDevice), 1)) < 0) return false;

		//Send 3 bytes: 0x0a 0x43 0x0d
		if(!write(check_msg_1_send, 3)) return false;
		if(!read(receive_buf, 4)) return false;
	
		//Set to:
		// DTR Low
		// 140000 baud (0x15 clock ticks per signal)
		if((m_deviceErrorCode = ftdi_setdtr(&(m_falconDevice),0)) < 0) return false;
		if((m_deviceErrorCode = ftdi_set_baudrate(&(m_falconDevice), 140000)) < 0) return false;

		//Send "A" character
		if(!write(check_msg_2, 1)) return false;

		//Expect back 2 bytes:
		// 0x13 0x41
		if(!read(receive_buf, 2)) return false;
		m_errorCode = 0;
		return true;
	}

	bool FalconCommLibFTDI::setNormalMode()
	{
		if(!m_isCommOpen)
		{
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}
		m_errorCode = FALCON_COMM_DEVICE_ERROR;
		if((m_deviceErrorCode = ftdi_set_latency_timer(&(m_falconDevice), 1)) < 0) return false;
		if((m_deviceErrorCode = ftdi_set_baudrate(&(m_falconDevice), 1456312)) < 0) return false;
		m_errorCode = 0;
		return true;
	}

};
