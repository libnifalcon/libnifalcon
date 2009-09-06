/***
 * @file FalconCommFTD2XX.h
 * @brief FTD2XX (http://www.ftdichip.com) based implementation of FTDI communication for the falcon
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
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
		FalconCommFTD2XX() { }
		virtual ~FalconCommFTD2XX();
		virtual bool getDeviceCount(unsigned int& );
		virtual void poll();
		virtual bool open(unsigned int );
		virtual bool close();
		virtual bool read(uint8_t*, unsigned int);
		virtual bool write(uint8_t*, unsigned int);
		virtual bool readBlocking(uint8_t*, unsigned int); //{ return read(size, buffer); }
		virtual bool writeBlocking(uint8_t* buffer, unsigned int size) { return write(buffer, size); }
		virtual bool setFirmwareMode();
		virtual bool setNormalMode();
	protected:
		const static char* FALCON_DESCRIPTION;
		int8_t openDeviceFTD2XX(unsigned int , bool );
		//This would usually be a FT_HANDLE, but FT_HANDLE
		//is just a void*, so this saves us having to deal
		//with the include at this level.
		void* m_falconDevice;
	};
};

#endif
