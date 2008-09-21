/***
 * @file FalconNovintHDALCompat.cpp
 * @brief Utility class for compatibility with Novint's HDAL SDK classes
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

#include "FalconNovintHDALCompat.h"

namespace libnifalcon
{

	FalconNovintHDALCompat::FalconNovintHDALCompat() :
		FalconDeviceBoostThread(),
		m_opHandleCount(0),
		m_deviceHandleCount(0)
	{
	}
	
	FalconNovintHDALCompat::~FalconNovintHDALCompat()
	{
	}

	int FalconNovintHDALCompat::addServoOp(HDLServoOp o, void* param)
	{		
		m_servoOps.push_back(HDLServoObj(m_opHandleCount, o, param));
		return m_opHandleCount++;
	}

	bool FalconNovintHDALCompat::removeServoOp(int op_handle)
	{
		for(std::list<HDLServoObj>::iterator itr = m_servoOps.begin(); itr != m_servoOps.end(); ++itr)
		{
			if((*itr).handle == op_handle) (*itr).delete_now = true;
			return true;
		}
		return false;
	}

	void FalconNovintHDALCompat::runThreadLoop()
	{
		while(m_runThreadLoop)
		{
			runIOLoop();
			runServoOps();
		}
	}

	void FalconNovintHDALCompat::runServoOps()
	{
		for(std::list<HDLServoObj>::iterator itr = m_servoOps.begin(); itr != m_servoOps.end(); ++itr)
		{
			if(!(*itr).delete_now && !(*itr).func((*itr).param))
			{
				(*itr).delete_now = true;
			}
		}
		m_servoOps.remove_if(isOpDeleteable());
	}
}
