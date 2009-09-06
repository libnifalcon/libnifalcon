/***
 * @file FalconCommLibFTDI.h
 * @brief LibFTDI (http://www.intra2net.com/de/produkte/opensource/ftdi/) based implementation of FTDI communication for the falcon
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONLIBFTDICOMM_H
#define FALCONLIBFTDICOMM_H

#include "falcon/core/FalconLogger.h"
#include "falcon/core/FalconComm.h"

struct ftdi_context;

namespace libnifalcon
{
	class FalconCommLibFTDI : public FalconComm
	{
	public:
		FalconCommLibFTDI();
		~FalconCommLibFTDI();
		virtual bool getDeviceCount(unsigned int& );
		virtual bool open(unsigned int );
		virtual bool close();
		virtual bool read(uint8_t*, unsigned int);
		virtual bool write(uint8_t*, unsigned int);
		virtual bool readBlocking(uint8_t* size, unsigned int buffer) { return read(size, buffer); }
		virtual bool writeBlocking(uint8_t* size, unsigned int buffer) { return write(size, buffer); }
		virtual bool setFirmwareMode();
		virtual bool setNormalMode();
	protected:
		bool initLibFTDI();
		ftdi_context* m_falconDevice;
		bool m_isInitialized;
		DECLARE_LOGGER();

	};
};

#endif
