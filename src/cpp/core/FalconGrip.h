/***
 * @file FalconGrip.h
 * @brief Base class for grip definition classes
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
#ifndef FALCONGRIP_H
#define FALCONGRIP_H

#include <cstdlib>

namespace libnifalcon
{	
	class FalconGrip : public FalconCore
	{
	public:

		enum {
			FALCON_GRIP_INDEX_OUT_OF_RANGE = 4000
		};
		FalconGrip(int32_t digital_inputs, int32_t analog_inputs) : m_numDigitalInputs(digital_inputs), m_numAnalogInputs(analog_inputs), m_digitalInputs(0)
		{
		}
		virtual ~FalconGrip()
		{
		}
		virtual bool runGripLoop(int size, u_int8_t* data) = 0;
		int32_t getNumDigitalInputs() const { return m_numDigitalInputs; }
		int32_t getNumAnalogInputs() const { return m_numAnalogInputs; }
		bool getDigitalInput(int index) const
		{
			if(index > m_numDigitalInputs) return false;
			return m_digitalInputs & (1 << index);
		}
		int32_t getDigitalInputs() const { return m_digitalInputs; }
		int32_t getAnalogInput(int index) const
		{
			if(index > m_numAnalogInputs) return 0;
			return m_analogInputs[index];
		}
	protected:
		int m_numDigitalInputs;
		int m_numAnalogInputs;
		//I think assuming 32 digital inputs and 128 analog is enough
		int32_t m_digitalInputs;
		int32_t m_analogInputs[128];
	};
}

#endif 
