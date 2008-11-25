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

namespace libnifalcon
{
	class FalconCommFTD2XX : public FalconComm
	{
	public:
		FalconCommFTD2XX() { m_requiresPoll = true; }
		virtual ~FalconCommFTD2XX();
		virtual bool getDeviceCount(int8_t& );
		virtual void poll();
		virtual bool open(uint8_t );
		virtual bool close();
		virtual bool read(uint8_t*, uint32_t);
		virtual bool write(uint8_t*, uint32_t);
		virtual bool setFirmwareMode();
		virtual bool setNormalMode();
	protected:
		const static char* FALCON_DESCRIPTION;
		int8_t openDeviceFTD2XX(uint8_t , bool );
		//This would usually be a FT_HANDLE, but FT_HANDLE
		//is just a void*, so this saves us having to deal
		//with the include at this level.
		void* m_falconDevice;
	};
};

#endif
