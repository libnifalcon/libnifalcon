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

#include "core/FalconComm.h"
#include "libusb-1.0/libusb.h"

namespace libnifalcon
{
	class FalconCommLibUSB : public FalconComm
	{
	public:
		FalconCommLibUSB();
		~FalconCommLibUSB();
		virtual bool getDeviceCount(int8_t& );
		virtual bool open(uint8_t );
		virtual bool close();
		virtual bool read(uint8_t*, u_int32_t);
		virtual bool write(uint8_t*, u_int32_t);
		virtual bool setFirmwareMode();
		virtual bool setNormalMode();
		static void cb_in(struct libusb_transfer *transfer);
		static void cb_out(struct libusb_transfer *transfer);
		void setHasBytesAvailable(bool v);
		bool initLibUSB();
		void poll();
	protected:
		struct timeval m_tv;
		libusb_device_handle* m_falconDevice;
		unsigned char input[128], output[128];
		struct libusb_transfer* in_transfer;
		struct libusb_transfer* out_transfer;
		struct libusb_pollfd** pollfds;
	};
};

#endif
