/***
 * @file FalconCommLibUSB.cpp
 * @brief LibUSB (http://libusb.sourceforge.net/) based implementation of FTDI communication for the falcon
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include <boost/bind.hpp>
#include "falcon/comm/FalconCommLibUSB.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include "libusb.h"

// Taken from LibFTDI
// http://www.intra2net.com/en/developer/libftdi/
// I just don't want to wait for the libusb 1.0 port of libftdi
// And I don't exactly want to do it myself
// So, here we are. File level defines.
#define SIO_RESET          0 /* Reset the port */
#define SIO_MODEM_CTRL     1 /* Set the modem control register */
#define SIO_SET_FLOW_CTRL  2 /* Set flow control register */
#define SIO_SET_BAUD_RATE  3 /* Set baud rate */
#define SIO_SET_DATA       4 /* Set the data characteristics of the port */

#define SIO_RESET_REQUEST_TYPE 0x40
#define SIO_RESET_REQUEST SIO_RESET
#define SIO_RESET_SIO 0
#define SIO_RESET_PURGE_RX 1
#define SIO_RESET_PURGE_TX 2

#define SIO_SET_BAUDRATE_REQUEST_TYPE 0x40
#define SIO_SET_BAUDRATE_REQUEST SIO_SET_BAUD_RATE

#define SIO_SET_DATA_REQUEST_TYPE 0x40
#define SIO_SET_DATA_REQUEST SIO_SET_DATA

#define SIO_SET_FLOW_CTRL_REQUEST SIO_SET_FLOW_CTRL
#define SIO_SET_FLOW_CTRL_REQUEST_TYPE 0x40

#define SIO_DISABLE_FLOW_CTRL 0x0
#define SIO_RTS_CTS_HS (0x1 << 8)
#define SIO_DTR_DSR_HS (0x2 << 8)
#define SIO_XON_XOFF_HS (0x4 << 8)

#define SIO_SET_MODEM_CTRL_REQUEST_TYPE 0x40
#define SIO_SET_MODEM_CTRL_REQUEST SIO_MODEM_CTRL

#define SIO_SET_DTR_MASK 0x1
#define SIO_SET_DTR_HIGH ( 1 | ( SIO_SET_DTR_MASK  << 8))
#define SIO_SET_DTR_LOW  ( 0 | ( SIO_SET_DTR_MASK  << 8))
#define SIO_SET_RTS_MASK 0x2
#define SIO_SET_RTS_HIGH ( 2 | ( SIO_SET_RTS_MASK << 8 ))
#define SIO_SET_RTS_LOW ( 0 | ( SIO_SET_RTS_MASK << 8 ))

#define SIO_RTS_CTS_HS (0x1 << 8)

#define INTERFACE_ANY 0
#define INTERFACE_A 1
#define INTERFACE_B 2


namespace libnifalcon
{

	FalconCommLibUSB::FalconCommLibUSB() :
		m_isWriteAllocated(false),
		m_isReadAllocated(false),
		INIT_LOGGER("FalconCommLibUSB")
	{
		LOG_INFO("Constructing object");
		m_tv = new timeval;
		m_tv->tv_sec = 0;
		m_tv->tv_usec = 100;
		initLibUSB();
	}

	FalconCommLibUSB::~FalconCommLibUSB()
	{
		if(m_isCommOpen)
		{
			close();
		}
		reset();
		//libusb_exit(m_usbContext);
		delete m_tv;
		LOG_INFO("Destructing object");
	}

	bool FalconCommLibUSB::initLibUSB()
	{
		LOG_INFO("Initializing communications");
		if((m_deviceErrorCode = libusb_init(&m_usbContext)) < 0)
		{
			LOG_ERROR("Failed to initialize");
			return false;
		}
#if defined(LIBUSB_DEBUG)
		//Spam libusb messages
		//Between 0-3 for libusb 1.0
		LOG_INFO("Setting libusb debug level to 3");
		libusb_set_debug(m_usbContext, 3);
#else
		LOG_INFO("Setting libusb debug level to 0");
		libusb_set_debug(m_usbContext, 0);
#endif
	}

