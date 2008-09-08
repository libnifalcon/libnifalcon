#ifndef FALCONNOVINTFIRMWARE_H
#define FALCONNOVINTFIRMWARE_H

#include "core/FalconFirmware.h"

namespace libnifalcon
{
	class FalconFirmwareNovintSDK : public FalconFirmware
	{
	public:
		const static u_int8_t RED_LED = 0x2;
		const static u_int8_t GREEN_LED = 0x4;
		const static u_int8_t BLUE_LED = 0x8;

		FalconFirmwareNovintSDK();
		~FalconFirmwareNovintSDK() {}
		bool runIOLoop();
		int32_t getGripInfoSize() { return 1; }
		//Things I am going to hell for, #8926:
		u_int8_t* getGripInfo() { return (u_int8_t*)(m_rawOutput+13); }
	protected:
	
		void formatInput();
		void formatOutput();

		bool m_hasWritten;
		u_int8_t m_rawInput[16], m_rawOutput[16];
	};
}

#endif
