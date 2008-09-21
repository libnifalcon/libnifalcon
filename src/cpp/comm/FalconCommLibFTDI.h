/***
 * @file FalconCommLibFTDI.h
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
#ifndef FALCONLIBFTDICOMM_H
#define FALCONLIBFTDICOMM_H

#include "core/FalconComm.h"
#include "ftdi.h"

namespace libnifalcon
{
	class FalconCommLibFTDI : public FalconComm
	{
	public:
		FalconCommLibFTDI();
		~FalconCommLibFTDI();
		virtual bool getDeviceCount(int8_t& );
		virtual bool open(u_int8_t );
		virtual bool close();
		virtual bool read(u_int8_t*, u_int32_t);
		virtual bool write(u_int8_t*, u_int32_t);
		virtual bool setFirmwareMode();
		virtual bool setNormalMode();
	protected:
//		int8_t openDeviceFTD2XX(u_int8_t , bool );
		bool initLibFTDI();
		ftdi_context m_falconDevice;
		bool m_isInitialized;
	};
};

#endif
