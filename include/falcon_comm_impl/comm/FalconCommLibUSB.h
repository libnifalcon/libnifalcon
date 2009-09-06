/***
 * @file FalconCommLibUSB.h
 * @brief LibUSB 1.0 (http://libusb.sourceforge.net/) based implementation of FTDI communication for the falcon
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
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
		virtual bool readBlocking(uint8_t*, unsigned int);
		virtual bool writeBlocking(uint8_t*, unsigned int);
		virtual bool setFirmwareMode();
		virtual bool setNormalMode();
		virtual void setBytesAvailable(uint32_t b);
		static void cb_in(struct libusb_transfer *transfer);
		static void cb_out(struct libusb_transfer *transfer);

		void setHasBytesAvailable(bool v);
		void setSent() { m_isWriteAllocated = false; }
		void setReceived()  { m_isReadAllocated = false; }
		bool initLibUSB();
		void poll();
		void reset();
		void issueRead();
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
