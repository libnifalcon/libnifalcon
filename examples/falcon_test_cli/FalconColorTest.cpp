
#include "falcon/kinematic/FalconKinematicStamper.h"
#include "FalconColorTest.h"

FalconColorTest::FalconColorTest(boost::shared_ptr<libnifalcon::FalconDevice> d) :
	FalconTestBase(d),
	m_tickLimit(20)
{
	m_posLimit[0] = 0.060;
	m_posLimit[1] = 0.060;
	m_posLimit[2] = 0.060;
	for(int i = 0; i < 3; ++i)
	{
		m_ledState[i] = 0;
		m_ledLevel[i] = 0;
		m_ledCount[i] = 0;
	}
	m_falconDevice->setFalconKinematic<libnifalcon::FalconKinematicStamper>();
}

FalconColorTest::~FalconColorTest()
{
}

void FalconColorTest::runFunction()
{
	if(!m_falconDevice->runIOLoop())
		return;

	boost::array<double, 3> pos = m_falconDevice->getPosition();

	for(int i = 0; i < 3; ++i)
	{
		if(i == 2) pos[2] -= .125;
		if(m_ledCount[i] <= 0)
		{
			float calc = (m_posLimit[i] - fabs(pos[i])) / m_posLimit[i];
			if(calc > 1.0) calc = 1.0;
			if(m_ledState[i])
			{
				m_ledCount[i] = m_tickLimit - (calc * m_tickLimit);
			}
			else
			{
				m_ledCount[i] = (calc * m_tickLimit);
			}
			if(m_ledCount[i] < 0) m_ledCount[i] = -m_ledCount[i];
			m_ledState[i] = !m_ledState[i];
		}
		else
		{
			--m_ledCount[i];
		}
	}

	uint8_t ledCommand =
		(m_ledState[0] > 0 ? libnifalcon::FalconFirmware::RED_LED : 0) |
		(m_ledState[1] > 0 ? libnifalcon::FalconFirmware::GREEN_LED : 0) |
		(m_ledState[2] > 0 ? libnifalcon::FalconFirmware::BLUE_LED : 0);
	m_falconDevice->getFalconFirmware()->setLEDStatus(ledCommand);
}
