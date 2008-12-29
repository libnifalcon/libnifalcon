/***
 * @file FalconCore.h
 * @brief Base class for all C++ libnifalcon classes
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

#ifndef FALCONCORE_H
#define FALCONCORE_H

#include "falcon/core/FalconLogger.h"

namespace libnifalcon
{
	class FalconCore
	{
	public:
		/** 
		 * Constructor for the core falcon objects (Device, Communications, Firmware, Kinematics, Grip)
		 *
		 * All falcon objects inherit from this class.
		 */
		FalconCore() : m_errorCode(0) {}
		/** 
		 * Deconstructor for the core falcon object
		 *
		 */
		virtual ~FalconCore() {}
		/** 
		 * Returns the error code for the object. Error codes are listed per class
		 * 
		 * @return Error code for the object
		 */
		int getErrorCode() { return m_errorCode; }
	protected:
		int m_errorCode; /**< Error code for the object */
	};
}
#endif
