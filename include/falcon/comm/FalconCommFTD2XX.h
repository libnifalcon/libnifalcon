/***
 * @file FalconCommFTD2XX.h
 * @brief FTD2XX (http://www.ftdichip.com) based implementation of FTDI communication for the falcon
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

#ifndef FALCONFTD2XXCOMM_H
#define FALCONFTD2XXCOMM_H

#include "falcon/core/FalconComm.h"
#include "ftd2xx.h"

namespace libnifalcon
{
	class FalconCommFTD2XX : public FalconComm
	{
	public:
		FalconCommFTD2XX() {}
		virtual ~FalconCommFTD2XX();
		virtual bool getDeviceCount(int8_t& );
		virtual bool open(uint8_t );
		virtual bool close();
		virtual bool read(uint8_t*, uint32_t);
		virtual bool write(uint8_t*, uint32_t);
		virtual bool setFirmwareMode();
		virtual bool setNormalMode();
	protected:
		const static char* FALCON_DESCRIPTION;
		int8_t openDeviceFTD2XX(uint8_t , bool );
		FT_HANDLE m_falconDevice;
	};
};

#endif
