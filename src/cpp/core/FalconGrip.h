#ifndef FALCONGRIP_H
#define FALCONGRIP_H

namespace libnifalcon
{	
	class FalconGrip
	{
	public:

		FalconGrip();
		virtual ~FalconGrip();
		virtual bool runGripLoop(int size, u_int8_t data) = 0;
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
	private:
		int m_numDigitalInputs;
		int m_numAnalogInputs;
		bool* m_digitalInputs;
		int32_t* m_analogInputs;
	};
}

#endif 
