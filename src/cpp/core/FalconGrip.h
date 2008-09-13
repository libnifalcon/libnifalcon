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
		FalconGrip(int32_t digital_inputs, int32_t analog_inputs) : m_numDigitalInputs(digital_inputs), m_numAnalogInputs(analog_inputs)
		{
			if(m_numDigitalInputs > 0)
			{
				m_digitalInputs = new bool[m_numDigitalInputs];
			}
			else
			{
				m_digitalInputs = NULL;
			}
			if(m_numAnalogInputs > 0)
			{
				m_analogInputs = new int32_t[m_numAnalogInputs];
			}
			else
			{
				m_analogInputs = NULL;
			}
		}
		virtual ~FalconGrip() {}
		virtual bool runGripLoop(int size, u_int8_t* data) = 0;
		bool getDigitalInput(int index)
		{
			if(index > m_numDigitalInputs) return false;
			return m_digitalInputs[index];
		}
		int32_t getAnalogInput(int index)
		{
			if(index > m_numAnalogInputs) return 0;
			return m_analogInputs[index];
		}
	protected:
		int m_numDigitalInputs;
		int m_numAnalogInputs;
		bool* m_digitalInputs;
		int32_t* m_analogInputs;
	};
}

#endif 
