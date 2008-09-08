#ifndef FALCONGRIPFOURBUTTON_H
#define FALCONGRIPFOURBUTTON_H

#include <cstdlib>
#include "core/FalconGrip.h"

namespace libnifalcon
{
	class FalconGripFourButton : public FalconGrip
	{
		enum
		{
			BUTTON_1 = 0x1,
			BUTTON_2 = 0x2,
			BUTTON_3 = 0x4,
			BUTTON_4 = 0x8
		};

		FalconGripFourButton() : m_numDigitalInputs(4), m_numAnalogInputs(0)
		{
			m_digitalInputs = new bool[m_numDigitalInputs];
			m_analogInputs = NULL;
		}

		~FalconGripFourButton()
		{
		}

		bool runGripLoop(int size, u_int8_t* data)
		{
			//Assuming this to always be 1, since the data can easily be packed into a nibble
			if(size != 1)
			{
				return false;
			}
			for(int i = 0; i < m_numDigitalInputs; ++i)
			{
				m_digitalInputs[i] = data & (1 << i);
			}
			return true;
		}
	};
}


#endif
