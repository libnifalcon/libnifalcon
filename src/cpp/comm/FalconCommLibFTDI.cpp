#include "FalconCommLibFTDI.h"

namespace libnifalcon {

	FalconCommLibFTDI::FalconCommLibFTDI() :
		m_isInitialized(false)
	{
		initLibFTDI();
	}

	FalconCommLibFTDI::~FalconCommLibFTDI() 
	{
		close();
	}
		
	
	bool FalconCommLibFTDI::initLibFTDI()
	{
		if((m_errorCode = ftdi_init(&(m_falconDevice))) < 0) return false;
		m_isInitialized = true;
		return true;
	}

	bool FalconCommLibFTDI::getDeviceCount(int8_t& device_count)
	{
		if(!m_isInitialized) return false;
		struct ftdi_device_list* dev_list[128];
		device_count = ftdi_usb_find_all(&(m_falconDevice), dev_list, FALCON_VENDOR_ID, FALCON_PRODUCT_ID);
		ftdi_list_free(dev_list);
		return true;
	}

	bool FalconCommLibFTDI::open(u_int8_t device_index)
	{
		if(!m_isInitialized) return false;
		unsigned int count, i, status;
		struct ftdi_device_list *dev_list, *current;
		if(m_isCommOpen) close();

		count = ftdi_usb_find_all(&(m_falconDevice), &dev_list, FALCON_VENDOR_ID, FALCON_PRODUCT_ID);
		if(count <= 0 || device_index > count)
		{
			ftdi_list_free(&dev_list);
			if(count == 0)
			{
				m_errorCode = NIFALCON_DEVICE_NOT_FOUND_ERROR;
				return false;
			}
			else if(device_index > count)
			{
				m_errorCode = NIFALCON_DEVICE_INDEX_OUT_OF_RANGE_ERROR;
				return false;
			}
		}
		for(i = 0, current = dev_list; current != NULL && i < device_index; current = dev_list->next, ++i);
		if((m_errorCode = ftdi_usb_open_dev(&(m_falconDevice), current->dev)) < 0) return false;
		setNormalMode();
		m_falconDevice.usb_read_timeout = 1000;
		ftdi_list_free(&dev_list);
		m_isCommOpen = true;
		return true;
	}

	bool FalconCommLibFTDI::close()
	{
		if(!m_isCommOpen)
		{
			m_errorCode = NIFALCON_DEVICE_NOT_FOUND_ERROR;
			return false;
		}
		if((m_errorCode = ftdi_usb_close(&m_falconDevice)) < 0) return false;
		m_isCommOpen = false;
		return true;
	}

	bool FalconCommLibFTDI::read(u_int8_t* str, u_int32_t size, u_int32_t& bytes_read)
	{
		unsigned long bytes_rx;
		clock_t timeout;

		if(!m_isCommOpen)
		{
			m_errorCode = NIFALCON_DEVICE_NOT_FOUND_ERROR;
			return false;
		}

		bytes_read = ftdi_read_data(&(m_falconDevice), str, size);
		if(bytes_read < 0)
		{
			m_errorCode = bytes_read;
			return false;
		}
		return true;
	}

	bool FalconCommLibFTDI::write(u_int8_t* str, u_int32_t size, u_int32_t& bytes_written)
	{
		if(!m_isCommOpen)
		{
			m_errorCode = NIFALCON_DEVICE_NOT_FOUND_ERROR;
			return false;
		}

		bytes_written = ftdi_write_data(&(m_falconDevice), str, size);
		if(bytes_written < 0)
		{
			m_errorCode = bytes_written;
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
			m_errorCode = NIFALCON_DEVICE_NOT_FOUND_ERROR;
			return false;
		}
  
		//Clear out current buffers to make sure we have a fresh start
		if((m_errorCode = ftdi_usb_purge_buffers(&(m_falconDevice))) < 0) return false;

		//Reset the device
		if((m_errorCode = ftdi_usb_reset(&(m_falconDevice))) < 0) return false;

		//Make sure our latency timer is at 16ms, otherwise firmware checks tend to always fail
		if((m_errorCode = ftdi_set_latency_timer(&(m_falconDevice), 16)) < 0) return false;
	
		//Set to:
		// 9600 baud
		// 8n1
		// No Flow Control
		// RTS Low
		// DTR High	
		if((m_errorCode = ftdi_set_baudrate(&(m_falconDevice), 9600)) < 0) return false;
		if((m_errorCode = ftdi_set_line_property(&(m_falconDevice), BITS_8, STOP_BIT_1, NONE)) < 0) return false;
		if((m_errorCode = ftdi_setflowctrl(&(m_falconDevice), SIO_DISABLE_FLOW_CTRL)) < 0) return false;
		if((m_errorCode = ftdi_setrts(&(m_falconDevice), 0)) < 0) return false;
		if((m_errorCode = ftdi_setdtr(&(m_falconDevice), 0)) < 0) return false;
		if((m_errorCode = ftdi_setdtr(&(m_falconDevice), 1)) < 0) return false;
		//Send 3 bytes: 0x0a 0x43 0x0d

		if((m_errorCode = write(check_msg_1_send, 3, bytes_written)) < 0) return false;
		if((m_errorCode = read(receive_buf, 4, bytes_read)) < 0) return false;
	
		//Set to:
		// DTR Low
		// 140000 baud (0x15 clock ticks per signal)
		if((m_errorCode = ftdi_setdtr(&(m_falconDevice),0)) < 0) return false;
		if((m_errorCode = ftdi_set_baudrate(&(m_falconDevice), 140000)) < 0) return false;

		//Send "A" character
		if((m_errorCode = write(check_msg_2, 1, bytes_written)) < 0) return false;

		//Expect back 2 bytes:
		// 0x13 0x41
		if((m_errorCode = read(receive_buf, 2, bytes_read)) < 0) return false;
		return true;
	}

	bool FalconCommLibFTDI::setNormalMode()
	{
		if(!m_isCommOpen)
		{
			m_errorCode = NIFALCON_DEVICE_NOT_FOUND_ERROR;
			return false;
		}
		if((m_errorCode = ftdi_set_latency_timer(&(m_falconDevice), 1)) < 0) return false;
		if((m_errorCode = ftdi_set_baudrate(&(m_falconDevice), 1456312)) < 0) return false;
		return true;
	}

};
