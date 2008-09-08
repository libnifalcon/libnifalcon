#ifndef FALCONGRIPFOURBUTTON_H
#define FALCONGRIPFOURBUTTON_H

#include "core/FalconGrip.h"

namespace libnifalcon
{
	class FalconGripFourButton : public FalconGrip
	{
	public:		
		enum
		{
			BUTTON_1 = 0x1,
			BUTTON_2 = 0x2,
			BUTTON_3 = 0x4,
			BUTTON_4 = 0x8
		};

		FalconGripFourButton() : FalconGrip(4,0)
		{
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
				m_digitalInputs[i] = data[0] & (1 << i);
			}
			return true;
		}
	};
}


#endif
