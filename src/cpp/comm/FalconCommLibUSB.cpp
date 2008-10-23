#include "FalconCommLibUSB.h"
#include <iostream>
#include <cstdio>

namespace libnifalcon
{

	FalconCommLibUSB::FalconCommLibUSB()
	{
		m_tv.tv_sec = 0;
		m_tv.tv_usec = 100;
		m_requiresPoll = true;
		initLibUSB();
#if defined(LIBUSB_DEBUG)
		//Spam libusb messages
		libusb_set_debug(NULL, 4);
#endif

	}
	
	FalconCommLibUSB::~FalconCommLibUSB()
	{
		libusb_free_transfer(in_transfer);
		libusb_free_transfer(out_transfer);		
	}
	
	bool FalconCommLibUSB::initLibUSB()
	{
		int r = libusb_init(NULL);
		if (r < 0)
		{
			std::cout << "failed to initialise libusb" << std::endl;
			return false;
		}
		return true;
	}
	bool FalconCommLibUSB::getDeviceCount(int8_t& count)
	{
		count = 1;
		return true;
	}
	bool FalconCommLibUSB::open(uint8_t index)
	{
		m_falconDevice = libusb_open_device_with_vid_pid(NULL, FALCON_VENDOR_ID, FALCON_PRODUCT_ID);
		if(m_falconDevice == 0)
		{
			std::cout << "Cannot open device "<< std::endl;
			return false;
		}
			
		int r = libusb_claim_interface(m_falconDevice, 0);
		if (r < 0) {
			std::cout << "usb_claim_interface error " << r << std::endl;
			return false;
		}
		out_transfer = libusb_alloc_transfer(0);
		if (!out_transfer)
		{
			std::cout << "Cannot allocate out transfer" << std::endl;
			return false;
		}
		in_transfer = libusb_alloc_transfer(0);
		if (!in_transfer)
		{
			std::cout << "Cannot allocate in transfer\n" << std::endl;
			return false;
		}
		return true;
	}

	bool FalconCommLibUSB::close()
	{
		libusb_close(m_falconDevice);
		return true;
	}

	void FalconCommLibUSB::setHasBytesAvailable(bool v)
	{
		m_hasBytesAvailable = true;
	}
	
	bool FalconCommLibUSB::read(uint8_t* buffer, u_int32_t size)
	{
		//Plus 2. Stupid modem bits.
		if(size > 0)
			memcpy(buffer, output+2, size);
		m_hasBytesAvailable = false;
		m_bytesAvailable -= size;
		return true;
	}
	
	bool FalconCommLibUSB::write(uint8_t* buffer, u_int32_t size)
	{		
		libusb_fill_bulk_transfer(in_transfer, m_falconDevice, 0x02, buffer,
								  size, FalconCommLibUSB::cb_in, NULL, 0);
		libusb_submit_transfer(in_transfer);
		
		libusb_fill_bulk_transfer(out_transfer, m_falconDevice, 0x81, output,
								  64, FalconCommLibUSB::cb_out, this, 1000);
		libusb_submit_transfer(out_transfer);
		return true;
	}
	bool FalconCommLibUSB::setFirmwareMode()
	{
		return true;
	}
	bool FalconCommLibUSB::setNormalMode()
	{
		return true;		
	}

	void FalconCommLibUSB::poll()
	{
		libusb_handle_events_timeout(NULL, &m_tv);
	}
	
	void FalconCommLibUSB::cb_in(struct libusb_transfer *transfer)
	{

	}

	void FalconCommLibUSB::cb_out(struct libusb_transfer *transfer)
	{
		((FalconCommLibUSB*)transfer->user_data)->setBytesAvailable(transfer->actual_length - 2);
		((FalconCommLibUSB*)transfer->user_data)->setHasBytesAvailable(true);
		//Minus 2. Stupid modem bits.

	}

}
