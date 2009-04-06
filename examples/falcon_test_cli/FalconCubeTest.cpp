#include "FalconCubeTest.h"

#include "falcon/core/FalconDevice.h"
#include "falcon/kinematic/FalconKinematicStamper.h"

FalconCubeTest::FalconCubeTest(boost::shared_ptr<libnifalcon::FalconDevice> d) :
	FalconTestBase(d),
	m_cornerA(-.030, -.030, .095),
	m_cornerB(.030, .030, .155 ),
	m_stiffness(1000),
	m_isInitializing(true),
	m_hasPrintedInitMsg(false)
{
	setPrintOnCount(1000);
	m_falconDevice->setFalconKinematic<libnifalcon::FalconKinematicStamper>();
}

void FalconCubeTest::runFunction()
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
			std::cout << "Starting cube simulation..." << std::endl;
			m_isInitializing = false;
			tstart();
		}
		m_lastLoopCount = m_falconDevice->getFalconFirmware()->getLoopCount();
		return;
	}

	boost::array<double, 3> force;

	double dist = 10000;
	int closest = -1, outside=3, axis;

	// For each axis, check if the end effector is inside
	// the cube.  Record the distance to the closest wall.

	for (axis=0; axis<3; axis++)
	{
		force[axis] = 0;
		if (pos[axis] > m_cornerA[axis] && pos[axis] < m_cornerB[axis])
		{
			double dA = pos[axis]-m_cornerA[axis];
			double dB = pos[axis]-m_cornerB[axis];
			if (fabs(dA) < fabs(dist)) { dist = dA; closest = axis; }
			if (fabs(dB) < fabs(dist)) { dist = dB; closest = axis; }
			outside--;
		}
	}

	// If so, add a proportional force to kick it back
	// outside from the nearest wall.

	if (closest > -1 && !outside)
		force[closest] = -m_stiffness*dist;
	m_falconDevice->setForce(force);
}

