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
