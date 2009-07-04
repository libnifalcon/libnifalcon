#include "FalconWallTest.h"

#include "falcon/core/FalconDevice.h"
#include "falcon/grip/FalconGripFourButton.h"
#include "falcon/kinematic/FalconKinematicStamper.h"

FalconWallTest::FalconWallTest(boost::shared_ptr<libnifalcon::FalconDevice> d, unsigned int axis) :
	FalconTestBase(d),
	m_axisBounds(0, 0, .130),
	m_stiffness(1000),
	m_isInitializing(true),
	m_hasPrintedInitMsg(false),
	m_axis(axis),
	m_positiveForce(true),
	m_runClickCount(0),
	m_buttonDown(false)
{
	m_falconDevice->setFalconKinematic<libnifalcon::FalconKinematicStamper>();
	m_falconDevice->setFalconGrip<libnifalcon::FalconGripFourButton>();
}

void FalconWallTest::runFunction()
{
	if(!m_falconDevice->runIOLoop())
		return;

	boost::array<double, 3> pos = m_falconDevice->getPosition();

	if(m_isInitializing)
	{
		if(!m_hasPrintedInitMsg)
		{
			std::cout << "Move the end effector out of the wall area" << std::endl;
			m_hasPrintedInitMsg = true;
		}
		if(((pos[m_axis] > m_axisBounds[m_axis]) && m_positiveForce) || ((pos[m_axis] < m_axisBounds[m_axis]) && !m_positiveForce))
		{
			std::cout << "Starting wall simulation... Press center button (circle button) to change direction of force..." << std::endl;
			m_isInitializing = false;
			tstart();
		}
		return;
	}

	//Cheap debounce
	if(m_falconDevice->getFalconGrip()->getDigitalInputs() & libnifalcon::FalconGripFourButton::BUTTON_3)
	{
		m_buttonDown = true;
	}
	else if(m_buttonDown)
	{
		m_buttonDown = false;
		std::cout << "Flipping force vector..." << std::endl;
		m_runClickCount = m_lastLoopCount;
		m_positiveForce = !m_positiveForce;
		m_hasPrintedInitMsg = false;
		m_isInitializing = true;
		return;
	}

	boost::array<double, 3> force = {0,0,0};

	double dist = 10000;
	int closest = -1, outside=3;

	// For each axis, check if the end effector is inside
	// the cube.  Record the distance to the closest wall.

	force[m_axis] = 0;
	if(((pos[m_axis] < m_axisBounds[m_axis]) && m_positiveForce) || ((pos[m_axis] > m_axisBounds[m_axis]) && !m_positiveForce))
	{
		double dA = pos[m_axis]-m_axisBounds[m_axis];
		dist = dA;
		closest = m_axis;
	}

	// If so, add a proportional force to kick it back
	// outside from the nearest wall.

	if (closest > -1)
		force[closest] = -m_stiffness*dist;
	m_falconDevice->setForce(force);

}

