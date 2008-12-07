/***
 * @file PositionMatrix.cpp
 * @brief Lookup table generator for discerning positions from angles (workaround to avoid calculating the 32 degree polynomial from Stamper paper)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net)
 * @version $Id$
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL$
 * 
 * Project info at http://libnifalcon.sourceforge.net/ 
 *
 */
#include "falcon/kinematic/stamper/PositionMatrix.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		PositionMatrix::PositionMatrix(const double (&center)[3], const float& range, const uint32_t density) :
			m_matrixRange(range),
			m_matrixDensity(density),
			m_matrixDelta(range/(float)density)
		{
			for(int i = 0; i < 3; ++i)
				m_matrixCenter[i] = center[i];
		}

		PositionMatrix::~PositionMatrix()
		{
		}

		void PositionMatrix::populate(AngularMatrix& angularMatrix)
		{
			for (int i=-(m_matrixDensity/2); i<=(m_matrixDensity/2); i++)
				for (int j=-(m_matrixDensity/2); j<=(m_matrixDensity/2); j++)
					for (int k=-(m_matrixDensity/2); k<=(m_matrixDensity/2); k++)
					{
						angularMatrix.setPosition(gmtl::Vec3f(m_matrixCenter[0]+(i*m_matrixDelta),m_matrixCenter[1]+(j*m_matrixDelta),m_matrixCenter[2]+(k*m_matrixDelta)));
					}
		}

		float PositionMatrix::getRange()
		{
			return m_matrixRange;
		}

	}
}