	//Ripped out of libusb_open_device_with_vid_pid
	bool FalconCommLibUSB::getDeviceCount(unsigned int& count)
	{
		LOG_INFO("Getting device count");
		struct libusb_device **devs;
		struct libusb_device *found = NULL;
		struct libusb_device *dev;
		size_t i = 0;
		count = 0;

		if (libusb_get_device_list(m_usbContext, &devs) < 0)
		{
			LOG_ERROR("Device list not retrievable");
			return false;
		}

		while ((dev = devs[i++]) != NULL)
		{
			struct libusb_device_descriptor desc;
			m_deviceErrorCode = libusb_get_device_descriptor(dev, &desc);
			if (m_deviceErrorCode < 0)
			{
				LOG_ERROR("Device descriptor not retrievable - Device error code " << m_deviceErrorCode);
				break;
			}
			if (desc.idVendor == FALCON_VENDOR_ID && desc.idProduct == FALCON_PRODUCT_ID)
			{
				++count;
			}
		}

		libusb_free_device_list(devs, 1);
		return true;
	}

	//Ripped out of libusb_open_device_with_vid_pid
	bool FalconCommLibUSB::open(unsigned int index)
	{
		LOG_INFO("Opening device");
		struct libusb_device **devs;
		struct libusb_device *found = NULL;
		struct libusb_device *dev;
		size_t i = 0;
		int count = 0;


		if ((m_deviceErrorCode = libusb_get_device_list(m_usbContext, &devs)) < 0)
		{
			LOG_ERROR("Device list not retrievable - Device error code " << m_deviceErrorCode);
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return false;
		}

		while ((dev = devs[i++]) != NULL)
		{
			struct libusb_device_descriptor desc;
			m_deviceErrorCode = libusb_get_device_descriptor(dev, &desc);
			if (m_deviceErrorCode < 0)
			{
				LOG_ERROR("Device descriptor not retrievable - Device error code " << m_deviceErrorCode);
				m_errorCode = FALCON_COMM_DEVICE_ERROR;
				libusb_free_device_list(devs, 1);
				return false;
			}
			if (desc.idVendor == FALCON_VENDOR_ID && desc.idProduct == FALCON_PRODUCT_ID)
			{
				if(count == index)
				{
					found = dev;
					break;
				}
				++count;
			}
		}

		if (found)
		{
			m_deviceErrorCode = libusb_open(found, &m_falconDevice);
			if (m_deviceErrorCode < 0)
			{
				LOG_ERROR("Cannot open device - Device error code " << m_deviceErrorCode);
				m_falconDevice = NULL;
				m_errorCode = FALCON_COMM_DEVICE_ERROR;
				libusb_free_device_list(devs, 1);
				return false;
			}
		}
		else
		{
			LOG_ERROR("Device index " << index << " out of range");
			m_errorCode = FALCON_COMM_DEVICE_INDEX_OUT_OF_RANGE_ERROR;
			return false;
		}


		if ((m_deviceErrorCode = libusb_claim_interface(m_falconDevice, 0)) < 0)
		{
			LOG_ERROR("Cannot claim device interface - Device error code " << m_deviceErrorCode);
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			return false;
		}
		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_RESET_REQUEST_TYPE, SIO_RESET_REQUEST, SIO_RESET_PURGE_RX, INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			LOG_ERROR("Cannot rx purge - Device error code " << m_deviceErrorCode);
			return false;
		}
		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_RESET_REQUEST_TYPE, SIO_RESET_REQUEST, SIO_RESET_PURGE_TX, INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			LOG_ERROR("Cannot tx purge - Device error code " << m_deviceErrorCode);
			return false;
		}
		reset();
		m_isCommOpen = true;
		setNormalMode();

		return true;
	}

	bool FalconCommLibUSB::close()
	{
		LOG_INFO("Closing device communications");
		if(!m_isCommOpen)
		{
			LOG_ERROR("Device not open");
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}
		m_isCommOpen = false;

		if ((m_deviceErrorCode = libusb_release_interface(m_falconDevice, 0)) < 0)
		{
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			LOG_ERROR("Cannot release device interface - Device error code " << m_deviceErrorCode);
			return false;
		}

		libusb_close(m_falconDevice);
		reset();
		m_falconDevice = NULL;
		return true;
	}

	void FalconCommLibUSB::setHasBytesAvailable(bool v)
	{
		m_hasBytesAvailable = true;
	}

	bool FalconCommLibUSB::read(uint8_t* buffer, unsigned int size)
	{
		LOG_DEBUG("Reading " << size << " bytes");
		if(!m_isCommOpen)
		{
			LOG_ERROR("Device not open");
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}
		if(m_hasBytesAvailable && m_bytesAvailable == 0)
		{
			issueRead();
			m_hasBytesAvailable = false;
			m_lastBytesRead = 0;
			return true;
		}
		if(size > 0 && size < m_bytesAvailable)
		{
			memcpy(buffer, output, size);
			memcpy(output, output+size, m_bytesAvailable-size);
			m_lastBytesRead = size;
			m_bytesAvailable -= size;
		}
		else if (size >= m_bytesAvailable)
		{
			memcpy(buffer, output, m_bytesAvailable);
			m_lastBytesRead = m_bytesAvailable;
			m_bytesAvailable = 0;
			m_hasBytesAvailable = false;
		}
		return true;
	}

	bool FalconCommLibUSB::write(uint8_t* buffer, unsigned int size)
	{
		LOG_DEBUG("Writing " << size << " bytes");
		if(!m_isCommOpen)
		{
			LOG_ERROR("Device not open");
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}

		m_lastBytesWritten = size;
		in_transfer = libusb_alloc_transfer(0);
		if (!in_transfer)
		{
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			LOG_ERROR("Cannot allocate inbound transfer");
			return false;
		}

		libusb_fill_bulk_transfer(in_transfer, m_falconDevice, 0x02, buffer,
								  size, FalconCommLibUSB::cb_in, this, 0);
		libusb_submit_transfer(in_transfer);
		m_isWriteAllocated = true;
		m_hasBytesAvailable = false;
		issueRead();
		return true;
	}

	bool FalconCommLibUSB::readBlocking(uint8_t* buffer, unsigned int size)
	{
		LOG_DEBUG("Reading " << size << " bytes blocking");
		if(!m_isCommOpen)
		{
			LOG_ERROR("Device not open");
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}

		if((m_deviceErrorCode = libusb_bulk_transfer(m_falconDevice, 0x81, buffer, size+2, &m_lastBytesRead, 1000)) != 0)
		{
			LOG_ERROR("Cannot do blocking read - Device error " << m_deviceErrorCode);
			return false;
		}
		m_lastBytesRead -= 2;
		if(m_lastBytesRead != 0)
			memcpy(buffer, buffer+2, m_lastBytesRead);
		LOG_DEBUG("Read " << m_lastBytesRead << " bytes while blocking");
		return true;
	}

	bool FalconCommLibUSB::writeBlocking(uint8_t* buffer, unsigned int size)
	{
		LOG_DEBUG("Writing " << size << " bytes blocking");
		if(!m_isCommOpen)
		{
			LOG_ERROR("Device not open");
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}

		if((m_deviceErrorCode = libusb_bulk_transfer(m_falconDevice, 0x2, buffer, size, &m_lastBytesWritten, 1000)) != 0)
		{
			LOG_ERROR("Cannot do blocking write - Device error " << m_deviceErrorCode);
			return false;
		}
		LOG_DEBUG("Wrote " << m_lastBytesWritten << " bytes while blocking");
		return true;
	}

	bool FalconCommLibUSB::setFirmwareMode()
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

		//reset();

		//Clear out current buffers to make sure we have a fresh start
		//if((m_deviceErrorCode = ftdi_usb_purge_buffers(&(m_falconDevice))) < 0) return false;

		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_RESET_REQUEST_TYPE, SIO_RESET_REQUEST, SIO_RESET_PURGE_RX, INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			LOG_ERROR("Cannot purge buffers - Device error " << m_deviceErrorCode);
			return false;
		}
		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_RESET_REQUEST_TYPE, SIO_RESET_REQUEST, SIO_RESET_PURGE_TX, INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			LOG_ERROR("Cannot purge buffers - Device error " << m_deviceErrorCode);
			return false;
		}

		//Reset the device
		//if((m_deviceErrorCode = ftdi_usb_reset(&(m_falconDevice))) < 0) return false;

		//Make sure our latency timer is at 16ms, otherwise firmware checks tend to always fail
		//if((m_deviceErrorCode = ftdi_set_latency_timer(&(m_falconDevice), 16)) < 0) return false;
	    if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, 0x40, 0x09, 16, INTERFACE_ANY, NULL, 0, 1000)) != 0)
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

		//if((m_deviceErrorCode = ftdi_set_baudrate(&(m_falconDevice), 9600)) < 0) return false;
		//Baud for 9600 is 0x4138. Just trust me. It is.
		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_SET_BAUDRATE_REQUEST_TYPE, SIO_SET_BAUDRATE_REQUEST, 0x4138, INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			LOG_ERROR("Cannot set baud rate - Device error " << m_deviceErrorCode);
			return false;
		}

		//if((m_deviceErrorCode = ftdi_set_line_property(&(m_falconDevice), BITS_8, STOP_BIT_1, NONE)) < 0) return false;
		//BITS_8 = 8, STOP_BIT_1 = 0, NONE = 0
		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_SET_DATA_REQUEST_TYPE, SIO_SET_DATA_REQUEST, (8 | (0x00 << 11) | (0x00 << 8)), INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			LOG_ERROR("Cannot set line properties - Device error " << m_deviceErrorCode);
			return false;
		}

		//if((m_deviceErrorCode = ftdi_setflowctrl(&(m_falconDevice), SIO_DISABLE_FLOW_CTRL)) < 0) return false;
		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_SET_FLOW_CTRL_REQUEST_TYPE, SIO_SET_FLOW_CTRL_REQUEST, 0, INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			LOG_ERROR("Cannot set flow control - Device error " << m_deviceErrorCode);
			return false;
		}

		//if((m_deviceErrorCode = ftdi_setrts(&(m_falconDevice), 0)) < 0) return false;
		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_SET_MODEM_CTRL_REQUEST_TYPE, SIO_SET_MODEM_CTRL_REQUEST, SIO_SET_RTS_LOW, INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			LOG_ERROR("Cannot set RTS properties - Device error " << m_deviceErrorCode);
			return false;
		}

		//if((m_deviceErrorCode = ftdi_setdtr(&(m_falconDevice), 0)) < 0) return false;
		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_SET_MODEM_CTRL_REQUEST_TYPE, SIO_SET_MODEM_CTRL_REQUEST, SIO_SET_DTR_LOW, INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			LOG_ERROR("Cannot set DTR properties (1) - Device error " << m_deviceErrorCode);
			return false;
		}

		//if((m_deviceErrorCode = ftdi_setdtr(&(m_falconDevice), 1)) < 0) return false;
		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_SET_MODEM_CTRL_REQUEST_TYPE, SIO_SET_MODEM_CTRL_REQUEST, SIO_SET_DTR_HIGH, INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			LOG_ERROR("Cannot set DTR properties (2) - Device error " << m_deviceErrorCode);
			return false;
		}

		int i;
		for(i = 0; i < 100; ++i)
		{
			//Send 3 bytes: 0x0a 0x43 0x0d
			if((m_deviceErrorCode = libusb_bulk_transfer(m_falconDevice, 0x2, check_msg_1_send, 3, &m_lastBytesWritten, 1000)) != 0)
			{
				LOG_ERROR("Cannot write check values (1) - Device error " << m_deviceErrorCode);
				return false;
			}

			//Expect back 5 bytes: 0x00 0xa 0x44 0x2c 0xd
			if((m_deviceErrorCode = libusb_bulk_transfer(m_falconDevice, 0x81, receive_buf, 7, &m_lastBytesRead, 1000)) != 0)
			{
				LOG_ERROR("Cannot read check values (1) - Device error " << m_deviceErrorCode);
				return false;
			}
			//printf("CHECK 1 OUT %d 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", m_lastBytesRead, receive_buf[0], receive_buf[1], receive_buf[2], receive_buf[3], receive_buf[4], receive_buf[5], receive_buf[6]);
			//Apple case
			if(m_lastBytesRead == 7 && memcmp(receive_buf+2,check_msg_1_recv, 5) == 0)
			{
				break;
			}
			//Linux case
			if(m_lastBytesRead == 6 && memcmp(receive_buf+2,check_msg_1_recv+1, 4) == 0)
			{
				break;
			}
		}
		if(i == 100)
		{
			LOG_ERROR("Cannot match check values(1)");
			return false;
		}




		//Set to:
		// DTR Low
		// 140000 baud (0x15 clock ticks per signal)

		//if((m_deviceErrorCode = ftdi_setdtr(&(m_falconDevice),0)) < 0) return false;
		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_SET_MODEM_CTRL_REQUEST_TYPE, SIO_SET_MODEM_CTRL_REQUEST, SIO_SET_DTR_LOW, INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			LOG_ERROR("Cannot set DTR properties (3) - Device error " << m_deviceErrorCode);
			return false;
		}

		//if((m_deviceErrorCode = ftdi_set_baudrate(&(m_falconDevice), 140000)) < 0) return false;
		if (m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_SET_BAUDRATE_REQUEST_TYPE, SIO_SET_BAUDRATE_REQUEST, 0x15, INTERFACE_ANY, NULL, 0, 1000) != 0)
		{
			LOG_ERROR("Cannot set baudrate for firmware load - Device error " << m_deviceErrorCode);
			return false;
		}

		//Send "A" character
		if((m_deviceErrorCode = libusb_bulk_transfer(m_falconDevice, 0x2, check_msg_2, 1, &m_lastBytesWritten, 1000)) != 0)
		{
			LOG_ERROR("Cannot write check values(2) - Device error " << m_deviceErrorCode);
			return false;
		}
		//Expect back 1 byte:
		//0x41
		if((m_deviceErrorCode = libusb_bulk_transfer(m_falconDevice, 0x81, receive_buf, 3, &m_lastBytesRead, 1000)) != 0)
		{
			LOG_ERROR("Cannot read check values(2) - Device error " << m_deviceErrorCode);
			return false;
		}
		if(m_lastBytesRead != 3 || receive_buf[2] != 0x41)
		{
			LOG_ERROR("Cannot match check values(2)");
			return false;
		}

		m_errorCode = 0;

		return true;
	}

	bool FalconCommLibUSB::setNormalMode()
	{
		LOG_INFO("Setting normal communications mode");
		if(!m_isCommOpen)
		{
			LOG_ERROR("Device not open");
			m_errorCode = FALCON_COMM_DEVICE_NOT_VALID_ERROR;
			return false;
		}
		if(m_isWriteAllocated || m_isReadAllocated)
		{
			reset();
		}

		m_errorCode = FALCON_COMM_DEVICE_ERROR;
		if (m_deviceErrorCode = libusb_control_transfer(m_falconDevice, 0x40, 0x09, 1, INTERFACE_ANY, NULL, 0, 1000) != 0)
		{
			LOG_ERROR("Cannot set latency timers - Device error " << m_deviceErrorCode);
			return false;
		}
		if (m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_SET_BAUDRATE_REQUEST_TYPE, SIO_SET_BAUDRATE_REQUEST, 0x2, INTERFACE_ANY, NULL, 0, 0) != 0)
		{
			LOG_ERROR("Cannot set baud rate - Device error " << m_deviceErrorCode);
			return false;
		}

		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_RESET_REQUEST_TYPE, SIO_RESET_REQUEST, SIO_RESET_PURGE_RX, INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			LOG_ERROR("Cannot purge buffers - Device error " << m_deviceErrorCode);
			return false;
		}
		if ((m_deviceErrorCode = libusb_control_transfer(m_falconDevice, SIO_RESET_REQUEST_TYPE, SIO_RESET_REQUEST, SIO_RESET_PURGE_TX, INTERFACE_ANY, NULL, 0, 1000)) != 0)
		{
			LOG_ERROR("Cannot purge buffers - Device error " << m_deviceErrorCode);
			return false;
		}
		m_errorCode = 0;
		return true;
	}

	void FalconCommLibUSB::poll()
	{
		libusb_handle_events_timeout(m_usbContext, m_tv);
	}

	void FalconCommLibUSB::reset()
	{
		if(m_isWriteAllocated)
		{
			libusb_cancel_transfer(in_transfer);
			setSent();
		}
		if(m_isReadAllocated)
		{
			libusb_cancel_transfer(out_transfer);
			setReceived();
		}
	}

	void FalconCommLibUSB::issueRead()
	{
		//If a read is already allocated, don't reallocate
		//We'll expect someone else to do this for us again later
		if(m_isReadAllocated)
		{
			return;
		}

		//Try to read over 64 and you'll fry libusb-1.0. Try to read under
		//64 and you'll fry OS X. So, read 64.
		out_transfer = libusb_alloc_transfer(0);
		if (!out_transfer)
		{
			m_errorCode = FALCON_COMM_DEVICE_ERROR;
			LOG_ERROR("Cannot allocate outbound transfer");
			return;
		}

		libusb_fill_bulk_transfer(out_transfer, m_falconDevice, 0x81, output,
								  64, FalconCommLibUSB::cb_out, this, 1000);
		libusb_submit_transfer(out_transfer);
		m_isReadAllocated = true;

	}

	void FalconCommLibUSB::setBytesAvailable(uint32_t b)
	{
		//Shift out modem bytes
		if(b > 2)
		{
			memcpy(output, output+2, b - 2);
			FalconComm::setBytesAvailable(b - 2);
			return;
		}
		FalconComm::setBytesAvailable(0);
	}

	void FalconCommLibUSB::cb_in(struct libusb_transfer *transfer)
	{
		((FalconCommLibUSB*)transfer->user_data)->setSent();
		libusb_free_transfer(transfer);
	}

	void FalconCommLibUSB::cb_out(struct libusb_transfer *transfer)
	{
		if(transfer->status == LIBUSB_TRANSFER_COMPLETED && transfer->actual_length >= 2)
		{
			((FalconCommLibUSB*)transfer->user_data)->setBytesAvailable(transfer->actual_length);
			((FalconCommLibUSB*)transfer->user_data)->setHasBytesAvailable(true);
			((FalconCommLibUSB*)transfer->user_data)->setReceived();
		}
		//We will always at least get modem status bytes back
		//If we get nothing back, we have disconnected.
		//Thanks to Devanshi Shah for this fix.
		else
		{
			LOG_ERROR("Device disconnected");
			((FalconCommLibUSB*)transfer->user_data)->m_isCommOpen = false;
			((FalconCommLibUSB*)transfer->user_data)->setBytesAvailable(0);
			((FalconCommLibUSB*)transfer->user_data)->setHasBytesAvailable(false);
			((FalconCommLibUSB*)transfer->user_data)->setReceived();
		}
		libusb_free_transfer(transfer);
	}

}
