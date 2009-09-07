/***
 * @file FalconCore.h
 * @brief Base class for all C++ libnifalcon classes
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONCORE_H
#define FALCONCORE_H

#include "falcon/core/FalconLogger.h"

namespace libnifalcon
{
/**
 * @class FalconCore
 * @ingroup CoreClasses
 *
 * FalconCore is a simple base class that all libnifalcon objects derive from. It provides a common error
 * handling interface to child objects. 
 */
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
