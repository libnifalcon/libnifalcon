/***
 * @file FalconCommLibUSB.h
 * @brief LibUSB 1.0 (http://libusb.sourceforge.net/) based implementation of FTDI communication for the falcon
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @version $Id: FalconCommLibFTDI.h 226 2008-10-19 01:32:09Z kmachulis $
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL: https://kmachulis@libnifalcon.svn.sourceforge.net/svnroot/libnifalcon/trunk/libnifalcon/src/cpp/comm/FalconCommLibFTDI.h $
 *
 * Project info at http://libnifalcon.sourceforge.net/
 *
 */
#ifndef FALCONCOMMLIBUSB_H
#define FALCONCOMMLIBUSB_H

#include "falcon/core/FalconComm.h"

struct timeval;
struct libusb_device_handle;
struct libusb_transfer;
struct libusb_context;

namespace libnifalcon
{

	class FalconCommLibUSB : public FalconComm
	{
	public:
		FalconCommLibUSB();
		~FalconCommLibUSB();
		virtual bool getDeviceCount(unsigned int& );
		virtual bool open(unsigned int );
		virtual bool close();
		virtual bool read(uint8_t*, unsigned int);
		virtual bool write(uint8_t*, unsigned int);
		virtual bool setFirmwareMode();
		virtual bool setNormalMode();
		static void cb_in(struct libusb_transfer *transfer);
		static void cb_out(struct libusb_transfer *transfer);
		void setHasBytesAvailable(bool v);
		void setSent() { m_isWriteAllocated = false; }
		void setReceived()  { m_isReadAllocated = false; }
		bool initLibUSB();
		void poll();
		void reset();
	protected:
		bool m_isWriteAllocated;
		bool m_isReadAllocated;
		struct timeval* m_tv;
		libusb_device_handle* m_falconDevice;
		unsigned char input[128], output[128];
		struct libusb_transfer* in_transfer;
		struct libusb_transfer* out_transfer;
		struct libusb_context* m_usbContext;
	private:
		DECLARE_LOGGER();
	};
};

#endif
