#include "FalconSphereTest.h"

#include "falcon/core/FalconDevice.h"
#include "falcon/grip/FalconGripFourButton.h"
#include "falcon/kinematic/FalconKinematicStamper.h"

FalconSphereTest::FalconSphereTest(boost::shared_ptr<libnifalcon::FalconDevice> d) :
	FalconTestBase(d),
	m_radius(0.028),
	m_stiffness(1000.0),
	m_isInitializing(true),
	m_hasPrintedInitMsg(false),
	m_plusButtonDown(false),
	m_minusButtonDown(false),
	m_centerButtonDown(false),
	m_forwardButtonDown(false)
{
	setPrintOnCount(1000);
	m_falconDevice->setFalconKinematic<libnifalcon::FalconKinematicStamper>();
	m_falconDevice->setFalconGrip<libnifalcon::FalconGripFourButton>();
	m_oldpos[0] = m_oldpos[1] = m_oldpos[2] = 0.0;
}

void FalconSphereTest::runFunction()
{
	if(!m_falconDevice->runIOLoop())
		return;

	boost::array<double, 3> pos = m_falconDevice->getPosition();

	if(m_isInitializing)
	{
		if(!m_hasPrintedInitMsg)
		{
			std::cout << "Move the end effector all the way out" << std::endl;
			m_hasPrintedInitMsg = true;
		}
		if(pos[2] > .170)
		{
			std::cout << "Starting sphere simulation..." << std::endl;
			m_isInitializing = false;
			tstart();
		} else {
			m_oldpos = pos;
		}
		m_lastLoopCount = m_falconDevice->getFalconFirmware()->getLoopCount();
		return;
	}

	// increase radius
	if(m_falconDevice->getFalconGrip()->getDigitalInputs() 
	   & libnifalcon::FalconGripFourButton::PLUS_BUTTON)
	{
		m_plusButtonDown = true;
	}
	else if(m_plusButtonDown)
	{
		m_plusButtonDown = false;
		m_radius += 0.002;
		std::cout << "Plus button pressed... radius now:" << m_radius << std::endl;
		return;
	}

	// decrease radius
	if(m_falconDevice->getFalconGrip()->getDigitalInputs() 
	   & libnifalcon::FalconGripFourButton::MINUS_BUTTON)
	{
		m_minusButtonDown = true;
	}
	else if(m_minusButtonDown)
	{
		m_minusButtonDown = false;
		m_radius -= 0.002;
		std::cout << "Minus button pressed... radius now:" << m_radius << std::endl;
		return;
	}

	if(m_falconDevice->getFalconGrip()->getDigitalInputs() 
	   & libnifalcon::FalconGripFourButton::FORWARD_BUTTON)
	{
		m_forwardButtonDown = true;
	} else{
		m_forwardButtonDown = false;
	}

	// make sphere soft radius or "slippery"
	if(m_falconDevice->getFalconGrip()->getDigitalInputs() 
	   & libnifalcon::FalconGripFourButton::CENTER_BUTTON)
	{
		if(m_forwardButtonDown)
		{
			m_stiffness = -300.0;
                } 
                else 
                {
			m_stiffness = 1000.0;
                }
	} else{
                if(m_forwardButtonDown)
                {
			m_stiffness = 500.0;
		} 
		else 
		{
			m_stiffness = 100.0;
		}
	}


	boost::array<double, 3> force;
	force[0] = 0.0;
	force[1] = 0.0;
	force[2] = 0.0;
	
	// offset z 
	pos[2] -= 0.11;
	
	double dist = sqrt(pos[0]*pos[0]+pos[1]*pos[1]+pos[2]*pos[2]);

	if (dist < m_radius) {
		if (m_forwardButtonDown) {
			force[0] = (m_oldpos[0] - pos[0]) * m_stiffness * 10.0;
			force[1] = (m_oldpos[1] - pos[1]) * m_stiffness * 10.0;
			force[2] = (m_oldpos[2] - pos[2]) * m_stiffness * 10.0;
		} else {
			force[0] = (pos[0] / dist) * (m_radius - dist) * m_stiffness;
			force[1] = (pos[1] / dist) * (m_radius - dist) * m_stiffness;
			force[2] = (pos[2] / dist) * (m_radius - dist) * m_stiffness;
		} 
	}
        // dampen position update to remove noise.
	m_oldpos[0] = 0.4*m_oldpos[0]+ 0.6*pos[0];
	m_oldpos[1] = 0.4*m_oldpos[1]+ 0.6*pos[1];
	m_oldpos[2] = 0.4*m_oldpos[2]+ 0.6*pos[2];
	m_falconDevice->setForce(force);
}

