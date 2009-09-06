/***
 * @file FalconCommLibFTDI.cpp
 * @brief LibFTDI (http://www.intra2net.com/de/produkte/opensource/ftdi/) based implementation of FTDI communication for the falcon
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include "falcon/comm/FalconCommLibFTDI.h"
#include <cstring>
#include "ftdi.h"

namespace libnifalcon {

	FalconCommLibFTDI::FalconCommLibFTDI() :
		m_isInitialized(false),
		INIT_LOGGER("FalconCommLibFTDI")
	{
		LOG_INFO("Constructing object");
		m_falconDevice = new ftdi_context;
		//No way to poll, so we act like we always have something available
		m_hasBytesAvailable = true;
		initLibFTDI();
	}

	FalconCommLibFTDI::~FalconCommLibFTDI()
	{
		LOG_INFO("Destructing object");
		close();
		ftdi_deinit((m_falconDevice));
		delete m_falconDevice;
	}

	bool FalconCommLibFTDI::initLibFTDI()
	{
		LOG_INFO("Initializing communications");
#if defined(LIBUSB_DEBUG)
		//Spam libusb messages
		LOG_INFO("Setting libusb debug level");
		usb_set_debug(10);
#endif
		if((m_deviceErrorCode = ftdi_init((m_falconDevice))) < 0)
		{
			LOG_ERROR("Cannot initialize device");
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return false;
		}
		m_falconDevice->usb_read_timeout = 5000;
		m_falconDevice->usb_write_timeout = 5000;

		m_isInitialized = true;
		return true;
	}

	bool FalconCommLibFTDI::getDeviceCount(unsigned int& device_count)
	{
		LOG_INFO("Getting device count");
		if(!m_isInitialized)
		{
			LOG_ERROR("Device not initialized");
			m_errorCode = FALCON_COMM_NOT_INITIALIZED;
			return false;
		}
		struct ftdi_device_list* dev_list[128];
		device_count = ftdi_usb_find_all(m_falconDevice, dev_list, FALCON_VENDOR_ID, FALCON_PRODUCT_ID);
		ftdi_list_free(dev_list);
		return true;
	}

	bool FalconCommLibFTDI::open(unsigned int device_index)
	{
		LOG_INFO("Opening device " << device_index);
		if(!m_isInitialized)
		{
			LOG_ERROR("Device not initialized");
			m_errorCode = FALCON_COMM_NOT_INITIALIZED;
			return false;
		}
		unsigned int count, i, status;
		struct ftdi_device_list *dev_list, *current;
		if(m_isCommOpen) close();

		count = ftdi_usb_find_all((m_falconDevice), &dev_list, FALCON_VENDOR_ID, FALCON_PRODUCT_ID);
		if(count <= 0 || device_index > count)
		{
			ftdi_list_free(&dev_list);
			if(count == 0)
			{
				LOG_ERROR("No devices found");
				m_errorCode = FALCON_COMM_DEVICE_NOT_FOUND_ERROR;
				return false;
			}
			else if(device_index > count)
			{
				LOG_ERROR("Device index " << device_index << " out of range");
				m_errorCode = FALCON_COMM_DEVICE_INDEX_OUT_OF_RANGE_ERROR;
				return false;
			}
		}
		for(i = 0, current = dev_list; current != NULL && i < device_index; current = current->next, ++i);
		if(current==NULL)
		{
			LOG_ERROR("No devices found");
			m_errorCode = FALCON_COMM_DEVICE_NOT_FOUND_ERROR;
			return false;
		}
		if((m_deviceErrorCode = ftdi_usb_open_dev((m_falconDevice), current->dev)) < 0)
		{
			LOG_ERROR("Device error " << m_deviceErrorCode);
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			ftdi_list_free(&dev_list);
			return false;
		}
		ftdi_list_free(&dev_list);
		m_isCommOpen = true;
		//Purge buffers
		//if((m_deviceErrorCode = ftdi_usb_purge_buffers((m_falconDevice))) < 0) return false;
		setNormalMode();
		return true;
	}

	bool FalconCommLibFTDI::close()
	{
		LOG_INFO("Closing device");
		if(!m_isCommOpen)
		{
			LOG_ERROR("Device not open");
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}
		if((m_deviceErrorCode = ftdi_usb_close(m_falconDevice)) < 0)
		{
			LOG_ERROR("Device error " << m_deviceErrorCode);
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return false;
		}
		m_isCommOpen = false;
		return true;
	}

	bool FalconCommLibFTDI::read(uint8_t* str, unsigned int size)
	{
		LOG_DEBUG("Reading " << size << " bytes of data");
		unsigned long bytes_rx;

		if(!m_isCommOpen)
		{
			LOG_ERROR("Device not open");
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}

		m_lastBytesRead = ftdi_read_data((m_falconDevice), str, size);
		if(m_lastBytesRead < 0)
		{
			LOG_ERROR("Device error " << m_deviceErrorCode);
			m_deviceErrorCode = m_lastBytesRead;
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return false;
		}
		/*
		  if(m_lastBytesRead < size)
		  {
		  LOG_ERROR("Read amount " << m_lastBytesRead << " less than requested size " << size);
		  m_errorCode = FALCON_COMM_READ_ERROR;
		  return false;
		  }
		*/
		return true;
	}

	bool FalconCommLibFTDI::write(uint8_t* str, unsigned int size)
	{
		LOG_DEBUG("Writing " << size << " bytes of data");
		if(!m_isCommOpen)
		{
			LOG_ERROR("Device not open");
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}

		m_lastBytesWritten = ftdi_write_data((m_falconDevice), str, size);
		if(m_lastBytesWritten < 0)
		{
			LOG_ERROR("Device error " << m_deviceErrorCode);
			m_deviceErrorCode = m_lastBytesRead;
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return false;
		}
		if(m_lastBytesWritten < size)
		{
			LOG_ERROR("Write amount " << m_lastBytesWritten << " less than requested size " << size);
			m_errorCode = FALCON_COMM_WRITE_ERROR;
			return false;
		}
		return true;
	}

	bool FalconCommLibFTDI::setFirmwareMode()
	{
		unsigned int bytes_written, bytes_read;
		unsigned char check_msg_1_send[3] = {0x0a, 0x43, 0x0d};
		unsigned char check_msg_1_recv[5] = {0x00, 0x0a, 0x44, 0x2c, 0x0d};
		unsigned char check_msg_2[1] = {0x41};
		unsigned char send_buf[128], receive_buf[128];
		int k;

		LOG_INFO("Setting firmware communications mode");

		if(!m_isCommOpen)
		{
			LOG_ERROR("Device not open");
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}

		//Save ourselves having to reset this on every error
		m_errorCode = FALCON_COMM_DEVICE_ERROR;

		//Clear out current buffers to make sure we have a fresh start
		if((m_deviceErrorCode = ftdi_usb_purge_buffers((m_falconDevice))) < 0)
		{
			LOG_ERROR("Cannot purge buffers - Device error " << m_deviceErrorCode);
			return false;
		}

		//Reset the device

		if((m_deviceErrorCode = ftdi_usb_reset((m_falconDevice))) < 0)
		{
			LOG_ERROR("Cannot reset - Device error " << m_deviceErrorCode);
			return false;
		}

		//Make sure our latency timer is at 16ms, otherwise firmware checks tend to always fail
		if((m_deviceErrorCode = ftdi_set_latency_timer((m_falconDevice), 16)) < 0)
		{
			LOG_ERROR("Cannot set latency timer - Device error " << m_deviceErrorCode);
			return false;
		}


		//Set to:
		// 9600 baud
		// 8n1
		// No Flow Control
		// RTS Low
		// DTR High
		if((m_deviceErrorCode = ftdi_set_baudrate((m_falconDevice), 9600)) < 0)
		{
			LOG_ERROR("Cannot set baud rate - Device error " << m_deviceErrorCode);
			return false;
		}
		if((m_deviceErrorCode = ftdi_set_line_property((m_falconDevice), BITS_8, STOP_BIT_1, NONE)) < 0)
		{
			LOG_ERROR("Cannot set line properties - Device error " << m_deviceErrorCode);
			return false;
		}

		if((m_deviceErrorCode = ftdi_setflowctrl((m_falconDevice), SIO_DISABLE_FLOW_CTRL)) < 0)
		{
			LOG_ERROR("Cannot set flow control - Device error " << m_deviceErrorCode);
			return false;
		}

		if((m_deviceErrorCode = ftdi_setrts((m_falconDevice), 0)) < 0)
		{
			LOG_ERROR("Cannot set RTS properties - Device error " << m_deviceErrorCode);
			return false;
		}
		if((m_deviceErrorCode = ftdi_setdtr((m_falconDevice), 0)) < 0)
		{
			LOG_ERROR("Cannot set DTR properties (1) - Device error " << m_deviceErrorCode);
			return false;
		}

		if((m_deviceErrorCode = ftdi_setdtr((m_falconDevice), 1)) < 0)
		{
			LOG_ERROR("Cannot set DTR properties (2) - Device error " << m_deviceErrorCode);
			return false;
		}



		int i;
		for(i = 0; i < 10; ++i)
		{
			//Send 3 bytes: 0x0a 0x43 0x0d
			if(!write(check_msg_1_send, 3))
			{
				LOG_ERROR("Cannot write check values (1) - Device error " << m_deviceErrorCode);
				return false;
			}
			if(!read(receive_buf, 5))
			{
				LOG_ERROR("Cannot read check values (1) - Device error " << m_deviceErrorCode);
				return false;
			}
			//printf("CHECK 1 OUT %d 0x%x 0x%x 0x%x 0x%x 0x%x\n", m_lastBytesRead, receive_buf[0], receive_buf[1], receive_buf[2], receive_buf[3], receive_buf[4]);
			if(m_lastBytesRead != 5 || memcmp(receive_buf,check_msg_1_recv, 5))
			{
				continue;
			}
			break;
		}
		if(i == 10)
		{
			LOG_ERROR("Cannot match check values (1)");
			return false;
		}

		//Set to:
		// DTR Low
		// 140000 baud (0x15 clock ticks per signal)
		if((m_deviceErrorCode = ftdi_setdtr((m_falconDevice),0)) < 0)
		{
			LOG_ERROR("Cannot set DTR properties (3) - Device error " << m_deviceErrorCode);
			return false;
		}
		if((m_deviceErrorCode = ftdi_set_baudrate((m_falconDevice), 140000)) < 0)
		{
			LOG_ERROR("Cannot set baudrate for firmware load - Device error " << m_deviceErrorCode);
			return false;
		}

		//Send "A" character
		if(!write(check_msg_2, 1))
		{
			LOG_ERROR("Cannot write check values(2) - Device error " << m_deviceErrorCode);
			return false;
		}

		//Expect back 1 bytes:
		// 0x41
		if(!read(receive_buf, 1))
		{
			LOG_ERROR("Cannot read check values(2) - Device error " << m_deviceErrorCode);
			return false;
		}
		if(m_lastBytesRead != 1 || receive_buf[0] != 0x41)
		{
			LOG_ERROR("Cannot match check values(2)");
			return false;
		}

		m_errorCode = 0;
		return true;
	}

	bool FalconCommLibFTDI::setNormalMode()
	{
		LOG_INFO("Setting normal communications mode");
		if(!m_isCommOpen)
		{
			LOG_ERROR("Device not open");
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}
		m_errorCode = FALCON_COMM_DEVICE_ERROR;
		if((m_deviceErrorCode = ftdi_set_latency_timer((m_falconDevice), 1)) < 0)
		{
			LOG_ERROR("Cannot set latency timers - Device error " << m_deviceErrorCode);
			return false;
		}
		if((m_deviceErrorCode = ftdi_set_baudrate((m_falconDevice), 1456312)) < 0)
		{
			LOG_ERROR("Cannot set baud rate - Device error " << m_deviceErrorCode);
			return false;
		}
		m_errorCode = 0;
		return true;
	}

};
