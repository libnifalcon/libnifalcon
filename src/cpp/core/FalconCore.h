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

namespace libnifalcon
{
	class FalconCore
	{
	public:
		FalconCore() : m_errorCode(0)
		{
		}
		virtual ~FalconCore() {}		
		int getErrorCode() { return m_errorCode; }
	protected:
		int m_errorCode;
	};
}
#endif
